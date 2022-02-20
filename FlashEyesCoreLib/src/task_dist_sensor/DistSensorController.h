#include "DistSensorTaskConstant.h"

#if (_CONF_DIST_SENSOR_CONTROLLER_ENABLED)

#ifndef _DIST_SENSOR_CONTROLLER_H
#define _DIST_SENSOR_CONTROLLER_H

/////////////////////////////////////////////////
// INCLUDE

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

/*DistSensorController*/

class DistSensorController
{
public:
  DistSensorController(byte deviceType);
  virtual ~DistSensorController(void);
  virtual bool                                                  isValid(void);
  byte                                                          deviceType(void);
  virtual int                                                   start(DistSensorDeviceConfigTAG& deviceConfig) = 0;
  virtual void                                                  stop(void) = 0;
  // controlling functions
  virtual int                                                   measureDistanceCm(byte deviceId, double& outDistance) = 0;
protected:
  byte                                                          device_Type;
  byte                                                          device_Id;
};

#endif // _DIST_SENSOR_CONTROLLER_H

#endif // _CONF_DIST_SENSOR_CONTROLLER_ENABLED