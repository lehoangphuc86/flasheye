#include "DisplayTaskConstant.h"

#if (_CONF_DISPLAY_CONTROL_TASK_ENABLED)

#ifndef _DISPLAY_CONTROL_TASK_H
#define _DISPLAY_CONTROL_TASK_H

/////////////////////////////////////////////////
// INCLUDE
#include "../task_manager/TaskManager.h"
#include "DisplayControllerFactory.h"

#if (!_CONF_TASK_MANAGER_ENABLED)
#error Task manager is required
#endif // _CONF_TASK_MANAGER_ENABLED

#if (!_CONF_DISPLAY_CONTROLLER_FACTORY_ENABLED)
#error Display controller factory is required
#endif // _CONF_DISPLAY_CONTROLLER_FACTORY_ENABLED
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
  DisplayControlTask(void);
  virtual ~DisplayControlTask(void);
  bool                                                          isValid(void);
  virtual int                                                   inititialize(void);
  virtual int                                                   startTask(DisplayTaskConfigTAG& displayConfig);
  BufferDataItem*                                               getBuff(void);
  BufferDataManager*                                            dataManager(void);
  void                                                          releaseBuff(BufferDataItem* dataItem);
  virtual void                                                  stopTask(void);
  virtual void                                                  cleanUp(void);
protected:
  virtual void                                                  proc(void) override;
  virtual void                                                  regEventSize(void);
  virtual void                                                  onEventHandler(EventDataItem* eventItem);
  virtual int                                                   prepare(void);
protected:
  DisplayController*                                            dp_Controller;
  BufferDataManager                                             data_Manager;
};

#endif // _DISPLAY_CONTROL_TASK_H

#endif // _CONF_DISPLAY_CONTROL_TASK_ENABLED