/*
 * 锁操作类
 *
 *  Created on: 2016年5月11日
 *      Author: hhq16
 */

#ifndef SRC_CORE_LOCK_H_
#define SRC_CORE_LOCK_H_

#include "OsType.h"

class CLock
{
public:
    CLock();
    virtual ~CLock();
    void lock();
    void unlock();
    pthread_mutex_t& getMutex() { return m_lock; }
#ifndef _WIN32
    virtual bool try_lock();
#endif
private:
#ifdef _WIN32
	CRITICAL_SECTION m_critical_section;
#else
    pthread_mutex_t m_lock;
#endif
};

#ifndef _WIN32
class CRWLock
{
public:
    CRWLock();
    virtual ~CRWLock();
    void rlock();
    void wlock();
    void unlock();
    bool try_rlock();
    bool try_wlock();
private:
    pthread_rwlock_t m_lock;
};

class CAutoRWLock
{
public:
    CAutoRWLock(CRWLock* pLock, bool bRLock = true);
    virtual ~CAutoRWLock();
private:
    CRWLock* m_pLock;
};

#endif

class CAutoLock
{
public:
    CAutoLock(CLock* pLock);
    virtual ~CAutoLock();
private:
    CLock* m_pLock;
};

#endif /* SRC_CORE_LOCK_H_ */
