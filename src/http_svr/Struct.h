/*
 * 当前模块会用到的一些struct
 *
 *  Created on: 2016年5月22日
 *      Author: hhq163
 */

#ifndef SRC_HTTP_SVR_STRUCT_H_
#define SRC_HTTP_SVR_STRUCT_H_

#include <string>

typedef void (*Handler)();

struct Route
{
    std::string method; //请求的方法，post/get/put等
    std::string urlFormat;
    Handler handler;

    Route( std::string method, std::string urlFormat, Handler handler ) {
        this->method = method;
        this->urlFormat = urlFormat;
        this->handler = handler;
    }

    static Route New( std::string method, std::string urlFormat, Handler handler ) {
        Route r( method, urlFormat, handler );
        return r;
    }

};



#endif /* SRC_HTTP_SVR_STRUCT_H_ */
