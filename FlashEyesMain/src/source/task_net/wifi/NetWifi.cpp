#include "NetWifi.h"
#if (_CONF_NET_WIFI_ENABLED)
/////////////////////////////////////////////////
// INCLUDE

/////////////////////////////////////////////////
// PREPROCESSOR
//#define NET_WIFI_CONSOLE_DEBUG_ENABLE
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
#ifdef NET_WIFI_CONSOLE_DEBUG_ENABLE
char netWifiLogBuf[SYSTEM_CONSOLE_OUT_BUF_LEN];
#endif // NET_WIFI_CONSOLE_DEBUG_ENABLE
/////////////////////////////////////////////////
// STATIC FUNCTIONS
static void cbWifiEventHandler(void* arg, esp_event_base_t eventBase, int32_t eventId, void* eventData);
/////////////////////////////////////////////////
// EXTERN

/////////////////////////////////////////////////
// CLASS IMPLEMENTAION
/*NetWifi*/

// static 
byte NetWifi::g_Instance_Count = 0;
WifiNetHandler_t NetWifi::g_AP_Handler = NULL;
WifiNetHandler_t NetWifi::g_STA_Handler = NULL;
int NetWifi::globalInit(void)
{
  do
  {
    NetWifi::g_Instance_Count++;
    if (NetWifi::g_Instance_Count > 1)
    {
      return 0; // already
    }
#ifdef NET_WIFI_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(netWifiLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[WFg] init %i", 0);
#endif // NET_WIFI_CONSOLE_DEBUG_ENABLE
    // first instance
    WifiRet_T ret = nvs_flash_init();
    /*if ( (ret == ESP_ERR_NVS_NO_FREE_PAGES)
      || (ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
      )
    {
      ret = nvs_flash_erase();
      if (ret != WIFI_DRV_RET_OK)
      {
        break;
      }
      ret = nvs_flash_init();
    }*/

    if (ret != WIFI_DRV_RET_OK)
    {
#ifdef NET_WIFI_CONSOLE_DEBUG_ENABLE
      CONSOLE_LOG_BUF(netWifiLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[WFg] init %i", -1);
#endif // NET_WIFI_CONSOLE_DEBUG_ENABLE
      break;
    }

    ret = esp_netif_init();
    if (ret != WIFI_DRV_RET_OK)
    {
#ifdef NET_WIFI_CONSOLE_DEBUG_ENABLE
      CONSOLE_LOG_BUF(netWifiLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[WFg] init %i", -2);
#endif // NET_WIFI_CONSOLE_DEBUG_ENABLE
      break;
    }

    ret = esp_event_loop_create_default();
    if (ret != WIFI_DRV_RET_OK)
    {
#ifdef NET_WIFI_CONSOLE_DEBUG_ENABLE
      CONSOLE_LOG_BUF(netWifiLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[WFg] init %i", -3);
#endif // NET_WIFI_CONSOLE_DEBUG_ENABLE
      break;
    }

    NetWifi::g_AP_Handler = esp_netif_create_default_wifi_ap();
    if (NetWifi::g_AP_Handler == WIFI_DRV_HANDLER_INVALID)
    {
#ifdef NET_WIFI_CONSOLE_DEBUG_ENABLE
      CONSOLE_LOG_BUF(netWifiLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[WFg] init %i", -4);
#endif // NET_WIFI_CONSOLE_DEBUG_ENABLE
      break;
    }

    NetWifi::g_STA_Handler = esp_netif_create_default_wifi_sta();
    if (NetWifi::g_STA_Handler == WIFI_DRV_HANDLER_INVALID)
    {
#ifdef NET_WIFI_CONSOLE_DEBUG_ENABLE
      CONSOLE_LOG_BUF(netWifiLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[WFg] init %i", -5);
#endif // NET_WIFI_CONSOLE_DEBUG_ENABLE
      break;
    }

    wifi_init_config_t wifiInit = wifi_init_config_t();
    WIFI_MAKE_CONFIG_DEFAULT(wifiInit);
    WifiRet_T wifiRet = esp_wifi_init(&wifiInit);
    if (wifiRet != WIFI_DRV_RET_OK)
    {
#ifdef NET_WIFI_CONSOLE_DEBUG_ENABLE
      CONSOLE_LOG_BUF(netWifiLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[WFg] init %i", -6);
#endif // NET_WIFI_CONSOLE_DEBUG_ENABLE
      break;
    }
#ifdef NET_WIFI_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(netWifiLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[WFg] init %i", 99);
#endif // NET_WIFI_CONSOLE_DEBUG_ENABLE
    return 0;
  } while (0);
#ifdef NET_WIFI_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(netWifiLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[WFg] init %i", -99);
#endif // NET_WIFI_CONSOLE_DEBUG_ENABLE
  NetWifi::globalDeinit();
  return -1;
}

int NetWifi::globalDeinit(void)
{
  do
  {
    NetWifi::g_Instance_Count--;
    if (NetWifi::g_Instance_Count > 0)
    {
      break;
    }
    // last instance
    esp_wifi_deinit();

    if (NetWifi::g_AP_Handler != WIFI_DRV_HANDLER_INVALID)
    {
      esp_netif_destroy_default_wifi(NetWifi::g_AP_Handler);
    }

    if (NetWifi::g_STA_Handler != WIFI_DRV_HANDLER_INVALID)
    {
      esp_netif_destroy_default_wifi(NetWifi::g_STA_Handler);
    }
    esp_event_loop_delete_default();
    esp_netif_deinit();
    nvs_flash_deinit();
    return 0;
  } while (0);
  return -1;
}


// end static 

NetWifi::NetWifi(void)
  : TaskManager()
  , NetInterface(NetInterfaceTypeUN::NetInterfaceWifi)
  , working_Mode(WIFI_MGR_MODE_INVALID)
  , ret_Start(-1)
  , ret_Stop(-1)
  , wifi_Event_Handler(NULL)
  , ip_Event_Handler(NULL)
  , sta_Conn_Retry_Count(0)
  , sta_Conn_Retry_Max(0)
{
  memset(&this->conn_Config, 0, sizeof(WifiConnectionConfigTAG));
}

NetWifi::~NetWifi(void)
{
  this->stopTask();
}

//int NetWifi::inititialize(void)
//{
//  do
//  {
//    int result = NetWifi::globalInit();
//    if (result != 0)
//    {
//      break;
//    }
//    
//    return result;
//  } while (0);
//  this->cleanUp();
//  return -1;
//}

bool NetWifi::isStarted(void)
{
  do
  {
    if (this->working_Mode == WIFI_MGR_MODE_INVALID)
    {
      break;
    }
    return true;
  } while (0);
  return false;
}

bool NetWifi::isEnabled(void)
{
  return this->isStarted();
}

WifiMode_t NetWifi::mode(void)
{
  return working_Mode;
}
int NetWifi::getNetConfig(NetIPConfigTAG& netConfig)
{
  netConfig = NetIPConfigTAG();
  do
  {
    if (this->isStarted() == false)
    {
      break;
    }

    tcpip_adapter_ip_info_t ipInfo;
    // IP address.
    WifiRet_T ret = WIFI_DRV_RET_OK;
    if (this->working_Mode == WIFI_MODE_STA)
    {
      ret = tcpip_adapter_get_ip_info(TCPIP_ADAPTER_IF_STA, &ipInfo);
    }
    else
    {
      ret = tcpip_adapter_get_ip_info(TCPIP_ADAPTER_IF_AP, &ipInfo);
    }
    if (ret != WIFI_DRV_RET_OK)
    {
      break;
    }

    netConfig.IPV4 = ipInfo.ip.addr;
    netConfig.defaultGW = ipInfo.gw.addr;
    netConfig.subnetMask = ipInfo.netmask.addr;
    return 0;
  } while (0);
  netConfig = NetIPConfigTAG();
  return -1;
}

int NetWifi::startTask(NetTaskConfigTAG& netTaskConfig)
{
  do
  {
    int result = 0;
    //if (NetWifi::g_Instance_Count <= 0)
    //{
    //  break; //not initilized yet
    //}

    if (this->isTaskRunning() != false)
    {
      return -1; // already running
    }

    // Set up tasks
    {
      //TaskManagerConfigTAG* taskConfig = (TaskManagerConfigTAG*)wifiTaskConfig.taskManagerConfig;
      // self
      this->registerHanldingEventStructSize(sizeof(EventWifiStartTAG));
      this->registerHanldingEventStructSize(sizeof(EventWifiStopTAG));
      this->registerHanldingEventStructSize(sizeof(EventWifiStartedTAG));
      this->registerHanldingEventStructSize(sizeof(EventWifiConnectionOpenedTAG));
      this->registerHanldingEventStructSize(sizeof(EventWifiConnectionClosedTAG));
      this->registerHanldingEventStructSize(sizeof(EventWifiNetGotIPV4TAG));
      
      result = TaskManager::setConfig(netTaskConfig.taskManagerConfig);
      if (result != 0)
      {
        break;
      }

      result = TaskManager::prepare();
      if (result != 0)
      {
        break;
      }
    }

    {
      //TaskThreadConfigTAG* threadConfig = (TaskThreadConfigTAG*)wifiTaskConfig.taskThreadConfig;
      result = TaskManager::startProcess(netTaskConfig.taskThreadConfig, true);
      if (result != 0)
      {
        break;
      }
    }

    return result;
  } while (0);

  this->stopTask();
  return -1;
}


int NetWifi::startNet(NetConnectionConfigTAG& netConnConfig, bool waitCompletion)
{
  do
  {
    if (this->isStarted() != false)
    {
      this->stopNet();
    }
#ifdef NET_WIFI_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(netWifiLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[NetWF] sta %i", 0);
#endif // NET_WIFI_CONSOLE_DEBUG_ENABLE
    {
      SystemMutexLocker locker(this->mutex_Operating);
      int ret = 0;
      memcpy(&this->conn_Config, netConnConfig.config.wifi, sizeof(WifiConnectionConfigTAG));
      EventWifiStartTAG eventStartData = EventWifiStartTAG();
      memcpy(&eventStartData.apConfig, &netConnConfig.config.wifi->apConfig, sizeof(WifiConnectionAPConfigTAG));
      memcpy(&eventStartData.staConfig, &netConnConfig.config.wifi->staConfig, sizeof(WifiConnectionSTAConfigTAG));
      this->ret_Start = -1;
      this->mutex_wait_Start.unlock();
      this->mutex_wait_Start.lock();
      ret = this->notify((EventId_t)EventManagerConstant::EventMessageId::WifiStart, sizeof(EventWifiStartTAG), (unsigned char*)&eventStartData);
      if (ret != 0)
      {
#ifdef NET_WIFI_CONSOLE_DEBUG_ENABLE
        CONSOLE_LOG_BUF(netWifiLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[NetWF] sta %i %i", -1, ret);
#endif // NET_WIFI_CONSOLE_DEBUG_ENABLE
        break;
      }

#ifdef NET_WIFI_CONSOLE_DEBUG_ENABLE
      CONSOLE_LOG_BUF(netWifiLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[NetWF] sta %i %i", 2, ret);
#endif // NET_WIFI_CONSOLE_DEBUG_ENABLE
      if (waitCompletion == false)
      {
        return 0;
      }
#ifdef NET_WIFI_CONSOLE_DEBUG_ENABLE
      CONSOLE_LOG_BUF(netWifiLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[NetWF] sta %i %i", 3, this->ret_Start);
#endif // NET_WIFI_CONSOLE_DEBUG_ENABLE
      this->mutex_wait_Start.lock(WIFI_MGR_START_WAIT_TIME);
#ifdef NET_WIFI_CONSOLE_DEBUG_ENABLE
      CONSOLE_LOG_BUF(netWifiLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[NetWF] sta %i %i", 4, this->ret_Start);
#endif // NET_WIFI_CONSOLE_DEBUG_ENABLE
      if (this->ret_Start != 0)
      {
#ifdef NET_WIFI_CONSOLE_DEBUG_ENABLE
        CONSOLE_LOG_BUF(netWifiLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[NetWF] sta %i %i", 5, this->ret_Start);
#endif // NET_WIFI_CONSOLE_DEBUG_ENABLE
        break;
      }
#ifdef NET_WIFI_CONSOLE_DEBUG_ENABLE
      CONSOLE_LOG_BUF(netWifiLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[NetWF] sta %i %i", 99, this->ret_Start);
#endif // NET_WIFI_CONSOLE_DEBUG_ENABLE
    }

    return 0;
  } while (0);
  this->stopNet();
  return -1;

}

void NetWifi::stopNet(bool waitCompletion)
{
  do
  {
    if (this->isTaskRunning() == false)
    {
      return; // already
    }

    if (this->isStarted() == false)
    {
      return; // already
    }

    {
      SystemMutexLocker locker(this->mutex_Operating);
      int ret = 0;
      EventWifiStopTAG eventStopData = EventWifiStopTAG();
      this->mutex_wait_Stop.unlock();
      this->mutex_wait_Stop.lock();
      ret = this->notify((EventId_t)EventManagerConstant::EventMessageId::WifiStop, sizeof(EventWifiStopTAG), (unsigned char*)&eventStopData);
      if (ret != 0)
      {
        break;
      }

      if (waitCompletion == false)
      {
        this->working_Mode = WIFI_MGR_MODE_INVALID;
        return;
      }
      this->mutex_wait_Stop.lock(WIFI_MGR_STOP_WAIT_TIME);
      this->working_Mode = WIFI_MGR_MODE_INVALID;
      memset(&this->conn_Config, 0, sizeof(WifiConnectionConfigTAG));
    }

    return;
  } while (0);
  this->working_Mode = WIFI_MGR_MODE_INVALID;
  return;
}

void NetWifi::stopTask(void)
{
  do
  {
    this->stopNet();
    TaskManager::stopProcess();
    return;
  } while (0);
  return;
}


void NetWifi::clear(void)
{
  //this->stopTask();
  NetWifi::globalDeinit();
}

int NetWifi::prepare(void)
{
  do
  {
    int result = NetWifi::globalInit();
    if (result != 0)
    {
      break;
    }
    return 0;
  } while (0);
  return -1;
}

void NetWifi::proc(void)
{
#ifdef NET_WIFI_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG("[WFTsk] proc %i", 0);
#endif // NET_WIFI_CONSOLE_DEBUG_ENABLE
  int prepareRet = this->prepare();
  this->reportPrepareResult(prepareRet);
  if (prepareRet != 0)
  {
    this->waitTerminating();
    this->clear();
    return;
  }
#ifdef NET_WIFI_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG("[WFTsk] proc %i", 1);
#endif // NET_WIFI_CONSOLE_DEBUG_ENABLE
  EventDataItem* eventData = NULL;
  bool isStop = false;
  while (isStop == false) // A Task shall never return or exit.
  {
    eventData = this->event_Manager.wait(1000);
#ifdef NET_WIFI_CONSOLE_DEBUG_ENABLE
    //CONSOLE_LOG("[WFTsk] proc %i", 2);
#endif // NET_WIFI_CONSOLE_DEBUG_ENABLE
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
      case (int)EventManagerConstant::EventMessageId::WifiStart:
      {
        this->onEventWifiStart(eventData->bufferAddress(), eventData->dataLength());
        break;
      }
      case (int)EventManagerConstant::EventMessageId::WifiStop:
      {
        this->onEventWifiStop(eventData->bufferAddress(), eventData->dataLength());
        break;
      }
      case (int)EventManagerConstant::EventMessageId::WifiStarted:
      {
        this->onEventWifiStarted(eventData->bufferAddress(), eventData->dataLength());
        break;
      }
      case (int)EventManagerConstant::EventMessageId::WifiStopped:
      {
        this->onEventWifiStopped(eventData->bufferAddress(), eventData->dataLength());
        break;
      }
      case (int)EventManagerConstant::EventMessageId::WifiGotIPV4:
      {
        this->onEventWifiNetGotIPV4(eventData->bufferAddress(), eventData->dataLength());
        break;
      }
      case (int)EventManagerConstant::EventMessageId::WifiConnectionOpened:
      {
        this->onEventWifiConnectionOpened(eventData->bufferAddress(), eventData->dataLength());
        break;
      }
      case (int)EventManagerConstant::EventMessageId::WifiConnectionClosed:
      {
        this->onEventWifiConnectionClosed(eventData->bufferAddress(), eventData->dataLength());
        break;
      }
      default:
        break;
    }
    this->event_Manager.release(eventData);
  }
  this->clear();
#ifdef NET_WIFI_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG("[WFTsk] stop %i", 99);
#endif // NET_WIFI_CONSOLE_DEBUG_ENABLE
}

int NetWifi::onEventWifiStart(unsigned char* data, unsigned int dataSize)
{
#ifdef NET_WIFI_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(netWifiLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[NetWF] eSta %i", 0);
#endif // NET_WIFI_CONSOLE_DEBUG_ENABLE
  WifiRet_T wifiRet = 0;
  do
  {
    if ((dataSize != sizeof(EventWifiStartTAG))
      || (data == NULL))
    {
      break;
    }

    if (this->isStarted() != false)
    {
      break;
    }

    EventWifiStartTAG* eventData = (EventWifiStartTAG*)data;
    //waitMutex = (SystemMutex*)eventData->mutex;
    byte workingMode = WIFI_MGR_MODE_INVALID;

    if (eventData->staConfig.isValid() != false)
    {
      workingMode = WIFI_MGR_MODE_STA;
    }

    if (eventData->apConfig.isValid() != false)
    {
      workingMode |= WIFI_MGR_MODE_AP;
    }

    if (workingMode == WIFI_MGR_MODE_INVALID)
    {
      break;
    }

    wifiRet = esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &cbWifiEventHandler,  this, this->wifi_Event_Handler);
    if (wifiRet != WIFI_DRV_RET_OK)
    {
#ifdef NET_WIFI_CONSOLE_DEBUG_ENABLE
      CONSOLE_LOG_BUF(netWifiLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[NetWF] eSta %i", -1);
#endif // NET_WIFI_CONSOLE_DEBUG_ENABLE
      break;
    }

    wifi_config_t wifi_config = wifi_config_t();
    if ( (workingMode & WIFI_DRV_MODE_STA) == WIFI_DRV_MODE_STA)
    {
      size_t ssidLen = SYSTEM_MIN(WIFI_DRV_SSID_MAX_LEN, SYSTEM_MAX(1, strlen(eventData->staConfig.ssid)));
      memcpy(wifi_config.sta.ssid, eventData->staConfig.ssid, ssidLen);
      size_t passLen = SYSTEM_MIN(WIFI_DRV_SSID_MAX_LEN, strlen(eventData->staConfig.password));
      if (passLen == 0)
      {
        wifi_config.sta.threshold.authmode = WIFI_DRV_AUTH_MODE_OPEN;
      }
      else
      {
        memcpy(wifi_config.sta.password, eventData->staConfig.password, passLen);
        wifi_config.sta.threshold.authmode = wifiDrvAuthModeConvert(eventData->staConfig.authMode);
      }
      wifi_config.sta.pmf_cfg.capable = WIFI_MGR_PMF_CAPABLE_DEFAULT;
      wifi_config.sta.pmf_cfg.required = WIFI_MGR_PMF_REQUIRED_DEFAULT;
      wifiRet = esp_event_handler_instance_register(IP_EVENT, ESP_EVENT_ANY_ID, &cbWifiEventHandler, this, this->ip_Event_Handler);
      if (wifiRet != WIFI_DRV_RET_OK)
      {
#ifdef NET_WIFI_CONSOLE_DEBUG_ENABLE
        CONSOLE_LOG_BUF(netWifiLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[NetWF] eSta %i", -2);
#endif // NET_WIFI_CONSOLE_DEBUG_ENABLE
        break;
      }

      if (eventData->staConfig.netConfig.useStaticIP() != false)
      {
        // set static ip
        esp_netif_dhcpc_stop(NetWifi::g_STA_Handler);
        esp_netif_ip_info_t ipInfo;
        ipInfo.ip.addr = eventData->staConfig.netConfig.IPV4;
        ipInfo.netmask.addr = eventData->staConfig.netConfig.subnetMask;
        ipInfo.gw.addr = eventData->staConfig.netConfig.defaultGW;
        wifiRet = esp_netif_set_ip_info(NetWifi::g_STA_Handler, &ipInfo);
#ifdef NET_WIFI_CONSOLE_DEBUG_ENABLE
        CONSOLE_LOG_BUF(netWifiLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[NetWF] eSta %i", 2, (int)wifiRet);
#endif // NET_WIFI_CONSOLE_DEBUG_ENABLE
        esp_netif_dhcpc_start(NetWifi::g_STA_Handler);
        if (wifiRet != WIFI_DRV_RET_OK)
        {
          break;
        }
      }

      this->sta_Conn_Retry_Max = eventData->staConfig.connRetryMax;
      this->sta_Conn_Retry_Count = this->sta_Conn_Retry_Max;
    }

    if ((workingMode & WIFI_DRV_MODE_AP) == WIFI_DRV_MODE_AP)
    {
      wifi_config.ap.ssid_len = SYSTEM_MIN(WIFI_DRV_SSID_MAX_LEN, SYSTEM_MAX(1, strlen(eventData->apConfig.ssid)));
      memcpy(wifi_config.ap.ssid, eventData->apConfig.ssid, wifi_config.ap.ssid_len);
      wifi_config.ap.channel = eventData->apConfig.channel;
      size_t passLen = SYSTEM_MIN(WIFI_DRV_SSID_MAX_LEN, strlen(eventData->apConfig.password));
      if (passLen == 0)
      {
        wifi_config.ap.authmode = WIFI_DRV_AUTH_MODE_OPEN;
      }
      else
      {
        memcpy(wifi_config.ap.password, eventData->apConfig.password, passLen);
        wifi_config.ap.authmode = wifiDrvAuthModeConvert(eventData->apConfig.authMode);
      }
      wifi_config.ap.max_connection = eventData->apConfig.maxConn;
      if (eventData->apConfig.netConfig.useStaticIP() != false)
      {
        // set static ip
        esp_netif_dhcpc_stop(NetWifi::g_AP_Handler);
        esp_netif_ip_info_t ipInfo;
        ipInfo.ip.addr = eventData->apConfig.netConfig.IPV4;
        ipInfo.netmask.addr = eventData->apConfig.netConfig.subnetMask;
        ipInfo.gw.addr = eventData->apConfig.netConfig.defaultGW;
        wifiRet = esp_netif_set_ip_info(NetWifi::g_AP_Handler, &ipInfo);
#ifdef NET_WIFI_CONSOLE_DEBUG_ENABLE
        CONSOLE_LOG_BUF(netWifiLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[NetWF] eSta %i", 3, (int)wifiRet);
#endif // NET_WIFI_CONSOLE_DEBUG_ENABLE
        esp_netif_dhcpc_start(NetWifi::g_AP_Handler);
        if (wifiRet != WIFI_DRV_RET_OK)
        {
          break;
        }
      }
    }

    switch (workingMode)
    {
      case WIFI_MGR_MODE_STA:
        wifiRet = esp_wifi_set_mode(WIFI_DRV_MODE_STA);
        break;
      case WIFI_MGR_MODE_AP:
        wifiRet = esp_wifi_set_mode(WIFI_DRV_MODE_AP);
        break;
      case WIFI_MGR_MODE_AP_STA:
        wifiRet = esp_wifi_set_mode(WIFI_DRV_MODE_APSTA);
      default:
        wifiRet = WIFI_DRV_RET_OK + 1;
        break;
    }

#ifdef NET_WIFI_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(netWifiLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[NetWF] eSta %i %i", 4, workingMode);
#endif // NET_WIFI_CONSOLE_DEBUG_ENABLE
    if (wifiRet != WIFI_DRV_RET_OK)
    {
      break;
    }

    switch (workingMode)
    {
    case WIFI_MGR_MODE_STA:
      wifiRet = esp_wifi_set_config(WIFI_IF_STA, &wifi_config);
      break;
    case WIFI_MGR_MODE_AP:
      wifiRet = esp_wifi_set_config(WIFI_IF_AP, &wifi_config);
      break;
    case WIFI_MGR_MODE_AP_STA:
      wifiRet = esp_wifi_set_config(WIFI_IF_STA, &wifi_config);
      wifiRet = esp_wifi_set_config(WIFI_IF_AP, &wifi_config);
    default:
      wifiRet = WIFI_DRV_RET_OK + 1;
      break;
    }
    
    if (wifiRet != WIFI_DRV_RET_OK)
    {
#ifdef NET_WIFI_CONSOLE_DEBUG_ENABLE
      wifi_mode_t mode = WIFI_MODE_MAX;
      esp_wifi_get_mode(&mode);
      CONSOLE_LOG_BUF(netWifiLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[NetWF] eSta %i %i", -4, (int)wifiRet);
      CONSOLE_LOG_BUF(netWifiLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[NetWF] eSta %i %i %i", -4, (int)mode, workingMode);
#endif // NET_WIFI_CONSOLE_DEBUG_ENABLE
      break;
    }
#ifdef NET_WIFI_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(netWifiLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[ssid] %s", wifi_config.sta.ssid);
    CONSOLE_LOG_BUF(netWifiLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[pwd] %s", wifi_config.sta.password);
#endif // NET_WIFI_CONSOLE_DEBUG_ENABLE




    wifiRet = esp_wifi_start();
    if (wifiRet != WIFI_DRV_RET_OK)
    {
#ifdef NET_WIFI_CONSOLE_DEBUG_ENABLE
      CONSOLE_LOG_BUF(netWifiLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[NetWF] eSta %i", -5);
#endif // NET_WIFI_CONSOLE_DEBUG_ENABLE
      break;
    }

    this->working_Mode = workingMode;
   /* if (waitMutex != NULL)
    {
      waitMutex->unlock();
    }*/
    return 0;
  } while (0);

  //if (waitMutex != NULL)
  {
    this->ret_Start = -1;
    this->mutex_wait_Start.unlock();
  }
  return -1;
}

int NetWifi::onEventWifiStop(unsigned char* data, unsigned int dataSize)
{
#ifdef NET_WIFI_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(netWifiLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[NetWF] sto %i", 0);
#endif // NET_WIFI_CONSOLE_DEBUG_ENABLE
  //SystemMutex* waitMutex = NULL;
  do
  {
    if ((dataSize != sizeof(EventWifiStopTAG))
      || (data == NULL))
    {
      break;
    }

    if (this->isStarted() == false)
    {
      break;
    }

    EventWifiStopTAG* eventData = (EventWifiStopTAG*)data;
   // waitMutex = (SystemMutex*)eventData->mutex;
    esp_wifi_stop();
    if (this->wifi_Event_Handler != NULL)
    {
      esp_event_handler_instance_unregister(WIFI_EVENT, ESP_EVENT_ANY_ID, this->wifi_Event_Handler);
    }
    
    if (this->ip_Event_Handler!= NULL)
    {
      esp_event_handler_instance_unregister(IP_EVENT, ESP_EVENT_ANY_ID, this->ip_Event_Handler);
    }
    this->wifi_Event_Handler = NULL;
    this->ip_Event_Handler = NULL;
    this->sta_Conn_Retry_Count = 0;
    this->sta_Conn_Retry_Max = 0;
    /*if (waitMutex != NULL)
    {
      waitMutex->unlock();
    }*/
    return 0;
  } while (0);

  //if (waitMutex != NULL)
  {
    //waitMutex->unlock();
    this->ret_Stop = -1;
    this->mutex_wait_Stop.unlock();
  }
  return -1;
}

int NetWifi::onEventWifiStarted(unsigned char* data, unsigned int dataSize)
{
#ifdef NET_WIFI_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(netWifiLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[NetWF] staed %i", 0);
#endif // NET_WIFI_CONSOLE_DEBUG_ENABLE
  do
  {
    if ((dataSize != sizeof(EventWifiStartedTAG))
      || (data == NULL))
    {
      break;
    }

    if (this->isStarted() == false)
    {
#ifdef NET_WIFI_CONSOLE_DEBUG_ENABLE
      CONSOLE_LOG_BUF(netWifiLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[NetWF] staed %i", -1);
#endif // NET_WIFI_CONSOLE_DEBUG_ENABLE
      break;
    }

    EventWifiStartedTAG* eventData = (EventWifiStartedTAG*)data;

    if (eventData->isAP != 0)
    {
      this->ret_Start = 0;
      this->mutex_wait_Start.unlock(); // APSTA mode?@@
      // @@ notify to coordinator
      return 0;
    }

#ifdef NET_WIFI_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(netWifiLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[NetWF] staed %i", 3);
#endif // NET_WIFI_CONSOLE_DEBUG_ENABLE
    // STA
    this->sta_Conn_Retry_Count = this->sta_Conn_Retry_Max;
    
    WifiRet_T wifiRet = esp_wifi_connect();
    if (wifiRet != WIFI_DRV_RET_OK)
    {
#ifdef NET_WIFI_CONSOLE_DEBUG_ENABLE
      CONSOLE_LOG_BUF(netWifiLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[NetWF] eSta %i %i", -5, wifiRet);
#endif // NET_WIFI_CONSOLE_DEBUG_ENABLE
      break;
    }

    return 0;
  } while (0);
  return -1;
}


int NetWifi::onEventWifiStopped(unsigned char* data, unsigned int dataSize)
{
#ifdef NET_WIFI_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(netWifiLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[NetWF] stoped %i", 0);
#endif // NET_WIFI_CONSOLE_DEBUG_ENABLE
  do
  {
    if ((dataSize != sizeof(EventWifiStoppedTAG))
      || (data == NULL))
    {
      break;
    }

    if (this->isStarted() == false)
    {
      break;
    }

    EventWifiStoppedTAG* eventData = (EventWifiStoppedTAG*)data;
    this->ret_Stop = 0;
    this->mutex_wait_Stop.unlock(); // APSTA mode?@@
    // @@ notify to coordinator
    return 0;
  } while (0);
  return -1;
}

int NetWifi::onEventWifiConnectionOpened(unsigned char* data, unsigned int dataSize)
{
#ifdef NET_WIFI_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(netWifiLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[NetWF] opned %i", 0);
#endif // NET_WIFI_CONSOLE_DEBUG_ENABLE
  do
  {
    if ((dataSize != sizeof(EventWifiConnectionOpenedTAG))
      || (data == NULL))
    {
      break;
    }

    if (this->isStarted() == false)
    {
      break;
    }

    EventWifiConnectionOpenedTAG* eventData = (EventWifiConnectionOpenedTAG*)data;
    //@@ notify coordinator
#ifdef NET_WIFI_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(netWifiLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[NetWF] opned %i %u", 1, eventData->connId);
#endif // NET_WIFI_CONSOLE_DEBUG_ENABLE
    return 0;
  } while (0);
  return -1;
}

int NetWifi::onEventWifiConnectionClosed(unsigned char* data, unsigned int dataSize)
{

  do
  {
    if ((dataSize != sizeof(EventWifiConnectionClosedTAG))
      || (data == NULL))
    {
      break;
    }

    if (this->isStarted() == false)
    {
      break;
    }

    EventWifiConnectionClosedTAG* eventData = (EventWifiConnectionClosedTAG*)data;
#ifdef NET_WIFI_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(netWifiLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[NetWF] closed %i %i %i", 0, eventData->connId, eventData->disReason);
#endif // NET_WIFI_CONSOLE_DEBUG_ENABLE
    if (eventData->isAP != 0)
    {
      //
      // @@ notify to coordinator
      return 0;
    }

    // STA
    if (this->sta_Conn_Retry_Count <= 0)
    {
      this->ret_Start = -1;
      this->mutex_wait_Start.unlock();
#ifdef NET_WIFI_CONSOLE_DEBUG_ENABLE
      CONSOLE_LOG_BUF(netWifiLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[NetWF] closed %i %i %i", -1, eventData->connId, this->ret_Start);
#endif // NET_WIFI_CONSOLE_DEBUG_ENABLE
      //notify coordinator
      break;
    }

    this->sta_Conn_Retry_Count--;
    esp_wifi_connect();

    return 0;
  } while (0);
  return -1;
}

int NetWifi::onEventWifiNetGotIPV4(unsigned char* data, unsigned int dataSize)
{
#ifdef NET_WIFI_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(netWifiLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[NetWF] ip4 %i", 0);
#endif // NET_WIFI_CONSOLE_DEBUG_ENABLE
  do
  {
    if (this->isStarted() == false)
    {
      break;
    }

    EventWifiNetGotIPV4TAG* eventData = (EventWifiNetGotIPV4TAG*)data;
    if (eventData->isAP != 0)
    {
      break; // this is for STA only
    }

#ifdef NET_WIFI_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(netWifiLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[NetWF] ip4 %i %u", 1, eventData->ipv4);
#endif // NET_WIFI_CONSOLE_DEBUG_ENABLE
    // @@ notify coordinator IP
    this->sta_Conn_Retry_Count = this->sta_Conn_Retry_Max;
    this->ret_Start = 0;
    this->mutex_wait_Start.unlock();

    return 0;
  } while (0);
  return -1;
}

static void cbWifiEventHandler(void* arg, esp_event_base_t eventBase, int32_t eventId, void* eventData)
{
  do
  {
    if (arg == NULL)
    {
      break;
    }

    NetWifi* handler = (NetWifi*)arg;
    switch (eventId)
    {
      // AP
    case WIFI_EVENT_AP_START:
    {
      EventWifiStartedTAG eventData = EventWifiStartedTAG();
      eventData.isAP = 1;
      handler->notify((EventId_t)EventManagerConstant::EventMessageId::WifiStarted, sizeof(EventWifiStartedTAG), (unsigned char*)&eventData);
      break;
    }
    case WIFI_EVENT_AP_STOP:
    {
      EventWifiStoppedTAG eventData = EventWifiStoppedTAG();
      eventData.isAP = 1;
      handler->notify((EventId_t)EventManagerConstant::EventMessageId::WifiStopped, sizeof(EventWifiStoppedTAG), (unsigned char*)&eventData);
      break;
    }
    case WIFI_EVENT_AP_STACONNECTED:
    {
      wifi_event_ap_staconnected_t* wifiEvent = (wifi_event_ap_staconnected_t*)eventData;
      EventWifiConnectionOpenedTAG eventData = EventWifiConnectionOpenedTAG();
      eventData.isAP = 1;
      eventData.connId = wifiEvent->aid;
      handler->notify((EventId_t)EventManagerConstant::EventMessageId::WifiConnectionOpened, sizeof(EventWifiConnectionOpenedTAG), (unsigned char*)&eventData);
      break;
    }
    case WIFI_EVENT_AP_STADISCONNECTED:
    {
      wifi_event_ap_stadisconnected_t* wifiEvent = (wifi_event_ap_stadisconnected_t*)eventData;
      EventWifiConnectionClosedTAG eventData = EventWifiConnectionClosedTAG();
      eventData.isAP = 1;
      eventData.connId = wifiEvent->aid;
      handler->notify((EventId_t)EventManagerConstant::EventMessageId::WifiConnectionClosed, sizeof(EventWifiConnectionClosedTAG), (unsigned char*)&eventData);
      break;
    }

    // STA
    case WIFI_EVENT_STA_START:
    {
      EventWifiStartedTAG eventData = EventWifiStartedTAG();
      eventData.isAP = 0;
      handler->notify((EventId_t)EventManagerConstant::EventMessageId::WifiStarted, sizeof(EventWifiStartedTAG), (unsigned char*)&eventData);
      break;
    }
    case WIFI_EVENT_STA_STOP:
    {
      EventWifiStoppedTAG eventData = EventWifiStoppedTAG();
      eventData.isAP = 0;
      handler->notify((EventId_t)EventManagerConstant::EventMessageId::WifiStopped, sizeof(EventWifiStoppedTAG), (unsigned char*)&eventData);
      break;
    }
    case WIFI_EVENT_STA_CONNECTED:
    {
      wifi_event_sta_connected_t* wifiEvent = (wifi_event_sta_connected_t*)eventData;
      EventWifiConnectionOpenedTAG eventData = EventWifiConnectionOpenedTAG();
      eventData.isAP = 0;
      eventData.connId = 0;
      handler->notify((EventId_t)EventManagerConstant::EventMessageId::WifiConnectionOpened, sizeof(EventWifiConnectionOpenedTAG), (unsigned char*)&eventData);
      break;
    }
    case WIFI_EVENT_STA_DISCONNECTED:
    {
      wifi_event_sta_disconnected_t* wifiEvent = (wifi_event_sta_disconnected_t*)eventData;
      EventWifiConnectionClosedTAG eventData = EventWifiConnectionClosedTAG();
      eventData.isAP = 0;
      eventData.connId = 0;
      eventData.disReason = wifiEvent->reason;
      handler->notify((EventId_t)EventManagerConstant::EventMessageId::WifiConnectionClosed, sizeof(EventWifiConnectionClosedTAG), (unsigned char*)&eventData);
      break;
    }
    case IP_EVENT_STA_GOT_IP:
    {
      ip_event_got_ip_t* wifiEvent = (ip_event_got_ip_t*)eventData;
      EventWifiNetGotIPV4TAG eventData = EventWifiNetGotIPV4TAG();
      eventData.isAP = 0;
      eventData.ipv4 = wifiEvent->ip_info.ip.addr;
      handler->notify((EventId_t)EventManagerConstant::EventMessageId::WifiGotIPV4, sizeof(EventWifiNetGotIPV4TAG), (unsigned char*)&eventData);
      break;
    }
    default:
      break;
    }

    return;
  } while (0);
  return;

}
#endif // _CONF_NET_WIFI_ENABLED