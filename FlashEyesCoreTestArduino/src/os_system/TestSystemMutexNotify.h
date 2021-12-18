/*
Note:
  Enable below preprocessors
    FlashEyeCoreTestConfig.h:
      __CONF_TEST_SYSTEM_MUTEX_NOTIFY_FUNC_ENABLED
    FlashEyesCoreLibConfig.h:
      __CONF_FREE_RTOS_FUNC_ENABLED
      __CONF_SYSTEM_MUTEX_FUNC_ENABLED
      __CONF_CONSOLE_LOG_FUNC_ENABLED
  How to enable 1 or 2:
    1/ Set in Visual studio
    2/ Define in the noted file
*/
#include "../FlashEyeCoreTestConfig.h"

#ifdef _CONF_TEST_SYSTEM_MUTEX_NOTIFY_ENABLED

#ifndef _TEST_SYSTEM_MUTEX_NOTIFY_H
#define _TEST_SYSTEM_MUTEX_NOTIFY_H

/////////////////////////////////////////////////
// INCLUDE
#include "os_system/SystemMutex.h"
/////////////////////////////////////////////////
// PREPROCESSOR

/////////////////////////////////////////////////
// DEFINE
#define TEST_INTERRUPT_PIN  18
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

/////////////////////////////////////////////////
// STATIC DATA

/////////////////////////////////////////////////
// STATIC FUNCTIONS

/////////////////////////////////////////////////
// EXTERN

/////////////////////////////////////////////////
// CLASS DEFINITION

/*TestSystemMutexNotify*/

class TestSystemMutexNotify
{

protected:
  static unsigned long mLastFired;
  static SystemMutex lock_Key;
  static unsigned int wait_Count;
  static unsigned int notify_Count;
  static unsigned int notify_Isr_Count;
protected:
  static void waitingTask(void* param)
  {
    CONSOLE_LOG("wTask...", "");
    do
    {
      lock_Key.lock(3000);
      TestSystemMutexNotify::wait_Count++;
      CONSOLE_LOG("%i wTask %i", millis(), TestSystemMutexNotify::wait_Count);

    } while (1);
  }

  static void notifyTask(void* param)
  {
    CONSOLE_LOG("nTask..", "");
    do
    {
      delay(4000);
      TestSystemMutexNotify::notify_Count++;
      CONSOLE_LOG("%i nTask %i", millis(), TestSystemMutexNotify::notify_Count);
      lock_Key.unlock();
      
    } while (1);
  }

  static void onButtonIsrTestMutex(void)
  {
    bool wakeUp = false;

    do
    {
      if (millis() - TestSystemMutexNotify::mLastFired < 200)
      { // Debounce
        return;
      }
      TestSystemMutexNotify::mLastFired = millis();
      TestSystemMutexNotify::notify_Isr_Count++;
      CONSOLE_LOG("%i nIsr %i", millis(), TestSystemMutexNotify::notify_Isr_Count);
      TestSystemMutexNotify::lock_Key.unlockFromISR(&wakeUp);
    } while (0);

    if (wakeUp != false)
    {
      portYIELD_FROM_ISR();
    }

  }

public:
  TestSystemMutexNotify(void)
  {

  }

  virtual ~TestSystemMutexNotify(void)
  {

  }

  void setup(void)
  {
#ifdef ESP_PLATFORM
    xTaskCreatePinnedToCore(
      waitingTask
      , "waitTask"
      , 2048  // Stack size
      , (void*)this
      , 2  // Priority
      , NULL
      , ARDUINO_RUNNING_CORE);

    xTaskCreatePinnedToCore(
      notifyTask
      , "notifyTask"
      , 2048  // Stack size
      , (void*)this
      , 2  // Priority
      , NULL
      , ARDUINO_RUNNING_CORE);
#else // ESP_PLATFORM
    xTaskCreate(
      waitingTask
      , "waitTask"
      , 2048  // Stack size
      , (void*)this
      , 2  // Priority
      , NULL);

    xTaskCreate(
      notifyTask
      , "notifyTask"
      , 2048  // Stack size
      , (void*)this
      , 2  // Priority
      , NULL);
#endif // ESP_PLATFORM
    {
      // button
      pinMode(TEST_INTERRUPT_PIN, INPUT_PULLUP);
      // Set up interrupts
      {
        attachInterrupt(digitalPinToInterrupt(TEST_INTERRUPT_PIN), TestSystemMutexNotify::onButtonIsrTestMutex, FALLING);

      }
    }
  }

  void run()
  {

  }
};


unsigned long TestSystemMutexNotify::mLastFired = 0;
SystemMutex TestSystemMutexNotify::lock_Key;
unsigned int TestSystemMutexNotify::wait_Count = 0;
unsigned int TestSystemMutexNotify::notify_Count = 0;
unsigned int TestSystemMutexNotify::notify_Isr_Count = 0;
#endif // _TEST_SYSTEM_MUTEX_NOTIFY_H

#endif // _CONF_TEST_SYSTEM_MUTEX_NOTIFY_ENABLED