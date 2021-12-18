#ifndef _WIFI_CONTROLLER_CONSTANT_H
#define _WIFI_CONTROLLER_CONSTANT_H

#include "../../../FlashEyesMain.h"
#if (_CONF_WIFI_MANAGER_CONSTANT_ENABLED)
/////////////////////////////////////////////////
// INCLUDE
#include "../../../drivers/esp32/net/wifi/wifi.h"
#include "../NetManagerConstant.h"
#include "task_manager/TaskManagerConstant.h"
/////////////////////////////////////////////////
// PREPROCESSOR

/////////////////////////////////////////////////
// DEFINE
#define WIFI_MGR_MODE_INVALID                       0x00
#define WIFI_MGR_MODE_STA                           0x01
#define WIFI_MGR_MODE_AP                            0x02
#define WIFI_MGR_MODE_AP_STA                        (WIFI_MGR_MODE_STA | WIFI_MGR_MODE_AP)
#define WIFI_MGR_CHANNEL_DEFAULT                    1
#define WIFI_MGR_MAX_CONN_DEFAULT                   4
#define WIFI_MGR_CONN_RETRY_MAX_DEFAULT             2
#define WIFI_MGR_AUTH_MODE_DEFAULT                  WIFI_DRV_AUTH_MODE_WPA2_PSK
#define WIFI_MGR_PMF_CAPABLE_DEFAULT                true
#define WIFI_MGR_PMF_REQUIRED_DEFAULT               false

#define WIFI_MGR_START_WAIT_TIME                    10000 // 10 sec
#define WIFI_MGR_STOP_WAIT_TIME                     WIFI_MGR_START_WAIT_TIME
/////////////////////////////////////////////////
// MARCO

/////////////////////////////////////////////////
// DATA TYPE (TYPEDEF)

/////////////////////////////////////////////////
// DATA TYPE (ENUM)

/////////////////////////////////////////////////
// DATA TYPE (STRUCT)
typedef struct _wifiConnectionAPConfigTAG
{
  char* ssid;
  char* password;
  uint8_t channel;
  uint8_t maxConn;
  uint8_t authMode;
  NetIPConfigTAG netConfig;
  _wifiConnectionAPConfigTAG()
  {
    this->ssid = NULL;
    this->password = NULL;
    this->channel = WIFI_MGR_CHANNEL_DEFAULT;
    this->maxConn = WIFI_MGR_MAX_CONN_DEFAULT;
    this->authMode = WIFI_MGR_AUTH_MODE_DEFAULT;
  }

  bool isValid(void)
  {
    do
    {
      if ((this->ssid == NULL)
        || (strlen(this->ssid) <= 0)
        )
      {
        break;
      }

      return true;
    } while (0);
    return false;
  }
} WifiConnectionAPConfigTAG;

typedef struct _wifiConnectionSTAConfigTAG
{
  char* ssid;
  char* password;
  uint8_t authMode;
  byte connRetryMax;
  NetIPConfigTAG netConfig;
  _wifiConnectionSTAConfigTAG()
  {
    this->ssid = NULL;
    this->password = NULL;
    this->authMode = WIFI_MGR_AUTH_MODE_DEFAULT;
    this->connRetryMax = WIFI_MGR_CONN_RETRY_MAX_DEFAULT;
  }

  bool isValid(void)
  {
    do
    {
      if ((this->ssid == NULL)
        || (strlen(this->ssid) <= 0)
        )
      {
        break;
      }

      if (this->connRetryMax < 1)
      {
        break;
      }

      return true;
    } while (0);
    return false;
  }
} WifiConnectionSTAConfigTAG;

typedef struct _wifiConnectionConfigTAG
{
  WifiConnectionAPConfigTAG apConfig;
  WifiConnectionSTAConfigTAG staConfig;
} WifiConnectionConfigTAG;

typedef struct _wifiTaskConfigTAG
{
  TaskManagerConfigTAG taskManagerConfig;
  TaskThreadConfigTAG taskThreadConfig;
} WifiTaskConfigTAG;

// event
typedef struct _eventWifiStartTAG
{
  WifiConnectionAPConfigTAG apConfig;
  WifiConnectionSTAConfigTAG staConfig;
  //void* mutex;
} EventWifiStartTAG;

typedef struct _eventWifiStopTAG
{
  //void* mutex;  
  byte reserved;
} EventWifiStopTAG;

typedef struct _eventWifiConnectionOpenedTAG
{
  // AP: when STA with connId connected
  // STA: when connected to AP. connnId is always 0
  byte isAP : 1; // 0: this is STA, 1 this is AP
  byte reserved : 7;
  WifiConnId_t connId; 
} EventWifiConnectionOpenedTAG;

typedef struct _eventWifiConnectionClosedTAG
{
  // AP: when STA with connId disconnected
  // STA: when AP disconnected. connnId is always 0
  byte isAP : 1; // 0: this is STA, 1 this is AP
  byte reserved : 7;
  WifiConnId_t connId;
  uint8_t disReason;
} EventWifiConnectionClosedTAG;

typedef struct _eventWifiStartedTAG
{
  // AP: when STA started
  // STA: when AP started
  byte isAP : 1; // 0: this is STA, 1 this is AP
  byte reserved : 7;
} EventWifiStartedTAG;

typedef struct _eventWifiStoppedTAG
{
  // AP: when STA stopped
  // STA: when AP stopped
  byte isAP : 1; // 0: this is STA, 1 this is AP
  byte reserved : 7;
} EventWifiStoppedTAG;

typedef struct _eventWifiNetGotIPV4TAG
{
  // AP: when STA got IP assigned
  // STA: when AP started
  byte isAP : 1; // 0: this is STA, 1 this is AP
  byte reserved : 7;
  uint32_t ipv4;
} EventWifiNetGotIPV4TAG;

/////////////////////////////////////////////////
// GLOBAL VARIABLES

/////////////////////////////////////////////////
// GLOBAL FUNCTIONS

/////////////////////////////////////////////////
// STATIC DATA

/////////////////////////////////////////////////
// STATIC FUNCTIONS

/////////////////////////////////////////////////
// EXTERN

/////////////////////////////////////////////////
// CLASS DEFINITION

#endif // _CONF_WIFI_MANAGER_CONSTANT_ENABLED

#endif // _WIFI_CONTROLLER_CONSTANT_H