/*
 * ImPduBase.cpp
 *
 *  Created on: 2016年5月11日
 *      Author: hhq163
 */

#include "Util.h"
#include "Log.h"
#include "ImPduBase.h"
#include "IM.BaseDefine.pb.h"
using namespace IM::BaseDefine;

CImPdu::CImPdu()
{
	m_pduHeader.version = IM_PDU_VERSION;
	m_pduHeader.flag = 0;
	m_pduHeader.serviceId = SID_OTHER;
	m_pduHeader.commandId = 0;
	m_pduHeader.seqNum = 0;
    m_pduHeader.reversed = 0;
}

uchar_t* CImPdu::GetBuffer()
{
    return m_buf.GetBuffer();
}

uint32_t CImPdu::GetLength()
{
    return m_buf.GetWriteOffset();
}

uchar_t* CImPdu::GetBodyData()
{
    return m_buf.GetBuffer() + sizeof(PduHeader_t);
}

uint32_t CImPdu::GetBodyLength()
{
    uint32_t body_length = 0;
    body_length = m_buf.GetWriteOffset() - sizeof(PduHeader_t);
    return body_length;
}

void CImPdu::WriteHeader()
{
	uchar_t* buf = GetBuffer();

	CByteStream::WriteInt32(buf, GetLength());
	CByteStream::WriteUint16(buf + 4, m_pduHeader.version);
	CByteStream::WriteUint16(buf + 6, m_pduHeader.flag);
	CByteStream::WriteUint16(buf + 8, m_pduHeader.serviceId);
	CByteStream::WriteUint16(buf + 10, m_pduHeader.commandId);
    CByteStream::WriteUint16(buf + 12, m_pduHeader.seqNum);
    CByteStream::WriteUint16(buf + 14, m_pduHeader.reversed);
}

void CImPdu::SetVersion(uint16_t version)
{
	uchar_t* buf = GetBuffer();
	CByteStream::WriteUint16(buf + 4, version);
}

void CImPdu::SetFlag(uint16_t flag)
{
	uchar_t* buf = GetBuffer();
	CByteStream::WriteUint16(buf + 6, flag);
}

void CImPdu::SetServiceId(uint16_t serviceId)
{
    uchar_t* buf = GetBuffer();
    CByteStream::WriteUint16(buf + 8, serviceId);
}

void CImPdu::SetCommandId(uint16_t commandId)
{
    uchar_t* buf = GetBuffer();
    CByteStream::WriteUint16(buf + 10, commandId);
}

void CImPdu::SetError(uint16_t error)
{
    uchar_t* buf = GetBuffer();
    CByteStream::WriteUint16(buf + 12, error);
}

void CImPdu::SetSeqNum(uint16_t seqNum)
{
	uchar_t* buf = GetBuffer();
	CByteStream::WriteUint16(buf + 12, seqNum);
}

void CImPdu::SetReversed(uint32_t reversed)
{
    uchar_t* buf = GetBuffer();
    CByteStream::WriteUint16(buf+14, reversed);
}

int CImPdu::ReadPduHeader(uchar_t* buf, uint32_t len)
{
	int ret = -1;
	if (len >= IM_PDU_HEADER_LEN && buf) {
		CByteStream is(buf, len);

		is >> m_pduHeader.length;
		is >> m_pduHeader.version;
		is >> m_pduHeader.flag;
		is >> m_pduHeader.serviceId;
		is >> m_pduHeader.commandId;
		is >> m_pduHeader.seqNum;
        is >> m_pduHeader.reversed;

		ret = 0;
	}

	return ret;
}

CImPdu* CImPdu::ReadPdu(uchar_t *buf, uint32_t len)
{
	uint32_t pduLen = 0;
	if (!IsPduAvailable(buf, len, pduLen))
		return NULL;

	uint16_t serviceId = CByteStream::ReadUint16(buf + 8);
	uint16_t commandId = CByteStream::ReadUint16(buf + 10);
	CImPdu* pPdu = NULL;

    pPdu = new CImPdu();
    pPdu->Write(buf, pduLen);
    pPdu->ReadPduHeader(buf, IM_PDU_HEADER_LEN);

    return pPdu;
}

bool CImPdu::IsPduAvailable(uchar_t* buf, uint32_t len, uint32_t& pduLen)
{
	if (len < IM_PDU_HEADER_LEN)
		return false;

	pduLen = CByteStream::ReadUint32(buf);
	if (pduLen > len)
	{
		Logger.Log(INFO, "pduLen=%d, len=%d\n", pduLen, len);
		return false;
	}

    if(0 == pduLen)
    {
        throw CPduException(1, "pduLen is 0");
    }

	return true;
}

void CImPdu::SetPBMsg(const google::protobuf::MessageLite* msg)
{
    //设置包体，则需要重置下空间
    m_buf.Read(NULL, m_buf.GetWriteOffset());
    m_buf.Write(NULL, sizeof(PduHeader_t));
    uint32_t msg_size = msg->ByteSize();
    uchar_t* szData = new uchar_t[msg_size];
    //ALLOC_FAIL_ASSERT(szData)
    if (!msg->SerializeToArray(szData, msg_size))
    {
    	Logger.Log(ERROR, "pb msg miss required fields.");
    }

    m_buf.Write(szData, msg_size);
    delete []szData;
    WriteHeader();
}


