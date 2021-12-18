

/////////////////////////////////////////////////
// INCLUDE
#include "DistanceSensorDeviceController.h"
#if (_CONF_SENSOR_DEVICE_CONTROLLER_ENABLED)
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

/* DistanceSensorDeviceController*/
DistanceSensorDeviceController::DistanceSensorDeviceController(void)
#ifdef SYSTEM_AVR_PLATFORM
  : sensor_Controller(NULL)
#endif // SYSTEM_AVR_PLATFORM
{

}
DistanceSensorDeviceController::~DistanceSensorDeviceController(void)
{

}

int DistanceSensorDeviceController::setConfig(DistanceSensorDeviceConfig& deviceConfig)
{
  do
  {
    if (this->isTaskValid() != false)
    {
      break;
    }

    if (this->device_Info.setConfig(deviceConfig) != 0)
    {
      break;
    }
    
    return 0;
  } while (0);
  return -1;
}

int DistanceSensorDeviceController::prepare(void)
{
  do
  {
    if (this->isTaskValid() != false)
    {
      break;
    }
    return this->device_Info.prepare();
  } while (0);
  return -1;
  
}

int DistanceSensorDeviceController::start(void)
{
  do
  {
    if (this->isTaskValid() != false)
    {
      break;
    }

    DistanceSensorDeviceConfig* deviceConfig = this->device_Info.deviceConfig();
#ifdef SYSTEM_AVR_PLATFORM
    this->sensor_Controller = new HCSR04(deviceConfig->pin_Trigger, deviceConfig->pin_Echo);
#endif // SYSTEM_AVR_PLATFORM
    return 0;
  } while (0);
  return -1;
}

bool DistanceSensorDeviceController::isTaskValid(void)
{
#ifdef SYSTEM_AVR_PLATFORM
  return  (this->sensor_Controller == NULL ? false : true);
#else 
  return false;
#endif // SYSTEM_AVR_PLATFORM
}

// controlling functions
double DistanceSensorDeviceController::measureDistanceCm(byte sensorId)
{
#ifdef SYSTEM_AVR_PLATFORM
  this->sensor_Controller->dist(sensorId);
#endif // SYSTEM_AVR_PLATFORM
}

#endif // _CONF_SENSOR_DEVICE_CONTROLLER_ENABLED