/*
 * HttpConn.cpp
 *
 *  Created on: 2016年5月22日
 *      Author: hhq163
 */

#include "Common.h"
#include "Log.h"
#include "HttpConn.h"
#include "HttpParserWrapper.h"
#include "HttpQuery.h"

static HttpConnMap g_httpConnMap;

// conn_handle 从0开始递增，可以防止因socket handle重用引起的一些冲突
static uint32_t g_connHandleGenerator = 0;

CHttpConn* FindHttpConnByHandle(uint32_t conn_handle)
{
    CHttpConn* pConn = NULL;
    HttpConnMap::iterator it = m_httpConnMap.find(conn_handle);
    if (it != m_httpConnMap.end()){
        pConn = it->second;
    }

    return pConn;
}

void httpconn_callback(void* callback_data, uint8_t msg, uint32_t handle, uint32_t uParam, void* pParam)
{
    NOTUSED_ARG(uParam);
    NOTUSED_ARG(pParam);

    // convert void* to uint32_t, oops
    uint32_t conn_handle = *((uint32_t*)(&callback_data));
    CHttpConn* pConn = FindHttpConnByHandle(conn_handle);
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
        Logger.Log(INFO, "!!!httpconn_callback error msg: %d ", msg);
        break;
    }
}

void http_conn_timer_callback(void* callback_data, uint8_t msg, uint32_t handle, void* pParam)
{
    CHttpConn* pConn = NULL;
    HttpConnMap::iterator it, it_old;
    uint64_t curTime = get_tick_count();

    for (it = m_httpConnMap.begin(); it != m_httpConnMap.end(); ) {
        it_old = it;
        it++;

        pConn = it_old->second;
        pConn->OnTimer(curTime);
    }
}

void init_http_conn()
{
    netlib_register_timer(http_conn_timer_callback, NULL, 1000);
}

//////////////////////////
CHttpConn::CHttpConn()
{
    m_busy = false;
    m_sockHandle = NETLIB_INVALID_HANDLE;
    m_state = CONN_STATE_IDLE;

    m_lastSendTick = m_lastRecvTick = get_tick_count();
    m_connHandle = ++g_connHandleGenerator;
    if (m_connHandle == 0) {
        m_connHandle = ++g_connHandleGenerator;
    }

    Logger.Log(INFO, "CHttpConn, handle=%u ", m_connHandle);
}

CHttpConn::~CHttpConn()
{
    Logger.Log(INFO, "~CHttpConn, handle=%u ", m_connHandle);
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

        m_httpConnMap.erase(m_connHandle);
        netlib_close(m_sockHandle);

        ReleaseRef();
    }
}

void CHttpConn::OnConnect(net_handle_t handle)
{
    printf("OnConnect, handle=%d\n", handle);
    m_sockHandle = handle;
    m_state = CONN_STATE_CONNECTED;
    m_httpConnMap.insert(make_pair(m_connHandle, this));

    netlib_option(handle, NETLIB_OPT_SET_CALLBACK, (void*)httpconn_callback);
    netlib_option(handle, NETLIB_OPT_SET_CALLBACK_DATA, reinterpret_cast<void *>(m_connHandle) );
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

    Logger.Log(INFO, "OnRead, buf_len=%u, conn_handle=%u content:%s\n", buf_len, m_connHandle,in_buf);

    m_HttpParser.ParseHttpContent(in_buf, buf_len);

    if (m_HttpParser.IsReadAll()) {
        string url =  m_HttpParser.GetUrl();
        if (strncmp(url.c_str(), "/query/", 7) == 0) {
            string content = m_HttpParser.GetBodyContent();
            CHttpQuery* pQueryInstance = CHttpQuery::GetInstance();
            pQueryInstance->DispatchQuery(url, content, this);
            Logger.Log(INFO, "++Query!!!++\n");
        } else {
            Logger.Log(INFO, "url unknown, url=%s ", url.c_str());
            Close();
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
        Logger.Log(INFO, "HttpConn timeout, handle=%d ", m_connHandle);
        Close();
    }
}

void CHttpConn::OnWriteCompelete()
{
    Close();
}

