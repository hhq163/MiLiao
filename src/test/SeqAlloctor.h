/*
 * SeqAlloctor.h
 *
 *  Created on: 2016年5月11日
 *      Author: hhq163
 */

#ifndef SRC_TEST_SEQALLOCTOR_H_
#define SRC_TEST_SEQALLOCTOR_H_

#include "OsType.h"

typedef enum
{
    ALLOCTOR_PACKET = 1,
} ALLOCTOR_TYPE;

/**
 * 序列号生成器
 */
class CSeqAlloctor
{
public:
    static CSeqAlloctor* getInstance();
    uint32_t getSeq(uint32_t nType);
private:
    CSeqAlloctor();
    virtual ~CSeqAlloctor();
private:
    static CSeqAlloctor* m_pInstance;
    hash_map<uint32_t, uint32_t> m_hmAlloctor;
};

#endif /* SRC_TEST_SEQALLOCTOR_H_ */
