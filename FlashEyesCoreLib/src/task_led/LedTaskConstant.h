#include "../FlashEyesCoreLibConfig.h"
#ifndef _LED_TASK_CONSTANT_H
#define _LED_TASK_CONSTANT_H
/////////////////////////////////////////////////
// INCLUDE
#if (_CONF_LED_TASK_CONSTANT_ENABLED)
#include "../os_system/SystemCommon.h"
#include "timer_manager/TimerManagerConstant.h"
/////////////////////////////////////////////////
// PREPROCESSOR

/////////////////////////////////////////////////
// DEFINE
#define LED_DEVICE_INTENSITY_LEVEL_DEFAULT          DIGITIAL_VALUE_MAX
#define LED_DEVICE_INTENSITY_LEVEL_MIN              DIGITIAL_VALUE_MIN
#define LED_DEVICE_INTENSITY_LEVEL_MAX              DIGITIAL_VALUE_MAX

#define LED_DEVICE_BLINK_TIME_DEFAULT               1000 // ms. 1000ms ON -> 1000ms OFF
#define LED_DEVICE_BLINK_TIME_MIN                   500 // ms
#define LED_DEVICE_BLINK_TIME_MAX                   100000 // 100 sec

#define LED_DEVICE_PIN_MIN                          PIN_NUMER_MIN
#define LED_DEVICE_PIN_MAX                          PIN_NUMBER_MAX

#define LED_DEVICE_PIN_INVALID                      255

#define LED_DEVICE_CHANNEL_ID_MIN                   0
#define LED_DEVICE_CHANNEL_ID_MAX                   8
#define LED_DEVICE_CHANNEL_ID_INVALID               255

#define LED_DEVICE_BLINK_TIMER_ID_INVALID           TIMER_MANAGER_TIMER_ID_INVALID

#define LED_DEVICE_MANAGER_MAX_DEVICE               8
#define LED_DEVICE_CHANNEL_ID_SET_ALL_DEVICE        0xFFFF

#define LED_CMD_TURN_OFF                            0
#define LED_CMD_TURN_ON                             1
//#define LED_CMD_BLINK                               2

#define LED_DEVICE_PIN_MODE                         GPIO_OUTPUT
/////////////////////////////////////////////////
// MARCO

/////////////////////////////////////////////////
// GLOBAL VARIABLES

/////////////////////////////////////////////////
// GLOBAL FUNCTIONS

/////////////////////////////////////////////////
// DATA TYPE (TYPEDEF)
typedef uint8_t                                     LedChannelId_t;
typedef uint16_t                                    LedChannelIdSet_t;
typedef uint16_t                                    LedBlinkTime_t;
/////////////////////////////////////////////////
// DATA TYPE (ENUM)

/////////////////////////////////////////////////
// DATA TYPE (STRUCT)
typedef struct _ledDeviceConfigTag
{
  LedChannelId_t channelId;
  byte pin;
  bool isPWM;
  byte defaultIntensity;
  //void* devManager; // internal use. No need to set.
} LedDeviceConfigTAG;

typedef struct _ledDeviceManagerConfigTag
{
  byte deviceMaxCount;
} LedDeviceManagerConfigTAG;

typedef struct _ledControllerConfigTag
{
  byte reserved;
} LedControllerConfigTAG;

typedef struct _ledControlParamTag
{
  LedChannelIdSet_t channelIdSet; // bit nth for channel (n-1)
  byte cmd; // LED_CMD_TURN_OFF / LED_CMD_TURN_ON/ LED_CMD_BLINK
  byte intensity; // LED_CMD_TURN_ON
  LedBlinkTime_t blinkTime;// LED_CMD_TURN_ON . set 0 to not blink
  //union 
  //{
  //  byte intensity; // LED_CMD_TURN_ON
  //  LedBlinkTime_t blinkTime;// LED_CMD_BLINK
  //} param;
} LedControlParamTAG;


typedef struct _ledManagerConfigTag
{
  LedControllerConfigTAG controllerConfig;
  LedDeviceConfigTAG* deviceConfig;
  byte deviceCount;
} LedManagerConfigTAG;

/////////////////////////////////////////////////
// STATIC DATA

/////////////////////////////////////////////////
// STATIC FUNCTIONS
//

/////////////////////////////////////////////////
// EXTERN

/////////////////////////////////////////////////
// CLASS DEFINITION


#endif // _CONF_LED_TASK_CONSTANT_ENABLED

#endif // _LED_TASK_CONSTANT_H

