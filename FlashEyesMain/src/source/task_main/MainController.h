#include "MainControllerConstant.h"

#if (_CONF_MAIN_CONTROLLER_ENABLED)

#ifndef _MAIN_CONTROLLER_H
#define _MAIN_CONTROLLER_H

/////////////////////////////////////////////////
// INCLUDE
#include "task_manager/TaskManager.h"
#if (!_CONF_TASK_MANAGER_ENABLED)
#error Task manager is required
#endif // !_CONF_TASK_MANAGER_ENABLED

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
/*MainController*/
class MainController
  : public TaskManager
{
public:
  MainController(byte systemMode);
  virtual ~MainController(void);
  bool                                                          isBusy(void);
  virtual bool                                                  isValid(void);
  byte                                                          systemMode(void);
  BufferDataItem*                                               getCommData(void);
  BufferDataManager*                                            dataManager(void);
  void                                                          releaseCommData(BufferDataItem* dataItem);
  virtual int                                                   start(MainControllerConfigTAG& controllerConfig);
  virtual void                                                  stop(void);
protected:
  void                                                          isBusy(bool flag);
  Seq_t                                                         curSeqId(void);
  Seq_t                                                         nextSeqId(void);
  virtual void                                                  regEventSize(void) = 0;
  virtual void                                                  proc(void) override;
  virtual int                                                   prepare(void);
  virtual void                                                  clear(void);
  virtual int                                                   onEventHandling(EventDataItem* eventData) = 0;
protected:
  byte                                                          system_Mode;
  BufferDataManager                                             data_Manager;
  byte                                                          is_Busy;
  Seq_t                                                         sequence_Id;
  SystemCriticalSession                                         is_Busy_Key;
  SystemMutex                                                   mutex_Operating;
  
};

#endif // _MAIN_CONTROLLER_H

#endif // _CONF_MAIN_CONTROLLER_ENABLED