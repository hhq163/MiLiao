/*
 * User.h
 *
 *  Created on: 2016年6月6日
 *      Author: hhq163
 */

#ifndef SRC_HTTP_SVR_MODEL_USER_H_
#define SRC_HTTP_SVR_MODEL_USER_H_
#include "PublicDefine.h"

class CUser {
public:
	static CUser* getInstance();
	virtual ~CUser();
	bool InsertUser(DBUserInfo_t& cUser);
	bool UpdateUser(DBUserInfo_t& cUser);
	bool GetIdByName(string& user_name);//根据用户名取用户ID
private:
	CUser();
private:
    static CUser* m_pInstance;
};

#endif /* SRC_HTTP_SVR_MODEL_USER_H_ */
