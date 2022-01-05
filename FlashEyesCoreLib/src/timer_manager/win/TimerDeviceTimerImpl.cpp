/////////////////////////////////////////////////
// INCLUDE
#include "TimerDeviceTimerImpl.h"
#if (_CONF_TIMER_DEVICE_TIMER_ENABLED) && defined(SYSTEM_WIN_PLATFORM)

/////////////////////////////////////////////////
// PREPROCESSOR
#define TIMER_DEVICE_TIMER_PC_CONSOLE_DEBUG_ENABLE
/////////////////////////////////////////////////
// DEFINE
#define TIMER_DEVICE_TIMER_PC_INTERVAL_MIN            100
#define TIMER_DEVICE_TIMER_PC_OCR_MIN                 0 //us
#define TIMER_DEVICE_TIMER_PC_OCR_PROC_DELAY          0 //us
#define TIMER_DEVICE_TIMER_PC_OCR_HW_DELAY            200 //@@@(TIMER_DEVICE_TIMER_PC_OCR_MIN + TIMER_DEVICE_TIMER_PC_OCR_PROC_DELAY)

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
TimerDeviceTimer::Impl::Impl(void)
  : TaskManager()
  , hw_Timer_Interval_Tick(TIMER_DEVICE_TIMER_INTERVAL_DEFAULT)
  , cb_Timer_Fired(NULL)
  , cb_Args(NULL)
{

}

TimerDeviceTimer::Impl::~Impl(void)
{
  this->stop();
}

int TimerDeviceTimer::Impl::inititialize(TimerDeviceTimerConfigTAG& config)
{
  do
  {
    int result = 0;
    if (this->isTaskRunning() != false)
    {
      return 0;
    }

    if (this->isValid() != false)
    {
      break;
    }

    if (config.isValid() == false)
    {
      break;
    }

    if ((config.taskManagerConfig == NULL)
      || (config.taskThreadConfig == NULL))
    {
      break;
    }

    this->cb_Timer_Fired = (TimerDeviceTimerCbOnExpired)config.handler;
    this->cb_Args = config.extraArg;

    // Set up tasks
    {
      TaskManagerConfigTAG* taskConfig = (TaskManagerConfigTAG*)config.taskManagerConfig;
      // self
      this->registerHanldingEventStructSize(sizeof(EventTimerPauseTAG));
      this->registerHanldingEventStructSize(sizeof(EventTimerSetAlarmValueTAG));
      
      result = TaskManager::setConfig(*taskConfig);
      if (result != 0)
      {
        break;
      }

      result = TaskManager::prepare();
      if (result != 0)
      {
        break;
      }
    }

    {
      TaskThreadConfigTAG* threadConfig = (TaskThreadConfigTAG*)config.taskThreadConfig;
      result = TaskManager::startProcess(*threadConfig, true);
      if (result != 0)
      {
        break;
      }
    }

    return result;
  } while (0);

  this->cb_Timer_Fired = NULL;
  this->cb_Args = NULL;
  this->stop();
  return -1;
}

bool TimerDeviceTimer::Impl::isValid(void)
{
  do
  {
    if (this->cb_Timer_Fired == NULL)
    {
      break;
    }
    return true;
  } while (0);
  return false;
}

TimePoint_t TimerDeviceTimer::Impl::minInterval(void)
{
  return TIMER_DEVICE_TIMER_PC_INTERVAL_MIN;
}

void TimerDeviceTimer::Impl::proc(void)
{
  int prepareRet = 0;// this->prepare();
  this->reportPrepareResult(prepareRet);
  if (prepareRet != 0)
  {
    this->waitTerminating();
    return;
  }
  EventDataItem* eventData = NULL;
  bool isPaused = false;
  bool woken = false;
  TimePoint_t nextInterval = 0;
  unsigned long napTime = 0;
  bool isStop = false;

  /*TimePoint_t realWaitTime = 0;
  TimePoint_t realWaitDiff = 0;*/
  while (isStop == false) // A Task shall never return or exit.
  {
    napTime = (unsigned long)TIMER_MANAGER_INTERVAL_TO_NANOSECOND(this->hw_Timer_Interval_Tick);
    //realWaitTime = this->nowFromISR();
    //napTime = (napTime > 200000? napTime - 200000: 0);
    eventData = this->event_Manager.waitAccurate(napTime);
    //realWaitTime = this->nowFromISR() - realWaitTime;
    //realWaitDiff = realWaitTime - this->hw_Timer_Interval_Tick;
    if (eventData != NULL)
    {
      switch (eventData->messageId())
      {
        case (int)EventManagerConstant::EventMessageId::TerminateProcess:
        {
          this->isTaskRunning(false);
          isStop = true;
          this->event_Manager.release(eventData);
          continue;
        }
        case (int)EventManagerConstant::EventMessageId::TimerPause:
        {
          isPaused = true;
          break;
        }
        case (int)EventManagerConstant::EventMessageId::TimerSetAlarm:
        {
          EventTimerSetAlarmValueTAG* setAlarmEvent = (EventTimerSetAlarmValueTAG*)eventData->bufferAddress();
          if (setAlarmEvent->interval != 0)
          {
            this->hw_Timer_Interval_Tick = setAlarmEvent->interval;
          }
          break;
        }
        default:
          break;
      }
      this->event_Manager.release(eventData);
    }

    if ((isPaused != false)
      || (this->cb_Timer_Fired == NULL)
      )
    {
      this->hw_Timer_Interval_Tick = TIMER_DEVICE_TIMER_INTERVAL_DEFAULT;
      continue;
    }
    
    nextInterval = 0;
    this->cb_Timer_Fired(this->cb_Args, nextInterval, &woken);
    // set hw timer next alarm timepoint
    //this->setAlarmValueByTick(nextInterval);
    if (nextInterval != 0)
    {
      this->hw_Timer_Interval_Tick = nextInterval;// SYSTEM_MAX(nextInterval, TIMER_DEVICE_TIMER_PC_OCR_MIN);
    }
#ifdef TIMER_DEVICE_TIMER_PC_CONSOLE_DEBUG_ENABLE
    //CONSOLE_LOG("[pcd0] %lu %u %lu", napTime, eventId, this->hw_Timer_Interval_Tick);
#endif // TIMER_DEVICE_TIMER_PC_CONSOLE_DEBUG_ENABLE
    
  }
#ifdef TIMER_DEVICE_TIMER_PC_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG("[pcd0] stop %i", 99);
#endif // TIMER_DEVICE_TIMER_PC_CONSOLE_DEBUG_ENABLE
}


TimerCount_t TimerDeviceTimer::Impl::now(void)
{
  return this->nowInTick();// (TimerCount_t)(this->nowInTick() * 1.0 / this->hw_Tick_Per_Unit);
}

int TimerDeviceTimer::Impl::start(void)
{
  return (this->isTaskRunning() == false ? -1 : 0);
}

void TimerDeviceTimer::Impl::stop(void)
{
  do
  {
    this->pause();
    TaskManager::stopProcess();
    this->cb_Timer_Fired = NULL;
    this->cb_Args = NULL;
    return;
  } while (0);
  return;
}

void TimerDeviceTimer::Impl::pause(void)
{
  do
  {
    if (this->isValid() == false)
    {
      break;
    }

    EventTimerPauseTAG eventData = EventTimerPauseTAG();
    this->notify((EventId_t)EventManagerConstant::EventMessageId::TimerSetAlarm, sizeof(EventTimerPauseTAG), (unsigned char*)&eventData);
    return;
  } while (0);
  return;
}

//float TimerDeviceTimer::Impl::hwTickPerUnit(void)
//{
//  return 1.0f;
//}

TimerReg_t TimerDeviceTimer::Impl::hwDelay(void)
{
  return TIMER_DEVICE_TIMER_PC_OCR_HW_DELAY;
}

int TimerDeviceTimer::Impl::setAlarmValue(TimerCount_t interval)
{
  return this->setAlarmValueByTick(interval);
}

void TimerDeviceTimer::Impl::notifyChange(void)
{
  this->setAlarmValueByTick(0);
}

TimerCount_t TimerDeviceTimer::Impl::nowInTick(void)
{
  return TIMER_MANAGER_CURRENT_TIMEPOINT();
}


int TimerDeviceTimer::Impl::setAlarmValueByTick(TimerCount_t intervalInTick)
{
  do
  {
    if (this->isValid() == false)
    {
      break;
    }
    
    EventTimerSetAlarmValueTAG eventData = EventTimerSetAlarmValueTAG();
    eventData.interval = intervalInTick;

    return this->notify((EventId_t)EventManagerConstant::EventMessageId::TimerSetAlarm, sizeof(EventTimerSetAlarmValueTAG), (unsigned char*)&eventData);
  } while (0);
  return -1;
}


TimerCount_t TimerDeviceTimer::Impl::nowFromISR(void)
{
  return this->now();
}

int TimerDeviceTimer::Impl::startFromISR(void)
{
  return this->start();
}

void TimerDeviceTimer::Impl::stopFromISR(void)
{
  return this->stop();
}

void TimerDeviceTimer::Impl::pauseFromISR(void)
{
  return this->pause();
}

int TimerDeviceTimer::Impl::setAlarmValueFromISR(TimerCount_t interval)
{
  return this->setAlarmValue(interval);
}
#else (_CONF_TIMER_CONTROL_TASK_ENABLED) && defined(SYSTEM_PC_BASED)
// to make compilier happy
class TimerControlTaskImplWin
{
  char reserved;
};
#endif // (_CONF_TIMER_CONTROL_TASK_ENABLED) && defined(SYSTEM_PC_BASED)