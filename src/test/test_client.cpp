/*
 * main.cpp
 *
 *  Created on: 2016年5月10日
 *  Author: hhq163
 */

#include <vector>
#include <iostream>
#include "ClientConn.h"
#include "NetLib.h"
#include "TokenValidator.h"
#include "Thread.h"
#include "IM.BaseDefine.pb.h"
#include "IM.Buddy.pb.h"
#include "Common.h"
#include "Client.h"
#include "Log.h"
using namespace std;

#define MAX_LINE_LEN	1024
string g_connect_route_addr = "http://192.168.200.170:8080";//接入路由服务地址
<<<<<<< HEAD
string strName = "hhq123";
=======
string strName = "hhq100";
>>>>>>> branch 'master' of https://github.com/hhq163/NewMessage.git
string strPass = "3c4f228a4e1e6a8ab900c483248142d7";

CClient* g_pClient = NULL;

/**
 * 登录操作
 */
void doLogin(const string& strName, const string& strPass)
{
    try{
        g_pClient = new CClient(strName, strPass, g_connect_route_addr);
    }catch(...){
        printf("get error while alloc memory\n");
        PROMPTION;
        return;
    }
    g_pClient->connect();
}

class CmdThread : public CThread
{
public:
	void OnThreadRun()
	{
		doLogin(strName, strPass);	//登录

		bool stat = true;
		int caseId;
		while (stat){
			caseId = 0;
			cout << "Enter TestCase ID number: \n" <<
							"(1) devicetokenV12  test; \n"<<
<<<<<<< HEAD
							"(2) GroupCreate test; \n"<<
							"(3) feedback test; \n"<<
=======
							"(2) getUserInfo test; \n"<<
							"(3) GroupInfoChange test; \n"<<
>>>>>>> branch 'master' of https://github.com/hhq163/NewMessage.git
							"(4) PersonalMsgSend test; \n"<<
							"(5) MsgSync test; \n"<<
							"(6) Exit test System; \n"<<
							endl;
				cin >> caseId;

				switch(caseId){
				case 1:
					g_pClient->reqDevicetokenV12();
					break;
				case 2:
<<<<<<< HEAD
					g_pClient->reqGroupCreate();
					break;
				case 3:
					g_pClient->reqFeedback();
=======
//					testCase.GroupJoin();
					break;
				case 3:
//					testCase.GroupInfoChange();
>>>>>>> branch 'master' of https://github.com/hhq163/NewMessage.git
					break;
				case 4:
//					testCase.PersonalMsgSend(client_info_t);
					break;
				case 5:
//					testCase.MsgSync(client_info_t);
					break;
				case 6:
					stat = false;
					break;
				default:
					cout << "Your enter is error ,please " << endl;
					break;
				}
		}
		cout <<"ByeBye"<< endl;
		exit(0);
	}
private:
	char	m_buf[MAX_LINE_LEN];
};


CmdThread g_cmd_thread;

int main(int argc, char* argv[]){
	//日志类初始化
	 if (!Logger.Init(2000, 0, 0)){
		 cout <<"log init failed!!!"<< endl;
		return -1;
	}
	/* 设置打印级别 */
	Logger.SetLogLevel(0x0040, true);

	g_cmd_thread.StartThread();

<<<<<<< HEAD
//	signal(SIGPIPE, SIG_IGN);
=======
	signal(SIGPIPE, SIG_IGN);
>>>>>>> branch 'master' of https://github.com/hhq163/NewMessage.git

	int ret = netlib_init();

	if (ret == NETLIB_ERROR)
		return ret;

	netlib_eventloop();

	return 0;
}


