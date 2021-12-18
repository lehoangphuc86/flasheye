#include "TaskManagerConstant.h"

#ifndef _TASK_SYSTEM_THREAD_H
#define _TASK_SYSTEM_THREAD_H

#if (_CONF_TASK_SYSTEM_THREAD_ENABLED)

/////////////////////////////////////////////////
// INCLUDE
#include "../os_system/SystemMutex.h"
#include "../os_system/SystemCriticalSession.h"
#include "../data_manager/DataItem.h" // for thread pool
#if (!_CONF_SYSTEM_MUTEX_ENABLED)
#error System mutex is required
#endif // _CONF_SYSTEM_MUTEX_ENABLED

#if (!_CONF_SYSTEM_CRITICAL_SESSION_ENABLED)
#error System critical session is required
#endif // _CONF_SYSTEM_CRITICAL_SESSION_ENABLED

#ifdef SYSTEM_ARDUINO_BASED

#elif defined (SYSTEM_PC_BASED)
#ifdef SYSTEM_WIN_PLATFORM
#include <thread>
#elif defined(SYSTEM_LINUX_PLATFORM)
#include <thread>
#else
#error Unsupported system type
#endif // 

#else // 
#error Unsupported system type
#endif // SYSTEM_ARDUINO_BASED


/////////////////////////////////////////////////
// PREPROCESSOR

/////////////////////////////////////////////////
// DEFINE
#ifdef SYSTEM_ARDUINO_BASED
#define SYSTEM_THREAD_DATA_TYPE                 TaskHandle_t
#elif defined (SYSTEM_PC_BASED)
#ifdef SYSTEM_WIN_PLATFORM
#define SYSTEM_THREAD_DATA_TYPE                 std::thread*
#elif defined(SYSTEM_LINUX_PLATFORM)
#define SYSTEM_THREAD_DATA_TYPE                 std::thread*
#else
#error Unsupported system type
#endif // 
#else // 
#error Unsupported system type
#endif // SYSTEM_ARDUINO_BASED

#define SYSTEM_THREAD_WAIT_JOB_TIME             5000

#define SYSTEM_THREAD_STATE_MASK_TERMINATING    0x80
#define SYSTEM_THREAD_STATE_MASK_RUNNING_JOB    0x40
/////////////////////////////////////////////////
// MARCO

/////////////////////////////////////////////////
// GLOBAL VARIABLES

/////////////////////////////////////////////////
// GLOBAL FUNCTIONS

/////////////////////////////////////////////////
// DATA TYPE (TYPEDEF)
typedef SYSTEM_THREAD_DATA_TYPE SystemThreadHandler;
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

/*TaskSystemThread*/
class TaskSystemThread
  : public DataItem
{
  ////////////////////Function to call from outside ISR///////////////////////
public:
  TaskSystemThread(unsigned int sequenceId, PoolId_t poolId);
  TaskSystemThread(SystemThreadConfigTAG& threadConfig, unsigned int sequenceId, PoolId_t poolId);
  // WARNING: if inherite from this class, deconstructor must be virtual
  __ATTRIBUTE_VIRTUAL_OPTIMIZED ~TaskSystemThread(void);
  int                                                           prepare(SystemThreadConfigTAG& threadConfig); // to compatible with DataManager for TaskSystemThreadPool
  int                                                           setJob(SystemThreadConfigTAG& threadConfig); // to switch from current proc to new one. Update priority and stack depth
  void                                                          deleteJob(void);
  bool                                                          isValid(void) override;
  bool                                                          isTerminated(void);
  bool                                                          isRunningJob(void);
  TaskThreadProc                                                threadProc(void);
  void*                                                         threadProcParams(void);
  SystemThreadHandler                                           nativeHandler(void);
  int                                                           setPriority(byte priority);
  bool                                                          joinable(void);
  void                                                          join(bool terminateThread = true);
  void                                                          detach(void);
protected:
  int                                                           reset(void) override;
  void                                                          isTerminated(bool flag);
  void                                                          isRunningJob(bool flag);
  int                                                           createHandler(SystemThreadConfigTAG& threadConfig);
  int                                                           deleteHandler(void);
  void                                                          threadProc(TaskThreadProc threadProc);
  void                                                          threadProcParams(void* procParams);
private:
  void                                                          proc(void);
  static void                                                   navigatingProc(void* param);

  ////////////////////Function to call from inside ISR///////////////////////
public:
  bool                                                          isValidFromISR(void);
  SystemThreadHandler                                           nativeHandlerFromISR(void);
  bool                                                          joinableFromISR(void);

protected:
  byte                                                          thread_State; //0th bit: is terminated. 1st: running job
  TaskThreadProc                                                thread_proc;
  void*                                                         thread_proc_Params;
  SystemThreadHandler                                           thread_Handler;
  SystemCriticalSession                                         critical_Key;
  SystemMutex                                                   join_Lock_Key;
  SystemMutex                                                   job_Lock_Key;
};


/*SystemThreadHandlerFactory*/
class SystemThreadHandlerFactory
{
public:
  SystemThreadHandlerFactory(void);
  // WARNING: if inherite from this class, deconstructor must be virtual
  __ATTRIBUTE_VIRTUAL_OPTIMIZED ~SystemThreadHandlerFactory(void);
public:
  static SystemThreadHandler                                    generate(SystemThreadConfigTAG& threadConfig, TaskThreadProc proc, void* procParams);
  static void                                                   release(SystemThreadHandler& handler);

};

#endif // _CONF_TASK_SYSTEM_THREAD_ENABLED

#endif // _TASK_SYSTEM_THREAD_H