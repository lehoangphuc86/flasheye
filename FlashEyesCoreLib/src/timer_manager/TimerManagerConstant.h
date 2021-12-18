#ifndef _TIMER_MANAGER_CONSTANT_H
#define _TIMER_MANAGER_CONSTANT_H
//
/**
* For linux: plesae include -lrt as library
**/

/////////////////////////////////////////////////
// INCLUDE
#include "../FlashEyesCoreLibConfig.h"
#if (_CONF_TIMER_MANAGER_CONSTANT_ENABLED)

#include "../os_system/SystemCommon.h"
/////////////////////////////////////////////////
// PREPROCESSOR

/////////////////////////////////////////////////
// DEFINE
#ifdef SYSTEM_ARDUINO_BASED
#define TIMER_MANAGER_TIMER_COUNT_MAX           16
#else // SYSTEM_ARDUINO_BASED
#define TIMER_MANAGER_TIMER_COUNT_MAX           255
#endif // SYSTEM_ARDUINO_BASED

#define TIMER_MANAGER_TIMER_ID_MIN              1
#define TIMER_MANAGER_TIMER_ID_INVALID          0
#define TIMER_MANAGER_TIMER_ID_MAX              255


//#define TIMER_MANAGER_HANDLING_MODE_CALLBACK    0
//#define TIMER_MANAGER_HANDLING_MODE_EVENT       1


#define TIMER_CONTROL_TASK_IDLE_SNAP_TIME       1000000
#define TIMER_DEVICE_TIMER_HANDLE_TIME_MIN      100 // us
#define TIMER_DEVICE_TIMER_HANDLE_DELAY_TIME    500 // us
#define TIMER_DEVICE_TIMER_INTERVAL_DEFAULT     1000000 // us
#define TIMER_DEVICE_TIMER_INTERVAL_MAX         UINT64_MAX  // 64 
#define TIMER_DEVICE_TIMER_SECOND_2_UNIT        1000000 // 1sc = 1000 us
#define TIMER_DEVICE_TIMER_UNIT_2_NANO          1000 // 1micro = 1000 nano
#define TIMER_DEVICE_TIMER_MS_2_UNIT            1000 // 1ms = 1000 micro
#define TIMER_DEVICE_TIMER_TICK_ACCURACY        50

#define TIMER_DEVICE_TIMER_HW_GROUP_ID_DEFAULT  0
#define TIMER_DEVICE_TIMER_HW_INDEX_ID_DEFAULT  1 // compatible both esp and arduino

#define TIMER_DEVICE_TIMER_HW_DEVIDER_DEFAULT   1024
/////////////////////////////////////////////////
// MARCO
#define TIMER_MANAGER_CURRENT_TIMEPOINT()       SYSTEM_TIME_MICROS()
#define TIMER_MANAGER_DEVICE_TIMEPOINT_MAX      UINT32_MAX
#define TIMER_MANAGER_INTERVAL_TO_NANOSECOND(x) (x*1000)
/////////////////////////////////////////////////
// GLOBAL VARIABLES

/////////////////////////////////////////////////
// GLOBAL FUNCTIONS

/////////////////////////////////////////////////
// DATA TYPE (TYPEDEF)
// @param1: timer Id
// @param2: extra arg
typedef void(*TimerManagerCbOnExpired)(TimerId_t timerId, void* extraArg, bool* woken);

// @param1: extra arg
// @param2: update current interval. set 0 to keep current interval.
// @return: none
typedef void(*TimerDeviceTimerCbOnExpired)(void* extraArg, TimePoint_t& nextInterval, bool* woken);

typedef void(*TimerDeviceHWCbOnExpired)(void* extraArg, bool* woken);

/////////////////////////////////////////////////
// DATA TYPE (ENUM)

/////////////////////////////////////////////////
// DATA TYPE (STRUCT)

typedef struct _timerConfigTAG
{
  bool enabled            : 1; // 0: disabled, 1: enabled
  bool oneShot            : 1; // 0: repeated, 1: oneshot 
  byte reserved           : 6; // reserved
  
  TimePoint_t             interval; // timer expired from now() + interval. Unit: micro second.
  TimePoint_t             expiredPoint; // timer expired at exactly/ after expiredPoint
  TimerManagerCbOnExpired handler; // callback or event manager
  void*                   extraArgument; //  arg to pass to the handler
  TimerHanlder_t          sysTimerHandler;
  void*                   parent; // parent who owns this struct 
  TimerId_t               timerId; // set TIMER_MANAGER_TIMER_ID_INVALID to auto generate (recommended)

  bool isValid(void)
  {
    do
    {
      if ((this->enabled == 0)
        || (this->handler == NULL)
        )
      {
        break;
      }

      if (this->interval <= 0)
      {
        break;
      }

      return true;
    } while (0);
    return false;
  }
} TimerConfigTAG;

typedef _timerConfigTAG TimerInfoTAG;

// event
typedef struct _eventTimerPauseTAG
{
  byte reserved;
} EventTimerPauseTAG;

typedef struct _eventTimerSetAlarmValueTAG
{
  TimePoint_t interval;
} EventTimerSetAlarmValueTAG;

//typedef struct _eventTimerStartTAG
//{
//  TimerInfoTAG timerInfo;
//} EventTimerStartTAG;
//
//typedef struct _eventTimerStopTAG
//{
//  TimerId_t timerId;
//} EventTimerStopTAG;

typedef struct _eventTimerFiredTAG
{
  TimerId_t timerId;
  void* extraArg;
} EventTimerFiredTAG;
/////////////////////////////////////////////////
// STATIC DATA

/////////////////////////////////////////////////
// STATIC FUNCTIONS

/////////////////////////////////////////////////
// EXTERN

/////////////////////////////////////////////////
// CLASS DEFINITION
#endif // _CONF_TIMER_MANAGER_CONSTANT_ENABLED

#endif // _TIMER_MANAGER_CONSTANT_H

