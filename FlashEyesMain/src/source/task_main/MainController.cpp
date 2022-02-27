/////////////////////////////////////////////////
// INCLUDE
#include "MainController.h"
#if (_CONF_MAIN_CONTROLLER_ENABLED)
#include "../db/DBManager.h"
#include "timer_manager/TimerManager.h"
#include "../task_net/NetManager.h"
#include "../task_net/wifi/NetWifi.h"
#include "../task_ui/UiManager.h"
#include "mess_broker/MessBrokerManager.h"
#include "../task_excomm/ExCommManager.h"
#include "../task_switch/SwitchManager.h"
#include "../task_button/ButtonManager.h"
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
  , timer_Id(TIMER_MANAGER_TIMER_ID_INVALID)
{

}

MainController::~MainController(void)
{
  this->stop();
}

bool MainController::isValid(void)
{
  return (this->system_Mode >= SYS_MODE_MAX ? false : true);
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
  SystemCriticalLocker locker(this->operation_Key);
  return this->sequence_Id;
}

Seq_t MainController::nextSeqId(void)
{
  SystemCriticalLocker locker(this->operation_Key);
  this->sequence_Id++;
  if (this->sequence_Id == 0)
  {
    this->sequence_Id++;
  }
  return this->sequence_Id;
}

Seq_t MainController::nextSeqId(Seq_t inSeqId)
{
  if (inSeqId == 0)
  {
    return this->nextSeqId();
  }
  SystemCriticalLocker locker(this->operation_Key);
  this->sequence_Id = inSeqId;
  return this->sequence_Id;
}

void MainController::regEventSize(void)
{
  this->registerHanldingEventStructSize(sizeof(EventSysPowerTAG));
}

void MainController::proc(void)
{
#ifdef MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(mainCtrlLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mTsk] proc %i %i", 0, this->systemMode());
#endif // MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
  int prepareRet = this->prepare();
  this->reportPrepareResult(prepareRet);
  if (prepareRet != 0)
  {
#ifdef MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(mainCtrlLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mTsk] proc %i", -1);
#endif // MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
    this->waitTerminating();
#ifdef MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(mainCtrlLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mTsk] proc %i", -2);
#endif // MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
    this->clear();
#ifdef MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(mainCtrlLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mTsk] proc %i", -109);
#endif // MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
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
    case (int)EventManagerConstant::EventMessageId::SysPower:
      this->onEventSysPower(eventData->bufferAddress(), eventData->dataLength());
      break;
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
  int ret = 0;
  do
  {
    ret = this->startMessBroker();
    if (ret != 0)
    {
      break;
    }

    return 0;
  } while (0);
  return 0;
}


// clear
void MainController::clear(void)
{
  this->stopMessBroker();
  return;
}

//###############below functions are implemented but called inside inherited classes##############
// start sub tasks
int MainController::startButtonTask(void)
{
  int ret = 0;
  do
  {
    {
      CONSOLE_LOG_BUF(mainCtrlLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mTask] stBtn %i", 0);

      if ( (FEM_PIN_SCAN_BUTTON == FEM_PIN_SW_1)
         || (FEM_PIN_RESET_BUTTON == FEM_PIN_SW_2)
        )
      {
        SwitchManager::getInstance().stop();
      }

      //set btn
      ButtonManagerConfigTAG btnConfig = ButtonManagerConfigTAG();
      ButtonItemTAG btnItems[FEM_BTN_BUTTON_COUNT];
      memset(&btnItems, 0, FEM_BTN_BUTTON_COUNT * sizeof(ButtonItemTAG));
      btnItems[0].pin = FEM_PIN_SCAN_BUTTON;
      btnItems[0].gpioFunc = FEM_GPIO_FUNC_SCAN_BUTTON;
      btnItems[0].triggerType = FEM_ISR_TYPE_SCAN_BUTTON;
      btnItems[0].opCode = FEM_OPCODE_SCAN_BUTTON;
      btnItems[0].cbOnPressedEx = MainController::cbButtonPressed;
      btnItems[0].cbOnPressedExArg = this;

      btnItems[1].pin = FEM_PIN_RESET_BUTTON;
      btnItems[1].gpioFunc = FEM_GPIO_FUNC_RESET_BUTTON;
      btnItems[1].triggerType = FEM_ISR_TYPE_RESET_BUTTON;
      btnItems[1].opCode = FEM_OPCODE_RESET_BUTTON;
      btnItems[1].cbOnPressedEx = MainController::cbButtonPressed;
      btnItems[1].cbOnPressedExArg = this;

      btnConfig.config.bounceTime = FEM_BTN_BUTTON_BOUNCE_TIME;
      btnConfig.config.items = btnItems;
      btnConfig.config.itemCount = FEM_BTN_BUTTON_COUNT;
      ret = ButtonManager::getInstance().start(btnConfig);
      if (ret != 0)
      {
        CONSOLE_LOG_BUF(mainCtrlLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mTask] stBtn %d", -90);
        break;
      }
    }
    CONSOLE_LOG_BUF(mainCtrlLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mTask] stBtn %d", 99);
    return 0;
  } while (0);
#ifdef MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(mainCtrlLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mTask] stBtn %i", -99);
#endif // MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
  return -1;
}

int MainController::startMessBroker(void)
{
#if (_CONF_MESS_BROKER_MANAGER_ENABLED)
  int ret = 0;
  do
  {
    {
      MBMessConfigTAG mbMessConfig[FEM_MB_MESS_ID_MAX];
      mbMessConfig[FEM_MB_MESS_ID_HEADLESS].subMaxCount = FEM_MB_SUB_MAX_HEADLESS;
      mbMessConfig[FEM_MB_MESS_ID_START1].subMaxCount = FEM_MB_SUB_MAX_START1;
      mbMessConfig[FEM_MB_MESS_ID_RESULT1].subMaxCount = FEM_MB_SUB_MAX_RESULT1;
      mbMessConfig[FEM_MB_MESS_ID_SYSTEMSETTING].subMaxCount = FEM_MB_SUB_MAX_SYSTEMSETTING;

      MBConfigTAG mbMgrConfig = MBConfigTAG();
      mbMgrConfig.messMaxCount = FEM_MB_MESS_ID_MAX;
      mbMgrConfig.messConfig = mbMessConfig;

      ret = MessBrokerManager::getInstance().start(mbMgrConfig);
#ifdef MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
      CONSOLE_LOG_BUF(mainCtrlLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mTask] stMB %i %i", 0, ret);
#endif // MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
      if (ret != 0)
      {
        break;
      }
    }

    return 0;
  } while (0);
#ifdef MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(mainCtrlLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mTask] stMB %i", -99);
#endif // MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
  return -1;
#else // _CONF_MESS_BROKER_MANAGER_ENABLED
  return 0;
#endif // _CONF_MESS_BROKER_MANAGER_ENABLED
}

int MainController::startNetManager(void)
{
  int ret = 0;
  do
  {
    {
      NetManagerConfigTAG netConfig = NetManagerConfigTAG();
      netConfig.interfaceType = FEM_NET_INTERFACE_TYPE;
      netConfig.netTaskConfig.taskManagerConfig.eventItemNumber = FEM_NET_TASK_EVENT_NUM;
      netConfig.netTaskConfig.taskManagerConfig.eventUsePool = false;
      netConfig.netTaskConfig.taskThreadConfig.enabled = true;
      netConfig.netTaskConfig.taskThreadConfig.useThreadPool = false;
      netConfig.netTaskConfig.taskThreadConfig.usStackDepth = FEM_NET_TASK_MEM;
      netConfig.netTaskConfig.taskThreadConfig.uxPriority = FEM_NET_TASK_PRIORITY;

      ret = NetManager::getInstance().startTask(netConfig);
#ifdef MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
      CONSOLE_LOG_BUF(mainCtrlLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mTask] stNet %i %i", 0, ret);
#endif // MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
      if (ret != 0)
      {
        break;
      }
    }

    //if (0)
    {
      WifiConnectionConfigTAG wifiConfig = WifiConnectionConfigTAG();
      if (SettingManager::getInstance().system().wifiMode() != 0)
      {
        wifiConfig.staConfig.ssid = SettingManager::getInstance().system().wifiSSID();
        wifiConfig.staConfig.password = SettingManager::getInstance().system().wifiPwd();
        wifiConfig.staConfig.connRetryMax = FEM_WIFI_CONN_RETRY;
        wifiConfig.staConfig.authMode = FEM_WIFI_AUTH_MODE;
        wifiConfig.staConfig.netConfig.enabledStaticIP = 0;
        wifiConfig.staConfig.netConfig.IPV4 = FEM_WIFI_STA_NET_STATIC_IP;
        wifiConfig.staConfig.netConfig.defaultGW = FEM_WIFI_STA_NET_STATIC_GW;
        wifiConfig.staConfig.netConfig.subnetMask = FEM_WIFI_STA_NET_STATIC_NETMARK;
      }
      else
      {
        wifiConfig.apConfig.ssid = SettingManager::getInstance().system().wifiSSID();
        wifiConfig.apConfig.password = SettingManager::getInstance().system().wifiPwd();
        wifiConfig.apConfig.channel = FEM_WIFI_AP_CHANNEL;
        wifiConfig.apConfig.authMode = FEM_WIFI_AUTH_MODE;
        wifiConfig.apConfig.maxConn = FEM_WIFI_AP_MAX_CONN;
        wifiConfig.apConfig.netConfig.IPV4 = FEM_WIFI_AP_NET_STATIC_IP;
        wifiConfig.apConfig.netConfig.defaultGW = FEM_WIFI_AP_NET_STATIC_GW;
        wifiConfig.apConfig.netConfig.subnetMask = FEM_WIFI_AP_NET_STATIC_NETMARK;
      }
      
      NetManagerConnectionConfigTAG netMgrconfig = NetManagerConnectionConfigTAG();
      netMgrconfig.netConnConfig.config.wifi = &wifiConfig;

      ret = NetManager::getInstance().startNet(netMgrconfig, true);
#ifdef MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
      CONSOLE_LOG_BUF(mainCtrlLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mTask] stNet %i %i", 1, ret);
#endif // MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
      if (ret != 0)
      {
        break;
      }

      
      NetIPConfigTAG ipConfig = NetIPConfigTAG();
      ret = NetManager::getInstance().getNetConfig(ipConfig);
      {
        UiMessNetStateTAG netStateTag = UiMessNetStateTAG();
        netStateTag.stateId = UI_MESS_NET_STATE_CONNECTED;
        netStateTag.stateSubId = UI_MESS_NET_STATE_SUB_NONE;
        netStateTag.mode = (SettingManager::getInstance().system().wifiMode() != 0 ? UI_MESS_NET_MODE_STA : UI_MESS_NET_MODE_AP);
        SYSTEM_PRINT_BUF(netStateTag.ip4, UI_MESS_IP4_LEN_MAX, "%d.%d.%d.%d", NET_IP4_2_STR(ipConfig.IPV4));
        UiManager::getInstance().show(UIConstant::UIMessageId::UiMessNetState, sizeof(netStateTag), (unsigned char*)&netStateTag);
      }
#ifdef MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
      CONSOLE_LOG_BUF(mainCtrlLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mTask] stNet %i %i", 2, ret);
#endif // MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
      if (ret != 0)
      {
        break;
      }
#ifdef MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
      CONSOLE_LOG_BUF(mainCtrlLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[%s] %d.%d.%d.%d", "ip", NET_IP4_2_STR(ipConfig.IPV4));
      CONSOLE_LOG_BUF(mainCtrlLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[%s] %d.%d.%d.%d", "mask", NET_IP4_2_STR(ipConfig.subnetMask));
      CONSOLE_LOG_BUF(mainCtrlLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[%s] %d.%d.%d.%d", "gw", NET_IP4_2_STR(ipConfig.defaultGW));
#endif // MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
      
    }
    
    return 0;
  } while (0);
#ifdef MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(mainCtrlLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mTask] stNet %i", -99);
#endif // MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
  return -1;
}

int MainController::startExCommManager(void)
{
  int ret = 0;
  do
  {
    // start task
    {
      ExCommManagerConfigTAG excommMgrConfig = ExCommManagerConfigTAG();
      excommMgrConfig.cbOnCommRevArg = this;
      excommMgrConfig.cbOnCommRev = MainController::cbExCommRev;
      
      ret = ExCommManager::getInstance().startTask(excommMgrConfig);
#ifdef MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
      CONSOLE_LOG_BUF(mainCtrlLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mTask] stEx %i %i", 0, ret);
#endif // MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
      if (ret != 0)
      {
        break;
      }
    }

    // start comm
    ret = ExCommManager::getInstance().startComm(true);
#ifdef MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(mainCtrlLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mTask] stEx %i %i", 1, ret);
#endif // MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
    if (ret != 0)
    {
      break;
    }

    return 0;
  } while (0);
#ifdef MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(mainCtrlLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mTask] stEx %i", -99);
#endif // MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
  return -1;
}

int MainController::startExportManager(void)
{
  int ret = 0;
  do
  {
    if (SettingManager::getInstance().system().exportMode() == FEM_EXPORT_MODE_NONE)
    {
      return 0;
    }

    // start task
    ExportManagerConfigTAG exportMgrConfig = ExportManagerConfigTAG();
    exportMgrConfig.taskManagerConfig.eventItemNumber = FEM_EXPORT_EM_EVENT_NUM;
    exportMgrConfig.taskManagerConfig.eventUsePool = FEM_EXPORT_EM_USE_POOL;

    exportMgrConfig.taskThreadConfig.enabled = true;
    exportMgrConfig.taskThreadConfig.useThreadPool = FEM_EXPORT_TM_USE_POOL;
    exportMgrConfig.taskThreadConfig.usStackDepth = FEM_EXPORT_TM_MEM;
    exportMgrConfig.taskThreadConfig.uxPriority = FEM_EXPORT_TM_PRIORITY;

    exportMgrConfig.exportProcConfig.exportProcType = ExportProcessorTypeUN::ExportProcessorHttpClient;
    exportMgrConfig.exportProcConfig.spec.httpClient.client = ExCommManager::getInstance().httpClient0();
    exportMgrConfig.exportProcConfig.spec.httpClient.reqParam.dataType = SettingManager::getInstance().system().httpCliDataType();
    exportMgrConfig.exportProcConfig.spec.httpClient.reqParam.reqMethod = SettingManager::getInstance().system().httpCliMethod();
    exportMgrConfig.exportProcConfig.spec.httpClient.reqParam.reqUri = SettingManager::getInstance().system().httpCliUri();

    ret = ExportManager::getInstance().startTask(exportMgrConfig);
#ifdef MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(mainCtrlLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mTask] stExp %i %i", 0, ret);
#endif // MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
    if (ret != 0)
    {
      break;
    }
    
    return 0;
  } while (0);
#ifdef MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(mainCtrlLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mTask] stExp %i", -99);
#endif // MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
  return -1;
}

int MainController::startScanningTask(void)
{
  int ret = 0;
  do
  {
    /*ret = this->scanning_Task.inititialize();
    if (ret != 0)
    {
      break;
    }*/

    {
      ScanningTaskConfigTAG scanningConfig = ScanningTaskConfigTAG();
      scanningConfig.parentEventer = this->eventManager();

      scanningConfig.taskManagerConfig.eventItemNumber = FEM_SCAN_EM_EVENT_NUM;
      scanningConfig.taskManagerConfig.eventUsePool = FEM_SCAN_EM_USE_POOL;

      scanningConfig.taskThreadConfig.enabled = true;
      scanningConfig.taskThreadConfig.useThreadPool = FEM_SCAN_TM_USE_POOL;
      scanningConfig.taskThreadConfig.usStackDepth = FEM_SCAN_TM_MEM;
      scanningConfig.taskThreadConfig.uxPriority = FEM_SCAN_TM_PRIORITY;

      scanningConfig.deviceConfig.dataTimeout = FEM_SCAN_DEV_DEVICE_DATA_TIMEOUT;
      scanningConfig.deviceConfig.deviceType = FEM_SCAN_DEV_DEVICE_TYPE;
      scanningConfig.deviceConfig.id = FEM_SCAN_DEV_DEVICE_ID;
      scanningConfig.deviceConfig.spec.evScanner.pin_Buzzer = FEM_SCAN_DEV_PIN_BUZZER;
      scanningConfig.deviceConfig.spec.evScanner.pin_Led = FEM_SCAN_DEV_PIN_LED;
      scanningConfig.deviceConfig.spec.evScanner.pin_Sensor = FEM_SCAN_DEV_PIN_SENSOR;
      scanningConfig.deviceConfig.spec.evScanner.serialConfig.instanceIndex = FEM_SCAN_DEV_DEVICE_SC_INSTANCE_IDX;
      scanningConfig.deviceConfig.spec.evScanner.serialConfig.baudrate = FEM_SCAN_DEV_DEVICE_SC_BAUDRATE;
      scanningConfig.deviceConfig.spec.evScanner.serialConfig.bitPerByte = FEM_SCAN_DEV_DEVICE_SC_BIT_PER_BYTE;

      scanningConfig.deviceConfig.spec.evScanner.serialConfig.parityType = FEM_SCAN_DEV_DEVICE_SC_PARITY_TYPE;
      scanningConfig.deviceConfig.spec.evScanner.serialConfig.pin_RX = FEM_SCAN_DEV_DEVICE_SC_PIN_RX;
      scanningConfig.deviceConfig.spec.evScanner.serialConfig.pin_TX = FEM_SCAN_DEV_DEVICE_SC_PIN_TX;
      scanningConfig.deviceConfig.spec.evScanner.serialConfig.stopBitNum = FEM_SCAN_DEV_DEVICE_SC_STOP_BIT;
      scanningConfig.deviceConfig.spec.evScanner.serialConfig.timeout = FEM_SCAN_DEV_DEVICE_SC_TIMEOUT;

      ret = this->scanning_Task.startTask(scanningConfig);
      if (ret != 0)
      {
        break;
      }
    }
    return 0;
  } while (0);
  return -1;
}


int MainController::startDistSensorTask(void)
{
  int ret = 0;
  do
  {
#ifdef MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(mainCtrlLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mTask] stSS %d", 0);
#endif // MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
    {
      DistSensorTaskConfigTAG distSensorConfig = DistSensorTaskConfigTAG();
      distSensorConfig.taskManagerConfig.eventItemNumber = FEM_DIST_SS_EM_EVENT_NUM;
      distSensorConfig.taskManagerConfig.eventUsePool = FEM_DIST_SS_EM_USE_POOL;

      distSensorConfig.taskThreadConfig.enabled = true;
      distSensorConfig.taskThreadConfig.useThreadPool = FEM_DIST_SS_TM_USE_POOL;
      distSensorConfig.taskThreadConfig.usStackDepth = FEM_DIST_SS_TM_MEM;
      distSensorConfig.taskThreadConfig.uxPriority = FEM_DIST_SS_TM_PRIORITY;
      
      
      distSensorConfig.config.measuringInterval = FEM_DIST_SS_MEAS_INTERVAL;
      distSensorConfig.config.opMode = FEM_DIST_SS_OP_MODE;
      distSensorConfig.config.trgCb = MainController::cbDistSensorTrg;
      distSensorConfig.config.trgUserArg = this;
      distSensorConfig.config.validRange.rangeBegin = SettingManager::getInstance().scanner().distSSRangeBegin();// FEM_DIST_SS_VALID_RANGE_BEGIN;
      distSensorConfig.config.validRange.rangeEnd = SettingManager::getInstance().scanner().distSSRangeEnd();// FEM_DIST_SS_VALID_RANGE_END;

      

      distSensorConfig.deviceConfig.deviceId = FEM_DIST_SS_DEV_ID;
      distSensorConfig.deviceConfig.deviceType = FEM_DIST_SS_DEV_DEVICE_TYPE;
      distSensorConfig.deviceConfig.spec.hrc04.pin_Echo = FEM_DIST_SS_DEV_DEVICE_HRC04_PIN_ECHO;
      distSensorConfig.deviceConfig.spec.hrc04.pin_Trigger = FEM_DIST_SS_DEV_DEVICE_HRC04_PIN_TRG;

      ret = this->dist_Sensor_Task.startTask(distSensorConfig);
      if (ret != 0)
      {
        break;
      }
    }
#ifdef MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(mainCtrlLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mTask] stSS %d", 99);
#endif // MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
    return 0;
  } while (0);
#ifdef MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(mainCtrlLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mTask] stSS %d", -99);
#endif // MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
  return -1;
}

// stop sub tasks

void MainController::stopDistSensorTask(void)
{
  this->dist_Sensor_Task.stopTask();
#ifdef MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(mainCtrlLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mTsk] stopSs %i", 99);
#endif // MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
  return;
}

void MainController::stopScanningTask(void)
{
  this->scanning_Task.stopTask();
#ifdef MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(mainCtrlLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mTsk] stopSc %i", 99);
#endif // MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
  return;
}

void MainController::stopExportManager(void)
{
  ExportManager::getInstance().stopTask();
#ifdef MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(mainCtrlLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mTsk] stopExp %i", 99);
#endif // MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
  return;
}

void MainController::stopExCommManager(void)
{
  ExCommManager::getInstance().stopComm(true);
  ExCommManager::getInstance().stopTask();
#ifdef MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(mainCtrlLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mTsk] stopEx %i", 99);
#endif // MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
  return;
}

void MainController::stopNetManager(void)
{
  NetManager::getInstance().stopTask();
#ifdef MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(mainCtrlLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mTsk] stopNet %i", 99);
#endif // MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
  return;
}

void MainController::stopMessBroker(void)
{
#if (_CONF_MESS_BROKER_MANAGER_ENABLED)
  MessBrokerManager::getInstance().stop();
#endif // _CONF_MESS_BROKER_MANAGER_ENABLED
  return;
}


void MainController::stopButtonTask(void)
{
  ButtonManager::getInstance().stop();
  return;
}

int MainController::onEventScanningDeviceSetting(unsigned char* data, unsigned int dataSize)
{
  int ret = 0;
  do
  {
#ifdef MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(mainCtrlLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mTsk] setS %i", 0);
#endif // MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
    if (this->isBusy() != false)
    {
      return -1;
    }

    if ((data == NULL)
      || (dataSize != sizeof(EventScanningDeviceSettingTAG))
      )
    {
      return -1;
    }

    EventScanningDeviceSettingTAG* eventData = (EventScanningDeviceSettingTAG*)data;
    eventData->seqId = this->nextSeqId();
    ret = this->scanning_Task.notify(
      EventManagerConstant::EventMessageId::ScanningDeviceSetting
      , sizeof(EventScanningDeviceSettingTAG)
      , (unsigned char*)eventData
    );
#ifdef MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(mainCtrlLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mTsk] setS %i %i %i", 5, ret, eventData->seqId);
#endif // MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
    if (ret != 0)
    {
      break;
    }

    this->isBusy(true);
    ret = this->startTimer(FEM_SCAN_OP_TIMEOUT);
    if (ret != 0)
    {
      break;
    }
    UiManager::getInstance().showSysState(UI_MESS_SYS_STATE_SETTING, UI_MESS_SYS_STATE_SUB_START);
#ifdef MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(mainCtrlLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mTsk] setS %i %i", 99, eventData->seqId);
#endif // MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
    return 0;
  } while (0);
  this->resetSequence();
#ifdef MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(mainCtrlLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mTsk] setS %i", -99);
#endif // MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
  return -1;
}

int MainController::onEventScanningDeviceSettingCompleted(unsigned char* data, unsigned int dataSize)
{
  int ret = 0;
  do
  {
#ifdef MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(mainCtrlLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mTsk] seC %i", 0);
#endif // MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
    if (this->isBusy() == false)
    {
      break;
    }

    if ((data == NULL)
      || (dataSize != sizeof(EventScanningDeviceSettingCompletedTAG))
      )
    {
      break;
    }

    EventScanningDeviceSettingCompletedTAG* eventData = (EventScanningDeviceSettingCompletedTAG*)data;
    if (eventData->seqId != this->curSeqId())
    {
      break;
    }
#ifdef MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(mainCtrlLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mTsk] seC %i %ld", 97, (long)(eventData->result.errorSet0 >> 32));
    CONSOLE_LOG_BUF(mainCtrlLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mTsk] seC %i %ld", 98, (long)eventData->result.errorSet0);
#endif // MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
    this->resetSequence();
    return 0;
  } while (0);

#ifdef MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(mainCtrlLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mTsk] seC %i", -99);
#endif // MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
  return -1;
}


int MainController::onEventScanningControl(unsigned char* data, unsigned int dataSize)
{
  int ret = 0;
  do
  {
    if ((data == NULL)
      || (dataSize != sizeof(EventScanningControlTAG))
      )
    {
      return -1;
    }

    EventScanningControlTAG* controlEvent = (EventScanningControlTAG*)data;
    if (controlEvent->isStart != 0)
    {
      return this->onEventScanningStart(data, dataSize);
    }
    return this->onEventScanningStop(data, dataSize);
    
    return 0;
  } while (0);
  return -1;
}


int MainController::onEventScanningStart(unsigned char* data, unsigned int dataSize)
{
  int ret = 0;
  do
  {
#ifdef MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(mainCtrlLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mTsk] sTrg %i", 0);
#endif // MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
    if (this->isBusy() != false)
    {
#ifdef MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
      CONSOLE_LOG_BUF(mainCtrlLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mTsk] sTrg %i", -1);
#endif // MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
      return -1;
    }

    EventScanningControlTAG* controlEvent = (EventScanningControlTAG*)data;
    if (controlEvent->trgParams.trgSource != SettingManager::getInstance().scanner().triggerMode())
    {
#ifdef MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
      CONSOLE_LOG_BUF(mainCtrlLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mTsk] sTrg %i", -2);
#endif // MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
      return -1;
    }

    EventScanningStartTAG scanningStartEvent = EventScanningStartTAG();
    scanningStartEvent.sequenceId = this->nextSeqId(controlEvent->sequenceId);
    memcpy(&scanningStartEvent.trgParams, &controlEvent->trgParams, sizeof(TrgStartScanningParamTAG));

    ret = this->scanning_Task.notify(
      EventManagerConstant::EventMessageId::ScanningStart
      , sizeof(EventScanningStartTAG)
      , (unsigned char*)&scanningStartEvent
    );
#ifdef MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(mainCtrlLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mTsk] sTrg %i %i %d", 5, ret, scanningStartEvent.trgParams.timeout);
#endif // MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
    if (ret != 0)
    {
      break;
    }

    this->isBusy(true);
    ret = this->startTimer(scanningStartEvent.trgParams.timeout);// FEM_SCAN_OP_TIMEOUT);
    if (ret != 0)
    {
      break;
    }
    UiManager::getInstance().showSysState(UI_MESS_SYS_STATE_SCANNING, UI_MESS_SYS_STATE_SUB_START);
#ifdef MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(mainCtrlLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mTsk] sTrg %i %i %i", 99, scanningStartEvent.sequenceId, scanningStartEvent.trgParams.maxScanCount);
#endif // MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
    return 0;
  } while (0);
  this->resetSequence();
#ifdef MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(mainCtrlLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mTsk] sTrg %i", -99);
#endif // MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
  return -1;
}

int MainController::onEventScanningStop(unsigned char* data, unsigned int dataSize)
{
  int ret = 0;
  do
  {
#ifdef MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(mainCtrlLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mTsk] sStp %i", 0);
#endif // MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
    if (this->isBusy() == false)
    {
#ifdef MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
      CONSOLE_LOG_BUF(mainCtrlLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mTsk] sStp %i", -1);
#endif // MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
      return -1;
    }

    EventScanningControlTAG* controlEvent = (EventScanningControlTAG*)data;
    if ( (controlEvent->sequenceId!=0) 
      && (controlEvent->sequenceId != this->curSeqId())
      )
    {
#ifdef MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
      CONSOLE_LOG_BUF(mainCtrlLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mTsk] sStp %i", -20);
#endif // MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
      return -1;
    }

    this->resetSequence();
    //UiManager::getInstance().showSysState(UI_MESS_SYS_STATE_SCANNING, UI_MESS_SYS_STATE_SUB_END);
#ifdef MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(mainCtrlLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mTsk] sStp %i", 99);
#endif // MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
    return 0;
  } while (0);
  this->resetSequence();
#ifdef MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(mainCtrlLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mTsk] sStp %i", -99);
#endif // MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
  return -1;
}

int MainController::onEventScanningResult(unsigned char* data, unsigned int dataSize)
{
  int ret = 0;
  do
  {
#ifdef MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(mainCtrlLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mTsk] sre %i", 0);
#endif // MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
    if (this->isBusy() == false)
    {
      break;
    }

    if ((data == NULL)
      || (dataSize != sizeof(EventScanningResultTAG))
      )
    {
      break;
    }

    EventScanningResultTAG* eventData = (EventScanningResultTAG*)data;
    if (eventData->result.sequenceId != this->curSeqId())
    {
      break;
    }

    if (ExportManager::getInstance().isRunning() != false)
    {
      ExportScanResultTAG scanResult = ExportScanResultTAG();
      scanResult.sequenceId = eventData->result.sequenceId;
      scanResult.scanIndex = eventData->result.scanIndex;
      scanResult.code.type = eventData->result.deviceResult.code.type;
      scanResult.code.codeLen = SYSTEM_MIN(eventData->result.deviceResult.code.codeLen, EXPORT_SCANNING_DEVICE_BARCODE_LEN_MAX);
      memcpy(scanResult.code.code, eventData->result.deviceResult.code.code, scanResult.code.codeLen);
      ExportManager::getInstance().exportScanResult(scanResult);
    }

    {
      UiMessScanResultTAG scanResult = UiMessScanResultTAG();
      scanResult.sequenceId = eventData->result.sequenceId;
      scanResult.scanIndex = eventData->result.scanIndex;
      scanResult.code.type = eventData->result.deviceResult.code.type;
      scanResult.code.codeLen = SYSTEM_MIN(eventData->result.deviceResult.code.codeLen, UI_SCANNING_DEVICE_BARCODE_LEN_MAX);
      memcpy(scanResult.code.code, eventData->result.deviceResult.code.code, scanResult.code.codeLen);
      UiManager::getInstance().show(UIConstant::UIMessageId::UiMessScanResult, sizeof(scanResult), (unsigned char*)&scanResult);
    }
    
#ifdef MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(mainCtrlLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mTsk] sre %i %i %i %i", 10, eventData->result.sequenceId, eventData->result.scanIndex, eventData->result.deviceResult.errorId);
    CONSOLE_LOG_BUF(mainCtrlLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mTsk] sre %i %i %i", 11, eventData->result.deviceResult.code.type, eventData->result.deviceResult.code.codeLen);
    CONSOLE_LOG_BUF(mainCtrlLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mTsk] sre %i %s", 12, eventData->result.deviceResult.code.code);
#endif // MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
    return 0;
  } while (0);

#ifdef MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(mainCtrlLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mTsk] ssto %i", -99);
#endif // MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
  return -1;
}

int MainController::onEventScanningCompleted(unsigned char* data, unsigned int dataSize)
{
  int ret = 0;
  do
  {
//#ifdef MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
//    CONSOLE_LOG_BUF(mainCtrlLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mTsk] sco %i", 0);
//#endif // MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
    if (this->isBusy() == false)
    {
      break;
    }

    if ((data == NULL)
      || (dataSize != sizeof(EventScanningCompletedTAG))
      )
    {
      break;
    }

    EventScanningCompletedTAG* eventData = (EventScanningCompletedTAG*)data;
    if (eventData->sequenceId != this->curSeqId())
    {
      break;
    }
    this->resetSequence();
    return 0;
  } while (0);

//#ifdef MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
//  CONSOLE_LOG_BUF(mainCtrlLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mTsk] sco %i", -99);
//#endif // MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
  return -1;
}

int MainController::onEventTimerFired1(unsigned char* data, unsigned int dataSize)
{
  int ret = 0;
  do
  {
#ifdef MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(mainCtrlLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mTsk] tim %i", 0);
#endif // MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE

    if ((data == NULL)
      || (dataSize != sizeof(EventTimerFiredTAG))
      )
    {
      break;
    }

    EventTimerFiredTAG* eventData = (EventTimerFiredTAG*)data;
    if (eventData->timerId != this->timer_Id)
    {
#ifdef MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
      CONSOLE_LOG_BUF(mainCtrlLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mTsk] tim %d %d %d", -3, eventData->timerId, this->timer_Id);
#endif // MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
      break;
    }
    this->resetSequence();
#ifdef MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(mainCtrlLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mTsk] tim %i", 10);
#endif // MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
    return 0;
  } while (0);

#ifdef MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(mainCtrlLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mTsk] tim %i", -99);
#endif // MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
  return -1;
}

int MainController::onEventSysPower(unsigned char* data, unsigned int dataSize)
{
  int ret = 0;
  do
  {
#ifdef MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(mainCtrlLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mTsk] pow %i", 0);
#endif // MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE

    if ((data == NULL)
      || (dataSize != sizeof(EventSysPowerTAG))
      )
    {
      break;
    }
    bool forwardEvent = false;
    EventSysPowerTAG* eventData = (EventSysPowerTAG*)data;
    switch (eventData->opCode)
    {
      case FEM_SYS_POWER_RESTART_FORCE:
        break;
      case FEM_SYS_POWER_RESTART:
        if (this->isBusy() != false)
        {
          forwardEvent = true;
        }
        break;
      default:
        ret = -1;
        break;
    }

    if (ret != 0)
    {
      break;
    }

    if (forwardEvent == false)
    {
      SYSTEM_SLEEP(eventData->delay);
      SYSTEM_REBOOT();
      return 0;
    }

    ret  =this->notify((int)EventManagerConstant::EventMessageId::SysPower, dataSize, data);
    if (ret != 0)
    {
      break;
    }
#ifdef MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(mainCtrlLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mTsk] pow %i", 10);
#endif // MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
    return 0;
  } while (0);

#ifdef MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(mainCtrlLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mTsk] pow %i", -99);
#endif // MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
  return -1;
}

//mbc comm handler
int MainController::onMbcCommMBCStart1(ExCommMBCParamTAG& mbcParams)
{
  int ret = 0;
  do
  {
    if (mbcParams.reqPackage.data == NULL)
    {
      break;
    }
    // print
    CommMBCStart1TAG* commData = (CommMBCStart1TAG*)mbcParams.reqPackage.data;
#ifdef MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(mainCtrlLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mTsk] mbcSta %i %d %d", 0, commData->param1, commData->param2);
#endif // MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE

    return 0;
  } while (0);
  return -1;
}

int MainController::onMbcCommMBCSystemSetting(ExCommMBCParamTAG& mbcParams)
{
  int ret = 0;
  do
  {
#ifdef MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(mainCtrlLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mTsk] mbcSS %i", 0);
#endif // MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
    if (mbcParams.reqPackage.data == NULL)
    {
      break;
    }

    // req
    CommMBCSystemSettingTAG* reqCommData = (CommMBCSystemSettingTAG*)mbcParams.reqPackage.data;
    // response
    CommMBCSystemSettingTAG* resCommData = reqCommData;
    resCommData->errorCode = 0;
    resCommData->bitSet1.isReply = 1;

    if (reqCommData->bitSet1.isUpdate == 0)
    {
      ret = SettingManager::getInstance().get(*resCommData);
    }
    else
    {
      ret = SettingManager::getInstance().set(*resCommData);
    }
    
#ifdef MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(mainCtrlLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mTsk] mbcSS %d %d %d %d %d", 10, ret, resCommData->sectionId, resCommData->settingId, resCommData->sLen);
#endif // MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
    if (ret != 0)
    {
      resCommData->errorCode = 1;
    }
    
    mbcParams.resPackage.data = (unsigned char*)reqCommData;
    mbcParams.resPackage.dataSize = sizeof(CommMBCSystemSettingTAG);

#ifdef MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(mainCtrlLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mTsk] mbcSS %d", 97);
#endif // MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
    if (reqCommData->bitSet1.isRebootRequired == 0)
    {
      return 0;
    }

    EventSysPowerTAG powerEvent = EventSysPowerTAG();
    powerEvent.opCode = FEM_SYS_POWER_RESTART;
    powerEvent.delay = FEM_SYS_POWER_DELAY;

    ret = this->notify((int)EventManagerConstant::EventMessageId::SysPower, sizeof(EventSysPowerTAG), (unsigned char*)&powerEvent);
    if (ret != 0)
    {
      break;
    }
#ifdef MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(mainCtrlLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mTsk] mbcSS %i", 99);
#endif // MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE

    return 0;
  } while (0);
#ifdef MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(mainCtrlLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mTsk] mbcSS %i", -99);
#endif // MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
  return -1;
}

int MainController::onMbcCommMBCScanningControl(ExCommMBCParamTAG& mbcParams)
{
  int ret = 0;
  do
  {
#ifdef MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(mainCtrlLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mTsk] mbcSCtrl %i", 0);
#endif // MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
    if (mbcParams.reqPackage.data == NULL)
    {
      break;
    }

    // req
    CommMBCScanningControlTAG* reqCommData = (CommMBCScanningControlTAG*)mbcParams.reqPackage.data;
    // response
    CommMBCScanningControlTAG* resCommData = reqCommData;
    resCommData->errorCode = 0;
    resCommData->bitSet1.isReply = 1;

    {
#ifdef MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
      CONSOLE_LOG_BUF(mainCtrlLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mTsk] mbcSCtrl %d %d %d", 1, reqCommData->sequenceId, (uint16_t)reqCommData->trgParams.timeout);
      CONSOLE_LOG_BUF(mainCtrlLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mTsk] mbcSCtrl %d %d %d", 2, reqCommData->bitSet1.isReply, reqCommData->bitSet1.isStart);
#endif // MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
      EventScanningControlTAG scanningCtrlEvent = EventScanningControlTAG();
      scanningCtrlEvent.sequenceId = reqCommData->sequenceId;
      scanningCtrlEvent.isStart = reqCommData->bitSet1.isStart;
      scanningCtrlEvent.trgParams.enabled = reqCommData->trgParams.enabled;
      scanningCtrlEvent.trgParams.maxScanCount = reqCommData->trgParams.maxScanCount;
      scanningCtrlEvent.trgParams.timeBtwScan = reqCommData->trgParams.timeBtwScan;
      scanningCtrlEvent.trgParams.timeout = reqCommData->trgParams.timeout;
      scanningCtrlEvent.trgParams.trgSource = reqCommData->trgParams.trgSource;
    
      ret = this->notify((int)EventManagerConstant::EventMessageId::ScanningControl, sizeof(EventScanningControlTAG), (unsigned char*)&scanningCtrlEvent);
    }
#ifdef MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(mainCtrlLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mTsk] mbcSCtrl %d %d", 10, ret);
#endif // MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
    if (ret != 0)
    {
      resCommData->errorCode = 1;
    }

    mbcParams.resPackage.data = (unsigned char*)resCommData;
    mbcParams.resPackage.dataSize = sizeof(CommMBCScanningControlTAG);

#ifdef MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(mainCtrlLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mTsk] mbcSCtrl %d %d", 90, resCommData->trgParams.timeBtwScan);
    CONSOLE_LOG_BUF(mainCtrlLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mTsk] mbcSCtrl %i", 99);
#endif // MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE

    return 0;
  } while (0);
#ifdef MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(mainCtrlLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mTsk] mbcSCtrl %i", -99);
#endif // MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
  return -1;
}

int MainController::onMbcCommMBCScanningResult(ExCommMBCParamTAG& mbcParams)
{
  int ret = 0;
  do
  {
#ifdef MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(mainCtrlLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mTsk] mbcSCRe %i", 0);
#endif // MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
    if (mbcParams.reqPackage.data == NULL)
    {
      break;
    }

    // check if it is http mode
    if (SettingManager::getInstance().scanner().triggerMode() != FEM_SCAN_TRG_SRC_HTTP)
    {
      break;
    }

    // req
    CommMBCScanningResultTAG* reqCommData = (CommMBCScanningResultTAG*)mbcParams.reqPackage.data;
    if (reqCommData->bitSet1.isReply != 0)
    {
      break;
    }
    // response
    Seq_t targetSeqId = reqCommData->sequenceId;
    CommMBCScanningResultTAG* resCommData = reqCommData;
    resCommData->bitSet1.isReply = 1;
    resCommData->bitSet1.enabled = false;
    resCommData->sequenceId = 0;
    resCommData->scanIndex = 0;
    resCommData->errorCode = FEM_SCAN_ERR_CODE_NONE;
    resCommData->deviceResult.bitSet1.enabled = 0;
    resCommData->deviceResult.bitSet1.reserved = 0;
    resCommData->deviceResult.errorId = 0;
    resCommData->deviceResult.code.codeLen = 0;
    resCommData->deviceResult.code.type = 0;
    resCommData->deviceResult.code.code = (char*)(resCommData + sizeof(CommMBCScanningResultTAG));
    resCommData->deviceResult.code.code[0] = '\0';

#ifdef MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(mainCtrlLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mTsk] mbcSCRe %d %d %s", 1, resCommData->deviceResult.code.codeLen, resCommData->deviceResult.code.code);
#endif // MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
    {
      ScanningResultTAG scannRets[SCANNING_SAVED_RESULT_COUNT_MAX];
      memset(&scannRets, 0, sizeof(ScanningResultTAG)* SCANNING_SAVED_RESULT_COUNT_MAX);
      ret = this->scanning_Task.getSavedScanningResult(SCANNING_SAVED_RESULT_COUNT_MAX, scannRets);
      if (ret != 0)
      {
        resCommData->errorCode = FEM_SCAN_ERR_CODE_OP_ERR;
        goto onMbcCommMBCScanningResult_res;
      }
      
      ScanningResultTAG* selectedRet = NULL;
      for (byte wkIdx = 0; wkIdx < SCANNING_SAVED_RESULT_COUNT_MAX; wkIdx++)
      {
//#ifdef MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
//        CONSOLE_LOG_BUF(mainCtrlLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mTsk] mbcSCRe %d %d %d %d %d", 5, wkIdx, scannRets[wkIdx].sequenceId, scannRets[wkIdx].enabled, scannRets[wkIdx].scanIndex);
//#endif // MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
        if ((scannRets[wkIdx].enabled == 0)
          || (scannRets[wkIdx].sequenceId != targetSeqId)
          )
        {
          continue;
        }
        selectedRet = &scannRets[wkIdx];
//#ifdef MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
//        CONSOLE_LOG_BUF(mainCtrlLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mTsk] mbcSCRe %d %d %d %d %d", 5, wkIdx, selectedRet->sequenceId, selectedRet->enabled, selectedRet->scanIndex);
//#endif // MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
        break;
      }

      if (selectedRet == NULL)
      {
        resCommData->errorCode = FEM_SCAN_ERR_CODE_NOT_AVAI;
        goto onMbcCommMBCScanningResult_res;
      }
      resCommData->bitSet1.enabled = true;
      resCommData->sequenceId = selectedRet->sequenceId;
      resCommData->scanIndex = selectedRet->scanIndex;
      resCommData->errorCode = FEM_SCAN_ERR_CODE_NONE;
      resCommData->deviceResult.bitSet1.enabled = selectedRet->deviceResult.bitSet1.enabled;
      resCommData->deviceResult.bitSet1.reserved = selectedRet->deviceResult.bitSet1.reserved;
      resCommData->deviceResult.errorId = selectedRet->deviceResult.errorId;
      resCommData->deviceResult.code.codeLen = SYSTEM_MIN(selectedRet->deviceResult.code.codeLen, COMM_MBC_SCANNING_DEVICE_BARCODE_LEN_MAX);
      resCommData->deviceResult.code.type = selectedRet->deviceResult.code.type;
      memcpy(resCommData->deviceResult.code.code, selectedRet->deviceResult.code.code, resCommData->deviceResult.code.codeLen);
    }
//#ifdef MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
//    CONSOLE_LOG_BUF(mainCtrlLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mTsk] mbcSCRe %d %d", 10, ret);
//    CONSOLE_LOG_BUF(mainCtrlLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mTsk] mbcSCRe %d %d", 11, resCommData->scanIndex);
//#endif // MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
    
onMbcCommMBCScanningResult_res:
    mbcParams.resPackage.data = (unsigned char*)resCommData;
    mbcParams.resPackage.dataSize = sizeof(CommMBCScanningResultTAG);
//#ifdef MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
//    CONSOLE_LOG_BUF(mainCtrlLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mTsk] mbcSCRe %d %d", 90, resCommData->errorCode);
//#endif // MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE

#ifdef MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(mainCtrlLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mTsk] mbcSCRe %i", 99);
#endif // MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE

    return 0;
  } while (0);
#ifdef MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(mainCtrlLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mTsk] mbcSCRe %i", -99);
#endif // MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
  return -1;
}

void MainController::resetSequence(void)
{
  this->stopTimer();
  this->stopScanning();
  this->isBusy(false);
  {
    DistSensorControlParamsTAG ssStrParam = DistSensorControlParamsTAG();
    ssStrParam.opCode = DIST_SENSOR_CTRL_OPCODE_START;
    ssStrParam.deviceId = FEM_DIST_SS_DEV_ID;
    ssStrParam.sequenceId = 0;
    ssStrParam.mode = FEM_DIST_SS_OP_MODE;
    this->dist_Sensor_Task.control(ssStrParam);
  }
  UiManager::getInstance().showSysState(UI_MESS_SYS_STATE_SCANNING, UI_MESS_SYS_STATE_SUB_END);
}

int MainController::startTimer(TimePoint_t timeout)
{
  do
  {
    this->stopTimer();
    if (timeout <= 0)
    {
      return 0; // no need to set
    }

    this->timer_Id = TimerManager::getInstance().start(MainController::cbTimerFired, this, timeout, true);
    if (this->timer_Id == TIMER_MANAGER_TIMER_ID_INVALID)
    {
      break;
    }

    return 0;
  } while (0);
  this->stopTimer();
  return -1;
}

void MainController::stopTimer(void)
{
  do
  {
    if (this->timer_Id == TIMER_MANAGER_TIMER_ID_INVALID)
    {
      return;
    }

    TimerManager::getInstance().stop(this->timer_Id);
    this->timer_Id = TIMER_MANAGER_TIMER_ID_INVALID;
    return;
  } while (0);
  return;
}

void MainController::stopScanning(void)
{
  int ret = 0;
  do
  {
    if (this->isBusy() == false)
    {
      return;
    }

    EventScanningStopTAG scanningStopEvent = EventScanningStopTAG();
    scanningStopEvent.sequenceId = this->curSeqId();
    ret = this->scanning_Task.notify(
      EventManagerConstant::EventMessageId::ScanningStop
      , sizeof(EventScanningStopTAG)
      , (unsigned char*)&scanningStopEvent
    );
    if (ret != 0)
    {
      break;
    }

    return;
  } while (0);
  return;
}

// static
void MainController::cbTimerFired(TimerId_t timerId, void* extraArg, bool* woken)
{
  do
  {
    if (extraArg == NULL)
    {
      break;
    }
    EventTimerFiredTAG eventData = EventTimerFiredTAG();
    eventData.timerId = timerId;
    eventData.extraArg = extraArg;
    ((MainController*)extraArg)->notifyFromISR(
      EventManagerConstant::EventMessageId::TimerFired1
      , sizeof(EventTimerFiredTAG)
      , (unsigned char*)&eventData
      , woken);

    return;
  } while (0);
  return;
}

int MainController::cbExCommRev(void* arg, ExCommMBCParamTAG& mbcParams)
{
  int ret = 0;
  do
  {
    if (arg == NULL)
    {
      break;
    }

    MainController* mainCtl = (MainController*)arg;

    mbcParams.resPackage.valid = true;
    mbcParams.resPackage.messId = mbcParams.reqPackage.messId;
    mbcParams.resPackage.data = mbcParams.reqPackage.data;
    mbcParams.resPackage.dataSize = mbcParams.reqPackage.dataSize;

    switch (mbcParams.reqPackage.messId)
    {
      case (int)CommMBCConstant::CommMBCMessageId::CommMBCStart1:
        ret = mainCtl->onMbcCommMBCStart1(mbcParams);
        break;
      case (int)CommMBCConstant::CommMBCMessageId::CommMBCSystemSetting:
        ret = mainCtl->onMbcCommMBCSystemSetting(mbcParams);
        break;
      case (int)CommMBCConstant::CommMBCMessageId::CommMBCScanningControl:
        ret = mainCtl->onMbcCommMBCScanningControl(mbcParams);
        break;
      case (int)CommMBCConstant::CommMBCMessageId::CommMBCScanningResult:
        ret = mainCtl->onMbcCommMBCScanningResult(mbcParams);
        break;
      default:
        ret = -1;
        break;
    }
    
    if (ret != 0)
    {
      break;
    }

    return 0;
  } while (0);
  return -1;
}

void MainController::cbButtonPressed(void* arg, byte opCode, bool* woken)
{
  int ret = 0;
  do
  {
//#ifdef MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
//    CONSOLE_LOG_ISR(mainCtrlLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mTsk] cbBP %d %d", 0, opCode);
//#endif // MAIN_CONTROLLER_CONSOLE_DEBUG_ENABLE
    if (arg == NULL)
    {
      break;
    }

    switch (opCode)
    {
      case FEM_BTN_OPCODE_SCAN_BTN:
      {
        EventScanningControlTAG scanningCtrlEvent = EventScanningControlTAG();
        scanningCtrlEvent.sequenceId = 0;
        scanningCtrlEvent.isStart = 1;
        scanningCtrlEvent.trgParams.enabled = 1;
        scanningCtrlEvent.trgParams.maxScanCount = FEM_SCAN_OP_MAX_SCAN_COUNT;
        scanningCtrlEvent.trgParams.timeBtwScan = FEM_SCAN_OP_BREAK_TIME;
        scanningCtrlEvent.trgParams.timeout = FEM_SCAN_OP_TIMEOUT;
        scanningCtrlEvent.trgParams.trgSource = FEM_SCAN_TRG_SRC_BUTTON;
        ret = ((MainController*)arg)->notifyFromISR((int)EventManagerConstant::EventMessageId::ScanningControl, sizeof(EventScanningControlTAG), (unsigned char*)&scanningCtrlEvent, woken);
        break;
      }
      case FEM_BTN_OPCODE_RESET_BTN:
      {
        EventScanningControlTAG scanningCtrlEvent = EventScanningControlTAG();
        scanningCtrlEvent.sequenceId = 0;
        scanningCtrlEvent.isStart = 0;
        scanningCtrlEvent.trgParams.enabled = 1;
        scanningCtrlEvent.trgParams.maxScanCount = FEM_SCAN_OP_MAX_SCAN_COUNT;
        scanningCtrlEvent.trgParams.timeBtwScan = FEM_SCAN_OP_BREAK_TIME;
        scanningCtrlEvent.trgParams.timeout = FEM_SCAN_OP_TIMEOUT;
        scanningCtrlEvent.trgParams.trgSource = FEM_SCAN_TRG_SRC_BUTTON;
        ret = ((MainController*)arg)->notifyFromISR((int)EventManagerConstant::EventMessageId::ScanningControl, sizeof(EventScanningControlTAG), (unsigned char*)&scanningCtrlEvent, woken);
        break;
      }
      default:
        break;
    }
  } while (0);
  
}

void MainController::cbDistSensorTrg(void* userArg, Seq_t sequenceId, Dist_t distance, byte mode)
{
  int ret = 0;
  do
  {
    if (userArg == NULL)
    {
      break;
    }

    EventScanningControlTAG scanningCtrlEvent = EventScanningControlTAG();
    scanningCtrlEvent.sequenceId = 0;
    scanningCtrlEvent.isStart = 1;
    scanningCtrlEvent.trgParams.enabled = 1;
    scanningCtrlEvent.trgParams.maxScanCount = FEM_SCAN_OP_MAX_SCAN_COUNT;
    scanningCtrlEvent.trgParams.timeBtwScan = FEM_SCAN_OP_BREAK_TIME;
    scanningCtrlEvent.trgParams.timeout = FEM_SCAN_OP_TIMEOUT;
    scanningCtrlEvent.trgParams.trgSource = FEM_SCAN_TRG_SRC_SENSOR;
    ((MainController*)userArg)->notify((int)EventManagerConstant::EventMessageId::ScanningControl, sizeof(EventScanningControlTAG), (unsigned char*)&scanningCtrlEvent);
    
  } while (0);

}
#endif // _CONF_MAIN_CONTROLLER_ENABLED