#ifndef _DISTANCE_SENSOR_TASK_COMMON_H
#define _DISTANCE_SENSOR_TASK_COMMON_H

/////////////////////////////////////////////////
// INCLUDE
#include "../FlashEyesCoreLibConfig.h"
#if (_CONF_SENSOR_TASK_COMMON_ENABLED)
#include "../os_system/SystemCommon.h"
/////////////////////////////////////////////////
// PREPROCESSOR

/////////////////////////////////////////////////
// DEFINE
#define DISTANCE_SENSOR_DEVICE_PIN_MIN          PIN_NUMER_MIN
#define DISTANCE_SENSOR_DEVICE_PIN_MAX          PIN_NUMBER_MAX


#define DISTANCE_SENSOR_ENGINE_RANGE_MIN        0 // cm
#define DISTANCE_SENSOR_ENGINE_RANGE_MAX        10000 // cm


#define DISTANCE_SENSOR_ENGINE_MEASURING_INTERVAL_MIN 50 // ms
#define DISTANCE_SENSOR_ENGINE_MEASURING_INTERVAL_MAX 10000// ms
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
//typedef enum
//{
//  InvalidDistanceMeasuringMode =0
//  , DistanceTimeDriven
//  , DistanceEventDriven
//} DistanceMeasuringMode;
// 
typedef enum
{
  InvalidDistanceSenosrTriggerMode = 0
  , DistanceSenosrTriggerByRange
} DistanceSenosrTriggerMode;
/////////////////////////////////////////////////
// DATA TYPE (STRUCT)
typedef struct
{
  byte id;
  byte pin_Trigger;
  byte pin_Echo;
} DistanceSensorDeviceConfig;

typedef struct
{
  int rangeBegin;
  int rangeEnd;
} DistanceSensorTriggerRangeConfig;

typedef struct
{
  byte triggerMode; // DistanceSenosrTriggerMode
  DistanceSensorTriggerRangeConfig triggerRange;
  byte defaultSensorId;
  void* targetTask;
  int measuringInterval; // for continous measuring
} DistanceSensorEngineConfig;

// event
typedef struct
{
  byte sensorId;
  unsigned long requestedTime;
  unsigned long measuredTime;
  double distance;
} EventDistanceDataReceivedTAG;

typedef struct
{
  void* requester;
  byte sensorId;
  unsigned long requestedTime;
} EventDistanceRequestTAG;

typedef struct
{
  byte sensorId;
  unsigned long measuredTime;
  double distance;
} EventDistanceTriggeredTAG;

/////////////////////////////////////////////////
// STATIC DATA

/////////////////////////////////////////////////
// STATIC FUNCTIONS
//

/////////////////////////////////////////////////
// EXTERN

/////////////////////////////////////////////////
// CLASS DEFINITION



#endif // _DISTANCE_SENSOR_TASK_COMMON_H

#endif // _CONF_SENSOR_TASK_COMMON_ENABLED