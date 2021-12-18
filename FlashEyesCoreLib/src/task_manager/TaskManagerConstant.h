#ifndef _TASK_MANAGER_CONSTANT_H
#define _TASK_MANAGER_CONSTANT_H

/////////////////////////////////////////////////
// INCLUDE
#include "../FlashEyesCoreLibConfig.h"
#include "../os_system/SystemCommon.h"
#include "../event_manager/EventManagerConstant.h"
#if (_CONF_TASK_MANAGER_CONSTANT_ENABLED)

/////////////////////////////////////////////////
// PREPROCESSOR


/////////////////////////////////////////////////
// DEFINE
#ifdef SYSTEM_ARDUINO_BASED
#define SYSTEM_TASK_PRIORITY_MAX                    (configMAX_PRIORITIES - 1)
#elif defined (SYSTEM_PC_BASED)
#define SYSTEM_TASK_PRIORITY_MAX                    255
#else // SYSTEM_ARDUINO_BASED
#error Unsupported system
#endif // SYSTEM_ARDUINO_BASED


#define TASK_MANAGER_TASK_NAME_LEN                  5
#define TASK_MANAGER_EVENT_WAIT_TIME_DEFAULT        10000
#define TASK_MANAGER_STACK_DEPTH_DEFAULT            256
#define TASK_MANAGER_PRIORITY_MAX                   SYSTEM_TASK_PRIORITY_MAX
#define TASK_MANAGER_PRIORITY_DEFAULT               TASK_MANAGER_PRIORITY_MAX/2
#define TASK_MANAGER_PREPARE_TIME_DEFAULT           100000
#define TASK_MANAGER_PREPARE_RET_CODE_OK            0
#define TASK_MANAGER_CHECK_TERMINATOR_INTERVAL      100
#define TASK_MANAGER_PREPARE_EVENT_CATCHING_TIME    100


#define TASK_THREAD_POOL_MANAGER_POOL_COUNT_MAX     255
#define TASK_THREAD_POOL_MANAGER_POOL_COUNT_MIN     1
#define TASK_THREAD_POOL_ID_INVALID                 TASK_THREAD_POOL_MANAGER_POOL_COUNT_MAX

#define TASK_THREAD_POOL_MANAGER_CANDIDATE_NUM      3
/////////////////////////////////////////////////
// MARCO

/////////////////////////////////////////////////
// GLOBAL VARIABLES

/////////////////////////////////////////////////
// GLOBAL FUNCTIONS

/////////////////////////////////////////////////
// DATA TYPE (TYPEDEF)
typedef void(*TaskThreadProc)(void* param);
//class TaskThreadManager;
/////////////////////////////////////////////////
// DATA TYPE (ENUM)

/////////////////////////////////////////////////
// DATA TYPE (STRUCT)


// DATA TYPE (STRUCT)

typedef struct _systemThreadConfigTAG
{
  bool enabled;
  TaskSize_t usStackDepth;
  byte uxPriority;
  TaskThreadProc proc;
  void* procParams;

  bool isValidMem(void)
  {
    if ((this->enabled == false)
      || (this->usStackDepth <= 0)
      )
    {
      return false;
    }
    return true;
  }

  bool isValidPriority(void)
  {
    if ((this->enabled == false)
      || (this->uxPriority < 0)
      || (this->uxPriority > TASK_MANAGER_PRIORITY_MAX)
      )
    {
      return false;
    }
    return true;
  }

  bool isValidProc(void)
  {
    if ((this->enabled == false)
      || (proc == NULL))
    {
      return false;
    }
    return true;
  }

  bool isValid(void)
  {
    do
    {
      if ((this->enabled == false)
        || (this->isValidProc() == false)
        || (this->isValidMem() == false)
        || (this->isValidPriority() == false)
        )
      {
        break;
      }
      return true;
    } while (0);
    return false;
  }

} SystemThreadConfigTAG;

typedef struct _taskThreadConfigTAG
{
  bool enabled;
  unsigned short usStackDepth;
  byte uxPriority;
  bool useThreadPool;
  bool isValid(void)
  {
    do
    {
      if ((this->enabled == false)
        //|| (proc == NULL)
        || (this->usStackDepth <= 0)
        || (this->uxPriority < 0)
        || (this->uxPriority > TASK_MANAGER_PRIORITY_MAX)
        )
      {
        break;
      }

      return true;
    } while (0);

    return false;
  }
} TaskThreadConfigTAG;

typedef struct _taskManagerConfigTAG
{
  EventCount_t eventItemNumber; // >0 means using eventmanager
  bool eventUsePool;
} TaskManagerConfigTAG;
// event
typedef struct _eventPrepareCompletedTAG
{
  TaskId_t taskId;
  int8_t retCode;
} EventPrepareCompletedTAG;




/////////////////////////////////////////////////
// STATIC DATA

/////////////////////////////////////////////////
// STATIC FUNCTIONS
//

/////////////////////////////////////////////////
// EXTERN

/////////////////////////////////////////////////
// CLASS DEFINITION


#endif // _CONF_TASK_MANAGER_CONSTANT_ENABLED

#endif // _TASK_MANAGER_CONSTANT_H

