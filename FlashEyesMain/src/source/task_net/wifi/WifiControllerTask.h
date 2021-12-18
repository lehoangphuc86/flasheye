#ifndef _WIFI_CONTROLLER_H
#define _WIFI_CONTROLLER_H
/////////////////////////////////////////////////
// INCLUDE
#include "WifiManagerConstant.h"
#if (_CONF_WIFI_CONTROLLER_TASK_ENABLED)

#include "task_manager/TaskManager.h"
#include "os_system/SystemCriticalSession.h"
/////////////////////////////////////////////////
// PREPROCESSOR
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

/////////////////////////////////////////////////
// STATIC FUNCTIONS

/////////////////////////////////////////////////
// EXTERN

/////////////////////////////////////////////////
// CLASS IMPLEMENTAION

/*WifiControllerTask*/
class WifiControllerTask
  : public TaskManager
{
public:
  WifiControllerTask(void);
  virtual ~WifiControllerTask(void);
  int                                                           inititialize(void);  
  bool                                                          isStarted(void);
  WifiMode_t                                                    mode(void);
  int                                                           getNetConfig(NetIPConfigTAG& netConfig);
  int                                                           startTask(WifiTaskConfigTAG& wifiTaskConfig);
  int                                                           startWifi(WifiConnectionConfigTAG& wifiConnConfig, bool waitCompletion = true);
  void                                                          stopWifi(bool waitCompletion = true);
  void                                                          stopTask(void);
  void                                                          cleanUp(void);
protected:
  int                                                           prepare(void);
  void                                                          proc(void) override;
  int                                                           onEventWifiStart(unsigned char* data, unsigned int dataSize);
  int                                                           onEventWifiStop(unsigned char* data, unsigned int dataSize);
  int                                                           onEventWifiStarted(unsigned char* data, unsigned int dataSize);
  int                                                           onEventWifiStopped(unsigned char* data, unsigned int dataSize);
  int                                                           onEventWifiConnectionOpened(unsigned char* data, unsigned int dataSize);
  int                                                           onEventWifiConnectionClosed(unsigned char* data, unsigned int dataSize);
  int                                                           onEventWifiNetGotIPV4(unsigned char* data, unsigned int dataSize);

protected:
  WifiMode_t                                                    working_Mode;
  WifiConnectionConfigTAG                                       conn_Config;
  SystemMutex                                                   mutex_Operating;
  SystemMutex                                                   mutex_wait_Start;
  SystemMutex                                                   mutex_wait_Stop;
  int                                                           ret_Start;
  int                                                           ret_Stop;
  WifiEventHandlerInsance_t                                     wifi_Event_Handler;
  WifiEventHandlerInsance_t                                     ip_Event_Handler;
  byte                                                          sta_Conn_Retry_Max;
  byte                                                          sta_Conn_Retry_Count;
private:
  static int                                                    globalInit(void);
  static int                                                    globalDeinit(void);
private:
  static byte                                                   g_Instance_Count;
  static WifiNetHandler_t                                       g_AP_Handler;
  static WifiNetHandler_t                                       g_STA_Handler;
};
#endif // (_CONF_TIMER_CONTROL_TASK_ENABLED)

#endif // _WIFI_CONTROLLER_H