#include "TaskOpCodeConstant.h"

#if (_CONF_TASK_OPCODE_ENABLED)

#ifndef _TASK_OPCODE_TASK_H
#define _TASK_OPCODE_TASK_H
/////////////////////////////////////////////////
// INCLUDE
#include "os_system/SystemCommon.h"
#include "task_manager/TaskManager.h"
/////////////////////////////////////////////////
// PREPROCESSOR
/////////////////////////////////////////////////
// DEFINE

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
  //int                                                           setConfig(OpCodeMenuConfigTAG& menuConfig);
  int                                                           prepare(OpCodeMenuConfigTAG& menuConfig);
  void                                                          clear(void);
  int                                                           appendItem(OpCodeMenuItemConfigTAG* menuItem, byte* outIndexP);
  int                                                           removeItemByIndex(byte opIndex);
  int                                                           removeItemByOpCode(byte opCode);
  byte                                                          menuItemCount(void);
  bool                                                          isPressedByIndex(byte opIndex);
  bool                                                          isPressedByOpCode(byte opCode);
  int                                                           select(void);
  int                                                           select(int* outOpCodes, int outOpCodeMaxCount, int& outOpCodeCount);
protected:
  int                                                           removeItem(OpCodeMenuItemTAG* menuItem);
  OpCodeMenuItemTAG*                                            findItemByOpCode(byte opCode);
  void                                                          display(void);
  bool                                                          isPressed(OpCodeMenuItemTAG* menuItem);
  int                                                           getOpCode(void);
  int                                                           getOpCode(int* outOpCodes, int outOpCodeMaxCount, int &outOpCodeCount);

  void                                                          onButtonPressed(byte buttonIndex, bool* woken);
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
  uint16_t                                                      bounce_Time;
  byte                                                          item_Count;
  OpCodeMenuItemTAG                                             item_List[TASK_OPCODE_MENU_ITEM_COUNT_MAX];
  IsrButtonPressed                                              item_Isr[TASK_OPCODE_MENU_ITEM_COUNT_MAX];
  SystemCriticalSession                                         critical_Key;
};

/*OpcodeInputTask*/
class OpcodeInputTask
  : public TaskManager
{
public:
  static OpcodeInputTask& getInstance(void);
  OpcodeInputTask(OpcodeInputTask const&) = delete;
  void operator=(OpcodeInputTask const&) = delete;
  bool                                                          isRunning(void);
  /*int                                                           setConfig(
                                                                  OpCodeMenuConfigTAG& menuConfig
                                                                  , CbOnOpCodeRecevied cbOnOpCodeRev
                                                                  , TaskManagerConfigTAG& taskConfig
                                                                  , bool useTask = true
  );*/
  int                                                           appendMenuItem(OpCodeMenuItemConfigTAG* menuItem, byte* outIndex);
  int                                                           start(OpCodeInputTaskConfigTAG& inputTaskConfig);
  void                                                          stop(void);
  void                                                          cleanUp(void);
protected:
  int                                                           prepare(void);
  void                                                          isRunning(bool state);
  void                                                          proc(void) override;
private:
  OpcodeInputTask(void);
  ~OpcodeInputTask(void);
protected:
  CbOnOpCodeRecevied                                            cb_OpCode_Rev;
  bool                                                          is_Running;
  bool                                                          use_Task;
};


#endif // _TASK_OPCODE_TASK_H

#endif // _CONF_TASK_OPCODE_ENABLED