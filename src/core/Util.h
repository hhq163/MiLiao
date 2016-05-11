#ifndef __UTIL_H__
#define __UTIL_H__
#define _CRT_SECURE_NO_DEPRECATE	// remove warning C4996,

#include "OsType.h"
#include "UtilPdu.h"
#include "Lock.h"
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef _WIN32
	#include <strings.h>
#endif

#include <sys/stat.h>
#include <assert.h>

#ifdef _WIN32
	#define	snprintf	sprintf_s
#else
	#include <stdarg.h>
	#include <pthread.h>
	#include <time.h>
	#include <sys/time.h>
#endif

#define NOTUSED_ARG(v) ((void)v)		// used this to remove warning C4100, unreferenced parameter

class CRefObject
{
public:
	CRefObject();
	virtual ~CRefObject();

	void SetLock(CLock* lock) { m_lock = lock; }
	void AddRef();
	void ReleaseRef();
private:
	int			m_refCount;
	CLock*	m_lock;
};

uint64_t get_tick_count();
void util_sleep(uint32_t millisecond);

class CStrExplode
{
public:
	CStrExplode(char* str, char seperator);
	virtual ~CStrExplode();

	uint32_t GetItemCnt() { return m_item_cnt; }
	char* GetItem(uint32_t idx) { return m_item_list[idx]; }
private:
	uint32_t	m_item_cnt;
	char** 		m_item_list;
};


/**
 * 信息号量操作类
 */
class CSem
{
public:
    CSem();
    ~CSem();

public:
    int Wait();
    int timedwait();
    int Post();
    int  GetValue();

private:
    sem_t *m_sem;
};

typedef void (*CallbackDel)(void* pVoid);
template<typename T>
/**
 * 链表类，用于保存对象池
 */
class CObjectPool
{
    struct Node_t
    {
        T*      pData;
        Node_t* pNext;
    };

public:
    CObjectPool(CallbackDel cb = NULL);
    virtual ~CObjectPool();

public:
    int     Init(int iMaxSize);
    void    Clear();
    int     SetObject(T* pInObj);
    int     GetObject(T** ppOutObj);
    int     GetObjectAll(vector<T*>& ppOutObjList);
    T*      GetObjectByPos(uint32_t pos);
    int     GetNum();

private:
    CallbackDel m_cbDel;
    Node_t      *m_pHead, *m_pTail, *m_pIdle;
    CLock       m_oLock;
    uint32_t      m_dwMaxSize;
    uint32_t      m_dwMySize;
};

template<typename T>
CObjectPool<T>::CObjectPool(CallbackDel cb /*= NULL*/):
m_cbDel(cb)
{
    m_pHead     = NULL;
    m_pTail     = NULL;
    m_pIdle     = NULL;
    m_dwMaxSize = 0;
    m_dwMySize  = 0;
}

template<typename T>
CObjectPool<T>::~CObjectPool()
{
    Clear();
}

template<typename T>
void CObjectPool<T>::Clear()
{
    CAutoLock lock(&m_oLock);
    while (NULL != m_pHead)
    {
        Node_t* pCur = m_pHead;
        m_pHead = m_pHead->pNext;
        if (NULL == m_cbDel)
        {
        delete pCur->pData;
        }
        else
        {
            m_cbDel(pCur->pData);
        }
        delete pCur;
    }

    while (NULL != m_pIdle)
    {
        Node_t* pCur = m_pIdle;
        m_pIdle = m_pIdle->pNext;
        delete pCur;
    }

    m_pHead = m_pTail = m_pIdle = NULL;
    m_dwMySize = 0;
}

template<typename T>
int CObjectPool<T>::Init(int iMaxSize)
{
    Clear();
    m_dwMaxSize = iMaxSize;
    m_dwMySize  = 0;
    return ERR_SUCCESS;
}

template<typename T>
int CObjectPool<T>::SetObject(T* pInObj)
{
    if(pInObj == NULL)
    {
        return ERR_FAILED;
    }

    CAutoLock lock(&m_oLock);
    /*if(m_dwMaxSize > 0 && m_dwMySize >= m_dwMaxSize)
    {
        Node_t* pCur   = m_pHead;
        m_pHead        = m_pHead->pNext;
        delete pCur->pData;
        pCur->pData    = pInObj;
        pCur->pNext    = NULL;
        m_pTail->pNext = pCur;
        m_pTail        = pCur;

        return ERR_SUCCESS;
    }*/

    Node_t* pCur = NULL;
    if (NULL == m_pIdle)
    {
        pCur = new Node_t();
    }
    else
    {
        pCur = m_pIdle;
        m_pIdle = m_pIdle->pNext;
    }
    pCur->pData  = pInObj;
    pCur->pNext  = NULL;

    if (NULL == m_pTail)
    {
        m_pHead = m_pTail = pCur;
    }
    else
    {
        m_pTail->pNext = pCur;
        m_pTail = pCur;
    }
    m_dwMySize++;

    return ERR_SUCCESS;
}

template<typename T>
int CObjectPool<T>::GetObject(T** ppOutObj)
{
    CAutoLock lock(&m_oLock);
    if(NULL == m_pHead)
    {
        return ERR_FAILED;
    }

    *ppOutObj = (T*)m_pHead->pData;
    Node_t* pCur = m_pHead;
    if (m_pHead == m_pTail)
    {
        m_pHead = m_pTail = NULL;
    }
    else
    {
        m_pHead = m_pHead->pNext;
    }
    m_dwMySize--;

    pCur->pData = NULL;
    pCur->pNext = NULL;
    if (NULL == m_pIdle)
    {
        m_pIdle = pCur;
    }
    else
    {
        pCur->pNext = m_pIdle;
        m_pIdle = pCur;
    }

    return ERR_SUCCESS;
}


template<typename T>
int CObjectPool<T>::GetObjectAll(vector<T*>& ppOutObjList)
{
    CAutoLock lock(&m_oLock);
    if(NULL == m_pHead)
    {
        return ERR_FAILED;
    }

    while ( NULL != m_pHead)
    {
        ppOutObjList.push_back((T*)m_pHead->pData);
        Node_t* pCur = m_pHead;
        if (m_pHead == m_pTail)
        {
            m_pHead = m_pTail = NULL;
        }
        else
        {
            m_pHead = m_pHead->pNext;
        }
        m_dwMySize--;

        pCur->pData = NULL;
        pCur->pNext = NULL;
        if (NULL == m_pIdle)
        {
            m_pIdle = pCur;
        }
        else
        {
            pCur->pNext = m_pIdle;
            m_pIdle = pCur;
        }
    }
    return ERR_SUCCESS;
}

template<typename T>
T* CObjectPool<T>::GetObjectByPos(uint32_t  pos)
{
    CAutoLock lock(&m_oLock);
    if( pos >  m_dwMySize ) {return NULL;}
    Node_t* pCur = m_pHead;
    while ( --pos > 0  ) {pCur = pCur->pNext;}

    T* ppOutObj = (T*)pCur->pData;
    return ppOutObj;
}


template<typename T>
int CObjectPool<T>::GetNum()
{
    //CAutoLock lock(m_oLock.getMutex());
    return m_dwMySize;
}


char* replaceStr(char* pSrc, char oldChar, char newChar);
string int2string(uint32_t user_id);
uint32_t string2int(const string& value);
void replace_mark(string& str, string& new_value, uint32_t& begin_pos);
void replace_mark(string& str, uint32_t new_value, uint32_t& begin_pos);

void writePid();
inline unsigned char toHex(const unsigned char &x);
inline unsigned char fromHex(const unsigned char &x);
string URLEncode(const string &sIn);
string URLDecode(const string &sIn);

int64_t get_file_size(const char *path);
const char*  memfind(const char *src_str,size_t src_len, const char *sub_str, size_t sub_len, bool flag = true);

#endif
