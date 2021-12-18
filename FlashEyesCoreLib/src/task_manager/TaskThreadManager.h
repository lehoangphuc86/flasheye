#include "TaskManagerConstant.h"

#ifndef _TASK_THREAD_MANAGER_H
#define _TASK_THREAD_MANAGER_H

#if (_CONF_TASK_THREAD_MANAGER_ENABLED)

/////////////////////////////////////////////////
// INCLUDE
#include "../data_manager/DataManager.h"
#include "TaskSystemThread.h"
#include "TaskThreadPoolManager.h"
#if (!_CONF_DATA_MANAGER_ENABLED)
#error Data manager is required
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
typedef DataManagerConfigTAG<SystemThreadConfigTAG> TaskThreadManagerConfigTAG;
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

/*TaskThreadManager*/
class TaskThreadManager
  : protected DataManager
{
  ////////////////////Function to call from outside ISR///////////////////////
public:
  TaskThreadManager(PoolId_t poolId = DATA_POOL_ID_INVALID);
  __ATTRIBUTE_VIRTUAL_OPTIMIZED ~TaskThreadManager(void);
  int                                                           prepare(TaskThreadManagerConfigTAG& poolConfig);
  void                                                          clear(void) override;
  bool                                                          isReadyQueueEmpty(void);
  DataCount_t                                                   readyQueueSize(void);
  TaskSystemThread*                                             get(void);
  int                                                           release(TaskSystemThread* thread);
};

#endif // _CONF_TASK_THREAD_MANAGER_ENABLED

#endif // _TASK_THREAD_MANAGER_H