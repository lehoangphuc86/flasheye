#include "TimerManagerConstant.h"

#if (_CONF_TIMER_MANAGER_ENABLED)

#ifndef _TIMER_MANAGER_H
#define _TIMER_MANAGER_H

/////////////////////////////////////////////////
// INCLUDE
#include "../os_system/SystemMutex.h"
#include "TimerControlTask.h"
#include "TimerControlTaskFactory.h"
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

/*TimerManager*/
class TimerManager
{
private:
  TimerManager(void);
  __ATTRIBUTE_VIRTUAL_OPTIMIZED ~TimerManager(void);

public:
  static TimerManager& getInstance(void);
  TimerManager(TimerManager const&) = delete;
  void operator=(TimerManager const&) = delete;

  int                                                           initialize(TimerControlTaskConfigTAG& controlTaskConfig);
  bool                                                          isValid(void);
  bool                                                          isRunning(void);

  TimerId_t                                                     start(TimerInfoTAG& timerConfig);
  int                                                           start(TimerManagerCbOnExpired callback, void* cbArg, unsigned long intervalMs, bool oneShot = true);
  void                                                          finalize(void);
  ////////////////////Function to call from outside ISR///////////////////////
public:
  TimePoint_t                                                   now(void);
  void                                                          stop(TimerId_t timerId);
  ////////////////////Function to call from inside ISR///////////////////////
public:
  TimePoint_t                                                   nowFromISR(void);
  void                                                          stopFromISR(TimerId_t timerId, bool* woken);
protected:
  TimerControlTask*                                             control_Task;
};
#endif // _TIMER_MANAGER_H

#endif // _CONF_TIMER_MANAGER_ENABLED