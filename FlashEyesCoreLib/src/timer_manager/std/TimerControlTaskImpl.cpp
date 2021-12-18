#include "TimerControlTaskImpl.h"
#if (_CONF_TIMER_CONTROL_TASK_ENABLED) && !defined(SYSTEM_LINUX_PLATFORM)
/////////////////////////////////////////////////
// INCLUDE
/////////////////////////////////////////////////
// PREPROCESSOR

/////////////////////////////////////////////////
// DEFINE
//#define TIMER_CONTROL_TASK_CONSOLE_DEBUG_ENABLE

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
#ifdef TIMER_CONTROL_TASK_CONSOLE_DEBUG_ENABLE
char timeCtrlTaskLogBuf[SYSTEM_CONSOLE_OUT_BUF_LEN];
#endif // TIMER_CONTROL_TASK_CONSOLE_DEBUG_ENABLE
/////////////////////////////////////////////////
// STATIC FUNCTIONS

/////////////////////////////////////////////////
// EXTERN

/////////////////////////////////////////////////
// CLASS IMPLEMENTAION

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
  return this->device_Timer.isValid();
}

int TimerControlTask::Impl::start(TimerControlTaskConfigTAG& config)
{
  do
  {
    int result = 0;
    if (this->isRunning() != false)
    {
      return 0;
    }

    // set up hardware timer
    {
      TimerDeviceTimerConfigTAG hwTimerConfig = TimerDeviceTimerConfigTAG();
      hwTimerConfig.hwDevider = config.hwTimerDevider;
      hwTimerConfig.hwGroupId = config.hwTimerGrpId;
      hwTimerConfig.hwTimerId = config.hwTimerIndex;
      hwTimerConfig.hwClockMHz = config.hwClockMHz;
      hwTimerConfig.handler = (void*)TimerControlTask::Impl::cbTimerHandlerNavigator;
      hwTimerConfig.extraArg = (void*)this;
      hwTimerConfig.taskManagerConfig = config.taskManagerConfig;
      hwTimerConfig.taskThreadConfig = config.taskThreadConfig;
      
      result = this->device_Timer.inititialize(hwTimerConfig);
#ifdef TIMER_CONTROL_TASK_CONSOLE_DEBUG_ENABLE
      CONSOLE_LOG_BUF(timeCtrlTaskLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[timAd] staT %i", 0);
#endif // TIMER_CONTROL_TASK_CONSOLE_DEBUG_ENABLE
      if (result != 0)
      {
        break;
      }
      
      result = this->device_Timer.start();
#ifdef TIMER_CONTROL_TASK_CONSOLE_DEBUG_ENABLE
      CONSOLE_LOG_BUF(timeCtrlTaskLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[timAd] staT %i", 1);
#endif // TIMER_CONTROL_TASK_CONSOLE_DEBUG_ENABLE
      if (result != 0)
      {
        break;
      }
    }
    return result;
  } while (0);

  this->stop();
  return -1;
}

void TimerControlTask::Impl::stop(void)
{
  this->device_Timer.stop();
}

TimerId_t TimerControlTask::Impl::startTimer(TimerConfigTAG& timerConfig)
{
  do
  {
    SystemCriticalLocker locker(this->operation_Key);
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

    
    //timerConfig.expiredPoint = this->device_Timer.now() + timerConfig.interval;//@@@
    
    TimerId_t wk_idx = 0;
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

    this->timer_List[wk_idx].oneShot = timerConfig.oneShot;
    this->timer_List[wk_idx].reserved = timerConfig.reserved;
    this->timer_List[wk_idx].interval = timerConfig.interval;
    this->timer_List[wk_idx].expiredPoint = 0;//@@@this->device_Timer.now() + timerConfig.interval;
    this->timer_List[wk_idx].handler = timerConfig.handler;
    this->timer_List[wk_idx].extraArgument = timerConfig.extraArgument;
    this->timer_List[wk_idx].timerId = timerConfig.timerId;
    this->timer_List[wk_idx].enabled = 1;
    //this->device_Timer.setAlarmValue(TIMER_DEVICE_TIMER_HANDLE_TIME_MIN); // make it expired
    this->device_Timer.notifyChange(); // make it expired @@@@

#ifdef TIMER_CONTROL_TASK_CONSOLE_DEBUG_ENABLE
    //CONSOLE_LOG("[timAd] staT %i %i", 9, wk_idx);
#endif // TIMER_CONTROL_TASK_CONSOLE_DEBUG_ENABLE
    return timerConfig.timerId;
  } while (0);
  return TIMER_MANAGER_TIMER_ID_INVALID;
}

void TimerControlTask::Impl::cleanUp(void)
{
  memset(&this->timer_List, 0, TIMER_MANAGER_TIMER_COUNT_MAX * sizeof(TimerInfoTAG));
}

TimerId_t TimerControlTask::Impl::nextTimerId(void)
{
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
  return this->device_Timer.now();
}

void TimerControlTask::Impl::stopTimer(TimerId_t timerId)
{
  do
  {
    SystemCriticalLocker locker(this->operation_Key);
    if ((timerId < TIMER_MANAGER_TIMER_ID_MIN)
      || (timerId > TIMER_MANAGER_TIMER_ID_MAX)
      )
    {
      break;
    }
    TimerId_t wk_idx = 0;
    for (wk_idx = 0; wk_idx < TIMER_MANAGER_TIMER_COUNT_MAX; wk_idx++)
    {
      if ( (this->timer_List[wk_idx].enabled == false)
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
    this->timer_List[wk_idx].enabled = 0;
    //this->device_Timer.setAlarmValue(TIMER_DEVICE_TIMER_HANDLE_TIME_MIN); // make it expired

    return;
  } while (0);
  return;
}

////////////////////Function to call from inside ISR///////////////////////
void TimerControlTask::Impl::stopTimerFromISR(TimerId_t timerId, bool* woken)
{
  do
  {
    SystemCriticalLockerFromISR locker(this->operation_Key);
    if ((timerId < TIMER_MANAGER_TIMER_ID_MIN)
      || (timerId > TIMER_MANAGER_TIMER_ID_MAX)
      )
    {
      break;
    }
    TimerId_t wk_idx = 0;
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
    this->timer_List[wk_idx].enabled = 0;
    //this->device_Timer.setAlarmValueFromISR(TIMER_DEVICE_TIMER_HANDLE_TIME_MIN); // make it expired

    return;
  } while (0);
  return;
}

TimePoint_t TimerControlTask::Impl::nowFromISR(void)
{
  return this->device_Timer.nowFromISR();
}

// optimized
#if (1) //for adruino
void TimerControlTask::Impl::cbTimerHandler(TimePoint_t& nextInterval, bool* woken)
{
  //SystemCriticalLockerFromISR locker(this->operation_Key);
  TimePoint_t currentPoint = this->device_Timer.nowFromISR();
  TimerInfoTAG* timerInfo = this->timer_List;
  TimerInfoTAG* lastTimerInfo = &this->timer_List[TIMER_MANAGER_TIMER_COUNT_MAX];
  TimePoint_t leftTime = 0;
  TimePoint_t napTime = TIMER_CONTROL_TASK_IDLE_SNAP_TIME;
  TimePoint_t procTime = 0;
  
  {
    leftTime = 0;
    napTime = TIMER_CONTROL_TASK_IDLE_SNAP_TIME;
    procTime = 0;
    
    while (timerInfo < lastTimerInfo)
    {
      if (timerInfo->enabled == 0) //not activated 
      {
        timerInfo++;
        continue;
      }

      if (timerInfo->expiredPoint == 0)
      {
        timerInfo->expiredPoint = currentPoint + timerInfo->interval;
      }

      if (timerInfo->expiredPoint > currentPoint)
      {
        timerInfo++;
        continue;
      }

      timerInfo->handler(timerInfo->timerId, timerInfo->extraArgument, woken);

      if (timerInfo->oneShot != false)
      {
        timerInfo->enabled = false; // no repeat required
        timerInfo++;
        continue;
      }

      // re-activate
      timerInfo->expiredPoint += timerInfo->interval;
      timerInfo++;
    }

    timerInfo = this->timer_List;
    while (timerInfo < lastTimerInfo)
    {
      if (timerInfo->enabled == 0) //not activated 
      {
        timerInfo++;
        continue;
      }

      if (timerInfo->expiredPoint <= (currentPoint + this->device_Timer.hwDelay()))
      {
        napTime = 1;
#ifdef TIMER_CONTROL_TASK_CONSOLE_DEBUG_ENABLE
        //CONSOLE_LOG("[timT1] %lu %lu %u", timerInfo->expiredPoint, currentPoint, this->device_Timer.hwDelay());
#endif // TIMER_CONTROL_TASK_CONSOLE_DEBUG_ENABLE

        break;
      }

      leftTime = timerInfo->expiredPoint - currentPoint;
      // get the least nap time
      if (leftTime < napTime)
      {
        napTime = leftTime;
      }
      timerInfo++;
    }

    if (napTime > 1)
    {
      procTime = (this->device_Timer.nowFromISR() - currentPoint);
#ifdef TIMER_CONTROL_TASK_CONSOLE_DEBUG_ENABLE
      //if (napTime < procTime)
      //{
      //  //CONSOLE_LOG("[timT2] %lu %lu", napTime, procTime);
      //}
#endif // TIMER_CONTROL_TASK_CONSOLE_DEBUG_ENABLE
      napTime = (napTime > procTime ? napTime - procTime : 1);

    }
  }

  nextInterval = napTime;
}
#else
void TimerControlTask::Impl::cbTimerHandler(TimePoint_t& nextInterval, bool* woken)
{
  //SystemCriticalLockerFromISR locker(this->operation_Key);
  TimePoint_t currentPoint = this->device_Timer.nowFromISR();
  TimerInfoTAG* timerInfo = this->timer_List;
  TimerInfoTAG* lastTimerInfo = &this->timer_List[TIMER_MANAGER_TIMER_COUNT_MAX];
  TimePoint_t leftTime = 0;
  TimePoint_t napTime = TIMER_CONTROL_TASK_IDLE_SNAP_TIME;
  TimePoint_t procTime = 0;

  {
    leftTime = 0;
    napTime = TIMER_CONTROL_TASK_IDLE_SNAP_TIME;
    procTime = 0;

    while (timerInfo < lastTimerInfo)
    {
      if (timerInfo->enabled == 0) //not activated 
      {
        timerInfo++;
        continue;
      }

      if (timerInfo->expiredPoint == 0)
      {
        timerInfo->expiredPoint = currentPoint + timerInfo->interval;
      }

      if (timerInfo->expiredPoint > currentPoint)
      {
        timerInfo++;
        continue;
      }

#ifdef TIMER_CONTROL_TASK_CONSOLE_DEBUG_ENABLE
      //CONSOLE_LOG("[timT0] %lu %lu %lu", timerInfo->expiredPoint, currentPoint, timerInfo->interval);
#endif // TIMER_CONTROL_TASK_CONSOLE_DEBUG_ENABLE
      timerInfo->handler(timerInfo->timerId, timerInfo->extraArgument, woken);

      if (timerInfo->oneShot != false)
      {
        timerInfo->enabled = false; // no repeat required
        timerInfo++;
        continue;
      }

#ifdef TIMER_CONTROL_TASK_CONSOLE_DEBUG_ENABLE
      //CONSOLE_LOG("[timT0] %lu %lu %lu", timerInfo->expiredPoint, currentPoint, timerInfo->interval);
#endif // TIMER_CONTROL_TASK_CONSOLE_DEBUG_ENABLE
      // re-activate
      timerInfo->expiredPoint += timerInfo->interval;
      timerInfo++;
    }

    timerInfo = this->timer_List;
    while (timerInfo < lastTimerInfo)
    {
      if (timerInfo->enabled == 0) //not activated 
      {
        timerInfo++;
        continue;
      }

      if (timerInfo->expiredPoint <= (currentPoint + this->device_Timer.hwDelay()))
      {
        napTime = 1;
#ifdef TIMER_CONTROL_TASK_CONSOLE_DEBUG_ENABLE
        //CONSOLE_LOG("[timT1] %lu %lu %u", timerInfo->expiredPoint, currentPoint, this->device_Timer.hwDelay());
#endif // TIMER_CONTROL_TASK_CONSOLE_DEBUG_ENABLE

        break;
      }

      leftTime = timerInfo->expiredPoint - currentPoint;
      // get the least nap time
      if (leftTime < napTime)
      {
        napTime = leftTime;
      }
      timerInfo++;
    }

    if (napTime > 1)
    {
      procTime = (this->device_Timer.nowFromISR() - currentPoint);
#ifdef TIMER_CONTROL_TASK_CONSOLE_DEBUG_ENABLE
      if (napTime < procTime)
      {
        //CONSOLE_LOG("[timT2] %lu %lu", napTime, procTime);
      }
#endif // TIMER_CONTROL_TASK_CONSOLE_DEBUG_ENABLE
      napTime = (napTime > procTime ? napTime - procTime : 1);

    }
  }

  nextInterval = napTime;
}

#endif // 
void TimerControlTask::Impl::cbTimerHandlerNavigator(void* arg, TimePoint_t& nextInterval, bool* woken)
{
  ((TimerControlTask::Impl*)arg)->cbTimerHandler(nextInterval, woken);
}
#endif // (_CONF_TIMER_CONTROL_TASK_ENABLED) && defined(SYSTEM_PC_BASED)