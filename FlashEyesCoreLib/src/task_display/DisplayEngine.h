#include "DisplayTaskCommon.h"

#if (_CONF_DISPLAY_ENGINE_ENABLED)

#ifndef _DISPLAY_ENGINE_H
#define _DISPLAY_ENGINE_H

/////////////////////////////////////////////////
// INCLUDE
#include "DisplayDeviceController.h"

#if (!_CONF_DISPLAY_DEVICE_CONTROLLER_ENABLED)
#error Display device controller is required
#endif // _CONF_DISPLAY_DEVICE_CONTROLLER_ENABLED
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

/*DisplayEngine*/

class DisplayEngine
{
public:
  DisplayEngine(void);
  virtual ~DisplayEngine(void);
  virtual bool                                                  isTaskValid(void);
  virtual int                                                   setConfig(DisplayDeviceConfig& deviceConfig);
  virtual int                                                   prepare(void);
  virtual int                                                   start(void);
  virtual int                                                   controlDisplay(EventDisplayControlTAG* data);
  virtual void                                                  displayStatus(EventDisplayStatusTAG* data);
  virtual void                                                  displayDistance(EventDisplayDistanceTAG* data);
  virtual void                                                  displayTime(EventDisplayTimeTAG* data);
protected:
  
protected:
  DisplayDeviceController                                       device_Controller;
};

#endif // _DISPLAY_ENGINE_H

#endif // _CONF_DISPLAY_ENGINE_ENABLED