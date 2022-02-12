#include "../FlashEyesCoreLibConfig.h"
#ifndef _TASK_OPCODE_CONSTANT_H
#define _TASK_OPCODE_CONSTANT_H

/////////////////////////////////////////////////
// INCLUDE
#if (_CONF_TASK_OPCODE_CONSTANT_ENABLED)
#include "../os_system/SystemCommon.h"
#include "../task_manager/TaskManagerConstant.h"
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

#define TASK_OPCODE_MENU_ITEM_OPCODE_MIN                  0
#define TASK_OPCODE_MENU_ITEM_OPCODE_MAX                  254
#define TASK_OPCODE_MENU_ITEM_OPCODE_INVALID              (TASK_OPCODE_MENU_ITEM_OPCODE_MAX + 1)

#define TASK_OPCODE_MENU_ITEM_INDEX_MIN                   0
#define TASK_OPCODE_MENU_ITEM_INDEX_MAX                   (TASK_OPCODE_MENU_ITEM_COUNT_MAX -1)
#define TASK_OPCODE_MENU_ITEM_INDEX_INVALID               (TASK_OPCODE_MENU_ITEM_INDEX_MAX + 1)
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
typedef void(*IsrButtonPressedEx)(void* arg, byte opCode, bool* woken);
/////////////////////////////////////////////////
// DATA TYPE (ENUM)

/////////////////////////////////////////////////
// DATA TYPE (STRUCT)


typedef struct _opCodeMenuItemConfigTAG
{
  bool enabled : 1; // internal used
  bool notIsr : 1;
  byte reserved : 6;
  byte opCode;
  byte pin;
  byte gpioFunc; // GPIO_INPUT/ GPIO_OUTPUT/ GPIO_PULLUP/ GPIO_INPUT_PULLUP
  byte triggerType; // ISR_RISING / ISR_FALLING/ ISR_CHANGE/ ISR_ONLOW/ ISR_ONHIGH/ ISR_ONLOW_WE/ ISR_ONHIGH_WE
  IsrButtonPressedEx cbOnPressEx; // this callback would be called if irs is triggerred
  void* cbOnPressExArg;
  char description[TASK_OPCODE_MENU_ITEM_DESCRIPTION_LEN_MAX]; // optional
} OpCodeMenuItemConfigTAG;


typedef struct _opCodeMenuItemTAG
{
  bool enabled      : 1; // internal used
  bool notIsr       : 1;
  bool isSelected   : 1;
  byte reserved     : 5;
  byte opCode;
  byte pin;
  byte triggerType; // ISR_RISING / ISR_FALLING/ ISR_CHANGE/ ISR_ONLOW/ ISR_ONHIGH/ ISR_ONLOW_WE/ ISR_ONHIGH_WE
  IsrButtonPressedEx cbOnPressEx; // this callback would be called if irs is triggerred
  void* cbOnPressExArg;
  char description[TASK_OPCODE_MENU_ITEM_DESCRIPTION_LEN_MAX]; // optional
  TimePoint_t lastSelected; // internal used
} OpCodeMenuItemTAG;

typedef struct _buttonPressedParamsTAG
{
  byte opCode;
} ButtonPressedParamsTAG;

typedef struct _opCodeMenuConfigTag
{
  OpCodeMenuItemConfigTAG* menuItems;
  byte menuItemCount;
  uint16_t bounceTime;
  CbOnOpCodeRecevied cbOnOpCodeRev;
  byte* outIndexs; // if not null, it returns indexs of the menuitems. length must be menuItemCount. 
} OpCodeMenuConfigTAG;

typedef struct _opCodeInputTaskConfigTag
{
  OpCodeMenuConfigTAG menuConfig;
  bool useTask;
  TaskThreadConfigTAG threadConfig; // useTask = true
  TaskManagerConfigTAG taskConfig; // useTask = true
} OpCodeInputTaskConfigTAG;

// events
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

