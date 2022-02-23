/////////////////////////////////////////////////
// INCLUDE
#include "ExportHttpClientProcessor.h"
#if (_CONF_EXPORT_HTTP_CLIENT_PROCESSOR_ENABLED)

#include "utility/JsonParser.h"
/////////////////////////////////////////////////
// PREPROCESSOR
//#define EXPORT_HTTP_CLIENT_PROCESSOR_CONSOLE_DEBUG_ENABLE
/////////////////////////////////////////////////
// DEFINE

/////////////////////////////////////////////////
// MARCO

/////////////////////////////////////////////////
// GLOBAL VARIABLES
#ifdef EXPORT_HTTP_CLIENT_PROCESSOR_CONSOLE_DEBUG_ENABLE
char exportHttpClientProcessorLogBuf[SYSTEM_CONSOLE_OUT_BUF_LEN];
#endif // EXPORT_HTTP_CLIENT_PROCESSOR_CONSOLE_DEBUG_ENABLE
/////////////////////////////////////////////////
// GLOBAL FUNCTIONS

/////////////////////////////////////////////////
// DATA TYPE (ENUM)

/////////////////////////////////////////////////
// DATA TYPE (STRUCT)

/////////////////////////////////////////////////
// STATIC DATA

/////////////////////////////////////////////////
// STATIC FUNCTIONS

/////////////////////////////////////////////////
// EXTERN

/////////////////////////////////////////////////
// CLASS IMPLEMENTAION

/* ExportHttpClientProcessor*/
ExportHttpClientProcessor::ExportHttpClientProcessor(void)
  : ExportProcessor(ExportProcessorTypeUN::ExportProcessorHttpClient)
  , http_Client(NULL)
{
  memset(&this->req_Param, 0, sizeof(ExportHttpClientProcessorReqParamTAG));
}

ExportHttpClientProcessor::~ExportHttpClientProcessor(void)
{
  this->stop();
}

bool ExportHttpClientProcessor::isValid(void)
{
  do
  {
    if (ExportProcessor::isValid() == false)
    {
      break;
    }

    if (this->http_Client == NULL)
    {
      break;
    }

    return true;
  } while (0);
  return false;
}

int ExportHttpClientProcessor::start(ExportProcessorConfigTAG& procConfig)
{
  int ret = 0;
#ifdef EXPORT_HTTP_CLIENT_PROCESSOR_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(exportHttpClientProcessorLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[expHCl] sta %d", 0);
#endif // EXPORT_HTTP_CLIENT_PROCESSOR_CONSOLE_DEBUG_ENABLE
  do
  {
    if (this->isValid() != false)
    {
      return 0; // already
    }

    this->stop();
#ifdef EXPORT_HTTP_CLIENT_PROCESSOR_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(exportHttpClientProcessorLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[expHCl] sta %d", 1);
#endif // EXPORT_HTTP_CLIENT_PROCESSOR_CONSOLE_DEBUG_ENABLE
    if (procConfig.spec.httpClient.client == NULL)
    {
      break;
    }
#ifdef EXPORT_HTTP_CLIENT_PROCESSOR_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(exportHttpClientProcessorLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[expHCl] sta %d", 98);
#endif // EXPORT_HTTP_CLIENT_PROCESSOR_CONSOLE_DEBUG_ENABLE
    this->http_Client = procConfig.spec.httpClient.client;
    memcpy(&this->req_Param, &procConfig.spec.httpClient.reqParam, sizeof(ExportHttpClientProcessorReqParamTAG));
#ifdef EXPORT_HTTP_CLIENT_PROCESSOR_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(exportHttpClientProcessorLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[expHCl] sta %d", 99);
#endif // EXPORT_HTTP_CLIENT_PROCESSOR_CONSOLE_DEBUG_ENABLE
    return 0;
  } while (0);
#ifdef EXPORT_HTTP_CLIENT_PROCESSOR_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(exportHttpClientProcessorLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[expHCl] sta %d", -98);
#endif // EXPORT_HTTP_CLIENT_PROCESSOR_CONSOLE_DEBUG_ENABLE
  this->stop();
#ifdef EXPORT_HTTP_CLIENT_PROCESSOR_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(exportHttpClientProcessorLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[expHCl] sta %d", -99);
#endif // EXPORT_HTTP_CLIENT_PROCESSOR_CONSOLE_DEBUG_ENABLE
  return -1;
}

void ExportHttpClientProcessor::stop(void)
{
  this->http_Client = NULL;
  memset(&this->req_Param, 0, sizeof(ExportHttpClientProcessorReqParamTAG));
}


int ExportHttpClientProcessor::exportScanResult(ExportScanResultTAG* param)
{
  BufferDataItem* bufferItem = NULL;
  int ret = 0;
  do
  {
#ifdef EXPORT_HTTP_CLIENT_PROCESSOR_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(exportHttpClientProcessorLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[expHCl] exSR %d", 0);
#endif // EXPORT_HTTP_CLIENT_PROCESSOR_CONSOLE_DEBUG_ENABLE
    if (this->isValid() == false)
    {
#ifdef EXPORT_HTTP_CLIENT_PROCESSOR_CONSOLE_DEBUG_ENABLE
      CONSOLE_LOG_BUF(exportHttpClientProcessorLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[expHCl] exSR %d", -1);
#endif // EXPORT_HTTP_CLIENT_PROCESSOR_CONSOLE_DEBUG_ENABLE
      break;
    }

    bufferItem = this->http_Client->getCommData();
    if (bufferItem == NULL)
    {
#ifdef EXPORT_HTTP_CLIENT_PROCESSOR_CONSOLE_DEBUG_ENABLE
      CONSOLE_LOG_BUF(exportHttpClientProcessorLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[expHCl] exSR %d", -2);
#endif // EXPORT_HTTP_CLIENT_PROCESSOR_CONSOLE_DEBUG_ENABLE
      break;
    }

    char* body = (char*)bufferItem->bufferAddress();
    int printedLen = JsonSnprintf((char*)body, bufferItem->bufferLength(),
      "{\"seqId\":%d,\"sIdx\":%d,\"code\":{\"len\":%d,\"type\":%d,\"code\":%Q}}"
      , param->sequenceId
      , param->scanIndex
      , param->code.codeLen
      , param->code.type
      , param->code.code
    );
    
    if ((printedLen >= bufferItem->bufferLength())
      || (printedLen < 0)
      )
    {
#ifdef EXPORT_HTTP_CLIENT_PROCESSOR_CONSOLE_DEBUG_ENABLE
      CONSOLE_LOG_BUF(exportHttpClientProcessorLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[expHCl] exSR %d", -3);
#endif // EXPORT_HTTP_CLIENT_PROCESSOR_CONSOLE_DEBUG_ENABLE
      break;
    }

    HttpReqId_t reqId = 0;
    CommMBCHttpClientRequestParamsTAG reqParams = CommMBCHttpClientRequestParamsTAG();
    reqParams.reqMessageId = CommMBCConstant::CommMBCMessageId::CommMBCHeadLess;

    reqParams.header.dataType = COMM_HTTP_DATA_TYPE_APP_JSON;
    reqParams.header.fileData = 0;
    reqParams.header.reqMethod = this->req_Param.reqMethod;
    reqParams.header.uri = this->req_Param.reqUri;
    
    reqParams.notifier.expiredTime = 0;
    reqParams.notifier.callback = NULL;
    reqParams.notifier.agr = NULL;

    reqParams.body = (char*)body;
    reqParams.bodyLen = printedLen;

    ret = this->http_Client->request(reqParams, reqId);
#ifdef EXPORT_HTTP_CLIENT_PROCESSOR_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(exportHttpClientProcessorLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[jmdP] sRe %d %s", 40, param->code.code);
#endif // EXPORT_HTTP_CLIENT_PROCESSOR_CONSOLE_DEBUG_ENABLE
    if (ret != 0)
    {
      break;
    }

    this->http_Client->releaseCommData(bufferItem);
#ifdef EXPORT_HTTP_CLIENT_PROCESSOR_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(exportHttpClientProcessorLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[expHCl] exSR %d", 99);
#endif // EXPORT_HTTP_CLIENT_PROCESSOR_CONSOLE_DEBUG_ENABLE
    return 0;
  } while (0);

#ifdef EXPORT_HTTP_CLIENT_PROCESSOR_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(exportHttpClientProcessorLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[expHCl] exSR %d", -98);
#endif // EXPORT_HTTP_CLIENT_PROCESSOR_CONSOLE_DEBUG_ENABLE
  if (bufferItem != NULL)
  {
    this->http_Client->releaseCommData(bufferItem);
  }
#ifdef EXPORT_HTTP_CLIENT_PROCESSOR_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(exportHttpClientProcessorLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[expHCl] exSR %d", -99);
#endif // EXPORT_HTTP_CLIENT_PROCESSOR_CONSOLE_DEBUG_ENABLE
  return -1;
}


#endif // _CONF_EXPORT_HTTP_CLIENT_PROCESSOR_ENABLED