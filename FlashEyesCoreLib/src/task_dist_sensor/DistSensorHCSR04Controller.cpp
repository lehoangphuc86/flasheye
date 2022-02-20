/////////////////////////////////////////////////
// INCLUDE
#include "DistSensorHCSR04Controller.h"
#if (_CONF_DIST_SENSOR_HCRS04_CONTROLLER_ENABLED)
#include "os_system/SystemCommon.h"
/////////////////////////////////////////////////
// PREPROCESSOR

//#define HCSR04_CONTROLLER_CONSOLE_DEBUG_ENABLE
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
#ifdef HCSR04_CONTROLLER_CONSOLE_DEBUG_ENABLE
char hcsr04ControllerLogBuf[SYSTEM_CONSOLE_OUT_BUF_LEN];
#endif // HCSR04_CONTROLLER_CONSOLE_DEBUG_ENABLE
/////////////////////////////////////////////////
// STATIC FUNCTIONS

/////////////////////////////////////////////////
// EXTERN

/////////////////////////////////////////////////
// CLASS IMPLEMENTAION
/* DistSensorHCSR04Controller*/
DistSensorHCSR04Controller::DistSensorHCSR04Controller(void)
  : DistSensorController(DistSensorDeviceTypeUN::DistSensorHRCS04Device)
  , sensor_Controller(NULL)
{

}

DistSensorHCSR04Controller::~DistSensorHCSR04Controller(void)
{
  this->stop();
}

bool DistSensorHCSR04Controller::isValid(void)
{
  do
  {
    if (DistSensorController::isValid() == false)
    {
      break;
    }

    if (this->sensor_Controller == NULL)
    {
      break;
    }

    return true;
  } while (0);
  return false;
}

int DistSensorHCSR04Controller::start(DistSensorDeviceConfigTAG& deviceConfig)
{
  int ret = 0;
#ifdef HCSR04_CONTROLLER_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(hcsr04ControllerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[ss04] sta %i", 0);
#endif // HCSR04_CONTROLLER_CONSOLE_DEBUG_ENABLE
  do
  {
    if (this->isValid() != false)
    {
      return 0; //already
    }
    this->stop();
    if ( (deviceConfig.spec.hrc04.pin_Trigger < DIST_SENSOR_DEVICE_PIN_MIN)
      || (deviceConfig.spec.hrc04.pin_Trigger > DIST_SENSOR_DEVICE_PIN_MAX)
      || (deviceConfig.spec.hrc04.pin_Echo < DIST_SENSOR_DEVICE_PIN_MIN)
      || (deviceConfig.spec.hrc04.pin_Echo > DIST_SENSOR_DEVICE_PIN_MAX)
      )
    {
      break;
    }

    this->device_Id = deviceConfig.deviceId;

    this->sensor_Controller = new HCSR04(deviceConfig.spec.hrc04.pin_Trigger, deviceConfig.spec.hrc04.pin_Echo);
    if (this->sensor_Controller == NULL)
    {
      break;
    }
#ifdef HCSR04_CONTROLLER_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(hcsr04ControllerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[ss04] sta %i", 99);
#endif // HCSR04_CONTROLLER_CONSOLE_DEBUG_ENABLE
    return 0;
  } while (0);
  this->stop();
#ifdef HCSR04_CONTROLLER_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(hcsr04ControllerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[ss04] sta %i", -99);
#endif // HCSR04_CONTROLLER_CONSOLE_DEBUG_ENABLE
  return -1;
}

void DistSensorHCSR04Controller::stop(void)
{
  if (this->sensor_Controller != NULL)
  {
    delete this->sensor_Controller;
  }

  this->sensor_Controller = NULL;
}

int DistSensorHCSR04Controller::measureDistanceCm(byte deviceId, double& outDistance)
{
  int ret = 0;
  outDistance = 0.0;
  do
  {
//#ifdef HCSR04_CONTROLLER_CONSOLE_DEBUG_ENABLE
//    CONSOLE_LOG_BUF(hcsr04ControllerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[ss04] scsta %i", 0);
//#endif // HCSR04_CONTROLLER_CONSOLE_DEBUG_ENABLE
    if (this->isValid() == false)
    {
      break;
    }

    outDistance = this->sensor_Controller->dist(deviceId);
//#ifdef HCSR04_CONTROLLER_CONSOLE_DEBUG_ENABLE
//    CONSOLE_LOG_BUF(hcsr04ControllerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[ss04] scsta %i", 99);
//#endif // HCSR04_CONTROLLER_CONSOLE_DEBUG_ENABLE
    return 0;
  } while (0);
//#ifdef HCSR04_CONTROLLER_CONSOLE_DEBUG_ENABLE
//  CONSOLE_LOG_BUF(hcsr04ControllerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[ss04] scsta %i", -99);
//#endif // HCSR04_CONTROLLER_CONSOLE_DEBUG_ENABLE
  return -1;
}

#endif // _CONF_DIST_SENSOR_HCRS04_CONTROLLER_ENABLED