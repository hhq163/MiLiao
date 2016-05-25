/*
 * 用户模型
 *
 *  Created on: 2016年6月6日
 *      Author: hhq163
 */

#include "DBPool.h"
#include "Log.h"
#include "User.h"

CUser* CUser::m_pInstance = NULL;
CUser::CUser() {
	// TODO Auto-generated constructor stub

}

CUser::~CUser() {
	// TODO Auto-generated destructor stub
}

CUser* CUser::getInstance()
{
    if(m_pInstance == NULL)
    {
        m_pInstance = new CUser();
    }
    return m_pInstance;
}

bool InsertUser(DBUserInfo_t& cUser)
{
	uint32_t ret= false;
	CDBManager* pDBManager = CDBManager::getInstance();
	CDBConn* pDBConn = pDBManager->GetDBConn("nm_slave");
	 if (pDBConn){
		string strSql = "insert into IMUser(`id`,`sex`,`nick`,`domain`,`name`,`phone`,`email`,`avatar`,`sign_info`,`departId`,`status`,`created`,`updated`,`level`,`country`) values(?,?,?,?,?,?,?,?,?,?,?,?,?,?)";
		CPrepareStatement* stmt = new CPrepareStatement();
		if (stmt->Init(pDBConn->GetMysql(), strSql)){
			uint32_t nNow = (uint32_t) time(NULL);
			uint32_t index = 0;
			uint32_t nGender = cUser.nSex;
			uint32_t nStatus = cUser.nStatus;
			stmt->SetParam(index++, cUser.nId);
			stmt->SetParam(index++, nGender);
			stmt->SetParam(index++, cUser.strNick);
			stmt->SetParam(index++, cUser.strDomain);
			stmt->SetParam(index++, cUser.strName);
			stmt->SetParam(index++, cUser.strTel);
			stmt->SetParam(index++, cUser.strEmail);
			stmt->SetParam(index++, cUser.strAvatar);
			stmt->SetParam(index++, cUser.sign_info);
			stmt->SetParam(index++, cUser.nDeptId);
			stmt->SetParam(index++, nStatus);
			stmt->SetParam(index++, nNow);
			stmt->SetParam(index++, nNow);
			bRet = stmt->ExecuteUpdate();

			if (!bRet){
				Logger.Log(ERROR, "insert user failed: %s", strSql.c_str());
			}
		}
		delete stmt;
		pDBManager->RelDBConn(pDBConn);
	}else{
		Logger.Log(ERROR, "no db connection for nm_master");
	}
	return ret;
}

bool UpdateUser(DBUserInfo_t& cUser)
{
	uint32_t ret= false;
	CDBManager* pDBManager = CDBManager::getInstance();
	CDBConn* pDBConn = pDBManager->GetDBConn("nm_slave");
	if (pDBConn){

	}
	return ret;
}
/**
 * 根据用户名取用户ID
 */
bool GetIdByName(string& user_name)
{
	uint32_t ret= false;
	CDBManager* pDBManager = CDBManager::getInstance();
	CDBConn* pDBConn = pDBManager->GetDBConn("nm_slave");
	if (pDBConn){

	}
	return ret;
}
