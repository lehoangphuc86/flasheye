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
  int                                                           setConfig(MainControllerConfigTAG& controllerConfig) override;
  int                                                           start(void) override;
  void                                                          stop(void) override;
  void                                                          cleanUp(void) override;

protected:
  void                                                          regEventSize(void) override;
  int                                                           prepare(void) override;

protected:
  //JsonParser                                                    js_Parser;
};
#endif // _MAIN_RESET_CONTROLLER_H

#endif // _CONF_MAIN_RESET_CONTROLLER_ENABLED