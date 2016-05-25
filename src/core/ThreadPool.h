/*
 * TheadPool.h
 *
 *  Created on: 2016年5月24日
 *      Author: hhq163
 */

#ifndef SRC_CORE_THREADPOOL_H_
#define SRC_CORE_THREADPOOL_H_

#include "OsType.h"
#include "Thread.h"
#include "Task.h"
#include <pthread.h>
#include <list>
using namespace std;

class CWorkerThread {
public:
	CWorkerThread();
	~CWorkerThread();

	static void* StartRoutine(void* arg);

	void Start();
	void Execute();
	void PushTask(CTask* pTask);

	void SetThreadIdx(uint32_t idx) { m_thread_idx = idx; }
private:

	uint32_t		m_thread_idx;
	uint32_t		m_task_cnt;
	pthread_t		m_thread_id;
	CThreadNotify	m_thread_notify;
	list<CTask*>	m_task_list;
};



class CThreadPool {
public:
	CThreadPool();
	virtual ~CThreadPool();

	int Init(uint32_t worker_size);
	void AddTask(CTask* pTask);
	void Destory();
private:
	uint32_t 		m_worker_size;
	CWorkerThread* 	m_worker_list;
};



#endif /* SRC_CORE_THREADPOOL_H_ */
