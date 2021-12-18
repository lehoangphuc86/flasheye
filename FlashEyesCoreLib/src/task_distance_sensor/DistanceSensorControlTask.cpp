/////////////////////////////////////////////////
// INCLUDE
#include "DistanceSensorControlTask.h"
#if (_CONF_SENSOR_CONTROL_TASK_ENABLED)
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
/* DistanceSensorControlTask*/

DistanceSensorControlTask::DistanceSensorControlTask(void)
  : TaskManager()
{

}
DistanceSensorControlTask::~DistanceSensorControlTask(void)
{

}

int DistanceSensorControlTask::setConfig(TaskConfigStruct& taskConfig, DistanceSensorEngineConfig& engineConfig, DistanceSensorDeviceConfig& deviceConfig)
{
  do
  {
    int result = this->distance_Sensor_Engine.setConfig(engineConfig, deviceConfig);
    if (result != 0)
    {
      break;
    }

    this->registerHanldingEventStructSize(sizeof(EventDistanceRequestTAG));

    return TaskManager::setConfig(taskConfig);
  } while (0);
  return -1;
}

int DistanceSensorControlTask::prepare(void)
{
  do
  {
    int result = this->distance_Sensor_Engine.prepare();
    if (result != 0)
    {
      CONSOLE_LOG("%s", "[DistanceSensorControlTask] [prepare] display_Engine failed.");
      break;
    }
    return TaskManager::prepare();
  } while (0);
  return -1;
}

int DistanceSensorControlTask::start(void)
{
  do
  {
    int result = this->distance_Sensor_Engine.start();
    if (result != 0)
    {
      CONSOLE_LOG("%s", "[DistanceSensorControlTask] [start] display_Engine failed.");
      break;
    }
    return TaskManager::startProcess();
  } while (0);
  return -1;
}

void DistanceSensorControlTask::proc(void)
{
  CONSOLE_LOG("%s", "[DistanceSensorControlTask] [proc] start");
  this->isTaskRunning(true);

  unsigned int timeToWait = (this->distance_Sensor_Engine.isTriggerEnabled()== false ? TASK_MANAGER_EVENT_WAIT_TIME_DEFAULT : this->distance_Sensor_Engine.measuringInterval());
  while (this->isTaskRunning() != false) // A Task shall never return or exit.
  {
    EventDataItem* eventData = this->event_Manager.wait(timeToWait);
    if (eventData == NULL)
    {
      // no event received, start measuring as continuos measuring
      this->distance_Sensor_Engine.measureDistanceTimeDriven();
      continue;
    }
    
    switch (eventData->messageId())
    {
    case (int)EventManagerConstant::EventMessageId::DistanceRequest:
      this->onEventDistanceRequest(eventData->dataLength(), eventData->bufferAddress());
      break;
    case (int)EventManagerConstant::EventMessageId::TerminateProcess:
      this->isTaskRunning(false);
      break;
    default:
      CONSOLE_LOG("%s %i", "[DistanceSensorControlTask] Received invalid id=", eventData->messageId());
      break;
    }
    this->event_Manager.release(eventData);
  }

  CONSOLE_LOG("%s", "[DistanceSensorControlTask] stopping");
}


void DistanceSensorControlTask::onEventDistanceRequest(unsigned int dataSize, unsigned char* data)
{
  do
  {
    if ((dataSize != sizeof(EventDistanceRequestTAG))
      || (data == NULL))
    {
      break;
    }

    this->distance_Sensor_Engine.measureDistanceEventDriven((EventDistanceRequestTAG*)data);
    return;
  } while (0);

  CONSOLE_LOG("%s", "[MainControllerTask] [onEventDisplayMessage] Failed to proceed ");
}

#endif // _CONF_SENSOR_CONTROL_TASK_ENABLED