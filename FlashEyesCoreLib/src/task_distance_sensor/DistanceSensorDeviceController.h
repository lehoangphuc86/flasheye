#include "DistanceSensorTaskCommon.h"

#if (_CONF_SENSOR_DEVICE_CONTROLLER_ENABLED)

#ifndef _DISTANCE_SENSOR_DEVICE_CONTROLLER_H
#define _DISTANCE_SENSOR_DEVICE_CONTROLLER_H

/////////////////////////////////////////////////
// INCLUDE
#include "DistanceSensorDeviceInfo.h"
#ifdef SYSTEM_AVR_PLATFORM
#include <HCSR04.h>
#endif // SYSTEM_AVR_PLATFORM

#if (!_CONF_SENSOR_DEVICE_INFO_ENABLED)
#error Sensor device infor is required
#endif // _CONF_SENSOR_DEVICE_INFO_ENABLED
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
// DATA TYPE (TYPEDEF)

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
// CLASS DEFINITION

/*DistanceSensorDeviceController*/

class DistanceSensorDeviceController
{
public:
  DistanceSensorDeviceController(void);
  virtual ~DistanceSensorDeviceController(void);
  virtual int                                                   setConfig(DistanceSensorDeviceConfig& deviceConfig);
  virtual int                                                   prepare(void);
  virtual int                                                   start(void);
  virtual bool                                                  isTaskValid(void);

  // controlling functions
  virtual double                                                measureDistanceCm(byte sensorId);

protected:
  DistanceSensorDeviceInfo                                      device_Info;
#ifdef SYSTEM_AVR_PLATFORM
  HCSR04*                                                       sensor_Controller; // for lcd device only
#endif // SYSTEM_AVR_PLATFORM
};

#endif // _DISTANCE_SENSOR_DEVICE_CONTROLLER_H

#endif // _CONF_SENSOR_DEVICE_CONTROLLER_ENABLED