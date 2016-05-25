/*
 * main.cpp
 *
 *  Created on: 2016年5月10日
 *  Author: hhq163
 */

#include <iostream>
#include "ConfigFileReader.h"
#include "OsType.h"
#include "Log.h"
#include "ServInfo.h"
#include "DBPool.h"
#include "HttpConn.h"
/**
 * 端口监听回调
 */
void HttpCallback(void* callback_data, uint8_t msg, uint32_t handle, void* pParam)
{
    if (msg == NETLIB_MSG_CONNECT){
        CHttpConn* pConn = new CHttpConn();
        pConn->OnConnect(handle);
    }else{
    	Logger.Log(ERROR, "!!!error msg: %d ", msg);
    }
}

int main(int argc, char* argv[])
{
    if ((argc == 2) && (strcmp(argv[1], "-v") == 0)) {
    	cout << "HttpMsgServer   " << endl;
    	cout << "Server Build:" << __DATE__ << __TIME__ << endl;
        return ERR_SUCCESS;
    }

    signal(SIGPIPE, SIG_IGN);
    srand(time(NULL));

    //日志类初始化
	 if (!Logger.Init(2000, 0, 0)){
		 cout <<"log init failed!!!"<< endl;
		return -1;
	}
	/* 设置打印级别 */
	Logger.SetLogLevel(0x0040, true);


    Logger.Log(INFO, "The Server max files can open: %d ", getdtablesize());

    CConfigFileReader* configFile = new CConfigFileReader("httpsvr.conf");
    char* listenIp = configFile->GetConfigName("ListenIP");
    char* strListenPort = configFile->GetConfigName("ListenPort");
	char* strThreadNum = configFile->GetConfigName("ThreadNum");

	//初始化数据库连接及管理类
	CDBManager* pDBManager = CDBManager::getInstance(configFile);
	if (!pDBManager) {
		Logger.Log(ERROR, "DBManager init failed");
		return ERR_FAILED;
	}
	pDBManager->Init();

    if (!listenIp || !strListenPort) {
    	Logger.Log(ERROR, "config file miss, exit ");
        return ERR_FAILED;
    }

    int ret = netlib_init();
    if (ret == NETLIB_ERROR){
        return ret;
    }

    uint16_t listenPort = atoi(strListenPort);
    CStrExplode listen_ip_list(listenIp, ';');
    for (uint32_t i = 0; i < listen_ip_list.GetItemCnt(); i++) {
        ret = netlib_listen(listen_ip_list.GetItem(i), listenPort, HttpCallback, NULL);
        if (ret == NETLIB_ERROR)
            return ret;
    }

    cout << "server start listen on: " << listenIp << listenPort << endl;

    uint32_t threadNum = atoi(strThreadNum);
    InitHttpConn(threadNum);//初始化线程，增加心跳定时器


    cout << "now enter the event loop!" << endl;

    writePid();

    netlib_eventloop();

    return 0;
}
