#include "CommHttpServer.h"
#if (_CONF_COMM_HTTP_SERVER_ENABLED)
/////////////////////////////////////////////////
// INCLUDE
#include "../file_system/FileSystemManager.h"
#include "timer_manager/TimerManager.h"
/////////////////////////////////////////////////
// PREPROCESSOR
#define COMM_HTTP_SERVER_CONSOLE_DEBUG_ENABLE
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
#ifdef COMM_HTTP_SERVER_CONSOLE_DEBUG_ENABLE
char commHttpServerLogBuf[SYSTEM_CONSOLE_OUT_BUF_LEN];
#endif // COMM_HTTP_SERVER_CONSOLE_DEBUG_ENABLE
/////////////////////////////////////////////////
// STATIC FUNCTIONS

/////////////////////////////////////////////////
// EXTERN

/////////////////////////////////////////////////
// CLASS IMPLEMENTAION
/*CommHttpServer*/
CommHttpServer::CommHttpServer(void)
  : CommHttp(false)
  , http_Handler(HTTP_HANDLER_INVALID)
//  , http_Url(NULL)
{
  memset(this->base_Path, 0, COMM_HTTP_SERVER_BASE_PATH_MAX_LEN + 1);
}

CommHttpServer::~CommHttpServer(void)
{
  
}

const char* CommHttpServer::basePath(void)
{
  return this->base_Path;
}

void CommHttpServer::regEventSize(void)
{
  this->registerHanldingEventStructSize(sizeof(EventCommHttpDataReceivedTAG));
}

int CommHttpServer::regUriHandler(CommHttpUriInfoTAG& uriInfo)
{
  HttpRet_t httpRet = HTTP_RET_OK;
  do
  {
//#ifdef COMM_HTTP_SERVER_CONSOLE_DEBUG_ENABLE
//    CONSOLE_LOG_BUF(commHttpServerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN,"[HtSTsk] reg %i", 0);
//#endif // COMM_HTTP_SERVER_CONSOLE_DEBUG_ENABLE
    if (uriInfo.isValid() == false)
    {
#ifdef COMM_HTTP_SERVER_CONSOLE_DEBUG_ENABLE
      CONSOLE_LOG_BUF(commHttpServerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[HtSTsk] reg %i", -1);
#endif // COMM_HTTP_SERVER_CONSOLE_DEBUG_ENABLE

      break;
    }

    httpd_uri_t drvUriConfig = httpd_uri_t();
    CommHttpUriInfoTAG* savedUriInfo = NULL;
    savedUriInfo = this->uri_List.append();
    if (savedUriInfo == NULL)
    {
#ifdef COMM_HTTP_SERVER_CONSOLE_DEBUG_ENABLE
      CONSOLE_LOG_BUF(commHttpServerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[HtSTsk] reg %i", -2);
#endif // COMM_HTTP_SERVER_CONSOLE_DEBUG_ENABLE
      break;
    }

    memcpy(savedUriInfo, &uriInfo, sizeof(CommHttpUriInfoTAG));
    savedUriInfo->serverHandler = this;
//#ifdef COMM_HTTP_SERVER_CONSOLE_DEBUG_ENABLE
//    CONSOLE_LOG_BUF(commHttpServerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[HtSTsk] reg %i %d %d", 3, savedUriInfo, this);
//    CONSOLE_LOG_BUF(commHttpServerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[HtSTsk] reg %i %d", 3, savedUriInfo->serverHandler);
//#endif // COMM_HTTP_SERVER_CONSOLE_DEBUG_ENABLE
    drvUriConfig.uri = uriInfo.uri;
    drvUriConfig.method = COMM_HTTP_SERVER_METHOD_TBL[uriInfo.method];
    drvUriConfig.handler = cbCommHttpRequestHandlerNavigator;
    drvUriConfig.user_ctx = savedUriInfo;
    httpRet = httpd_register_uri_handler(this->http_Handler, &drvUriConfig);
//#ifdef COMM_HTTP_SERVER_CONSOLE_DEBUG_ENABLE
//    CONSOLE_LOG_BUF(commHttpServerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[HtSTsk] reg %i %i %i", 4, (int)httpRet, (int)drvUriConfig.method);
//    CONSOLE_LOG_BUF(commHttpServerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "%s", drvUriConfig.uri);
//#endif // COMM_HTTP_SERVER_CONSOLE_DEBUG_ENABLE

    if (httpRet != HTTP_RET_OK)
    {
      break;
    }

    return 0;
  } while (0);
#ifdef COMM_HTTP_SERVER_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(commHttpServerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[HtSTsk] reg %i", -99);
#endif // COMM_HTTP_SERVER_CONSOLE_DEBUG_ENABLE
  return -1;
}

void CommHttpServer::proc(void)
{
#ifdef COMM_HTTP_SERVER_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(commHttpServerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[HtSTsk] proc %i", 0);
#endif // COMM_HTTP_SERVER_CONSOLE_DEBUG_ENABLE
  int prepareRet = this->prepare();
  this->reportPrepareResult(prepareRet);
  if (prepareRet != 0)
  {
    this->waitTerminating();
    return;
  }
#ifdef COMM_HTTP_SERVER_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(commHttpServerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[HtSTsk] proc %i", 1);
#endif // COMM_HTTP_SERVER_CONSOLE_DEBUG_ENABLE
  EventDataItem* eventData = NULL;
  bool isStop = false;
  while (isStop == false) // A Task shall never return or exit.
  {
    eventData = this->event_Manager.wait(1000);
#ifdef COMM_HTTP_SERVER_CONSOLE_DEBUG_ENABLE
    //CONSOLE_LOG_BUF(commHttpServerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[HtSTsk] proc %i", 2);
#endif // COMM_HTTP_SERVER_CONSOLE_DEBUG_ENABLE
    if (eventData == NULL)
    {
      continue;
    }

    switch (eventData->messageId())
    {
    case (int)EventManagerConstant::EventMessageId::TerminateProcess:
    {
      this->isTaskRunning(false);
      isStop = true;
      break;
    }
    case (int)EventManagerConstant::EventMessageId::CommHttpStart:
    {
      this->onEventCommHttpStart(eventData->bufferAddress(), eventData->dataLength());
      break;
    }
    case (int)EventManagerConstant::EventMessageId::CommHttpDataReceived:
    {
      this->onEventCommHttpDataReceived(eventData->bufferAddress(), eventData->dataLength());
      break;
    }
    case (int)EventManagerConstant::EventMessageId::CommHttpStop:
    {
      this->onEventCommHttpStop(eventData->bufferAddress(), eventData->dataLength());
      break;
    }
    default:
      break;
    }
    this->event_Manager.release(eventData);
  }
#ifdef COMM_HTTP_SERVER_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(commHttpServerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[HtSTsk] stop %i", 99);
#endif // COMM_HTTP_SERVER_CONSOLE_DEBUG_ENABLE
}

int CommHttpServer::onEventCommHttpStart(unsigned char* data, unsigned int dataSize)
{
#ifdef COMM_HTTP_SERVER_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(commHttpServerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[HtSTsk] eSta %i", 0);
#endif // COMM_HTTP_SERVER_CONSOLE_DEBUG_ENABLE
  HttpRet_t httpRet = HTTP_RET_OK;
  int ret = 0;
  do
  {
    if ((dataSize != sizeof(EventCommHttpStartTAG))
      || (data == NULL))
    {
      break;
    }

    if (this->isStarted() != false)
    {
      break;
    }

    EventCommHttpStartTAG* eventData = (EventCommHttpStartTAG*)data;
    CommHttpServerConfigTAG* serverConfig = &eventData->config.server;

    if (this->uri_List.initialize(serverConfig->uriMaxCount) != 0)
    {
      break;
    }
#ifdef COMM_HTTP_SERVER_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(commHttpServerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[HtSTsk] eSta %i", 3);
#endif // COMM_HTTP_SERVER_CONSOLE_DEBUG_ENABLE
    if ((serverConfig->basePath == NULL)
      || (strlen(serverConfig->basePath) <= 0)
      || (strlen(serverConfig->basePath) > COMM_HTTP_SERVER_BASE_PATH_MAX_LEN)
      )
    {
      break;
    }

    strcpy(this->base_Path, serverConfig->basePath);

    httpd_config_t httpConfig = httpd_config_t();
    HTTP_SERVER_DEFAULT_CONFIG(httpConfig);
    httpConfig.uri_match_fn = httpd_uri_match_wildcard;
    httpConfig.server_port = serverConfig->port;
    httpConfig.max_uri_handlers = serverConfig->uriMaxCount;
    httpConfig.max_open_sockets = serverConfig->connMaxCount;
    httpConfig.max_resp_headers = serverConfig->addHeaderMaxCount;
    httpConfig.backlog_conn = serverConfig->backlogConn;
    httpRet = httpd_start(&this->http_Handler, &httpConfig);
#ifdef COMM_HTTP_SERVER_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(commHttpServerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[HtSTsk] eSta %i %i", 4, (int)httpRet);
    CONSOLE_LOG_BUF(commHttpServerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "%s", this->base_Path);
#endif // COMM_HTTP_SERVER_CONSOLE_DEBUG_ENABLE
    if (httpRet != HTTP_RET_OK)
    {
      break;
    }

    if (this->http_Handler == HTTP_HANDLER_INVALID)
    {
      break;
    }

    this->is_Started = 1;
    this->ret_Start = 0;
    this->mutex_wait_Start.unlock();
    return 0;
  } while (0);

  {
    this->ret_Start = -1;
    this->mutex_wait_Start.unlock();
  }
#ifdef COMM_HTTP_SERVER_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(commHttpServerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[HtSTsk] eSta %i", -99);
#endif // COMM_HTTP_SERVER_CONSOLE_DEBUG_ENABLE
  return -1;
}

int CommHttpServer::onEventCommHttpDataReceived(unsigned char* data, unsigned int dataSize)
{
  do
  {
    if ((dataSize != sizeof(EventCommHttpDataReceivedTAG))
      || (data == NULL))
    {
      break;
    }

#ifdef COMM_HTTP_SERVER_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(commHttpServerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[HtSTsk] ereved %i", 0);
#endif // COMM_HTTP_SERVER_CONSOLE_DEBUG_ENABLE

    return 0;
  } while (0);
  return -1;
}

int CommHttpServer::onEventCommHttpStop(unsigned char* data, unsigned int dataSize)
{
#ifdef COMM_HTTP_SERVER_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(commHttpServerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[HtSTsk] esto %i", 0);
#endif // COMM_HTTP_SERVER_CONSOLE_DEBUG_ENABLE
  //SystemMutex* waitMutex = NULL;
  do
  {
    if ((dataSize != sizeof(EventCommHttpStopTAG))
      || (data == NULL))
    {
      break;
    }

    if (this->isStarted() == false)
    {
      break;
    }

    EventCommHttpStopTAG* eventData = (EventCommHttpStopTAG*)data;
    this->uri_List.reset();
    memset(this->base_Path, 0, COMM_HTTP_SERVER_BASE_PATH_MAX_LEN + 1);
    if (this->http_Handler != HTTP_HANDLER_INVALID)
    {
      httpd_stop(this->http_Handler);
    }

    this->http_Handler = HTTP_HANDLER_INVALID;
    this->is_Started = 0;
    this->ret_Stop = 0;
    this->mutex_wait_Stop.unlock();
    return 0;
  } while (0);

  {
    this->ret_Stop = -1;
    this->mutex_wait_Stop.unlock();
  }
#ifdef COMM_HTTP_SERVER_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(commHttpServerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[HtSTsk] esto %i", -99);
#endif // COMM_HTTP_SERVER_CONSOLE_DEBUG_ENABLE
  return -1;
}

int CommHttpServer::sendResponseBuff(HttpReq_t* httpReq, char* buffer, DataSize_t bufferSize)
{
  HttpRet_t httpRet = HTTP_RET_OK;
  int ret = 0;
  do
  {
//#ifdef COMM_HTTP_SERVER_CONSOLE_DEBUG_ENABLE
//    CONSOLE_LOG_BUF(commHttpServerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[HtSTsk] resB %i %lu", 0, bufferSize);
//#endif // COMM_HTTP_SERVER_CONSOLE_DEBUG_ENABLE
    if (bufferSize < COMM_HTTP_PACKAGE_BUFF_CHUNK_SIZE)
    {
      httpRet = httpd_resp_send(httpReq, buffer, bufferSize);
//#ifdef COMM_HTTP_SERVER_CONSOLE_DEBUG_ENABLE
//      CONSOLE_LOG_BUF(commHttpServerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[HtSTsk] resB %i %i", 1, (int)httpRet);
//#endif // COMM_HTTP_SERVER_CONSOLE_DEBUG_ENABLE
      if (httpRet != HTTP_RET_OK)
      {
        break;
      }
      return 0;
    }

    DataSize_t remainingLen = bufferSize;
    DataSize_t lenToSend = 0;
    do
    {
      /* Read file in chunks into the scratch buffer */
      lenToSend = SYSTEM_MIN(remainingLen, COMM_HTTP_PACKAGE_BUFF_CHUNK_SIZE);
      /* Send the buffer contents as HTTP response chunk */
      httpRet = httpd_resp_send_chunk(httpReq, buffer, lenToSend);
      if (httpRet != HTTP_RET_OK)
      {
        // Abort sending chunk
        httpd_resp_send_chunk(httpReq, NULL, 0);
        break;
      }

      buffer += lenToSend;
      remainingLen -= lenToSend;
    } while (remainingLen > 0);

//#ifdef COMM_HTTP_SERVER_CONSOLE_DEBUG_ENABLE
//    CONSOLE_LOG_BUF(commHttpServerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[HtSTsk] resB %i %lu", 5, remainingLen);
//#endif // COMM_HTTP_SERVER_CONSOLE_DEBUG_ENABLE
    if (remainingLen > 0)
    {
      break;
    }
    // Signal HTTP response completion
    httpd_resp_send_chunk(httpReq, NULL, 0);
//#ifdef COMM_HTTP_SERVER_CONSOLE_DEBUG_ENABLE
//    CONSOLE_LOG_BUF(commHttpServerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[HtSTsk] resB %i", 99);
//#endif // COMM_HTTP_SERVER_CONSOLE_DEBUG_ENABLE
    return 0;
  } while (0);
#ifdef COMM_HTTP_SERVER_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(commHttpServerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[HtSTsk] resB %i", -99);
#endif // COMM_HTTP_SERVER_CONSOLE_DEBUG_ENABLE
  return -1;
}

int CommHttpServer::sendResponseFileData(HttpReq_t* httpReq, char* buffer, DataSize_t bufferSize)
{
  HttpRet_t httpRet = HTTP_RET_OK;
  int ret = 0;
  do
  {
//#ifdef COMM_HTTP_SERVER_CONSOLE_DEBUG_ENABLE
//    CONSOLE_LOG_BUF(commHttpServerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[HtSTsk] resF %i", 0);
//#endif // COMM_HTTP_SERVER_CONSOLE_DEBUG_ENABLE
    const char* filePath = buffer;
    FileHandler_t file = FileSystemManager::getInstance().openFile(filePath, "r");
    if (FileSystemManager::getInstance().isValidFileHandler(file) == false)
    {
      break;
    }

    DataSize_t lenToSend = 0;
    int error = 0;
    do
    {
      lenToSend = fread(buffer, 1, bufferSize, file);
      if (lenToSend <=0)
      {
        break;
      }

      httpRet = httpd_resp_send_chunk(httpReq, buffer, lenToSend);
      if (httpRet != HTTP_RET_OK)
      {
        error = -2;
        httpd_resp_sendstr_chunk(httpReq, NULL);
        break;
      }

    } while (lenToSend > 0);
    FileSystemManager::getInstance().closeFile(file);
    httpd_resp_send_chunk(httpReq, NULL, 0);
//#ifdef COMM_HTTP_SERVER_CONSOLE_DEBUG_ENABLE
//    CONSOLE_LOG_BUF(commHttpServerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[HtSTsk] resF %i %i", 8, error);
//#endif // COMM_HTTP_SERVER_CONSOLE_DEBUG_ENABLE
    if (error != 0)
    {
      break;
    }
//#ifdef COMM_HTTP_SERVER_CONSOLE_DEBUG_ENABLE
//    CONSOLE_LOG_BUF(commHttpServerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[HtSTsk] resF %i", 99);
//#endif // COMM_HTTP_SERVER_CONSOLE_DEBUG_ENABLE
    return 0;
  } while (0);
#ifdef COMM_HTTP_SERVER_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(commHttpServerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[HtSTsk] resF %i", -99);
#endif // COMM_HTTP_SERVER_CONSOLE_DEBUG_ENABLE
  return -1;
}

int CommHttpServer::sendResponse(HttpReq_t* httpReq, CommHttpPackageTAG& package)
{
  HttpRet_t httpRet = HTTP_RET_OK;
  int ret = 0;
  DataSize_t bodyLen;
  CommHttpHeaderTAG* header;
  char* body = NULL;
  do
  {
//#ifdef COMM_HTTP_SERVER_CONSOLE_DEBUG_ENABLE
//    CONSOLE_LOG_BUF(commHttpServerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[HtSTsk] res %i", 0);
//#endif // COMM_HTTP_SERVER_CONSOLE_DEBUG_ENABLE
    // set header type
    if (package.isValid() == false)
    {
#ifdef COMM_HTTP_SERVER_CONSOLE_DEBUG_ENABLE
      CONSOLE_LOG_BUF(commHttpServerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[HtSTsk] res %i", -1);
#endif // COMM_HTTP_SERVER_CONSOLE_DEBUG_ENABLE
      break;
    }
    header = package.header();;
    body = package.body();
    bodyLen = package.bodyLen();
    if (header->resStatusCode >= COMM_HTTP_STATUS_CODE_MAX)
    {
#ifdef COMM_HTTP_SERVER_CONSOLE_DEBUG_ENABLE
      CONSOLE_LOG_BUF(commHttpServerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[HtSTsk] res %i", -2);
#endif // COMM_HTTP_SERVER_CONSOLE_DEBUG_ENABLE
      break;
    }

    httpRet = httpd_resp_set_status(httpReq, COMM_HTTP_CODE_STR_TBL[header->resStatusCode]);
    if (httpRet != HTTP_RET_OK)
    {
#ifdef COMM_HTTP_SERVER_CONSOLE_DEBUG_ENABLE
      CONSOLE_LOG_BUF(commHttpServerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[HtSTsk] res %i", -3);
#endif // COMM_HTTP_SERVER_CONSOLE_DEBUG_ENABLE
      break;
    }

    if (bodyLen == 0)
    {
      httpd_resp_send(httpReq, NULL, 0);
      package.releaseData();
      return 0;
    }

    httpRet = httpd_resp_set_type(httpReq, COMM_HTTP_DATA_TYPE_STR_TBL[header->dataType]);
    if (httpRet != HTTP_RET_OK)
    {
#ifdef COMM_HTTP_SERVER_CONSOLE_DEBUG_ENABLE
      CONSOLE_LOG_BUF(commHttpServerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[HtSTsk] res %i", -5);
#endif // COMM_HTTP_SERVER_CONSOLE_DEBUG_ENABLE
      break;
    }

//#ifdef COMM_HTTP_SERVER_CONSOLE_DEBUG_ENABLE
//    CONSOLE_LOG_BUF(commHttpServerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[HtSTsk] res %i", 6);
//#endif // COMM_HTTP_SERVER_CONSOLE_DEBUG_ENABLE
    if (header->fileData == 0)
    {
      ret = this->sendResponseBuff(httpReq, body, bodyLen);
    }
    else
    {
      ret = this->sendResponseFileData(httpReq, body, package.bodyMaxLen());
    }

    if (ret != 0)
    {
      break;
    }
//#ifdef COMM_HTTP_SERVER_CONSOLE_DEBUG_ENABLE
//    CONSOLE_LOG_BUF(commHttpServerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[HtSTsk] res %i", 99);
//#endif // COMM_HTTP_SERVER_CONSOLE_DEBUG_ENABLE
    package.releaseData();
    return 0;
  } while (0);
  ret = -1;
#ifdef COMM_HTTP_SERVER_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(commHttpServerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[HtSTsk] res %i", -99);
#endif // COMM_HTTP_SERVER_CONSOLE_DEBUG_ENABLE
  package.releaseData();
  return ret;
}

int CommHttpServer::cbCommHttpDefaultUriHandler(CommHttpUriRequestTAG* uriReqInfo)
{
  int ret = 0;
  CommHttpPackageTAG* reqPackage = &uriReqInfo->reqData;
  CommHttpPackageTAG* resPackage = &uriReqInfo->resData;
#ifdef COMM_HTTP_SERVER_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(commHttpServerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[HtSTsk] cbDH %i", 0);
#endif // COMM_HTTP_SERVER_CONSOLE_DEBUG_ENABLE
  do
  {
    CommHttpHeaderTAG* reqHeader = reqPackage->header();
    if (resPackage->setData(&this->data_Manager) !=0)
    {
      break;
    }
    CommHttpHeaderTAG* resHeader = resPackage->header();
    char* resBody = resPackage->body();
    DataSize_t bodyMaxLen = resPackage->bodyMaxLen() - 1;
    //set default header
    resHeader->dataType = COMM_HTTP_DATA_TYPE_TEXT_PLAIN;
    //resHeader->reqMethod = COMM_HTTP_METHOD_POST;
    resHeader->fileData = 0;
    resHeader->resStatusCode = COMM_HTTP_STATUS_CODE_500;
    // set body
    resBody[bodyMaxLen] = '\0';
    const char* basePath = this->basePath();// ((CommHttpServer*)uriReqInfo->uriInfo->serverHandler)->basePath();

    strlcpy(resBody, basePath, bodyMaxLen);
//#ifdef COMM_HTTP_SERVER_CONSOLE_DEBUG_ENABLE
//    CONSOLE_LOG_BUF(commHttpServerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[HtSTsk] cbDH %i %s", 3, resBody);
//    CONSOLE_LOG_BUF(commHttpServerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "%s", basePath);
//    CONSOLE_LOG_BUF(commHttpServerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "%s", reqHeader->uri);
//#endif // COMM_HTTP_SERVER_CONSOLE_DEBUG_ENABLE
    if (reqHeader->uri[strlen(reqHeader->uri) - 1] == '/')
    {
      strlcat(resBody, "/index.html", bodyMaxLen);
    }
    else
    {
      strlcat(resBody, reqHeader->uri, bodyMaxLen);
    }
//#ifdef COMM_HTTP_SERVER_CONSOLE_DEBUG_ENABLE
//    CONSOLE_LOG_BUF(commHttpServerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[HtSTsk] cbDH %i", 4);
//    CONSOLE_LOG_BUF(commHttpServerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "%s", resBody);
//#endif // COMM_HTTP_SERVER_CONSOLE_DEBUG_ENABLE
    if (FileSystemManager::getInstance().exist(resBody) == false)
    {
#ifdef COMM_HTTP_SERVER_CONSOLE_DEBUG_ENABLE
      CONSOLE_LOG_BUF(commHttpServerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[HtSTsk] cbDH %i", -4);
#endif // COMM_HTTP_SERVER_CONSOLE_DEBUG_ENABLE
      resHeader->resStatusCode = COMM_HTTP_STATUS_CODE_404;
      goto _Cb_Comm_Http_Default_Uri_Handler;
    }

    resPackage->bodyLen(strlen(resBody));

    // set header
    resHeader->dataType = CommHttpConstant::fileExt2DataType(resBody);
    //resHeader->reqMethod = COMM_HTTP_METHOD_POST;
    resHeader->fileData = 1;
    resHeader->resStatusCode = COMM_HTTP_STATUS_CODE_200;
#ifdef COMM_HTTP_SERVER_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(commHttpServerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[HtSTsk] cbDH %i", 99);
#endif // COMM_HTTP_SERVER_CONSOLE_DEBUG_ENABLE
    // release request

_Cb_Comm_Http_Default_Uri_Handler:
    uriReqInfo->reqData.releaseData();
    return 0;
  } while (0);

  resPackage->releaseData();
  uriReqInfo->reqData.releaseData();
  return -1;
}

int CommHttpServer::regDefaultUriHandler(void)
{
  int ret = 0;
  do
  {
    CommHttpUriInfoTAG defaultUriHandler = CommHttpUriInfoTAG();
    defaultUriHandler.id = COMM_HTTP_DEFAULT_URI_ID;
    //defaultUriHandler.basePath = this->base_Path;
    defaultUriHandler.uri = COMM_HTTP_DEFAULT_URI;
    defaultUriHandler.method = COMM_HTTP_METHOD_GET;
    defaultUriHandler.dataType = COMM_HTTP_DATA_TYPE_TEXT_HTML;
    defaultUriHandler.reqCallback = CommHttpServer::cbCommHttpRequestDefaultUriHandlerNavigator;
    defaultUriHandler.userArg = this;

    ret = this->regUriHandler(defaultUriHandler);
//#ifdef COMM_HTTP_SERVER_CONSOLE_DEBUG_ENABLE
//    CONSOLE_LOG_BUF(commHttpServerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[HtSTsk] regd %i %i", 1, ret);
//#endif // COMM_HTTP_SERVER_CONSOLE_DEBUG_ENABLE
    if (ret != 0)
    {
      break;
    }

    return 0;
  } while (0);
  return -1;
}

DataSize_t CommHttpServer::readRequest(HttpReq_t* httpReq, char* buffer, DataSize_t bufferSize, DataSize_t maxLenToRead)
{
  DataSize_t curLen = 0;
  DataSize_t received = 0;
//#ifdef COMM_HTTP_SERVER_CONSOLE_DEBUG_ENABLE
//  CONSOLE_LOG_BUF(commHttpServerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[HtSTsk] redr %i %d %d", 0, bufferSize, maxLenToRead);
//#endif // COMM_HTTP_SERVER_CONSOLE_DEBUG_ENABLE
  do
  {
    if ((buffer == NULL)
      || (bufferSize < maxLenToRead)
      )
    {
      break;
    }
    // read body
    if (maxLenToRead == 0)
    {
      return 0;
    }

    while (curLen < maxLenToRead)
    {
      received = httpd_req_recv(httpReq, buffer + curLen, maxLenToRead);
      if (received <= 0)
      {
        break;
      }
      curLen += received;
    }
//#ifdef COMM_HTTP_SERVER_CONSOLE_DEBUG_ENABLE
//    CONSOLE_LOG_BUF(commHttpServerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[HtSTsk] redr %i %d %d", 5, curLen, maxLenToRead);
//#endif // COMM_HTTP_SERVER_CONSOLE_DEBUG_ENABLE
    if (curLen < maxLenToRead)
    {
      break;
    }
    return curLen;
  } while (0);
  return 0;
}

int CommHttpServer::cbCommHttpRequestHandler(HttpReq_t* httpReq)
{
  int ret = 0;
#ifdef COMM_HTTP_SERVER_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(commHttpServerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[HtSTsk] cbRH %i %d", 0, httpReq);
#endif // COMM_HTTP_SERVER_CONSOLE_DEBUG_ENABLE
  CommHttpUriRequestTAG requestInfo = CommHttpUriRequestTAG();
  CommHttpPackageTAG* resPackage = &requestInfo.resData;
  CommHttpPackageTAG* reqPackage = &requestInfo.reqData;
  do
  {
    // check uri
    CommHttpUriInfoTAG* uriInfo = (CommHttpUriInfoTAG*)httpReq->user_ctx;
    if (uriInfo == NULL)
    {
#ifdef COMM_HTTP_SERVER_CONSOLE_DEBUG_ENABLE
      CONSOLE_LOG_BUF(commHttpServerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[HtSTsk] cbRH %i %d", -1, httpReq);
#endif // COMM_HTTP_SERVER_CONSOLE_DEBUG_ENABLE
      break;
    }

    size_t totalLen = httpReq->content_len;
    ret = reqPackage->setData(this->dataManager());
    if ((ret != 0)
      || (reqPackage->bodyMaxLen() < (totalLen + 1))
      )
    {
#ifdef COMM_HTTP_SERVER_CONSOLE_DEBUG_ENABLE
      CONSOLE_LOG_BUF(commHttpServerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[HtSTsk] cbRH %i %d %d", -2, reqPackage->bodyMaxLen(), totalLen);
#endif // COMM_HTTP_SERVER_CONSOLE_DEBUG_ENABLE
      break;
    }
    reqPackage->bodyLen(totalLen);

    // make request infor
    requestInfo.requestId = this->nextReqId();
    // below should be set inside reqCallback
    memset(&requestInfo.resNoti, 0, sizeof(CommHttpNotifierTAG));
    
    
//#ifdef COMM_HTTP_SERVER_CONSOLE_DEBUG_ENABLE
//    CONSOLE_LOG_BUF(commHttpServerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[HtSTsk] cbRH %i %i", 4, uriInfo->id);
//    CONSOLE_LOG_BUF(commHttpServerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[HtSTsk] cbRH %i %lu", 5, totalLen);
//#endif // COMM_HTTP_SERVER_CONSOLE_DEBUG_ENABLE
    // read header
    {
      //header
      CommHttpHeaderTAG* reqHeader = reqPackage->header();
      reqHeader->dataType = uriInfo->dataType;
      reqHeader->reqMethod = uriInfo->method;
      reqHeader->uri = httpReq->uri;
      reqHeader->uriId = uriInfo->id;
      // read body
      char* reqBody = reqPackage->body();
      DataSize_t readLen = readRequest(httpReq, reqBody, reqPackage->bodyMaxLen(), totalLen);
#ifdef COMM_HTTP_SERVER_CONSOLE_DEBUG_ENABLE
      CONSOLE_LOG_BUF(commHttpServerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[HtSTsk] cbRH %i %lu %lu", 6, totalLen, readLen);
#endif // COMM_HTTP_SERVER_CONSOLE_DEBUG_ENABLE
      if (readLen != totalLen)
      {
        break;
      }
    }
    
    if (uriInfo->reqCallback == NULL)
    {
      reqPackage->releaseData();
      return 0;
    }
    
    ret = uriInfo->reqCallback(uriInfo->userArg, &requestInfo); // callee is responsible to release req data item
//#ifdef COMM_HTTP_SERVER_CONSOLE_DEBUG_ENABLE
//    CONSOLE_LOG_BUF(commHttpServerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[HtSTsk] cbRH %i %i", 10, ret);
//#endif // COMM_HTTP_SERVER_CONSOLE_DEBUG_ENABLE
    if (ret != 0)
    {
      goto _cb_Comm_Http_Request_Handler_End_Error;
    }
    // send respose
    ret = this->sendResponse(httpReq, requestInfo.resData);
//#ifdef COMM_HTTP_SERVER_CONSOLE_DEBUG_ENABLE
//    CONSOLE_LOG_BUF(commHttpServerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[HtSTsk] cbRH %i %i", 11, ret);
//#endif // COMM_HTTP_SERVER_CONSOLE_DEBUG_ENABLE
    if (ret != 0)
    {
      break;
    }
    return 0;
  } while (0);

  reqPackage->releaseData();
_cb_Comm_Http_Request_Handler_End_Error:
  httpd_resp_send_500(httpReq);
  return -1;
}

HttpRet_t CommHttpServer::cbCommHttpRequestHandlerNavigator(HttpReq_t* httpReq)
{
  do
  {
    if (httpReq->user_ctx == NULL)
    {
      break;
    }
    CommHttpServer* serverHandler = (CommHttpServer*)((CommHttpUriInfoTAG*)httpReq->user_ctx)->serverHandler;
    int ret = serverHandler->cbCommHttpRequestHandler(httpReq);
    if (ret != 0)
    {
      break;
    }

    return HTTP_RET_OK;
  } while (0);
  return HTTP_RET_ERR;
}

int CommHttpServer::cbCommHttpRequestDefaultUriHandlerNavigator(void* arg, CommHttpUriRequestTAG* uriReqInfo)
{
  int ret = 0;
  do
  {
    if ((uriReqInfo == NULL)
      || (arg == NULL)
      )
    {
      break;
    }
    CommHttpServer* serverHandler = (CommHttpServer*)(arg);
    ret = serverHandler->cbCommHttpDefaultUriHandler(uriReqInfo);
    if (ret != 0)
    {
      break;
    }
    return 0;
  } while (0);
  return -1;
}
#endif // _CONF_COMM_HTTP_SERVER_ENABLED