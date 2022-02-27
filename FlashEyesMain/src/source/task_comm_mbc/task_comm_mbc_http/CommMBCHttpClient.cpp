#include "CommMBCHttpClient.h"
#if (_CONF_COMM_MBC_HTTP_CLIENT_ENABLED)
/////////////////////////////////////////////////
// INCLUDE
#include "timer_manager/TimerManager.h"
/////////////////////////////////////////////////
// PREPROCESSOR
//#define COMM_MBC_HTTP_CLIENT_CONSOLE_DEBUG_ENABLE
/////////////////////////////////////////////////
// DEFINE

/////////////////////////////////////////////////
// MARCO

/////////////////////////////////////////////////
// DATA TYPE (TYPEDEF)

/////////////////////////////////////////////////
// DATA TYPE (ENUM)

/////////////////////////////////////////////////
// DATA TYPE (STRUCT)

/////////////////////////////////////////////////
// GLOBAL VARIABLES

/////////////////////////////////////////////////
// GLOBAL FUNCTIONS

/////////////////////////////////////////////////
// STATIC DATA
#ifdef COMM_MBC_HTTP_CLIENT_CONSOLE_DEBUG_ENABLE
char commMbcHttpClientLogBuf[SYSTEM_CONSOLE_OUT_BUF_LEN];
#endif // COMM_MBC_HTTP_CLIENT_CONSOLE_DEBUG_ENABLE
/////////////////////////////////////////////////
// STATIC FUNCTIONS

/////////////////////////////////////////////////
// EXTERN

/////////////////////////////////////////////////
// CLASS IMPLEMENTAION
/*CommMBCHttpClient*/
CommMBCHttpClient::CommMBCHttpClient(void)
  : CommHttpClient()
  , comm_Mbc_Engine((byte)CommMBCDataType::CommMbcJsonData)
{

}

CommMBCHttpClient::~CommMBCHttpClient(void)
{
  
}

uint16_t CommMBCHttpClient::registerMessageId(uint8_t messageId, uint16_t maxRawSize)
{
  return this->comm_Mbc_Engine.registerMessageId(messageId, maxRawSize);
}


int CommMBCHttpClient::request(CommHttpClientRequestParamsTAG& params, HttpReqId_t& reqId)
{
  return CommHttpClient::request(params, reqId);
}

int CommMBCHttpClient::request(CommMBCHttpClientRequestParamsTAG& mbcParams, HttpReqId_t& reqId)
{
#ifdef COMM_MBC_HTTP_CLIENT_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(commMbcHttpClientLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mHtC] req %i", 0);
#endif // COMM_MBC_HTTP_CLIENT_CONSOLE_DEBUG_ENABLE
  int ret = 0;
  CommHttpClientRequestParamsTAG httpRequestParam = CommHttpClientRequestParamsTAG();
  BufferDataItem* cbArgData = NULL;
  BufferDataItem* mbcReqDataItem = NULL;
  CommMBCHttpClientArgTAG mbcArg = CommMBCHttpClientArgTAG();
  do
  {
    if (this->isStarted() == false)
    {
#ifdef COMM_MBC_HTTP_CLIENT_CONSOLE_DEBUG_ENABLE
      CONSOLE_LOG_BUF(commMbcHttpClientLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mHtC] req %i", -1);
#endif // COMM_MBC_HTTP_CLIENT_CONSOLE_DEBUG_ENABLE
      break;
    }

    reqId = 0;
    mbcReqDataItem = this->getCommData();
    cbArgData = this->getCommData();
    if ((mbcReqDataItem == NULL)
      || (cbArgData == NULL)
      )
    {
#ifdef COMM_MBC_HTTP_CLIENT_CONSOLE_DEBUG_ENABLE
      CONSOLE_LOG_BUF(commMbcHttpClientLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mHtC] req %i", -2);
#endif // COMM_MBC_HTTP_CLIENT_CONSOLE_DEBUG_ENABLE
      break;
    }
    // create http client param
    httpRequestParam.header.dataType = mbcParams.header.dataType;
    httpRequestParam.header.fileData = mbcParams.header.fileData;
    httpRequestParam.header.reqMethod = mbcParams.header.reqMethod;
    httpRequestParam.header.uri = mbcParams.header.uri;
    httpRequestParam.header.uriId = mbcParams.header.uriId;

    // convert
    if (this->comm_Mbc_Engine.isHeaderLess() != false)
    {
      mbcParams.reqMessageId = (MbcMessageId_t)CommMBCConstant::CommMBCMessageId::CommMBCHeadLess;
    }

    ret = this->comm_Mbc_Engine.encodeData(mbcParams.reqMessageId, (unsigned char*)mbcParams.body, mbcParams.bodyLen, mbcReqDataItem);
    if (ret != 0)
    {
#ifdef COMM_MBC_HTTP_CLIENT_CONSOLE_DEBUG_ENABLE
      CONSOLE_LOG_BUF(commMbcHttpClientLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mHtC] req %i", -3);
#endif // COMM_MBC_HTTP_CLIENT_CONSOLE_DEBUG_ENABLE
      break;
    }
    httpRequestParam.body = (char*)mbcReqDataItem->bufferAddress();
    httpRequestParam.bodyLen = mbcReqDataItem->dataLength();

    // set arg
    mbcArg.mbcHandler = this;
    mbcArg.reqMessageId = mbcParams.reqMessageId;
    memcpy(&mbcArg.mbcNotifier, &mbcParams.notifier, sizeof(CommMBCHttpNotifierTAG));
    ret = cbArgData->setData(sizeof(CommMBCHttpClientArgTAG), (unsigned char*)&mbcArg);
    if (ret != 0)
    {
#ifdef COMM_MBC_HTTP_CLIENT_CONSOLE_DEBUG_ENABLE
      CONSOLE_LOG_BUF(commMbcHttpClientLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mHtC] req %i", -4);
#endif // COMM_MBC_HTTP_CLIENT_CONSOLE_DEBUG_ENABLE
      break;
    }
    httpRequestParam.notifier.agr = cbArgData;
    httpRequestParam.notifier.callback = CommMBCHttpClient::cbHttpClientResponse;
    httpRequestParam.notifier.expiredTime = mbcParams.notifier.expiredTime;
    // request
    ret = CommHttpClient::request(httpRequestParam, reqId);
#ifdef COMM_MBC_HTTP_CLIENT_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(commMbcHttpClientLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[%s]: %i %d", "set", 97, reqId);
#endif // COMM_MBC_HTTP_CLIENT_CONSOLE_DEBUG_ENABLE
    if (ret != 0)
    {
#ifdef COMM_MBC_HTTP_CLIENT_CONSOLE_DEBUG_ENABLE
      CONSOLE_LOG_BUF(commMbcHttpClientLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mHtC] req %i", -5);
#endif // COMM_MBC_HTTP_CLIENT_CONSOLE_DEBUG_ENABLE
      break;
    }
    //this->releaseCommData(cbArgData);
    this->releaseCommData(mbcReqDataItem);
#ifdef COMM_MBC_HTTP_CLIENT_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(commMbcHttpClientLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mHtC] req %i", 99);
#endif // COMM_MBC_HTTP_CLIENT_CONSOLE_DEBUG_ENABLE
    return 0;
  } while (0);

  if (cbArgData != NULL)
  {
    this->releaseCommData(cbArgData);
  }
  if (mbcReqDataItem != NULL)
  {
    this->releaseCommData(mbcReqDataItem);
  }
#ifdef COMM_MBC_HTTP_CLIENT_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(commMbcHttpClientLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mHtC] req %i", -99);
#endif // COMM_MBC_HTTP_CLIENT_CONSOLE_DEBUG_ENABLE
  return -1;
}

int CommMBCHttpClient::startTask(CommMBCHttpTaskConfigTAG& commMbcHttpConfig)
{
  int result = 0;
#ifdef COMM_MBC_HTTP_CLIENT_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(commMbcHttpClientLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mHtC] st %i", 0);
#endif // COMM_MBC_HTTP_CLIENT_CONSOLE_DEBUG_ENABLE
  do
  {
    if (this->isTaskRunning() != false)
    {
      return 0; // already running
    }
    
    result = this->comm_Mbc_Engine.prepare();
    if (result != 0)
    {
#ifdef COMM_MBC_HTTP_CLIENT_CONSOLE_DEBUG_ENABLE
      CONSOLE_LOG_BUF(commMbcHttpClientLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mHtC] st %i", -1);
#endif // COMM_MBC_HTTP_CLIENT_CONSOLE_DEBUG_ENABLE
      break;
    }
    
    CommHttpTaskConfigTAG httpTaskConfig = CommHttpTaskConfigTAG();
    memcpy(&httpTaskConfig.bufferConfig, &commMbcHttpConfig.bufferConfig, sizeof(BufferDataManagerConfigTAG));
    httpTaskConfig.bufferConfig.dataItemConfig.bufferSize = this->comm_Mbc_Engine.maxBufferSize();
    memcpy(&httpTaskConfig.taskManagerConfig, &commMbcHttpConfig.taskManagerConfig, sizeof(TaskManagerConfigTAG));
    memcpy(&httpTaskConfig.taskThreadConfig, &commMbcHttpConfig.taskThreadConfig, sizeof(TaskThreadConfigTAG));

#ifdef COMM_MBC_HTTP_CLIENT_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(commMbcHttpClientLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mHtC] st %i %i", 10, httpTaskConfig.bufferConfig.preparedDataNumber);
#endif // COMM_MBC_HTTP_CLIENT_CONSOLE_DEBUG_ENABLE
    result = CommHttpClient::startTask(httpTaskConfig);
#ifdef COMM_MBC_HTTP_CLIENT_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(commMbcHttpClientLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mHtC] st %i %i", 11, result);
#endif // COMM_MBC_HTTP_CLIENT_CONSOLE_DEBUG_ENABLE
    if (result != 0)
    {
      break;
    }
#ifdef COMM_MBC_HTTP_CLIENT_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(commMbcHttpClientLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mHtC] st %i", 99);
#endif // COMM_MBC_HTTP_CLIENT_CONSOLE_DEBUG_ENABLE
    return 0;
  } while (0);

#ifdef COMM_MBC_HTTP_CLIENT_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(commMbcHttpClientLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mHtC] st %i", -99);
#endif // COMM_MBC_HTTP_CLIENT_CONSOLE_DEBUG_ENABLE
  this->stopTask();
  return -1;
}

//// hide from public
//int CommMBCHttpClient::startTask(CommHttpTaskConfigTAG& commHttpConfig)
//{
//  return CommHttpClient::startTask(commHttpConfig);
//}
// hide from public
int CommMBCHttpClient::startHttp(CommHttpConnectionConfigTAG& commHttpConnConfig, bool waitCompletion)
{
  return CommHttpClient::startHttp(commHttpConnConfig, waitCompletion);
}

int CommMBCHttpClient::startHttp(CommMBCHttpConnectionConfigTAG& commMbcHttpConnConfig, bool waitCompletion)
{
  CommHttpConnectionConfigTAG httpConnConfig = CommHttpConnectionConfigTAG();
  if (sizeof(CommMBCHttpConnectionConfigTAG) != sizeof(CommHttpConnectionConfigTAG))
  {
    return -1;
  }
  memcpy(&httpConnConfig, &commMbcHttpConnConfig, sizeof(CommMBCHttpConnectionConfigTAG));
  return CommHttpClient::startHttp(httpConnConfig, waitCompletion);
}

void CommMBCHttpClient::cleanUp(void)
{
  this->comm_Mbc_Engine.clear();
  CommHttpClient::cleanUp();
}

void CommMBCHttpClient::regEventSize(void)
{
  this->registerHanldingEventStructSize(sizeof(EventCommMBCHttpClientRequestTAG));
  this->registerHanldingEventStructSize(sizeof(EventCommMBCHttpConnectionOpenedTAG));
  this->registerHanldingEventStructSize(sizeof(EventCommMBCHttpConnectionClosedTAG));
  this->registerHanldingEventStructSize(sizeof(EventCommMBCHttpFinishedTAG));
}

// hide from public
int CommMBCHttpClient::inititialize(void)
{
  return CommHttpClient::inititialize();
}

int CommMBCHttpClient::inititialize(CommMBCSettingParamTAG mbcSetting)
{
  int result = 0;
  do
  {
    if (mbcSetting.processorType != CommMBCDataType::CommMbcJsonData)
    {
      break; // unsupported
    }

    result = this->comm_Mbc_Engine.setConfig(mbcSetting);
    if (result != 0)
    {
      break;
    }
    return 0;
  } while (0);
  return -1;
}


int CommMBCHttpClient::onEventCommHttpStart(unsigned char* data, unsigned int dataSize)
{
#ifdef COMM_MBC_HTTP_CLIENT_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(commMbcHttpClientLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mHtC] eSta %i", 0);
#endif // COMM_MBC_HTTP_CLIENT_CONSOLE_DEBUG_ENABLE
  int ret = 0;
  do
  {
    ret = this->comm_Mbc_Engine.start();
    if (ret != 0)
    {
      break;
    }
    return CommHttpClient::onEventCommHttpStart(data, dataSize);
  } while (0);
#ifdef COMM_MBC_HTTP_CLIENT_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(commMbcHttpClientLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mHtC] eSta %i", -99);
#endif // COMM_MBC_HTTP_CLIENT_CONSOLE_DEBUG_ENABLE
  return -1;
}

int CommMBCHttpClient::onEventCommHttpStop(unsigned char* data, unsigned int dataSize)
{
  this->comm_Mbc_Engine.clear();
  return CommHttpClient::onEventCommHttpStop(data, dataSize);
}

int CommMBCHttpClient::cbHttpClientResponse(void* arg, CommHttpUriRequestTAG* httpUriRequest)
{
  CommHttpPackageTAG* httpResPackage = &httpUriRequest->resData;
  BufferDataItem* mbcArgBuf = (BufferDataItem*)arg;
  CommMBCHttpClientArgTAG* mbcArg = NULL;
  CommMBCHttpClient* mbcHandler = NULL;
  CommMBCHttpUriRequestTAG mbcUriRequest = CommMBCHttpUriRequestTAG();
  DataSize_t mbcDecodedLen = 0;
  int ret = 0;
#ifdef COMM_MBC_HTTP_CLIENT_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(commMbcHttpClientLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mHtC] cbClRes %d", 0);
#endif // COMM_MBC_HTTP_CLIENT_CONSOLE_DEBUG_ENABLE
  do
  {
    if (arg == NULL)
    {
      break;
    }

    // get info
    mbcArg = (CommMBCHttpClientArgTAG*)mbcArgBuf->bufferAddress();;
    mbcHandler = (CommMBCHttpClient*)mbcArg->mbcHandler;
    if (mbcHandler == NULL)
    {
      break;
    }

    // ****************req******************
    // copy req info
    mbcUriRequest.requestId = httpUriRequest->requestId;
    memcpy(&mbcUriRequest.resNoti, &mbcArg->mbcNotifier, sizeof(CommMBCHttpNotifierTAG));
    mbcUriRequest.reqMessageId = mbcArg->reqMessageId;

    // release arg buff/
    mbcHandler->releaseCommData(mbcArgBuf);
    mbcArgBuf = NULL; // avoid being re-released

    // copy req info
    ret = mbcUriRequest.reqData.setData(mbcHandler->dataManager());
    if (ret != 0)
    {
      break;
    }
    memcpy(mbcUriRequest.reqData.header(), httpUriRequest->reqData.header(), httpUriRequest->reqData.headerSize());
    memcpy(mbcUriRequest.reqData.body(), httpUriRequest->reqData.body(), httpUriRequest->reqData.bodyLen());
    mbcUriRequest.reqData.bodyLen(httpUriRequest->reqData.bodyLen());

    // ****************res******************
    ret = mbcUriRequest.resData.setData(mbcHandler->dataManager());
    if (ret != 0)
    {
      break;
    }

    // set header
    memcpy(mbcUriRequest.resData.header(), httpUriRequest->resData.header(), httpUriRequest->resData.headerSize());

#ifdef COMM_MBC_HTTP_CLIENT_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(commMbcHttpClientLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mHtC] cb %i rId=%d ", 1, httpUriRequest->requestId);
#endif // COMM_MBC_HTTP_CLIENT_CONSOLE_DEBUG_ENABLE

    // set body
    // convert
    ret = mbcHandler->comm_Mbc_Engine.decodeData(
      mbcHandler->comm_Mbc_Engine.isHeaderLess()
      , (unsigned char*)httpResPackage->body()
      , httpResPackage->bodyLen()
      , mbcUriRequest.resMessageId
      , (unsigned char*)mbcUriRequest.resData.body()
      , mbcUriRequest.resData.bodyMaxLen()
      , mbcDecodedLen
    );

    if (ret != 0)
    {
      break;
    }

    mbcUriRequest.resData.bodyLen(mbcDecodedLen);
    // release http package
    httpResPackage->releaseData();

    // check legacy
    if ((mbcUriRequest.resNoti.expiredTime > 0)
      && (TimerManager::getInstance().now() > mbcUriRequest.resNoti.expiredTime)
      )
    {
#ifdef COMM_MBC_HTTP_CLIENT_CONSOLE_DEBUG_ENABLE
      CONSOLE_LOG_BUF(commMbcHttpClientLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mmHtC] cbClRes %s", "went off");
#endif // COMM_MBC_HTTP_CLIENT_CONSOLE_DEBUG_ENABLE
      break; // went off.
    }
    // callback 
#ifdef COMM_MBC_HTTP_CLIENT_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(commMbcHttpClientLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mmHtC] cbClRes %d", 96);
#endif // COMM_MBC_HTTP_CLIENT_CONSOLE_DEBUG_ENABLE
    if (mbcUriRequest.resNoti.callback != NULL)
    {
#ifdef COMM_MBC_HTTP_CLIENT_CONSOLE_DEBUG_ENABLE
      CONSOLE_LOG_BUF(commMbcHttpClientLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mmHtC] cbClRes %d", 97);
#endif // COMM_MBC_HTTP_CLIENT_CONSOLE_DEBUG_ENABLE
      mbcUriRequest.resNoti.callback(mbcUriRequest.resNoti.agr, &mbcUriRequest);
    }
#ifdef COMM_MBC_HTTP_CLIENT_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(commMbcHttpClientLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mmHtC] cbClRes %d", 98);
#endif // COMM_MBC_HTTP_CLIENT_CONSOLE_DEBUG_ENABLE

    // release
    mbcUriRequest.reqData.releaseData();
    mbcUriRequest.resData.releaseData();
#ifdef COMM_MBC_HTTP_CLIENT_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(commMbcHttpClientLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mHtC] cbClRes %d", 99);
#endif // COMM_MBC_HTTP_CLIENT_CONSOLE_DEBUG_ENABLE
    return 0;
  } while (0);
  if ((mbcHandler != NULL)
    && (mbcArgBuf != NULL)
    )
  {
    mbcHandler->releaseCommData(mbcArgBuf);
  }

  httpResPackage->releaseData();
  mbcUriRequest.reqData.releaseData();
  mbcUriRequest.resData.releaseData();
#ifdef COMM_MBC_HTTP_CLIENT_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(commMbcHttpClientLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mHtC] cbClRes %d", -99);
#endif // COMM_MBC_HTTP_CLIENT_CONSOLE_DEBUG_ENABLE
  return -1;
}


#endif // _CONF_COMM_HTTP_CLIENT_ENABLED