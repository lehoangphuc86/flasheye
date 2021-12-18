/////////////////////////////////////////////////
// INCLUDE
#include "DisplayControlTask.h"
#if (_CONF_DISPLAY_CONTROL_TASK_ENABLED)
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

/* DisplayControlTask*/

DisplayControlTask& DisplayControlTask::getInstance(void)
{
  static DisplayControlTask instance;
  return instance;
}

DisplayControlTask::DisplayControlTask(void)
  : TaskManager()
{

}
DisplayControlTask::~DisplayControlTask(void)
{

}

int DisplayControlTask::setConfig(TaskConfigStruct& taskConfig, DisplayDeviceConfig& deviceConfig)
{
  do
  {
    int result = this->display_Engine.setConfig(deviceConfig);
    if (result != 0)
    {
      break;
    }

    this->registerHanldingEventStructSize(sizeof(EventDisplayStatusTAG));
    this->registerHanldingEventStructSize(sizeof(EventDisplayTimeTAG));
    this->registerHanldingEventStructSize(sizeof(EventDisplayControlTAG));
    this->registerHanldingEventStructSize(sizeof(EventDisplayDistanceTAG));

    return TaskManager::setConfig(taskConfig);
  } while (0);
  return -1;
}

int DisplayControlTask::prepare(void)
{
  do
  {
    int result = this->display_Engine.prepare();
    if (result != 0)
    {
      CONSOLE_LOG("%s", "[DisplayControlTask] [prepare] display_Engine failed.");
      break;
    }
    return TaskManager::prepare();
  } while (0);
  return -1;
}

int DisplayControlTask::startProcess(void)
{
  do
  {
    int result = this->display_Engine.start();
    if (result != 0)
    {
      CONSOLE_LOG("%s", "[DisplayControlTask] [start] display_Engine failed.");
      break;
    }
    return TaskManager::startProcess();
  } while (0);
  return -1;
}

void DisplayControlTask::proc(void)
{
  CONSOLE_LOG("%s", "[DisplayControlTask] [proc] start");
  this->isTaskRunning(true);
  while (this->isTaskRunning() != false) // A Task shall never return or exit.
  {
    EventDataItem* eventData = this->event_Manager.wait(TASK_MANAGER_EVENT_WAIT_TIME_DEFAULT);
    if (eventData == NULL)
    {
      CONSOLE_LOG("%s", "[DisplayControlTask] Received NULL event");
      continue;
    }
    CONSOLE_LOG("%s %i", "[DisplayControlTask] eventId=", eventData->messageId());
    switch (eventData->messageId())
    {
    case (int)EventManagerConstant::EventMessageId::DisplayStatus:
      this->onEventDisplayStatus(eventData->dataLength(), eventData->bufferAddress());
      break;
    case (int)EventManagerConstant::EventMessageId::DisplayDistance:
      CONSOLE_LOG("%s", "[dis] wait recv trigger");
      this->onEventDisplayDistance(eventData->dataLength(), eventData->bufferAddress());
      break;
    case (int)EventManagerConstant::EventMessageId::DisplayTime:
      this->onEventDisplayTime(eventData->dataLength(), eventData->bufferAddress());
      break;
    case (int)EventManagerConstant::EventMessageId::DisplayControl:
      this->onEventDisplayControl(eventData->dataLength(), eventData->bufferAddress());
      break;
    case (int)EventManagerConstant::EventMessageId::TerminateProcess:
      this->isTaskRunning(false);
      break;
    default:
      CONSOLE_LOG("%s %i", "[DisplayControlTask] Received invalid id=", eventData->messageId());
      break;
    }
    this->event_Manager.release(eventData);
  }

  CONSOLE_LOG("%s", "[DisplayControlTask] stopping");
}


void DisplayControlTask::onEventDisplayStatus(unsigned int dataSize, unsigned char* data)
{
  do
  {
    if ((dataSize != sizeof(EventDisplayStatusTAG))
      || (data == NULL))
    {
      break;
    }

    EventDisplayStatusTAG* eventData = (EventDisplayStatusTAG*)data;
    this->display_Engine.displayStatus(eventData);
    return;
  } while (0);

  CONSOLE_LOG("%s", "[MainControllerTask] [onEventDisplayMessage] Failed to proceed ");
}

void DisplayControlTask::onEventDisplayDistance(unsigned int dataSize, unsigned char* data)
{
  do
  {
    if ((dataSize != sizeof(EventDisplayDistanceTAG))
      || (data == NULL))
    {
      break;
    }
    CONSOLE_LOG("%s", "[dis] recv trigger");
    EventDisplayDistanceTAG* eventData = (EventDisplayDistanceTAG*)data;
    this->display_Engine.displayDistance(eventData);
    return;
  } while (0);

  CONSOLE_LOG("%s", "[MainControllerTask] [onEventDisplayDistance] Failed to proceed ");
}


void DisplayControlTask::onEventDisplayTime(unsigned int dataSize, unsigned char* data)
{
  do
  {
    if ((dataSize != sizeof(EventDisplayTimeTAG))
      || (data == NULL))
    {
      break;
    }

    EventDisplayTimeTAG* eventData = (EventDisplayTimeTAG*)data;
    this->display_Engine.displayTime(eventData);
    return;
  } while (0);

  CONSOLE_LOG("%s", "[MainControllerTask] [onEventDisplayMessage] Failed to proceed ");
}

void DisplayControlTask::onEventDisplayControl(unsigned int dataSize, unsigned char* data)
{
  do
  {
    if ((dataSize != sizeof(EventDisplayControlTAG))
      || (data == NULL))
    {
      break;
    }

    EventDisplayControlTAG* eventData = (EventDisplayControlTAG*)data;
    this->display_Engine.controlDisplay(eventData);
    return;
  } while (0);

  CONSOLE_LOG("%s", "[MainControllerTask] [onEventDisplayMessage] Failed to proceed ");
}

#endif // _CONF_DISPLAY_CONTROL_TASK_ENABLED