
/////////////////////////////////////////////////
// INCLUDE
#include "TimerControlTaskImpl.h"
#if (_CONF_TIMER_CONTROL_TASK_ENABLED) && defined(SYSTEM_LINUX_PLATFORM)
/////////////////////////////////////////////////
// PREPROCESSOR

/////////////////////////////////////////////////
// DEFINE
#define TIMER_CONTROL_TASK_CONSOLE_DEBUG_ENABLE
#define TIMER_CONTROL_TASK_LINUX_SYS_TIMER_HANDLER_INVALID      0
#define TIMER_CONTROL_TASK_LINUX_CLOCK_ID                       CLOCK_REALTIME

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
/*TimerControlTask::Impl*/
TimerControlTask::Impl::Impl(void)
  : next_Timer_Id(0)
{
  this->cleanUp();
}

TimerControlTask::Impl::~Impl(void)
{
  this->stop();
  this->cleanUp();
}

bool TimerControlTask::Impl::isRunning(void)
{
  return true;// (this->signal_Action.sa_sigaction == NULL ? false : true);
}

int TimerControlTask::Impl::start(TimerControlTaskConfigTAG& config)
{
  return 0;
}

void TimerControlTask::Impl::stop(void)
{
  //nothing
}

TimerId_t TimerControlTask::Impl::startTimer(TimerConfigTAG& timerConfig)
{
  TimerId_t timerId = TIMER_MANAGER_TIMER_ID_INVALID;
  do
  {
    
    if ((timerConfig.isValid() == false)
      || (this->isRunning() == false)
      )
    {
      break;
    }

    if (timerConfig.timerId == TIMER_MANAGER_TIMER_ID_INVALID)
    {
      timerConfig.timerId = this->nextTimerId();
    }
    timerId = timerConfig.timerId;
    TimerHanlder_t timerHandler = TIMER_CONTROL_TASK_LINUX_SYS_TIMER_HANDLER_INVALID;
    struct sigevent signalEvent;
    struct itimerspec timerSpec;
    TimerId_t wk_idx = 0;
    {
      SystemCriticalLocker locker(this->operation_Key);
      for (wk_idx = 0; wk_idx < TIMER_MANAGER_TIMER_COUNT_MAX; wk_idx++)
      {
        if (this->timer_List[wk_idx].enabled == false)
        {
          break;
        }
      }

      if (wk_idx >= TIMER_MANAGER_TIMER_COUNT_MAX)
      {
        break; // full
      }
      this->timer_List[wk_idx].enabled = 1;
      
    }

    /* Create the timer. */
    //signalEvent.sigev_notify = SIGEV_SIGNAL;
    //signalEvent.sigev_signo = TIMER_CONTROL_TASK_LINUX_SIGNO;
    signalEvent.sigev_value.sival_ptr = &this->timer_List[wk_idx]; // point to timer info
    signalEvent.sigev_notify = SIGEV_THREAD;
    signalEvent.sigev_notify_function = TimerControlTask::Impl::cbTimerHandlerNavigator;
    signalEvent.sigev_notify_attributes = NULL;
    if (timer_create(TIMER_CONTROL_TASK_LINUX_CLOCK_ID, &signalEvent, &timerHandler) == -1)
    {
      break;
    }

    this->timer_List[wk_idx].oneShot = timerConfig.oneShot;
    this->timer_List[wk_idx].reserved = timerConfig.reserved;
    this->timer_List[wk_idx].interval = timerConfig.interval;
    this->timer_List[wk_idx].expiredPoint = 0;
    this->timer_List[wk_idx].handler = timerConfig.handler;
    this->timer_List[wk_idx].extraArgument = timerConfig.extraArgument;
    this->timer_List[wk_idx].timerId = timerConfig.timerId;
    this->timer_List[wk_idx].sysTimerHandler = timerHandler;
    this->timer_List[wk_idx].parent = this;

    time_t intervalInSec = this->timer_List[wk_idx].interval / TIMER_DEVICE_TIMER_SECOND_2_UNIT;
    time_t intervalInNano = (this->timer_List[wk_idx].interval % TIMER_DEVICE_TIMER_SECOND_2_UNIT) * TIMER_DEVICE_TIMER_UNIT_2_NANO;
    /* Start the timer. */
    timerSpec.it_value.tv_sec = (this->timer_List[wk_idx].oneShot != false? 0: intervalInSec);
    timerSpec.it_value.tv_nsec = (this->timer_List[wk_idx].oneShot != false ? 0 : intervalInNano);
    timerSpec.it_interval.tv_sec = intervalInSec;
    timerSpec.it_interval.tv_nsec = intervalInNano;

    if (timer_settime(timerHandler, 0, &timerSpec, NULL) == -1)
    {
      break;
    }
    return timerConfig.timerId;
  } while (0);

  if (timerId != TIMER_MANAGER_TIMER_ID_INVALID)
  {
    this->stopTimer(timerId);
  }
  return TIMER_MANAGER_TIMER_ID_INVALID;
}

void TimerControlTask::Impl::cleanUp(void)
{
  memset(&this->timer_List, 0, TIMER_MANAGER_TIMER_COUNT_MAX * sizeof(TimerInfoTAG));
}

TimerId_t TimerControlTask::Impl::nextTimerId(void)
{
  SystemCriticalLocker locker(this->operation_Key);
  this->next_Timer_Id++;
  if (this->next_Timer_Id == 0)
  {
    this->next_Timer_Id = 1;
  }

  return this->next_Timer_Id;
  
}

////////////////////Function to call from outside ISR///////////////////////
TimePoint_t TimerControlTask::Impl::now(void)
{
  return SYSTEM_TIME_MICROS();
}

void TimerControlTask::Impl::stopTimer(TimerId_t timerId)
{
  do
  {
    if ((timerId < TIMER_MANAGER_TIMER_ID_MIN)
      || (timerId > TIMER_MANAGER_TIMER_ID_MAX)
      )
    {
      break;
    }
    TimerId_t wk_idx = 0;
    TimerHanlder_t timerHandler;
    {
      SystemCriticalLocker locker(this->operation_Key);
      for (wk_idx = 0; wk_idx < TIMER_MANAGER_TIMER_COUNT_MAX; wk_idx++)
      {
        if ((this->timer_List[wk_idx].enabled == false)
          || (this->timer_List[wk_idx].timerId != timerId)
          )
        {
          continue;
        }
        break;
      }
      if (wk_idx >= TIMER_MANAGER_TIMER_COUNT_MAX)
      {
        break; // nothing found
      }
      timerHandler = this->timer_List[wk_idx].sysTimerHandler;
      this->timer_List[wk_idx].oneShot = 0;
      this->timer_List[wk_idx].reserved = 0;
      this->timer_List[wk_idx].interval = 0;
      this->timer_List[wk_idx].expiredPoint = 0;
      this->timer_List[wk_idx].handler = NULL;
      this->timer_List[wk_idx].extraArgument = NULL;
      this->timer_List[wk_idx].timerId = 0;
      this->timer_List[wk_idx].sysTimerHandler = 0;
      this->timer_List[wk_idx].parent = this;
    }

    timer_delete(timerHandler);

    return;
  } while (0);
  return;
}

////////////////////Function to call from inside ISR///////////////////////
void TimerControlTask::Impl::stopTimerFromISR(TimerId_t timerId, bool* woken)
{
  return this->stopTimer(timerId);
}

TimePoint_t TimerControlTask::Impl::nowFromISR(void)
{
  return this->now();
}

void TimerControlTask::Impl::cbTimerHandler(TimerId_t timerId)
{
  do
  {
    TimerManagerCbOnExpired handler = NULL;
    void* arg = NULL;
    if ((timerId < TIMER_MANAGER_TIMER_ID_MIN)
      || (timerId > TIMER_MANAGER_TIMER_ID_MAX)
      )
    {
      break;
    }
    TimerId_t wk_idx = 0;

    {
      SystemCriticalLocker locker(this->operation_Key);
      for (wk_idx = 0; wk_idx < TIMER_MANAGER_TIMER_COUNT_MAX; wk_idx++)
      {
        if ((this->timer_List[wk_idx].enabled == false)
          || (this->timer_List[wk_idx].timerId != timerId)
          )
        {
          continue;
        }
        break;
      }

      if (wk_idx >= TIMER_MANAGER_TIMER_COUNT_MAX)
      {
        break; // nothing found
      }

      handler = this->timer_List[wk_idx].handler;
      arg = this->timer_List[wk_idx].extraArgument;
    }
    
    bool woken = false;
    handler(timerId, arg, &woken);
    return;
  } while (0);
  return;
}

void TimerControlTask::Impl::cbTimerHandlerNavigator(union sigval signalValue)
{
  do
  {
    TimerInfoTAG* timerInfo = (TimerInfoTAG*)signalValue.sival_ptr;
    TimerId_t timerId = 0;
    if (timerInfo == NULL)
    {
      break;
    }

    timerId = timerInfo->timerId;
    if (timerInfo->parent == NULL)
    {
      break;
    }

    ((TimerControlTask::Impl*)timerInfo->parent)->cbTimerHandler(timerId);

    return;
  } while (0);
  return;
}
#endif // (_CONF_TIMER_CONTROL_TASK_ENABLED) && defined(SYSTEM_LINUX_PLATFORM)