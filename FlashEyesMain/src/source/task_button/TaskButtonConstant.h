#include "../../FlashEyesMain.h"
#ifndef _TASK_BUTTON_CONSTANT_H
#define _TASK_BUTTON_CONSTANT_H

/////////////////////////////////////////////////
// INCLUDE
#if (_CONF_TASK_BUTTON_CONSTANT_ENABLED)
#include "os_system/SystemCommon.h"
#include "task_opcode/TaskOpCodeConstant.h"
/////////////////////////////////////////////////
// PREPROCESSOR

/////////////////////////////////////////////////
// DEFINE
#define TASK_BUTTON_DEBOUNCE_TIME_DEFAULT                 200 // ms
#define TASK_BUTTON_ITEM_COUNT_MIN                        1
#define TASK_BUTTON_ITEM_COUNT_MAX                        3

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

typedef struct _buttonItemTAG
{
  byte opCode;
  byte pin;
  byte gpioFunc; // GPIO_INPUT/ GPIO_OUTPUT/ GPIO_PULLUP/ GPIO_INPUT_PULLUP
  byte triggerType; // ISR_RISING / ISR_FALLING/ ISR_CHANGE/ ISR_ONLOW/ ISR_ONHIGH/ ISR_ONLOW_WE/ ISR_ONHIGH_WE
  IsrButtonPressedEx cbOnPressedEx;
  void* cbOnPressedExArg;
} ButtonItemTAG;

typedef struct _buttonConfigTag
{
  ButtonItemTAG* items;
  byte itemCount;
  uint16_t bounceTime;
} ButtonConfigTAG;

typedef struct _buttonManagerConfigTag
{
  ButtonConfigTAG config;
  byte reserved;
} ButtonManagerConfigTAG;

/////////////////////////////////////////////////
// STATIC DATA

/////////////////////////////////////////////////
// STATIC FUNCTIONS

/////////////////////////////////////////////////
// EXTERN

/////////////////////////////////////////////////
// CLASS DEFINITION

#endif // _CONF_TASK_BUTTON_CONSTANT_ENABLED

#endif // _TASK_BUTTON_CONSTANT_H