/*
 * HttpTask.h
 *
 *  Created on: 2016年5月24日
 *      Author: hhq163
 */

#ifndef SRC_HTTP_SVR_HTTPTASK_H_
#define SRC_HTTP_SVR_HTTPTASK_H_

#include "OsType.h"
#include "Task.h"
#include "HttpParserWrapper.h"

//定义了一个名为ActionHandlerT,参数带两个参数，返回值为void的函数指针
typedef void (*ActionHandlerT)(CHttpParserWrapper* m_HttpParser, uint32_t conn_uuid);

class CHttpTask:public CTask {
public:
	CHttpTask(uint32_t connUuid, ActionHandlerT actHandler, CHttpParserWrapper* httpParser);
	virtual ~CHttpTask();
	virtual void run();
private:
	uint32_t 		m_connUuid;			//连接唯一标识
	ActionHandlerT	m_actionHandler;		//方法名
	CHttpParserWrapper* m_httpParser;//解析的HTTP协议参数值
};

#endif /* SRC_HTTP_SVR_HTTPTASK_H_ */
