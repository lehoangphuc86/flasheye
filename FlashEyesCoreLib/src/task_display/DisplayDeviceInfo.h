#include "DisplayTaskCommon.h"

#if (_CONF_DISPLAY_DEVICE_INFO_ENABLED)

#ifndef _DISPLAY_DEVICE_INFO_H
#define _DISPLAY_DEVICE_INFO_H

/////////////////////////////////////////////////
// INCLUDE
#include "../os_system/SystemCommon.h"
#include "DisplayTaskCommon.h"

#if (!_CONF_DISPLAY_TASK_COMMON_ENABLED)
#error Display task common is required
#endif // _CONF_DISPLAY_TASK_COMMON_ENABLED
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

/*DisplayDeviceInfo*/

class DisplayDeviceInfo
{
public:
  DisplayDeviceInfo(void);
  virtual ~DisplayDeviceInfo(void);
  bool                                                          isValidConfig(void);
  bool                                                          isBacklightControllable(void);
  bool                                                          isBrightnessControllable(void);
  byte                                                          backlightPin(void);
  byte                                                          brightnessPin(void);
  virtual DisplayDeviceConfig*                                  deviceConfig(void);
  virtual byte                                                  backlightLevel(void);
  virtual byte                                                  brightnessLevel(void);
  virtual void                                                  backlightLevel(byte val);
  virtual void                                                  brightnessLevel(byte val);
  virtual int                                                   setConfig(DisplayDeviceConfig& deviceConfig);
  virtual int                                                   prepare(void);

  
protected:
  bool                                                          isValidConfig(DisplayDeviceConfig& deviceConfig);

protected:
  DisplayDeviceConfig                                           device_Config;
  byte                                                          backlight_Level;
  byte                                                          brightness_Level;
  
};

#endif // _DISPLAY_DEVICE_INFO_H

#endif // _CONF_DISPLAY_DEVICE_INFO_ENABLED