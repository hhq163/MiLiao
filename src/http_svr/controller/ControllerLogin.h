/*
 * 登录逻辑处理
 *
 *  Created on: 2016年5月22日
 *      Author: hhq163
 */

#ifndef SRC_HTTP_SVR_CONTROLLER_CONTROLLERLOGIN_H_
#define SRC_HTTP_SVR_CONTROLLER_CONTROLLERLOGIN_H_

#include "OsType.h"
#include "HttpParserWrapper.h"

namespace HTTP_CONTROLLER {

	void doLogin(CHttpParserWrapper* m_HttpParser, uint32_t conn_uuid);

}

#endif /* SRC_HTTP_SVR_CONTROLLER_CONTROLLERLOGIN_H_ */
