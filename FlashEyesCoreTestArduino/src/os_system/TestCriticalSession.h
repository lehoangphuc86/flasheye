/*
Note: 
  Enable below preprocessors
    FlashEyeCoreTestConfig.h:
      __CONF_TEST_CRITICAL_SESSION_FUNC_ENABLED
    FlashEyesCoreLibConfig.h:
      __CONF_FREE_RTOS_FUNC_ENABLED
      __CONF_CONSOLE_LOG_FUNC_ENABLED
  How to enable 1 or 2:
    1/ Set in Visual studio
    2/ Define in the noted file
*/


#include "../FlashEyeCoreTestConfig.h"

#ifdef _CONF_TEST_CRITICAL_SESSION_ENABLED
#ifndef _TEST_CRITICAL_SESSION_H
#define _TEST_CRITICAL_SESSION_H

/////////////////////////////////////////////////
// INCLUDE
#include "os_system/SystemCriticalSession.h"
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

/*TestCriticalSession*/

class TestCriticalSession
{

protected:
  static unsigned long criticalLastFired;
  static TestCriticalSession* mainObect;

protected:
  static void testCriticalLockerNavigator(void* param)
  {
    if (param != NULL)
    {
      ((TestCriticalSession*)param)->testCriticalLocker();
    }
  }


  static void onButtonIsrTestCritical(void)
  {
    bool wakeUp = false;

    do
    {
      if (millis() - TestCriticalSession::criticalLastFired < 200)
      { // Debounce
        return;
      }
      TestCriticalSession::criticalLastFired = millis();
      CONSOLE_LOG("cri val=%i", TestCriticalSession::mainObect->testCriticalSessionValue);

    } while (0);

    if (wakeUp != false)
    {
      SYSTEM_YEILD_FROM_ISR(wakeUp);

    }

  }

public:
  TestCriticalSession(void)
  {
    
  }

  virtual ~TestCriticalSession(void)
  {

  }

  void setup(void)
  {
    this->setMainObject();
    
#ifdef ESP_PLATFORM
    xTaskCreatePinnedToCore(
      testCriticalLockerNavigator
      , "criLock"
      , 1024  // Stack size
      , (void*)this
      , 1  // Priority
      , NULL
      , ARDUINO_RUNNING_CORE);
#else // ESP_PLATFORM
    xTaskCreate(
      testCriticalLockerNavigator
      , "criLock"
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
        attachInterrupt(digitalPinToInterrupt(TEST_INTERRUPT_PIN), TestCriticalSession::onButtonIsrTestCritical, FALLING);

      }
    }
  }

  void loop(void)
  {

  }
protected:
  void setMainObject(void)
  {
    TestCriticalSession::mainObect = this;
  }

  void testCriticalLocker()
  {
    do
    {
      {
        SystemCriticalLocker criticalLocker(criticalSession);
        testCriticalSessionValue = 0;
        // do something here
        for (unsigned int wk_count = 0; wk_count < 10; wk_count++)
        {
          testCriticalSessionValue++;
          delay(50);
        }

        CONSOLE_LOG("taskVal=%i", testCriticalSessionValue);
      }
      delay(500);
    } while (1);
  }
  
protected:
  SystemCriticalSession criticalSession;
  volatile unsigned int testCriticalSessionValue = 0;

};


unsigned long TestCriticalSession::criticalLastFired = 0;
TestCriticalSession* TestCriticalSession::mainObect = NULL;
#endif // _TEST_CRITICAL_SESSION_H

#endif // _CONF_TEST_CRITICAL_SESSION_ENABLED