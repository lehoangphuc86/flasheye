#include "../FlashEyesCoreLibConfig.h"
#ifndef _BUZZER_TASK_CONSTANT_H
#define _BUZZER_TASK_CONSTANT_H
/////////////////////////////////////////////////
// INCLUDE
#if (_CONF_BUZZER_TASK_CONSTANT_ENABLED)
#include "../os_system/SystemCommon.h"

/////////////////////////////////////////////////
// PREPROCESSOR

/////////////////////////////////////////////////
// DEFINE
#define BUZZER_DEVICE_PIN_MIN                       PIN_NUMER_MIN
#define BUZZER_DEVICE_PIN_MAX                       PIN_NUMBER_MAX

#define BUZZER_DEVICE_PIN_INVALID                   255

#define BUZZER_DEVICE_HW_CHANNEL_ID_MIN             0
#define BUZZER_DEVICE_HW_CHANNEL_ID_MAX             8
#define BUZZER_DEVICE_HW_CHANNEL_ID_INVALID         255

#define BUZZER_DEVICE_ID_MIN                        0
#define BUZZER_DEVICE_ID_MAX                        7
#define BUZZER_DEVICE_ID_INVALID                    255


#define BUZZER_DEVICE_MANAGER_MAX_DEVICE            (BUZZER_DEVICE_ID_MAX + 1)
#define BUZZER_DEVICE_ID_SET_ALL_DEVICE             0xFFFF

#define BUZZER_CMD_TURN_OFF                         0
#define BUZZER_CMD_TURN_ON                          1

#define BUZZER_DEVICE_PIN_MODE                      GPIO_OUTPUT

#define BUZZER_NODE_DURATION_DEFAULT                100 // ms
/////////////////////////////////////////////////
// MARCO

/////////////////////////////////////////////////
// GLOBAL VARIABLES

/////////////////////////////////////////////////
// GLOBAL FUNCTIONS

/////////////////////////////////////////////////
// DATA TYPE (TYPEDEF)
typedef uint8_t                                     BuzzerHWChannelId_t;
typedef uint8_t                                     BuzzerDeviceId_t;
typedef uint16_t                                    BuzzerDeviceIdSet_t;
typedef uint16_t                                    BuzzerNoteFreq_t;
typedef uint8_t                                     BuzzerMelodyLen_t;
typedef uint16_t                                    BuzzerNoteLen_t;
/////////////////////////////////////////////////
// DATA TYPE (ENUM)

/////////////////////////////////////////////////
// DATA TYPE (STRUCT)
typedef struct _buzzerDeviceConfigTag
{
  BuzzerDeviceId_t deviceId;
  byte pin;
  BuzzerHWChannelId_t hwChannelId;
} BuzzerDeviceConfigTAG;

typedef struct _buzzerDeviceManagerConfigTag
{
  byte deviceMaxCount;
} BuzzerDeviceManagerConfigTAG;

typedef struct _buzzerControllerConfigTag
{
  byte reserved;
} BuzzerControllerConfigTAG;

typedef struct _buzzerControlParamTag
{
  BuzzerDeviceIdSet_t  deviceIdSet; // bit nth for device (n-1)
  byte cmd; // BUZZER_CMD_TURN_OFF / BUZZER_CMD_TURN_ON  
  BuzzerNoteFreq_t* melody;
  BuzzerNoteLen_t* noteLen; // ms
  BuzzerMelodyLen_t melodyLen;
} BuzzerControlParamTAG;

typedef struct _buzzerManagerConfigTag
{
  BuzzerControllerConfigTAG controllerConfig;
  BuzzerDeviceConfigTAG* deviceConfig;
  byte deviceCount;
} BuzzerManagerConfigTAG;

/////////////////////////////////////////////////
// STATIC DATA

/////////////////////////////////////////////////
// STATIC FUNCTIONS
//

/////////////////////////////////////////////////
// EXTERN

/////////////////////////////////////////////////
// CLASS DEFINITION


#endif // _CONF_BUZZER_TASK_CONSTANT_ENABLED

#endif // _BUZZER_TASK_CONSTANT_H

