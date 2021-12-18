#include "../TimerDeviceTimer.h"
#if (_CONF_TIMER_DEVICE_TIMER_ENABLED) && defined (SYSTEM_AVR_PLATFORM)

#ifndef _TIMER_DEVICE_TIMER_IMPL_AVR_H
#define _TIMER_DEVICE_TIMER_IMPL_AVR_H

/////////////////////////////////////////////////
// PREPROCESSOR

/////////////////////////////////////////////////
// INCLUDE
#include "os_system/SystemCriticalSession.h"

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
typedef byte                                          Timer_GrpId_t;
typedef byte                                          Timer_Index_t;
//#ifdef SYSTEM_ESP_PLATFORM
//typedef timer_group_t                             Timer_GrpId_t;
//typedef timer_idx_t                               Timer_Index_t;
//#else
//typedef byte                                      Timer_GrpId_t;
//typedef byte                                      Timer_Index_t;
//#endif // SYSTEM_AVR_PLATFORM
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
// 
// 
///*TimerDeviceTimer::Impl*/
class TimerDeviceTimer::Impl
{
public:
  Impl(void);
  virtual ~Impl(void);
  int                                                           inititialize(TimerDeviceTimerConfigTAG& config);
  bool                                                          isValid(void);
  TimePoint_t                                                   minInterval(void);

  ////////////////////Function to call from outside ISR///////////////////////
  TimerCount_t                                                  now(void);
  int                                                           start(void);
  void                                                          stop(void);
  void                                                          pause(void);
  TimerReg_t                                                    hwDelay(void);
  int                                                           setAlarmValue(TimerCount_t interval);
  void                                                          notifyChange(void);
  TimerCount_t                                                  nowInTick(void);
  int                                                           setAlarmValueByTick(TimerCount_t intervalInTick);

  ////////////////////Function to call from inside ISR///////////////////////
public:
  TimerCount_t                                                  nowFromISR(void);
  int                                                           startFromISR(void);
  void                                                          stopFromISR(void);
  void                                                          pauseFromISR(void);
  int                                                           setAlarmValueFromISR(TimerCount_t interval);
  TimerCount_t                                                  nowInTickFromISR(void);
  int                                                           setAlarmValueByTickFromISR(TimerCount_t intervalInTick);
  void                                                          cbHwTimerOverflow(bool* woken);
  void                                                          cbHwTimerCompareMatched(bool* woken);

private:
  static void                                                   cbHwTimerOverflowNavigator(void* arg, bool* woken);
  static void                                                   cbHwTimerCompareMatchedNavigator(void* arg, bool* woken);

protected:
  Timer_Index_t                                                 hw_Timer_Id;
  double                                                        hw_Tick_Per_Unit; // how many tick per 1 unit of interval (microsec)
  TimerReg_t                                                    hw_Tick_Delay;
  TimerCount_t                                                  hw_Timer_Interval_Tick; // AVR
  TimePoint_t                                                   hw_Alarm_Point_Tick; // AVR
  TimerOVFCount_t                                               hw_Timer_Overflow_Count; // AVR. Support 64bit timer interval in AVR 16 bit counter
  TimerDeviceTimerCbOnExpired                                   cb_Timer_Fired;
  void*                                                         cb_Args;
  SystemCriticalSession                                         critical_Session;
};

#endif // _TIMER_DEVICE_TIMER_IMPL_AVR_H
#endif // _CONF_TIMER_DEVICE_TIMER_ENABLED && not defined (SYSTEM_LINUX_PLATFORM)