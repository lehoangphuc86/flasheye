#include "CommHttpClient.h"
#if (_CONF_COMM_HTTP_CLIENT_ENABLED)
/////////////////////////////////////////////////
// INCLUDE
#include "../file_system/FileSystemManager.h"
#include "timer_manager/TimerManager.h"
/////////////////////////////////////////////////
// PREPROCESSOR
#define COMM_HTTP_CLIENT_CONSOLE_DEBUG_ENABLE
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
#ifdef COMM_HTTP_CLIENT_CONSOLE_DEBUG_ENABLE
char commHttpClientLogBuf[SYSTEM_CONSOLE_OUT_BUF_LEN];
#endif // COMM_HTTP_CLIENT_CONSOLE_DEBUG_ENABLE
/////////////////////////////////////////////////
// STATIC FUNCTIONS

/////////////////////////////////////////////////
// EXTERN

/////////////////////////////////////////////////
// CLASS IMPLEMENTAION
/*CommHttpClient*/
CommHttpClient::CommHttpClient(void)
  : CommHttp(false)
  , http_Handler(HTTP_HANDLER_INVALID) 
{

}

CommHttpClient::~CommHttpClient(void)
{
  
}

void CommHttpClient::regEventSize(void)
{
  this->registerHanldingEventStructSize(sizeof(EventCommHttpClientRequestTAG));
  this->registerHanldingEventStructSize(sizeof(EventCommHttpConnectionOpenedTAG));
  this->registerHanldingEventStructSize(sizeof(EventCommHttpConnectionClosedTAG));
  this->registerHanldingEventStructSize(sizeof(EventCommHttpFinishedTAG));
}


DataSize_t CommHttpClient::readResponse(CommHttpUriRequestTAG* httpReq, char* buffer, DataSize_t bufferLen)
{
  DataSize_t curLen = 0;
  DataSize_t received = 0;
  do
  {
    DataSize_t responseLen = esp_http_client_fetch_headers(this->http_Handler);
#ifdef COMM_HTTP_CLIENT_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(commHttpClientLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[HtCTsk] redr %i %d %d", 0, bufferLen, responseLen);
#endif // COMM_HTTP_CLIENT_CONSOLE_DEBUG_ENABLE
    if (responseLen < 0)
    {
      break;
    }

    if ((responseLen == 0)
      && (esp_http_client_is_chunked_response(this->http_Handler) == false)
      )
    {
      break;
    }

    curLen = esp_http_client_read_response(this->http_Handler, buffer, bufferLen);
    if (curLen <= 0)
    {
      break;
    }
    
    return curLen;
  } while (0);
  return 0;
}

int CommHttpClient::request(CommHttpClientRequestParamsTAG& params, HttpReqId_t& reqId)
{
#ifdef COMM_HTTP_CLIENT_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(commHttpClientLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[HtCTsk] req %i", 0);
#endif // COMM_HTTP_CLIENT_CONSOLE_DEBUG_ENABLE
  int ret = 0;
  EventCommHttpClientRequestTAG sendRequestData = EventCommHttpClientRequestTAG();
  do
  {
    if (this->isStarted() == false)
    {
#ifdef COMM_HTTP_CLIENT_CONSOLE_DEBUG_ENABLE
      CONSOLE_LOG_BUF(commHttpClientLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[HtCTsk] req %i", -1);
#endif // COMM_HTTP_CLIENT_CONSOLE_DEBUG_ENABLE
      break;
    }

    reqId = this->nextReqId();
    sendRequestData.param.requestId = reqId;
    memcpy(&sendRequestData.param.resNoti, &params.notifier, sizeof(CommHttpNotifierTAG));
    ret = sendRequestData.param.reqData.setData(this->dataManager());
#ifdef COMM_HTTP_CLIENT_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(commHttpClientLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[HtCTsk] req %i %i", 3, ret);
#endif // COMM_HTTP_CLIENT_CONSOLE_DEBUG_ENABLE
    if (ret != 0)
    {
      break;
    }

    CommHttpPackageTAG* reqPackage = &sendRequestData.param.reqData;
    CommHttpHeaderTAG* reqHeader = reqPackage->header();
    char* reqBody = reqPackage->body();
    memcpy(reqHeader, &params.header, sizeof(CommHttpHeaderTAG));
    memcpy(reqBody, params.body, params.bodyLen);
    reqPackage->bodyLen(params.bodyLen);
#ifdef COMM_HTTP_CLIENT_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(commHttpClientLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[HtCTsk] req %i %i %i", 5, params.bodyLen, reqPackage->bodyLen());
#endif // COMM_HTTP_CLIENT_CONSOLE_DEBUG_ENABLE
    // notify
    ret = this->notify(EventManagerConstant::EventMessageId::CommHttpClientRequest, sizeof(EventCommHttpClientRequestTAG), (unsigned char*)&sendRequestData);
#ifdef COMM_HTTP_CLIENT_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(commHttpClientLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[HtCTsk] req %i %i", 6, ret);
#endif // COMM_HTTP_CLIENT_CONSOLE_DEBUG_ENABLE
    if (ret != 0)
    {
      break;
    }
#ifdef COMM_HTTP_CLIENT_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(commHttpClientLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[HtCTsk] req %i", 99);
#endif // COMM_HTTP_CLIENT_CONSOLE_DEBUG_ENABLE
    return 0;
  } while (0);

  sendRequestData.param.reqData.releaseData();
#ifdef COMM_HTTP_CLIENT_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(commHttpClientLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[HtCTsk] req %i", -99);
#endif // COMM_HTTP_CLIENT_CONSOLE_DEBUG_ENABLE
  return -1;
}

int clientSendChunk(HttpClientHandler_t httpClientHandler, char* buff, DataSize_t buffLen, char* tempBuf, DataSize_t tempBuffLen)
{
  int sentLen = 0;
  int sizeTextLen = 0;
  if (buff == NULL)
  {
    esp_http_client_write(httpClientHandler, "0", 1);  // end
    esp_http_client_write(httpClientHandler, COMM_HTTP_TERMINATOR, COMM_HTTP_TERMINATOR_LEN);
    esp_http_client_write(httpClientHandler, COMM_HTTP_TERMINATOR, COMM_HTTP_TERMINATOR_LEN);
    return 0;
  }
  
  do
  {
    if (tempBuf == NULL)
    {
      break;
    }
    tempBuf[tempBuffLen - 1] = '\0';
    sizeTextLen = snprintf(tempBuf, tempBuffLen, "%lu", buffLen);
    sentLen = esp_http_client_write(httpClientHandler, tempBuf, sizeTextLen);
    if (sentLen != sizeTextLen)
    {
      break;
    }

    sentLen = esp_http_client_write(httpClientHandler, buff, buffLen);
    if (sentLen != buffLen)
    {
      break;
    }
    return 0;
  } while (0);
  return -1;
}

int CommHttpClient::sendBuff(CommHttpUriRequestTAG* httpReq, char* buffer, DataSize_t bufferLen)
{
  HttpRet_t httpRet = HTTP_RET_OK;
  int ret = 0;
  int sentLen = 0;
  do
  {
    if (bufferLen < COMM_HTTP_PACKAGE_BUFF_CHUNK_SIZE)
    {
      httpRet = esp_http_client_open(this->http_Handler, bufferLen);
      if (httpRet != HTTP_RET_OK)
      {
        break;
      }

      sentLen = esp_http_client_write(this->http_Handler, buffer, bufferLen);
#ifdef COMM_HTTP_CLIENT_CONSOLE_DEBUG_ENABLE
      CONSOLE_LOG_BUF(commHttpClientLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[HtCTsk] resB %i %i %i", 1, sentLen, bufferLen);
#endif // COMM_HTTP_CLIENT_CONSOLE_DEBUG_ENABLE
      if (sentLen != bufferLen)
      {
        break;
      }
      return 0;
    }

    DataSize_t remainingLen = bufferLen;
    DataSize_t lenToSend = 0;
    httpRet = esp_http_client_set_header(this->http_Handler, COMM_HTTP_HEADER_TRANS_ENCODE_NAME, COMM_HTTP_HEADER_TRANS_ENCODE_CHUNKED);
    if (httpRet != HTTP_RET_OK)
    {
      break;
    }

    httpRet = esp_http_client_open(this->http_Handler, bufferLen);
    if (httpRet != HTTP_RET_OK)
    {
      break;
    }

    char tempBuff[COMM_HTTP_DATA_SIZE_LEN];
    memset(tempBuff, 0, COMM_HTTP_DATA_SIZE_LEN);
    do
    {
      lenToSend = SYSTEM_MIN(remainingLen, COMM_HTTP_PACKAGE_BUFF_CHUNK_SIZE);
      ret = clientSendChunk(this->http_Handler, buffer, lenToSend, tempBuff, COMM_HTTP_DATA_SIZE_LEN);
      if (ret != 0)
      {
        clientSendChunk(this->http_Handler, NULL, 0, tempBuff, COMM_HTTP_DATA_SIZE_LEN);
        break;
      }

      buffer += lenToSend;
      remainingLen -= lenToSend;
    } while (remainingLen > 0);

#ifdef COMM_HTTP_CLIENT_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(commHttpClientLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[HtCTsk] resB %i %lu", 5, remainingLen);
#endif // COMM_HTTP_CLIENT_CONSOLE_DEBUG_ENABLE
    if (remainingLen > 0)
    {
      break;
    }
    clientSendChunk(this->http_Handler, NULL, 0, tempBuff, COMM_HTTP_DATA_SIZE_LEN);
#ifdef COMM_HTTP_CLIENT_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(commHttpClientLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[HtCTsk] resB %i", 99);
#endif // COMM_HTTP_CLIENT_CONSOLE_DEBUG_ENABLE
    return 0;
  } while (0);
  return -1;
}

int CommHttpClient::sendFileData(CommHttpUriRequestTAG* httpReq, char* buffer, DataSize_t bufferSize)
{

  HttpRet_t httpRet = HTTP_RET_OK;
  int ret = 0;
  do
  {
#ifdef COMM_HTTP_CLIENT_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(commHttpClientLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[HtCTsk] resF %i", 0);
#endif // COMM_HTTP_CLIENT_CONSOLE_DEBUG_ENABLE
    const char* filePath = buffer;
    FileHandler_t file = FileSystemManager::getInstance().openFile(filePath, "r");
    if (FileSystemManager::getInstance().isValidFileHandler(file) == false)
    {
      break;
    }
    
    httpRet = esp_http_client_set_header(this->http_Handler, COMM_HTTP_HEADER_TRANS_ENCODE_NAME, COMM_HTTP_HEADER_TRANS_ENCODE_CHUNKED);
    if (httpRet != HTTP_RET_OK)
    {
      break;
    }

    DataSize_t remainingLen = FileSystemManager::getInstance().fileSize(filePath);
    DataSize_t lenToSend = 0;
    int error = 0;
    char tempBuff[COMM_HTTP_DATA_SIZE_LEN];
    memset(tempBuff, 0, COMM_HTTP_DATA_SIZE_LEN);

    httpRet = esp_http_client_open(this->http_Handler, remainingLen);
    if (httpRet != HTTP_RET_OK)
    {
      break;
    }

    do
    {
      /* Read file in chunks into the scratch buffer */
      lenToSend = fread(buffer, 1, bufferSize, file);
      //lenToSend = file.read((uint8_t*)buffer, bufferSize);
      if (lenToSend <= 0)
      {
        break;
      }

      ret = clientSendChunk(this->http_Handler, buffer, lenToSend, tempBuff, COMM_HTTP_DATA_SIZE_LEN);
      if (ret != 0)
      {
        error = -2;
        /* Abort sending file */
        clientSendChunk(this->http_Handler, NULL, 0, tempBuff, COMM_HTTP_DATA_SIZE_LEN);
        break;
      }
      remainingLen -= lenToSend;
    } while (remainingLen > 0);
    /* Close file after sending complete */
    //file.close();
    FileSystemManager::getInstance().closeFile(file);
    // Signal HTTP response completion
    clientSendChunk(this->http_Handler, NULL, 0, tempBuff, COMM_HTTP_DATA_SIZE_LEN);
#ifdef COMM_HTTP_CLIENT_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(commHttpClientLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[HtCTsk] resF %i %i", 8, error);
#endif // COMM_HTTP_CLIENT_CONSOLE_DEBUG_ENABLE
    if ((remainingLen > 0)
      && (error != 0)
      )
    {
      break;
    }
#ifdef COMM_HTTP_CLIENT_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(commHttpClientLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[HtCTsk] resF %i", 99);
#endif // COMM_HTTP_CLIENT_CONSOLE_DEBUG_ENABLE
    return 0;
  } while (0);
#ifdef COMM_HTTP_CLIENT_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(commHttpClientLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[HtCTsk] resF %i", -99);
#endif // COMM_HTTP_CLIENT_CONSOLE_DEBUG_ENABLE
  return -1;
}


void CommHttpClient::proc(void)
{
#ifdef COMM_HTTP_CLIENT_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(commHttpClientLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[HtCTsk] proc %i", 0);
#endif // COMM_HTTP_CLIENT_CONSOLE_DEBUG_ENABLE
  int prepareRet = this->prepare();
  this->reportPrepareResult(prepareRet);
  if (prepareRet != 0)
  {
    this->waitTerminating();
    return;
  }
#ifdef COMM_HTTP_CLIENT_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(commHttpClientLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[HtCTsk] proc %i", 1);
#endif // COMM_HTTP_CLIENT_CONSOLE_DEBUG_ENABLE
  EventDataItem* eventData = NULL;
  bool isStop = false;
  while (isStop == false) // A Task shall never return or exit.
  {
    eventData = this->event_Manager.wait(1000);
#ifdef COMM_HTTP_CLIENT_CONSOLE_DEBUG_ENABLE
    //CONSOLE_LOG_BUF(commHttpClientLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[WFTsk] proc %i", 2);
#endif // COMM_HTTP_CLIENT_CONSOLE_DEBUG_ENABLE
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
    case (int)EventManagerConstant::EventMessageId::CommHttpConnectionOpened:
    {
      this->onEventCommHttpConnectionOpened(eventData->bufferAddress(), eventData->dataLength());
      break;
    }
    case (int)EventManagerConstant::EventMessageId::CommHttpClientRequest:
    {
      this->onEventCommHttpClientRequest(eventData->bufferAddress(), eventData->dataLength());
      break;
    }
    case (int)EventManagerConstant::EventMessageId::CommHttpConnectionClosed:
    {
      this->onEventCommHttpConnectionClosed(eventData->bufferAddress(), eventData->dataLength());
      break;
    }
    case (int)EventManagerConstant::EventMessageId::CommHttpStop:
    {
      this->onEventCommHttpStop(eventData->bufferAddress(), eventData->dataLength());
      break;
    }
    case (int)EventManagerConstant::EventMessageId::CommHttpFinished:
    {
      this->onEventCommHttpFinished(eventData->bufferAddress(), eventData->dataLength());
      break;
    }
    default:
      break;
    }
    this->event_Manager.release(eventData);
  }
#ifdef COMM_HTTP_CLIENT_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG("[HtCTsk] stop %i", 99);
#endif // COMM_HTTP_CLIENT_CONSOLE_DEBUG_ENABLE
}

int CommHttpClient::onEventCommHttpStart(unsigned char* data, unsigned int dataSize)
{
#ifdef COMM_HTTP_CLIENT_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(commHttpClientLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[HtCTsk] eSta %i", 0);
#endif // COMM_HTTP_CLIENT_CONSOLE_DEBUG_ENABLE
  //HttpRet_T httpRet = 0;
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
    esp_http_client_config_t httpConfig = esp_http_client_config_t();
    httpConfig.url = eventData->config.client.url;
    httpConfig.host = eventData->config.client.host;
    httpConfig.port = eventData->config.client.port;
    httpConfig.cert_pem = eventData->config.client.certPem;
    httpConfig.event_handler = cbHttpEventHandlerNavigator;
    httpConfig.user_data = this;
    httpConfig.disable_auto_redirect = (!COMM_HTTP_CLIENT_AUTO_DIRECT_DEFAULT);
#ifdef COMM_HTTP_CLIENT_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(commHttpClientLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[HtCTsk] eSta %i %i", 0, httpConfig.port);
    //CONSOLE_LOG_BUF(commHttpClientLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "%s:%s", httpConfig.host, httpConfig.path);
#endif // COMM_HTTP_CLIENT_CONSOLE_DEBUG_ENABLE

    
    this->http_Handler = esp_http_client_init(&httpConfig);
    if (this->http_Handler == HTTP_HANDLER_INVALID)
    {
      break;
    }

    this->is_Started = 1;
    this->ret_Start = 0;
    this->mutex_wait_Start.unlock();
#ifdef COMM_HTTP_CLIENT_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(commHttpClientLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[HtCTsk] eSta %i", 99);
#endif // COMM_HTTP_CLIENT_CONSOLE_DEBUG_ENABLE
    return 0;
  } while (0);

  {
    this->ret_Start = -1;
    this->mutex_wait_Start.unlock();
  }
#ifdef COMM_HTTP_CLIENT_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(commHttpClientLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[HtCTsk] eSta %i", -99);
#endif // COMM_HTTP_CLIENT_CONSOLE_DEBUG_ENABLE
  return -1;
}

int CommHttpClient::onEventCommHttpConnectionOpened(unsigned char* data, unsigned int dataSize)
{
  do
  {
    if ((dataSize != sizeof(EventCommHttpConnectionOpenedTAG))
      || (data == NULL))
    {
      break;
    }
#ifdef COMM_HTTP_CLIENT_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(commHttpClientLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[HtCTsk] eop %i", 0);
#endif // COMM_HTTP_CLIENT_CONSOLE_DEBUG_ENABLE
    /*this->is_Started = 1;
    this->ret_Start = 0;
    this->mutex_wait_Start.unlock();*/

    return 0;
  } while (0);
  return -1;

}

int CommHttpClient::onEventCommHttpDataReceived(unsigned char* data, unsigned int dataSize)
{
  do
  {
    if ((dataSize != sizeof(EventCommHttpDataReceivedTAG))
      || (data == NULL))
    {
      break;
    }

#ifdef COMM_HTTP_CLIENT_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(commHttpClientLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[HtCTsk] ereved %i", 0);
#endif // COMM_HTTP_CLIENT_CONSOLE_DEBUG_ENABLE

    return 0;
  } while (0);
  return -1;
}



int CommHttpClient::onEventCommHttpConnectionClosed(unsigned char* data, unsigned int dataSize)
{
  do
  {
    if ((dataSize != sizeof(EventCommHttpConnectionClosedTAG))
      || (data == NULL))
    {
      break;
    }

#ifdef COMM_HTTP_CLIENT_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(commHttpClientLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[HtCTsk] eclosed %i", 0);
#endif // COMM_HTTP_CLIENT_CONSOLE_DEBUG_ENABLE
    this->is_Started = 0;
    this->ret_Stop = 0;
    this->mutex_wait_Stop.unlock();
    return 0;
  } while (0);
  return -1;
}

int CommHttpClient::onEventCommHttpStop(unsigned char* data, unsigned int dataSize)
{
#ifdef COMM_HTTP_CLIENT_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(commHttpClientLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[HtCTsk] esto %i", 0);
#endif // COMM_HTTP_CLIENT_CONSOLE_DEBUG_ENABLE
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
    if (this->http_Handler != HTTP_HANDLER_INVALID)
    {
      esp_http_client_cleanup(this->http_Handler);
    }

    this->http_Handler = HTTP_HANDLER_INVALID;
    //this->is_Started = 0;
    //this->ret_Stop = 0;
    //this->mutex_wait_Stop.unlock();
    return 0;
  } while (0);

  {
    this->ret_Stop = -1;
    this->mutex_wait_Stop.unlock();
  }
#ifdef COMM_HTTP_CLIENT_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(commHttpClientLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[HtCTsk] esto %i", -99);
#endif // COMM_HTTP_CLIENT_CONSOLE_DEBUG_ENABLE
  return -1;
}

int CommHttpClient::onEventCommHttpClientRequest(unsigned char* data, unsigned int dataSize)
{
#ifdef COMM_HTTP_CLIENT_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(commHttpClientLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[HtCTsk] ereq %i", 0);
#endif // COMM_HTTP_CLIENT_CONSOLE_DEBUG_ENABLE
  HttpRet_t httpRet = HTTP_RET_OK;
  CommHttpPackageTAG* reqPackage = NULL;
  CommHttpPackageTAG* resPackage = NULL;
  int ret = 0;
  do
  {
    if ((dataSize != sizeof(EventCommHttpClientRequestTAG))
      || (data == NULL))
    {
      break;
    }

    if ((this->isStarted() == false)
      || (this->http_Handler == NULL)
      )
    {
      break;
    }

    EventCommHttpClientRequestTAG* eventData = (EventCommHttpClientRequestTAG*)data;
    CommHttpUriRequestTAG* httpReq = &eventData->param;
    reqPackage = &httpReq->reqData;
    resPackage = &httpReq->resData;
    CommHttpHeaderTAG* reqHeader = reqPackage->header();
    CommHttpNotifierTAG* notifier = &httpReq->resNoti;
    if ((reqHeader->reqMethod >= COMM_HTTP_METHOD_MAX)
      || (reqHeader->dataType >= COMM_HTTP_DATA_TYPE_MAX)
      )
    {
      break;
    }

    httpRet = esp_http_client_set_url(this->http_Handler, reqHeader->uri);
#ifdef COMM_HTTP_CLIENT_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(commHttpClientLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[HtCTsk] ereq %i %i", 6, (int)httpRet);
#endif // COMM_HTTP_CLIENT_CONSOLE_DEBUG_ENABLE
    if (httpRet != HTTP_RET_OK)
    {
      break;
    }

    httpRet = esp_http_client_set_method(this->http_Handler, COMM_HTTP_CLIENT_METHOD_TBL[reqHeader->reqMethod]);
#ifdef COMM_HTTP_CLIENT_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(commHttpClientLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[HtCTsk] ereq %i %i", 7, (int)httpRet);
#endif // COMM_HTTP_CLIENT_CONSOLE_DEBUG_ENABLE
    if (httpRet != HTTP_RET_OK)
    {
      break;
    }

    httpRet = esp_http_client_set_header(this->http_Handler, COMM_HTTP_HEADER_CONTENT_NAME, COMM_HTTP_DATA_TYPE_STR_TBL[reqHeader->dataType]);
#ifdef COMM_HTTP_CLIENT_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(commHttpClientLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[HtCTsk] ereq %i %i", 8, (int)httpRet);
#endif // COMM_HTTP_CLIENT_CONSOLE_DEBUG_ENABLE
    if (httpRet != HTTP_RET_OK)
    {
      break;
    }

    // prepare res
    ret = resPackage->setData(this->dataManager());
    if (ret != 0)
    {
      break;
    }
#ifdef COMM_HTTP_CLIENT_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(commHttpClientLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[HtCTsk] ereq %i %i", 11, reqPackage->bodyLen());
    //CONSOLE_LOG_BUF(commHttpClientLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "%s", reqPackage->body());
#endif // COMM_HTTP_CLIENT_CONSOLE_DEBUG_ENABLE
    if (reqHeader->fileData == 0)
    {
      ret = this->sendBuff(httpReq, reqPackage->body(), reqPackage->bodyLen());
    }
    else
    {
      ret = this->sendFileData(httpReq, reqPackage->body(), reqPackage->bodyLen());
    }
#ifdef COMM_HTTP_CLIENT_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(commHttpClientLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[HtCTsk] ereq %i %i", 15, ret);
#endif // COMM_HTTP_CLIENT_CONSOLE_DEBUG_ENABLE
    if (ret != 0)
    {
      break;
    }
#ifdef COMM_HTTP_CLIENT_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(commHttpClientLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[HtCTsk] ereq %i", 16);
#endif // COMM_HTTP_CLIENT_CONSOLE_DEBUG_ENABLE
    // download response
    CommHttpHeaderTAG* resHeader = resPackage->header();
    {
      char* headerKey = COMM_HTTP_HEADER_CONTENT_NAME;
      char* headerVal = NULL;
      httpRet = esp_http_client_get_header(this->http_Handler, headerKey, &headerVal);
      if (httpRet != HTTP_RET_OK)
      {
        break;
      }
#ifdef COMM_HTTP_CLIENT_CONSOLE_DEBUG_ENABLE
      CONSOLE_LOG_BUF(commHttpClientLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[HtCTsk] ereq %i %s", 17, (headerVal== NULL? "null" : headerVal));
#endif // COMM_HTTP_CLIENT_CONSOLE_DEBUG_ENABLE
      resHeader->dataType = CommHttpConstant::dataType2Code(headerVal);
      resHeader->fileData = 0;
      resHeader->resStatusCode = CommHttpConstant::statusCode2Code(esp_http_client_get_status_code(this->http_Handler));
      resHeader->uri = reqHeader->uri;
      resHeader->uriId = reqHeader->uriId;
    }
#ifdef COMM_HTTP_CLIENT_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(commHttpClientLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[HtCTsk] ereq %i", 20);
#endif // COMM_HTTP_CLIENT_CONSOLE_DEBUG_ENABLE
    {
      char* resBody = resPackage->body();
      DataSize_t responseSize = this->readResponse(httpReq, resBody, resPackage->bodyMaxLen());
      resPackage->bodyLen(responseSize);
    }

    if (notifier->callback == NULL)
    {
      reqPackage->releaseData();
      return 0;
    }
#ifdef COMM_HTTP_CLIENT_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(commHttpClientLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[HtCTsk] ereq %i", 21);
    CONSOLE_LOG_BUF(commHttpClientLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "%s", resPackage->body());
#endif // COMM_HTTP_CLIENT_CONSOLE_DEBUG_ENABLE
    if ((notifier->expiredTime > 0)
      && (TimerManager::getInstance().now() > notifier->expiredTime)
      )
    {
#ifdef COMM_HTTP_CLIENT_CONSOLE_DEBUG_ENABLE
      CONSOLE_LOG_BUF(commHttpClientLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[HtCTsk] ereq %i %lu", -21, notifier->expiredTime);
      CONSOLE_LOG_BUF(commHttpClientLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[HtCTsk] ereq %i %i", -21, TimerManager::getInstance().now());
#endif // COMM_HTTP_CLIENT_CONSOLE_DEBUG_ENABLE
      break; // went off.
    }
    notifier->callback(notifier->agr, httpReq);
    reqPackage->releaseData();
#ifdef COMM_HTTP_CLIENT_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(commHttpClientLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[HtCTsk] ereq %i", 99);
#endif // COMM_HTTP_CLIENT_CONSOLE_DEBUG_ENABLE
    return 0;
  } while (0);
#ifdef COMM_HTTP_CLIENT_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(commHttpClientLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[HtCTsk] ereq %i", -98);
#endif // COMM_HTTP_CLIENT_CONSOLE_DEBUG_ENABLE
  if (reqPackage != NULL)
  {
    reqPackage->releaseData();
  }
  if (resPackage != NULL)
  {
    resPackage->releaseData();
  }
#ifdef COMM_HTTP_CLIENT_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(commHttpClientLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[HtCTsk] ereq %i", -99);
#endif // COMM_HTTP_CLIENT_CONSOLE_DEBUG_ENABLE
  return -1;
}

int CommHttpClient::onEventCommHttpFinished(unsigned char* data, unsigned int dataSize)
{
  do
  {
    if ((dataSize != sizeof(EventCommHttpFinishedTAG))
      || (data == NULL))
    {
      break;
    }

    this->http_Handler = HTTP_HANDLER_INVALID;
    this->is_Started = 0;
    this->ret_Stop = 0;
    this->mutex_wait_Stop.unlock();

    return 0;
  } while (0);
  return -1;
}

int CommHttpClient::cbEventCommHttpHandler(void* eventData)
{
  do
  {
    HttpClientEvent_t* evt = (HttpClientEvent_t*)eventData;
#ifdef COMM_HTTP_CLIENT_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(commHttpClientLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[HtCTsk] eHN %i %i", 0, evt->event_id);
#endif // COMM_HTTP_CLIENT_CONSOLE_DEBUG_ENABLE
    switch (evt->event_id)
    {
      case HTTP_EVENT_ERROR:
      case HTTP_EVENT_HEADER_SENT:
      case HTTP_EVENT_ON_HEADER:
        break;
      case HTTP_EVENT_ON_CONNECTED:
      {
        /*EventCommHttpConnectionOpenedTAG eventData = EventCommHttpConnectionOpenedTAG();
        eventData.reserved = 0;
        this->notify((int)EventManagerConstant::EventMessageId::CommHttpConnectionOpened, sizeof(EventCommHttpConnectionOpenedTAG), (unsigned char*)&eventData);*/
        break;
      }
      case HTTP_EVENT_DISCONNECTED:
      {
        EventCommHttpConnectionClosedTAG eventData = EventCommHttpConnectionClosedTAG();
        eventData.reserved = 0;
        this->notify((int)EventManagerConstant::EventMessageId::CommHttpConnectionClosed, sizeof(EventCommHttpConnectionClosedTAG), (unsigned char*)&eventData);
        break;
      }
      case HTTP_EVENT_ON_DATA:
      {
        // currently not supported
        break;
      }
      case HTTP_EVENT_ON_FINISH:
      {
        EventCommHttpFinishedTAG eventData = EventCommHttpFinishedTAG();
        eventData.reserved = 0;
        this->notify((int)EventManagerConstant::EventMessageId::CommHttpFinished, sizeof(EventCommHttpFinishedTAG), (unsigned char*)&eventData);
        break;
      }
      default:
        break;
    }
    return 0;
  } while (0);
  return -1;
}

HttpRet_t CommHttpClient::cbHttpEventHandlerNavigator(HttpClientEvent_t* evt)
{
  do
  {
    if (evt->user_data == NULL)
    {
      break;
    }

    int ret = ((CommHttpClient*)evt->user_data)->cbEventCommHttpHandler(evt);
    if (ret != 0)
    {
      break;
    }

    return HTTP_RET_OK;
  } while (0);
  return HTTP_RET_OK + 1;

}
#endif // _CONF_COMM_HTTP_CLIENT_ENABLED