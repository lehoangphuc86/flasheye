
#include "../TimerControlTask.h"
#ifndef _TIMER_CONTROL_TASK_IMPL_STD_H
#define _TIMER_CONTROL_TASK_IMPL_STD_H
#if (_CONF_TIMER_CONTROL_TASK_ENABLED) 
/////////////////////////////////////////////////
// INCLUDE
#include "os_system/SystemCriticalSession.h"
#include "../TimerDeviceTimer.h"
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

/*TimerControlTask::Impl*/
class TimerControlTask::Impl
{
public:
  Impl(void);
  ~Impl(void);
  bool                                                          isRunning(void);
  int                                                           start(TimerControlTaskConfigTAG& config);
  void                                                          stop(void);
  TimerId_t                                                     startTimer(TimerConfigTAG& timerConfig);
  void                                                          cleanUp(void);
  TimerId_t                                                     nextTimerId(void);

  ////////////////////Function to call from outside ISR///////////////////////
  TimePoint_t                                                   now(void);
  void                                                          stopTimer(TimerId_t timerId);

  ////////////////////Function to call from inside ISR///////////////////////
  TimePoint_t                                                   nowFromISR(void);
  void                                                          stopTimerFromISR(TimerId_t timerId, bool* woken);
  void                                                          cbTimerHandler(TimePoint_t& nextInterval, bool* woken);
  static void                                                   cbTimerHandlerNavigator(void* arg, TimePoint_t& nextInterval, bool* woken);

public:
  TimerInfoTAG                                                  timer_List[TIMER_MANAGER_TIMER_COUNT_MAX];
  TimerId_t                                                     next_Timer_Id;
  SystemCriticalSession                                         operation_Key;
  TimerDeviceTimer                                              device_Timer;
};

#endif // _CONF_TIMER_CONTROL_TASK_ENABLED
#endif // _TIMER_CONTROL_TASK_IMPL_STD_H