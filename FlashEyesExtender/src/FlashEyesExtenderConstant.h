#ifndef _FLASH_EYE_EXTENDER_CONSTANT_H
#define _FLASH_EYE_EXTENDER_CONSTANT_H

/////////////////////////////////////////////////
// INCLUDE
#include "src/shared/SharedCore.h"
/////////////////////////////////////////////////
// PREPROCESSOR

/////////////////////////////////////////////////
// DEFINE
#define FLASH_EYE_TASK_MEM_MAIN_CONTROLLER            1024
#define FLASH_EYE_TASK_MEM_INPUT_HANDLER              1024
#define FLASH_EYE_TASK_MEM_DISPLAY_CONTROL            1024
#define FLASH_EYE_TASK_MEM_DISTANCE_SEN_CONTROL       1024

#define FLASH_EYE_TASK_PRIORITY_MAIN_CONTROLLER       2
#define FLASH_EYE_TASK_PRIORITY_INPUT_HANDLER         2
#define FLASH_EYE_TASK_PRIORITY_DISPLAY_CONTROL       2
#define FLASH_EYE_TASK_PRIORITY_DISTANCE_SEN_CONTROL  2

#define FLASH_EYE_TASK_EVENT_NO_MAIN_CONTROLLER       5
#define FLASH_EYE_TASK_EVENT_NO_INPUT_HANDLER         3
#define FLASH_EYE_TASK_EVENT_NO_DISPLAY_CONTROL       3
#define FLASH_EYE_TASK_EVENT_NO_DISTANCE_SEN_CONTROL  3

#define FLASH_EYE_INPUT_SETTER_INTERRUPT_PIN          2
#define FLASH_EYE_INPUT_SETTER_BUTTON_0               4
#define FLASH_EYE_INPUT_SETTER_BUTTON_1               5


// Display
#define FLASH_EYE_DISPLAY_LCD_COLUMN_NO               16
#define FLASH_EYE_DISPLAY_LCD_ROW_NO                  2
#define FLASH_EYE_DISPLAY_LCD_PIN_V0                  10 //DISPLAY_DEVICE_PIN_INVALID // pwm
#define FLASH_EYE_DISPLAY_LCD_PIN_RS                  23
#define FLASH_EYE_DISPLAY_LCD_PIN_RW                  25
#define FLASH_EYE_DISPLAY_LCD_PIN_E                   27
#define FLASH_EYE_DISPLAY_LCD_PIN_D4                  29
#define FLASH_EYE_DISPLAY_LCD_PIN_D5                  31
#define FLASH_EYE_DISPLAY_LCD_PIN_D6                  33
#define FLASH_EYE_DISPLAY_LCD_PIN_D7                  35
#define FLASH_EYE_DISPLAY_LCD_PIN_A                   9   // pwm
#define FLASH_EYE_DISPLAY_LCD_INIT_BACKLIGHT          250
#define FLASH_EYE_DISPLAY_LCD_INIT_BRIGHTNESS         0

// Distance sensor
#define FLASH_EYE_DISTANCE_SENSOR_PIN_TRIGGER         47
#define FLASH_EYE_DISTANCE_SENSOR_PIN_ECHO            45


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

/*For event*/



typedef struct _eventInputReceivedTAG
{
  byte count;
  byte id;
  unsigned long time;
} EventInputReceivedTAG;

/////////////////////////////////////////////////
// STATIC DATA

/////////////////////////////////////////////////
// STATIC FUNCTIONS

/////////////////////////////////////////////////
// EXTERN

/////////////////////////////////////////////////
// CLASS DEFINITION

#endif // _FLASH_EYE_EXTENDER_CONSTANT_H