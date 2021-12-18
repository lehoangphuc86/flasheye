/////////////////////////////////////////////////
// INCLUDE
#include "TimerControlTaskFactory.h"

#if (_CONF_TIMER_CONTROL_TASK_ENABLED)
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
/*TimerControlTaskFactory*/
TimerControlTaskFactory::TimerControlTaskFactory(void)
{

}

TimerControlTaskFactory::~TimerControlTaskFactory(void)
{

}

TimerControlTask* TimerControlTaskFactory::generate(void)
{
  TimerControlTask* controlTask = NULL;
  do
  {
    controlTask = new TimerControlTask();
    return controlTask;
  } while (0);

  TimerControlTaskFactory::release(controlTask);
  return controlTask;
}

void TimerControlTaskFactory::release(TimerControlTask*& controlTask)
{
  do
  {
    if (controlTask == NULL)
    {
      break;
    }
    delete controlTask;
    controlTask = NULL;
    return;
  } while (0);
  return;
}

#endif // _CONF_TIMER_CONTROL_TASK_ENABLED