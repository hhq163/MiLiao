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

class CHttpConn : public CRefObject
{
public:
    CHttpConn();
    virtual ~CHttpConn();

    uint32_t GetConnHandle() { return m_connHandle; }
    char* GetPeerIP() { return (char*)m_peerIp.c_str(); }

    int Send(void* data, int len);

    void Close();
    void OnConnect(net_handle_t handle);
    void OnRead();
    void OnWrite();
    void OnClose();
    void OnTimer(uint64_t curr_tick);

    virtual void OnWriteCompelete();

protected:
    net_handle_t    m_sockHandle;
    uint32_t        m_connHandle;
    bool            m_busy;

    uint32_t        m_state;
    std::string     m_peerIp;
    uint16_t        m_peerPort;
    CSimpleBuffer   m_inBuf;
    CSimpleBuffer   m_outBuf;

    uint64_t        m_lastSendTick;
    uint64_t        m_lastRecvTick;

    CHttpParserWrapper m_HttpParser;
};

typedef hash_map<uint32_t, CHttpConn*> HttpConnMap;

CHttpConn* FindHttpConnByHandle(uint32_t handle);
void init_http_conn();


#endif /* SRC_HTTP_SVR_HTTPCONN_H_ */
