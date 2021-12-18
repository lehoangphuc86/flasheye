#include "DisplayTaskCommon.h"

#if (_CONF_DISPLAY_CONTROL_TASK_ENABLED)

#ifndef _DISPLAY_CONTROL_TASK_H
#define _DISPLAY_CONTROL_TASK_H

/////////////////////////////////////////////////
// INCLUDE
#include "../task_manager/TaskManager.h"
#include "DisplayEngine.h"

#if (!_CONF_TASK_MANAGER_ENABLED)
#error Task manager is required
#endif // _CONF_TASK_MANAGER_ENABLED

#if (!_CONF_DISPLAY_ENGINE_ENABLED)
#error Display engine is required
#endif // _CONF_DISPLAY_ENGINE_ENABLED
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

/*DisplayControlTask*/

class DisplayControlTask
  : public TaskManager
{
public:
  static DisplayControlTask&                                    getInstance(void);
  DisplayControlTask(DisplayControlTask const&)                 = delete;
  void operator=(DisplayControlTask const&)                     = delete;
  virtual int                                                   setConfig(TaskConfigStruct& taskConfig, DisplayDeviceConfig& deviceConfig);
  virtual int                                                   prepare(void);
  virtual int                                                   startProcess(void) override;

protected:
  virtual void                                                  proc(void) override;
  virtual void                                                  onEventDisplayStatus(unsigned int dataSize, unsigned char* data);
  virtual void                                                  onEventDisplayDistance(unsigned int dataSize, unsigned char* data);
  virtual void                                                  onEventDisplayTime(unsigned int dataSize, unsigned char* data);
  virtual void                                                  onEventDisplayControl(unsigned int dataSize, unsigned char* data);
private:
  DisplayControlTask(void);
  virtual ~DisplayControlTask(void);

protected:
  DisplayEngine                                                 display_Engine;
};

#endif // _DISPLAY_CONTROL_TASK_H

#endif // _CONF_DISPLAY_CONTROL_TASK_ENABLED