
/*
Note:
  Enable below preprocessors
    FlashEyeCoreTestConfig.h:
      __CONF_TEST_TASK_COMM_SERIAL_FUNC_ENABLED
    FlashEyesCoreLibConfig.h:
      __CONF_COMM_SERIAL_TASK_FUNC_ENABLED OR __CONF_COMM_SERIAL_WITH_TASK_FUNC_ENABLED
      __CONF_CONSOLE_LOG_FUNC_ENABLED
  How to enable 1 or 2:
    1/ Set in Visual studio
    2/ Define in the noted file
*/

#include "../FlashEyeCoreTestConfig.h"

#ifdef __CONF_TEST_TIMER_MANAGER_FUNC_ENABLED

#ifndef _TASK_TIMER_MANAGER_TEST_H
#define _TASK_TIMER_MANAGER_TEST_H

/////////////////////////////////////////////////
// INCLUDE
#include "TimerManagerTest.h"
#include "../task_manager/OpcodeInputTask.h"
/////////////////////////////////////////////////
// PREPROCESSOR
#define TIMER_MANAGER_TEST_MAIN_CONSOLE_DEBUG_ENABLED
/////////////////////////////////////////////////
// DEFINE

// comm
#define TIMER_MANAGER_TEST_CONTROL_TASK_EVENT_NO      32
#define TIMER_MANAGER_TEST_TASK_EVENT_NO              5
#define TIMER_MANAGER_TEST_USE_POOL                   false

#ifdef SYSTEM_16BIT_SUPPORT
#define TIMER_MANAGER_TEST_TASK_MEM                   2048
#else //SYSTEM_16BIT_SUPPORT
#define TIMER_MANAGER_TEST_TASK_MEM                   256
#endif // SYSTEM_16BIT_SUPPORT
#define TIMER_MANAGER_TEST_TASK_PRIORITY              2

#ifdef SYSTEM_AVR_PLATFORM
#define TIMER_MANAGER_HW_GROUP_ID_DEFAULT             0
#define TIMER_MANAGER_HW_INDEX_ID_DEFAULT             1
#define TIMER_MANAGER_HW_DEVIDER_DEFAULT              8
#elif defined (SYSTEM_ESP_PLATFORM)
#define TIMER_MANAGER_HW_GROUP_ID_DEFAULT             0
#define TIMER_MANAGER_HW_INDEX_ID_DEFAULT             1
//#define TIMER_MANAGER_HW_DEVIDER_DEFAULT              1024
#endif // SYSTEM_AVR_PLATFORM

#define TIMER_MANAGER_HW_GROUP_ID_DEFAULT             0
#define TIMER_MANAGER_HW_INDEX_ID_DEFAULT             1
#define TIMER_MANAGER_HW_DEVIDER_DEFAULT              8

#define OP_QUIT                 9
#define OP_START                1
#define OP_ADD_TIMER            2
#define OP_STOP_TIMER           3
#define OP_REV1                 4
#define OP_REV2                 5
#define OP_STOP                 6

#ifdef SYSTEM_ARDUINO_BASED

#define BUTTON_ISR_TRIGGER_TYPE       FALLING
#ifdef SYSTEM_ESP_PLATFORM
#define PIN_INTERRUPT_QUIT_BUTTON           -1
#define PIN_INTERRUPT_START_BUTTON          5
#define PIN_INTERRUPT_ADD_TIMER_BUTTON      18
#define PIN_INTERRUPT_STOP_TIMER_BUTTON     19
#define PIN_INTERRUPT_REV1_BUTTON           -1
#define PIN_INTERRUPT_REV2_BUTTON           -1
#define PIN_INTERRUPT_STOP_BUTTON           -1
#else // SYSTEM_ESP_PLATFORM

#define PIN_INTERRUPT_QUIT_BUTTON           -1
#define PIN_INTERRUPT_START_BUTTON          20
#define PIN_INTERRUPT_ADD_TIMER_BUTTON      2
#define PIN_INTERRUPT_STOP_TIMER_BUTTON     3
#define PIN_INTERRUPT_REV1_BUTTON           -1
#define PIN_INTERRUPT_REV2_BUTTON           -1
#define PIN_INTERRUPT_STOP_BUTTON           -1
#endif // SYSTEM_ESP_PLATFORM

#else // SYSTEM_ARDUINO_BASED
#define BUTTON_ISR_TRIGGER_TYPE           0
#define PIN_INTERRUPT_QUIT_BUTTON         -1
#define PIN_INTERRUPT_START_BUTTON        -1
#define PIN_INTERRUPT_ADD_TIMER_BUTTON    -1
#define PIN_INTERRUPT_STOP_TIMER_BUTTON   -1
#define PIN_INTERRUPT_REV1_BUTTON         -1
#define PIN_INTERRUPT_REV2_BUTTON         -1
#define PIN_INTERRUPT_STOP_BUTTON         -1
#endif // SYSTEM_ARDUINO_BASED
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
//
class TimerManagerMainTest
{
public:
  TimerManagerMainTest();
  ~TimerManagerMainTest();
  static int setup(void);
  static void loop(void);
  static TimerId_t timerid[TEST_TIMER_COUNT];
protected:
  static bool cbOpCodeReceived(int opCode);
public:
  static TimerManagerTest timer_Manager_Test;
  static TimerInfoTAG timer_List[TEST_TIMER_COUNT];

};

TimerManagerTest TimerManagerMainTest::timer_Manager_Test;
TimerInfoTAG TimerManagerMainTest::timer_List[] = {};
TimerId_t TimerManagerMainTest::timerid[] = {};

TimerManagerMainTest::TimerManagerMainTest()
{

}

TimerManagerMainTest::~TimerManagerMainTest()
{
  //TimerManager::getInstance().finalize();
}

int TimerManagerMainTest::setup()
{
  //CONSOLE_LOG("%s", "test setup");
  int ret = 0;
  //const int timePace = 5;
  do
  {
    {
      TimerManagerMainTest::timer_List[0].enabled = 1;
      TimerManagerMainTest::timer_List[0].handler = TimerManagerTest::cbTimerFired;
      TimerManagerMainTest::timer_List[0].extraArgument = (void*)&TimerManagerMainTest::timer_Manager_Test;
      TimerManagerMainTest::timer_List[0].interval = 5200;
      TimerManagerMainTest::timer_List[0].oneShot = false;

      TimerManagerMainTest::timer_List[1].enabled = 1; 
      TimerManagerMainTest::timer_List[1].handler = TimerManagerTest::cbTimerFired;
      TimerManagerMainTest::timer_List[1].extraArgument = (void*)&TimerManagerMainTest::timer_Manager_Test;
      TimerManagerMainTest::timer_List[1].interval = 6100; // 1ms
      TimerManagerMainTest::timer_List[1].oneShot = false;
    }


    TaskManagerConfigTAG taskConfig = TaskManagerConfigTAG();
    taskConfig.eventUsePool = false;
    taskConfig.eventItemNumber = TIMER_MANAGER_TEST_CONTROL_TASK_EVENT_NO; // TIMER_MANAGER_TEST_TASK_EVENT_NO;
    TaskThreadConfigTAG threadConfig = TaskThreadConfigTAG();
    threadConfig.enabled = true;
    threadConfig.useThreadPool = false;
    threadConfig.usStackDepth = TIMER_MANAGER_TEST_TASK_MEM;
    threadConfig.uxPriority = TIMER_MANAGER_TEST_TASK_PRIORITY;
    {
      TimerControlTaskConfigTAG timerConfig = TimerControlTaskConfigTAG();
      timerConfig.taskManagerConfig = &taskConfig;
      timerConfig.taskThreadConfig = &threadConfig;
      timerConfig.hwTimerGrpId = TIMER_MANAGER_HW_GROUP_ID_DEFAULT;
      timerConfig.hwTimerIndex = TIMER_MANAGER_HW_INDEX_ID_DEFAULT;
      timerConfig.hwTimerDevider = TIMER_MANAGER_HW_DEVIDER_DEFAULT;
      timerConfig.hwClockMHz = F_CPU/1000000;

      if (TimerManager::getInstance().isValid() != false)
      {
#ifdef TIMER_MANAGER_TEST_MAIN_CONSOLE_DEBUG_ENABLED
        CONSOLE_LOG("[ma] %i", -11);
#endif // TIMER_MANAGER_TEST_MAIN_CONSOLE_DEBUG_ENABLED
        return -1; // already config
      }
      
      ret = TimerManager::getInstance().initialize(timerConfig);
      if (ret != 0)
      {
#ifdef TIMER_MANAGER_TEST_MAIN_CONSOLE_DEBUG_ENABLED
        CONSOLE_LOG("[ma] %i", -22);
#endif // TIMER_MANAGER_TEST_MAIN_CONSOLE_DEBUG_ENABLED
        break;
      }

      ret = TimerManagerMainTest::timer_Manager_Test.setConfig(taskConfig);
      if (ret != 0)
      {
#ifdef TIMER_MANAGER_TEST_MAIN_CONSOLE_DEBUG_ENABLED
        CONSOLE_LOG("[ma] %i", -33);
#endif // TIMER_MANAGER_TEST_MAIN_CONSOLE_DEBUG_ENABLED
        break;
      }

    }
    {
      taskConfig.eventItemNumber = 2;
      OpCodeMenuItemTAG menuItem = OpCodeMenuItemTAG();
      menuItem.pin = PIN_INTERRUPT_QUIT_BUTTON;
      menuItem.triggerType = BUTTON_ISR_TRIGGER_TYPE;
      menuItem.opCode = OP_QUIT;
      memcpy(menuItem.description, "quit", SYSTEM_MIN(OPCODE_MENU_ITEM_DESCRIPTION_LEN_MAX, strlen("quit")));

      ret = OpcodeInputTask::getInstance().setConfig(
        &menuItem
        , 1
        , TimerManagerMainTest::cbOpCodeReceived
#ifdef OPCODE_INPUT_TAS_USE_TASK
        , taskConfig
#endif // OPCODE_INPUT_TAS_USE_TASK
      );
      if (ret != 0)
      {
        break;
      }

      menuItem.pin = PIN_INTERRUPT_START_BUTTON;
      menuItem.triggerType = BUTTON_ISR_TRIGGER_TYPE;
      menuItem.opCode = OP_START;
      memset(menuItem.description, 0, OPCODE_MENU_ITEM_DESCRIPTION_LEN_MAX);
      memcpy(menuItem.description, "start", SYSTEM_MIN(OPCODE_MENU_ITEM_DESCRIPTION_LEN_MAX, strlen("start")));
      ret = OpcodeInputTask::getInstance().appendMenuItem(&menuItem);
      if (ret != 0)
      {
        break;
      }

      menuItem.pin = PIN_INTERRUPT_ADD_TIMER_BUTTON;
      menuItem.triggerType = BUTTON_ISR_TRIGGER_TYPE;
      menuItem.opCode = OP_ADD_TIMER;
      memset(menuItem.description, 0, OPCODE_MENU_ITEM_DESCRIPTION_LEN_MAX);
      memcpy(menuItem.description, "add t", SYSTEM_MIN(OPCODE_MENU_ITEM_DESCRIPTION_LEN_MAX, strlen("add t")));
      ret = OpcodeInputTask::getInstance().appendMenuItem(&menuItem);
      if (ret != 0)
      {
        break;
      }

      menuItem.pin = PIN_INTERRUPT_STOP_TIMER_BUTTON;
      menuItem.triggerType = BUTTON_ISR_TRIGGER_TYPE;
      menuItem.opCode = OP_STOP_TIMER;
      memset(menuItem.description, 0, OPCODE_MENU_ITEM_DESCRIPTION_LEN_MAX);
      memcpy(menuItem.description, "stop t", SYSTEM_MIN(OPCODE_MENU_ITEM_DESCRIPTION_LEN_MAX, strlen("stop t")));
      ret = OpcodeInputTask::getInstance().appendMenuItem(&menuItem);
      if (ret != 0)
      {
        break;
      }

      menuItem.pin = PIN_INTERRUPT_REV1_BUTTON;
      menuItem.triggerType = BUTTON_ISR_TRIGGER_TYPE;
      menuItem.opCode = OP_REV1;
      memset(menuItem.description, 0, OPCODE_MENU_ITEM_DESCRIPTION_LEN_MAX);
      memcpy(menuItem.description, "rev1", SYSTEM_MIN(OPCODE_MENU_ITEM_DESCRIPTION_LEN_MAX, strlen("rev1")));
      ret = OpcodeInputTask::getInstance().appendMenuItem(&menuItem);
      if (ret != 0)
      {
        break;
      }

      menuItem.pin = PIN_INTERRUPT_REV2_BUTTON;
      menuItem.triggerType = BUTTON_ISR_TRIGGER_TYPE;
      menuItem.opCode = OP_REV2;
      memset(menuItem.description, 0, OPCODE_MENU_ITEM_DESCRIPTION_LEN_MAX);
      memcpy(menuItem.description, "rev2", SYSTEM_MIN(OPCODE_MENU_ITEM_DESCRIPTION_LEN_MAX, strlen("rev2")));
      ret = OpcodeInputTask::getInstance().appendMenuItem(&menuItem);
      if (ret != 0)
      {
        break;
      }

      menuItem.pin = PIN_INTERRUPT_STOP_BUTTON;
      menuItem.triggerType = BUTTON_ISR_TRIGGER_TYPE;
      menuItem.opCode = OP_STOP;
      memset(menuItem.description, 0, OPCODE_MENU_ITEM_DESCRIPTION_LEN_MAX);
      memcpy(menuItem.description, "stop", SYSTEM_MIN(OPCODE_MENU_ITEM_DESCRIPTION_LEN_MAX, strlen("stop")));
      ret = OpcodeInputTask::getInstance().appendMenuItem(&menuItem);
      if (ret != 0)
      {
        break;
      }

      menuItem.pin = PIN_INTERRUPT_QUIT_BUTTON;
      menuItem.triggerType = BUTTON_ISR_TRIGGER_TYPE;
      menuItem.opCode = OP_QUIT;
      memset(menuItem.description, 0, OPCODE_MENU_ITEM_DESCRIPTION_LEN_MAX);
      memcpy(menuItem.description, "quit", SYSTEM_MIN(OPCODE_MENU_ITEM_DESCRIPTION_LEN_MAX, strlen("quit")));
      ret = OpcodeInputTask::getInstance().appendMenuItem(&menuItem);
      if (ret != 0)
      {
        break;
      }
    }

#ifdef SYSTEM_ARDUINO_BASED
    {
      ret = TimerManagerMainTest::timer_Manager_Test.start(threadConfig);
      if (ret != 0)
      {
        //CONSOLE_LOG("set %i", -33);
        break;
        //CONSOLE_LOG("start %i", ret);
      }
    }

#endif // SYSTEM_ARDUINO_BASED
    //CONSOLE_LOG("set %i", 99);
    return 0;
  } while (0);

  CONSOLE_LOG("set %i", -1);
  return -1;
}

void TimerManagerMainTest::loop()
{
  //CONSOLE_LOG("%s","test loop");
  do
  {
    int ret = 0;
#ifdef OPCODE_INPUT_TAS_USE_TASK
    {
      TaskThreadConfigTAG threadConfig = TaskThreadConfigTAG();
      threadConfig.enabled = true;
      threadConfig.usStackDepth = TIMER_MANAGER_TEST_TASK_MEM;
      threadConfig.uxPriority = TIMER_MANAGER_TEST_TASK_PRIORITY;
      threadConfig.useThreadPool = TaskThreadPoolManager::getInstance().isValid();
      ret = OpcodeInputTask::getInstance().start(threadConfig);
    }
#else // OPCODE_INPUT_TAS_USE_TASK
    ret = OpcodeInputTask::getInstance().start();
#endif // OPCODE_INPUT_TAS_USE_TASK
    if (ret != 0)
    {
      break;
    }
  } while (0);
}
extern unsigned int countT[TEST_TIMER_COUNT];
#ifdef TIMER_MANAGER_TEST_NO_BUTTON
void startTestTimers(void)
{
  memset(&countT, 0, sizeof(unsigned int) * TEST_TIMER_COUNT);
  TimerManagerMainTest::timerid[0] = TimerManager::getInstance().start(TimerManagerMainTest::timer_List[0]);
  //SYSTEM_SLEEP(1);
  TimerManagerMainTest::timerid[1] = TimerManager::getInstance().start(TimerManagerMainTest::timer_List[1]);
  CONSOLE_LOG("[timM] aT %i %i", TimerManagerMainTest::timerid[0], TimerManagerMainTest::timerid[1]);
}
#endif // TIMER_MANAGER_TEST_NO_BUTTON
bool TimerManagerMainTest::cbOpCodeReceived(int opCode)
{
  CONSOLE_LOG("[tmt] oprev %i", opCode);
  bool isStopped = false;
  int ret = 0;
  //static byte sequenceId = 124;
  do
  {
    switch (opCode)
    {
    case OP_START:
    {
      TaskThreadConfigTAG threadConfig = TaskThreadConfigTAG();
      threadConfig.enabled = true;
      threadConfig.usStackDepth = TIMER_MANAGER_TEST_TASK_MEM;
      threadConfig.uxPriority = TIMER_MANAGER_TEST_TASK_PRIORITY;
      threadConfig.useThreadPool = TaskThreadPoolManager::getInstance().isValid();
      ret = TimerManagerMainTest::timer_Manager_Test.start(threadConfig);
      break;
    }
    case OP_ADD_TIMER:
    {
      memset(&countT, 0, sizeof(unsigned int)* TEST_TIMER_COUNT);
      TimerManagerMainTest::timerid[0] = TimerManager::getInstance().start(TimerManagerMainTest::timer_List[0]);
      TimerManagerMainTest::timerid[1] = TimerManager::getInstance().start(TimerManagerMainTest::timer_List[1]);
      CONSOLE_LOG("[timM] aT %i %i", TimerManagerMainTest::timerid[0], TimerManagerMainTest::timerid[1]);
      break;
    }
    case OP_STOP_TIMER:
    {
      TimerManager::getInstance().stop(TimerManagerMainTest::timerid[0]);
      TimerManager::getInstance().stop(TimerManagerMainTest::timerid[1]);
      CONSOLE_LOG("[timM] sT %i %i", TimerManagerMainTest::timerid[0], TimerManagerMainTest::timerid[1]);
      break;
    }
    case OP_REV1:
    {
      
    }
    case OP_REV2:
    {

    }
    case OP_STOP:
      TimerManagerMainTest::timer_Manager_Test.stop();
      break;
    case OP_QUIT:
      isStopped = true;
      TimerManagerMainTest::timer_Manager_Test.stop();
      break;
    default:
      break;
    }
    CONSOLE_LOG("opt: %i", opCode);
    if (ret != 0)
    {
      CONSOLE_LOG("error(%i)", ret);
    }
    return isStopped;
  } while (0);
  return true;
}
#endif // _TASK_COMM_SERIAL_TEST_H

#endif // _CONF_TEST_TASK_COMM_SERIAL_ENABLED