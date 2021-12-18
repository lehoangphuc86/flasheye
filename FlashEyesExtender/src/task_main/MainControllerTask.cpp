/////////////////////////////////////////////////
// INCLUDE
#include "MainControllerTask.h"

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

/* MainControllerTask*/
MainControllerTask::MainControllerTask(void)
  : TaskManager()
{

}
MainControllerTask::~MainControllerTask(void)
{

}

int MainControllerTask::setConfig(void)
{
  do
  {

    int result = 0;

    


    {
      this->registerHanldingEventStructSize(sizeof(EventInputReceivedTAG));
      TaskConfigStruct taskConfigStruct = TaskConfigStruct();
      memset(&taskConfigStruct, 0, sizeof(TaskConfigStruct));
      taskConfigStruct.usingEvent = true;
      taskConfigStruct.eventItemNumber = FLASH_EYE_TASK_EVENT_NO_MAIN_CONTROLLER;
      taskConfigStruct.threadConfig.enabled = true;
      taskConfigStruct.threadConfig.usStackDepth = FLASH_EYE_TASK_MEM_MAIN_CONTROLLER;
      taskConfigStruct.threadConfig.uxPriority = FLASH_EYE_TASK_PRIORITY_MAIN_CONTROLLER;
      result =  TaskManager::setConfig(taskConfigStruct);
      if (result != 0)
      {
        CONSOLE_LOG("%s", "[MainControllerTask] [setConfig] self failed.");
        break;
      }
    }

    return result;
  } while (0);
  return -1;
}

int MainControllerTask::prepare(void)
{
  do
  {
    int result = TaskManager::prepare();
    if (result != 0)
    {
      CONSOLE_LOG("%s", "[MainControllerTask] [prepare] self failed.");
      break;
    }

    return 0;
  } while (0);
  return -1;
}

int MainControllerTask::start(void)
{
  do
  {
    if ( (this->isTaskRunning() != false)
      || (this->threadHandler() != NULL)
      )
    {
      break;
    }

    int result = TaskManager::startProcess();
    if (result != 0)
    {
      CONSOLE_LOG("%s", "[MainControllerTask] [start] self failed.");
      break;
    }

    return 0;
  } while (0);
  return -1;
}

void MainControllerTask::proc(void)
{
  CONSOLE_LOG("%s", "[MainControllerTask] [proc] start");
  this->isTaskRunning(true);
  while (this->isTaskRunning() != false) // A Task shall never return or exit.
  {
    //vTaskDelay(10 / portTICK_PERIOD_MS); // wait for one second
    EventDataItem* eventData = this->event_Manager.wait(TASK_MANAGER_EVENT_WAIT_TIME_DEFAULT);
    if (eventData == NULL)
    {
      CONSOLE_LOG("%s", "[MainControllerTask] Received NULL event");
      continue;
    }
    //CONSOLE_LOG("%s %i", "[MainControllerTask] eventId=", eventData->messageId());
    switch (eventData->messageId())
    {
    case (int)EventManagerConstant::EventMessageId::InputReceived:
      this->onEventInputReceived(eventData->dataLength(), eventData->bufferAddress());
      break;
    case (int)EventManagerConstant::EventMessageId::TerminateProcess:
      this->isTaskRunning(false);
      break;
    default:
      CONSOLE_LOG("%s %i", "[MainControllerTask] Received invalid id=", eventData->messageId());
      break;
    }
    this->event_Manager.release(eventData);
  }

  CONSOLE_LOG("%s", "[MainControllerTask] stopping");
}

void MainControllerTask::onEventInputReceived(unsigned int dataSize, unsigned char* data)
{
  do
  {
    if ((dataSize != sizeof(EventInputReceivedTAG))
      || (data == NULL))
    {
      break;
    }

    EventInputReceivedTAG* eventData = (EventInputReceivedTAG*)data;
    CONSOLE_LOG("cout=%i id=%i time=%i", eventData->count, eventData->id, eventData->time);
    return;
  } while (0);

  CONSOLE_LOG("%s", "[MainControllerTask] [onEventDisplayMessage] Failed to proceed ");
}
