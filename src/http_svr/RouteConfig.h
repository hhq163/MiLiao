/*
 * RouteConfig.h
 *
 *  Created on: 2016年5月22日
 *      Author: hhq163
 */

#ifndef SRC_HTTP_SVR_ROUTECONFIG_H_
#define SRC_HTTP_SVR_ROUTECONFIG_H_
#include "OsType.h"
#include "HttpTask.h"

typedef map<string, ActionHandlerT> RouteConfigMap;

/*
 * 路由配置类
 */
class CRouteConfig {
public:
    static CRouteConfig* getInstance();

    virtual ~CRouteConfig();
    void Init();
    ActionHandlerT GetAction(string uri);
private:
    CRouteConfig();
    static  CRouteConfig* m_handlerInstance;
    RouteConfigMap    m_routeConfigMap;
};

#endif /* SRC_HTTP_SVR_ROUTECONFIG_H_ */
