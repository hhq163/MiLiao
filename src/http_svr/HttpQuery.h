/*
 * HttpQuery.h
 *
 *  Created on: 2013-10-22
 *      Author: ziteng@mogujie.com
 */

#ifndef HTTPQUERY_H_
#define HTTPQUERY_H_
#include "json/json.h"
#include "Common.h"
#include "HttpConn.h"


class CHttpQuery
{
public:
	virtual ~CHttpQuery() {}

	static CHttpQuery* GetInstance();

	static void DispatchQuery(std::string& url, std::string& post_data, CHttpConn* pHttpConn);
    
private:
	CHttpQuery() {}
    static void _QueryCreateGroup(const string& strAppKey,Json::Value& post_json_obj, CHttpConn* pHttpConn);
    static void _QueryChangeMember(const string& strAppKey,Json::Value& post_json_obj, CHttpConn* pHttpConn);
    static HTTP_ERROR_CODE _CheckAuth(const string& strAppKey, const uint32_t userId, const string& strInterface, const string& strIp);
    static HTTP_ERROR_CODE _CheckPermission(const string& strAppKey, uint8_t nType, const list<uint32_t>& lsToId , string strMsg);
    
private:
	static CHttpQuery*	m_query_instance;
};


#endif /* HTTPQUERY_H_ */
