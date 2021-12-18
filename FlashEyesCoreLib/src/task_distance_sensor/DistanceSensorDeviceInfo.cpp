
/////////////////////////////////////////////////
// INCLUDE
#include "DistanceSensorDeviceInfo.h"
#if (_CONF_SENSOR_DEVICE_INFO_ENABLED)
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

/* DistanceSensorDeviceInfo*/
DistanceSensorDeviceInfo::DistanceSensorDeviceInfo(void)
{
  memset(&this->device_Config, 0, sizeof(DistanceSensorDeviceConfig));
}
DistanceSensorDeviceInfo::~DistanceSensorDeviceInfo(void)
{

}

bool DistanceSensorDeviceInfo::isValidConfig(void)
{
  return this->isValidConfig(this->device_Config);
}

byte DistanceSensorDeviceInfo::triggerPin(void)
{
  return (this->device_Config.pin_Trigger);
}

byte DistanceSensorDeviceInfo::echoPin(void)
{
  return (this->device_Config.pin_Echo);
}

DistanceSensorDeviceConfig* DistanceSensorDeviceInfo::deviceConfig(void)
{
  return &this->device_Config;
}

int DistanceSensorDeviceInfo::setConfig(DistanceSensorDeviceConfig& deviceConfig)
{
  do
  {
    if (this->isValidConfig(deviceConfig) == false)
    {
      break;
    }
    memcpy(&this->device_Config, &deviceConfig, sizeof(DistanceSensorDeviceConfig));
    return 0;
  } while (0);
  return -1;
}

int DistanceSensorDeviceInfo::prepare(void)
{
  return 0;
}

bool DistanceSensorDeviceInfo::isValidConfig(DistanceSensorDeviceConfig& deviceConfig)
{
  do
  {
    if ( (deviceConfig.pin_Trigger < DISTANCE_SENSOR_DEVICE_PIN_MIN)
      || (deviceConfig.pin_Trigger > DISTANCE_SENSOR_DEVICE_PIN_MAX)
      || (deviceConfig.pin_Echo < DISTANCE_SENSOR_DEVICE_PIN_MIN)
      || (deviceConfig.pin_Echo > DISTANCE_SENSOR_DEVICE_PIN_MAX)
      )
    {
      break;
    }

    return true;
  } while (0);

  return false;
}

#endif // _CONF_SENSOR_DEVICE_INFO_ENABLED