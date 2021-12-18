/////////////////////////////////////////////////
// INCLUDE
#include "TimerDeviceTimer.h"
#if (_CONF_TIMER_DEVICE_TIMER_ENABLED) && !defined (SYSTEM_LINUX_PLATFORM)

#ifdef SYSTEM_WIN_PLATFORM
#include "win/TimerDeviceTimerImpl.h"
#elif defined (SYSTEM_AVR_PLATFORM)
#include "avr/TimerDeviceTimerImpl.h"
#elif defined (SYSTEM_ESP_PLATFORM)
#include "esp/TimerDeviceTimerImpl.h"
#else 
#error "Unsupported platform"
#endif // SYSTEM_WIN_PLATFORM

/////////////////////////////////////////////////
// PREPROCESSOR

/////////////////////////////////////////////////
// DEFINE

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


/*TimerDeviceTimer*/
TimerDeviceTimer::TimerDeviceTimer(void)
  : m_pImpl(new Impl())
{

}

TimerDeviceTimer::~TimerDeviceTimer(void)
{
  if (this->m_pImpl != NULL)
  {
    delete this->m_pImpl;
  }
  this->m_pImpl = NULL;
}

int TimerDeviceTimer::inititialize(TimerDeviceTimerConfigTAG& config)
{
  return this->m_pImpl->inititialize(config);
}

bool TimerDeviceTimer::isValid(void)
{
  return this->m_pImpl->isValid();
}

TimePoint_t TimerDeviceTimer::minInterval(void)
{
  return this->m_pImpl->minInterval();
}

TimerCount_t TimerDeviceTimer::now(void)
{
  return this->m_pImpl->now();
}

int TimerDeviceTimer::start(void)
{
  return this->m_pImpl->start();
}

void TimerDeviceTimer::stop(void)
{
  return this->m_pImpl->stop();
}

void TimerDeviceTimer::pause(void)
{
  return this->m_pImpl->pause();
}

TimerReg_t TimerDeviceTimer::hwDelay(void)
{
  return this->m_pImpl->hwDelay();
}

int TimerDeviceTimer::setAlarmValue(TimerCount_t interval)
{
  return this->m_pImpl->setAlarmValue(interval);
}

void TimerDeviceTimer::notifyChange(void)
{
  return this->m_pImpl->notifyChange();
}

////////////////////Function to call from inside ISR///////////////////////
TimerCount_t TimerDeviceTimer::nowFromISR(void)
{
  return this->m_pImpl->nowFromISR();
}

int TimerDeviceTimer::startFromISR(void)
{
  return this->m_pImpl->startFromISR();
}

void TimerDeviceTimer::stopFromISR(void)
{
  return this->m_pImpl->stopFromISR();
}

void TimerDeviceTimer::pauseFromISR(void)
{
  return this->m_pImpl->pauseFromISR();
}

int TimerDeviceTimer::setAlarmValueFromISR(TimerCount_t interval)
{
  return this->m_pImpl->setAlarmValueFromISR(interval);
}


#endif // (_CONF_TIMER_CONTROL_TASK_ENABLED) && not defined (SYSTEM_LINUX_PLATFORM)