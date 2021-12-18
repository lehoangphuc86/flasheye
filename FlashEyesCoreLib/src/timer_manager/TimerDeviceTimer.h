#include "TimerManagerConstant.h"

#if (_CONF_TIMER_DEVICE_TIMER_ENABLED) && !defined (SYSTEM_LINUX_PLATFORM)

#ifndef _TIMER_DEVICE_TIMER_H
#define _TIMER_DEVICE_TIMER_H

/////////////////////////////////////////////////
// PREPROCESSOR

/////////////////////////////////////////////////
// INCLUDE

/////////////////////////////////////////////////
// DEFINE

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
typedef struct _timerDeviceTimerConfigTAG
{
  byte hwGroupId; //esp
  byte hwTimerId;
  void* handler;
  void* extraArg;
  byte hwClockMHz;
  uint16_t hwDevider;
  void* taskManagerConfig; // PC
  void* taskThreadConfig; // PC
  bool isValid(void)
  {
    do
    {
      if (this->handler == NULL)
      {
        break;
      }
      if (this->hwClockMHz <= 0)
      {
        break;
      }

      return true;
    } while (0);
    return false;
  }
} TimerDeviceTimerConfigTAG;

/////////////////////////////////////////////////
// STATIC DATA

/////////////////////////////////////////////////
// STATIC FUNCTIONS

/////////////////////////////////////////////////
// EXTERN

/////////////////////////////////////////////////
// CLASS DEFINITION
// 
// 

//*TimerDeviceTimer*/
class TimerDeviceTimer
{
public:
  TimerDeviceTimer(void);
  virtual ~TimerDeviceTimer(void);
  int                                                           inititialize(TimerDeviceTimerConfigTAG& config);
  bool                                                          isValid(void);
  TimePoint_t                                                   minInterval(void);
 
  ////////////////////Function to call from outside ISR///////////////////////
public:
  TimerCount_t                                                  now(void);
  int                                                           start(void);
  void                                                          stop(void);
  void                                                          pause(void);
  TimerReg_t                                                    hwDelay(void);
  int                                                           setAlarmValue(TimerCount_t interval);
  void                                                          notifyChange(void);
  
  ////////////////////Function to call from inside ISR///////////////////////
public:
  TimerCount_t                                                  nowFromISR(void);
  int                                                           startFromISR(void);
  void                                                          stopFromISR(void);
  void                                                          pauseFromISR(void);
  int                                                           setAlarmValueFromISR(TimerCount_t interval);

private:
  class                                                         Impl;
  Impl*                                                         m_pImpl;
};

#endif // _TIMER_DEVICE_TIMER_H

#endif // _CONF_TIMER_DEVICE_TIMER_ENABLED && not defined (SYSTEM_LINUX_PLATFORM)