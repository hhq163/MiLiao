/*
 * HttpConn.cpp
 *
 *  Created on: 2016年5月22日
 *      Author: hhq163
 */

#include "OsType.h"
#include "Common.h"
#include "Log.h"
#include "RouteConfig.h"
#include "HttpTask.h"
#include "ThreadPool.h"
#include "HttpConn.h"
#include "HttpParserWrapper.h"

static HttpConnMap 	g_httpConnMap;//短连接存储结构
static UserMap g_uuidConnMap;		//短连接存储结构，结构和上面不一样
static CRouteConfig* g_routeConfig;//路由配置；

static CThreadPool	 g_threadPool;//工作线程池


CLock CHttpConn::g_listLock;
list<ResponseT*> CHttpConn::g_responseList;//响应队列

// uuid 从0开始递增，可以防止因socket handle重用引起的一些冲突
static uint32_t g_uuidAlloctor = 0;

void HttpConnCallback(void* callback_data, uint8_t msg, uint32_t handle, uint32_t uParam, void* pParam)
{
    NOTUSED_ARG(uParam);
    NOTUSED_ARG(pParam);

    // convert void* to uint32_t, oops
    uint32_t uuid = *((uint32_t*)(&callback_data));
    CHttpConn* pConn = GetHttpConnByUuid(uuid);
    if (!pConn) {
        return;
    }

    switch (msg){
    case NETLIB_MSG_READ:
        pConn->OnRead();
        break;
    case NETLIB_MSG_WRITE:
        pConn->OnWrite();
        break;
    case NETLIB_MSG_CLOSE:
        pConn->OnClose();
        break;
    default:
        Logger.Log(INFO, "!!!HttpConnCallback error msg: %d ", msg);
        break;
    }
}

void HttpConnTimerCallback(void* callback_data, uint8_t msg, uint32_t handle, void* pParam)
{
    CHttpConn* pConn = NULL;
    HttpConnMap::iterator it, it_old;
    uint64_t curTime = get_tick_count();

    for (it = g_httpConnMap.begin(); it != g_httpConnMap.end(); ) {
        it_old = it;
        it++;

        pConn = it_old->second;
        pConn->OnTimer(curTime);
    }
}

void HttpLoopCallback(void* callback_data, uint8_t msg, uint32_t handle, void* pParam)
{
	CHttpConn::SendResponseList();
}

CHttpConn* GetHttpConnByUuid(uint32_t uuid)
{
	CHttpConn* pConn = NULL;
	UserMap::iterator it = g_uuidConnMap.find(uuid);
	if (it != g_uuidConnMap.end()) {
		pConn = (CHttpConn *)it->second;
	}

	return pConn;
}

/*
 * 用于优雅的关闭连接：
 * 服务器收到SIGTERM信号后，发送CImPduStopReceivePacket数据包给每个连接，
 * 通知消息服务器不要往自己发送数据包请求，
 * 然后注册4s后调用的回调函数，回调时再退出进程
 */
void ExitCallback(void* callback_data, uint8_t msg, uint32_t handle, void* pParam)
{
	Logger.Log(ERROR, "exit_callback");
	exit(0);
}

static void SigHandler(int sig_no)
{
	if (sig_no == SIGTERM) {
		Logger.Log(ERROR, "receive SIGTERM, prepare for exit");

        // callback after 4 second to exit process;
		netlib_register_timer(ExitCallback, NULL, 4000);
	}
}


/**
 * 初始化线程池及命令字与处理方法的映射关系
 */
int InitHttpConn(uint32_t thread_num)
{
	g_routeConfig = CRouteConfig::getInstance();
	g_threadPool.Init(thread_num);

	netlib_add_loop(HttpLoopCallback, NULL);

	signal(SIGTERM, SigHandler);

	return netlib_register_timer(HttpConnTimerCallback, NULL, 1000);
}

//////////////////////////
CHttpConn::CHttpConn()
{
    m_busy = false;
    m_sockHandle = NETLIB_INVALID_HANDLE;
    m_state = CONN_STATE_IDLE;

    m_lastSendTick = m_lastRecvTick = get_tick_count();

    m_uuid = ++g_uuidAlloctor;
    if (m_uuid == 0) {
        m_uuid = ++g_uuidAlloctor;
    }
    g_uuidConnMap.insert(make_pair(m_uuid, this));

    m_HttpParser = new CHttpParserWrapper();
    Logger.Log(INFO, "CHttpConn, handle=%u ", m_uuid);
}

CHttpConn::~CHttpConn()
{
    Logger.Log(INFO, "~CHttpConn, handle=%u ", m_uuid);
}

int CHttpConn::Send(void* data, int len)
{
    m_lastSendTick = get_tick_count();

    if (m_busy){
        m_outBuf.Write(data, len);
        return len;
    }

    int ret = netlib_send(m_sockHandle, data, len);
    if (ret < 0)
        ret = 0;

    if (ret < len){
        m_outBuf.Write((char*)data + ret, len - ret);
        m_busy = true;
        Logger.Log(INFO, "not send all, remain=%d ", m_outBuf.GetWriteOffset());
    }else{
        OnWriteCompelete();
    }

    return len;
}

void CHttpConn::Close()
{
    if (m_state != CONN_STATE_CLOSED) {
        m_state = CONN_STATE_CLOSED;

        g_httpConnMap.erase(m_sockHandle);
        g_uuidConnMap.erase(m_uuid);
        netlib_close(m_sockHandle);

        ReleaseRef();
    }
}

void CHttpConn::OnConnect(net_handle_t handle)
{
    Logger.Log(INFO, "OnConnect, handle=%d\n", handle);
    m_sockHandle = handle;
    m_state = CONN_STATE_CONNECTED;
    g_httpConnMap.insert(make_pair(handle, this));

    netlib_option(handle, NETLIB_OPT_SET_CALLBACK, (void*)HttpConnCallback);
    netlib_option(handle, NETLIB_OPT_SET_CALLBACK_DATA, reinterpret_cast<void *>(m_uuid) );
    netlib_option(handle, NETLIB_OPT_GET_REMOTE_IP, (void*)&m_peerIp);
}

void CHttpConn::OnRead()
{
    for (;;){
        uint32_t free_buf_len = m_inBuf.GetAllocSize() - m_inBuf.GetWriteOffset();
        if (free_buf_len < READ_BUF_SIZE + 1){
            m_inBuf.Extend(READ_BUF_SIZE + 1);
        }

        int ret = netlib_recv(m_sockHandle, m_inBuf.GetBuffer() + m_inBuf.GetWriteOffset(), READ_BUF_SIZE);
        if (ret <= 0)
            break;

        m_inBuf.IncWriteOffset(ret);

        m_lastRecvTick = get_tick_count();
    }

    // 每次请求对应一个HTTP连接，所以读完数据后，不用在同一个连接里面准备读取下个请求
    char* in_buf = (char*)m_inBuf.GetBuffer();
    uint32_t buf_len = m_inBuf.GetWriteOffset();
    in_buf[buf_len] = '\0';

    Logger.Log(INFO, "OnRead, buf_len=%u, uuid=%u content:%s\n", buf_len, m_uuid,in_buf);

    m_HttpParser->ParseHttpContent(in_buf, buf_len);

    if (m_HttpParser->IsReadAll()) {
        string url =  m_HttpParser->GetUrl();
        //参数安全性校验
//        if(!ParamVerify(url)){
//        		Logger.Log(ERROR, "url param is not safe! url=%s", url.c_str());
//        }

        ActionHandlerT actHandler = g_routeConfig->GetAction(url);

	   if (actHandler) {
		   CTask* pTask = new CHttpTask(m_uuid, actHandler, m_HttpParser);
		   g_threadPool.AddTask(pTask);
	   } else {
		   Logger.Log(ERROR, "no handler for request: %s", url.c_str());
		   string strResponse = "服务端未找到对应的控制器";
		   Send((void*)strResponse.c_str(), strResponse.length());
	   }

    }
}

void CHttpConn::OnWrite()
{
    if (!m_busy)
        return;

    int ret = netlib_send(m_sockHandle, m_outBuf.GetBuffer(), m_outBuf.GetWriteOffset());
    if (ret < 0)
        ret = 0;

    int out_buf_size = (int)m_outBuf.GetWriteOffset();

    m_outBuf.Read(NULL, ret);

    if (ret < out_buf_size){
        m_busy = true;
        Logger.Log(INFO, "not send all, remain=%d ", m_outBuf.GetWriteOffset());
    }else{
        m_busy = false;
        OnWriteCompelete();
    }
}

void CHttpConn::OnClose()
{
    Close();
}

void CHttpConn::OnTimer(uint64_t curr_tick)
{
    if (curr_tick > m_lastRecvTick + HTTP_CONN_TIMEOUT) {
        Logger.Log(INFO, "HttpConn timeout, handle=%d ", m_uuid);
        Close();
    }
}

void CHttpConn::OnWriteCompelete()
{
    Close();
}

void CHttpConn::AddResponse(uint32_t connUuid, string strContent)
{
	ResponseT* pResp = new ResponseT;
	pResp->connUuid = connUuid;
	pResp->strContent = strContent;

	g_listLock.lock();
	g_responseList.push_back(pResp);
	g_listLock.unlock();
}

void CHttpConn::SendResponseList()
{
	g_listLock.lock();
	while (!g_responseList.empty()) {
		ResponseT* pResp = g_responseList.front();
		g_responseList.pop_front();
		g_listLock.unlock();

		CHttpConn* pConn = GetHttpConnByUuid(pResp->connUuid);
		if (pConn) {
			if (pResp->strContent.length() > 0) {
				pConn->Send((void*)pResp->strContent.c_str(), pResp->strContent.length());
			} else {
				Logger.Log(ERROR, "close connection uuid=%d by parse string error\b", pResp->connUuid);
				pConn->Close();
			}
		}

//		if (pResp->strContent)
//			delete pResp->strContent;
		delete pResp;

		g_listLock.lock();
	}

	g_listLock.unlock();
}

/**
 * 参数是否安全
 */
bool CHttpConn::ParamVerify(string url)
{
	bool ret = false;
	if(url.compare( "*" ) == 0 ){//此处需要改进
		ret = true;
	}

	return ret;
}
