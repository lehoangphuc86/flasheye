#include "LedTaskConstant.h"
#ifndef _LED_MANAGER_H
#define _LED_MANAGER_H
#if (_CONF_LED_MANAGER_ENABLED)
/////////////////////////////////////////////////
// INCLUDE
#include "LedController.h"
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

/*LedManager*/
class LedManager
{
private:
  LedManager(void);
  __ATTRIBUTE_VIRTUAL_OPTIMIZED ~LedManager(void);

public:
  static LedManager& getInstance(void);
  LedManager(LedManager const&) = delete;
  void operator=(LedManager const&) = delete;
  bool                                                          isValid(void);
  int                                                           start(LedManagerConfigTAG& ledManagerConfig);
  void                                                          stop(void);
  int                                                           control(LedControlParamTAG& controlParam);
  int                                                           turnOnAll(byte intensity, LedBlinkTime_t blinkTime = 0);
  int                                                           turnOffAll(void);
  int                                                           turnOnMul(LedChannelIdSet_t channelIdSet, byte intensity, LedBlinkTime_t blinkTime = 0);
  int                                                           turnOffMul(LedChannelIdSet_t channelIdSet);
  int                                                           turnOn(LedChannelId_t channelId, byte intensity, LedBlinkTime_t blinkTime = 0);
  int                                                           turnOff(LedChannelId_t channelId);
protected:
  LedController                                                 led_Controller;
};
#endif // _CONF_LED_MANAGER_ENABLED

#endif // _LED_MANAGER_H