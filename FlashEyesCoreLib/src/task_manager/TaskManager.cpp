
/////////////////////////////////////////////////
// INCLUDE
#include "TaskManager.h"
#if (_CONF_TASK_MANAGER_ENABLED)
#include "../os_system/SystemCommon.h"
#include "../utility/Utility.h"
/////////////////////////////////////////////////
// PREPROCESSOR

/////////////////////////////////////////////////
// DEFINE
//#define TASK_MANAGER_MEM_USAGE_ENABLED
//#define TASK_MANAGER_DEBUG_CONSOLE_ENABLED

#define TASK_STATE_MASK_RUNNING_FLAG                0x80
#define TASK_STATE_MASK_WAIT_PREPARE_FLAG           0x40
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
#ifdef TASK_MANAGER_DEBUG_CONSOLE_ENABLED
char taskManagerLogBuf[SYSTEM_CONSOLE_OUT_BUF_LEN];
#endif // TASK_MANAGER_DEBUG_CONSOLE_ENABLED
/////////////////////////////////////////////////
// STATIC FUNCTIONS

/////////////////////////////////////////////////
// EXTERN

/////////////////////////////////////////////////
// CLASS IMPLEMENTAION

/* TaskManager*/
TaskId_t TaskManager::next_Task_Id = 0;
TaskManager::TaskManager(void)
  : task_State(0)
  , thread_Handler(NULL)
  , task_Id(0)
  , max_Handling_Event_Size(sizeof(uint8_t))
  //, thread_Pool_Id(TASK_THREAD_POOL_ID_INVALID)
{
  this->task_Id = TaskManager::getNextTaskId();
}

TaskManager::~TaskManager(void)
{
  this->stopProcess();
}

bool TaskManager::isTaskRunning(void)
{
  SystemCriticalLocker criticalLocker(this->critical_Key);
  return (this->task_State & TASK_STATE_MASK_RUNNING_FLAG);
}

bool TaskManager::isWaitingPrepare(void)
{
  SystemCriticalLocker criticalLocker(this->critical_Key);
  return (this->task_State & TASK_STATE_MASK_WAIT_PREPARE_FLAG);
}

bool TaskManager::isTaskValid(void)
{
  SystemCriticalLocker criticalLocker(this->critical_Key);
  return (this->thread_Handler == NULL? false : true);
}

TaskId_t TaskManager::taskId(void)
{
  return this->task_Id;
}

bool TaskManager::usingEvent(void)
{
  //return (this->task_Config.eventItemNumber > 0? true: false);
  return (this->max_Handling_Event_Size > 0 ? true : false);
}

void TaskManager::isTaskRunning(bool state)
{
  SystemCriticalLocker criticalLocker(this->critical_Key);
  if ((bool)(this->task_State & TASK_STATE_MASK_RUNNING_FLAG) == state)
  {
    return;
  }
  this->task_State = (this->task_State ^ TASK_STATE_MASK_RUNNING_FLAG);
}

void TaskManager::isWaitingPrepare(bool state)
{
  SystemCriticalLocker criticalLocker(this->critical_Key);
  if ((bool)(this->task_State & TASK_STATE_MASK_WAIT_PREPARE_FLAG) == state)
  {
    return;
  }
  this->task_State = (this->task_State ^ TASK_STATE_MASK_WAIT_PREPARE_FLAG);
}

TaskSystemThread* TaskManager::threadHandler(void)
{
  SystemCriticalLocker criticalLocker(this->critical_Key);
  return this->thread_Handler;
}

void TaskManager::registerHanldingEventStructSize(uint16_t structSize)
{
  if (structSize > this->max_Handling_Event_Size)
  {
    this->max_Handling_Event_Size = structSize;
  }
}

int TaskManager::setConfig(TaskManagerConfigTAG& taskConfig)
{
  do
  {
    if ((this->isTaskRunning() != false)
     || (this->threadHandler() != NULL)
      )
    {
      break;
    }

    if (this->event_Manager.isValid() != false)
    {
      break; // already config
    }
    // prepare event manager for preparation completion
    EventCount_t eventItemNumber = 1;// 1 for prepareCompleted
    EventSize_t maxEventSize = this->max_Handling_Event_Size;
    if (taskConfig.eventItemNumber > 0)
    {
      // using event
      this->registerHanldingEventStructSize(sizeof(EventPrepareCompletedTAG));
      eventItemNumber = SYSTEM_MAX(eventItemNumber, taskConfig.eventItemNumber);
      maxEventSize = this->max_Handling_Event_Size;
    }
    else
    {
      this->max_Handling_Event_Size = 0;
      eventItemNumber = 1;
      maxEventSize = sizeof(EventPrepareCompletedTAG);
      // not
    }
    return this->event_Manager.prepare(eventItemNumber, maxEventSize, taskConfig.eventUsePool);
  } while (0);

  return -1;
}

int TaskManager::prepare(void)
{
  do
  {
    if ((this->isTaskRunning() != false)
     || (this->threadHandler() != NULL)
      )
    {
      break;
    }

    if (this->event_Manager.isValid() == false)
    {
      break; // not prepared
    }
    return 0;
  } while (0);

  return -1;
}

void TaskManager::proc(void)
{
  // @@ must to be overried by child classes
}

int TaskManager::startProcess(TaskThreadConfigTAG& threadConfig, bool waitPrepare)
{
  int ret = 0;
  do
  {
    if ((this->isTaskRunning() != false)
      || (this->threadHandler() != NULL)
      )
    {
      break;
    }

#ifdef SYSTEM_PC_BASED
    if (threadConfig.enabled == false)
    {
      threadConfig.enabled = true;
      threadConfig.usStackDepth = TASK_MANAGER_STACK_DEPTH_DEFAULT;
      threadConfig.uxPriority = TASK_MANAGER_PRIORITY_DEFAULT;
    }
#endif // SYSTEM_PC_BASED

    if (threadConfig.isValid() == false)
    {
      break;
    }

    
    this->isWaitingPrepare(waitPrepare);

    SystemThreadConfigTAG systemThreadConfig = SystemThreadConfigTAG();
    systemThreadConfig.enabled = true;
    systemThreadConfig.usStackDepth = threadConfig.usStackDepth;
    systemThreadConfig.uxPriority = threadConfig.uxPriority;
    systemThreadConfig.proc = TaskManager::navigatingProc;
    systemThreadConfig.procParams = (void*)this;

    if (threadConfig.useThreadPool == false)
    {
      this->thread_Handler = new TaskSystemThread(systemThreadConfig, 0, DATA_POOL_ID_INVALID);
      if ((this->thread_Handler == NULL)
        || (this->thread_Handler->isValid() == false)
        )
      {
        break;
      }
    }
    else
    {
      TaskThreadPoolQueryParamTAG queryParam = TaskThreadPoolQueryParamTAG();
      queryParam.poolProperties.taskSize = systemThreadConfig.usStackDepth;
      ret = TaskThreadPoolManager::getInstance().request(queryParam, this->thread_Handler);
      if ((ret != 0)
        || (this->thread_Handler == NULL)
        )
      {
        break;
      }

      ret = this->thread_Handler->setJob(systemThreadConfig);
      if (ret != 0)
      {
        break;
      }
    }

    if (waitPrepare == false)
    {
      return 0;
    }

    ret = this->waitPrepareResult();

    if (ret != 0)
    {
      break;
    }

    return 0;
  } while (0);

  this->isWaitingPrepare(false);
  this->stopProcess();
  return -1;
}

int TaskManager::stopProcess(void)
{
  do
  {
    if ((this->isTaskRunning() == false)
      || (this->threadHandler() == NULL)
      )
    {
      break;
    }

    if (this->usingEvent() != false)
    {
      this->notify((int)EventManagerConstant::EventMessageId::TerminateProcess, 0, NULL);
    }
    else
    {
      this->isTaskRunning(false);
    }

    bool usingPool = (this->thread_Handler->poolId() != TASK_THREAD_POOL_ID_INVALID ? true : false);
    this->thread_Handler->join(!usingPool);
    if (usingPool == false)
    {
      delete this->thread_Handler;
      this->thread_Handler = NULL;
      return 0;
    }
    
    TaskThreadPoolManager::getInstance().release(this->thread_Handler);
    //this->thread_Pool_Id = TASK_THREAD_POOL_ID_INVALID;
    //this->thread_Handler->resetPoolId();
    this->thread_Handler = NULL;
    return 0;
  } while (0);

  return -1;
}

int TaskManager::reportPrepareResult(int retCode)
{
  do
  {
    if (this->isWaitingPrepare() == false)
    {
      break;
    }
    EventPrepareCompletedTAG prepareResult = EventPrepareCompletedTAG();
    prepareResult.retCode = (int8_t)retCode;
    prepareResult.taskId = this->taskId();

    int ret = this->notify((int)EventManagerConstant::EventMessageId::PrepareCompleted, sizeof(EventPrepareCompletedTAG), (unsigned char*)&prepareResult);
    this->isWaitingPrepare(false);
    SYSTEM_SLEEP(TASK_MANAGER_PREPARE_EVENT_CATCHING_TIME); // sleep to wait catcher for getting the message
    return ret;
  } while (0);
  return -1;
}

int TaskManager::waitPrepareResult(void)
{
  EventDataItem* eventData = NULL;
  do
  {
    if (SYSTEM_IS_OS_RUNNING() == false)
    {
      this->isWaitingPrepare(false);
      return 0;
    }
    this->isWaitingPrepare(true);
    eventData = this->event_Manager.wait(TASK_MANAGER_PREPARE_TIME_DEFAULT);
    if (eventData == NULL)
    {
      break;
    }

    if (eventData->messageId() == (int)EventManagerConstant::EventMessageId::TerminateProcess)
    {
      this->event_Manager.notify(eventData->messageId(), eventData->dataLength(), eventData->bufferAddress());
      break;
    }

    if ( (eventData->messageId() != (int)EventManagerConstant::EventMessageId::PrepareCompleted)
      || (eventData->bufferAddress() == NULL)
      || (eventData->dataLength() != sizeof(EventPrepareCompletedTAG))
      )
    {
      break;
    }

    EventPrepareCompletedTAG* prepareResult = (EventPrepareCompletedTAG*)eventData->bufferAddress();
    if ( (prepareResult->taskId != this->taskId())
      || (prepareResult->retCode != TASK_MANAGER_PREPARE_RET_CODE_OK)
      )
    {
      break;
    }

    this->event_Manager.release(eventData);
    return 0;
  } while (0);

  if (eventData != NULL)
  {
    this->event_Manager.release(eventData);
  }
  return -1;
}

void TaskManager::waitTerminating(void)
{
#ifdef TASK_MANAGER_DEBUG_CONSOLE_ENABLED
  CONSOLE_LOG_BUF(taskManagerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[tm] wT %i %i", 0, this->usingEvent());
#endif // TASK_MANAGER_DEBUG_CONSOLE_ENABLED
  if (this->usingEvent() != false)
  {
    while (this->isTaskRunning() != false) // A Task shall never return or exit.
    {
      EventDataItem* eventData = this->event_Manager.wait(TASK_MANAGER_EVENT_WAIT_TIME_DEFAULT);
      if (eventData == NULL)
      {
        continue;
      }
#ifdef TASK_MANAGER_DEBUG_CONSOLE_ENABLED
      CONSOLE_LOG_BUF(taskManagerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[tm] wT %i %i", 3, eventData->messageId());
#endif // TASK_MANAGER_DEBUG_CONSOLE_ENABLED
      switch (eventData->messageId())
      {
      case (int)EventManagerConstant::EventMessageId::TerminateProcess:
        this->isTaskRunning(false);
        break;
      default:
        break;
      }
      this->event_Manager.release(eventData);
    }
    return;
  }
  
#ifdef TASK_MANAGER_DEBUG_CONSOLE_ENABLED
  CONSOLE_LOG_BUF(taskManagerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[tm] wT %i %i", 4, this->isTaskRunning());
#endif // TASK_MANAGER_DEBUG_CONSOLE_ENABLED
  while (this->isTaskRunning() != false)
  {
    SYSTEM_SLEEP(TASK_MANAGER_CHECK_TERMINATOR_INTERVAL); // sleep and wait for terminator
  }
  return;
}

EventManager* TaskManager::eventManager(void)
{
  return &this->event_Manager;
}

int TaskManager::notify(EventId_t messageId, EventSize_t eventSize, unsigned char* eventData)
{
  do
  {
    if (this->isTaskRunning() == false)
    {
      break;
    }
    return this->event_Manager.notify(messageId, eventSize, eventData);
  } while (0);
  return -1;
}

void TaskManager::mainProc(void)
{
  this->isTaskRunning(true);
#ifdef TASK_MANAGER_MEM_USAGE_ENABLED
  SYSTEM_TASK_MEM_DATA_TYPE usedMem = SYSTEM_TASK_MEM_USAGE(this->thread_Handler->nativeHandler());
  int globalFreeMem =  Utility::getFreeMemory();
  CONSOLE_LOG("[tm] mem T%i TM%i GM%i", this->taskId(), usedMem, globalFreeMem);
#endif // TASK_MANAGER_MEM_USAGE_ENABLED
  this->proc();
  this->isTaskRunning(false);
}

void TaskManager::navigatingProc(void* param)
{
  do
  {
    if (param==NULL)
    {
      break;
    }
    TaskManager* caller = (TaskManager*)param;
    caller->mainProc();
    //return;
  } while (0);
}

TaskId_t TaskManager::getNextTaskId(void)
{
  TaskManager::next_Task_Id++;
  return TaskManager::next_Task_Id;
}

////////////////////Function to call from inside ISR///////////////////////
bool TaskManager::isTaskRunningFromISR(void)
{
  SystemCriticalLockerFromISR criticalLocker(this->critical_Key);
  return (this->task_State & TASK_STATE_MASK_RUNNING_FLAG);
}

bool TaskManager::isTaskValidFromISR(void)
{
  SystemCriticalLockerFromISR criticalLocker(this->critical_Key);
  return (this->thread_Handler == NULL ? false : true);
}

int TaskManager::notifyFromISR(EventId_t messageId, EventSize_t eventSize, unsigned char* eventData, bool* higherTaskWoken)
{
  do
  {
    if (this->isTaskRunningFromISR() == false)
    {
      break;
    }
    return this->event_Manager.notifyFromISR(messageId, eventSize, eventData, higherTaskWoken);
  } while (0);
  return -1;
}

#endif // _CONF_TASK_MANAGER_ENABLED