/////////////////////////////////////////////////
// INCLUDE
#include "../TimerDeviceTimer.h"
#if (_CONF_TIMER_DEVICE_TIMER_ENABLED) && defined(SYSTEM_ESP_PLATFORM)

#ifndef _TIMER_DEVICE_TIMER_IMPL_ESP_H
#define _TIMER_DEVICE_TIMER_IMPL_ESP_H
#include "driver/timer.h"
#include "os_system/SystemCriticalSession.h"
/////////////////////////////////////////////////
// PREPROCESSOR
//#define TIMER_DEVICE_TIMER_CONSOLE_DEBUG_ENABLE
/////////////////////////////////////////////////
// DEFINE
#define TIMER_DEVICE_TIMER_ESP_INTERVAL_MIN           300
#define TIMER_DEVICE_TIMER_ESP_OCR_MIN                30 //us
#define TIMER_DEVICE_TIMER_ESP_OCR_PROC_DELAY         50 //us
#define TIMER_DEVICE_TIMER_ESP_OCR_HW_DELAY           (TIMER_DEVICE_TIMER_ESP_OCR_MIN + TIMER_DEVICE_TIMER_ESP_OCR_PROC_DELAY)
/////////////////////////////////////////////////
// MARCO

/////////////////////////////////////////////////
// DATA TYPE (TYPEDEF)
typedef timer_group_t                                 Timer_GrpId_t;
typedef timer_idx_t                                   Timer_Index_t;

/////////////////////////////////////////////////
// DATA TYPE (ENUM)

/////////////////////////////////////////////////
// DATA TYPE (STRUCT)

/////////////////////////////////////////////////
// GLOBAL VARIABLES

/////////////////////////////////////////////////
// GLOBAL FUNCTIONS

/////////////////////////////////////////////////
// STATIC DATA

/////////////////////////////////////////////////
// STATIC FUNCTIONS

/////////////////////////////////////////////////
// EXTERN

/////////////////////////////////////////////////
// CLASS IMPLEMENTAION
/*TimerDeviceTimer::Impl*/
class TimerDeviceTimer::Impl
{
public:
  Impl(void);
  virtual ~Impl(void);
  int                                                           inititialize(TimerDeviceTimerConfigTAG& config);
  bool                                                          isValid(void);
  TimePoint_t                                                   minInterval(void);

  ////////////////////Function to call from outside ISR///////////////////////
public:
  TimerCount_t                                                  now(void);
  int                                                           start(void);
  void                                                          stop(void);
  void                                                          pause(void);
  TimerReg_t                                                    hwDelay(void);
  int                                                           setAlarmValue(TimerCount_t interval);
  void                                                          notifyChange(void);
  TimerCount_t                                                  nowInTick(void);
  int                                                           setAlarmValueByTick(TimerCount_t intervalInTick, bool updateInterval = true);
  TimePoint_t                                                   lastTimePoint(void);
  ////////////////////Function to call from inside ISR///////////////////////
public:
  TimerCount_t                                                  nowFromISR(void);
  int                                                           startFromISR(void);
  void                                                          stopFromISR(void);
  void                                                          pauseFromISR(void);
  int                                                           setAlarmValueFromISR(TimerCount_t interval);
protected:
  TimerCount_t                                                  nowInTickFromISR(void);
  int                                                           setAlarmValueByTickFromISR(TimerCount_t intervalInTick);
  void                                                          cbHwTimerCompareMatched(bool* woken);

private:
  //static void                                                   cbHwTimerCompareMatchedNavigator(void* arg);
  static bool                                                   cbHwTimerCompareMatchedNavigator(void* arg);

protected:
  Timer_GrpId_t                                                 hw_Group_Id; //esp
  Timer_Index_t                                                 hw_Timer_Id;
  double                                                        hw_Tick_Per_Unit; // how many tick per 1 unit of interval (microsec)
  TimerCount_t                                                  hw_Timer_Interval_Tick;
  TimerDeviceTimerCbOnExpired                                   cb_Timer_Fired;
  void*                                                         cb_Args;
  TimePoint_t                                                   last_Time_Point;
  SystemCriticalSession                                         critical_Key;
};

#endif // _TIMER_DEVICE_TIMER_IMPL_ESP_H

#endif // (_CONF_TIMER_CONTROL_TASK_ENABLED) && defined(SYSTEM_PC_BASED)