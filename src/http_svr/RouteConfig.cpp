/*
 * RouteConfig.cpp
 *
 *  Created on: 2016年5月22日
 *      Author: hhq163
 */

#include "RouteConfig.h"
#include "controller/ControllerLogin.h"
#include "IM.BaseDefine.pb.h"

using namespace IM::BaseDefine;
CRouteConfig* CRouteConfig::m_handlerInstance = NULL;

CRouteConfig::CRouteConfig() {
    // TODO Auto-generated constructor stub

}

CRouteConfig::~CRouteConfig() {
    // TODO Auto-generated destructor stub
}

/**
 *  单例
 *
 *  @return 返回指向CRouteConfig的单例指针
 */
CRouteConfig* CRouteConfig::getInstance()
{
    if (!m_handlerInstance) {
        m_handlerInstance = new CRouteConfig();
        m_handlerInstance->Init();
    }

    return m_handlerInstance;
}

/**
 *  初始化函数,加载了各URL对应的处理函数
 */
void CRouteConfig::Init()
{
    // Login模块逻辑
	m_routeConfigMap.insert(make_pair("/api/login", HTTP_CONTROLLER::doLogin));


}

/**
 *  获取URI对应的控制器及方法
 */
ActionHandlerT CRouteConfig::GetAction(string uri){
    RouteConfigMap::iterator it = m_routeConfigMap.find(uri);
    if (it != m_routeConfigMap.end()) {
        return it->second;
    } else {
        return NULL;
    }
}
