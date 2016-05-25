/*
 * HttpConn.h
 *
 *  Created on: 2016年5月22日
 *      Author: hhq163
 */

#ifndef SRC_HTTP_SVR_HTTPCONN_H_
#define SRC_HTTP_SVR_HTTPCONN_H_

#include "NetLib.h"
#include "Util.h"
#include "Common.h"
#include "HttpParserWrapper.h"
#include "ImConn.h"

typedef struct {
	uint32_t		connUuid;
	string		strContent;
} ResponseT;





class CHttpConn : public CImConn
{
public:
    CHttpConn();
    virtual ~CHttpConn();

    uint32_t GetConnHandle() { return m_uuid; }
    char* GetPeerIP() { return (char*)m_peerIp.c_str(); }

    int Send(void* data, int len);

    void Close();
    void OnConnect(net_handle_t handle);
    void OnRead();
    void OnWrite();
    void OnClose();
    void OnTimer(uint64_t curr_tick);

    virtual void OnWriteCompelete();
    static void AddResponse(uint32_t connUuid, string strContent);	// 工作线程调用
    static void SendResponseList();	// 主线程调用

private:
   bool ParamVerify(string url);
protected:
    net_handle_t    m_sockHandle;
    uint32_t        m_uuid;
    bool            m_busy;

    uint32_t        m_state;
    std::string     m_peerIp;
    uint16_t        m_peerPort;
    CSimpleBuffer   m_inBuf;
    CSimpleBuffer   m_outBuf;

    uint64_t        m_lastSendTick;
    uint64_t        m_lastRecvTick;

    CHttpParserWrapper* m_HttpParser;
private:
    static CLock			g_listLock;
    static list<ResponseT*>	g_responseList;	// 主线程发送回复消息
};

typedef hash_map<net_handle_t, CHttpConn*> HttpConnMap;
typedef hash_map<uint32_t, CHttpConn*> UserMap;

CHttpConn* GetHttpConnByUuid(uint32_t uuid);
int InitHttpConn(uint32_t thread_num);

#endif /* SRC_HTTP_SVR_HTTPCONN_H_ */
