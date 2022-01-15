#include "MainController.h"

#if (_CONF_MAIN_SETTING_CONTROLLER_ENABLED)

#ifndef _MAIN_SETTING_CONTROLLER_H
#define _MAIN_SETTING_CONTROLLER_H

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
/*MainSettingController*/
class MainSettingController
  : public MainController
{
public:
  MainSettingController(void);
  virtual ~MainSettingController(void);
public:
  bool                                                          isValid(void) override;  

protected:
  void                                                          regEventSize(void) override;
  int                                                           prepare(void) override;
  void                                                          clear(void) override;
  int                                                           onEventHandling(EventDataItem* eventData) override;

protected:
  //JsonParser                                                    js_Parser;
};
#endif // _MAIN_SETTING_CONTROLLER_H

#endif // _CONF_MAIN_SETTING_CONTROLLER_ENABLED