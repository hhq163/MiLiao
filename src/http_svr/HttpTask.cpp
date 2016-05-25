/*
 * HttpTask.cpp
 *
 *  Created on: 2016年5月24日
 *      Author: hhq163
 */

#include "Log.h"
#include "HttpParserWrapper.h"
#include "HttpConn.h"
#include "HttpTask.h"

CHttpTask::CHttpTask(uint32_t connUuid, ActionHandlerT actHandler, CHttpParserWrapper* httpParser) {
	// TODO Auto-generated constructor stub

	m_connUuid = connUuid;
	m_actionHandler = actHandler;
	m_httpParser = httpParser;
}

CHttpTask::~CHttpTask() {
	// TODO Auto-generated destructor stub

	if (m_httpParser) {
			delete m_httpParser;
		}
}

void CHttpTask::run()
{
	Logger.Log(INFO, "CHttpTask::run 111");
	if (!m_httpParser) {
		Logger.Log(INFO, "CHttpTask::run 222");
		// tell CProxyConn to close connection with m_conn_uuid
		CHttpConn::AddResponse(m_connUuid, NULL);
	} else {
		if (m_actionHandler) {
			Logger.Log(INFO, "CHttpTask::run 333");
			m_actionHandler(m_httpParser, m_connUuid);
		}
	}
}
