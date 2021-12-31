#include "CommMBCHttpServer.h"
#if (_CONF_COMM_HTTP_SERVER_ENABLED)
/////////////////////////////////////////////////
// INCLUDE

/////////////////////////////////////////////////
// PREPROCESSOR
#define COMM_MBC_HTTP_SERVER_CONSOLE_DEBUG_ENABLE
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
#ifdef COMM_MBC_HTTP_SERVER_CONSOLE_DEBUG_ENABLE
char commMbcHttpServerLogBuf[SYSTEM_CONSOLE_OUT_BUF_LEN];
#endif // COMM_MBC_HTTP_SERVER_CONSOLE_DEBUG_ENABLE
/////////////////////////////////////////////////
// STATIC FUNCTIONS

/////////////////////////////////////////////////
// EXTERN

/////////////////////////////////////////////////
// CLASS IMPLEMENTAION
/*CommMBCHttpServer*/
CommMBCHttpServer::CommMBCHttpServer(void)
  : CommHttpServer()
  , comm_Mbc_Engine((byte)CommMBCDataType::CommMbcJsonData)
{

}

CommMBCHttpServer::~CommMBCHttpServer(void)
{
  
}

uint16_t CommMBCHttpServer::registerMessageId(uint8_t messageId, uint16_t maxRawSize)
{
  return this->comm_Mbc_Engine.registerMessageId(messageId, maxRawSize);
}

int CommMBCHttpServer::regDefaultUriHandler(void)
{
  int ret = 0;
  do
  {
    CommHttpUriInfoTAG defaultUriHandler = CommHttpUriInfoTAG();
    defaultUriHandler.id = COMM_HTTP_DEFAULT_URI_ID;
    defaultUriHandler.uri = COMM_HTTP_DEFAULT_URI;
    defaultUriHandler.method = COMM_HTTP_METHOD_GET;
    defaultUriHandler.dataType = COMM_HTTP_DATA_TYPE_TEXT_HTML;
    defaultUriHandler.reqCallback = (void*)CommHttpServer::cbCommHttpRequestDefaultUriHandlerNavigator;
    defaultUriHandler.userArg = this;
    ret = CommHttpServer::regUriHandler(defaultUriHandler);
    if (ret != 0)
    {
      break;
    }

    return 0;
  } while (0);
  return -1;
}

//// hide from public
//int CommMBCHttpServer::regUriHandler(CommHttpUriInfoTAG& uriInfo)
//{
//  return CommHttpServer::regUriHandler(uriInfo);
//}

int CommMBCHttpServer::regUriHandler(CommMBCHttpUriInfoTAG& uriInfo)
{
  int ret = 0;
  CommHttpUriInfoTAG httpUriInfo = CommHttpUriInfoTAG();
  BufferDataItem* mbcArgBuf = NULL;
  CommMBCHttpServerArgTAG mbcArg = CommMBCHttpServerArgTAG();
  do
  {
    if (uriInfo.isValid() == false)
    {
      break;
    }

    mbcArgBuf = this->getCommData();
    if (mbcArgBuf == NULL)
    {
      break;
    }
    memcpy(&httpUriInfo, &uriInfo, sizeof(CommHttpUriInfoTAG));
    httpUriInfo.serverHandler = this;
    httpUriInfo.reqCallback = (void*)CommMBCHttpServer::cbCommHttpUriHandler;
    httpUriInfo.userArg = mbcArgBuf;

    mbcArg.mbcHandler = this;
    mbcArg.mbcArg = uriInfo.userArg;
    mbcArg.reqCallback = (CommMBCHttpCbOnUriResponse)uriInfo.reqCallback;
    ret = mbcArgBuf->setData(sizeof(CommMBCHttpServerArgTAG), (unsigned char*)&mbcArg);
    if (ret != 0)
    {
      break;
    }

    ret = CommHttpServer::regUriHandler(httpUriInfo);
    if (ret != 0)
    {
      break;
    }
    return 0;
  } while (0);

  if (mbcArgBuf != NULL)
  {
    this->releaseCommData(mbcArgBuf);
  }
  return -1;
}


// hide from public
int CommMBCHttpServer::inititialize(void)
{
  return CommMBCHttpServer::inititialize();
}


int CommMBCHttpServer::inititialize(CommMBCSettingParamTAG mbcSetting)
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

int CommMBCHttpServer::startTask(CommMBCHttpTaskConfigTAG& commMbcHttpConfig)
{
  int result = 0;
  do
  {
    if (this->isTaskRunning() != false)
    {
      return 0; // already running
    }

    result = this->comm_Mbc_Engine.prepare();
    if (result != 0)
    {
      break;
    }

    CommHttpTaskConfigTAG httpTaskConfig = CommHttpTaskConfigTAG();
    memcpy(&httpTaskConfig.bufferConfig, &commMbcHttpConfig.bufferConfig, sizeof(BufferDataManagerConfigTAG));
    httpTaskConfig.bufferConfig.dataItemConfig.bufferSize = this->comm_Mbc_Engine.maxBufferSize();
    memcpy(&httpTaskConfig.taskManagerConfig, &commMbcHttpConfig.taskManagerConfig, sizeof(TaskManagerConfigTAG));
    memcpy(&httpTaskConfig.taskThreadConfig, &commMbcHttpConfig.taskThreadConfig, sizeof(TaskThreadConfigTAG));
    result =  CommHttpServer::startTask(httpTaskConfig);
    if (result != 0)
    {
      break;
    }
    return 0;
  } while (0);

  this->stopTask();
  return -1;
}

//// hide from public
//int CommMBCHttpServer::startTask(CommHttpTaskConfigTAG& commHttpConfig)
//{
//  return CommHttpServer::startTask(commHttpConfig);
//}
// hide from public
int CommMBCHttpServer::startHttp(CommHttpConnectionConfigTAG& commHttpConnConfig, bool waitCompletion)
{
  return CommHttpServer::startHttp(commHttpConnConfig, waitCompletion);
}

int CommMBCHttpServer::startHttp(CommMBCHttpConnectionConfigTAG& commMbcHttpConnConfig, bool waitCompletion)
{
  CommHttpConnectionConfigTAG httpConnConfig = CommHttpConnectionConfigTAG();
  httpConnConfig.config.client.certPem = commMbcHttpConnConfig.config.client.certPem;
  httpConnConfig.config.client.host = commMbcHttpConnConfig.config.client.host;
  httpConnConfig.config.client.port = commMbcHttpConnConfig.config.client.port;
  httpConnConfig.config.client.url = commMbcHttpConnConfig.config.client.url;
  return CommHttpServer::startHttp(httpConnConfig, waitCompletion);
}

int CommMBCHttpServer::setResponse(MbcMessageId_t messageId, unsigned char* inputBuffer, MbcDataSize_t inputSize, unsigned char* outBuffer, MbcDataSize_t outSize, MbcDataSize_t& encodedSize)
{
  return this->comm_Mbc_Engine.encodeData(messageId, inputBuffer, inputSize, outBuffer, outSize, encodedSize);
}

void CommMBCHttpServer::cleanUp(void)
{
  this->comm_Mbc_Engine.clear();
  CommHttpServer::cleanUp();
}

void CommMBCHttpServer::regEventSize(void)
{
  this->registerHanldingEventStructSize(sizeof(EventCommMBCHttpDataReceivedTAG));
}

int CommMBCHttpServer::onEventCommHttpStart(unsigned char* data, unsigned int dataSize)
{
  int ret = 0;
  do
  {
    ret = this->comm_Mbc_Engine.start();
    if (ret != 0)
    {
      break;
    }
    return CommHttpServer::onEventCommHttpStart(data, dataSize);
  } while (0);
  return -1;
}

int CommMBCHttpServer::onEventCommHttpStop(unsigned char* data, unsigned int dataSize)
{
  this->comm_Mbc_Engine.clear();
  return CommHttpServer::onEventCommHttpStop(data, dataSize);
}


int CommMBCHttpServer::cbCommHttpUriHandler(void* arg, CommHttpUriRequestTAG* httpUriReqInfo)
{
  int ret = 0;
  CommMBCHttpUriRequestTAG mbcUriReqInfo = CommMBCHttpUriRequestTAG();
  BufferDataItem* mbcArgBuf = (BufferDataItem*)arg; // dont release this. It is shared among requests from same reg uri.
  CommMBCHttpServer* mbcHandler = NULL;
  CommMBCHttpServerArgTAG* mbcArg = NULL;
  //MbcDataSize_t encodedSize = 0;
  MbcDataSize_t decodedSize = 0;
#ifdef COMM_MBC_HTTP_SERVER_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(commMbcHttpServerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mHtS] cUH %i", 0);
#endif // COMM_MBC_HTTP_SERVER_CONSOLE_DEBUG_ENABLE
  do
  {
    if (mbcArgBuf == NULL)
    {
      break;
    }
    mbcArg = (CommMBCHttpServerArgTAG*)mbcArgBuf->bufferAddress();
    mbcHandler = (CommMBCHttpServer*)mbcArg->mbcHandler;

    // copy info to req
    mbcUriReqInfo.requestId = httpUriReqInfo->requestId;
    memcpy(&mbcUriReqInfo.resNoti, &httpUriReqInfo->resNoti, sizeof(CommHttpNotifierTAG));
    ret = mbcUriReqInfo.reqData.setData(mbcHandler->dataManager());
    if (ret != 0)
    {
      break;
    }

    // copy req header info
    if (mbcUriReqInfo.reqData.headerSize() != httpUriReqInfo->reqData.headerSize())
    {
      break;
    }
    memcpy(mbcUriReqInfo.reqData.header(), httpUriReqInfo->reqData.header(), httpUriReqInfo->reqData.headerSize());

    // decode
    ret = mbcHandler->comm_Mbc_Engine.decodeData(
      mbcHandler->comm_Mbc_Engine.isHeaderLess()
      , (unsigned char*)httpUriReqInfo->reqData.body()
      , httpUriReqInfo->reqData.bodyLen()
      , mbcUriReqInfo.reqMessageId
      , (unsigned char*)mbcUriReqInfo.reqData.body()
      , mbcUriReqInfo.reqData.bodyMaxLen()
      , decodedSize
    );

    if (ret != 0)
    {
      break;
    }
    mbcUriReqInfo.reqData.bodyLen(decodedSize);
    httpUriReqInfo->reqData.releaseData();

    mbcUriReqInfo.resMessageId = CommMBCConstant::CommMBCMessageId::CommMBCMessageIdInvalid;
    // navigate to upper side
    ret = mbcArg->reqCallback(mbcArg->mbcArg, &mbcUriReqInfo);
    if (ret != 0)
    {
      break;
    }
    mbcUriReqInfo.reqData.releaseData();

    // prepare res buf for http layer
    ret = httpUriReqInfo->resData.setData(mbcHandler->dataManager());
    if (ret != 0)
    {
      break;
    }

    // encode response
    // upper side need to call setResponse() to encode
    //set header
    memcpy(httpUriReqInfo->resData.header(), mbcUriReqInfo.resData.header(), mbcUriReqInfo.resData.headerSize());

    // set body
    if (mbcUriReqInfo.resData.bodyLen() > httpUriReqInfo->resData.bodyMaxLen())
    {
      break;
    }

    memcpy(httpUriReqInfo->resData.body(), mbcUriReqInfo.resData.body(), mbcUriReqInfo.resData.bodyLen());
    httpUriReqInfo->resData.bodyLen(mbcUriReqInfo.resData.bodyLen());
    
    // release mbc
    mbcUriReqInfo.resData.releaseData();
#ifdef COMM_MBC_HTTP_SERVER_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(commMbcHttpServerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mHtS] cUH %i", 99);
#endif // COMM_MBC_HTTP_SERVER_CONSOLE_DEBUG_ENABLE
    return 0;
  } while (0);
  httpUriReqInfo->reqData.releaseData();
  mbcUriReqInfo.reqData.releaseData();
  mbcUriReqInfo.resData.releaseData();
#ifdef COMM_MBC_HTTP_SERVER_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(commMbcHttpServerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mHtS] cUH %i", -99);
#endif // COMM_MBC_HTTP_SERVER_CONSOLE_DEBUG_ENABLE
  return -1;
}
#endif // _CONF_COMM_HTTP_SERVER_ENABLED