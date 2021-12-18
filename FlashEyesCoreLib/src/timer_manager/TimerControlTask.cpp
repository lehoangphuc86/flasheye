#include "TimerControlTask.h"
#if (_CONF_TIMER_CONTROL_TASK_ENABLED)
/////////////////////////////////////////////////
// INCLUDE
#ifdef SYSTEM_LINUX_PLATFORM
#include "linux/TimerControlTaskImpl.h"
#else // 
#include "std/TimerControlTaskImpl.h"
#endif // SYSTEM_LINUX_PLATFORM
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

/////////////////////////////////////////////////
// CLASS IMPLEMENTATION
/*TimerControlTask*/
TimerControlTask::TimerControlTask(void)
  : m_pImpl(new Impl())
{

}

TimerControlTask::~TimerControlTask(void)
{
  if (this->m_pImpl != NULL)
  {
    delete this->m_pImpl;
  }
  this->m_pImpl = NULL;
}

bool TimerControlTask::isRunning(void)
{
  return this->m_pImpl->isRunning();
}

int TimerControlTask::start(TimerControlTaskConfigTAG& config)
{
  return this->m_pImpl->start(config);
}

void TimerControlTask::stop(void)
{
  return this->m_pImpl->stop();
}

TimerId_t TimerControlTask::startTimer(TimerConfigTAG& timerConfig)
{
  return this->m_pImpl->startTimer(timerConfig);
}

void TimerControlTask::cleanUp(void)
{
  return this->m_pImpl->cleanUp();
}

////////////////////Function to call from outside ISR///////////////////////
TimePoint_t TimerControlTask::now(void)
{
  return this->m_pImpl->now();
}

void TimerControlTask::stopTimer(TimerId_t timerId)
{
  return this->m_pImpl->stopTimer(timerId);
}

////////////////////Function to call from inside ISR///////////////////////
TimePoint_t TimerControlTask::nowFromISR(void)
{
  return this->m_pImpl->nowFromISR();
}

void TimerControlTask::stopTimerFromISR(TimerId_t timerId, bool* woken)
{
  return this->m_pImpl->stopTimerFromISR(timerId, woken);
}

#endif // _CONF_TIMER_CONTROL_TASK_ENABLED