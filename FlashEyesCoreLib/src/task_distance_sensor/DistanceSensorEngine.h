#include "DistanceSensorTaskCommon.h"

#if (_CONF_SENSOR_ENGINE_ENABLED)

#ifndef _DISTANCE_SENSOR_ENGINE_H
#define _DISTANCE_SENSOR_ENGINE_H

/////////////////////////////////////////////////
// INCLUDE
#include "DistanceSensorDeviceController.h"

#if (!_CONF_SENSOR_DEVICE_CONTROLLER_ENABLED)
#error Sensor device controller is required
#endif // _CONF_FREE_RTOS_ENABLED
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

/*DistanceSensorEngine*/

class DistanceSensorEngine
{
public:
  DistanceSensorEngine(void);
  virtual ~DistanceSensorEngine(void);
  virtual bool                                                  isTaskValid(void);
  virtual bool                                                  isTriggerEnabled(void);
  virtual int                                                   measuringInterval(void);
  virtual int                                                   setConfig(DistanceSensorEngineConfig& engineConfig,DistanceSensorDeviceConfig& deviceConfig);
  virtual int                                                   prepare(void);
  virtual int                                                   start(void);
  virtual void                                                  measureDistanceTimeDriven(void);
  virtual void                                                  measureDistanceEventDriven(EventDistanceRequestTAG* data);
  
protected:
  DistanceSensorDeviceController                                device_Controller;
  DistanceSensorEngineConfig                                    engine_Config;
};

#endif // _DISTANCE_SENSOR_ENGINE_H

#endif // _CONF_SENSOR_ENGINE_ENABLED