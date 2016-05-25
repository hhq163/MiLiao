/*
 * ControllerLogin.cpp
 *
 *  Created on: 2016年5月22日
 *      Author: hhq163
 */

#include "Log.h"
#include "HttpConn.h"
#include "OsType.h"
#include "json/json.h"
#include "ControllerLogin.h"

namespace HTTP_CONTROLLER
{
    void doLogin(CHttpParserWrapper* m_HttpParser, uint32_t conn_uuid)
    {
    	int32_t ret = ERR_FAILED;
    	string strContent = "Welcome to NM http_server  run success!!! \n";
    	Json::Value value;
		value["code"] = 0;
		value["msg"] = "";
		value["priorIP"] = "192.168.200.170";
		value["backupIP"] = "192.168.200.170";
		value["port"] = 8000;
		strContent += value.toStyledString();

    	Logger.Log(INFO, "doLogin success");

    	CHttpConn::AddResponse(conn_uuid, strContent);

    }
};


