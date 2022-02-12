#include "TaskButtonConstant.h"

#if (_CONF_BUTTON_MANAGER_ENABLED)

#ifndef _BUTTON_MANAGER_H
#define _BUTTON_MANAGER_H
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

/*ButtonManager*/
class ButtonManager
{
public:
  static ButtonManager& getInstance(void);
  ButtonManager(ButtonManager const&) = delete;
  void operator=(ButtonManager const&) = delete;
  bool                                                          isValid(void);
  int                                                           start(ButtonManagerConfigTAG& btnManagerConfig);
  bool                                                          isPressed(byte opCode);
  void                                                          stop(void);
private:
  ButtonManager(void);
  ~ButtonManager(void);
protected:
  byte                                                          btn_Opcode[TASK_BUTTON_ITEM_COUNT_MAX];
  byte                                                          btn_Count;
};

#endif // _BUTTON_MANAGER_H

#endif // _CONF_BUTTON_MANAGER_ENABLED