#include "../FlashEyeCoreTestConfig.h"

#ifdef __CONF_TEST_TIMER_MANAGER_FUNC_ENABLED

#ifndef _TIMER_MANAGER_TEST_H
#define _TIMER_MANAGER_TEST_H

/////////////////////////////////////////////////
// INCLUDE
#include "timer_manager/TimerManager.h"
#include "task_manager/TaskManager.h"
/////////////////////////////////////////////////
// PREPROCESSOR
#define TIMER_MANAGER_TEST_NO_BUTTON
/////////////////////////////////////////////////
// DEFINE
#define TEST_TIMER_COUNT    2
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

/*TimerManagerTest*/
class TimerManagerTest
  : public TaskManager
{
public:
  TimerManagerTest(void);
  virtual ~TimerManagerTest(void);
  int                                                           setConfig(TaskManagerConfigTAG& taskConfig);
  int                                                           start(TaskThreadConfigTAG& threadConfig);
  int                                                           stop(void);

  static void                                                   cbTimerFired(TimerId_t timerId, void* extraArg, bool* woken);
protected:
  int                                                           prepare(void);
  void                                                          proc(void) override;
  int                                                           onEventTimerFired(unsigned char* data, unsigned int dataSize);
  int                                                           onEventTimerFiredResult(unsigned char* data, unsigned int dataSize);
};


#endif // _TIMER_MANAGER_TEST_H

#endif // __CONF_TEST_TIMER_MANAGER_FUNC_ENABLED