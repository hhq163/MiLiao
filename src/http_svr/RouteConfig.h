/*
 * RouteConfig.h
 *
 *  Created on: 2016年5月22日
 *      Author: hhq163
 */

#ifndef SRC_HTTP_SVR_ROUTECONFIG_H_
#define SRC_HTTP_SVR_ROUTECONFIG_H_
#include "OsType.h"

typedef void (*pdu_handler_t)(CImPdu* pPdu, uint32_t conn_uuid);
typedef map<uint32_t, pdu_handler_t> RouteConfigMap;

/*
 * 路由配置类
 */
class CRouteConfig {
public:
    static CRouteConfig* getInstance();

    virtual ~CRouteConfig();
    void Init();
    pdu_handler_t GetAction(string uri);
private:
    CRouteConfig();
    static  CRouteConfig* m_handlerInstance;
    RouteConfigMap    m_routeConfigMap;
};

#endif /* SRC_HTTP_SVR_ROUTECONFIG_H_ */
