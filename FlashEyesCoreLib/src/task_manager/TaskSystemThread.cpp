/////////////////////////////////////////////////
// INCLUDE
#include "TaskSystemThread.h"
#if (_CONF_TASK_SYSTEM_THREAD_ENABLED)

#include "../utility/Utility.h"
/////////////////////////////////////////////////
// PREPROCESSOR

/////////////////////////////////////////////////
// DEFINE
#define TASK_THREAD_STATE_MASK_TERMINATED_FLAG      0x80
#define TASK_THREAD_STATE_MASK_RUNNING_JOB_FLAG     0x40

//#define TASK_THREAD_RUNNING_JOB_DEBUG_ENABLED
/////////////////////////////////////////////////
// MARCO

/////////////////////////////////////////////////
// GLOBAL VARIABLES

/////////////////////////////////////////////////
// GLOBAL FUNCTIONS

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
// CLASS IMPLEMENTAION

/* TaskSystemThread*/
TaskSystemThread::TaskSystemThread(unsigned int sequenceId, PoolId_t poolId)
  : DataItem((byte)DataManagerConstant::DataItemType::TaskSystemThread, (DataCount_t)sequenceId, poolId)
  , thread_State(0)
  , thread_proc(NULL)
  , thread_proc_Params(NULL)
  , thread_Handler(NULL)
{
  this->isTerminated(false);
  this->isRunningJob(false);
}


TaskSystemThread::TaskSystemThread(SystemThreadConfigTAG& threadConfig, unsigned int sequenceId, PoolId_t poolId)
  : TaskSystemThread(sequenceId, poolId)
{
  this->prepare(threadConfig);
}

TaskSystemThread::~TaskSystemThread(void)
{
  this->join(true);
  this->deleteHandler();
}

int TaskSystemThread::prepare(SystemThreadConfigTAG& threadConfig)
{
  return this->createHandler(threadConfig);
}

int TaskSystemThread::setJob(SystemThreadConfigTAG& threadConfig)
{
  do
  {
    if (this->isRunningJob() != false)
    {
      return -1;
    }

    if (this->isValid() == false)
    {
      break;
    }
    
    if (threadConfig.isValidProc() == false)
    {
      break;
    }

    this->threadProc(threadConfig.proc);
    this->threadProcParams(threadConfig.procParams);
    this->job_Lock_Key.unlock();
    this->setPriority(threadConfig.uxPriority);
    return 0;
  } while (0);

  return -1;
}

void TaskSystemThread::deleteJob(void)
{
  this->threadProc(NULL);
  this->threadProcParams(NULL);
}

bool TaskSystemThread::isValid(void)
{
  //SystemCriticalLocker criticalLocker(this->critical_Key);
  return (this->thread_Handler == NULL? false: true);
}

bool TaskSystemThread::isTerminated(void)
{
  SystemCriticalLocker criticalLocker(this->critical_Key);
  return (this->thread_State & TASK_THREAD_STATE_MASK_TERMINATED_FLAG);
}

bool TaskSystemThread::isRunningJob(void)
{
  SystemCriticalLocker criticalLocker(this->critical_Key);
  return (this->thread_State & TASK_THREAD_STATE_MASK_RUNNING_JOB_FLAG);
}

TaskThreadProc TaskSystemThread::threadProc(void)
{
  SystemCriticalLocker criticalLocker(this->critical_Key);
  return this->thread_proc;
}

void* TaskSystemThread::threadProcParams(void)
{
  SystemCriticalLocker criticalLocker(this->critical_Key);
  return this->thread_proc_Params;
}

SystemThreadHandler TaskSystemThread::nativeHandler(void)
{
  //SystemCriticalLocker criticalLocker(this->critical_Key);
  return this->thread_Handler;
}

bool TaskSystemThread::joinable(void)
{
  //SystemCriticalLocker criticalLocker(this->critical_Key);
  do
  {
    if (this->thread_Handler == NULL)
    {
      break;
    }

#ifdef SYSTEM_ARDUINO_BASED
    return true;
#elif defined (SYSTEM_PC_BASED)
    return this->thread_Handler->joinable();
#endif // SYSTEM_ARDUINO_BASED
    return true;
  } while (0);
  return false;
}

void TaskSystemThread::join(bool terminateThread)
{
  //SystemCriticalLocker criticalLocker(this->critical_Key);
  do
  {
    if ( (this->isValid() == false)
      || (this->joinable() == false)
      )
    {
      break;
    }

    if (terminateThread != false)
    {
      this->isTerminated(true); // terminate thread after proc
    }
    this->deleteJob();
    this->job_Lock_Key.unlock();
#ifdef SYSTEM_ARDUINO_BASED
    this->join_Lock_Key.lock(SYSTEM_MUTEX_MAX_WAIT_TIME); // wait for proc completing
    //SYSTEM_SLEEP(200);
#elif defined (SYSTEM_PC_BASED)
    if (terminateThread != false)
    {
      this->thread_Handler->join(); // wait and terminate thread
      return;
    }
    this->join_Lock_Key.lock(SYSTEM_MUTEX_MAX_WAIT_TIME); // wait for job proc completing then back to idle proc
#endif  // SYSTEM_ARDUINO_BASED
    return;
  } while (0);
  return;
}

void TaskSystemThread::detach(void)
{
  //SystemCriticalLocker criticalLocker(this->critical_Key);
  do
  {
    if (this->isValid() == false)
    {
      break;
    }

#ifdef SYSTEM_ARDUINO_BASED
    return; // not suported
#elif defined (SYSTEM_PC_BASED)
    return this->thread_Handler->detach();
#endif  // SYSTEM_ARDUINO_BASED
    return;
  } while (0);
  return;
}

int TaskSystemThread::reset(void)
{
  return 0;
}

void TaskSystemThread::isTerminated(bool flag)
{
  SystemCriticalLocker criticalLocker(this->critical_Key);
  if ((bool)(this->thread_State & TASK_THREAD_STATE_MASK_TERMINATED_FLAG) == flag)
  {
    return;
  }
  this->thread_State = (this->thread_State ^ TASK_THREAD_STATE_MASK_TERMINATED_FLAG);
}

void TaskSystemThread::isRunningJob(bool flag)
{
  SystemCriticalLocker criticalLocker(this->critical_Key);
  if ((bool)(this->thread_State & TASK_THREAD_STATE_MASK_RUNNING_JOB_FLAG) == flag)
  {
    return;
  }
  this->thread_State = (this->thread_State ^ TASK_THREAD_STATE_MASK_RUNNING_JOB_FLAG);
}

int TaskSystemThread::createHandler(SystemThreadConfigTAG& threadConfig)
{
  do
  {
    if (this->thread_Handler != NULL)
    {
      break;
    }
    
    if ((threadConfig.isValidMem()==false)
      || (threadConfig.isValidPriority() == false)
      )
    {
      break;
    }

    this->threadProc(threadConfig.proc);
    this->threadProcParams(threadConfig.procParams);
    this->thread_Handler = SystemThreadHandlerFactory::generate(threadConfig, TaskSystemThread::navigatingProc, (void*)this);
    if (thread_Handler == NULL)
    {
      break;
    }
    return 0;
  } while (0);
  
  this->deleteHandler();
  return -1;
}


int TaskSystemThread::deleteHandler(void)
{
  do
  {
    if (this->thread_Handler == NULL)
    {
      break;
    }
    this->isTerminated(true);
    SystemThreadHandlerFactory::release(this->thread_Handler);
    return 0;
  } while (0);
  return -1;
}

void TaskSystemThread::threadProc(TaskThreadProc threadProc)
{
  SystemCriticalLocker criticalLocker(this->critical_Key);
  this->thread_proc = threadProc;
}

void TaskSystemThread::threadProcParams(void* procParams)
{
  SystemCriticalLocker criticalLocker(this->critical_Key);
  this->thread_proc_Params = procParams;
}

int TaskSystemThread::setPriority(byte priority)
{
  do
  {
    SystemCriticalLocker criticalLocker(this->critical_Key);
    if (this->isValid() == false)
    {
      break;
    }

#ifdef SYSTEM_ARDUINO_BASED
    vTaskPrioritySet(this->thread_Handler, priority);
#elif defined (SYSTEM_PC_BASED)
    return 0; // not suported
#endif  // SYSTEM_ARDUINO_BASED
    return 0;
  } while (0);
  return -1;
}

void TaskSystemThread::proc(void)
{
  do
  {
    while (this->isTerminated() == false)
    {
      if (this->threadProc() == NULL)
      {
        //idle
#ifdef TASK_THREAD_RUNNING_JOB_DEBUG_ENABLED
        CONSOLE_LOG("[thd] idl %i", this->id());
#endif // TASK_THREAD_RUNNING_JOB_DEBUG_ENABLED

        this->isRunningJob(false);
        this->job_Lock_Key.lock(SYSTEM_THREAD_WAIT_JOB_TIME);
        continue;
      }
      this->isRunningJob(true);
#ifdef TASK_THREAD_RUNNING_JOB_DEBUG_ENABLED
      CONSOLE_LOG("[thd] job %i", this->id());
#endif // TASK_THREAD_RUNNING_JOB_DEBUG_ENABLED
      this->join_Lock_Key.lock();
      TaskThreadProc runningProc = this->threadProc();
      void* procParams = this->threadProcParams();
      this->deleteJob();
      runningProc(procParams);
      this->join_Lock_Key.unlock();
    }
    return;
  } while (0);
  return;
}

void TaskSystemThread::navigatingProc(void* param)
{
  do
  {
    if (param == NULL)
    {
      break;
    }
    TaskSystemThread* caller = (TaskSystemThread*)param;
    caller->proc();
#ifdef SYSTEM_ARDUINO_BASED
    vTaskDelete(caller->thread_Handler);
#endif // SYSTEM_ARDUINO_BASED
    return;
  } while (0);
  return;
}


////////////////////Function to call from inside ISR///////////////////////
bool TaskSystemThread::isValidFromISR(void)
{
  //SystemCriticalLockerFromISR criticalLocker(this->critical_Key);
  return (this->thread_Handler == NULL ? false : true);
}

SystemThreadHandler TaskSystemThread::nativeHandlerFromISR(void)
{
  //SystemCriticalLockerFromISR criticalLocker(this->critical_Key);
  return this->thread_Handler;
}

bool TaskSystemThread::joinableFromISR(void)
{
  //SystemCriticalLockerFromISR criticalLocker(this->critical_Key);
  do
  {
    if (this->thread_Handler == NULL)
    {
      break;
    }

#ifdef SYSTEM_ARDUINO_BASED
    return true;
#elif defined (SYSTEM_PC_BASED)
    return this->thread_Handler->joinable();
#endif // SYSTEM_ARDUINO_BASED

    return true;
  } while (0);
  return false;
}



/*SystemThreadHandlerFactory*/
SystemThreadHandlerFactory::SystemThreadHandlerFactory(void)
{

}

SystemThreadHandlerFactory::~SystemThreadHandlerFactory(void)
{

}


SystemThreadHandler SystemThreadHandlerFactory::generate(SystemThreadConfigTAG& threadConfig, TaskThreadProc proc, void* procParams)
{
  SystemThreadHandler handler = NULL;
  int result = 0;
  do
  {

    if ( (threadConfig.isValidMem() == false)
      || (threadConfig.isValidPriority() == false)
      )
    {
      break;
    }

    if (proc == NULL)
    {
      break;
    }

#ifdef SYSTEM_ARDUINO_BASED
    BaseType_t basedRet = xTaskCreate(
      proc
      //, this->task_Config.threadConfig.pcName   // A name just for humans
      , "task"
      , threadConfig.usStackDepth  // This stack size can be checked & adjusted by reading the Stack Highwater
      , procParams
      , threadConfig.uxPriority  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
      , &handler);

    result = (basedRet != pdPASS ? -1 : 0);
    //SYSTEM_CONSOLE("[thrFac] gen %i %i", 2, result);
#elif defined (SYSTEM_PC_BASED)
    handler = new std::thread(proc, procParams);
#endif // SYSTEM_ARDUINO_BASED
    if ((result != 0)
      || (handler == NULL)
      )
    {
      break;
    }
    return handler;
  } while (0);

  SystemThreadHandlerFactory::release(handler);
  return handler;
}

void SystemThreadHandlerFactory::release(SystemThreadHandler& handler)
{
  do
  {
    if (handler == NULL)
    {
      break;
    }

#ifdef SYSTEM_ARDUINO_BASED
    //vTaskDelete(handler);
#elif defined (SYSTEM_PC_BASED)
    delete handler;
#endif  // SYSTEM_ARDUINO_BASED

    handler = NULL;
    return;
  } while (0);

  return;
}


#endif // _CONF_TASK_SYSTEM_THREAD_ENABLED