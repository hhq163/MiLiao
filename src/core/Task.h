/*
 * 任务基类
 *
 *  Created on: 2016年5月24日
 *      Author: hhq163
 */

#ifndef SRC_CORE_TASK_H_
#define SRC_CORE_TASK_H_

class CTask
{
public:
    CTask(){}
    virtual ~CTask(){}

    virtual void run() = 0;
private:
};

#endif /* SRC_CORE_TASK_H_ */
