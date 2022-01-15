#include "TaskSwitchConstant.h"

#if (_CONF_SWITCH_MANAGER_ENABLED)

#ifndef _SWITCH_MANAGER_H
#define _SWITCH_MANAGER_H
/////////////////////////////////////////////////
// INCLUDE
#include "os_system/SystemCommon.h"
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

/*SwitchManager*/
class SwitchManager
{
public:
  static SwitchManager& getInstance(void);
  SwitchManager(SwitchManager const&) = delete;
  void operator=(SwitchManager const&) = delete;
  bool                                                          isValid(void);
  int                                                           start(SwitchManagerConfigTAG& swManagerConfig);
  SysMode_t                                                     getSystemMode(void);
  SysMode_t                                                     getState(void);
  bool                                                          isPressed(byte opCode);
  void                                                          stop(void);
private:
  SwitchManager(void);
  ~SwitchManager(void);
protected:
  byte                                                          sw_Opcode[TASK_SWITCH_ITEM_COUNT_MAX];
  byte                                                          sw_Count;
};

#endif // _SWITCH_MANAGER_H

#endif // _CONF_SWITCH_MANAGER_ENABLED