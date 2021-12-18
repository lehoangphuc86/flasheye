#ifndef _TIMER_CONTROL_TASK_IMPL_LINUX_H
#define _TIMER_CONTROL_TASK_IMPL_LINUX_H
/////////////////////////////////////////////////
// INCLUDE
#include "../TimerControlTask.h"
#if (_CONF_TIMER_CONTROL_TASK_ENABLED) && defined(SYSTEM_LINUX_PLATFORM)
#include <signal.h>
#include <time.h>
#include "os_system/SystemCriticalSession.h"
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
// CLASS IMPLEMENTAION
//

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
  void                                                          cbTimerHandler(TimerId_t timdeId); // for PC
  static void                                                   cbTimerHandlerNavigator(union sigval signalValue);

public:
  TimerInfoTAG                                                  timer_List[TIMER_MANAGER_TIMER_COUNT_MAX];
  TimerId_t                                                     next_Timer_Id;
  SystemCriticalSession                                         operation_Key;
};
#endif // (_CONF_TIMER_CONTROL_TASK_ENABLED) && defined(SYSTEM_LINUX_PLATFORM)

#endif // _TIMER_CONTROL_TASK_IMPL_LINUX_H