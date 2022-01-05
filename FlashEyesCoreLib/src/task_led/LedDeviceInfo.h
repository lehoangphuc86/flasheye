#include "LedTaskConstant.h"

#ifdef _CONF_LED_DEVICE_INFO_ENABLED

#ifndef _LED_DEVICE_INFO_H
#define _LED_DEVICE_INFO_H
/////////////////////////////////////////////////
// INCLUDE
#include "os_system/SystemMutex.h"
/////////////////////////////////////////////////
// PREPROCESSOR
/////////////////////////////////////////////////
// DEFINE

/////////////////////////////////////////////////
// MARCO

/////////////////////////////////////////////////
// GLOBAL VARIABLES

/////////////////////////////////////////////////
// GLOBAL FUNCTIONS

/////////////////////////////////////////////////
// DATA TYPE (TYPEDEF)

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
// CLASS DEFINITION
class LedController;
/*LedDeviceInfo*/
class LedDeviceInfo
{
  friend LedController;
public:
  LedDeviceInfo(void);
  __ATTRIBUTE_VIRTUAL_OPTIMIZED ~LedDeviceInfo(void);
  bool                                                          isValid(void);
  bool                                                          isOn(void);
  LedChannelId_t                                                channelId(void);
  int                                                           initialize(LedDeviceConfigTAG& deviceConfig);
  void                                                          clear(void);
  void                                                          dump(void);
protected:
  LedChannelId_t                                                channel_Id;
  byte                                                          led_Pin;
  byte                                                          cur_Intensity;
  byte                                                          default_Intensity;
  TimerId_t                                                     blink_Timer_Id; ///blink
  bool                                                          is_On;
  bool                                                          is_PWM;
  LedController*                                                dev_Controller; // LedController
  //SystemMutex                                                   mutex_Key;
};


#endif // _LED_DEVICE_INFO_H

#endif // _CONF_LED_DEVICE_INFO_ENABLED