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


#define DISPLAY_DEVICE_DRAW_UPPER_RIGHT         0x01
#define DISPLAY_DEVICE_DRAW_UPPER_LEFT          0x02
#define DISPLAY_DEVICE_DRAW_LOWER_LEFT          0x04
#define DISPLAY_DEVICE_DRAW_LOWER_RIGHT         0x08
#define DISPLAY_DEVICE_DRAW_ALL                 (DISPLAY_DEVICE_DRAW_UPPER_RIGHT|DISPLAY_DEVICE_DRAW_UPPER_LEFT|DISPLAY_DEVICE_DRAW_LOWER_RIGHT|DISPLAY_DEVICE_DRAW_LOWER_LEFT)

//# font
#define DISPLAY_FONT_IDX_PCSENIOR_8F            0
#define DISPLAY_FONT_IDX_6x13_MF                1
#define DISPLAY_FONT_IDX_MAX                    2

// Serial console
#define DISPLAY_DEVICE_PIN_LIB_DEFAULT          -1
//#########################Setting enum###############
//-------------------comm----------------------
// Comm: Baudrate
#define DISPLAY_SET_COMM_BAUD_1200              0
#define DISPLAY_SET_COMM_BAUD_2400              1
#define DISPLAY_SET_COMM_BAUD_4800              2
#define DISPLAY_SET_COMM_BAUD_9600              3
#define DISPLAY_SET_COMM_BAUD_14400             4
#define DISPLAY_SET_COMM_BAUD_19200             5
#define DISPLAY_SET_COMM_BAUD_38400             6
#define DISPLAY_SET_COMM_BAUD_57600             7
#define DISPLAY_SET_COMM_BAUD_115200            8
#define DISPLAY_SET_COMM_BAUD_MAX               9
// Comm: parity
#define DISPLAY_SET_COMM_PARITY_NONE            0
#define DISPLAY_SET_COMM_PARITY_ODD             1
#define DISPLAY_SET_COMM_PARITY_EVEN            2
#define DISPLAY_SET_COMM_PARITY_MAX             3
// Comm: stopbit
#define DISPLAY_SET_COMM_STOPBIT_1              0
#define DISPLAY_SET_COMM_STOPBIT_2              1
#define DISPLAY_SET_COMM_STOPBIT_MAX            2
// Comm: databit
#define DISPLAY_SET_COMM_DATABIT_8              0
#define DISPLAY_SET_COMM_DATABIT_7              1
#define DISPLAY_SET_COMM_DATABIT_6              2
#define DISPLAY_SET_COMM_DATABIT_5              3
#define DISPLAY_SET_COMM_DATABIT_MAX            4
// Comm: control follow
#define DISPLAY_SET_COMM_FLOW_CTRL_NONE         0
#define DISPLAY_SET_COMM_FLOW_CTRL_RTS_NO       1
#define DISPLAY_SET_COMM_FLOW_CTRL_CTS_NO       2
#define DISPLAY_SET_COMM_FLOW_CTRL_CTS          3
#define DISPLAY_SET_COMM_FLOW_CTRL_MAX          4

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
  DisplayJMDDevice,
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

typedef struct _displayJMDDeviceConfigTag
{
  byte pin_SCL; // currently always use 22
  byte pin_SDA; // currently always use 21
} DisplayJMDDeviceConfigTAG;

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
    DisplayJMDDeviceConfigTAG jmd;
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

