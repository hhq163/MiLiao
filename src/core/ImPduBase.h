/*
 * 协议数据单元基类
 *
 *  Created on: 2016年5月11日
 *      Author: hhq163
 */

#ifndef SRC_CORE_IMPDUBASE_H_
#define SRC_CORE_IMPDUBASE_H_

#include "UtilPdu.h"
#include "google/protobuf/message_lite.h"

#define IM_PDU_HEADER_LEN		16
#define IM_PDU_VERSION			1


#define ALLOC_FAIL_ASSERT(p) if (p == NULL) { \
throw CPduException(m_pduHeader.serviceId, m_pduHeader.commandId, ERROR_CODE_ALLOC_FAILED, "allocate failed"); \
}

#define CHECK_PB_PARSE_MSG(ret) { \
    if (ret == false) \
    {\
    	Logger.Log(ERROR, "parse pb msg failed.");\
        return;\
    }\
}



#ifdef WIN32
	#ifdef BUILD_PDU
		#define DLL_MODIFIER __declspec(dllexport)
	#else
		#define DLL_MODIFIER __declspec(dllimport)
	#endif
#else
	#define DLL_MODIFIER
#endif

/**
 * 包头类
 */
typedef struct {
    uint32_t 	length;		  // the whole pdu length
    uint16_t 	version;	  // pdu version number
    uint16_t	flag;		  // not used
    uint16_t	serviceId;	  // 服务类型
    uint16_t	commandId;	  //命令字
    uint16_t	seqNum;     // 包序号
    uint16_t    reversed;    // 保留
} PduHeader_t;

/**
 * protocol data unit；协议数据单元
 */
class DLL_MODIFIER CImPdu
{
public:
    CImPdu();
    virtual ~CImPdu() {}

    uchar_t* GetBuffer();
    uint32_t GetLength();
    uchar_t* GetBodyData();
    uint32_t GetBodyLength();


    uint16_t GetVersion() { return m_pduHeader.version; }
    uint16_t GetFlag() { return m_pduHeader.flag; }
    uint16_t GetServiceId() { return m_pduHeader.serviceId; }
    uint16_t GetCommandId() { return m_pduHeader.commandId; }
    uint16_t GetSeqNum() { return m_pduHeader.seqNum; }
    uint32_t GetReversed() { return m_pduHeader.reversed; }

    void SetVersion(uint16_t version);
    void SetFlag(uint16_t flag);
    void SetServiceId(uint16_t serviceId);
    void SetCommandId(uint16_t commandId);
    void SetError(uint16_t error);
    void SetSeqNum(uint16_t seqNum);
    void SetReversed(uint32_t reversed);
    void WriteHeader();

    static bool IsPduAvailable(uchar_t* buf, uint32_t len, uint32_t& pduLen);
    static CImPdu* ReadPdu(uchar_t* buf, uint32_t len);
    void Write(uchar_t* buf, uint32_t len) { m_buf.Write((void*)buf, len); }
    int ReadPduHeader(uchar_t* buf, uint32_t len);
    void SetPBMsg(const google::protobuf::MessageLite* msg);

protected:
    CSimpleBuffer	m_buf;
    PduHeader_t		m_pduHeader;
};




#endif /* SRC_CORE_IMPDUBASE_H_ */
