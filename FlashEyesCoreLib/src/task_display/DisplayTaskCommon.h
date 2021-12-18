#ifndef _DISPLAY_TASK_COMMON_H
#define _DISPLAY_TASK_COMMON_H

/////////////////////////////////////////////////
// INCLUDE
#include "../FlashEyesCoreLibConfig.h"
#if (_CONF_DISPLAY_TASK_COMMON_ENABLED)
#include "../os_system/SystemCommon.h"
/////////////////////////////////////////////////
// PREPROCESSOR

/////////////////////////////////////////////////
// DEFINE
#define DISPLAY_DEVICE_BRIGHTNESS_LEVEL_DEFAULT DIGITIAL_VALUE_MIN
#define DISPLAY_DEVICE_BRIGHTNESS_LEVEL_MIN     DIGITIAL_VALUE_MIN
#define DISPLAY_DEVICE_BRIGHTNESS_LEVEL_MAX     DIGITIAL_VALUE_MAX

#define DISPLAY_DEVICE_BACKLIGHT_LEVEL_DEFAULT  DIGITIAL_VALUE_MIN
#define DISPLAY_DEVICE_BACKLIGHT_LEVEL_MIN      DIGITIAL_VALUE_MIN
#define DISPLAY_DEVICE_BACKLIGHT_LEVEL_MAX      DIGITIAL_VALUE_MAX

#define DISPLAY_DEVICE_PIN_MIN                  PIN_NUMER_MIN
#define DISPLAY_DEVICE_PIN_MAX                  PIN_NUMBER_MAX

#define DISPLAY_DEVICE_PIN_INVALID              255

#define DISPLAY_DEVICE_ROW_MIN                  1
#define DISPLAY_DEVICE_COLUMN_MIN               1


#define DISPLAY_MESSAGE_LEN                     10
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

typedef struct
{
  byte id;
  byte i2c_Add;
  byte columnNo;
  byte rowNo;
  byte pin_V0;
  byte pin_RS;
  byte pin_RW;
  byte pin_E;
  byte pin_D4;
  byte pin_D5;
  byte pin_D6;
  byte pin_D7;
  byte pin_A;
  byte initBacklight;
  byte initBrightness;
} DisplayDeviceConfig;

// event
typedef enum
{
  InvalidDisplayControlId = 0
  , DisplayControlBrightness
  , DisplayControlBacklight
} DisplayControlId;
/////////////////////////////////////////////////
// DATA TYPE (STRUCT)
typedef struct
{
  byte count;
  unsigned long time;
  //char status[DISPLAY_MESSAGE_LEN + 1];
} EventDisplayStatusTAG;

typedef struct
{
  byte count;
  unsigned long time;
} EventDisplayTimeTAG;

typedef struct
{
  byte sensorId;
  unsigned long measuredTime;
  unsigned long delay;
  double distance;
} EventDisplayDistanceTAG;

typedef struct
{
  byte controlId;
  byte brighnessLevel;
  byte backLightLevel;
} EventDisplayControlTAG;
/////////////////////////////////////////////////
// STATIC DATA

/////////////////////////////////////////////////
// STATIC FUNCTIONS
//

/////////////////////////////////////////////////
// EXTERN

/////////////////////////////////////////////////
// CLASS DEFINITION


#endif // _CONF_DISPLAY_TASK_COMMON_ENABLED

#endif // _DISPLAY_TASK_COMMON_H

