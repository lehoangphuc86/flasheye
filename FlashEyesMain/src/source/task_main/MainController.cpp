/////////////////////////////////////////////////
// INCLUDE
#include "MainController.h"
#if (_CONF_MAIN_CONTROLLER_ENABLED)
/////////////////////////////////////////////////
// PREPROCESSOR
#define MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE

/////////////////////////////////////////////////
// DEFINE

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
#ifdef MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
char mainCtrlLogBuf[SYSTEM_CONSOLE_OUT_BUF_LEN];
#endif // MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
/////////////////////////////////////////////////
// STATIC FUNCTIONS

/////////////////////////////////////////////////
// EXTERN

/////////////////////////////////////////////////
// CLASS IMPLEMENTAION
//
/*MainController*/
MainController::MainController(byte systemMode)
  : system_Mode(systemMode)
  , is_Busy(false)
  , sequence_Id(FEM_SCAN_SEQ_ID_INVALID)
{

}

MainController::~MainController(void)
{
  this->stop();
}

bool MainController::isValid(void)
{
  return (this->system_Mode >= SystemMode::SystemModeMax ? false : true);
}

byte MainController::systemMode(void)
{
  return this->system_Mode;
}

bool MainController::isBusy(void)
{
  SystemCriticalLocker locker(this->is_Busy_Key);
  return this->is_Busy;
}

BufferDataItem* MainController::getCommData(void)
{
  return (BufferDataItem*)this->data_Manager.get();
}

BufferDataManager* MainController::dataManager(void)
{
  return &this->data_Manager;
}

void MainController::releaseCommData(BufferDataItem* dataItem)
{
  if (dataItem != NULL)
  {
    this->data_Manager.release(dataItem);
  }
}

int MainController::start(MainControllerConfigTAG& controllerConfig)
{
  int ret = 0;
#ifdef MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(mainCtrlLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mTsk] sta %i", 0);
#endif // MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
  do
  {
    
    if (this->isTaskRunning() != false)
    {
      return 0; // already running
    }

    this->stop();

    // data manager
    ret = this->data_Manager.prepare(controllerConfig.bufferConfig);
#ifdef MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(mainCtrlLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mTsk] sta %i %i", 3, ret);
#endif // MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
    if (ret != 0)
    {
      break;
    }

    // Set up tasks
    // self
    this->registerHanldingEventStructSize(sizeof(EventPrepareCompletedTAG));
    this->regEventSize();
    ret = TaskManager::setConfig(controllerConfig.taskManagerConfig);
#ifdef MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(mainCtrlLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mTsk] sta %i %i", 5, ret);
#endif // MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
    if (ret != 0)
    {
      break;
    }

    ret = TaskManager::prepare();
#ifdef MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(mainCtrlLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mTsk] sta %i %i", 6, ret);
#endif // MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
    if (ret != 0)
    {
      break;
    }
    
    ret = TaskManager::startProcess(controllerConfig.taskThreadConfig, true);
#ifdef MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(mainCtrlLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mTsk] sta %i %i", 7, ret);
#endif // MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
    if (ret != 0)
    {
      break;
    }
#ifdef MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(mainCtrlLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mTsk] sta %i", 99);
#endif // MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
    return ret;
  } while (0);

  this->stop();
#ifdef MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(mainCtrlLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mTsk] sta %i", -99);
#endif // MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
  return -1;
}

void MainController::stop(void)
{
  do
  {
    this->data_Manager.releaseAll();
    TaskManager::stopProcess();
    return ;
  } while (0);
  return;
}

void MainController::isBusy(bool flag)
{
  SystemCriticalLocker locker(this->is_Busy_Key);
  this->is_Busy = flag;
}

Seq_t MainController::curSeqId(void)
{
  return this->sequence_Id;
}

Seq_t MainController::nextSeqId(void)
{
  this->sequence_Id++;
  if (this->sequence_Id == 0)
  {
    this->sequence_Id++;
  }
  return this->sequence_Id;
}

void MainController::proc(void)
{
#ifdef MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(mainCtrlLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mTsk] proc %i", 0);
#endif // MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
  int prepareRet = this->prepare();
  this->reportPrepareResult(prepareRet);
  if (prepareRet != 0)
  {
    this->waitTerminating();
    this->clear();
    return;
  }
  EventDataItem* eventData = NULL;
  bool isStop = false;
  while (isStop == false) // A Task shall never return or exit.
  {
    eventData = this->event_Manager.wait(1000);//@@ TASK_MANAGER_EVENT_WAIT_TIME_DEFAULT);
    if (eventData == NULL)
    {
      this->onEventHandling(eventData);
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
    default:
      this->onEventHandling(eventData);
      break;
    }
    this->event_Manager.release(eventData);
  }

  this->clear();
#ifdef MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(mainCtrlLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mTsk] proc %i", 99);
#endif // MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
}

// prepare/start sub tasks
int MainController::prepare(void)
{
  
  return 0;
}

// stop sub tasks
void MainController::clear(void)
{
  
}

//
//#include "main.h"
//#include "task_main/MainControllerFactory.h"
//
//#include "file_system/FileSystemManager.h"
//#include "db/DBManager.h"
////#include "task_http/CommHttpServer.h"
////#include "task_http/CommHttpClient.h"
//#include "task_comm_mbc/task_comm_mbc_http/CommMBCHttpServer.h"
//#include "task_comm_mbc/task_comm_mbc_http/CommMBCHttpClient.h"
//#include "task_net/wifi/WifiControllerTask.h"
//#include "timer_manager/TimerManager.h"
//#include "utility/JsonParser.h"
///////////////////////////////////////////////////
//// PREPROCESSOR
//#define MAIN_CONSOLE_DEBUG_ENABLE
//
//#define MAIN_WIFI_SSID                              "SamsungJ7"
//#define MAIN_WIFI_PWD                               "donotconnect"
//
//#define MAIN_WIFI_AP_SSID                           "SamsungJ7_2"
//#define MAIN_WIFI_AP_PWD                            "donotconnect"
//
//#define MAIN_WIFI_STA_NET_STATIC_IP                 NET_IP4_INIT(192, 168, 137, 100)
//#define MAIN_WIFI_STA_NET_STATIC_NETMARK            NET_IP4_INIT(255, 255, 255, 0)
//#define MAIN_WIFI_STA_NET_STATIC_GW                 NET_IP4_INIT(192, 168, 137, 1)
//
//#define MAIN_WIFI_AP_NET_STATIC_IP                  NET_IP4_INIT(192, 169, 0, 100)
//#define MAIN_WIFI_AP_NET_STATIC_NETMARK             NET_IP4_INIT(255, 255, 255, 0)
//#define MAIN_WIFI_AP_NET_STATIC_GW                  NET_IP4_INIT(192, 169, 0, 1)
//
//
///////////////////////////////////////////////////
//// DEFINE
//
///////////////////////////////////////////////////
//// MARCO
//
///////////////////////////////////////////////////
//// GLOBAL VARIABLES
//#ifdef MAIN_CONSOLE_DEBUG_ENABLE
//char mainBufLog[SYSTEM_CONSOLE_OUT_BUF_LEN];
//#endif // MAIN_CONSOLE_DEBUG_ENABLE
///////////////////////////////////////////////////
//// GLOBAL FUNCTIONS
//
///////////////////////////////////////////////////
//// DATA TYPE (ENUM)
//
///////////////////////////////////////////////////
//// DATA TYPE (STRUCT)
//
///////////////////////////////////////////////////
//// STATIC DATA
//
///////////////////////////////////////////////////
//// STATIC FUNCTIONS
//
///////////////////////////////////////////////////
//// EXTERN
//
///////////////////////////////////////////////////
//// CLASS IMPLEMENTAION
//
///////////////////////////////////////////////////
//// CLASS IMPLEMENTATION
//
////// db
////const char* data = "Callback function called";
////static int callback(void* data, int argc, char** argv, char** azColName) {
////  int i;
////  Serial.printf("%s: ", (const char*)data);
////  for (i = 0; i < argc; i++) {
////    Serial.printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
////  }
////  Serial.printf("\n");
////  return 0;
////}
//
//// http server 
//int cbCommHttpUriHandler(void* arg, CommMBCHttpUriRequestTAG* uriReqInfo)
//{
//  int ret = 0;
//  CommMBCHttpPackageTAG* reqPackage = &uriReqInfo->reqData;
//  CommMBCHttpPackageTAG* resPackage = &uriReqInfo->resData;
//  CommMBCHttpServer* serverHandler = (CommMBCHttpServer*)(arg);
//  int error = 0;
//  do
//  {
//    CommMBCHttpHeaderTAG* reqHeader = (CommMBCHttpHeaderTAG*)reqPackage->header();
//    CONSOLE_LOG_BUF(mainBufLog, SYSTEM_CONSOLE_OUT_BUF_LEN, "[m] %s: %i id=%i mId=%i", "cbUH", 0, reqHeader->uriId, uriReqInfo->reqMessageId);
//    CONSOLE_LOG_BUF(mainBufLog, SYSTEM_CONSOLE_OUT_BUF_LEN, "%s", reqHeader->uri);
//    if (resPackage->setData(serverHandler->dataManager()) != 0)
//    {
//      break;
//    }
//
//    // read data
//    switch (uriReqInfo->reqMessageId)
//    {
//    case CommMBCConstant::CommMBCMessageId::CommMBCHeadLess:
//    {
//      char* reqBody = reqPackage->body();
//      reqBody[reqPackage->bodyLen()] = '\0';
//      CONSOLE_LOG_BUF(mainBufLog, SYSTEM_CONSOLE_OUT_BUF_LEN, "%s", reqBody);
//
//      //rep
//      CommMBCHttpHeaderTAG* resHeader = (CommMBCHttpHeaderTAG*)resPackage->header();
//      uriReqInfo->resMessageId = uriReqInfo->reqMessageId;
//      resHeader->fileData = 0;
//      resHeader->resStatusCode = COMM_HTTP_STATUS_CODE_200;
//      resHeader->dataType = reqHeader->dataType;
//
//      char* resBody = resPackage->body();
//      memcpy(resBody, reqBody, reqPackage->bodyLen());
//      resPackage->bodyLen(reqPackage->bodyLen());
//      break;
//    }
//    case CommMBCConstant::CommMBCMessageId::CommMBCSystemSetting:
//    {
//      CommMBCSystemSettingTAG* mbcRevTag = (CommMBCSystemSettingTAG*)reqPackage->body();
//      CONSOLE_LOG_BUF(mainBufLog, SYSTEM_CONSOLE_OUT_BUF_LEN, "[m] %s: %i %i %i %i %i", "cbUH", 10, mbcRevTag->sectionId, mbcRevTag->settingId, mbcRevTag->errorCode, mbcRevTag->sLen);
//      CONSOLE_LOG_BUF(mainBufLog, SYSTEM_CONSOLE_OUT_BUF_LEN, "[m] %s: %i %i %i %i", "cbUH", 11, mbcRevTag->bitSet1.dataType, mbcRevTag->bitSet1.isReply, mbcRevTag->bitSet1.isUpdate);
//      if (mbcRevTag->bitSet1.dataType == COMM_MBC_DATA_T_STRING)
//      {
//        CONSOLE_LOG_BUF(mainBufLog, SYSTEM_CONSOLE_OUT_BUF_LEN, "[m] %s", mbcRevTag->data.sVal);
//      }
//      else
//      {
//        CONSOLE_LOG_BUF(mainBufLog, SYSTEM_CONSOLE_OUT_BUF_LEN, "[m] %f", mbcRevTag->data.dVal);
//      }
//
//      // echo
//      CommMBCHttpHeaderTAG* resHeader = (CommMBCHttpHeaderTAG*)resPackage->header();
//      uriReqInfo->resMessageId = uriReqInfo->reqMessageId;
//      resHeader->fileData = 0;
//      resHeader->resStatusCode = COMM_HTTP_STATUS_CODE_200;
//      resHeader->dataType = reqHeader->dataType;
//      CONSOLE_LOG_BUF(mainBufLog, SYSTEM_CONSOLE_OUT_BUF_LEN, "[m] %i %i", 20, resHeader->dataType);
//      MbcDataSize_t encodedSize = 0;
//      ret = serverHandler->setResponse(uriReqInfo->resMessageId, (unsigned char*)mbcRevTag, sizeof(CommMBCSystemSettingTAG), (unsigned char*)resPackage->body(), resPackage->bodyMaxLen(), encodedSize);
//      if (ret != 0)
//      {
//        error = -1;
//        break;
//      }
//      resPackage->bodyLen(encodedSize);
//      break;
//    }
//    default:
//      error = -1;
//      break;
//    }
//
//    if (error != 0)
//    {
//      break;
//    }
//
//    reqPackage->releaseData();
//    return 0;
//  } while (0);
//  reqPackage->releaseData();
//  resPackage->releaseData();
//  return -1;
//}
//
//
//int cbHttpClientResponse(void* arg, CommMBCHttpUriRequestTAG* uriRequest)
//{
//  CommMBCHttpPackageTAG* resPackage = &uriRequest->resData;
//  do
//  {
//    if (arg == NULL)
//    {
//      break;
//    }
//
//    CONSOLE_LOG_BUF(mainBufLog, SYSTEM_CONSOLE_OUT_BUF_LEN, "[m] %s %i rId=%i", "cbCR", 1, uriRequest->requestId);
//    CONSOLE_LOG_BUF(mainBufLog, SYSTEM_CONSOLE_OUT_BUF_LEN, "[m] %s %i reqM=%i resM=%i", "cbCR", 2, uriRequest->reqMessageId, uriRequest->resMessageId);
//    CommMBCHttpHeaderTAG* resHeader = (CommMBCHttpHeaderTAG*)resPackage->header();
//    char* resBody = resPackage->body();
//    CONSOLE_LOG_BUF(mainBufLog, SYSTEM_CONSOLE_OUT_BUF_LEN, "[m] %s", (resHeader->uri == NULL ? "nul" : resHeader->uri));
//    CONSOLE_LOG_BUF(mainBufLog, SYSTEM_CONSOLE_OUT_BUF_LEN, "[m] %s %i sc=%d dt=%d", "cbCR", 3, resHeader->resStatusCode, resHeader->dataType);
//    CONSOLE_LOG_BUF(mainBufLog, SYSTEM_CONSOLE_OUT_BUF_LEN, "[m] %s %i bL=%d", "cbCR", 4, resPackage->bodyLen());
//
//    switch (uriRequest->resMessageId)
//    {
//    case CommMBCConstant::CommMBCMessageId::CommMBCHeadLess:
//    {
//      resBody[resPackage->bodyLen()] = '\0';
//      CONSOLE_LOG_BUF(mainBufLog, SYSTEM_CONSOLE_OUT_BUF_LEN, "[m] %s", resBody);
//      break;
//    }
//    case CommMBCConstant::CommMBCMessageId::CommMBCSystemSetting:
//    {
//      CommMBCSystemSettingTAG* mbcRevTag = (CommMBCSystemSettingTAG*)resBody;
//      CONSOLE_LOG_BUF(mainBufLog, SYSTEM_CONSOLE_OUT_BUF_LEN, "[m] %s: %i %i %i %i %i", "cbCR", 10, mbcRevTag->sectionId, mbcRevTag->settingId, mbcRevTag->errorCode, mbcRevTag->sLen);
//      CONSOLE_LOG_BUF(mainBufLog, SYSTEM_CONSOLE_OUT_BUF_LEN, "[m] %s: %i %i %i %i", "cbCR", 11, mbcRevTag->bitSet1.dataType, mbcRevTag->bitSet1.isReply, mbcRevTag->bitSet1.isUpdate);
//      if (mbcRevTag->bitSet1.dataType == COMM_MBC_DATA_T_STRING)
//      {
//        CONSOLE_LOG_BUF(mainBufLog, SYSTEM_CONSOLE_OUT_BUF_LEN, "[m] %s %s", "cbCR", mbcRevTag->data.sVal);
//      }
//      else
//      {
//        CONSOLE_LOG_BUF(mainBufLog, SYSTEM_CONSOLE_OUT_BUF_LEN, "[m] %s %f", "cbCR", mbcRevTag->data.dVal);
//      }
//
//      break;
//    }
//    default:
//      break;
//    }
//
//    resPackage->releaseData();
//    ((SystemMutex*)arg)->unlock();
//    return 0;
//  } while (0);
//
//  resPackage->releaseData();
//  return -1;
//}
//
//static WifiControllerTask wifiTask;
//static CommMBCHttpServer httpServer;
//static CommMBCHttpClient httpClient;
///*main*/
//int main(void)
//{
//
//  int result = 0;
//  do
//  {
//    // initialize serial communication at 9600 bits per second:
//    SYSTEM_CONSOLE_HANDLER.begin(56000);
//    while (!SYSTEM_CONSOLE_HANDLER)
//    {
//      ; // wait for serial port to connect. Needed for native USB, on LEONARDO, MICRO, YUN, and other 32u4 based boards.
//    }
//
//    SYSTEM_SLEEP(1000);
//    CONSOLE_LOG_BUF(mainBufLog, SYSTEM_CONSOLE_OUT_BUF_LEN, "[%s]: %i", "set", 0);
//    // Set up tasks
//    // start timer 
//
//    //if (0)
//    {
//      TaskManagerConfigTAG taskConfig = TaskManagerConfigTAG();
//      taskConfig.eventUsePool = FEM_TM_USE_POOL;
//      taskConfig.eventItemNumber = FEM_TM_TASK_EVENT_NO; // TIMER_MANAGER_TEST_TASK_EVENT_NO;
//      TaskThreadConfigTAG threadConfig = TaskThreadConfigTAG();
//      threadConfig.enabled = true;
//      threadConfig.useThreadPool = false;
//      threadConfig.usStackDepth = FEM_TM_TASK_MEM;
//      threadConfig.uxPriority = FEM_TM_TASK_PRIORITY;
//      {
//        TimerControlTaskConfigTAG timerConfig = TimerControlTaskConfigTAG();
//        timerConfig.taskManagerConfig = &taskConfig;
//        timerConfig.taskThreadConfig = &threadConfig;
//        timerConfig.hwTimerGrpId = FEM_TM_HW_GROUP_ID;
//        timerConfig.hwTimerIndex = FEM_TM_HW_INDEX_ID;
//        timerConfig.hwTimerDevider = FEM_TM_HW_DEVIDER;
//        timerConfig.hwClockMHz = F_CPU / 1000000;
//
//        if (TimerManager::getInstance().isValid() != false)
//        {
//          CONSOLE_LOG_BUF(mainBufLog, SYSTEM_CONSOLE_OUT_BUF_LEN, "[%s]: %i", "set", -101);
//          return -1; // already config
//        }
//
//        result = TimerManager::getInstance().initialize(timerConfig);
//        if (result != 0)
//        {
//          CONSOLE_LOG_BUF(mainBufLog, SYSTEM_CONSOLE_OUT_BUF_LEN, "[%s]: %i", "set", -102);
//          break;
//        }
//      }
//    }
//
//
//    // file system
//    {
//      FileSystemConfigTAG fsConfig = FileSystemConfigTAG();
//      //memcpy(fsConfig.basePath, FEM_FS_BASE_PATH, SYSTEM_MIN(strlen(FEM_FS_BASE_PATH), FS_MGR_BASE_PATH_LEN_MAX));
//      fsConfig.basePath = FEM_FS_BASE_PATH;
//      fsConfig.partitionLabel = FEM_FS_PARTITION_LABEL;
//      fsConfig.formatIfFailed = FEM_FS_FORMAT;
//      fsConfig.maxOpenFile = FEM_FS_MAX_FILE;
//      CONSOLE_LOG_BUF(mainBufLog, SYSTEM_CONSOLE_OUT_BUF_LEN, "[%s]: %i %s", "set", 1, fsConfig.basePath);
//
//      result = FileSystemManager::getInstance().initialize(fsConfig);
//      if (result != 0)
//      {
//        CONSOLE_LOG_BUF(mainBufLog, SYSTEM_CONSOLE_OUT_BUF_LEN, "[%s]: %i", "set", -1);
//        break;
//      }
//    }
//
//    /*CONSOLE_LOG_BUF(mainBufLog, SYSTEM_CONSOLE_OUT_BUF_LEN, "[%s]: %i", "set", 2);
//    result = FileSystemManager::getInstance().listDir("/", 2);
//    if (result != 0)
//    {
//      CONSOLE_LOG_BUF(mainBufLog, SYSTEM_CONSOLE_OUT_BUF_LEN, "[%s]: %i", "set", -2);
//      break;
//    }
//    */
//    CONSOLE_LOG_BUF(mainBufLog, SYSTEM_CONSOLE_OUT_BUF_LEN, "[%s]: %i", "set", 3);
//
//    {
//      DBManagerConfigTAG dbManagerConfig = DBManagerConfigTAG();
//      dbManagerConfig.dbPath = FEM_DB_PATH;
//      result = DBManager::getInstance().initialize(dbManagerConfig);
//      if (result != 0)
//      {
//        CONSOLE_LOG_BUF(mainBufLog, SYSTEM_CONSOLE_OUT_BUF_LEN, "[%s]: %i", "set", -3);
//        break;
//      }
//    }
//    CONSOLE_LOG_BUF(mainBufLog, SYSTEM_CONSOLE_OUT_BUF_LEN, "[%s]: %i", "set", 4);
//    DBHandler_t dbHandler = DBManager::getInstance().dbHandler();
//    //result = DBManager::getInstance().exec(dbHandler, "CREATE TABLE TBL_CONFIG (id INTEGER PRIMARY KEY, Name TEXT NOT NULL, Type INTEGER NOT NULL, Value TEXT NOT NULL)", callback, (void*)data, NULL);
//    if (result != 0)
//    {
//      CONSOLE_LOG_BUF(mainBufLog, SYSTEM_CONSOLE_OUT_BUF_LEN, "[%s]: %i", "set", -4);
//      break;
//    }
//
//    CONSOLE_LOG_BUF(mainBufLog, SYSTEM_CONSOLE_OUT_BUF_LEN, "[%s]: %i", "set", 5);
//
//    //result = DBManager::getInstance().exec(dbHandler, "INSERT INTO TBL_CONFIG (Name, Type, Value) VALUES('StartMode', 1, '2')", callback, (void*)data, NULL);
//    //result += DBManager::getInstance().exec(dbHandler, "INSERT INTO TBL_CONFIG (Name, Type, Value) VALUES('ScanMode', 2, '3')", callback, (void*)data, NULL);
//    char* tmpSqlCmdBuf = new char[250];
//    result = DBManager::getInstance().exeScriptFile(FEM_DB_TBL_CONFIG_SCRIPT_PATH, tmpSqlCmdBuf, 250);
//    delete[] tmpSqlCmdBuf;
//    tmpSqlCmdBuf = NULL;
//    if (result != 0)
//    {
//      CONSOLE_LOG_BUF(mainBufLog, SYSTEM_CONSOLE_OUT_BUF_LEN, "[%s]: %i", "set", -5);
//      break;
//    }
//    CONSOLE_LOG_BUF(mainBufLog, SYSTEM_CONSOLE_OUT_BUF_LEN, "[%s]: %i", "set", 6);
//    /*result = DBManager::getInstance().exec(dbHandler, "SELECT * FROM TBL_CONFIG", callback, (void*)data, NULL);
//    if (result != 0)
//    {
//      CONSOLE_LOG_BUF(mainBufLog, SYSTEM_CONSOLE_OUT_BUF_LEN, "[%s]: %i", "set", -6);
//      break;
//    }*/
//    char valCh[32] = {};
//    //char* val = valCh;
//    long val = 0;
//    result = DBManager::getInstance().selectCell(DB_TBL_CONFIG_ID, "Type", "id", 2, &val);
//    if (result != 0)
//    {
//      CONSOLE_LOG_BUF(mainBufLog, SYSTEM_CONSOLE_OUT_BUF_LEN, "[%s]: %i", "set", -6);
//      break;
//    }
//    CONSOLE_LOG_BUF(mainBufLog, SYSTEM_CONSOLE_OUT_BUF_LEN, "[%s]: %i %i", "set", 7, val);
//
//    result = DBManager::getInstance().updateCell(DB_TBL_CONFIG_ID, "Type", "id", 2, 5);
//    if (result != 0)
//    {
//      CONSOLE_LOG_BUF(mainBufLog, SYSTEM_CONSOLE_OUT_BUF_LEN, "[%s]: %i", "set", -71);
//      break;
//    }
//
//    result = DBManager::getInstance().selectCell(DB_TBL_CONFIG_ID, "Type", "id", 2, &val);
//    if (result != 0)
//    {
//      CONSOLE_LOG_BUF(mainBufLog, SYSTEM_CONSOLE_OUT_BUF_LEN, "[%s]: %i", "set", -72);
//      break;
//    }
//    CONSOLE_LOG_BUF(mainBufLog, SYSTEM_CONSOLE_OUT_BUF_LEN, "[%s]: %i %i", "set", 73, val);
//
//    //------------------------
//    //if (0)
//    {
//      result = wifiTask.inititialize();
//      CONSOLE_LOG_BUF(mainBufLog, SYSTEM_CONSOLE_OUT_BUF_LEN, "[%s]: %i %i", "set", 8, result);
//      if (result != 0)
//      {
//        break;
//      }
//    }
//
//    //if (0)
//    {
//      WifiTaskConfigTAG wifiTaskConfig = WifiTaskConfigTAG();
//
//      wifiTaskConfig.taskManagerConfig.eventItemNumber = FEM_WIFI_SERVER_TASK_EVENT_NUM;
//      wifiTaskConfig.taskManagerConfig.eventUsePool = false;
//
//
//      wifiTaskConfig.taskThreadConfig.enabled = true;
//      wifiTaskConfig.taskThreadConfig.useThreadPool = false;
//      wifiTaskConfig.taskThreadConfig.usStackDepth = FEM_WIFI_SERVER_TASK_MEM;
//      wifiTaskConfig.taskThreadConfig.uxPriority = FEM_WIFI_SERVER_TASK_PRIORITY;
//
//      result = wifiTask.startTask(wifiTaskConfig);
//      CONSOLE_LOG_BUF(mainBufLog, SYSTEM_CONSOLE_OUT_BUF_LEN, "[%s]: %i %i", "set", 9, result);
//      if (result != 0)
//      {
//        break;
//      }
//    }
//
//    //if (0)
//    {
//      WifiConnectionConfigTAG wifiConfig = WifiConnectionConfigTAG();
//#if (1)
//      wifiConfig.staConfig.ssid = (char*)MAIN_WIFI_SSID;
//      wifiConfig.staConfig.password = (char*)MAIN_WIFI_PWD;
//      wifiConfig.staConfig.connRetryMax = WIFI_MGR_CONN_RETRY_MAX_DEFAULT;
//      wifiConfig.staConfig.authMode = WIFI_MGR_AUTH_MODE_DEFAULT;
//      wifiConfig.staConfig.netConfig.enabledStaticIP = 0;
//      wifiConfig.staConfig.netConfig.IPV4 = MAIN_WIFI_STA_NET_STATIC_IP;
//      wifiConfig.staConfig.netConfig.defaultGW = MAIN_WIFI_STA_NET_STATIC_GW;
//      wifiConfig.staConfig.netConfig.subnetMask = MAIN_WIFI_STA_NET_STATIC_NETMARK;
//#else // 0
//
//      wifiConfig.apConfig.ssid = (char*)MAIN_WIFI_AP_SSID;
//      wifiConfig.apConfig.password = (char*)MAIN_WIFI_AP_PWD;
//      wifiConfig.apConfig.channel = WIFI_MGR_CHANNEL_DEFAULT;
//      wifiConfig.apConfig.authMode = WIFI_MGR_AUTH_MODE_DEFAULT;
//      wifiConfig.apConfig.maxConn = WIFI_MGR_MAX_CONN_DEFAULT;
//      wifiConfig.apConfig.netConfig.IPV4 = MAIN_WIFI_AP_NET_STATIC_IP;
//      wifiConfig.apConfig.netConfig.defaultGW = MAIN_WIFI_AP_NET_STATIC_GW;
//      wifiConfig.apConfig.netConfig.subnetMask = MAIN_WIFI_AP_NET_STATIC_NETMARK;
//#endif // 0
//      result = wifiTask.startWifi(wifiConfig, true);
//      CONSOLE_LOG_BUF(mainBufLog, SYSTEM_CONSOLE_OUT_BUF_LEN, "[%s]: %i %i", "set", 10, result);
//      if (result != 0)
//      {
//        break;
//      }
//
//      NetIPConfigTAG ipConfig = NetIPConfigTAG();
//      result = wifiTask.getNetConfig(ipConfig);
//      CONSOLE_LOG_BUF(mainBufLog, SYSTEM_CONSOLE_OUT_BUF_LEN, "[%s]: %i %i", "set", 11, result);
//      if (result != 0)
//      {
//        break;
//      }
//      CONSOLE_LOG_BUF(mainBufLog, SYSTEM_CONSOLE_OUT_BUF_LEN, "[%s] %d.%d.%d.%d", "ip", NET_IP4_2_STR(ipConfig.IPV4));
//      CONSOLE_LOG_BUF(mainBufLog, SYSTEM_CONSOLE_OUT_BUF_LEN, "[%s] %d.%d.%d.%d", "mask", NET_IP4_2_STR(ipConfig.subnetMask));
//      CONSOLE_LOG_BUF(mainBufLog, SYSTEM_CONSOLE_OUT_BUF_LEN, "[%s] %d.%d.%d.%d", "gw", NET_IP4_2_STR(ipConfig.defaultGW));
//    }
//    //-----------------------
//    /*Web server*/
//    //FEM_HTTP_SERVER_BASE_PATH
//    //if (0)
//    {
//      CommMBCSettingParamTAG serMbcSetting = CommMBCSettingParamTAG();
//      serMbcSetting.enabled = true;
//      serMbcSetting.isHeaderLess = FEM_HTTP_SERVER_IS_HEADER_LESS;
//      serMbcSetting.processorType = CommMBCDataType::CommMbcJsonData;
//      result = httpServer.inititialize(serMbcSetting);
//      CONSOLE_LOG_BUF(mainBufLog, SYSTEM_CONSOLE_OUT_BUF_LEN, "[%s]: %i %i", "set", 12, result);
//      if (result != 0)
//      {
//        break;
//      }
//      // reg message id
//      httpServer.registerMessageId(CommMBCConstant::CommMBCMessageId::CommMBCHeadLess, 200);
//      httpServer.registerMessageId(CommMBCConstant::CommMBCMessageId::CommMBCSystemSetting);
//      // start task
//      CommMBCHttpTaskConfigTAG httpTaskConfig = CommMBCHttpTaskConfigTAG();
//      httpTaskConfig.taskManagerConfig.eventItemNumber = FEM_HTTP_SERVER_TASK_EVENT_NUM;
//      httpTaskConfig.taskManagerConfig.eventUsePool = false;
//      httpTaskConfig.taskThreadConfig.enabled = true;
//      httpTaskConfig.taskThreadConfig.useThreadPool = false;
//      httpTaskConfig.taskThreadConfig.usStackDepth = FEM_HTTP_SERVER_TASK_MEM;
//      httpTaskConfig.taskThreadConfig.uxPriority = FEM_HTTP_SERVER_TASK_PRIORITY;
//      httpTaskConfig.bufferConfig.usePool = false;
//      httpTaskConfig.bufferConfig.preparedDataNumber = FEM_HTTP_SERVER_BUFF_COUNT;
//      httpTaskConfig.bufferConfig.dataItemConfig.bufferSize = FEM_HTTP_SERVER_BUFF_SIZE;
//      result = httpServer.startTask(httpTaskConfig);
//      CONSOLE_LOG_BUF(mainBufLog, SYSTEM_CONSOLE_OUT_BUF_LEN, "[%s]: %i %i", "set", 13, result);
//      if (result != 0)
//      {
//        break;
//      }
//    }
//
//    //if (0)
//    {
//      // start http server
//      CommMBCHttpConnectionConfigTAG httpConnConfig = CommMBCHttpConnectionConfigTAG();
//      httpConnConfig.config.server.basePath = FEM_HTTP_SERVER_BASE_PATH;
//      httpConnConfig.config.server.port = FEM_HTTP_SERVER_PORT;
//      httpConnConfig.config.server.uriMaxCount = FEM_HTTP_SERVER_URI_COUNT_MAX;
//      httpConnConfig.config.server.connMaxCount = FEM_HTTP_SERVER_CONN_COUNT_MAX;
//      httpConnConfig.config.server.addHeaderMaxCount = FEM_HTTP_SERVER_ADD_HEADER_COUNT_MAX;
//      httpConnConfig.config.server.backlogConn = FEM_HTTP_SERVER_BACKLOG_CONN;
//      result = httpServer.startHttp(httpConnConfig);
//      CONSOLE_LOG_BUF(mainBufLog, SYSTEM_CONSOLE_OUT_BUF_LEN, "[%s]: %i %i", "set", 14, result);
//      if (result != 0)
//      {
//        break;
//      }
//    }
//
//
//    //if (0)
//    {
//      // start uri handlers
//
//      // uri 1
//      CommMBCHttpUriInfoTAG uriInfo = CommMBCHttpUriInfoTAG();
//      uriInfo.dataType = FEM_HTTP_SERVER_URI_1_DATATYPE;
//      uriInfo.id = FEM_HTTP_SERVER_URI_1_ID;
//      uriInfo.method = FEM_HTTP_SERVER_URI_1_METHOD;
//      uriInfo.reqCallback = (void*)cbCommHttpUriHandler;
//      uriInfo.userArg = &httpServer;
//      uriInfo.uri = FEM_HTTP_SERVER_URI_1_URI;
//      result = httpServer.regUriHandler(uriInfo);
//      CONSOLE_LOG_BUF(mainBufLog, SYSTEM_CONSOLE_OUT_BUF_LEN, "[%s]: %i %i", "set", 15, result);
//      if (result != 0)
//      {
//        break;
//      }
//
//      // uri 2
//      uriInfo.dataType = FEM_HTTP_SERVER_URI_2_DATATYPE;
//      uriInfo.id = FEM_HTTP_SERVER_URI_2_ID;
//      uriInfo.method = FEM_HTTP_SERVER_URI_2_METHOD;
//      uriInfo.reqCallback = (void*)cbCommHttpUriHandler;
//      uriInfo.userArg = &httpServer;
//      uriInfo.uri = FEM_HTTP_SERVER_URI_2_URI;
//      result = httpServer.regUriHandler(uriInfo);
//      CONSOLE_LOG_BUF(mainBufLog, SYSTEM_CONSOLE_OUT_BUF_LEN, "[%s]: %i %i", "set", 16, result);
//      if (result != 0)
//      {
//        break;
//      }
//
//      result = httpServer.regDefaultUriHandler();
//      CONSOLE_LOG_BUF(mainBufLog, SYSTEM_CONSOLE_OUT_BUF_LEN, "[%s]: %i %i", "set", 30, result);
//      if (result != 0)
//      {
//        break;
//      }
//    }
//
//    // start client
//    // start task
//    {
//      CommMBCSettingParamTAG cliMbcSetting = CommMBCSettingParamTAG();
//      cliMbcSetting.enabled = true;
//      cliMbcSetting.isHeaderLess = FEM_HTTP_CLIENT_IS_HEADER_LESS;
//      cliMbcSetting.processorType = CommMBCDataType::CommMbcJsonData;
//      result = httpClient.inititialize(cliMbcSetting);
//      CONSOLE_LOG_BUF(mainBufLog, SYSTEM_CONSOLE_OUT_BUF_LEN, "[%s]: %i %i", "set", 50, result);
//      if (result != 0)
//      {
//        break;
//      }
//      // reg message id
//      httpClient.registerMessageId(CommMBCConstant::CommMBCMessageId::CommMBCHeadLess, 200);
//      httpClient.registerMessageId(CommMBCConstant::CommMBCMessageId::CommMBCSystemSetting);
//      // start task
//      CommMBCHttpTaskConfigTAG httpTaskConfig = CommMBCHttpTaskConfigTAG();
//      httpTaskConfig.taskManagerConfig.eventItemNumber = FEM_HTTP_CLIENT_TASK_EVENT_NUM;
//      httpTaskConfig.taskManagerConfig.eventUsePool = false;
//      httpTaskConfig.taskThreadConfig.enabled = true;
//      httpTaskConfig.taskThreadConfig.useThreadPool = false;
//      httpTaskConfig.taskThreadConfig.usStackDepth = FEM_HTTP_CLIENT_TASK_MEM;
//      httpTaskConfig.taskThreadConfig.uxPriority = FEM_HTTP_CLIENT_TASK_PRIORITY;
//      httpTaskConfig.bufferConfig.usePool = false;
//      httpTaskConfig.bufferConfig.preparedDataNumber = FEM_HTTP_CLIENT_BUFF_COUNT;
//      httpTaskConfig.bufferConfig.dataItemConfig.bufferSize = FEM_HTTP_CLIENT_BUFF_SIZE;
//      result = httpClient.startTask(httpTaskConfig);
//      CONSOLE_LOG_BUF(mainBufLog, SYSTEM_CONSOLE_OUT_BUF_LEN, "[%s]: %i %i", "set", 51, result);
//      if (result != 0)
//      {
//        break;
//      }
//    }
//    // start conn
//    {
//      CommMBCHttpConnectionConfigTAG httpConnConfig = CommMBCHttpConnectionConfigTAG();
//      httpConnConfig.config.client.url = FEM_HTTP_CLIENT_PATH;
//      httpConnConfig.config.client.certPem = FEM_HTTP_CLIENT_REQ_CERT;
//      result = httpClient.startHttp(httpConnConfig);
//      CONSOLE_LOG_BUF(mainBufLog, SYSTEM_CONSOLE_OUT_BUF_LEN, "[%s]: %i %i", "set", 52, result);
//      if (result != 0)
//      {
//        break;
//      }
//    }
//
//    /*{
//      SystemMutex clientReqMutex;
//      clientReqMutex.lock();
//      HttpReqId_t reqId = 0;
//      BufferDataItem* reqDataItem = httpClient.getCommData();
//      if (reqDataItem == NULL)
//      {
//        break;
//      }
//
//      const char* json = "{ \"name\": \"abc\", \"age\": 50}";
//      char* reqBody = (char*)reqDataItem->bufferAddress();
//      DataSize_t reqBodyLen = strlen(json);
//      reqDataItem->setDataLen(reqBodyLen);
//      memcpy(reqBody, json, reqBodyLen);
//      CommHttpClientRequestParamsTAG requestParam = CommHttpClientRequestParamsTAG();
//      requestParam.header.dataType = COMM_HTTP_DATA_TYPE_APP_JS;
//      requestParam.header.fileData = 0;
//      requestParam.header.reqMethod = COMM_HTTP_METHOD_POST;
//      requestParam.header.uri = FEM_HTTP_CLIENT_REQ_URI;
//      requestParam.header.uriId = 1;
//
//      requestParam.body = reqBody;
//      requestParam.bodyLen = reqDataItem->dataLength();
//
//      requestParam.notifier.agr = &clientReqMutex;
//      requestParam.notifier.callback = cbHttpClientResponse;
//      requestParam.notifier.expiredTime = TimerManager::getInstance().now() + (COMM_HTTP_REQUEST_HANDLE_WAIT_TIME + COMM_HTTP_REQUEST_HANDLE_WAIT_TIME_ADD)* TIMER_DEVICE_TIMER_MS_2_UNIT;
//      result = httpClient.request(requestParam, reqId);
//      CONSOLE_LOG_BUF(mainBufLog, SYSTEM_CONSOLE_OUT_BUF_LEN, "[%s]: %i %d", "set", 95, reqId);
//      if (result != 0)
//      {
//        httpClient.releaseCommData(reqDataItem);
//        break;
//      }
//
//      clientReqMutex.lock(10000);
//    }*/
//
//    //{
//    //  SystemMutex clientReqMutex;
//    //  clientReqMutex.lock();
//    //  HttpReqId_t reqId = 0;
//
//    //  CommMBCSystemSettingTAG sysSettingTag = CommMBCSystemSettingTAG();
//
//
//    //  CommMBCHttpClientRequestParamsTAG requestParam = CommMBCHttpClientRequestParamsTAG();
//    //  requestParam.reqMessageId = CommMBCConstant::CommMBCMessageId::CommMBCHeadLess;
//    //  requestParam.header.dataType = COMM_HTTP_DATA_TYPE_APP_JSON;
//    //  requestParam.header.fileData = 0;
//    //  requestParam.header.reqMethod = COMM_HTTP_METHOD_POST;
//    //  requestParam.header.uri = FEM_HTTP_CLIENT_REQ_URI;
//    //  requestParam.header.uriId = 2;
//
//    //  requestParam.body = ;// FEM_HTTP_CLIENT_REQ_BODY;
//    //  requestParam.bodyLen = strlen(FEM_HTTP_CLIENT_REQ_BODY);
//
//    //  requestParam.notifier.agr = &clientReqMutex;
//    //  requestParam.notifier.callback = cbHttpClientResponse;
//    //  requestParam.notifier.expiredTime = TimerManager::getInstance().now() + (COMM_HTTP_REQUEST_HANDLE_WAIT_TIME + COMM_HTTP_REQUEST_HANDLE_WAIT_TIME_ADD) * TIMER_DEVICE_TIMER_MS_2_UNIT;
//    //  result = httpClient.request(requestParam, reqId);
//    //  CONSOLE_LOG_BUF(mainBufLog, SYSTEM_CONSOLE_OUT_BUF_LEN, "[%s]: %i %d %d", "set", 97, reqId, result);
//    //  if (result != 0)
//    //  {
//    //    break;
//    //  }
//
//    //  clientReqMutex.lock(10000);
//    //}
//
//
//    {
//      // test
//
//      CommMBCSystemSettingTAG inputTag = CommMBCSystemSettingTAG();
//      inputTag.sectionId = 1;
//      inputTag.settingId = 2;
//      inputTag.bitSet1.dataType = 5;
//      inputTag.bitSet1.isReply = 0;
//      inputTag.bitSet1.isUpdate = 0;
//      inputTag.bitSet1.reserved = 0;
//
//      inputTag.errorCode = 4;
//      inputTag.sLen = 5;
//      char* jsonBuf = new char[200];
//      JsonSnprintf(
//        jsonBuf
//        , 200
//        , "\"b\":{\"secId\":%u,\"setId\":%d,\"bSet1\":%d,\"eCode\":%d,\"sLen\":%d,"
//        , inputTag.sectionId
//        , inputTag.settingId
//        , inputTag.bitSet1
//        , inputTag.errorCode
//        , inputTag.sLen);
//
//      CONSOLE_LOG_BUF(mainBufLog, SYSTEM_CONSOLE_OUT_BUF_LEN, "[98]: %s", jsonBuf);
//      delete[] jsonBuf;
//    }
//    CONSOLE_LOG_BUF(mainBufLog, SYSTEM_CONSOLE_OUT_BUF_LEN, "[%s]: %i", "set", 99);
//    return 0;
//  } while (0);
//
//  CONSOLE_LOG_BUF(mainBufLog, SYSTEM_CONSOLE_OUT_BUF_LEN, "[%s]: %i", "set", -99);
//  return -1;
//}
//




#endif // _CONF_MAIN_CONTROLLER_ENABLED