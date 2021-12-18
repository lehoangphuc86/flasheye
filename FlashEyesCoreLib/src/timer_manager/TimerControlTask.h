#include "TimerManagerConstant.h"

#if (_CONF_TIMER_CONTROL_TASK_ENABLED) 

#ifndef _TIMER_CONTROL_TASK_H
#define _TIMER_CONTROL_TASK_H

/////////////////////////////////////////////////
// INCLUDE

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
typedef struct _timerControlTaskConfigTAG
{
  void* taskManagerConfig; // PC
  void* taskThreadConfig; // PC

  byte hwTimerGrpId;
  byte hwTimerIndex;
  byte hwClockMHz;
  uint16_t hwTimerDevider;
} TimerControlTaskConfigTAG;
/////////////////////////////////////////////////
// STATIC DATA

/////////////////////////////////////////////////
// STATIC FUNCTIONS

/////////////////////////////////////////////////
// EXTERN

/////////////////////////////////////////////////
// CLASS DEFINITION

/*TimerControlTask*/
class TimerControlTask
{
public:
  TimerControlTask(void);
  ~TimerControlTask(void);
  bool                                                          isRunning(void);
  int                                                           start(TimerControlTaskConfigTAG& config);
  void                                                          stop(void);
  TimerId_t                                                     startTimer(TimerConfigTAG& timerConfig);
  void                                                          cleanUp(void);
  ////////////////////Function to call from outside ISR///////////////////////
public:
  TimePoint_t                                                   now(void);
  void                                                          stopTimer(TimerId_t timerId);

  ////////////////////Function to call from inside ISR///////////////////////
public:
  TimePoint_t                                                   nowFromISR(void);
  void                                                          stopTimerFromISR(TimerId_t timerId, bool* woken);

private:
  class                                                         Impl;
  Impl*                                                         m_pImpl;
};

#endif // _TIMER_CONTROL_TASK_H

#endif // _CONF_TIMER_CONTROL_TASK_ENABLED