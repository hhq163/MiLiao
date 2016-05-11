/*
 * Client.h
 *
 *  Created on: 2016年5月11日
 *      Author: hhq163
 */

#ifndef SRC_TEST_CLIENT_H_
#define SRC_TEST_CLIENT_H_

#include <iostream>
#include "OsType.h"
#include "IM.BaseDefine.pb.h"
#include "PacketCallback.h"
using namespace std;

typedef hash_map<uint32_t, IM::BaseDefine::UserInfo*> CMapId2User_t;
typedef hash_map<string, IM::BaseDefine::UserInfo*> CMapNick2User_t;
class CClient:public PacketCallback
{
public:
    CClient(const string& strName, const string& strPass, const string strDomain="http://192.168.200.170");
    ~CClient();
public:
    string getName(){ return m_strName; }
    string getDomain() { return m_strLoginDomain; }
    static void TimerCallback(void* callback_data, uint8_t msg, uint32_t handle, void* pParam);
    CMapNick2User_t& getNick2UserMap() { return m_mapNick2UserInfo; }

public:
    void connect();
    void close();
    uint32_t login(const string& strName, const string& strPass);
    uint32_t getChangedUser();
    uint32_t getUserInfo(list<uint32_t>& lsUserId);
    uint32_t reqDevicetokenV12();
    uint32_t reqGroupCreate();
    uint32_t sendMsg(uint32_t nToId,IM::BaseDefine::MsgType nType, const string& strMsg);
    uint32_t getUnreadMsgCnt();
    uint32_t getRecentSession();
    uint32_t getMsgList(IM::BaseDefine::SessionType nType, uint32_t nPeerId, uint32_t nMsgId, uint32_t nMsgCnt);
    uint32_t sendReadAck();
public:
    virtual void onError(uint32_t nSeqNo, uint32_t nCmd, const string& strMsg);
    virtual void onConnect();
    virtual void onClose();
    virtual void onLogin(uint32_t nSeqNo, uint32_t nResultCode, string& strMsg, IM::BaseDefine::UserInfo* pUser = NULL);
    virtual void onGetChangedUser(uint32_t nSeqNo,const list<IM::BaseDefine::UserInfo>& lsUser);
    virtual void onGetUserInfo(uint32_t nSeqNo, const list<IM::BaseDefine::UserInfo>& lsUser);
    virtual void onSendMsg(uint32_t nSeqNo, uint32_t nSendId, uint32_t nRecvId, IM::BaseDefine::SessionType nType, uint32_t nMsgId);
    virtual void onGetUnreadMsgCnt(uint32_t nSeqNo, uint32_t nUserId, uint32_t nTotalCnt, const list<IM::BaseDefine::UnreadInfo>& lsUnreadCnt);
    virtual void onGetRecentSession(uint32_t nSeqNo, uint32_t nUserId, const list<IM::BaseDefine::ContactSessionInfo>& lsSession);
    virtual void onGetMsgList(uint32_t nSeqNo, uint32_t nUserId, uint32_t nPeerId, IM::BaseDefine::SessionType nType, uint32_t nMsgId, uint32_t nMsgCnt, const list<IM::BaseDefine::MsgInfo>& lsMsg);
    virtual void onRecvMsg(uint32_t nSeqNo, uint32_t nFromId, uint32_t nToId, uint32_t nMsgId, uint32_t nCreateTime, IM::BaseDefine::MsgType nMsgType, const string& strMsgData);
private:
    string          m_strName;
    string          m_strPass;
    string          m_strLoginDomain;
    uint32_t        m_nLastGetUser;
    uint32_t        m_nLastGetSession;
    net_handle_t    m_nHandle;
    IM::BaseDefine::UserInfo    m_cSelfInfo;

    CMapId2User_t m_mapId2UserInfo;
    CMapNick2User_t m_mapNick2UserInfo;
};

#endif /* SRC_TEST_CLIENT_H_ */
