#include "ExCommManager.h"
#if (_CONF_EX_COMM_MANAGER_ENABLED)
/////////////////////////////////////////////////
// INCLUDE
#include "../settings/SettingManager.h"
#include "mess_broker/MessBrokerManager.h"
#include "timer_manager/TimerManager.h"
#include "os_system/SystemMutex.h"
/////////////////////////////////////////////////
// PREPROCESSOR

/////////////////////////////////////////////////
// DEFINE
#define EX_COMM_MANAGER_CONSOLE_DEBUG_ENABLE

/////////////////////////////////////////////////
// MARCO

/////////////////////////////////////////////////
// GLOBAL VARIABLES

/////////////////////////////////////////////////
// GLOBAL FUNCTIONS

/////////////////////////////////////////////////
// DATA TYPE (ENUM)

/////////////////////////////////////////////////
// DATA TYPE (STRUCT)

/////////////////////////////////////////////////
// STATIC DATA
#ifdef EX_COMM_MANAGER_CONSOLE_DEBUG_ENABLE
char exCommMgrLogBuf[SYSTEM_CONSOLE_OUT_BUF_LEN];
#endif // EX_COMM_MANAGER_CONSOLE_DEBUG_ENABLE
/////////////////////////////////////////////////
// STATIC FUNCTIONS

/////////////////////////////////////////////////
// EXTERN

/////////////////////////////////////////////////
// CLASS IMPLEMENTAION
/*ExCommManager*/
ExCommManager& ExCommManager::getInstance(void)
{
  static ExCommManager instance;
  return instance;
}

ExCommManager::ExCommManager(void)
  : cb_Comm_Rev(NULL)
  , cb_Comm_Rev_Arg(NULL)
  , http_Client_0(NULL)
  , http_Server_0(NULL)
{

}

ExCommManager::~ExCommManager(void)
{
  this->stopTask();
}

bool ExCommManager::isValid(void)
{
  return (this->cb_Comm_Rev == NULL ? false : true);
}

CommMBCHttpClient* ExCommManager::httpClient0(void)
{
  return this->http_Client_0;
}

CommMBCHttpServer* ExCommManager::httpServer0(void)
{
  return this->http_Server_0;
}

int ExCommManager::startTask(ExCommManagerConfigTAG& exCommConfig)
{
  int ret = 0;
  do
  {
    if (this->isValid() != false)
    {
      return -1; // already started
    }

    if (exCommConfig.cbOnCommRev == NULL)
    {
      break;
    }

    this->cb_Comm_Rev = exCommConfig.cbOnCommRev;
    this->cb_Comm_Rev_Arg = exCommConfig.cbOnCommRevArg;

    ret = this->startTaskHttpClient0();
#ifdef EX_COMM_MANAGER_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(exCommMgrLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[exCMgr] stTT %i %i", 1, ret);
#endif // EX_COMM_MANAGER_CONSOLE_DEBUG_ENABLE
    if (ret != 0)
    {
      break;
    }

    ret = this->startTaskHttpServer0();
#ifdef EX_COMM_MANAGER_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(exCommMgrLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[exCMgr] stTT %i %i", 2, ret);
#endif // EX_COMM_MANAGER_CONSOLE_DEBUG_ENABLE
    if (ret != 0)
    {
      break;
    }

    // reg data
    // subscribe for mess SYSTEMSETTING
//    {
//      MBSubscriberConfigTAG subConfig = MBSubscriberConfigTAG();
//      subConfig.subId = FEM_MB_MEM_ID_MAIN_CONTROLLER;
//      subConfig.arg = this;
//      subConfig.cb = MainController::cbMessBrokerNotified;
//      subConfig.pattern = FEM_MB_MATCH_PATTERN_REV;
//      ret = MessBrokerManager::getInstance().subscribe(FEMConstant::MBC2MBMessId(CommMBCConstant::CommMBCMessageId::CommMBCSystemSetting), subConfig);
//#ifdef MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
//      CONSOLE_LOG_BUF(mainCtrlLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mTask] stMB %i %i", 0, ret);
//#endif // MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
//      if (ret != 0)
//      {
//        break;
//      }
//    }


#ifdef EX_COMM_MANAGER_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(exCommMgrLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[exCMgr] stTT %i", 99);
#endif // EX_COMM_MANAGER_CONSOLE_DEBUG_ENABLE
    return 0;
  } while (0);
  this->stopTask();
#ifdef EX_COMM_MANAGER_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(exCommMgrLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[exCMgr] stTT %i",-99);
#endif // EX_COMM_MANAGER_CONSOLE_DEBUG_ENABLE
  return -1;
}

int ExCommManager::startComm(bool waitCompletion)
{
  int ret = 0;
  do
  {
    ret = this->startCommHttpClient0(waitCompletion);
#ifdef EX_COMM_MANAGER_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(exCommMgrLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[exCMgr] stTC %i %i", 1, ret);
#endif // EX_COMM_MANAGER_CONSOLE_DEBUG_ENABLE
    if (ret != 0)
    {
      break;
    }

    ret = this->startCommHttpServer0(waitCompletion);
#ifdef EX_COMM_MANAGER_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(exCommMgrLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[exCMgr] stTC %i %i", 2, ret);
#endif // EX_COMM_MANAGER_CONSOLE_DEBUG_ENABLE
    if (ret != 0)
    {
      break;
    }

#ifdef EX_COMM_MANAGER_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(exCommMgrLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[exCMgr] stTC %i", 99);
#endif // EX_COMM_MANAGER_CONSOLE_DEBUG_ENABLE
    return 0;
  } while (0);
  this->stopComm(waitCompletion);
#ifdef EX_COMM_MANAGER_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(exCommMgrLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[exCMgr] stTC %i", -99);
#endif // EX_COMM_MANAGER_CONSOLE_DEBUG_ENABLE
  return -1;
}

int ExCommManager::sendComm(CommId_t commId, ExCommMBCParamTAG& mbcParams, TimePoint_t resWaitTimeMs)
{
  switch (commId)
  {
    case EX_COMM_ID_MBC_HTTP_CLIENT_0:
      return this->sendCommHttpClient(this->http_Client_0, mbcParams, resWaitTimeMs);
    default:
      return -1;
  }
}

void ExCommManager::stopComm(bool waitCompletion)
{
  this->stopCommHttpClient0(waitCompletion);
  this->stopCommHttpServer0(waitCompletion);
  return;
}

void ExCommManager::stopTask(void)
{
  this->stopTaskHttpClient0();
  this->stopTaskHttpServer0();
  this->cb_Comm_Rev = NULL;
  this->cb_Comm_Rev_Arg = NULL;
  return;
}

//int ExCommManager::notifyParent(EventId_t messageId, EventSize_t eventSize, unsigned char* eventData)
//{
//  do
//  {
//    if (this->main_Eventer == NULL)
//    {
//      break;
//    }
//
//    return this->main_Eventer->notify(messageId, eventSize, eventData);
//  } while (0);
//  return -1;
//}

int ExCommManager::startTaskHttpClient0(void)
{
  int ret = 0;
  do
  {
    if (this->http_Client_0 != NULL)
    {
#ifdef EX_COMM_MANAGER_CONSOLE_DEBUG_ENABLE
      CONSOLE_LOG_BUF(exCommMgrLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[exCMgr] stHtCl %i", -1);
#endif // EX_COMM_MANAGER_CONSOLE_DEBUG_ENABLE
      return -1; //already
    }
    this->stopTaskHttpClient0();

    this->http_Client_0 = new CommMBCHttpClient();
    if (this->http_Client_0 == NULL)
    {
      break;
    }

    {
      CommMBCSettingParamTAG cliMbcSetting = CommMBCSettingParamTAG();
      cliMbcSetting.enabled = true;
      cliMbcSetting.isHeaderLess = FEM_HTTP_CLIENT_IS_HEADER_LESS;
      cliMbcSetting.processorType = CommMBCDataType::CommMbcJsonData;
      ret = this->http_Client_0->inititialize(cliMbcSetting);
#ifdef EX_COMM_MANAGER_CONSOLE_DEBUG_ENABLE
      CONSOLE_LOG_BUF(exCommMgrLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[exCMgr] stHtCl %i %i", 2, ret);
#endif // EX_COMM_MANAGER_CONSOLE_DEBUG_ENABLE
      if (ret != 0)
      {
        break;
      }
    }
    // reg message id
    this->http_Client_0->registerMessageId(CommMBCConstant::CommMBCMessageId::CommMBCHeadLess, FEM_HTTP_CLIENT_BUFF_SIZE);
    // start task
    {
      CommMBCHttpTaskConfigTAG httpTaskConfig = CommMBCHttpTaskConfigTAG();
      httpTaskConfig.taskManagerConfig.eventItemNumber = FEM_HTTP_CLIENT_TASK_EVENT_NUM;
      httpTaskConfig.taskManagerConfig.eventUsePool = FEM_HTTP_CLIENT_TASK_EVENT_USE_POOL;
      httpTaskConfig.taskThreadConfig.enabled = true;
      httpTaskConfig.taskThreadConfig.useThreadPool = FEM_HTTP_CLIENT_TASK_USE_POOL;
      httpTaskConfig.taskThreadConfig.usStackDepth = FEM_HTTP_CLIENT_TASK_MEM;
      httpTaskConfig.taskThreadConfig.uxPriority = FEM_HTTP_CLIENT_TASK_PRIORITY;
      httpTaskConfig.bufferConfig.usePool = FEM_HTTP_CLIENT_BUFF_USE_POOL;
      httpTaskConfig.bufferConfig.preparedDataNumber = FEM_HTTP_CLIENT_BUFF_COUNT;
      httpTaskConfig.bufferConfig.dataItemConfig.bufferSize = FEM_HTTP_CLIENT_BUFF_SIZE;
      ret = this->http_Client_0->startTask(httpTaskConfig);
#ifdef EX_COMM_MANAGER_CONSOLE_DEBUG_ENABLE
      CONSOLE_LOG_BUF(exCommMgrLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[exCMgr] stHtCl %i %i", 3, ret);
#endif // EX_COMM_MANAGER_CONSOLE_DEBUG_ENABLE
      if (ret != 0)
      {
        break;
      }
    }
    

#ifdef EX_COMM_MANAGER_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(exCommMgrLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[exCMgr] stHtCl %i", 99);
#endif // EX_COMM_MANAGER_CONSOLE_DEBUG_ENABLE
    return 0;
  } while (0);
  this->stopTaskHttpClient0();
#ifdef EX_COMM_MANAGER_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(exCommMgrLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[exCMgr] stHtCl %i", -99);
#endif // EX_COMM_MANAGER_CONSOLE_DEBUG_ENABLE
  return -1;
}

int ExCommManager::startCommHttpClient0(bool waitCompletion)
{
  int ret = 0;
  do
  {
    if (this->http_Client_0 == NULL)
    {
      return -1; // invalid
    }
    // start conn
    {
      CommMBCHttpConnectionConfigTAG httpConnConfig = CommMBCHttpConnectionConfigTAG();
      httpConnConfig.config.client.url = FEM_HTTP_CLIENT_PATH;
      httpConnConfig.config.client.certPem = FEM_HTTP_CLIENT_REQ_CERT;
      ret = this->http_Client_0->startHttp(httpConnConfig, waitCompletion);
#ifdef EX_COMM_MANAGER_CONSOLE_DEBUG_ENABLE
      CONSOLE_LOG_BUF(exCommMgrLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[exCMgr] staCHtCl %i %i", 0, ret);
#endif // EX_COMM_MANAGER_CONSOLE_DEBUG_ENABLE
      if (ret != 0)
      {
        break;
      }
    }

    return 0;
  } while (0);
  return -1;
}

void ExCommManager::stopCommHttpClient0(bool waitCompletion)
{
  do
  {
    if (this->http_Client_0 == NULL)
    {
      return; // invalid
    }

    this->http_Client_0->stopHttp(waitCompletion);

    return;
  } while (0);
  return;
}

void ExCommManager::stopTaskHttpClient0(void)
{
  do
  {
    if (this->http_Client_0 != NULL)
    {
      this->http_Client_0->stopTask();
      delete this->http_Client_0;
    }

    this->http_Client_0 = NULL;
    return;
  } while (0);
  return;
}

// server
int ExCommManager::startTaskHttpServer0(void)
{
  int ret = 0;
  do
  {
    if (this->http_Server_0 != NULL)
    {
#ifdef EX_COMM_MANAGER_CONSOLE_DEBUG_ENABLE
      CONSOLE_LOG_BUF(exCommMgrLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[exCMgr] stHtSv %i", -1);
#endif // EX_COMM_MANAGER_CONSOLE_DEBUG_ENABLE
      return -1; //already
    }
    this->stopTaskHttpServer0();

    this->http_Server_0 = new CommMBCHttpServer();
    if (this->http_Server_0 == NULL)
    {
      break;
    }

    {
      CommMBCSettingParamTAG serMbcSetting = CommMBCSettingParamTAG();
      serMbcSetting.enabled = true;
      serMbcSetting.isHeaderLess = FEM_HTTP_SERVER_IS_HEADER_LESS;
      serMbcSetting.processorType = CommMBCDataType::CommMbcJsonData;
      ret = this->http_Server_0->inititialize(serMbcSetting);
#ifdef EX_COMM_MANAGER_CONSOLE_DEBUG_ENABLE
      CONSOLE_LOG_BUF(exCommMgrLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[exCMgr] stHtSv %i %i", 2, ret);
#endif // EX_COMM_MANAGER_CONSOLE_DEBUG_ENABLE
      if (ret != 0)
      {
        break;
      }
    }
    // reg message id
    this->http_Server_0->registerMessageId(CommMBCConstant::CommMBCMessageId::CommMBCHeadLess, FEM_HTTP_SERVER_BUFF_SIZE);
    this->http_Server_0->registerMessageId(CommMBCConstant::CommMBCMessageId::CommMBCSystemSetting);
    
    // start task
    {
      CommMBCHttpTaskConfigTAG httpTaskConfig = CommMBCHttpTaskConfigTAG();
      httpTaskConfig.taskManagerConfig.eventItemNumber = FEM_HTTP_SERVER_TASK_EVENT_NUM;
      httpTaskConfig.taskManagerConfig.eventUsePool = FEM_HTTP_SERVER_TASK_EVENT_USE_POOL;
      httpTaskConfig.taskThreadConfig.enabled = true;
      httpTaskConfig.taskThreadConfig.useThreadPool = FEM_HTTP_SERVER_TASK_USE_POOL;
      httpTaskConfig.taskThreadConfig.usStackDepth = FEM_HTTP_SERVER_TASK_MEM;
      httpTaskConfig.taskThreadConfig.uxPriority = FEM_HTTP_SERVER_TASK_PRIORITY;
      httpTaskConfig.bufferConfig.usePool = FEM_HTTP_SERVER_BUFF_USE_POOL;
      httpTaskConfig.bufferConfig.preparedDataNumber = FEM_HTTP_SERVER_BUFF_COUNT;
      httpTaskConfig.bufferConfig.dataItemConfig.bufferSize = FEM_HTTP_SERVER_BUFF_SIZE;
      ret = this->http_Server_0->startTask(httpTaskConfig);
#ifdef EX_COMM_MANAGER_CONSOLE_DEBUG_ENABLE
      CONSOLE_LOG_BUF(exCommMgrLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[exCMgr] stHtSv %i %i", 3, ret);
#endif // EX_COMM_MANAGER_CONSOLE_DEBUG_ENABLE
      if (ret != 0)
      {
        break;
      }
    }


#ifdef EX_COMM_MANAGER_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(exCommMgrLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[exCMgr] stHtSv %i", 99);
#endif // EX_COMM_MANAGER_CONSOLE_DEBUG_ENABLE
    return 0;
  } while (0);
  this->stopTaskHttpServer0();
#ifdef EX_COMM_MANAGER_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(exCommMgrLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[exCMgr] stHtSv %i", -99);
#endif // EX_COMM_MANAGER_CONSOLE_DEBUG_ENABLE
  return -1;
}

int ExCommManager::startCommHttpServer0(bool waitCompletion)
{
  int ret = 0;
  do
  {
    if (this->http_Server_0 == NULL)
    {
      return -1; // invalid
    }
    // start conn
    {
      CommMBCHttpConnectionConfigTAG httpConnConfig = CommMBCHttpConnectionConfigTAG();
      httpConnConfig.config.server.basePath = FEM_HTTP_SERVER_BASE_PATH;
      httpConnConfig.config.server.port = FEM_HTTP_SERVER_PORT;
      httpConnConfig.config.server.uriMaxCount = FEM_HTTP_SERVER_URI_COUNT_MAX;
      httpConnConfig.config.server.connMaxCount = FEM_HTTP_SERVER_CONN_COUNT_MAX;
      httpConnConfig.config.server.addHeaderMaxCount = FEM_HTTP_SERVER_ADD_HEADER_COUNT_MAX;
      httpConnConfig.config.server.backlogConn = FEM_HTTP_SERVER_BACKLOG_CONN;
      ret = this->http_Server_0->startHttp(httpConnConfig);
#ifdef EX_COMM_MANAGER_CONSOLE_DEBUG_ENABLE
      CONSOLE_LOG_BUF(exCommMgrLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[exCMgr] staCHtSv %i %i", 0, ret);
#endif // EX_COMM_MANAGER_CONSOLE_DEBUG_ENABLE
      if (ret != 0)
      {
        break;
      }

      {
        // start uri handlers
        // uri 1
        CommMBCHttpUriInfoTAG uriInfo = CommMBCHttpUriInfoTAG();
        uriInfo.dataType = FEM_HTTP_SERVER_URI_1_DATATYPE;
        uriInfo.id = FEM_HTTP_SERVER_URI_1_ID;
        uriInfo.method = FEM_HTTP_SERVER_URI_1_METHOD;
        uriInfo.reqCallback = (void*)ExCommManager::cbHttpUriHandlerServer0;
        uriInfo.userArg = this;// this->http_Server_0;
        uriInfo.uri = FEM_HTTP_SERVER_URI_1_URI;
        ret = this->http_Server_0->regUriHandler(uriInfo);
#ifdef EX_COMM_MANAGER_CONSOLE_DEBUG_ENABLE
        CONSOLE_LOG_BUF(exCommMgrLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[exCMgr] staCHtSv %i %i", 2, ret);
#endif // EX_COMM_MANAGER_CONSOLE_DEBUG_ENABLE
        if (ret != 0)
        {
          break;
        }
        
        // uri 2
        uriInfo.dataType = FEM_HTTP_SERVER_URI_2_DATATYPE;
        uriInfo.id = FEM_HTTP_SERVER_URI_2_ID;
        uriInfo.method = FEM_HTTP_SERVER_URI_2_METHOD;
        uriInfo.reqCallback = (void*)ExCommManager::cbHttpUriHandlerServer0;
        uriInfo.userArg = this;// this->http_Server_0;
        uriInfo.uri = FEM_HTTP_SERVER_URI_2_URI;
        ret = this->http_Server_0->regUriHandler(uriInfo);
#ifdef EX_COMM_MANAGER_CONSOLE_DEBUG_ENABLE
        CONSOLE_LOG_BUF(exCommMgrLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[exCMgr] staCHtSv %i %i", 3, ret);
#endif // EX_COMM_MANAGER_CONSOLE_DEBUG_ENABLE
        if (ret != 0)
        {
          break;
        }
        
        ret = this->http_Server_0->regDefaultUriHandler();
#ifdef EX_COMM_MANAGER_CONSOLE_DEBUG_ENABLE
        CONSOLE_LOG_BUF(exCommMgrLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[exCMgr] staCHtSv %i %i", 4, ret);
#endif // EX_COMM_MANAGER_CONSOLE_DEBUG_ENABLE
        if (ret != 0)
        {
          break;
        }
      }

    }

    return 0;
  } while (0);
  return -1;
}

void ExCommManager::stopCommHttpServer0(bool waitCompletion)
{
  do
  {
    if (this->http_Server_0 == NULL)
    {
      return; // invalid
    }

    this->http_Server_0->stopHttp(waitCompletion);

    return;
  } while (0);
  return;
}

void ExCommManager::stopTaskHttpServer0(void)
{
  do
  {
    if (this->http_Server_0 != NULL)
    {
      this->http_Server_0->stopTask();
      delete this->http_Server_0;
    }

    this->http_Server_0 = NULL;
    return;
  } while (0);
  return;
}


int ExCommManager::cbHttpUriHandlerServer(CommMBCHttpServer* mbcHttpServer, CommMBCHttpUriRequestTAG* uriReqInfo)
{
  int ret = 0;
  CommMBCHttpPackageTAG* reqPackage = &uriReqInfo->reqData;
  CommMBCHttpPackageTAG* resPackage = &uriReqInfo->resData;
  CommMBCHttpServer* serverHandler = mbcHttpServer;
  bool releaseReqPackage = true;
  int error = 0;
  do
  {
    CommMBCHttpHeaderTAG* reqHeader = (CommMBCHttpHeaderTAG*)reqPackage->header();
    CONSOLE_LOG_BUF(exCommMgrLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[exCMgr] %s: %i id=%i mId=%i", "cbUH", 0, reqHeader->uriId, uriReqInfo->reqMessageId);
    CONSOLE_LOG_BUF(exCommMgrLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "%s", reqHeader->uri);
    if (resPackage->setData(serverHandler->dataManager()) != 0)
    {
      break;
    }

    ExCommMBCParamTAG mbcParam = ExCommMBCParamTAG();
    mbcParam.commId = (mbcHttpServer == this->http_Server_0? EX_COMM_ID_MBC_HTTP_SERVER_0 :EX_COMM_ID_INVALID);
    mbcParam.reqPackage.valid = true;
    mbcParam.reqPackage.messId = uriReqInfo->reqMessageId;
    mbcParam.reqPackage.data = (unsigned char*)reqPackage->body();
    mbcParam.reqPackage.dataSize = reqPackage->bodyLen();
    mbcParam.reqPackage.dataItem = reqPackage->getDataItem();
    mbcParam.reqPackage.dataManager = reqPackage->getDataManager();


    mbcParam.resPackage.valid = true;
    mbcParam.resPackage.messId = CommMBCConstant::CommMBCMessageIdInvalid; // upside need to set
    mbcParam.resPackage.data = NULL; // upside can use and write data 
    mbcParam.resPackage.dataSize = 0; // upside need to set
    mbcParam.resPackage.dataItem = NULL;// // upside need to set // resPackage->getDataItem();
    mbcParam.resPackage.dataManager = NULL; // // upside need to set // resPackage->getDataManager();

    // procceed the mbc message
    ret = this->cb_Comm_Rev(this->cb_Comm_Rev_Arg, mbcParam);// this->mbcMessageProceed(mbcParam, EX_COMM_MBC_HTTP_SERVER_WAIT_RES_TIMEOUT);
    if (ret != 0)
    {
      break;
    }

    releaseReqPackage = false;// upper side inside mbcMessageProceed takes responsibility to release

    // send response
    CommMBCHttpHeaderTAG* resHeader = (CommMBCHttpHeaderTAG*)resPackage->header();
    uriReqInfo->resMessageId = mbcParam.resPackage.messId;
    resHeader->fileData = 0;
    resHeader->resStatusCode = COMM_HTTP_STATUS_CODE_200;
    resHeader->dataType = reqHeader->dataType;
    CONSOLE_LOG_BUF(exCommMgrLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[exCMgr] %i %i", 20, resHeader->dataType);
    MbcDataSize_t encodedSize = 0;
    if (mbcParam.resPackage.valid != false)
    {
      ret = serverHandler->setResponse(uriReqInfo->resMessageId, mbcParam.resPackage.data, mbcParam.resPackage.dataSize, (unsigned char*)resPackage->body(), resPackage->bodyMaxLen(), encodedSize);
    }
    
    mbcParam.resPackage.release(); //if any

    if (ret != 0)
    {
      break;
    }
    resPackage->bodyLen(encodedSize);
    //reqPackage->releaseData(); // upper side inside mbcMessageProceed takes responsibility to release
    return 0;
  } while (0);
  if (releaseReqPackage != false)
  {
    reqPackage->releaseData();
  }
  resPackage->releaseData();
  return -1;
}


int ExCommManager::sendCommHttpClient(CommMBCHttpClient* httpClient, ExCommMBCParamTAG& mbcParams, TimePoint_t resWaitTimeMs)
{
  int ret = 0;
  do
  {
    if (this->http_Client_0 == NULL)
    {
      break;
    }

    {
      SystemMutex clientReqMutex;
      clientReqMutex.unlock();
      clientReqMutex.lock();
      HttpReqId_t reqId = 0;
      CommMBCHttpClientRequestParamsTAG requestParam = CommMBCHttpClientRequestParamsTAG();
      requestParam.header.dataType = FEM_HTTP_CLIENT_REQ_DATATYPE;
      requestParam.header.fileData = 0;
      requestParam.header.reqMethod = FEM_HTTP_CLIENT_REQ_METHOD;
      requestParam.header.uri = FEM_HTTP_CLIENT_REQ_URI;
      requestParam.header.uriId = FEM_HTTP_CLIENT_REQ_URI_ID;

      requestParam.reqMessageId = mbcParams.reqPackage.messId;

      requestParam.body = (char*)mbcParams.reqPackage.data;
      requestParam.bodyLen = mbcParams.reqPackage.dataSize;

      requestParam.notifier.agr = NULL;
      requestParam.notifier.callback = NULL;
      requestParam.notifier.expiredTime = 0;

      mbcParams.resSpec.flag = 1;
      mbcParams.resSpec.mutex = NULL;
      //mbcParams.resSpec.expiredTime = 0;

      if (resWaitTimeMs > 0)
      {
        resWaitTimeMs = SYSTEM_MAX(EX_COMM_SEND_RES_WAIT_TIME_MIN, resWaitTimeMs);
        mbcParams.exCommMgr = this;
        mbcParams.resSpec.flag = 0;
        mbcParams.resSpec.mutex = &clientReqMutex;
        //mbcParams.resSpec.expiredTime = TimerManager::getInstance().nowMs() + (resWaitTimeMs + EX_COMM_SEND_RES_WAIT_TIME_COFF);
        requestParam.notifier.expiredTime = TimerManager::getInstance().now() + (resWaitTimeMs + EX_COMM_SEND_RES_WAIT_TIME_COFF) * TIMER_DEVICE_TIMER_MS_2_UNIT;
        requestParam.notifier.callback = ExCommManager::cbHttpClientResponseClient0;
        requestParam.notifier.agr = &mbcParams;
      }

      ret = httpClient->request(requestParam, reqId);
      CONSOLE_LOG_BUF(exCommMgrLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[%s]: %i %d", "set", 95, reqId);
      if (ret != 0)
      {
        break;
      }

      //release
      clientReqMutex.lock(resWaitTimeMs);
      if (mbcParams.resSpec.flag == 0)
      {
        break;
      }
      //mbcParams.resPackage.release();
    }

    return 0;
  } while (0);
  return -1;
}

int ExCommManager::cbHttpClientResponseClient(CommMBCHttpClient* mbcHttpClient, ExCommMBCParamTAG* mbcParams, CommMBCHttpUriRequestTAG* uriRequest)
{
  CommMBCHttpPackageTAG* resPackage = &uriRequest->resData;
  do
  {
    if (mbcHttpClient == NULL)
    {
      break;
    }

    CommMBCHttpHeaderTAG* resHeader = (CommMBCHttpHeaderTAG*)resPackage->header();
    mbcParams->resPackage.messId = uriRequest->resMessageId;
    mbcParams->resPackage.data = (unsigned char*)resPackage->body();
    mbcParams->resPackage.dataSize = resPackage->bodyLen();
    mbcParams->resPackage.dataItem = resPackage->getDataItem();
    mbcParams->resPackage.dataManager = resPackage->getDataManager();

    //// check legacy
    //if ((mbcParams->resSpec.i > 0)
    //  && (TimerManager::getInstance().nowMs() > mbcParams->resSpec.expiredTime)
    //  )
    //{
    //  break; // went off.
    //}

    mbcParams->resSpec.flag++;
    mbcParams->resSpec.mutex->unlock();
    return 0;
  } while (0);

  resPackage->releaseData();
  return -1;
}

//
//int ExCommManager::mbcMessageProceed(ExCommMBCParamTAG& mbcParams, TimePoint_t resWaitTimeMs)
//{
//  int ret = 0;
//  bool notify = false;
//  do
//  {
//    mbcParams.resPackage.valid = true;
//    mbcParams.resPackage.messId = mbcParams.reqPackage.messId;
//    mbcParams.resPackage.data = mbcParams.reqPackage.data;
//    mbcParams.resPackage.dataSize = mbcParams.reqPackage.dataSize;
//
//    switch (mbcParams.reqPackage.messId)
//    {
//      case (int)CommMBCConstant::CommMBCMessageId::CommMBCSystemSetting:
//        notify = true;
//        break;
//      default:
//        break;
//    }
//
//    if (notify == false)
//    {
//      return 0;
//    }
//
//    ret = this->notifyParent((int)EventManagerConstant::EventMessageId::ExCommReceived, sizeof(EventExCommReceivedTAG), (unsigned char*)&mbcParams);
//    if (ret != 0)
//    {
//      break;
//    }
//
//    return 0;
//  } while (0);
//  return -1;
//}

int ExCommManager::cbHttpUriHandlerServer0(void* arg, CommMBCHttpUriRequestTAG* uriReqInfo)
{
  ExCommManager* exComm= (ExCommManager*)(arg);
  return exComm->cbHttpUriHandlerServer(exComm->http_Server_0, uriReqInfo);
}

int ExCommManager::cbHttpClientResponseClient0(void* arg, CommMBCHttpUriRequestTAG* uriRequest)
{
  do
  {
    if (arg == NULL)
    {
      break;
    }

    ExCommMBCParamTAG* excommParam = (ExCommMBCParamTAG*)arg;
    if (excommParam->exCommMgr == NULL)
    {
      break;
    }

    switch (excommParam->commId)
    {
      case EX_COMM_ID_MBC_HTTP_CLIENT_0:
        return excommParam->exCommMgr->cbHttpClientResponseClient(excommParam->exCommMgr->http_Client_0, excommParam, uriRequest);
      default:
        return -1;
    }

    return 0;
  } while (0);
  return -1;
}
#endif // _CONF_EX_COMM_MANAGER_ENABLED