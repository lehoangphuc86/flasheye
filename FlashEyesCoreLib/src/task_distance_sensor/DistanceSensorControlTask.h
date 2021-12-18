#include "DistanceSensorTaskCommon.h"

#if (_CONF_SENSOR_CONTROL_TASK_ENABLED)

#ifndef _DISTANCE_SENSOR_CONTROL_TASK_H
#define _DISTANCE_SENSOR_CONTROL_TASK_H

/////////////////////////////////////////////////
// INCLUDE
#include "../task_manager/TaskManager.h"
#include "DistanceSensorEngine.h"

#if (!_CONF_TASK_MANAGER_ENABLED)
#error Task manager is required
#endif // _CONF_TASK_MANAGER_ENABLED

#if (!_CONF_SENSOR_ENGINE_ENABLED)
#error Sensor engine is required
#endif // _CONF_SENSOR_ENGINE_ENABLED
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

/*DistanceSensorControlTask*/

class DistanceSensorControlTask
  : public TaskManager
{
public:
  DistanceSensorControlTask(void);
  virtual ~DistanceSensorControlTask(void);
  virtual int                                                   setConfig(
    TaskConfigStruct& taskConfig
    , DistanceSensorEngineConfig& engineConfig
    , DistanceSensorDeviceConfig& deviceConfig);
  virtual int                                                   prepare(void);
  virtual int                                                   start(void);

protected:
  virtual void                                                  proc(void) override;
  virtual void                                                  onEventDistanceRequest(unsigned int dataSize, unsigned char* data);
protected:
  // Time between 2 measuring time. milisecond.
  // Set -1 to disable continuous measuring
  //int                                                           measuring_Interval;
  DistanceSensorEngine                                          distance_Sensor_Engine;
};

#endif // _DISTANCE_SENSOR_CONTROL_TASK_H

#endif // _CONF_SENSOR_CONTROL_TASK_ENABLED