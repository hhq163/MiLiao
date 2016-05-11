/*
 * Http协议操作类
 *
 *  Created on: 2016年5月11日
 *      Author: hhq163
 */

#ifndef SRC_CORE_HTTPCLIENT_H_
#define SRC_CORE_HTTPCLIENT_H_

#include <string>
#include <curl/curl.h>
#include "PublicDefine.h"

class CHttpClient
{
public:
    CHttpClient(void);
    ~CHttpClient(void);

public:
    CURLcode Post(const string & strUrl, const string & strPost, string & strResponse);
    CURLcode Get(const string & strUrl, string & strResponse);
    string UploadByteFile(const string &url, void* data, int data_len);
    bool DownloadByteFile(const string &url, AudioMsgInfo* pAudioMsg);
};

#endif /* SRC_CORE_HTTPCLIENT_H_ */
