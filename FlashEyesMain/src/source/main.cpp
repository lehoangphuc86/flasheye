#ifndef _FEM_MAIN_CPP
#define _FEM_MAIN_CPP
/////////////////////////////////////////////////
// INCLUDE
#include "main.h"
#include "file_system/FileSystemManager.h"
#include "db/DBManager.h"
#include "task_http/CommHttpServer.h"
#include "task_http/CommHttpClient.h"
#include "task_net/wifi/WifiControllerTask.h"
#include "timer_manager/TimerManager.h"

/////////////////////////////////////////////////
// PREPROCESSOR
#define MAIN_CONSOLE_DEBUG_ENABLE

#define MAIN_WIFI_SSID                              "SamsungJ7"
#define MAIN_WIFI_PWD                               "donotconnect"

#define MAIN_WIFI_AP_SSID                           "SamsungJ7_2"
#define MAIN_WIFI_AP_PWD                            "donotconnect"

#define MAIN_WIFI_STA_NET_STATIC_IP                 NET_IP4_INIT(192, 168, 137, 100)
#define MAIN_WIFI_STA_NET_STATIC_NETMARK            NET_IP4_INIT(255, 255, 255, 0)
#define MAIN_WIFI_STA_NET_STATIC_GW                 NET_IP4_INIT(192, 168, 137, 1)

#define MAIN_WIFI_AP_NET_STATIC_IP                  NET_IP4_INIT(192, 169, 0, 100)
#define MAIN_WIFI_AP_NET_STATIC_NETMARK             NET_IP4_INIT(255, 255, 255, 0)
#define MAIN_WIFI_AP_NET_STATIC_GW                  NET_IP4_INIT(192, 169, 0, 1)


/////////////////////////////////////////////////
// DEFINE

/////////////////////////////////////////////////
// MARCO

/////////////////////////////////////////////////
// GLOBAL VARIABLES
#ifdef MAIN_CONSOLE_DEBUG_ENABLE
char mainBufLog[SYSTEM_CONSOLE_OUT_BUF_LEN];
#endif // MAIN_CONSOLE_DEBUG_ENABLE
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

/////////////////////////////////////////////////
// CLASS IMPLEMENTATION

// db
const char* data = "Callback function called";
static int callback(void* data, int argc, char** argv, char** azColName) {
  int i;
  Serial.printf("%s: ", (const char*)data);
  for (i = 0; i < argc; i++) {
    Serial.printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
  }
  Serial.printf("\n");
  return 0;
}

// http server 
int cbCommHttpUriHandler(void* arg, CommHttpUriRequestTAG* uriReqInfo)
{
  int ret = 0;
  CommHttpPackageTAG* reqPackage = &uriReqInfo->reqData;
  CommHttpPackageTAG* resPackage = &uriReqInfo->resData;
  CommHttpServer* serverHandler = (CommHttpServer*)(arg);
  int error = 0;
  do
  {
    CommHttpHeaderTAG* reqHeader = reqPackage->header();
    CONSOLE_LOG_BUF(mainBufLog, SYSTEM_CONSOLE_OUT_BUF_LEN, "[m] %s: %i id=%i", "cb1", 0, reqHeader->uriId);
    CONSOLE_LOG_BUF(mainBufLog, SYSTEM_CONSOLE_OUT_BUF_LEN, "%s", reqHeader->uri);
    if (resPackage->setData(serverHandler->dataManager()) != 0)
    {
      break;
    }
    CommHttpHeaderTAG* resHeader = resPackage->header();

    switch (reqHeader->uriId)
    {
      case FEM_HTTP_SERVER_URI_1_ID:
      {
        static const char* resMess = "{\"name\":\"John\", \"age\":30, \"car\":null}";
        resHeader->fileData = 0;
        resHeader->resStatusCode = COMM_HTTP_STATUS_CODE_200;
        resHeader->dataType = COMM_HTTP_DATA_TYPE_APP_JSON;
        //resHeader->reqMethod = COMM_HTTP_METHOD_GET;
        resPackage->bodyLen(SYSTEM_MIN(strlen(resMess), resPackage->bodyMaxLen()));
        memcpy(resPackage->body(), resMess, resPackage->bodyLen());
        break;
      }
      case FEM_HTTP_SERVER_URI_2_ID:
      {
        char* reqBody = reqPackage->body();
        char* resBody = resPackage->body();
        DataSize_t reqBodySize = reqPackage->bodyLen();
        CommHttpHeaderTAG* reqHeader = reqPackage->header();
        if ( (reqHeader->dataType != COMM_HTTP_DATA_TYPE_APP_JS)
          && (reqBodySize <= 0)
          )
        {
          error = -1;
          break;
        }

        // json parser
        //@@
        reqBody[reqBodySize] = '\0';
        CONSOLE_LOG_BUF(mainBufLog, SYSTEM_CONSOLE_OUT_BUF_LEN, "[m] %s: %i", "cb1", 4);
        CONSOLE_LOG_BUF(mainBufLog, SYSTEM_CONSOLE_OUT_BUF_LEN, "%s", reqBody);

        resHeader->fileData = 0;
        resHeader->resStatusCode = COMM_HTTP_STATUS_CODE_200;
        resHeader->dataType = reqHeader->dataType;
        //resHeader->reqMethod = reqHeader->reqMethod;

        // respond
        memcpy(resBody, reqBody, reqBodySize);
        resPackage->bodyLen(reqBodySize);
        break;
      }
      default:
        error = -1;
        break;
    }

    if (error != 0)
    {
      break;
    }

    reqPackage->releaseData();
    return 0;
  } while (0);
  reqPackage->releaseData();
  resPackage->releaseData();
  return -1;
}


int cbHttpClientResponse(void* arg, CommHttpUriRequestTAG* uriRequest)
{
  CommHttpPackageTAG* resPackage = &uriRequest->resData;
  do
  {
    if (arg == NULL)
    {
      break;
    }

    CONSOLE_LOG_BUF(mainBufLog, SYSTEM_CONSOLE_OUT_BUF_LEN, "[m] %s %i rId=%d ","cbCR", 1, uriRequest->requestId);
    CommHttpHeaderTAG* resHeader = resPackage->header();
    char* resBody = resPackage->body();
    CONSOLE_LOG_BUF(mainBufLog, SYSTEM_CONSOLE_OUT_BUF_LEN, "[m] %s", resHeader->uri);
    CONSOLE_LOG_BUF(mainBufLog, SYSTEM_CONSOLE_OUT_BUF_LEN, "[m] %s %i sc=%d dt=%d", "cbCR", 1, resHeader->resStatusCode, resHeader->dataType);
    CONSOLE_LOG_BUF(mainBufLog, SYSTEM_CONSOLE_OUT_BUF_LEN, "[m] %s %i bL=%d", "cbCR", 1, resPackage->bodyLen());
    resBody[resPackage->bodyLen()] = '\0';
    CONSOLE_LOG_BUF(mainBufLog, SYSTEM_CONSOLE_OUT_BUF_LEN, "[m] %s", resBody);
    resPackage->releaseData();

    ((SystemMutex*)arg)->unlock();
    return 0;
  } while (0);

  resPackage->releaseData();
  return -1;
}

static WifiControllerTask wifiTask;
static CommHttpServer httpServer;
static CommHttpClient httpClient;
/*main*/
int main(void)
{
  
  int result = 0;
  do
  {
    // initialize serial communication at 9600 bits per second:
    SYSTEM_CONSOLE_HANDLER.begin(56000);
    while (!SYSTEM_CONSOLE_HANDLER)
    {
      ; // wait for serial port to connect. Needed for native USB, on LEONARDO, MICRO, YUN, and other 32u4 based boards.
    }

    SYSTEM_SLEEP(1000);
    CONSOLE_LOG_BUF(mainBufLog, SYSTEM_CONSOLE_OUT_BUF_LEN, "[%s]: %i", "set", 0);
    // Set up tasks
    // start timer 
        
    //if (0)
    {
      TaskManagerConfigTAG taskConfig = TaskManagerConfigTAG();
      taskConfig.eventUsePool = FEM_TM_USE_POOL;
      taskConfig.eventItemNumber = FEM_TM_TASK_EVENT_NO; // TIMER_MANAGER_TEST_TASK_EVENT_NO;
      TaskThreadConfigTAG threadConfig = TaskThreadConfigTAG();
      threadConfig.enabled = true;
      threadConfig.useThreadPool = false;
      threadConfig.usStackDepth = FEM_TM_TASK_MEM;
      threadConfig.uxPriority = FEM_TM_TASK_PRIORITY;
      {
        TimerControlTaskConfigTAG timerConfig = TimerControlTaskConfigTAG();
        timerConfig.taskManagerConfig = &taskConfig;
        timerConfig.taskThreadConfig = &threadConfig;
        timerConfig.hwTimerGrpId = FEM_TM_HW_GROUP_ID;
        timerConfig.hwTimerIndex = FEM_TM_HW_INDEX_ID;
        timerConfig.hwTimerDevider = FEM_TM_HW_DEVIDER;
        timerConfig.hwClockMHz = F_CPU / 1000000;

        if (TimerManager::getInstance().isValid() != false)
        {
          CONSOLE_LOG_BUF(mainBufLog, SYSTEM_CONSOLE_OUT_BUF_LEN, "[%s]: %i", "set", -101);
          return -1; // already config
        }

        result = TimerManager::getInstance().initialize(timerConfig);
        if (result != 0)
        {
          CONSOLE_LOG_BUF(mainBufLog, SYSTEM_CONSOLE_OUT_BUF_LEN, "[%s]: %i", "set", -102);
          break;
        }
      }
    }


    // file system
    {
      FileSystemConfigTAG fsConfig = FileSystemConfigTAG();
      //memcpy(fsConfig.basePath, FEM_FS_BASE_PATH, SYSTEM_MIN(strlen(FEM_FS_BASE_PATH), FS_MGR_BASE_PATH_LEN_MAX));
      fsConfig.basePath = FEM_FS_BASE_PATH;
      fsConfig.partitionLabel = FEM_FS_PARTITION_LABEL;
      fsConfig.formatIfFailed = FEM_FS_FORMAT;
      fsConfig.maxOpenFile = FEM_FS_MAX_FILE;
      CONSOLE_LOG_BUF(mainBufLog, SYSTEM_CONSOLE_OUT_BUF_LEN, "[%s]: %i %s", "set", 1, fsConfig.basePath);
    
      result = FileSystemManager::getInstance().initialize(fsConfig);
      if (result != 0)
      {
        CONSOLE_LOG_BUF(mainBufLog, SYSTEM_CONSOLE_OUT_BUF_LEN, "[%s]: %i", "set", -1);
        break;
      }
    }

    /*CONSOLE_LOG_BUF(mainBufLog, SYSTEM_CONSOLE_OUT_BUF_LEN, "[%s]: %i", "set", 2);
    result = FileSystemManager::getInstance().listDir("/", 2);
    if (result != 0)
    {
      CONSOLE_LOG_BUF(mainBufLog, SYSTEM_CONSOLE_OUT_BUF_LEN, "[%s]: %i", "set", -2);
      break;
    }
    */
    CONSOLE_LOG_BUF(mainBufLog, SYSTEM_CONSOLE_OUT_BUF_LEN, "[%s]: %i", "set", 3);

    {
      DBManagerConfigTAG dbManagerConfig = DBManagerConfigTAG();
      dbManagerConfig.dbPath = FEM_DB_PATH;
      result = DBManager::getInstance().initialize(dbManagerConfig);
      if (result != 0)
      {
        CONSOLE_LOG_BUF(mainBufLog, SYSTEM_CONSOLE_OUT_BUF_LEN, "[%s]: %i", "set", -3);
        break;
      }
    }
    CONSOLE_LOG_BUF(mainBufLog, SYSTEM_CONSOLE_OUT_BUF_LEN, "[%s]: %i", "set", 4);
    DBHandler_t dbHandler = DBManager::getInstance().dbHandler();
    //result = DBManager::getInstance().exec(dbHandler, "CREATE TABLE TBL_CONFIG (id INTEGER PRIMARY KEY, Name TEXT NOT NULL, Type INTEGER NOT NULL, Value TEXT NOT NULL)", callback, (void*)data, NULL);
    if (result != 0)
    {
      CONSOLE_LOG_BUF(mainBufLog, SYSTEM_CONSOLE_OUT_BUF_LEN, "[%s]: %i", "set", -4);
      break;
    }

    CONSOLE_LOG_BUF(mainBufLog, SYSTEM_CONSOLE_OUT_BUF_LEN, "[%s]: %i", "set", 5);

    //result = DBManager::getInstance().exec(dbHandler, "INSERT INTO TBL_CONFIG (Name, Type, Value) VALUES('StartMode', 1, '2')", callback, (void*)data, NULL);
    //result += DBManager::getInstance().exec(dbHandler, "INSERT INTO TBL_CONFIG (Name, Type, Value) VALUES('ScanMode', 2, '3')", callback, (void*)data, NULL);
    if (result != 0)
    {
      CONSOLE_LOG_BUF(mainBufLog, SYSTEM_CONSOLE_OUT_BUF_LEN, "[%s]: %i", "set", -5);
      break;
    }
    CONSOLE_LOG_BUF(mainBufLog, SYSTEM_CONSOLE_OUT_BUF_LEN, "[%s]: %i", "set", 6);
    /*result = DBManager::getInstance().exec(dbHandler, "SELECT * FROM TBL_CONFIG", callback, (void*)data, NULL);
    if (result != 0)
    {
      CONSOLE_LOG_BUF(mainBufLog, SYSTEM_CONSOLE_OUT_BUF_LEN, "[%s]: %i", "set", -6);
      break;
    }*/
    //char valCh[32] = {};
    ////char* val = valCh;
    //long val = 0;
    //result = DBManager::getInstance().selectCell(DB_TBL_CONFIG_ID, "Type", "id", 2, &val);
    //if (result != 0)
    //{
    //  CONSOLE_LOG_BUF(mainBufLog, SYSTEM_CONSOLE_OUT_BUF_LEN, "[%s]: %i", "set", -6);
    //  break;
    //}
    //CONSOLE_LOG_BUF(mainBufLog, SYSTEM_CONSOLE_OUT_BUF_LEN, "[%s]: %i %i", "set", 7, val);



    //------------------------
    //if (0)
    {
      result = wifiTask.inititialize();
      CONSOLE_LOG_BUF(mainBufLog, SYSTEM_CONSOLE_OUT_BUF_LEN, "[%s]: %i %i", "set", 8, result);
      if (result != 0)
      {
        break;
      }
    }

    //if (0)
    {
      WifiTaskConfigTAG wifiTaskConfig = WifiTaskConfigTAG();
      
      wifiTaskConfig.taskManagerConfig.eventItemNumber = FEM_WIFI_SERVER_TASK_EVENT_NUM;
      wifiTaskConfig.taskManagerConfig.eventUsePool = false;

      
      wifiTaskConfig.taskThreadConfig.enabled = true;
      wifiTaskConfig.taskThreadConfig.useThreadPool = false;
      wifiTaskConfig.taskThreadConfig.usStackDepth = FEM_WIFI_SERVER_TASK_MEM;
      wifiTaskConfig.taskThreadConfig.uxPriority = FEM_WIFI_SERVER_TASK_PRIORITY;

      result = wifiTask.startTask(wifiTaskConfig);
      CONSOLE_LOG_BUF(mainBufLog, SYSTEM_CONSOLE_OUT_BUF_LEN, "[%s]: %i %i", "set", 9, result);
      if (result != 0)
      {
        break;
      }
    }

    //if (0)
    {
      WifiConnectionConfigTAG wifiConfig = WifiConnectionConfigTAG();
#if (1)
      wifiConfig.staConfig.ssid = (char*)MAIN_WIFI_SSID;
      wifiConfig.staConfig.password = (char*)MAIN_WIFI_PWD;
      wifiConfig.staConfig.connRetryMax = WIFI_MGR_CONN_RETRY_MAX_DEFAULT;
      wifiConfig.staConfig.authMode = WIFI_MGR_AUTH_MODE_DEFAULT;
      wifiConfig.staConfig.netConfig.enabledStaticIP = 0;
      wifiConfig.staConfig.netConfig.IPV4 = MAIN_WIFI_STA_NET_STATIC_IP;
      wifiConfig.staConfig.netConfig.defaultGW = MAIN_WIFI_STA_NET_STATIC_GW;
      wifiConfig.staConfig.netConfig.subnetMask = MAIN_WIFI_STA_NET_STATIC_NETMARK;
#else // 0
    
      wifiConfig.apConfig.ssid = (char*)MAIN_WIFI_AP_SSID;
      wifiConfig.apConfig.password = (char*)MAIN_WIFI_AP_PWD;
      wifiConfig.apConfig.channel = WIFI_MGR_CHANNEL_DEFAULT;
      wifiConfig.apConfig.authMode = WIFI_MGR_AUTH_MODE_DEFAULT;
      wifiConfig.apConfig.maxConn = WIFI_MGR_MAX_CONN_DEFAULT;
      wifiConfig.apConfig.netConfig.IPV4 = MAIN_WIFI_AP_NET_STATIC_IP;
      wifiConfig.apConfig.netConfig.defaultGW = MAIN_WIFI_AP_NET_STATIC_GW;
      wifiConfig.apConfig.netConfig.subnetMask = MAIN_WIFI_AP_NET_STATIC_NETMARK;
#endif // 0
      result = wifiTask.startWifi(wifiConfig, true);
      CONSOLE_LOG_BUF(mainBufLog, SYSTEM_CONSOLE_OUT_BUF_LEN, "[%s]: %i %i", "set", 10, result);
      if (result != 0)
      {
        break;
      }

      NetIPConfigTAG ipConfig = NetIPConfigTAG();
      result = wifiTask.getNetConfig(ipConfig);
      CONSOLE_LOG_BUF(mainBufLog, SYSTEM_CONSOLE_OUT_BUF_LEN, "[%s]: %i %i", "set", 11, result);
      if (result != 0)
      {
        break;
      }
      CONSOLE_LOG_BUF(mainBufLog, SYSTEM_CONSOLE_OUT_BUF_LEN, "[%s] %d.%d.%d.%d", "ip", NET_IP4_2_STR(ipConfig.IPV4));
      CONSOLE_LOG_BUF(mainBufLog, SYSTEM_CONSOLE_OUT_BUF_LEN, "[%s] %d.%d.%d.%d", "mask", NET_IP4_2_STR(ipConfig.subnetMask));
      CONSOLE_LOG_BUF(mainBufLog, SYSTEM_CONSOLE_OUT_BUF_LEN, "[%s] %d.%d.%d.%d", "gw", NET_IP4_2_STR(ipConfig.defaultGW));
    }
    //-----------------------
    /*Web server*/
    //FEM_HTTP_SERVER_BASE_PATH
    //if (0)
    {
      result = httpServer.inititialize();
      CONSOLE_LOG_BUF(mainBufLog, SYSTEM_CONSOLE_OUT_BUF_LEN, "[%s]: %i %i", "set", 12, result);
      if (result != 0)
      {
        break;
      }
      // start task
      CommHtttpTaskConfigTAG httpTaskConfig = CommHtttpTaskConfigTAG();
      httpTaskConfig.taskManagerConfig.eventItemNumber = FEM_HTTP_SERVER_TASK_EVENT_NUM;
      httpTaskConfig.taskManagerConfig.eventUsePool = false;
      httpTaskConfig.taskThreadConfig.enabled = true;
      httpTaskConfig.taskThreadConfig.useThreadPool = false;
      httpTaskConfig.taskThreadConfig.usStackDepth = FEM_HTTP_SERVER_TASK_MEM;
      httpTaskConfig.taskThreadConfig.uxPriority = FEM_HTTP_SERVER_TASK_PRIORITY;
      httpTaskConfig.bufferConfig.usePool = false;
      httpTaskConfig.bufferConfig.preparedDataNumber = FEM_HTTP_SERVER_BUFF_COUNT;
      httpTaskConfig.bufferConfig.dataItemConfig.bufferSize = FEM_HTTP_SERVER_BUFF_SIZE;
      result = httpServer.startTask(httpTaskConfig);
      CONSOLE_LOG_BUF(mainBufLog, SYSTEM_CONSOLE_OUT_BUF_LEN, "[%s]: %i %i", "set", 13, result);
      if (result != 0)
      {
        break;
      }
    }

    //if (0)
    {
      // start http server
      CommHttpConnectionConfigTAG httpConnConfig = CommHttpConnectionConfigTAG();
      httpConnConfig.config.server.basePath = FEM_HTTP_SERVER_BASE_PATH;
      httpConnConfig.config.server.port = FEM_HTTP_SERVER_PORT;
      httpConnConfig.config.server.uriMaxCount = FEM_HTTP_SERVER_URI_COUNT_MAX;
      httpConnConfig.config.server.connMaxCount = FEM_HTTP_SERVER_CONN_COUNT_MAX;
      httpConnConfig.config.server.addHeaderMaxCount = FEM_HTTP_SERVER_ADD_HEADER_COUNT_MAX;
      httpConnConfig.config.server.backlogConn = FEM_HTTP_SERVER_BACKLOG_CONN;
      result = httpServer.startHttp(httpConnConfig);
      CONSOLE_LOG_BUF(mainBufLog, SYSTEM_CONSOLE_OUT_BUF_LEN, "[%s]: %i %i", "set", 14, result);
      if (result != 0)
      {
        break;
      }
    }


    //if (0)
    {
      // start uri handlers

      // uri 1
      CommHttpUriInfoTAG uriInfo = CommHttpUriInfoTAG();
      uriInfo.dataType = FEM_HTTP_SERVER_URI_1_DATATYPE;
      uriInfo.id = FEM_HTTP_SERVER_URI_1_ID;
      uriInfo.method = FEM_HTTP_SERVER_URI_1_METHOD;
      uriInfo.reqCallback = cbCommHttpUriHandler;
      uriInfo.userArg = &httpServer;
      uriInfo.uri = FEM_HTTP_SERVER_URI_1_URI;
      result = httpServer.regUriHandler(uriInfo);
      CONSOLE_LOG_BUF(mainBufLog, SYSTEM_CONSOLE_OUT_BUF_LEN, "[%s]: %i %i", "set", 15, result);
      if (result != 0)
      {
        break;
      }

      // uri 2
      uriInfo.dataType = FEM_HTTP_SERVER_URI_2_DATATYPE;
      uriInfo.id = FEM_HTTP_SERVER_URI_2_ID;
      uriInfo.method = FEM_HTTP_SERVER_URI_2_METHOD;
      uriInfo.reqCallback = cbCommHttpUriHandler;
      uriInfo.userArg = &httpServer;
      uriInfo.uri = FEM_HTTP_SERVER_URI_2_URI;
      result = httpServer.regUriHandler(uriInfo);
      CONSOLE_LOG_BUF(mainBufLog, SYSTEM_CONSOLE_OUT_BUF_LEN, "[%s]: %i %i", "set", 16, result);
      if (result != 0)
      {
        break;
      }

      result = httpServer.regDefaultUriHandler();
      CONSOLE_LOG_BUF(mainBufLog, SYSTEM_CONSOLE_OUT_BUF_LEN, "[%s]: %i %i", "set", 30, result);
      if (result != 0)
      {
        break;
      }
    }

    // start client
    // start task
    {
      result = httpClient.inititialize();
      CONSOLE_LOG_BUF(mainBufLog, SYSTEM_CONSOLE_OUT_BUF_LEN, "[%s]: %i %i", "set", 50, result);
      if (result != 0)
      {
        break;
      }
      // start task
      CommHtttpTaskConfigTAG httpTaskConfig = CommHtttpTaskConfigTAG();
      httpTaskConfig.taskManagerConfig.eventItemNumber = FEM_HTTP_CLIENT_TASK_EVENT_NUM;
      httpTaskConfig.taskManagerConfig.eventUsePool = false;
      httpTaskConfig.taskThreadConfig.enabled = true;
      httpTaskConfig.taskThreadConfig.useThreadPool = false;
      httpTaskConfig.taskThreadConfig.usStackDepth = FEM_HTTP_CLIENT_TASK_MEM;
      httpTaskConfig.taskThreadConfig.uxPriority = FEM_HTTP_CLIENT_TASK_PRIORITY;
      httpTaskConfig.bufferConfig.usePool = false;
      httpTaskConfig.bufferConfig.preparedDataNumber = FEM_HTTP_CLIENT_BUFF_COUNT;
      httpTaskConfig.bufferConfig.dataItemConfig.bufferSize = FEM_HTTP_CLIENT_BUFF_SIZE;
      result = httpClient.startTask(httpTaskConfig);
      CONSOLE_LOG_BUF(mainBufLog, SYSTEM_CONSOLE_OUT_BUF_LEN, "[%s]: %i %i", "set", 51, result);
      if (result != 0)
      {
        break;
      }
    }
    // start conn
    {
      CommHttpConnectionConfigTAG httpConnConfig = CommHttpConnectionConfigTAG();
      httpConnConfig.config.client.url = FEM_HTTP_CLIENT_PATH;
      httpConnConfig.config.client.certPem = FEM_HTTP_CLIENT_REQ_CERT;
      result = httpClient.startHttp(httpConnConfig);
      CONSOLE_LOG_BUF(mainBufLog, SYSTEM_CONSOLE_OUT_BUF_LEN, "[%s]: %i %i", "set", 52, result);
      if (result != 0)
      {
        break;
      }
    }

    /*{
      SystemMutex clientReqMutex;
      clientReqMutex.lock();
      HttpReqId_t reqId = 0;
      BufferDataItem* reqDataItem = httpClient.getCommData();
      if (reqDataItem == NULL)
      {
        break;
      }

      const char* json = "{ \"name\": \"abc\", \"age\": 50}";
      char* reqBody = (char*)reqDataItem->bufferAddress();
      DataSize_t reqBodyLen = strlen(json);
      reqDataItem->setDataLen(reqBodyLen);
      memcpy(reqBody, json, reqBodyLen);
      CommHttpClientRequestParamsTAG requestParam = CommHttpClientRequestParamsTAG();
      requestParam.header.dataType = COMM_HTTP_DATA_TYPE_APP_JS;
      requestParam.header.fileData = 0;
      requestParam.header.reqMethod = COMM_HTTP_METHOD_POST;
      requestParam.header.uri = FEM_HTTP_CLIENT_REQ_URI;
      requestParam.header.uriId = 1;

      requestParam.body = reqBody;
      requestParam.bodyLen = reqDataItem->dataLength();
      
      requestParam.notifier.agr = &clientReqMutex;
      requestParam.notifier.callback = cbHttpClientResponse;
      requestParam.notifier.expiredTime = TimerManager::getInstance().now() + (COMM_HTTP_REQUEST_HANDLE_WAIT_TIME + COMM_HTTP_REQUEST_HANDLE_WAIT_TIME_ADD)* TIMER_DEVICE_TIMER_MS_2_UNIT;
      result = httpClient.request(requestParam, reqId);
      CONSOLE_LOG_BUF(mainBufLog, SYSTEM_CONSOLE_OUT_BUF_LEN, "[%s]: %i %d", "set", 95, reqId);
      if (result != 0)
      {
        httpClient.releaseCommData(reqDataItem);
        break;
      }
      
      clientReqMutex.lock(10000);
    }*/

    {
      SystemMutex clientReqMutex;
      clientReqMutex.lock();
      HttpReqId_t reqId = 0;
      BufferDataItem* reqDataItem = httpClient.getCommData();
      if (reqDataItem == NULL)
      {
        break;
      }

      reqDataItem->setDataLen(0);
      CommHttpClientRequestParamsTAG requestParam = CommHttpClientRequestParamsTAG();
      requestParam.header.dataType = COMM_HTTP_DATA_TYPE_APP_JS;
      requestParam.header.fileData = 0;
      requestParam.header.reqMethod = COMM_HTTP_METHOD_GET;
      requestParam.header.uri = FEM_HTTP_CLIENT_REQ_GET_URI;
      requestParam.header.uriId = 2;

      requestParam.body = NULL;
      requestParam.bodyLen = 0;

      requestParam.notifier.agr = &clientReqMutex;
      requestParam.notifier.callback = cbHttpClientResponse;
      requestParam.notifier.expiredTime = TimerManager::getInstance().now() + (COMM_HTTP_REQUEST_HANDLE_WAIT_TIME + COMM_HTTP_REQUEST_HANDLE_WAIT_TIME_ADD) * TIMER_DEVICE_TIMER_MS_2_UNIT;
      result = httpClient.request(requestParam, reqId);
      CONSOLE_LOG_BUF(mainBufLog, SYSTEM_CONSOLE_OUT_BUF_LEN, "[%s]: %i %d", "set", 97, reqId);
      if (result != 0)
      {
        httpClient.releaseCommData(reqDataItem);
        break;
      }

      clientReqMutex.lock(10000);
    }
    
    CONSOLE_LOG_BUF(mainBufLog, SYSTEM_CONSOLE_OUT_BUF_LEN, "[%s]: %i", "set", 99);
    return 0;
  } while (0);

  CONSOLE_LOG_BUF(mainBufLog, SYSTEM_CONSOLE_OUT_BUF_LEN, "[%s]: %i", "set", -99);
  return -1;
}


#endif // _FEM_MAIN_CPP