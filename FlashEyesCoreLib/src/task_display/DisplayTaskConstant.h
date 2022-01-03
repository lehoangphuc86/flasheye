#include "../FlashEyesCoreLibConfig.h"
#ifndef _DISPLAY_TASK_CONSTANT_H
#define _DISPLAY_TASK_CONSTANT_H
/////////////////////////////////////////////////
// INCLUDE
#if (_CONF_DISPLAY_TASK_CONSTANT_ENABLED)
#include "../os_system/SystemCommon.h"
#include "../data_manager/BufferDataManager.h"
#include "../task_manager/TaskManagerConstant.h"
/////////////////////////////////////////////////
// PREPROCESSOR
//#define DISPLAY_DEVICE_I2C_DEVICE
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

// Serial console
#define DISPLAY_DEVICE_SC_PIN_LIB_DEFAULT       -1
#define DISPLAY_DEVICE_SC_PARITY_NONE           0
#define DISPLAY_DEVICE_SC_PARITY_ODD            1
#define DISPLAY_DEVICE_SC_PARITY_EVEN           2

//#define DISPLAY_MESSAGE_LEN                     10
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
typedef enum _displayDeviceTypeUN
{
  DisplayLCDDevice =0,
  DisplayLCDI2CDevice,
  DisplaySerialConsoleDevice,
  DisplayDeviceTypeMax
} DisplayDeviceTypeUN;


/////////////////////////////////////////////////
// DATA TYPE (STRUCT)
typedef struct _displayLcdDeviceConfigTag
{
  byte pin_V0_Brightness;
  byte pin_RS;
  byte pin_RW;
  byte pin_E;
  byte pin_D4;
  byte pin_D5;
  byte pin_D6;
  byte pin_D7;
  byte pin_A_Backlight;
  byte initBacklight;
  byte initBrightness;
} DisplayLcdDeviceConfigTAG;

typedef struct _displayLcdI2CDeviceConfigTag
{
  byte i2c_Add;
  byte pin_V0_Brightness;
  byte pin_A_Backlight;
  byte initBacklight;
  byte initBrightness;
} DisplayLcdI2CDeviceConfigTAG;

typedef struct _displaySCDeviceConfigTag
{
  byte instanceIndex;
  uint32_t baudrate;
  byte bitPerByte;  // 5-8
  byte parityType; // DISPLAY_DEVICE_SC_PARITY_NONE / DISPLAY_DEVICE_SC_PARITY_ODD / DISPLAY_DEVICE_SC_PARITY_EVEN
  byte stopBitNum; // 1-2
  uint16_t timeout; // ms. set 0 to use default
  byte pin_TX;
  byte pin_RX;
} DisplaySCDeviceConfigTAG;

typedef struct
{
  byte deviceType;
  byte id;
  byte columnNo;
  byte rowNo;
  union 
  {
    DisplayLcdDeviceConfigTAG lcd;
    DisplayLcdI2CDeviceConfigTAG lcdI2C;
    DisplaySCDeviceConfigTAG serialConsole;
  } spec;
} DisplayDeviceConfig;

typedef struct _displayTaskConfigTag
{
  DisplayDeviceConfig deviceConfig;
  BufferDataManagerConfigTAG bufferConfig;
  TaskManagerConfigTAG taskManagerConfig;
  TaskThreadConfigTAG taskThreadConfig;
} DisplayTaskConfigTAG;
/////////////////////////////////////////////////
// STATIC DATA

/////////////////////////////////////////////////
// STATIC FUNCTIONS
//

/////////////////////////////////////////////////
// EXTERN

/////////////////////////////////////////////////
// CLASS DEFINITION


#endif // _CONF_DISPLAY_TASK_CONSTANT_ENABLED

#endif // _DISPLAY_TASK_CONSTANT_H

