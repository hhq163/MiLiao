/*
 * PublicDefine.h
 *
 *  Created on: 2016年5月11日
 *      Author: hhq163
 */

#ifndef SRC_CORE_PUBLICDEFINE_H_
#define SRC_CORE_PUBLICDEFINE_H_

#include <iostream>
#include <set>
#include "IM.BaseDefine.pb.h"
#include "OsType.h"
using namespace std;


//error code
#define ERR_SUCCESS          0
#define ERR_FAILED          1
#define ERR_PROTOCOL			2
#define ERR_NO_MORE_DATA    3
#define ERR_NO_MORE_SPACE   4

enum {
    USER_CNT_INC = 1,
    USER_CNT_DEC = 2,
};

enum {
    IM_GROUP_SETTING_PUSH = 1,
};

enum {
    IM_PUSH_TYPE_NORMAL = 1,
    IM_PUSH_TYPE_SILENT = 2,
};

enum {
    IM_PC_LOGIN_STATUS_ON = 1,
    IM_PC_LOGIN_STATUS_OFF = 0,
};

// client type:
#define CLIENT_TYPE_FLAG_NONE    0x00
#define CLIENT_TYPE_FLAG_PC      0x01
#define CLIENT_TYPE_FLAG_MOBILE  0x02
#define CLIENT_TYPE_FLAG_BOTH    0x03

#define CHECK_CLIENT_TYPE_PC(type) \
({\
bool bRet = false;\
if ((type & 0x10) == 0x00)\
{\
bRet = true;\
}\
bRet;\
})

#define CHECK_CLIENT_TYPE_MOBILE(type) \
({\
bool bRet = false;\
if ((type & 0x10) == 0x10)\
{\
bRet = true;\
}\
bRet;\
})

enum {
    GENDER_UNKNOWN  = 0,
    GENDER_MAN      = 1,
    GENDER_WOMAN    = 2,
};


#define CHECK_MSG_TYPE_SINGLE(type) \
({\
bool bRet = false;\
if ((IM::BaseDefine::MSG_TYPE_SINGLE_TEXT == type) || (IM::BaseDefine::MSG_TYPE_SINGLE_AUDIO == type))\
{\
bRet = true;\
}\
bRet;\
})


#define CHECK_MSG_TYPE_GROUP(type) \
({\
bool bRet = false;\
if ((IM::BaseDefine::MSG_TYPE_GROUP_TEXT == type) || (IM::BaseDefine::MSG_TYPE_GROUP_AUDIO == type))\
{\
bRet = true;\
}\
bRet;\
})

typedef struct AudioMsgInfo{
    uint32_t    audioId;
    uint32_t    fileSize;
    uint32_t    data_len;
    uchar_t*    data;
    string      path;

} AudioMsgInfo_t;

typedef struct DBUserInfo_t
{
    uint32_t nId;//用户ID
    uint8_t nSex;// 用户性别 1.男;2.女
    uint8_t nStatus; // 用户状态0 正常， 1 离职
    string strNick;// 花名
    string strPinyin;// 花名拼音
    string strName;// 真名
    string strTel;// 手机号码
    string strEmail;// Email
    string strAvatar;// 头像
    string sign_info;//个性签名
    uint8 nLevel;//用户等级
    string strCountry;//国家英文缩写
    DBUserInfo_t& operator=(const DBUserInfo_t& rhs)
    {
        if(this != &rhs)
        {
            nId = rhs.nId;
            nSex = rhs.nSex;
            nStatus = rhs.nStatus;
            strNick = rhs.strNick;
            strPinyin = rhs.strPinyin;
            strName = rhs.strName;
            strTel = rhs.strTel;
            strEmail = rhs.strEmail;
            strAvatar = rhs.strAvatar;
            sign_info = rhs.sign_info;
            nLevel = rhs.nLevel;
            strCountry = rhs.strCountry;
        }
        return *this;
    }

} DBUserInfo_t;

typedef hash_map<uint32_t, DBUserInfo_t*> DBUserMap_t;


typedef struct {
    uint32_t 	user_id;
    uint32_t	conn_cnt;
} user_conn_t;

typedef struct {
    uint32_t user_id;
    uint32_t status;
    uint32_t client_type;
} user_stat_t;

typedef struct
{
    uint32_t user_id;
    set<uint32_t> allow_user_ids;
    set<uint32_t> allow_group_ids;
    set<string>  authed_ips;
    set<string>  authed_interfaces;
} auth_struct;

#define MAX_MSG_LEN     4096		//最大消息长度

#endif /* SRC_CORE_PUBLICDEFINE_H_ */
