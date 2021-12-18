/////////////////////////////////////////////////
// INCLUDE
#include "TimerDeviceTimerImpl.h"
#if (_CONF_TIMER_DEVICE_TIMER_ENABLED) && defined(SYSTEM_ESP_PLATFORM)
/////////////////////////////////////////////////
// PREPROCESSOR
//#define TIMER_DEVICE_TIMER_CONSOLE_DEBUG_ENABLE
/////////////////////////////////////////////////
// DEFINE
#define TIMER_DEVICE_TIMER_ESP_INTERVAL_MIN           300
#define TIMER_DEVICE_TIMER_ESP_OCR_MIN                30 //us
#define TIMER_DEVICE_TIMER_ESP_OCR_PROC_DELAY         50 //us
#define TIMER_DEVICE_TIMER_ESP_OCR_HW_DELAY           (TIMER_DEVICE_TIMER_ESP_OCR_MIN + TIMER_DEVICE_TIMER_ESP_OCR_PROC_DELAY)

#define TIMER_DEVICE_TIMER_ESP_CONSOLE_DEBUG_ENABLE
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
#ifdef TIMER_DEVICE_TIMER_ESP_CONSOLE_DEBUG_ENABLE
char timeDeviceEspLogBuf[SYSTEM_CONSOLE_OUT_BUF_LEN];
#endif // TIMER_DEVICE_TIMER_ESP_CONSOLE_DEBUG_ENABLE
/////////////////////////////////////////////////
// STATIC FUNCTIONS

/////////////////////////////////////////////////
// EXTERN

/////////////////////////////////////////////////
// CLASS IMPLEMENTAION
/*TimerDeviceTimer::Impl*/
TimerDeviceTimer::Impl::Impl(void)
  : hw_Group_Id(timer_group_t::TIMER_GROUP_MAX)
  , hw_Timer_Id(timer_idx_t::TIMER_MAX)
  , hw_Tick_Per_Unit(0)
  , hw_Timer_Interval_Tick(0)
  , cb_Timer_Fired(NULL)
  , cb_Args(NULL)
{

}

TimerDeviceTimer::Impl::~Impl(void)
{

}

int TimerDeviceTimer::Impl::inititialize(TimerDeviceTimerConfigTAG& config)
{
  do
  {
    if (this->isValid() != false)
    {
      break;
    }

    if (config.isValid() == false)
    {
      break;
    }
    
    if (config.hwGroupId >= (byte)timer_group_t::TIMER_GROUP_MAX)
    {
      break;
    }

    if (config.hwTimerId >= (byte)timer_idx_t::TIMER_MAX)
    {
      break;
    }

    timer_group_t timerGrpId = (timer_group_t)config.hwGroupId;
    timer_idx_t timerIdx = (timer_idx_t)config.hwTimerId;
    double timerTickPerUnit = (config.hwClockMHz * 1.0 * SYSTEM_CONST_MHZ_2_HZ) / (TIMER_DEVICE_TIMER_SECOND_2_UNIT * config.hwDevider);

    timer_config_t hwConfig = timer_config_t();
    hwConfig.alarm_en = timer_alarm_t::TIMER_ALARM_EN;
    hwConfig.auto_reload = timer_autoreload_t::TIMER_AUTORELOAD_EN;
    hwConfig.counter_dir = timer_count_dir_t::TIMER_COUNT_UP;
    hwConfig.counter_en = timer_start_t::TIMER_PAUSE;
    hwConfig.divider = config.hwDevider;
    hwConfig.intr_type = timer_intr_mode_t::TIMER_INTR_LEVEL;
    esp_err_t retE = timer_init(timerGrpId, timerIdx, &hwConfig);
    if (retE != ESP_OK)
    {
      break;
    }
    
    retE = timer_set_counter_value(timerGrpId, timerIdx, 0);
    if (retE != ESP_OK)
    {
      break;
    }

    this->hw_Timer_Interval_Tick = (TIMER_DEVICE_TIMER_INTERVAL_DEFAULT * timerTickPerUnit);
    retE = timer_set_alarm_value(timerGrpId, timerIdx, this->hw_Timer_Interval_Tick);
    if (retE != ESP_OK)
    {
      break;
    }

    retE = timer_enable_intr(timerGrpId, timerIdx);
    if (retE != ESP_OK)
    {
      break;
    }
 
    retE = timer_isr_callback_add(timerGrpId, timerIdx, (bool(*)(void*))TimerDeviceTimer::Impl::cbHwTimerCompareMatchedNavigator, (void*)this, 0);
    if (retE != ESP_OK)
    {
      break;
    }

    this->hw_Group_Id = timerGrpId;
    this->hw_Timer_Id = timerIdx;
    this->hw_Tick_Per_Unit = timerTickPerUnit;
    this->cb_Timer_Fired = (TimerDeviceTimerCbOnExpired)config.handler;
    this->cb_Args = config.extraArg;
    return 0;
  } while (0);

  this->hw_Group_Id = timer_group_t::TIMER_GROUP_MAX;
  this->hw_Timer_Id = timer_idx_t::TIMER_MAX;
  this->hw_Tick_Per_Unit = 0;
  this->cb_Timer_Fired = NULL;
  this->cb_Args = NULL;
  return -1;
}

bool TimerDeviceTimer::Impl::isValid(void)
{
  do
  {
    if ( (this->hw_Group_Id == timer_group_t::TIMER_GROUP_MAX)
      || (this->hw_Timer_Id == timer_idx_t::TIMER_MAX)
      )
    {
      break;
    }
    return true;
  } while (0);
  return false;
}

TimePoint_t TimerDeviceTimer::Impl::minInterval(void)
{
  return TIMER_DEVICE_TIMER_ESP_INTERVAL_MIN;
}

TimerCount_t TimerDeviceTimer::Impl::now(void)
{
  return this->nowInTick() / this->hw_Tick_Per_Unit;
}

int TimerDeviceTimer::Impl::start(void)
{
  do
  {
    if (this->isValid() == false)
    {
      break;
    }

    esp_err_t retE = timer_start(this->hw_Group_Id, this->hw_Timer_Id);
    if (retE != ESP_OK)
    {
      break;
    }
    return 0;
  } while (0);
  this->stop();
  return -1;
}

void TimerDeviceTimer::Impl::stop(void)
{
  do
  {
    this->pause();
    timer_enable_intr(this->hw_Group_Id, this->hw_Timer_Id);
    this->hw_Group_Id = timer_group_t::TIMER_GROUP_MAX;
    this->hw_Timer_Id = timer_idx_t::TIMER_MAX;
    this->hw_Tick_Per_Unit = 0;
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

    esp_err_t retE = timer_pause(this->hw_Group_Id, this->hw_Timer_Id);
    if (retE != ESP_OK)
    {
      break;
    }
    return;
  } while (0);
  return;  
}

TimerReg_t TimerDeviceTimer::Impl::hwDelay(void)
{
  return TIMER_DEVICE_TIMER_ESP_OCR_HW_DELAY;
}

int TimerDeviceTimer::Impl::setAlarmValue(TimerCount_t interval)
{
  return this->setAlarmValueByTick(interval * this->hw_Tick_Per_Unit);
}

void TimerDeviceTimer::Impl::notifyChange(void)
{
  do
  {
    if (this->isValid() == false)
    {
      break;
    }

    esp_err_t retE = timer_set_alarm_value(this->hw_Group_Id, this->hw_Timer_Id, 10);
    if (retE != ESP_OK)
    {
      break;
    }
    return;
  } while (0);
  return;
}

TimerCount_t TimerDeviceTimer::Impl::nowInTick(void)
{
  TimerCount_t nowVal = 0;
  timer_get_counter_value(this->hw_Group_Id, this->hw_Timer_Id, &nowVal);
  return nowVal;
}

int TimerDeviceTimer::Impl::setAlarmValueByTick(TimerCount_t intervalInTick)
{
  do
  {
    if (this->isValid() == false)
    {
      break;
    }

    if (intervalInTick == 0)
    {
      intervalInTick = this->hw_Timer_Interval_Tick;
    }

    esp_err_t retE = timer_set_alarm_value(this->hw_Group_Id, this->hw_Timer_Id, intervalInTick);
    this->hw_Timer_Interval_Tick = intervalInTick;
    if (retE != ESP_OK)
    {
      break;
    }
    return 0;
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

TimerCount_t TimerDeviceTimer::Impl::nowInTickFromISR(void)
{
  return this->nowInTick();
}

int TimerDeviceTimer::Impl::setAlarmValueByTickFromISR(TimerCount_t intervalInTick)
{
  return this->setAlarmValueByTick(intervalInTick);
}

void TimerDeviceTimer::Impl::cbHwTimerCompareMatched(bool* woken)
{
  do
  {

    if (this->cb_Timer_Fired == NULL)
    {
      break;
    }
    TimePoint_t nextInterval = 0;
    this->cb_Timer_Fired(this->cb_Args, nextInterval, woken);
    if (nextInterval != 0)
    {
      nextInterval = nextInterval * 1.0 * this->hw_Tick_Per_Unit; // in tick
    }
    // set hw timer next alarm timepoint
    this->setAlarmValueByTickFromISR(nextInterval);
    return;
  } while (0);
  return;
}

SYSTEM_IRAM_ATTR bool TimerDeviceTimer::Impl::cbHwTimerCompareMatchedNavigator(void* arg)
{
  bool woken = false;
  do
  {
    if (arg == NULL)
    {
      break;
    }
    
    ((TimerDeviceTimer::Impl*)arg)->cbHwTimerCompareMatched(&woken);

    return woken;
  } while (0);
  return woken;
}
#endif // (_CONF_TIMER_CONTROL_TASK_ENABLED) && defined(SYSTEM_PC_BASED)