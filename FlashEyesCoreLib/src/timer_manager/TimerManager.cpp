/////////////////////////////////////////////////
// INCLUDE
#include "TimerManager.h"

#if (_CONF_TIMER_MANAGER_ENABLED)
/////////////////////////////////////////////////
// PREPROCESSOR

/////////////////////////////////////////////////
// DEFINE
#define TIMER_MANAGER_CONSOLE_DEBUG_ENABLE

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
/*TimerManager*/
TimerManager& TimerManager::getInstance(void)
{
  static TimerManager instance;
  return instance;
}

TimerManager::TimerManager(void)
  : control_Task(NULL)
{

}

TimerManager::~TimerManager(void)
{
  this->finalize();
}

bool TimerManager::isValid(void)
{
  return (this->control_Task == NULL ? false : true);
}

bool TimerManager::isRunning(void)
{
  do
  {
    if (this->control_Task == NULL)
    {
      break;
    }

    return this->control_Task->isRunning();
  } while (0);
  return false;
}

int TimerManager::initialize(TimerControlTaskConfigTAG& controlTaskConfig)
{
  int result = 0;
  do
  {
    if (this->isValid() != 0)
    {
      return -1; // already initialized
    }

    this->finalize();

    this->control_Task = TimerControlTaskFactory::generate();
    if (this->control_Task == NULL)
    {
      break;
    }
    
    result = this->control_Task->start(controlTaskConfig);
    if (result != 0)
    {
      break;
    }

    return 0;
  } while (0);

  this->finalize();
  return -1;
}

TimerId_t TimerManager::start(TimerInfoTAG& timerConfig)
{
  do
  {
    if (this->isRunning() == false)
    {
      break;
    }

    return this->control_Task->startTimer(timerConfig);
  } while (0);
  return TIMER_MANAGER_TIMER_ID_INVALID;
}

int TimerManager::start(TimerManagerCbOnExpired callback, void* cbArg, unsigned long intervalMs, bool oneShot)
{
  do
  {
    if (this->isRunning() == false)
    {
      break;
    }

    TimerInfoTAG config = TimerInfoTAG();
    config.enabled = 1;
    config.interval = intervalMs*TIMER_DEVICE_TIMER_MS_2_UNIT;
    //config.handlingMode = TIMER_MANAGER_HANDLING_MODE_CALLBACK;
    config.oneShot = (oneShot == false ? 0 : 1);
    config.handler = callback;
    config.extraArgument = cbArg;
    return this->control_Task->startTimer(config);

  } while (0);
  return -1;  
}

void TimerManager::finalize(void)
{
  //int result = 0;
  do
  {
    if (this->isValid() == false)
    {
      break;
    }
    this->control_Task->stop();
    TimerControlTaskFactory::release(this->control_Task);
    this->control_Task = NULL;
    return;
  } while (0);

  this->control_Task = NULL;
  return;
}

////////////////////Function to call from outside ISR///////////////////////
TimePoint_t TimerManager::now(void)
{
  return this->control_Task->now();
}

TimePoint_t TimerManager::nowMs(void)
{
  return this->now() / TIMER_DEVICE_TIMER_MS_2_UNIT;
}

void TimerManager::stop(TimerId_t timerId)
{
  return this->control_Task->stopTimer(timerId);
}

////////////////////Function to call from inside ISR///////////////////////
TimePoint_t TimerManager::nowFromISR(void)
{
  return this->control_Task->nowFromISR();
}

TimePoint_t TimerManager::nowMsFromISR(void)
{
  return this->nowFromISR() / TIMER_DEVICE_TIMER_MS_2_UNIT;
}

void TimerManager::stopFromISR(TimerId_t timerId, bool* woken)
{
  return this->control_Task->stopTimerFromISR(timerId, woken);
}

#endif // _CONF_TIMER_MANAGER_ENABLED