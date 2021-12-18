#include "task_manager/TaskManagerConstant.h"

#ifndef _TEST_TASK_MANAGER_H
#define _TEST_TASK_MANAGER_H

#if (1)

/////////////////////////////////////////////////
// INCLUDE
#include "event_manager/EventManager.h"
#include "os_system/SystemMutex.h"
#include "os_system/SystemCriticalSession.h"
#include "task_manager/TaskSystemThread.h"

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

/*TestTaskManager*/
class TestTaskManager
{
  ////////////////////Function to call from outside ISR///////////////////////
public:
  TestTaskManager(void);
  // WARNING: if inherite from this class, deconstructor must be virtual
  __ATTRIBUTE_VIRTUAL_OPTIMIZED ~TestTaskManager(void);
  void                                                          proc(void);
private:
  void                                                          mainProc(void);
  static void                                                   navigatingProc(void* param);
  static uint16_t                                               getNextTaskId(void);
protected:
  EventManager                                                  event_Manager;
  SystemCriticalSession                                         critical_Key;
  TaskSystemThread*                                             thread_Handler;
  TaskManagerConfigTAG                                          task_Config;
  bool                                                          is_Running;
  uint16_t                                                      task_Id;
  uint16_t                                                      max_Handling_Event_Size;
  bool                                                          is_Waiting_Prepare_Result;
private:
  static uint16_t                                               next_Task_Id;
};

#endif // 

#endif // _TEST_TASK_MANAGER_H