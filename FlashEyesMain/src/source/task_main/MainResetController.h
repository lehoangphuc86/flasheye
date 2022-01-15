#include "MainController.h"

#if (_CONF_MAIN_RESET_CONTROLLER_ENABLED)

#ifndef _MAIN_RESET_CONTROLLER_H
#define _MAIN_RESET_CONTROLLER_H

/////////////////////////////////////////////////
// INCLUDE

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
/*MainResetController*/
class MainResetController
  : public MainController
{
public:
  MainResetController(void);
  virtual ~MainResetController(void);
public:
  bool                                                          isValid(void) override;
protected:
  void                                                          regEventSize(void) override;
  int                                                           prepare(void) override;
  void                                                          clear(void) override;
  int                                                           onEventHandling(EventDataItem* eventData) override;

protected:
  char*                                                         sql_Script_Buff;
};
#endif // _MAIN_RESET_CONTROLLER_H

#endif // _CONF_MAIN_RESET_CONTROLLER_ENABLED