#ifndef _LOG_H
#define _LOG_H

#include "Util.h"

enum LOG_LEVEL
{
    //the level value start from 0, and the level value is bigger, the level is lower.
    EMERG_LEVEL = 0x0001,
    ALERT_LEVEL = 0x0002,
    ERROR_LEVEL = 0x0004,
    NOTIC_LEVEL = 0x0008,
    INFO_LEVEL  = 0x0010,
    STAT_LEVEL  = 0x0020,
    DEBUG_LEVEL = 0x0040,
    ALL_LEVEL   = 0xFFFF
};

#define EMERG __FILE__,__LINE__,EMERG_LEVEL
#define ALERT __FILE__,__LINE__,ALERT_LEVEL
#define ERROR __FILE__,__LINE__,ERROR_LEVEL
#define NOTIC __FILE__,__LINE__,NOTIC_LEVEL
#define INFO  __FILE__,__LINE__,INFO_LEVEL
#define STAT  __FILE__,__LINE__,STAT_LEVEL
#define DEBUG __FILE__,__LINE__,DEBUG_LEVEL

#define MAX_LOG_BUFFER  2048
#define MAX_LOG_PATH    256

//30M
#define MAX_FILE_SIZE 30*1024*1024
#define DEFAULT_LOG_PATH    "./log/"
#define DEFAULT_STAT_PATH   "./stat/"


typedef struct
{
    LOG_LEVEL   eLevel;
    time_t      tTime;
    uint16_t      wLen;
    char        cBuffer[MAX_LOG_BUFFER];
}LOG_INFO;

/**
 * 日志操作类
 */
class CLogger
{
public:
    CLogger();
    ~CLogger();

    bool Init(uint32_t dwID, uint32_t dwIP, uint16_t wPort);
    void SetLogLevel(uint16_t wLevel, bool bSet);

    int  Log(const char *pFileName, unsigned int nLine, LOG_LEVEL eLevel, const char *pBuffer, ...);

protected:
    static void * ThreadCallback(void *pParam);
    bool ConnServer();
    int Write(LOG_INFO *pLogInfo);
    const char* GetLevelString(LOG_LEVEL eLevel);
    unsigned long getFileSize(const char *path);

private:
    uint16_t          m_wLevel;
    uint32_t          m_dwLogID;
    uint32_t          m_dwIP;
    uint16_t          m_wPort;
    int32_t           m_dwSockFd;

    CObjectPool<LOG_INFO>   m_oLogQ, m_oIdleQ;
    CSem            m_oSem;

    FILE*           m_pLogFile;
    uint32_t          m_dwIndexS;
    FILE*           m_pStatFile;
};

extern CLogger Logger;

#ifdef __CONNECT_SVR__
void stat_log(const char* pszFileName, const int nLine, const char* func, const char *pszInfo, ...);
#define statlog(fmt, args...) stat_log( __FILE__, __LINE__, __func__, fmt , ##args);
int init_stat_log(uint32_t dwid);
#endif

#endif

