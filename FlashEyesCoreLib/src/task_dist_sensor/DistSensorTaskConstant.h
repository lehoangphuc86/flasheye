#include "../FlashEyesCoreLib.h"
#ifndef _DIST_SENSOR_TASK_CONSTANT_H
#define _DIST_SENSOR_TASK_CONSTANT_H
/////////////////////////////////////////////////
// INCLUDE
#if (_CONF_DIST_SENSOR_TASK_CONSTANT_ENABLED)
#include "os_system/SystemCommon.h"
#include "task_manager/TaskManagerConstant.h"
/////////////////////////////////////////////////
// PREPROCESSOR

/////////////////////////////////////////////////
// DEFINE
//#########################HW#########################
//-------------------------HW pin---------------------
#define DIST_SENSOR_DEVICE_PIN_MIN                    PIN_NUMER_MIN
#define DIST_SENSOR_DEVICE_PIN_MAX                    PIN_NUMBER_MAX
#define DIST_SENSOR_DEVICE_PIN_INVALID                255

#define DIST_SENSOR_ENGINE_RANGE_MIN                  0 // cm
#define DIST_SENSOR_ENGINE_RANGE_MAX                  10000 // cm


#define DIST_SENSOR_ENGINE_MEASURING_INTERVAL_MIN     50 // ms
#define DIST_SENSOR_ENGINE_MEASURING_INTERVAL_MAX     10000// ms


#define DIST_SENSOR_CTRL_OPCODE_STOP                  0
#define DIST_SENSOR_CTRL_OPCODE_START                 1
#define DIST_SENSOR_CTRL_OPCODE_CHANGE                2
#define DIST_SENSOR_CTRL_OPCODE_MAX                   3
/////////////////////////////////////////////////
// MARCO

/////////////////////////////////////////////////
// GLOBAL VARIABLES

/////////////////////////////////////////////////
// GLOBAL FUNCTIONS

/////////////////////////////////////////////////
// DATA TYPE (TYPEDEF)
typedef uint16_t                                    Dist_t;

// @userArg: the arg set by user
// @sequenceId: seqId
// @distance: measured distance
// @mode:  operation mode
typedef void(*DistSensorCbOnTrg)(void* userArg, Seq_t sequenceId, Dist_t distance, byte mode);
/////////////////////////////////////////////////
// DATA TYPE (ENUM)
typedef enum _distSensorDeviceTypeUN
{
  DistSensorHRCS04Device =0,
  DistSensorDeviceTypeMax
} DistSensorDeviceTypeUN;

typedef enum
{
  DistSensorRequestMode = 0
  , DistSensorRangeMode
  , DistSensorOpModeMax
} DistSensorOpModeUN;
/////////////////////////////////////////////////
// DATA TYPE (STRUCT)

typedef struct _distSensorTriggerRangeConfigTag
{
  Dist_t rangeBegin;
  Dist_t rangeEnd;
  bool isValid(void)
  {
    do
    {
      if ((this->rangeBegin < DIST_SENSOR_ENGINE_RANGE_MIN)
        || (this->rangeBegin > DIST_SENSOR_ENGINE_RANGE_MAX)
        || (this->rangeEnd < DIST_SENSOR_ENGINE_RANGE_MIN)
        || (this->rangeEnd > DIST_SENSOR_ENGINE_RANGE_MAX)
        || (this->rangeEnd < this->rangeBegin)
        )
      {
        break;
      }

      return true;
    } while (0);
    return false;
  }

  bool isValidDist(Dist_t inDist)
  {
    do
    {
      if ((inDist < this->rangeBegin)
        || (inDist > this->rangeEnd)
        )
      {
        break;
      }

      return true;
    } while (0);
    return false;
  }
} DistSensorTriggerRangeConfigTAG;

typedef struct _distSensorHRCS04DeviceConfigTag
{
  byte pin_Trigger;
  byte pin_Echo;
} DistSensorHRCS04DeviceConfigTAG;

typedef struct _distSensorDeviceConfigTag
{
  byte deviceType;
  byte deviceId;
  union 
  {
    DistSensorHRCS04DeviceConfigTAG hrc04;
  } spec;
} DistSensorDeviceConfigTAG;

typedef struct _distSensorCtrlConfigTag
{
  void* trgUserArg;
  DistSensorCbOnTrg trgCb;
  byte opMode;
  uint16_t measuringInterval;
  DistSensorTriggerRangeConfigTAG validRange;
} DistSensorCtrlConfigTAG;

typedef struct _distSensorTaskConfigTag
{
  DistSensorCtrlConfigTAG config;
  DistSensorDeviceConfigTAG deviceConfig;
  TaskManagerConfigTAG taskManagerConfig;
  TaskThreadConfigTAG taskThreadConfig;
} DistSensorTaskConfigTAG;

typedef struct _distSensorResultTag
{
  bool enabled;
  Seq_t sequenceId;
  byte deviceId;
  byte opMode;
  Dist_t distance; // cm
} DistSensorResultTAG;

typedef struct _distSensorControlParamsTag
{
  byte opCode; // 0: stop (not supported), 1: start, 2: change mode (not supported)
  Seq_t sequenceId;
  byte deviceId;
  byte mode; 
} DistSensorControlParamsTAG;

//event
typedef struct _eventDistSensorGetReadyTag
{
  byte reserved;
} EventDistSensorGetReadyTAG;

typedef struct _eventDistSensorControlTag
{
  DistSensorControlParamsTAG reqParams;
} EventDistSensorControlTAG;

typedef struct _eventDistSensorResultTag
{
  DistSensorResultTAG result;
} EventDistSensorResultTAG;

/////////////////////////////////////////////////
// STATIC DATA

/////////////////////////////////////////////////
// STATIC FUNCTIONS
//

/////////////////////////////////////////////////
// EXTERN

/////////////////////////////////////////////////
// CLASS DEFINITION


#endif // _CONF_DIST_SENSOR_TASK_CONSTANT_ENABLED

#endif // _DIST_SENSOR_TASK_CONSTANT_H

