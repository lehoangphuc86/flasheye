#include "TaskManagerConstant.h"

#ifndef _TASK_MANAGER_H
#define _TASK_MANAGER_H

#if (_CONF_TASK_MANAGER_ENABLED)

/////////////////////////////////////////////////
// INCLUDE
#include "../event_manager/EventManager.h"
#include "../os_system/SystemMutex.h"
#include "../os_system/SystemCriticalSession.h"
//#include "TaskSystemThread.h"
//#include "TaskThreadPoolManager.h"
#include "TaskThreadManager.h"

#if (!_CONF_EVENT_MANAGER_ENABLED)
#error Event manager is required
#endif // _CONF_EVENT_MANAGER_ENABLED

#if (!_CONF_SYSTEM_MUTEX_ENABLED)
#error System mutex is required
#endif // _CONF_SYSTEM_MUTEX_ENABLED

#if (!_CONF_SYSTEM_CRITICAL_SESSION_ENABLED)
#error System critical session is required
#endif // _CONF_SYSTEM_CRITICAL_SESSION_ENABLED

#if (!_CONF_TASK_SYSTEM_THREAD_ENABLED)
#error Task system thread is required
#endif // _CONF_TASK_SYSTEM_THREAD_ENABLED

#if (!_CONF_TASK_THREAD_POOL_MANAGER_ENABLED)
#error Task thread pool manager is required
#endif // _CONF_TASK_THREAD_POOL_MANAGER_ENABLED
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

/*TaskManager*/
class TaskManager
{
  ////////////////////Function to call from outside ISR///////////////////////
public:
  TaskManager(void);
  virtual ~TaskManager(void);
  bool                                                          isTaskRunning(void);
  bool                                                          isWaitingPrepare(void);
  bool                                                          isTaskValid(void);
  TaskId_t                                                      taskId(void);
  EventManager*                                                 eventManager(void);
  int                                                           notify(EventId_t messageId, EventSize_t eventSize, unsigned char* eventData);
protected:
  bool                                                          usingEvent(void);
  void                                                          isTaskRunning(bool state);
  void                                                          isWaitingPrepare(bool state);
  TaskSystemThread*                                             threadHandler(void);
  void                                                          registerHanldingEventStructSize(uint16_t structSize);
  int                                                           setConfig(TaskManagerConfigTAG& taskConfig);
  int                                                           prepare(void);
  virtual void                                                  proc(void);
  int                                                           startProcess(TaskThreadConfigTAG& threadConfig, bool waitPrepare);// = false);
  int                                                           stopProcess(void);
  int                                                           reportPrepareResult(int retCode);
  int                                                           waitPrepareResult(void);
  void                                                          waitTerminating(void);
private:
  void                                                          mainProc(void);
  static void                                                   navigatingProc(void* param);
  static TaskId_t                                               getNextTaskId(void);

  ////////////////////Function to call from inside ISR///////////////////////
public:
  bool                                                          isTaskRunningFromISR(void);
  bool                                                          isTaskValidFromISR(void);
  int                                                           notifyFromISR(EventId_t messageId, EventSize_t eventSize, unsigned char* eventData, bool* higherTaskWoken);

protected:
  //bool                                                          is_Running;
  //bool                                                          is_Waiting_Prepare_Result;
  byte                                                          task_State; // 0th bit: isRunning, 1st bit: wait prepare
  EventManager                                                  event_Manager;
  TaskSystemThread*                                             thread_Handler;
  SystemCriticalSession                                         critical_Key;
  TaskId_t                                                      task_Id;
  EventSize_t                                                   max_Handling_Event_Size; // >0 means using event. 
  //byte                                                          thread_Pool_Id;
private:
  static TaskId_t                                               next_Task_Id;
};

#endif // _TASK_MANAGER_H

#endif // _CONF_TASK_MANAGER_ENABLED