#ifndef _TIMER_DEVICE_TIMER_IMPL_WIN_H
#define _TIMER_DEVICE_TIMER_IMPL_WIN_H
/////////////////////////////////////////////////
// INCLUDE
#include "../TimerDeviceTimer.h"
#if (_CONF_TIMER_DEVICE_TIMER_ENABLED) && defined(SYSTEM_WIN_PLATFORM)

#include "task_manager/TaskManager.h"
#include "os_system/SystemCriticalSession.h"
/////////////////////////////////////////////////
// PREPROCESSOR
/////////////////////////////////////////////////
// DEFINE

/////////////////////////////////////////////////
// MARCO

/////////////////////////////////////////////////
// DATA TYPE (TYPEDEF)

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
  : public TaskManager
{
public:
  Impl(void);
  virtual ~Impl(void);
  int                                                           inititialize(TimerDeviceTimerConfigTAG& config);
  bool                                                          isValid(void);
  TimePoint_t                                                   minInterval(void);
  void                                                          proc(void) override;
  TimerReg_t                                                    hwDelay(void);


  void                                                          notifyChange(void);
  TimerCount_t                                                  nowInTick(void);
  int                                                           setAlarmValueByTick(TimerCount_t intervalInTick);

  ////////////////////Function to call from outside ISR///////////////////////
  TimerCount_t                                                  now(void);
  int                                                           start(void);
  void                                                          stop(void);
  void                                                          pause(void);
  int                                                           setAlarmValue(TimerCount_t interval);

  ////////////////////Function to call from inside ISR///////////////////////
  TimerCount_t                                                  nowFromISR(void);
  int                                                           startFromISR(void);
  void                                                          stopFromISR(void);
  void                                                          pauseFromISR(void);
  int                                                           setAlarmValueFromISR(TimerCount_t interval);
public:
  TimerCount_t                                                  hw_Timer_Interval_Tick;
  TimerDeviceTimerCbOnExpired                                   cb_Timer_Fired;
  void*                                                         cb_Args;
  SystemCriticalSession                                         critical_Session;
};
#endif // (_CONF_TIMER_CONTROL_TASK_ENABLED) && defined(SYSTEM_PC_BASED)

#endif // _TIMER_DEVICE_TIMER_IMPL_WIN_H