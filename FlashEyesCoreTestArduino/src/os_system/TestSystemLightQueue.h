/*
Note:
  Enable below preprocessors
    FlashEyeCoreTestConfig.h:
      __CONF_TEST_SYSTEM_LIGHT_QUEUE_FUNC_ENABLED
    FlashEyesCoreLibConfig.h:
      __CONF_DATA_MANAGER_FUNC_ENABLED
      __CONF_CONSOLE_LOG_FUNC_ENABLED
  How to enable 1 or 2:
    1/ Set in Visual studio
    2/ Define in the noted file
*/

#include "../FlashEyeCoreTestConfig.h"

#ifdef _CONF_TEST_SYSTEM_LIGHT_QUEUE_ENABLED
#ifndef _TEST_SYSTEM_LIGHT_QUEUE_H
#define _TEST_SYSTEM_LIGHT_QUEUE_H

/////////////////////////////////////////////////
// INCLUDE
#include "os_system/SystemLightQueue.h"
#include "os_system/SystemLightQueue.cpp"
#include "data_manager/DataItem.h"
typedef SystemLightQueue<BufferDataItem*> SystemBufferQueue;
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

/*TestSystemLightQueue*/

class TestSystemLightQueue
{

public:
  static unsigned long criticalLastFired;
  static TestSystemLightQueue* mainObect;

  static void onButtonIsrTest(void)
  {
    bool wakeUp = false;

    do
    {
      if (millis() - TestSystemLightQueue::criticalLastFired < 200)
      { // Debounce
        return;
      }
      TestSystemLightQueue::criticalLastFired = millis();
      TestSystemLightQueue::mainObect->runTestISR(&wakeUp);

    } while (0);

    if (wakeUp != false)
    {
      SYSTEM_YEILD_FROM_ISR(wakeUp);
    }

  }

public:
  TestSystemLightQueue(void)
    : test_ISR(false)
  {

  }
  
  virtual ~TestSystemLightQueue(void)
  {

  }

  void setup(bool testISR = false)
  {
    this->test_ISR = testISR;
   
    
    dataQueue.initialize(4);
    bufferInit.bufferSize = 1;
    CONSOLE_LOG("%s", "----------");
    dataQueue.dump();

    if (this->test_ISR == false)
    {
      return;
    }
    // test isr
    {
      this->setMainObject();
      // button
      pinMode(TEST_INTERRUPT_PIN, INPUT_PULLUP);
      // Set up interrupts
      {
        attachInterrupt(digitalPinToInterrupt(TEST_INTERRUPT_PIN), TestSystemLightQueue::onButtonIsrTest, FALLING);

      }
    }
  }

  void run()
  {
    if (this->test_ISR == false)
    {
      this->runTestTask();
    }
  }
  


protected:
  void setMainObject(void)
  {
    TestSystemLightQueue::mainObect = this;
  }

  void runTestTask()
  {
    CONSOLE_LOG("%s", "----runTestTask start------");
    int ret = 0;
    CONSOLE_LOG("%s", "----------");
    BufferDataItem* buff1 = new BufferDataItem(1);
    buff1->prepare(bufferInit);
    dataQueue.push(buff1);
    CONSOLE_LOG("buf id=%i", buff1->id());
    dataQueue.dump();

    CONSOLE_LOG("%s", "----------");
    BufferDataItem* buff2 = new BufferDataItem(2);
    buff2->prepare(bufferInit);
    dataQueue.push(buff2);
    CONSOLE_LOG("buf id=%i", buff2->id());
    dataQueue.dump();

    CONSOLE_LOG("%s", "----------");
    BufferDataItem* buff3 = new BufferDataItem(3);
    buff3->prepare(bufferInit);
    dataQueue.push(buff3);
    CONSOLE_LOG("buf id=%i", buff3->id());
    dataQueue.dump();


    pop(dataQueue);
    front(dataQueue);
    pop(dataQueue);
    front(dataQueue);
    pop(dataQueue);


    front(dataQueue);

    CONSOLE_LOG("%s", "----------");
    BufferDataItem* buff4 = new BufferDataItem(4);
    buff4->prepare(bufferInit);
    dataQueue.push(buff4);
    CONSOLE_LOG("buf id=%i", buff4->id());
    dataQueue.dump();



    /*
    pop(dataQueue);
    front(dataQueue);*/

    //pickItem(dataQueue, 3);
    //dataQueue.dump();
    removeItem(dataQueue, buff3);
    dataQueue.dump();
    removeItem(dataQueue, buff4);
    dataQueue.dump();
    removeItem(dataQueue, buff4);
    dataQueue.dump();


    CONSOLE_LOG("%s", "----------");
    BufferDataItem* buff5 = new BufferDataItem(5);
    buff5->prepare(bufferInit);
    dataQueue.push(buff5);
    CONSOLE_LOG("buf id=%i", buff5->id());
    dataQueue.dump();

    //pop(dataQueue);
    //front(dataQueue);
    //dataQueue.dump();
    removeItem(dataQueue, buff5);
    dataQueue.dump();
    removeItem(dataQueue, buff4);
    dataQueue.dump();

    CONSOLE_LOG("%s", "----runTestTask end------");
  }

  void runTestISR(bool* woken)
  {
    CONSOLE_LOG("%s", "----runTestISR start------");
    int ret = 0;
    CONSOLE_LOG("%s", "----------");
    BufferDataItem* buff1 = new BufferDataItem(1);
    buff1->prepare(bufferInit);
    dataQueue.pushFromISR(buff1);
    CONSOLE_LOG("buf id=%i", buff1->id());
    dataQueue.dumpFromISR();

    CONSOLE_LOG("%s", "----------");
    BufferDataItem* buff2 = new BufferDataItem(2);
    buff2->prepare(bufferInit);
    dataQueue.pushFromISR(buff2);
    CONSOLE_LOG("buf id=%i", buff2->id());
    dataQueue.dumpFromISR();

    CONSOLE_LOG("%s", "----------");
    BufferDataItem* buff3 = new BufferDataItem(3);
    buff3->prepare(bufferInit);
    dataQueue.pushFromISR(buff3);
    CONSOLE_LOG("buf id=%i", buff3->id());
    dataQueue.dumpFromISR();


    popFromISR(dataQueue);
    frontFromISR(dataQueue);
    popFromISR(dataQueue);
    frontFromISR(dataQueue);
    popFromISR(dataQueue);


    frontFromISR(dataQueue);

    CONSOLE_LOG("%s", "----------");
    BufferDataItem* buff4 = new BufferDataItem(4);
    buff4->prepare(bufferInit);
    dataQueue.pushFromISR(buff4);
    CONSOLE_LOG("buf id=%i", buff4->id());
    dataQueue.dumpFromISR();


    removeItemFromISR(dataQueue, buff3);
    dataQueue.dumpFromISR();
    removeItemFromISR(dataQueue, buff4);
    dataQueue.dumpFromISR();
    removeItemFromISR(dataQueue, buff4);
    dataQueue.dumpFromISR();


    CONSOLE_LOG("%s", "----------");
    BufferDataItem* buff5 = new BufferDataItem(5);
    buff5->prepare(bufferInit);
    dataQueue.pushFromISR(buff5);
    CONSOLE_LOG("buf id=%i", buff5->id());
    dataQueue.dumpFromISR();

    //pop(dataQueue);
    //front(dataQueue);
    //dataQueue.dump();
    removeItemFromISR(dataQueue, buff5);
    dataQueue.dumpFromISR();
    removeItemFromISR(dataQueue, buff4);
    dataQueue.dumpFromISR();
    CONSOLE_LOG("%s", "----runTestISR end------");
  }

  void removeItem(SystemBufferQueue& dataQueue, BufferDataItem* val)
  {
    CONSOLE_LOG("%s%i%s", "----removing", val->id(), "----");
    int ret = dataQueue.remove(val);

    CONSOLE_LOG("removeRet=%i", ret);

  }

  void front(SystemBufferQueue& dataQueue)
  {
    CONSOLE_LOG("%s", "----------");
    BufferDataItem** frontVal = NULL;
    int ret = 0;
    frontVal = dataQueue.front();
    CONSOLE_LOG("frontRet=%i", ret);
    if (ret != 0 || frontVal == NULL)
    {
      CONSOLE_LOG("frontFa %i", ret);
    }
    else
    {
      CONSOLE_LOG("frontId=%i", (*frontVal)->id());
    }
  }

  void pop(SystemBufferQueue& dataQueue)
  {
    CONSOLE_LOG("%s", "----------");
    int ret = dataQueue.pop();
    CONSOLE_LOG("popRet=%i", ret);
    //dataQueue.dump();
  }

  void removeItemFromISR(SystemBufferQueue& dataQueue, BufferDataItem* val)
  {
    CONSOLE_LOG("%s%i%s", "----removing", val->id(), "----");
    int ret = dataQueue.removeFromISR(val);

    CONSOLE_LOG("removeRet=%i", ret);

  }

  void frontFromISR(SystemBufferQueue& dataQueue)
  {
    CONSOLE_LOG("%s", "----------");
    BufferDataItem** frontVal = NULL;
    int ret = 0;
    frontVal = dataQueue.frontFromISR();
    CONSOLE_LOG("frontRet=%i", ret);
    if (ret != 0 || frontVal == NULL)
    {
      CONSOLE_LOG("frontFa %i", ret);
    }
    else
    {
      CONSOLE_LOG("frontId=%i", (*frontVal)->id());
    }
  }

  void popFromISR(SystemBufferQueue& dataQueue)
  {
    CONSOLE_LOG("%s", "----------");
    int ret = dataQueue.popFromISR();
    CONSOLE_LOG("popRet=%i", ret);
    //dataQueue.dump();
  }


protected:
  SystemBufferQueue  dataQueue;
  BufferDataItemConfigTAG bufferInit = BufferDataItemConfigTAG();
  bool test_ISR;
};

unsigned long TestSystemLightQueue::criticalLastFired = 0;
TestSystemLightQueue* TestSystemLightQueue::mainObect = NULL;

#endif // _TEST_SYSTEM_LIGHT_QUEUE_H

#endif // _CONF_TEST_SYSTEM_LIGHT_QUEUE_ENABLED