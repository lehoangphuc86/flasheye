#include "DistanceSensorTaskCommon.h"

#if (_CONF_SENSOR_DEVICE_INFO_ENABLED)

#ifndef _DISTANCE_SENSOR_DEVICE_INFO_H
#define _DISTANCE_SENSOR_DEVICE_INFO_H

/////////////////////////////////////////////////
// INCLUDE
#include "../os_system/SystemCommon.h"
#include "DistanceSensorTaskCommon.h"

#if (!_CONF_SENSOR_TASK_COMMON_ENABLED)
#error Sensor task common is required
#endif // _CONF_SENSOR_TASK_COMMON_ENABLED
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

/*DistanceSensorDeviceInfo*/

class DistanceSensorDeviceInfo
{
public:
  DistanceSensorDeviceInfo(void);
  virtual ~DistanceSensorDeviceInfo(void);
  bool                                                          isValidConfig(void);
  byte                                                          triggerPin(void);
  byte                                                          echoPin(void);
  virtual DistanceSensorDeviceConfig*                           deviceConfig(void);
  virtual int                                                   setConfig(DistanceSensorDeviceConfig& deviceConfig);
  virtual int                                                   prepare(void);

  
protected:
  bool                                                          isValidConfig(DistanceSensorDeviceConfig& deviceConfig);

protected:
  DistanceSensorDeviceConfig                                    device_Config;  
};

#endif // _DISTANCE_SENSOR_DEVICE_INFO_H

#endif // _CONF_SENSOR_DEVICE_INFO_ENABLED