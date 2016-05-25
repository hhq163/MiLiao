/**
 * 当前模块预定义变量
 */
#ifndef SRC_HTTP_SVR_COMMON_H_
#define SRC_HTTP_SVR_COMMON_H_

#include "OsType.h"

#define READ_BUF_SIZE   2048
#define HTTP_CONN_TIMEOUT           60000*2

//连接状态
enum CONN_STATE{
    CONN_STATE_IDLE,
    CONN_STATE_CONNECTED,
    CONN_STATE_OPEN,
    CONN_STATE_CLOSED,
};

//http错误码
typedef enum {
    HTTP_ERROR_SUCCESS = 0,
    HTTP_ERROR_PARMENT,
    HTTP_ERROR_APPKEY,
    HTTP_ERROR_MATCH,
    HTTP_ERROR_PERMISSION,
    HTTP_ERROR_INTERFACE,
    HTTP_ERROR_IP,
    HTTP_ERROR_SEND_TYPE,
    HTTP_ERROR_UNKNOWN,
    HTTP_ERROR_MAX,
    HTTP_ERROR_SERVER_EXCEPTION,
    HTTP_ERROR_CREATE_GROUP,
    HTTP_ERROR_CHANGE_MEMBER,
    HTTP_ERROR_ENCRYPT,
} HTTP_ERROR_CODE;

static string HTTP_ERROR_MSG[] =
{
    "成功",
    "参数错误",
    "appKey不存在",
    "appKey与用户不匹配",
    "含有不允许发送的Id",
    "未授权的接口",
    "未授权的IP",
    "非法的发送类型",
    "未知错误",
    "服务器异常",
    "创建群失败",
    "更改群成员失败",
    "消息加密失败",
};

#endif /* SRC_HTTP_SVR_COMMON_H_ */
