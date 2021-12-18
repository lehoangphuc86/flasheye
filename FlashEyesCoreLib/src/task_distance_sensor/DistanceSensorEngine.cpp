
/////////////////////////////////////////////////
// INCLUDE
#include "DistanceSensorEngine.h"
#if (_CONF_SENSOR_ENGINE_ENABLED)
#include "../event_manager/EventManager.h"
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

/* DistanceSensorEngine*/
DistanceSensorEngine::DistanceSensorEngine(void)
{
  memset(&this->engine_Config, 0, sizeof(DistanceSensorEngineConfig));
}
DistanceSensorEngine::~DistanceSensorEngine(void)
{

}

bool DistanceSensorEngine::isTaskValid(void)
{
  return this->device_Controller.isTaskValid();
}

bool DistanceSensorEngine::isTriggerEnabled(void)
{
  return (this->engine_Config.triggerMode == (byte) DistanceSenosrTriggerMode::InvalidDistanceSenosrTriggerMode? false : true);
}

int DistanceSensorEngine::measuringInterval(void)
{
  return (this->engine_Config.measuringInterval);
}

int DistanceSensorEngine::setConfig(DistanceSensorEngineConfig& engineConfig, DistanceSensorDeviceConfig& deviceConfig)
{
  do
  {
    if (this->isTaskValid() != false)
    {
      break;
    }

    if ((engineConfig.triggerMode != (byte)DistanceSenosrTriggerMode::InvalidDistanceSenosrTriggerMode)
        && 
        (
          (engineConfig.measuringInterval < DISTANCE_SENSOR_ENGINE_MEASURING_INTERVAL_MIN)
          || (engineConfig.measuringInterval > DISTANCE_SENSOR_ENGINE_MEASURING_INTERVAL_MAX)
        )
      )
    {
      break;
    }

    if ( 
        (engineConfig.triggerMode == (byte)DistanceSenosrTriggerMode::DistanceSenosrTriggerByRange)
        &&
        ( (engineConfig.triggerRange.rangeBegin > engineConfig.triggerRange.rangeEnd)
          || (engineConfig.triggerRange.rangeBegin < DISTANCE_SENSOR_ENGINE_RANGE_MIN)
          || (engineConfig.triggerRange.rangeBegin > DISTANCE_SENSOR_ENGINE_RANGE_MAX)
          || (engineConfig.triggerRange.rangeEnd < DISTANCE_SENSOR_ENGINE_RANGE_MIN)
          || (engineConfig.triggerRange.rangeEnd > DISTANCE_SENSOR_ENGINE_RANGE_MAX)
        )
      )
    {
      break;
    }

    if ( (engineConfig.triggerMode!= (byte)DistanceSenosrTriggerMode::InvalidDistanceSenosrTriggerMode)
      && (engineConfig.targetTask == NULL)
      )
    {
      break;
    }

    memcpy(&this->engine_Config, &engineConfig, sizeof(DistanceSensorEngineConfig));

    return this->device_Controller.setConfig(deviceConfig);
  } while (0);
  return -1;
}

int DistanceSensorEngine::prepare(void)
{
  return this->device_Controller.prepare();
}

int DistanceSensorEngine::start(void)
{
  return this->device_Controller.start();
}

void DistanceSensorEngine::measureDistanceTimeDriven(void)
{
  do
  {
    if (this->isTriggerEnabled() == false)
    {
      break;
    }

    unsigned long measuredTime = millis();
    double distance = this->device_Controller.measureDistanceCm(this->engine_Config.defaultSensorId);
    int distanceInInteger = (int)distance;
    if ( (this->engine_Config.triggerMode == (byte)DistanceSenosrTriggerMode::DistanceSenosrTriggerByRange)
      && (
           (distanceInInteger < this->engine_Config.triggerRange.rangeBegin)
        || (distanceInInteger > this->engine_Config.triggerRange.rangeEnd)
        )
      )
    {
      break;
    }
    EventDistanceTriggeredTAG triggerDataEvent = EventDistanceTriggeredTAG();
    triggerDataEvent.distance = distance;
    triggerDataEvent.measuredTime = measuredTime;
    triggerDataEvent.sensorId = this->engine_Config.defaultSensorId;
    EventManager* targetTask = (EventManager*)this->engine_Config.targetTask;
    targetTask->notify(
      (int)EventManagerConstant::EventMessageId::DistanceTriggered
      , sizeof(EventDistanceTriggeredTAG)
      , (unsigned char*)&triggerDataEvent
    );

    return;
  } while (0);
  return;
}

void DistanceSensorEngine::measureDistanceEventDriven(EventDistanceRequestTAG* data)
{
  do
  {
    if ( (data == NULL)
      || (data->requester == NULL)
      )
    {
      break;
    }
    unsigned long measuredTime = millis();
    double distance = this->device_Controller.measureDistanceCm(data->sensorId);
    EventDistanceDataReceivedTAG disanceRevEventData = EventDistanceDataReceivedTAG();
    disanceRevEventData.distance = distance;
    disanceRevEventData.requestedTime = data->requestedTime;
    disanceRevEventData.measuredTime = measuredTime;
    disanceRevEventData.sensorId = data->sensorId;
    EventManager* requester = (EventManager*)data->requester;
    requester->notify(
      (int)EventManagerConstant::EventMessageId::DistanceReceived
      , sizeof(EventDistanceDataReceivedTAG)
      , (unsigned char*)&disanceRevEventData
    );

    return;
  } while (0);
  return;
}

#endif // _CONF_SENSOR_ENGINE_ENABLED