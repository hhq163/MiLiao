/*
 * main.cpp
 *
 *  Created on: 2016年5月10日
 *  Author: hhq163
 */

#include "OsType.h"
#include "Log.h"
#include "ServInfo.h"
#include "HttpConn.h"

void HttpCallback(void* callback_data, uint8_t msg, uint32_t handle, void* pParam)
{
    if (msg == NETLIB_MSG_CONNECT)
    {
        CHttpConn* pConn = new CHttpConn();
        pConn->OnConnect(handle);
    }
    else
    {
        log("!!!error msg: %d ", msg);
    }
}

int main(int argc, char* argv[])
{
    if ((argc == 2) && (strcmp(argv[1], "-v") == 0)) {
        printf("Server Version: HttpMsgServer/%s\n", VERSION);
        printf("Server Build: %s %s\n", __DATE__, __TIME__);
        return ERR_SUCCESS;
    }

    signal(SIGPIPE, SIG_IGN);
    srand(time(NULL));

    Logger.Log(INFO, "The Server max files can open: %d ", getdtablesize());

    CConfigFileReader configFile("httpsvr.conf");

    char* listenIp = configFile.GetConfigName("ListenIP");
    char* strListenPort = configFile.GetConfigName("ListenPort");

    uint32_t db_server_count = 0;
    serv_info_t* db_server_list = read_server_config(&configFile, "DBServerIP", "DBServerPort", db_server_count);

    // 到db的开多个并发的连接，这段逻辑好奇怪
    uint32_t concurrent_db_conn_cnt = DEFAULT_CONCURRENT_DB_CONN_CNT;
    uint32_t db_server_count2 = db_server_count * DEFAULT_CONCURRENT_DB_CONN_CNT;
    char* concurrent_db_conn = configFile.GetConfigName("ConcurrentDBConnCnt");
    if (concurrent_db_conn) {
        concurrent_db_conn_cnt  = atoi(concurrent_db_conn);
        db_server_count2 = db_server_count * concurrent_db_conn_cnt;
    }

    serv_info_t* db_server_list2 = NULL;
    if (db_server_count2 > 0) {
        db_server_list2 = new serv_info_t [ db_server_count2];
        for (uint32_t i = 0; i < db_server_count2; i++) {
            db_server_list2[i].server_ip = db_server_list[i / concurrent_db_conn_cnt].server_ip.c_str();
            db_server_list2[i].server_port = db_server_list[i / concurrent_db_conn_cnt].server_port;
        }
    }

    if (!listenIp || !strListenPort) {
    	Logger.Log(ERROR, "config file miss, exit... ");
        return ERR_FAILED;
    }

    uint16_t listenPort = atoi(strListenPort);

    int ret = netlib_init();

    if (ret == NETLIB_ERROR){
        return ret;
    }

    CStrExplode listen_ip_list(listenIp, ';');
    for (uint32_t i = 0; i < listen_ip_list.GetItemCnt(); i++) {
        ret = netlib_listen(listen_ip_list.GetItem(i), listenPort, HttpCallback, NULL);
        if (ret == NETLIB_ERROR)
            return ret;
    }

    printf("server start listen on: %s:%d\n", listenIp, listenPort);

    init_http_conn();//增加一个定时器
    log("===db_server_count : %u ===\n", db_server_count);
    if (db_server_count > 0) {
        log("++db_server_count : %u 0++\n", db_server_count);
        HTTP::init_db_serv_conn(db_server_list2, db_server_count2, concurrent_db_conn_cnt);
    }

    if (route_server_count > 0) {
        HTTP::init_route_serv_conn(route_server_list, route_server_count);
    }

    printf("now enter the event loop...\n");

    writePid();

    netlib_eventloop();

    return 0;
}
