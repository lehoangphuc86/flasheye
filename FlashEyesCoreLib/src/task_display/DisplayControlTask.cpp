/////////////////////////////////////////////////
// INCLUDE
#include "DisplayControlTask.h"
#if (_CONF_DISPLAY_CONTROL_TASK_ENABLED)
/////////////////////////////////////////////////
// PREPROCESSOR
#define DISPLAY_TASK_CONSOLE_DEBUG_ENABLE
/////////////////////////////////////////////////
// DEFINE

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
#ifdef DISPLAY_TASK_CONSOLE_DEBUG_ENABLE
char displayTaskLogBuf[SYSTEM_CONSOLE_OUT_BUF_LEN];
#endif // DISPLAY_TASK_CONSOLE_DEBUG_ENABLE
/////////////////////////////////////////////////
// STATIC FUNCTIONS

/////////////////////////////////////////////////
// EXTERN

/////////////////////////////////////////////////
// CLASS IMPLEMENTAION

/* DisplayControlTask*/
DisplayControlTask::DisplayControlTask(void)
  : TaskManager()
  , dp_Controller(NULL)
{

}
DisplayControlTask::~DisplayControlTask(void)
{
  this->cleanUp();
}

bool DisplayControlTask::isValid(void)
{
  return (this->dp_Controller == NULL ? false : true);
}

int DisplayControlTask::inititialize(void)
{
  do
  {
    return 0;
  } while (0);
  this->cleanUp();
  return -1;
}

int DisplayControlTask::startTask(DisplayTaskConfigTAG& displayConfig)
{

  do
  {
    int result = 0;
    if (this->isTaskRunning() != false)
    {
      return 0; // already running
    }

    this->stopTask();

    result = this->data_Manager.prepare(displayConfig.bufferConfig);
    if (result != 0)
    {
      break;
    }

    this->dp_Controller = DisplayControllerFactory::generate(displayConfig.deviceConfig.deviceType);
    if (this->dp_Controller == NULL)
    {
      break;
    }

    result = this->dp_Controller->start(displayConfig.deviceConfig);
    if (result != 0)
    {
      break;
    }

    // Set up tasks
    {
      // self
      this->registerHanldingEventStructSize(sizeof(EventPrepareCompletedTAG));
      this->regEventSize();
      result = TaskManager::setConfig(displayConfig.taskManagerConfig);
      if (result != 0)
      {
        break;
      }

      result = TaskManager::prepare();
      if (result != 0)
      {
        break;
      }
    }

    {
      result = TaskManager::startProcess(displayConfig.taskThreadConfig, true);
      if (result != 0)
      {
        break;
      }
    }

    return result;
  } while (0);

  this->stopTask();
  return -1;
}


BufferDataItem* DisplayControlTask::getBuff(void)
{
  return (BufferDataItem*)this->data_Manager.get();
}

BufferDataManager* DisplayControlTask::dataManager(void)
{
  return &this->data_Manager;
}

void DisplayControlTask::releaseBuff(BufferDataItem* dataItem)
{
  if (dataItem != NULL)
  {
    this->data_Manager.release(dataItem);
  }
}

void DisplayControlTask::stopTask(void)
{
  do
  {
    TaskManager::stopProcess();
    DisplayControllerFactory::release(this->dp_Controller);
    this->data_Manager.releaseAll();
    return;
  } while (0);
  return;
}

void DisplayControlTask::cleanUp(void)
{
  this->stopTask();
}

void DisplayControlTask::proc(void)
{
#ifdef DISPLAY_TASK_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(displayTaskLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[dpCTsk] proc %i", 0);
#endif // DISPLAY_TASK_CONSOLE_DEBUG_ENABLE
  int prepareRet = this->prepare();
  this->reportPrepareResult(prepareRet);
  if (prepareRet != 0)
  {
    this->waitTerminating();
    return;
  }
  EventDataItem* eventData = NULL;
  bool isStop = false;
  while (isStop == false) // A Task shall never return or exit.
  {
    eventData = this->event_Manager.wait(TASK_MANAGER_EVENT_WAIT_TIME_DEFAULT);
    if (eventData == NULL)
    {
      continue;
    }

    switch (eventData->messageId())
    {
      case (int)EventManagerConstant::EventMessageId::TerminateProcess:
      {
        this->isTaskRunning(false);
        isStop = true;
        break;
      }
      default:
        this->onEventHandler(eventData);
        break;
    }
    this->event_Manager.release(eventData);
  }
#ifdef DISPLAY_TASK_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG("[HtCTsk] stop %i", 99);
  CONSOLE_LOG_BUF(displayTaskLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[dpCTsk] proc %i", 99);
#endif // DISPLAY_TASK_CONSOLE_DEBUG_ENABLE
}

void DisplayControlTask::regEventSize(void)
{
  // placeholder for inherited classes
}

void DisplayControlTask::onEventHandler(EventDataItem* eventItem)
{
  // placeholder for inherited classes
}

int DisplayControlTask::prepare(void)
{
  return 0; //do nothing
}

#endif // _CONF_DISPLAY_CONTROL_TASK_ENABLED