#include "LedTaskConstant.h"

#if (_CONF_LED_CONTROLLER_ENABLED)

#ifndef _LED_CONTROLLER_H
#define _LED_CONTROLLER_H

/////////////////////////////////////////////////
// INCLUDE
#include "LedDeviceManager.h"
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

/*LedController*/

class LedController
{
public:
  LedController(void);
  virtual ~LedController(void);
  bool                                                          isValid(void);
  int                                                           start(LedControllerConfigTAG& controllerConfig);
  void                                                          stop(void);
  int                                                           control(LedControlParamTAG& controlParam);
  int                                                           turnOnAll(byte intensity, LedBlinkTime_t blinkTime = 0);
  int                                                           turnOffAll(void);
  int                                                           turnOnMul(LedChannelIdSet_t channelIdSet, byte intensity, LedBlinkTime_t blinkTime = 0);
  int                                                           turnOffMul(LedChannelIdSet_t channelIdSet);
  int                                                           turnOn(LedChannelId_t channelId, byte intensity, LedBlinkTime_t blinkTime = 0);
  int                                                           turnOff(LedChannelId_t channelId);
protected:
  int                                                           blinkFromISR(LedDeviceInfo* device, TimerId_t timerId, bool* woken);
private:
  static void                                                   cbTimerFired(TimerId_t timerId, void* extraArg, bool* woken);
protected:
  SystemMutex                                                   mutex_Key;
};

#endif // _LED_CONTROLLER_H

#endif // _CONF_LED_CONTROLLER_ENABLED