#include "../FlashEyeCoreTestConfig.h"

#ifdef _CONF_TEST_OP_CODE_INPUT_ENABLED

#ifndef _OP_CODE_INPUT_TASK_H
#define _OP_CODE_INPUT_TASK_H

/////////////////////////////////////////////////
// INCLUDE
#include "os_system/SystemCommon.h"

#ifdef SYSTEM_PC_BASED

#else // SYSTEM_PC_BASED

#if (_CONF_TASK_MANAGER_FUNC_ENABLED_VAL)
#define OPCODE_INPUT_TAS_USE_TASK
#endif // _CONF_TASK_MANAGER_FUNC_ENABLED_VAL

#endif // SYSTEM_PC_BASED

#ifdef OPCODE_INPUT_TAS_USE_TASK
#include "task_manager/TaskManager.h"
#endif // OPCODE_INPUT_TAS_USE_TASK
/////////////////////////////////////////////////
// PREPROCESSOR
/////////////////////////////////////////////////
// DEFINE
#define OPCODE_MENU_BUTTON_DEBOUNC_TIME         200 // ms
#ifdef SYSTEM_ARDUINO_BASED
#define OPCODE_MENU_ITEM_DESCRIPTION_LEN_MAX    1
#else // SYSTEM_ARDUINO_BASED
#define OPCODE_MENU_ITEM_DESCRIPTION_LEN_MAX    12
#endif // SYSTEM_ARDUINO_BASED
#define OPCODE_MENU_ITEM_COUNT_MAX              8
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

typedef struct _opCodeMenuItemTAG
{
  byte opCode;
  byte pin;
  byte triggerType; // FALLING RISING 
  char description[OPCODE_MENU_ITEM_DESCRIPTION_LEN_MAX];
  bool isSelected;
  unsigned long lastSelected;
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
// DATA TYPE (STRUCT)

/////////////////////////////////////////////////
// STATIC DATA

/////////////////////////////////////////////////
// STATIC FUNCTIONS

/////////////////////////////////////////////////
// EXTERN

/////////////////////////////////////////////////
// CLASS DEFINITION

/*OpCodeMenu*/
class OpCodeMenu
{
public:
  static OpCodeMenu& getInstance(void);
  OpCodeMenu(OpCodeMenu const&) = delete;
  void operator=(OpCodeMenu const&) = delete;
  int                                                           setConfig(OpCodeMenuItemTAG* menuItems, byte menuItemCount);
  int                                                           appendItem(OpCodeMenuItemTAG* menuItem);
  byte                                                          menuItemCount(void);
  int                                                           prepare(void);
  int                                                           select(void);
protected:
  void                                                          display(void);
  int                                                           getOpCode(void);

  void                                                          onButtonPressed(byte buttonIndex);
  static void                                                   isrButtonPressed00(void);
  static void                                                   isrButtonPressed01(void);
  static void                                                   isrButtonPressed02(void);
  static void                                                   isrButtonPressed03(void);
  static void                                                   isrButtonPressed04(void);
  static void                                                   isrButtonPressed05(void);
  static void                                                   isrButtonPressed06(void);
  static void                                                   isrButtonPressed07(void);
private:
  OpCodeMenu(void);
  ~OpCodeMenu(void);
protected:
  byte                                                          item_Count;
  OpCodeMenuItemTAG                                             item_List[OPCODE_MENU_ITEM_COUNT_MAX];
  IsrButtonPressed                                              item_Isr[OPCODE_MENU_ITEM_COUNT_MAX];
};

/*OpcodeInputTask*/
class OpcodeInputTask
#ifdef OPCODE_INPUT_TAS_USE_TASK
  : public TaskManager
#endif // OPCODE_INPUT_TAS_USE_TASK
{
public:
  static OpcodeInputTask& getInstance(void);
  OpcodeInputTask(OpcodeInputTask const&) = delete;
  void operator=(OpcodeInputTask const&) = delete;
  bool                                                          isRunning(void);
  int                                                           setConfig(
                                                                  OpCodeMenuItemTAG* menuItems
                                                                  , byte menuItemCount
                                                                  , CbOnOpCodeRecevied cbOnOpCodeRev
#ifdef OPCODE_INPUT_TAS_USE_TASK
                                                                  , TaskManagerConfigTAG& taskConfig
#endif // OPCODE_INPUT_TAS_USE_TASK
  );
  int                                                           appendMenuItem(OpCodeMenuItemTAG* menuItem);
  /*int                                                           notifyButtonPressed(byte opCode);
  int                                                           notifyButtonPressedFromISR(byte opCode);*/
#ifdef OPCODE_INPUT_TAS_USE_TASK
  int                                                           start(TaskThreadConfigTAG& threadConfig);
#else
  int                                                           start(void);
#endif // OPCODE_INPUT_TAS_USE_TASK
  void                                                          stop(void);
  void                                                          cleanUp(void);
protected:
  int                                                           prepare(void);
  void                                                          isRunning(bool state);
#ifdef OPCODE_INPUT_TAS_USE_TASK
  void                                                          proc(void) override;
#else // OPCODE_INPUT_TAS_USE_TASK
  void                                                          proc(void);
#endif //OPCODE_INPUT_TAS_USE_TASK
  //int                                                           onEventButtonPressed(unsigned char* data, unsigned int dataSize);
private:
  OpcodeInputTask(void);
  ~OpcodeInputTask(void);
protected:
  CbOnOpCodeRecevied                                            cb_OpCode_Rev;
#ifndef COMM_SERIAL_USE_TASK
  bool                                                          is_Running;
#endif // COMM_SERIAL_USE_TASK
};


#endif // _OP_CODE_INPUT_TASK_H

#endif // _CONF_TEST_OP_CODE_INPUT_ENABLED