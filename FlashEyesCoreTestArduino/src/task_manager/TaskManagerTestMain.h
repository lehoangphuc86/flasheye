
/*
Note:
  Enable below preprocessors
    FlashEyeCoreTestConfig.h:
      __CONF_TEST_TASK_MANAGER_FUNC_ENABLED
    FlashEyesCoreLibConfig.h:
      __CONF_TASK_MANAGER_FUNC_ENABLED
      __CONF_CONSOLE_LOG_FUNC_ENABLED
  How to enable 1 or 2:
    1/ Set in Visual studio
    2/ Define in the noted file
*/

#include "../FlashEyeCoreTestConfig.h"
#ifdef _CONF_TEST_TASK_MANAGER_ENABLED

#ifndef _TASK_MANAGER_TEST_H
#define _TASK_MANAGER_TEST_H

/////////////////////////////////////////////////
// INCLUDE
#include "PingpongTask.h"
#include "OpcodeInputTask.h"
/////////////////////////////////////////////////
// PREPROCESSOR

/////////////////////////////////////////////////
// DEFINE
#ifdef SYSTEM_16BIT_SUPPORT
#define TASK_MANAGER_TEST_TASK_MEM              2048
#else //SYSTEM_16BIT_SUPPORT
#define TASK_MANAGER_TEST_TASK_MEM              256
#endif // SYSTEM_16BIT_SUPPORT
#define TASK_MANAGER_TEST_TASK_PRIORITY         2
#define TASK_MANAGER_TEST_POOL_COUNT            2
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

#define OP_QUIT                 9
#define OP_START                1
#define OP_STOP                 2

#ifdef SYSTEM_ARDUINO_BASED

#define BUTTON_ISR_TRIGGER_TYPE       FALLING
#ifdef SYSTEM_ESP_PLATFORM
#define PIN_INTERRUPT_REQUEST_BUTTON  18
#define PIN_INTERRUPT_SEND_BUTTON     19

#else // SYSTEM_ESP_PLATFORM
#define PIN_INTERRUPT_REQUEST_BUTTON  2
#define PIN_INTERRUPT_SEND_BUTTON     3

#endif // SYSTEM_ESP_PLATFORM

#else // SYSTEM_ARDUINO_BASED

#define PIN_INTERRUPT_REQUEST_BUTTON  -1
#define PIN_INTERRUPT_SEND_BUTTON     -1
#define BUTTON_ISR_TRIGGER_TYPE       0
#endif // SYSTEM_ARDUINO_BASED
class TaskManagerTest
{
public:
  TaskManagerTest();
  virtual ~TaskManagerTest();
  static int setup(void);
  static void loop(void);
protected:
  static bool cbOpCodeReceived(int opCode);
  //static int menu(void);

protected:
  static PingpongTask tienMinh;
  static PingpongTask maLong;
  static PingpongTask kama;
};

PingpongTask TaskManagerTest::tienMinh;
PingpongTask TaskManagerTest::maLong;
PingpongTask TaskManagerTest::kama;
TaskManagerTest::TaskManagerTest()
{

}

TaskManagerTest::~TaskManagerTest()
{

}

int TaskManagerTest::setup()
{
  CONSOLE_LOG("%s", "setup");
  int ret = 0;
  const int timePace = 500;
  do
  {
    if (TASK_MANAGER_TEST_POOL_COUNT > 0)
    {
      TaskThreadManagerConfigTAG threadPoolConfig[TASK_MANAGER_TEST_POOL_COUNT];
      memset(&threadPoolConfig, 0, sizeof(TaskThreadManagerConfigTAG) * TASK_MANAGER_TEST_POOL_COUNT);
      threadPoolConfig[0].preparedDataNumber = 2;
      threadPoolConfig[0].dataItemConfig.enabled = true;
      threadPoolConfig[0].dataItemConfig.proc = NULL;
      threadPoolConfig[0].dataItemConfig.procParams = NULL;
      threadPoolConfig[0].dataItemConfig.usStackDepth = TASK_MANAGER_TEST_TASK_MEM;
      threadPoolConfig[0].dataItemConfig.uxPriority = TASK_MANAGER_TEST_TASK_PRIORITY;

      memcpy(&threadPoolConfig[1], &threadPoolConfig[0], sizeof(TaskThreadManagerConfigTAG));
      threadPoolConfig[1].preparedDataNumber = 2;
      threadPoolConfig[1].dataItemConfig.usStackDepth = TASK_MANAGER_TEST_TASK_MEM + 10;

      ret = TaskThreadPoolManager::getInstance().prepare(TASK_MANAGER_TEST_POOL_COUNT, threadPoolConfig);
      if (ret != 0)
      {
        CONSOLE_LOG("[tmt] set PM %i", -1);
      }
    }


    TaskManagerConfigTAG taskConfig = TaskManagerConfigTAG();
    taskConfig.eventItemNumber = 5;
    taskConfig.eventUsePool = EventPoolManager::getInstance().isValid();
    /*taskConfig.usingEvent = true;
    taskConfig.threadConfig.enabled = true;
    taskConfig.threadConfig.usStackDepth = TASK_MANAGER_TEST_TASK_MEM;
    taskConfig.threadConfig.uxPriority = TASK_MANAGER_TEST_TASK_PRIORITY;*/
    //setupInterruption();
    ret = tienMinh.setConfig(false, 2, timePace, &maLong, taskConfig);
    if (ret != 0)
    {
      break;
    }

    ret = tienMinh.prepare();
    if (ret != 0)
    {
      break;
    }

    ret = maLong.setConfig(true, 3, timePace, &tienMinh, taskConfig);
    if (ret != 0)
    {
      break;
    }

    ret = maLong.prepare();
    if (ret != 0)
    {
      break;
    }

    ret = kama.setConfig(true, 4, timePace, &tienMinh, taskConfig);
    if (ret != 0)
    {
      break;
    }

    ret = kama.prepare();
    if (ret != 0)
    {
      break;
    }

    {
      OpCodeMenuItemTAG menuItems[3];
      memset(&menuItems, 0, sizeof(menuItems));
      menuItems[0].pin = PIN_INTERRUPT_REQUEST_BUTTON;
      menuItems[0].triggerType = BUTTON_ISR_TRIGGER_TYPE;
      menuItems[0].opCode = OP_START;
      memcpy(menuItems[0].description, "start", strlen("start"));

      menuItems[1].pin = PIN_INTERRUPT_SEND_BUTTON;
      menuItems[1].triggerType = BUTTON_ISR_TRIGGER_TYPE;
      menuItems[1].opCode = OP_STOP;
      memcpy(menuItems[1].description, "stop", strlen("stop"));

      menuItems[2].pin = -1;
      menuItems[2].triggerType = BUTTON_ISR_TRIGGER_TYPE;
      menuItems[2].opCode = OP_QUIT;
      memcpy(menuItems[2].description, "quit", strlen("quit"));

      ret = OpcodeInputTask::getInstance().setConfig(
        menuItems
        , 3
        , TaskManagerTest::cbOpCodeReceived
#ifdef OPCODE_INPUT_TAS_USE_TASK
        , taskConfig
#endif // OPCODE_INPUT_TAS_USE_TASK
      );


      if (ret != 0)
      {
        break;
      }
    }
    
    return 0;
  } while (0);
  return -1;
}

void TaskManagerTest::loop()
{
  do
  {
#ifdef OPCODE_INPUT_TAS_USE_TASK
    TaskThreadConfigTAG threadConfig = TaskThreadConfigTAG();
    threadConfig.enabled = true;
    threadConfig.usStackDepth = TASK_MANAGER_TEST_TASK_MEM;
    threadConfig.uxPriority = TASK_MANAGER_TEST_TASK_PRIORITY;
    threadConfig.useThreadPool = TaskThreadPoolManager::getInstance().isValid();
    int ret = OpcodeInputTask::getInstance().start(threadConfig);
#else // OPCODE_INPUT_TAS_USE_TASK
    int ret = OpcodeInputTask::getInstance().start();
#endif // OPCODE_INPUT_TAS_USE_TASK
    if (ret != 0)
    {
      CONSOLE_LOG("[tmt] opt %i", -1);
      break;
    }
  } while (0);

}

bool TaskManagerTest::cbOpCodeReceived(int opCode)
{
  CONSOLE_LOG("[tmt] oprev %i", opCode);
  bool isStopped = false;
  
  do
  {
    switch (opCode)
    {
    case OP_START:
    {
      TaskThreadConfigTAG threadConfig = TaskThreadConfigTAG();
      threadConfig.enabled = true;
      threadConfig.usStackDepth = TASK_MANAGER_TEST_TASK_MEM;
      threadConfig.uxPriority = TASK_MANAGER_TEST_TASK_PRIORITY;
      threadConfig.useThreadPool = TaskThreadPoolManager::getInstance().isValid();
      tienMinh.start(threadConfig);
      //maLong.start(threadConfig);
      //kama.start(threadConfig);
      break;
    }
    case OP_STOP:
      tienMinh.stop();
      maLong.stop();
      kama.stop();
      break;
    case OP_QUIT:
      
      isStopped = true;
      break;
    default:
      break;
    }

    tienMinh.stop();
    maLong.stop();
    kama.stop();
    return isStopped;
  } while (0);
  return true;
}

#endif // _TASK_SYSTEM_THREAD_H

#endif // _CONF_TEST_TASK_MANAGER_ENABLED