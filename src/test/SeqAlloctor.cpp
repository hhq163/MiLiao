/*
 * SeqAlloctor.cpp
 *
 *  Created on: 2016年5月11日
 *      Author: hhq163
 */

#include <stdlib.h>
#include "SeqAlloctor.h"

CSeqAlloctor* CSeqAlloctor::m_pInstance = NULL;

CSeqAlloctor::CSeqAlloctor()
{

}

CSeqAlloctor::~CSeqAlloctor()
{

}

CSeqAlloctor* CSeqAlloctor::getInstance()
{
    if(!m_pInstance){
        m_pInstance = new CSeqAlloctor();
    }
    return m_pInstance;
}
/**
 * 获取消息序列号
 */
uint32_t CSeqAlloctor::getSeq(uint32_t nType)
{
    auto it=m_hmAlloctor.find(nType);
    uint32_t nSeqNo = 0;
    if(it != m_hmAlloctor.end()){
        it->second++;
        nSeqNo = it->second;
    }else{
        srand(time(NULL));
        nSeqNo = rand() + 1;
        m_hmAlloctor[nType] = nSeqNo;
    }
    return nSeqNo;
}

