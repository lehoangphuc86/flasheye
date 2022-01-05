/////////////////////////////////////////////////
// INCLUDE
#include "TimerDeviceTimerImpl.h"
#if (_CONF_TIMER_DEVICE_TIMER_ENABLED) && defined(SYSTEM_AVR_PLATFORM)
#include <avr/io.h>
#include <avr/interrupt.h>

/////////////////////////////////////////////////
// PREPROCESSOR

/////////////////////////////////////////////////
// DEFINE
#define TIMER_DEVICE_TIMER_AVR_CONSOLE_DEBUG_ENABLE
#define TIMER_DEVICE_TIMER_AVR_HW_TIMER_COUNT         4
#define TIMER_DEVICE_TIMER_AVR_HW_INDEX_MIN           1
#define TIMER_DEVICE_TIMER_AVR_HW_INDEX_MAX           (TIMER_DEVICE_TIMER_AVR_HW_TIMER_COUNT -1)
#define TIMER_DEVICE_TIMER_AVR_TOIE_ENABLE            0x01
#define TIMER_DEVICE_TIMER_AVR_TOV_ENABLE             0x01
#define TIMER_DEVICE_TIMER_AVR_OCIE_ENABLE            0x01
#define TIMER_DEVICE_TIMER_AVR_OCR_MIN                30 //us
#define TIMER_DEVICE_TIMER_AVR_OCR_PROC_DELAY         50 //us
#define TIMER_DEVICE_TIMER_AVR_OCR_HW_DELAY           (TIMER_DEVICE_TIMER_AVR_OCR_MIN + TIMER_DEVICE_TIMER_AVR_OCR_PROC_DELAY)
#define TIMER_DEVICE_TIMER_AVR_INTERVAL_MIN           300

#define TIMER_DEVICE_TIMER_AVR_OVF_ISR_ENABLE_FLG     TOIE1
#define TIMER_DEVICE_TIMER_AVR_CMP_A_ISR_ENABLE_FLG   OCIE1A
#define TIMER_DEVICE_TIMER_AVR_OVF_STT_FLG            TOV1
#define TIMER_DEVICE_TIMER_AVR_OCF_STT_FLG            OCF1A
#define TIMER_DEVICE_TIMER_AVR_MAX_REG_VAL            UINT16_MAX
/////////////////////////////////////////////////
// MARCO


/////////////////////////////////////////////////
// DATA TYPE (TYPDEF)


/////////////////////////////////////////////////
// DATA TYPE (ENUM)

/////////////////////////////////////////////////
// DATA TYPE (STRUCT)

/////////////////////////////////////////////////
// GLOBAL VARIABLES
TimerDeviceHWCbOnExpired                              g_TimerOverflowISR[TIMER_DEVICE_TIMER_AVR_HW_TIMER_COUNT] = {NULL, NULL, NULL, NULL};
TimerDeviceHWCbOnExpired                              g_TimerCompareISR[TIMER_DEVICE_TIMER_AVR_HW_TIMER_COUNT] = { NULL, NULL, NULL, NULL };
void*                                                 g_TimerISRArgs[TIMER_DEVICE_TIMER_AVR_HW_TIMER_COUNT] = { NULL, NULL, NULL, NULL };


#ifdef TIMER_DEVICE_TIMER_AVR_CONSOLE_DEBUG_ENABLE
char timerDeviceAvrLogBuf[SYSTEM_CONSOLE_OUT_BUF_LEN] = {};
#endif // TIMER_DEVICE_TIMER_AVR_CONSOLE_DEBUG_ENABLE
/////////////////////////////////////////////////
// GLOBAL FUNCTIONS

// Fast find modulo of UINT16_MAX
inline uint64_t modUint16Max(uint64_t n)
{
  uint64_t mod; // n % d goes here.
  for (mod = n; n > UINT16_MAX; n = mod)
  {
    for (mod = 0; n; n >>= 16)
    {
      mod += n & UINT16_MAX;
    }
  }
  // Now m is a value from 0 to d, but since with modulus division
  // we want m to be 0 when it is d.
  mod = (mod == UINT16_MAX ? 0 : mod);
  return mod;
}

// Timer1 overflow interrupt
ISR(TIMER1_OVF_vect)
{
  byte timerIdx = 1;
  
  do
  {
    bool woken = false;
    if (g_TimerOverflowISR[timerIdx] == NULL)
    {
      break;
    }
    g_TimerOverflowISR[timerIdx](g_TimerISRArgs[timerIdx], &woken);

    SYSTEM_YEILD_FROM_ISR(woken);
    return;
  } while (0);
  return;
}

// Timer1 compare interrupt
ISR(TIMER1_COMPA_vect)
{
  byte timerIdx = 1;

  do
  {
    bool woken = false;
    if (g_TimerCompareISR[timerIdx] == NULL)
    {
      break;
    }
    g_TimerCompareISR[timerIdx](g_TimerISRArgs[timerIdx], &woken);

    SYSTEM_YEILD_FROM_ISR(woken);
    return;
  } while (0);
  return;
}

inline TimerReg_t TCCRA(Timer_Index_t timerIndex)
{
  switch (timerIndex)
  {
    case 0:
      return TCCR0A;
    case 1:
      return TCCR1A;
    case 2:
      return TCCR2A;
    case 3:
    default:
      return TCCR3A;
  }
}

inline void TCCRA(Timer_Index_t timerIndex, TimerReg_t val)
{
  switch (timerIndex)
  {
  case 0:
    TCCR0A = val;
    break;
  case 1:
    TCCR1A = val;
    break;
  case 2:
    TCCR2A = val;
    break;
  case 3:
  default:
    TCCR3A = val;
    break;
  }
}

inline TimerReg_t TCCRB(Timer_Index_t timerIndex)
{
  switch (timerIndex)
  {
  case 0:
    return TCCR0B;
  case 1:
    return TCCR1B;
  case 2:
    return TCCR2B;
  case 3:
  default:
    return TCCR3B;
  }
}

inline void TCCRB(Timer_Index_t timerIndex, TimerReg_t val)
{
  switch (timerIndex)
  {
  case 0:
    TCCR0B = val;
    break;
  case 1:
    TCCR1B = val;
    break;
  case 2:
    TCCR2B = val;
    break;
  case 3:
  default:
    TCCR3B = val;
    break;
  }
}

inline TimerReg_t OCRA(Timer_Index_t timerIndex)
{
  switch (timerIndex)
  {
  case 0:
    return OCR0A;
  case 1:
    return OCR1A;
  case 2:
    return OCR2A;
  case 3:
  default:
    return OCR3A;
  }
}

inline void OCRA(Timer_Index_t timerIndex, TimerReg_t val)
{
  switch (timerIndex)
  {
  case 0:
    OCR0A = val;
    break;
  case 1:
    OCR1A = val;
    break;
  case 2:
    OCR2A = val;
    break;
  case 3:
  default:
    OCR3A = val;
    break;
  }
}

inline TimerReg_t TCNT(Timer_Index_t timerIndex)
{
  switch (timerIndex)
  {
  case 0:
    return TCNT0;
  case 1:
    return TCNT1;
  case 2:
    return TCNT2;
  case 3:
  default:
    return TCNT3;
  }
}

inline void TCNT(Timer_Index_t timerIndex, TimerReg_t val)
{
  switch (timerIndex)
  {
  case 0:
    TCNT0 = val;
    break;
  case 1:
    TCNT1 = val;
    break;
  case 2:
    TCNT2 = val;
    break;
  case 3:
  default:
    TCNT3 = val;
    break;
  }
}

inline TimerReg_t TIFR(Timer_Index_t timerIndex)
{
  switch (timerIndex)
  {
  case 0:
    return TIFR0;
  case 1:
    return TIFR1;
  case 2:
    return TIFR2;
  case 3:
  default:
    return TIFR3;
  }
}

inline void TIFR(Timer_Index_t timerIndex, TimerReg_t val)
{
  switch (timerIndex)
  {
  case 0:
    TIFR0 = val;
    break;
  case 1:
    TIFR1 = val;
    break;
  case 2:
    TIFR2 = val;
    break;
  case 3:
  default:
    TIFR3 = val;
    break;
  }
}


inline TimerReg_t TIMSK(Timer_Index_t timerIndex)
{
  switch (timerIndex)
  {
  case 0:
    return TIMSK0;
  case 1:
    return TIMSK1;
  case 2:
    return TIMSK2;
  case 3:
  default:
    return TIMSK3;
  }
}

inline void TIMSK(Timer_Index_t timerIndex, TimerReg_t val)
{
  switch (timerIndex)
  {
  case 0:
    TIMSK0 = val;
    break;
  case 1:
    TIMSK1 = val;
    break;
  case 2:
    TIMSK2 = val;
    break;
  case 3:
  default:
    TIMSK3 = val;
    break;
  }
}

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
  : hw_Timer_Id(TIMER_DEVICE_TIMER_AVR_HW_INDEX_MAX)
  , hw_Timer_Overflow_Count(0)
  , hw_Tick_Per_Unit(0)
  , hw_Tick_Delay(0)
  , hw_Timer_Interval_Tick(0)
  , hw_Alarm_Point_Tick(0)
  , cb_Timer_Fired(NULL)
  , cb_Args(NULL)
{

}

TimerDeviceTimer::Impl::~Impl(void)
{

}

int TimerDeviceTimer::Impl::inititialize(TimerDeviceTimerConfigTAG& config)
{
  int result = 0;
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

    if ( (config.hwTimerId < TIMER_DEVICE_TIMER_AVR_HW_INDEX_MIN)
      || config.hwTimerId > TIMER_DEVICE_TIMER_AVR_HW_INDEX_MAX)
    {
      break;
    }
    this->hw_Timer_Id = config.hwTimerId;
    this->hw_Timer_Interval_Tick = 0;
    this->hw_Alarm_Point_Tick = 0;
    this->hw_Timer_Overflow_Count = 0;
    this->cb_Timer_Fired = (TimerDeviceTimerCbOnExpired)config.handler;
    this->cb_Args = config.extraArg;

    if ((g_TimerOverflowISR[config.hwTimerId] != NULL)
      || (g_TimerCompareISR[config.hwTimerId] != NULL)
      )// the timer is already being used
    {
      break;
    }
    TimerReg_t cs10 = 0;
    TimerReg_t cs11 = 0;
    TimerReg_t cs12 = 0;
    byte valid = true;
    switch (config.hwDevider)
    {
      case 1:
        cs10 = 0;
        cs11 = 0;
        cs12 = 1;
        break;
      case 8:
        cs10 = 0;
        cs11 = 1;
        cs12 = 0;
        break;
      case 64:
        cs10 = 0;
        cs11 = 1;
        cs12 = 1;
        break;
      case 256:
        cs10 = 1;
        cs11 = 0;
        cs12 = 0;
        break;
      case 1024:
        cs10 = 1;
        cs11 = 0;
        cs12 = 1;
        break;
      default:
        valid = false;
        break;
    }

    if (valid == false)
    {
      break;
    }

    {
      SystemCriticalLocker locker(this->critical_Session);
      TCCRA(this->hw_Timer_Id, 0);
      TCCRB(this->hw_Timer_Id, 0);
      TimerReg_t regTCCRB = TCCRB(this->hw_Timer_Id);
      regTCCRB |= (cs10 << CS10) | (cs11 << CS11) | (cs12 << CS12); // set clock divider
      TCCRB(this->hw_Timer_Id, regTCCRB);
    }
    

    this->hw_Tick_Per_Unit = (config.hwClockMHz * 1.0 * SYSTEM_CONST_MHZ_2_HZ) / (TIMER_DEVICE_TIMER_SECOND_2_UNIT * config.hwDevider);
    this->hw_Tick_Delay = (TIMER_DEVICE_TIMER_AVR_OCR_MIN * this->hw_Tick_Per_Unit);
#ifdef TIMER_DEVICE_TIMER_AVR_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG("[tdAvr] ini %i %i %i %f %u", 1, config.hwClockMHz, config.hwDevider, this->hw_Tick_Per_Unit, TCCRB(this->hw_Timer_Id));
#endif // TIMER_DEVICE_TIMER_AVR_CONSOLE_DEBUG_ENABLE
    result = this->setAlarmValue(TIMER_DEVICE_TIMER_INTERVAL_DEFAULT);
    if (result != 0)
    {
      break;
    }
#ifdef TIMER_DEVICE_TIMER_AVR_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG("[tdAvr] ini %i %u %lu", 2, OCRA(this->hw_Timer_Id), this->hw_Timer_Overflow_Count);
#endif // TIMER_DEVICE_TIMER_AVR_CONSOLE_DEBUG_ENABLE
    g_TimerOverflowISR[config.hwTimerId] = TimerDeviceTimer::Impl::cbHwTimerOverflowNavigator;
    g_TimerCompareISR[config.hwTimerId] = TimerDeviceTimer::Impl::cbHwTimerCompareMatchedNavigator;
    g_TimerISRArgs[config.hwTimerId] = (void*)this;
    return 0;
  } while (0);

  this->hw_Timer_Id = TIMER_DEVICE_TIMER_AVR_HW_INDEX_MAX;
  this->hw_Tick_Per_Unit = 0;
  this->hw_Tick_Delay = 0;
  this->hw_Timer_Interval_Tick = 0;
  this->hw_Alarm_Point_Tick = 0;
  this->hw_Timer_Overflow_Count = 0;
  this->cb_Timer_Fired = NULL;
  this->cb_Args = NULL;
  return -1;
}

bool TimerDeviceTimer::Impl::isValid(void)
{
  do
  {
    if ((this->hw_Timer_Id < TIMER_DEVICE_TIMER_AVR_HW_INDEX_MIN)
      || (this->hw_Timer_Id > TIMER_DEVICE_TIMER_AVR_HW_INDEX_MAX)
      || (this->hw_Tick_Per_Unit <= 0)
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
  return TIMER_DEVICE_TIMER_AVR_INTERVAL_MIN;
}

////////////////////Function to call from outside ISR///////////////////////
TimerCount_t TimerDeviceTimer::Impl::now(void)
{
  return this->nowInTick() *1.0 / this->hw_Tick_Per_Unit;
}

int TimerDeviceTimer::Impl::start(void)
{
  do
  {
    if (this->isValid() == false)
    {
      break;
    }

    {
      SystemCriticalLocker locker(this->critical_Session);
      TimerReg_t regTIMSK = TIMSK(this->hw_Timer_Id);
      regTIMSK |= (1 << TIMER_DEVICE_TIMER_AVR_OVF_ISR_ENABLE_FLG); // enable overflow interrupt
      regTIMSK |= (1 << TIMER_DEVICE_TIMER_AVR_CMP_A_ISR_ENABLE_FLG); // enable matched interrupt
      TIMSK(this->hw_Timer_Id, regTIMSK);
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
    {
      SystemCriticalLocker locker(this->critical_Session);
      TimerReg_t regTIMSK = TIMSK(this->hw_Timer_Id);
      regTIMSK &= (~(1 << TIMER_DEVICE_TIMER_AVR_OVF_ISR_ENABLE_FLG)); // clear overflow interrupt
      regTIMSK &= (~(1 << TIMER_DEVICE_TIMER_AVR_CMP_A_ISR_ENABLE_FLG)); // clear matched interrupt
      TIMSK(this->hw_Timer_Id, regTIMSK);
    }

    this->hw_Timer_Id = TIMER_DEVICE_TIMER_AVR_HW_INDEX_MAX;
    this->hw_Tick_Per_Unit = 0;
    this->hw_Tick_Delay = 0;
    this->hw_Timer_Interval_Tick = 0;
    this->hw_Alarm_Point_Tick = 0;
    this->hw_Timer_Overflow_Count = 0;
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

    {
      SystemCriticalLocker locker(this->critical_Session);
      TimerReg_t regTIMSK = TIMSK(this->hw_Timer_Id);
      regTIMSK &= (~(1 << TIMER_DEVICE_TIMER_AVR_OVF_ISR_ENABLE_FLG)); // clear overflow interrupt
      regTIMSK &= (~(1 << TIMER_DEVICE_TIMER_AVR_CMP_A_ISR_ENABLE_FLG)); // clear matched interrupt
      TIMSK(this->hw_Timer_Id, regTIMSK);
    }

    return;
  } while (0);
  return;
}

TimerReg_t TimerDeviceTimer::Impl::hwDelay(void)
{
  return TIMER_DEVICE_TIMER_AVR_OCR_HW_DELAY;
}

int TimerDeviceTimer::Impl::setAlarmValue(TimerCount_t interval)
{
  int ret = 0;
  do
  {
    {
      if (this->isValid() == false)
      {
        break;
      }

      TimerCount_t tick = interval * 1.0 * this->hw_Tick_Per_Unit;
      ret = this->setAlarmValueByTick(tick);
    }
    return ret;
  } while (0);
  return -1;
}

void TimerDeviceTimer::Impl::notifyChange(void)
{
  TimePoint_t savedInterval = this->hw_Timer_Interval_Tick;
  this->setAlarmValueByTick(this->hw_Tick_Delay);
  {
    SystemCriticalLocker locker(this->critical_Session);
    this->hw_Timer_Interval_Tick = savedInterval;
  }
}

TimerCount_t TimerDeviceTimer::Impl::nowInTick(void)
{
  TimerReg_t tcntReg1 = 0;
  TimerReg_t overflowReg = 0;
  TimerReg_t tcntReg2 = 0;
  byte compesationOv = 0;
  {
    SystemCriticalLocker locker(this->critical_Session);
    switch (this->hw_Timer_Id)
    {
    case 0:
      tcntReg1 = TCNT0;
      overflowReg = TIFR0;
      tcntReg2 = TCNT0;
      break;
    case 1:
      tcntReg1 = TCNT1;
      overflowReg = TIFR1;
      tcntReg2 = TCNT1;
      break;
    case 2:
      tcntReg1 = TCNT2;
      overflowReg = TIFR2;
      tcntReg2 = TCNT2;
      break;
    case 3:
    default:
      tcntReg1 = TCNT3;
      overflowReg = TIFR3;
      tcntReg2 = TCNT3;
      break;
    }
  }
  
  if ((overflowReg & (1 << TIMER_DEVICE_TIMER_AVR_OVF_STT_FLG) > 0)
    || (tcntReg2 < tcntReg1)
    )
  {
    compesationOv = 1;
  }

  return (TimerCount_t)((this->hw_Timer_Overflow_Count + compesationOv) * TIMER_DEVICE_TIMER_AVR_MAX_REG_VAL) + tcntReg2;
}


/// <summary>
/// Set interval to the hardware timer
/// Calculate the next alarm time point by alarmPointInTick = nowInTick() + intervalInTick
/// Set alarm point is alarmPointInTick% UINT16_MAX // max reg value is UINT16_MAX
/// </summary>
/// <param name="intervalInTick"></param>
/// <returns></returns>
int TimerDeviceTimer::Impl::setAlarmValueByTick(TimerCount_t intervalInTick)
{
  do
  {
    if (intervalInTick == 0)
    {
      intervalInTick = this->hw_Timer_Interval_Tick;
    }

    intervalInTick = SYSTEM_MAX(this->hw_Tick_Delay, intervalInTick);
    this->hw_Timer_Interval_Tick = intervalInTick;
    this->hw_Alarm_Point_Tick = intervalInTick;

    TimerReg_t tcntReg1 = 0;
    TimerReg_t overflowReg = 0;
    TimerReg_t tcntReg2 = 0;
    byte compesationOv = 0;
    //TimerReg_t cnt2 = 0;
    {
      SystemCriticalLocker locker(this->critical_Session);
      switch (this->hw_Timer_Id)
      {
        case 0:
        {
          tcntReg1 = TCNT0;
          overflowReg = TIFR0;
          tcntReg2 = TCNT0;
          if ((overflowReg & (1 << TIMER_DEVICE_TIMER_AVR_OVF_STT_FLG) > 0)
            || (tcntReg2 < tcntReg1)
            )
          {
            compesationOv = 1;
          }
          this->hw_Alarm_Point_Tick += tcntReg2;
          OCR0A = modUint16Max(this->hw_Alarm_Point_Tick);
          this->hw_Alarm_Point_Tick += (this->hw_Timer_Overflow_Count + compesationOv) * TIMER_DEVICE_TIMER_AVR_MAX_REG_VAL;
          break;
        }
        case 1:
        {
          tcntReg1 = TCNT1;
          overflowReg = TIFR1;
          tcntReg2 = TCNT1;
          if ((overflowReg & (1 << TIMER_DEVICE_TIMER_AVR_OVF_STT_FLG) > 0)
            || (tcntReg2 < tcntReg1)
            )
          {
            compesationOv = 1;
          }
          this->hw_Alarm_Point_Tick += tcntReg2;
          OCR1A = modUint16Max(this->hw_Alarm_Point_Tick);
          //cnt2 = TCNT1;
          this->hw_Alarm_Point_Tick += (this->hw_Timer_Overflow_Count + compesationOv) * TIMER_DEVICE_TIMER_AVR_MAX_REG_VAL;
          break;
        }
        case 2:
        {
          tcntReg1 = TCNT2;
          overflowReg = TIFR2;
          tcntReg2 = TCNT2;
          if ((overflowReg & (1 << TIMER_DEVICE_TIMER_AVR_OVF_STT_FLG) > 0)
            || (tcntReg2 < tcntReg1)
            )
          {
            compesationOv = 1;
          }
          this->hw_Alarm_Point_Tick += tcntReg2;
          OCR2A = modUint16Max(this->hw_Alarm_Point_Tick);
          this->hw_Alarm_Point_Tick += (this->hw_Timer_Overflow_Count + compesationOv) * TIMER_DEVICE_TIMER_AVR_MAX_REG_VAL;
          break;
        }
        case 3:
        default:
        {
          tcntReg1 = TCNT3;
          overflowReg = TIFR3;
          tcntReg2 = TCNT3;
          if ((overflowReg & (1 << TIMER_DEVICE_TIMER_AVR_OVF_STT_FLG) > 0)
            || (tcntReg2 < tcntReg1)
            )
          {
            compesationOv = 1;
          }
          this->hw_Alarm_Point_Tick += tcntReg2;
          OCR3A = modUint16Max(this->hw_Alarm_Point_Tick);
          this->hw_Alarm_Point_Tick += (this->hw_Timer_Overflow_Count + compesationOv) * TIMER_DEVICE_TIMER_AVR_MAX_REG_VAL;
          break;
        }
      }
    }
    

#ifdef TIMER_DEVICE_TIMER_AVR_CONSOLE_DEBUG_ENABLE
    //CONSOLE_LOG_ISR(timerDeviceAvrLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[sA0] %u", cnt2 - tcntReg1);
#endif // TIMER_DEVICE_TIMER_AVR_CONSOLE_DEBUG_ENABLE
    return 0;
  } while (0);
  return -1;
}

////////////////////Function to call from inside ISR///////////////////////
TimerCount_t TimerDeviceTimer::Impl::nowFromISR(void)
{
  return this->nowInTickFromISR() *1.0 / this->hw_Tick_Per_Unit;
}

int TimerDeviceTimer::Impl::startFromISR(void)
{
  do
  {
    if (this->isValid() == false)
    {
      break;
    }

    {
      //SystemCriticalLockerFromISR locker(this->critical_Session);
      TimerReg_t regTIMSK = TIMSK(this->hw_Timer_Id);
      regTIMSK |= (1 << TIMER_DEVICE_TIMER_AVR_OVF_ISR_ENABLE_FLG); // enable overflow interrupt
      regTIMSK |= (1 << TIMER_DEVICE_TIMER_AVR_CMP_A_ISR_ENABLE_FLG); // enable matched interrupt
      TIMSK(this->hw_Timer_Id, regTIMSK);
    }

    return 0;
  } while (0);
  this->stopFromISR();
  return -1;
}


void TimerDeviceTimer::Impl::stopFromISR(void)
{
  do
  {
    {
      //SystemCriticalLockerFromISR locker(this->critical_Session);
      TimerReg_t regTIMSK = TIMSK(this->hw_Timer_Id);
      regTIMSK &= (~(1 << TIMER_DEVICE_TIMER_AVR_OVF_ISR_ENABLE_FLG)); // clear overflow interrupt
      regTIMSK &= (~(1 << TIMER_DEVICE_TIMER_AVR_CMP_A_ISR_ENABLE_FLG)); // clear matched interrupt
      TIMSK(this->hw_Timer_Id, regTIMSK);
    }

    this->hw_Timer_Id = TIMER_DEVICE_TIMER_AVR_HW_INDEX_MAX;
    this->hw_Tick_Per_Unit = 0;
    this->hw_Tick_Delay = 0;
    this->hw_Timer_Interval_Tick = 0;
    this->hw_Alarm_Point_Tick = 0;
    this->hw_Timer_Overflow_Count = 0;
    this->cb_Timer_Fired = NULL;
    this->cb_Args = NULL;
    return;
  } while (0);
  return;
}


void TimerDeviceTimer::Impl::pauseFromISR(void)
{
  do
  {
    if (this->isValid() == false)
    {
      break;
    }

    {
      //SystemCriticalLockerFromISR locker(this->critical_Session);
      TimerReg_t regTIMSK = TIMSK(this->hw_Timer_Id);
      regTIMSK &= (~(1 << TIMER_DEVICE_TIMER_AVR_OVF_ISR_ENABLE_FLG)); // clear overflow interrupt
      regTIMSK &= (~(1 << TIMER_DEVICE_TIMER_AVR_CMP_A_ISR_ENABLE_FLG)); // clear matched interrupt
      TIMSK(this->hw_Timer_Id, regTIMSK);
    }

    return;
  } while (0);
  return;
}

int TimerDeviceTimer::Impl::setAlarmValueFromISR(TimerCount_t interval)
{
  int ret = 0;
  do
  {
    {
      if (this->isValid() == false)
      {
        break;
      }
      TimerCount_t tick = interval *1.0 * this->hw_Tick_Per_Unit;
      ret = this->setAlarmValueByTickFromISR(tick);
    }
    return ret;
  } while (0);
  return -1;
}

TimerCount_t TimerDeviceTimer::Impl::nowInTickFromISR(void)
{
  TimerReg_t tcntReg1 = 0;
  TimerReg_t overflowReg = 0;
  TimerReg_t tcntReg2 = 0;
  switch (this->hw_Timer_Id)
  {
    case 0:
      tcntReg1 = TCNT0;
      overflowReg = TIFR0;
      tcntReg2 = TCNT0;
      break;
    case 1:
      tcntReg1 = TCNT1;
      overflowReg = TIFR1;
      tcntReg2 = TCNT1;
      break;
    case 2:
      tcntReg1 = TCNT2;
      overflowReg = TIFR2;
      tcntReg2 = TCNT2;
      break;
    case 3:
    default:
      tcntReg1 = TCNT3;
      overflowReg = TIFR3;
      tcntReg2 = TCNT3;
      break;
  }

  byte compesationOv = 0;
  if ( (overflowReg & (1 << TIMER_DEVICE_TIMER_AVR_OVF_STT_FLG) > 0)
    || (tcntReg2 < tcntReg1)
    )
  {
    compesationOv = 1;
  }

  return (TimerCount_t)((this->hw_Timer_Overflow_Count + compesationOv) * TIMER_DEVICE_TIMER_AVR_MAX_REG_VAL) + tcntReg2;
}


int TimerDeviceTimer::Impl::setAlarmValueByTickFromISR(TimerCount_t intervalInTick)
{
  do
  {
    if (intervalInTick == 0)
    {
      intervalInTick = this->hw_Timer_Interval_Tick;
    }

    intervalInTick = SYSTEM_MAX(this->hw_Tick_Delay, intervalInTick);
    this->hw_Timer_Interval_Tick = intervalInTick;
    this->hw_Alarm_Point_Tick = intervalInTick;

    TimerReg_t tcntReg1 = 0;
    TimerReg_t overflowReg = 0;
    TimerReg_t tcntReg2 = 0;
    byte compesationOv = 0;
    //TimerReg_t cnt2 = 0;
    switch (this->hw_Timer_Id)
    {
    case 0:
    {
      tcntReg1 = TCNT0;
      overflowReg = TIFR0;
      tcntReg2 = TCNT0;
      if ((overflowReg & (1 << TIMER_DEVICE_TIMER_AVR_OVF_STT_FLG) > 0)
        || (tcntReg2 < tcntReg1)
        )
      {
        compesationOv = 1;
      }
      this->hw_Alarm_Point_Tick += tcntReg2;
      OCR0A = modUint16Max(this->hw_Alarm_Point_Tick);
      this->hw_Alarm_Point_Tick += (this->hw_Timer_Overflow_Count + compesationOv) * TIMER_DEVICE_TIMER_AVR_MAX_REG_VAL;
      break;
    }
    
    case 1:
    {
      tcntReg1 = TCNT1;
      overflowReg = TIFR1;
      tcntReg2 = TCNT1;
      if ((overflowReg & (1 << TIMER_DEVICE_TIMER_AVR_OVF_STT_FLG) > 0)
        || (tcntReg2 < tcntReg1)
        )
      {
        compesationOv = 1;
      }
      this->hw_Alarm_Point_Tick += tcntReg2;
      OCR1A = modUint16Max(this->hw_Alarm_Point_Tick);
      //cnt2 = TCNT1;
      this->hw_Alarm_Point_Tick += (this->hw_Timer_Overflow_Count + compesationOv) * TIMER_DEVICE_TIMER_AVR_MAX_REG_VAL;
      break;
    }
    case 2:
    {
      tcntReg1 = TCNT2;
      overflowReg = TIFR2;
      tcntReg2 = TCNT2;
      if ((overflowReg & (1 << TIMER_DEVICE_TIMER_AVR_OVF_STT_FLG) > 0)
        || (tcntReg2 < tcntReg1)
        )
      {
        compesationOv = 1;
      }
      this->hw_Alarm_Point_Tick += tcntReg2;
      OCR2A = modUint16Max(this->hw_Alarm_Point_Tick);
      this->hw_Alarm_Point_Tick += (this->hw_Timer_Overflow_Count + compesationOv) * TIMER_DEVICE_TIMER_AVR_MAX_REG_VAL;
      break;
    }
    case 3:
    default:
    {
      tcntReg1 = TCNT3;
      overflowReg = TIFR3;
      tcntReg2 = TCNT3;
      if ((overflowReg & (1 << TIMER_DEVICE_TIMER_AVR_OVF_STT_FLG) > 0)
        || (tcntReg2 < tcntReg1)
        )
      {
        compesationOv = 1;
      }
      this->hw_Alarm_Point_Tick += tcntReg2;
      OCR3A = modUint16Max(this->hw_Alarm_Point_Tick);
      this->hw_Alarm_Point_Tick += (this->hw_Timer_Overflow_Count + compesationOv) * TIMER_DEVICE_TIMER_AVR_MAX_REG_VAL;
      break;
    }
    }

#ifdef TIMER_DEVICE_TIMER_AVR_CONSOLE_DEBUG_ENABLE
    //CONSOLE_LOG_ISR(timerDeviceAvrLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[sA0] %u", cnt2 - tcntReg1);
#endif // TIMER_DEVICE_TIMER_AVR_CONSOLE_DEBUG_ENABLE
    return 0;
  } while (0);
  return -1;
}

void TimerDeviceTimer::Impl::cbHwTimerOverflow(bool* woken)
{
  this->hw_Timer_Overflow_Count++;
}

void TimerDeviceTimer::Impl::cbHwTimerCompareMatched(bool* woken)
{
  do
  {
    TIFR(this->hw_Timer_Id, (1 << TIMER_DEVICE_TIMER_AVR_OCF_STT_FLG));
    if (this->nowInTickFromISR() < this->hw_Alarm_Point_Tick) /// still running
    {
      return;
    }
    // reached the interval
    TimePoint_t nextInterval = 0;
    this->cb_Timer_Fired(this->cb_Args, nextInterval, woken);

    if (nextInterval != 0)
    {
      nextInterval = nextInterval * 1.0 * this->hw_Tick_Per_Unit; // in tick
    }
#ifdef TIMER_DEVICE_TIMER_AVR_CONSOLE_DEBUG_ENABLE
//    CONSOLE_LOG_ISR("[cbhw0] %lu", nextInterval);
#endif // TIMER_DEVICE_TIMER_AVR_CONSOLE_DEBUG_ENABLE
    // set hw timer next alarm timepoint
    this->setAlarmValueByTickFromISR(nextInterval);
    return;
  } while (0);
  return;
}


void TimerDeviceTimer::Impl::cbHwTimerOverflowNavigator(void* arg, bool* woken)
{
  do
  {
    if (arg == NULL)
    {
      break;
    }
    ((TimerDeviceTimer::Impl*)arg)->cbHwTimerOverflow(woken);
    return;
  } while (0);
  return;
}

void TimerDeviceTimer::Impl::cbHwTimerCompareMatchedNavigator(void* arg, bool* woken)
{
  do
  {
    if (arg == NULL)
    {
      break;
    }
    ((TimerDeviceTimer::Impl*)arg)->cbHwTimerCompareMatched(woken);
    return;
  } while (0);
  return;
}
#else // (_CONF_TIMER_CONTROL_TASK_ENABLED) && defined(SYSTEM_PC_BASED)
// to make compilier happy
class TimerControlTaskImplAvr
{
  volatile char reserved;
};
#endif // (_CONF_TIMER_CONTROL_TASK_ENABLED) && defined(SYSTEM_PC_BASED)