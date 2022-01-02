#include "../FlashEyesCoreLibConfig.h"
#ifndef _TASK_OPCODE_CONSTANT_H
#define _TASK_OPCODE_CONSTANT_H

/////////////////////////////////////////////////
// INCLUDE
#if (_CONF_TASK_OPCODE_CONSTANT_ENABLED)
#include "../os_system/SystemCommon.h"
/////////////////////////////////////////////////
// PREPROCESSOR

/////////////////////////////////////////////////
// DEFINE
#define TASK_OPCODE_BUTTON_DEBOUNCE_TIME_DEFAULT          200 // ms
#ifdef SYSTEM_ARDUINO_BASED
#define TASK_OPCODE_MENU_ITEM_DESCRIPTION_LEN_MAX         1
#else // SYSTEM_ARDUINO_BASED
#define TASK_OPCODE_MENU_ITEM_DESCRIPTION_LEN_MAX         12
#endif // SYSTEM_ARDUINO_BASED
#define TASK_OPCODE_MENU_ITEM_COUNT_MAX                   8

#define TASK_OPCODE_MENU_ITEM_OPCODE_INVALID              255
/////////////////////////////////////////////////
// MARCO

/////////////////////////////////////////////////
// GLOBAL VARIABLES

/////////////////////////////////////////////////
// GLOBAL FUNCTIONS

/////////////////////////////////////////////////
// DATA TYPE (TYPEDEF)
typedef bool(*CbOnOpCodeRecevied)(int opCode);
typedef void(*IsrButtonPressed)();
/////////////////////////////////////////////////
// DATA TYPE (ENUM)

/////////////////////////////////////////////////
// DATA TYPE (STRUCT)

typedef struct _opCodeMenuItemTAG
{
  byte opCode;
  byte pin;
  byte gpioFunc; // GPIO_INPUT/ GPIO_OUTPUT/ GPIO_PULLUP/ GPIO_INPUT_PULLUP
  byte triggerType; // ISR_RISING / ISR_FALLING/ ISR_CHANGE/ ISR_ONLOW/ ISR_ONHIGH/ ISR_ONLOW_WE/ ISR_ONHIGH_WE
  char description[TASK_OPCODE_MENU_ITEM_DESCRIPTION_LEN_MAX];
  bool isSelected;
  TimePoint_t lastSelected;
} OpCodeMenuItemTAG;

typedef struct _buttonPressedParamsTAG
{
  byte opCode;
} ButtonPressedParamsTAG;

typedef struct _eventButtonPressedParamsTAG
{
  byte opCode;
} EventButtonPressedParamsTAG;
/////////////////////////////////////////////////
// STATIC DATA

/////////////////////////////////////////////////
// STATIC FUNCTIONS

/////////////////////////////////////////////////
// EXTERN

/////////////////////////////////////////////////
// CLASS DEFINITION

#endif // _CONF_TASK_OPCODE_CONSTANT_ENABLED

#endif // _TASK_OPCODE_CONSTANT_H

