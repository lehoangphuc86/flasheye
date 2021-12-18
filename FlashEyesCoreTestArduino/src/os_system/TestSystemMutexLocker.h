/*
Note:
  Enable below preprocessors
    FlashEyeCoreTestConfig.h:
      __CONF_TEST_SYSTEM_MUTEX_LOCKER_FUNC_ENABLED
    FlashEyesCoreLibConfig.h:
      __CONF_FREE_RTOS_FUNC_ENABLED
      __CONF_SYSTEM_MUTEX_FUNC_ENABLED
      __CONF_CONSOLE_LOG_FUNC_ENABLED
  How to enable 1 or 2:
    1/ Set in Visual studio
    2/ Define in the noted file
*/

#include "../FlashEyeCoreTestConfig.h"
#ifdef _CONF_TEST_SYSTEM_MUTEX_LOCKER_ENABLED

#ifndef _TEST_SYSTEM_MUTEX_LOCKER_H
#define _TEST_SYSTEM_MUTEX_LOCKER_H
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

/*TestSystemMutexLocker*/

class TestSystemMutexLocker
{

protected:
  static unsigned long mutexLockerLastFired;
  static volatile unsigned int testMutexLockerValue;
  static SystemMutex mutex;
protected:
  static void testMutexLockerTask1(void* param)
  {
    do
    {
      {
        SystemMutexLocker mutexLocker(TestSystemMutexLocker::mutex);
        testMutexLockerValue = 0;
        // do something here
        for (unsigned int wk_count = 0; wk_count < 10; wk_count++)
        {
          testMutexLockerValue++;
          delay(50);
        }

        CONSOLE_LOG("task1Val=%i", testMutexLockerValue);
      }
      delay(100);
    } while (1);
  }

  static void testMutexLockerTask2(void* param)
  {
    do
    {
      {
        SystemMutexLocker mutexLocker(TestSystemMutexLocker::mutex);
        CONSOLE_LOG("task2Val=%i", testMutexLockerValue);
      }
      delay(50);
    } while (1);
  }


  static void onButtonIsrTestMutexLocker(void)
  {
    bool wakeUp = false;

    do
    {
      if (millis() - TestSystemMutexLocker::mutexLockerLastFired < 200)
      { // Debounce
        return;
      }
      TestSystemMutexLocker::mutexLockerLastFired = millis();
      CONSOLE_LOG("cri val=%i", TestSystemMutexLocker::testMutexLockerValue);

    } while (0);

    if (wakeUp != false)
    {
      SYSTEM_YEILD_FROM_ISR(wakeUp);
    }

  }

public:
  TestSystemMutexLocker(void)
  {

  }

  virtual ~TestSystemMutexLocker(void)
  {

  }

  void setup(void)
  {

#ifdef ESP_PLATFORM
    xTaskCreatePinnedToCore(
      testMutexLockerTask1
      , "task1"
      , 1024  // Stack size
      , (void*)this
      , 1  // Priority
      , NULL
      , ARDUINO_RUNNING_CORE);

    xTaskCreatePinnedToCore(
      testMutexLockerTask2
      , "task2"
      , 1024  // Stack size
      , (void*)this
      , 1  // Priority
      , NULL
      , ARDUINO_RUNNING_CORE);
#else // ESP_PLATFORM
    xTaskCreate(
      testMutexLockerTask1
      , "task1"
      , 1024  // Stack size
      , (void*)this
      , 1  // Priority
      , NULL);

    xTaskCreate(
      testMutexLockerTask2
      , "task2"
      , 1024  // Stack size
      , (void*)this
      , 1  // Priority
      , NULL);
#endif // ESP_PLATFORM
    {
      // button
      pinMode(TEST_INTERRUPT_PIN, INPUT_PULLUP);
      // Set up interrupts
      {
        attachInterrupt(digitalPinToInterrupt(TEST_INTERRUPT_PIN), TestSystemMutexLocker::onButtonIsrTestMutexLocker, FALLING);

      }
    }
  }

};


unsigned long TestSystemMutexLocker::mutexLockerLastFired = 0;
volatile unsigned int TestSystemMutexLocker::testMutexLockerValue = 0;
SystemMutex TestSystemMutexLocker::mutex;
#endif // _TEST_SYSTEM_MUTEX_LOCKER_H

#endif // _CONF_TEST_SYSTEM_MUTEX_LOCKER_ENABLED