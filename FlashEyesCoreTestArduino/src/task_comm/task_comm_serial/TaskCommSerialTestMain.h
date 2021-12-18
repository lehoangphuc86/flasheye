
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


#include "../../FlashEyeCoreTestConfig.h"

#ifdef _CONF_TEST_TASK_COMM_SERIAL_ENABLED

#ifndef _TASK_COMM_SERIAL_TEST_H
#define _TASK_COMM_SERIAL_TEST_H

/////////////////////////////////////////////////
// INCLUDE
#include "CommSerialTest.h"
#include "../../task_manager/OpcodeInputTask.h"
/////////////////////////////////////////////////
// PREPROCESSOR

/////////////////////////////////////////////////
// DEFINE

// comm
#define COMM_SERIAL_TEST_TERMINATOR                   COMM_SERIAL_TERMINATOR_DEFAULT
#define COMM_SERIAL_TEST_IS_HEADERLESS                true
#define COMM_SERIAL_TEST_USE_CHECKSUM                 true
#define COMM_SERIAL_TEST_CODEC_TYPE                   (byte)TaskCodecConstant::TaskCodecType::TASK_CODEC_NONE
#define COMM_SERIAL_TEST_MAX_BUFFER_SIZE              COMM_SERIAL_SYS_BUFFER_LEN*2
#define COMM_SERIAL_TEST_MAX_BUFFER_COUNT             5
// for arduino only
//#define COMM_SERIAL_TEST_TASK_REV_POLLING_TIME        10
#define COMM_SERIAL_TEST_TASK_EVENT_NUM               5
#ifdef SYSTEM_16BIT_SUPPORT
#define COMM_SERIAL_TEST_TASK_MEM                     2048
#else //SYSTEM_16BIT_SUPPORT
#define COMM_SERIAL_TEST_TASK_MEM                     256
#endif // SYSTEM_16BIT_SUPPORT

#define COMM_SERIAL_TEST_TASK_PRIORITY                2
#define TEST_THREAD_POOL_COUNT                        2
#define TEST_BUFFER_POOL_COUNT                        2
#define TEST_EVENT_POOL_COUNT                         2



// socket
#define COMM_SERIAL_TEST_BYTE_SIZE                    8
#define COMM_SERIAL_TEST_PARITY_TYPE                  (byte)CommSerialConstant::SerialParityType::SERIAL_PARITY_NONE
#define COMM_SERIAL_TEST_STOP_BIT_NUM                 1
#define COMM_SERIAL_TEST_BAUDRATE                     9600
#define COMM_SERIAL_TEST_TIMEOUT                      100
// arudino
#define COMM_SERIAL_TEST_INSTANCE_INDX                1
#define COMM_SERIAL_TEST_PIN_TX                       22
#define COMM_SERIAL_TEST_PIN_RX                       21

// pc
#ifdef SYSTEM_WIN_PLATFORM
#define COMM_SERIAL_TEST_COMM_PORT                    "COM3" // "COM5"
#else // SYSTEM_WIN_PLATFORM
#define COMM_SERIAL_TEST_COMM_PORT                    "/dev/ttyACM0"
#endif // SYSTEM_WIN_PLATFORM

//#define COMM_SERIAL_TEST_REV_WAIT_TIMEOUT             0

#define OP_QUIT                 9
#define OP_START                1
#define OP_CONN                 2
#define OP_SEND                 3
#define OP_READ                 4
#define OP_DISC                 5
#define OP_STOP                 6



#ifdef SYSTEM_ARDUINO_BASED

#define BUTTON_ISR_TRIGGER_TYPE       FALLING

#ifdef SYSTEM_ESP_PLATFORM
#define PIN_INTERRUPT_QUIT_BUTTON     -1
#define PIN_INTERRUPT_START_BUTTON    -1
#define PIN_INTERRUPT_CONN_BUTTON     5
#define PIN_INTERRUPT_READ_BUTTON     18
#define PIN_INTERRUPT_SEND_BUTTON     19
#define PIN_INTERRUPT_DIS_BUTTON      -1
#define PIN_INTERRUPT_STOP_BUTTON     -1
#else // SYSTEM_ESP_PLATFORM

#define PIN_INTERRUPT_QUIT_BUTTON     -1
#define PIN_INTERRUPT_START_BUTTON    -1
#define PIN_INTERRUPT_CONN_BUTTON     20
#define PIN_INTERRUPT_READ_BUTTON     2
#define PIN_INTERRUPT_SEND_BUTTON     3
#define PIN_INTERRUPT_DIS_BUTTON      -1
#define PIN_INTERRUPT_STOP_BUTTON     -1
#endif // SYSTEM_ESP_PLATFORM

#else // SYSTEM_ARDUINO_BASED
#define BUTTON_ISR_TRIGGER_TYPE       0
#define PIN_INTERRUPT_QUIT_BUTTON     -1
#define PIN_INTERRUPT_START_BUTTON    -1
#define PIN_INTERRUPT_CONN_BUTTON     -1
#define PIN_INTERRUPT_READ_BUTTON     -1
#define PIN_INTERRUPT_SEND_BUTTON     -1
#define PIN_INTERRUPT_DIS_BUTTON      -1
#define PIN_INTERRUPT_STOP_BUTTON     -1
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
class TaskCommSerialTest
{
public:
  TaskCommSerialTest();
  ~TaskCommSerialTest();
  static int setup(void);
  static void loop(void);
protected:
  static bool cbOpCodeReceived(int opCode);

protected:
#if (1)
  static CommSerialTest comm_Serial_Test;
#endif //0
};

#if (1)
CommSerialTest TaskCommSerialTest::comm_Serial_Test(1);
#endif //0

TaskCommSerialTest::TaskCommSerialTest()
{

}

TaskCommSerialTest::~TaskCommSerialTest()
{

}

#if (1)
int TaskCommSerialTest::setup()
{
  //CONSOLE_LOG("%s", "test setup");
  int ret = 0;
  //const int timePace = 5;
  do
  {
    // prepare pools
    //#thread pool
    if (TEST_THREAD_POOL_COUNT > 0)
    {
      TaskThreadManagerConfigTAG threadPoolConfig[TEST_THREAD_POOL_COUNT];
      memset(&threadPoolConfig, 0, sizeof(TaskThreadManagerConfigTAG) * TEST_THREAD_POOL_COUNT);
      threadPoolConfig[0].preparedDataNumber = 2;
      threadPoolConfig[0].dataItemConfig.enabled = true;
      threadPoolConfig[0].dataItemConfig.proc = NULL;
      threadPoolConfig[0].dataItemConfig.procParams = NULL;
      threadPoolConfig[0].dataItemConfig.usStackDepth = COMM_SERIAL_TEST_TASK_MEM;
      threadPoolConfig[0].dataItemConfig.uxPriority = COMM_SERIAL_TEST_TASK_PRIORITY;

      memcpy(&threadPoolConfig[1], &threadPoolConfig[0], sizeof(TaskThreadManagerConfigTAG));
      threadPoolConfig[1].preparedDataNumber = 3;
      threadPoolConfig[1].dataItemConfig.usStackDepth = COMM_SERIAL_TEST_TASK_MEM + 10;

      ret = TaskThreadPoolManager::getInstance().prepare(2, threadPoolConfig);
      if (ret != 0)
      {
        CONSOLE_LOG("[tmt] set tPM %i", -1);
        break;
      }
    }

    // #buffer pool
    if (TEST_BUFFER_POOL_COUNT > 0)
    {
      BufferDataManagerConfigTAG bufferPoolConfig[TEST_BUFFER_POOL_COUNT];
      memset(&bufferPoolConfig, 0, sizeof(BufferDataManagerConfigTAG) * TEST_BUFFER_POOL_COUNT);
      bufferPoolConfig[0].preparedDataNumber = COMM_SERIAL_TEST_MAX_BUFFER_COUNT - 2;
      bufferPoolConfig[0].dataItemConfig.bufferSize = COMM_SERIAL_TEST_MAX_BUFFER_SIZE;
      
      memcpy(&bufferPoolConfig[1], &bufferPoolConfig[0], sizeof(BufferDataManagerConfigTAG));
      bufferPoolConfig[1].preparedDataNumber = 2;
      bufferPoolConfig[1].dataItemConfig.bufferSize = COMM_SERIAL_TEST_MAX_BUFFER_SIZE + 10;

      ret = BufferDataPoolManager::getInstance().prepare(2, bufferPoolConfig);
      if (ret != 0)
      {
        CONSOLE_LOG("[tmt] set bPM %i", -1);
        break;
      }
    }

    //{
    //  BufferDataManager testDataManger;
    //  BufferDataManagerConfigTAG bufferManagerConfig = BufferDataManagerConfigTAG();
    //  bufferManagerConfig.dataItemConfig.bufferSize = COMM_SERIAL_TEST_MAX_BUFFER_SIZE;
    //  bufferManagerConfig.preparedDataNumber = 4;
    //  bufferManagerConfig.usePool = BufferDataPoolManager::getInstance().isValid();
    //  ret = testDataManger.prepare<BufferDataItem, BufferDataItemConfigTAG, BufferDataManagerPropTAG>(bufferManagerConfig);
    //  if (ret != 0)
    //  {
    //    CONSOLE_LOG("[tmt] test bDM %i", -1);
    //    break;
    //  }
    //}


    // #event pool
    if (TEST_EVENT_POOL_COUNT > 0)
    {
      EventManagerConfigTAG eventPoolConfig[TEST_EVENT_POOL_COUNT + 1];
      memset(&eventPoolConfig, 0, sizeof(EventManagerConfigTAG) * TEST_EVENT_POOL_COUNT);
      eventPoolConfig[0].preparedDataNumber = COMM_SERIAL_TEST_TASK_EVENT_NUM*3;
      eventPoolConfig[0].dataItemConfig.eventItemSize = sizeof(EventCommSerialConnectionClosedTAG);

      memcpy(&eventPoolConfig[1], &eventPoolConfig[0], sizeof(EventManagerConfigTAG));
      eventPoolConfig[1].preparedDataNumber = COMM_SERIAL_TEST_TASK_EVENT_NUM + 2;
      eventPoolConfig[1].dataItemConfig.eventItemSize = sizeof(EventCommSerialConnectionClosedTAG) + 1;

      ret = EventPoolManager::getInstance().prepare(2, eventPoolConfig);
      if (ret != 0)
      {
        CONSOLE_LOG("[tmt] set ePM %i", -1);
        break;
      }
    }

    //{
    //  EventManager testEventManger;
    //  ret = testEventManger.prepare(6, sizeof(EventCommConnectionClosedTAG), EventPoolManager::getInstance().isValid());
    //  if (ret != 0)
    //  {
    //    CONSOLE_LOG("[tmt] test bDM %i", -1);
    //    break;
    //  }
    //}

#if defined(COMM_SERIAL_USE_TASK) || defined(OPCODE_INPUT_TAS_USE_TASK)
    TaskManagerConfigTAG taskConfig = TaskManagerConfigTAG();
    taskConfig.eventUsePool = EventPoolManager::getInstance().isValid();
#endif // COMM_SERIAL_USE_TASK

    {
      CommSerialConfigTAG config = CommSerialConfigTAG();
      config.enabled(true);
      config.terminator = COMM_SERIAL_TEST_TERMINATOR;
      config.isHeaderless(true);
      config.useCheckSum(true);
      config.codecType = COMM_SERIAL_TEST_CODEC_TYPE;
      config.bufferMaxSize = COMM_SERIAL_TEST_MAX_BUFFER_SIZE;
      config.bufferCount = COMM_SERIAL_TEST_MAX_BUFFER_COUNT;
      config.bufferUsePool(BufferDataPoolManager::getInstance().isValid());
      config.cbParams = &TaskCommSerialTest::comm_Serial_Test;
#ifdef COMM_SERIAL_USE_TASK
      config.cbDataReceived = CommSerialTest::cbOnDataReceivedNavigator;
      config.cbConnectionClosed = CommSerialTest::cbOnConnectionClosedNavigator;
#else // COMM_SERIAL_USE_TASK
      config.cbDataReceived = NULL;
      config.cbConnectionClosed = NULL;
#endif // COMM_SERIAL_USE_TASK
      // use task
      // used for sender/receiver/serial when connecting
      config.taskEventItemNumber = COMM_SERIAL_TEST_TASK_EVENT_NUM;
      config.taskThreadUsePool(TaskThreadPoolManager::getInstance().isValid());
      config.taskEventUsePool(EventPoolManager::getInstance().isValid());
      // for arduino only
      config.taskStackDepth = COMM_SERIAL_TEST_TASK_MEM;
      config.taskPriority = COMM_SERIAL_TEST_TASK_PRIORITY;
      
#ifdef COMM_SERIAL_USE_TASK
      // this is used for task serial test task 
      taskConfig.eventItemNumber = COMM_SERIAL_TEST_TASK_EVENT_NUM;
      ret = TaskCommSerialTest::comm_Serial_Test.setConfig(config, taskConfig);
#else // COMM_SERIAL_USE_TASK
      ret = TaskCommSerialTest::comm_Serial_Test.setConfig(config);
#endif // COMM_SERIAL_USE_TASK
      if (ret != 0)
      {
        break;
      }

    }
    {
#ifdef OPCODE_INPUT_TAS_USE_TASK
      taskConfig.eventItemNumber = 2;
#endif // OPCODE_INPUT_TAS_USE_TASK

      OpCodeMenuItemTAG menuItem = OpCodeMenuItemTAG();
      menuItem.pin = PIN_INTERRUPT_QUIT_BUTTON;
      menuItem.triggerType = BUTTON_ISR_TRIGGER_TYPE;
      menuItem.opCode = OP_QUIT;
      memcpy(menuItem.description, "quit", SYSTEM_MIN(OPCODE_MENU_ITEM_DESCRIPTION_LEN_MAX, strlen("quit")));

      ret = OpcodeInputTask::getInstance().setConfig(
        &menuItem
        , 1
        , TaskCommSerialTest::cbOpCodeReceived
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

      menuItem.pin = PIN_INTERRUPT_CONN_BUTTON;
      menuItem.triggerType = BUTTON_ISR_TRIGGER_TYPE;
      menuItem.opCode = OP_CONN;
      memset(menuItem.description, 0, OPCODE_MENU_ITEM_DESCRIPTION_LEN_MAX);
      memcpy(menuItem.description, "conn", SYSTEM_MIN(OPCODE_MENU_ITEM_DESCRIPTION_LEN_MAX, strlen("conn")));
      ret = OpcodeInputTask::getInstance().appendMenuItem(&menuItem);
      if (ret != 0)
      {
        break;
      }

      menuItem.pin = PIN_INTERRUPT_READ_BUTTON;
      menuItem.triggerType = BUTTON_ISR_TRIGGER_TYPE;
      menuItem.opCode = OP_READ;
      memset(menuItem.description, 0, OPCODE_MENU_ITEM_DESCRIPTION_LEN_MAX);
      memcpy(menuItem.description, "read", SYSTEM_MIN(OPCODE_MENU_ITEM_DESCRIPTION_LEN_MAX, strlen("read")));
      ret = OpcodeInputTask::getInstance().appendMenuItem(&menuItem);
      if (ret != 0)
      {
        break;
      }

      menuItem.pin = PIN_INTERRUPT_SEND_BUTTON;
      menuItem.triggerType = BUTTON_ISR_TRIGGER_TYPE;
      menuItem.opCode = OP_SEND;
      memset(menuItem.description, 0, OPCODE_MENU_ITEM_DESCRIPTION_LEN_MAX);
      memcpy(menuItem.description, "send", SYSTEM_MIN(OPCODE_MENU_ITEM_DESCRIPTION_LEN_MAX, strlen("send")));
      ret = OpcodeInputTask::getInstance().appendMenuItem(&menuItem);
      if (ret != 0)
      {
        break;
      }

      menuItem.pin = PIN_INTERRUPT_DIS_BUTTON;
      menuItem.triggerType = BUTTON_ISR_TRIGGER_TYPE;
      menuItem.opCode = OP_DISC;
      memset(menuItem.description, 0, OPCODE_MENU_ITEM_DESCRIPTION_LEN_MAX);
      memcpy(menuItem.description, "disc", SYSTEM_MIN(OPCODE_MENU_ITEM_DESCRIPTION_LEN_MAX, strlen("disc")));
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
    }

#ifdef SYSTEM_ARDUINO_BASED
    {
#ifdef COMM_SERIAL_USE_TASK
      TaskThreadConfigTAG threadConfig = TaskThreadConfigTAG();
      threadConfig.enabled = true;
      threadConfig.usStackDepth = COMM_SERIAL_TEST_TASK_MEM;
      threadConfig.uxPriority = COMM_SERIAL_TEST_TASK_PRIORITY;
      threadConfig.useThreadPool = TaskThreadPoolManager::getInstance().isValid();
      ret = TaskCommSerialTest::comm_Serial_Test.start(threadConfig);
#else // COMM_SERIAL_USE_TASK
    ret = TaskCommSerialTest::comm_Serial_Test.start();
#endif // COMM_SERIAL_USE_TASK
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
#else // 0
int TaskCommSerialTest::setup()
{
  return 0;
}
#endif // 0
void TaskCommSerialTest::loop()
{
  //CONSOLE_LOG("%s","test loop");
  do
  {
    int ret = 0;
#ifdef OPCODE_INPUT_TAS_USE_TASK
    {
      TaskThreadConfigTAG threadConfig = TaskThreadConfigTAG();
      threadConfig.enabled = true;
      threadConfig.usStackDepth = COMM_SERIAL_TEST_TASK_MEM;
      threadConfig.uxPriority = COMM_SERIAL_TEST_TASK_PRIORITY;
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

#if (1)
bool TaskCommSerialTest::cbOpCodeReceived(int opCode)
{
  CONSOLE_LOG("[tmt] oprev %i", opCode);
  bool isStopped = false;
  int ret = 0;
  static byte sequenceId = 124;
#ifdef SYSTEM_ARDUINO_BASED
  static bool connected = false;
  // in arudino, there is no enough pin for disconnect
  if ((PIN_INTERRUPT_DIS_BUTTON < 0)
    && ( opCode == OP_CONN)
    )
  {
    opCode = (connected == false? OP_CONN: OP_DISC);
    connected = !connected;
  }
#endif // SYSTEM_ARDUINO_BASED
  
  do
  {
    switch (opCode)
    {
    case OP_START:
    {
#ifdef COMM_SERIAL_USE_TASK
      TaskThreadConfigTAG threadConfig = TaskThreadConfigTAG();
      threadConfig.enabled = true;
      threadConfig.usStackDepth = COMM_SERIAL_TEST_TASK_MEM;
      threadConfig.uxPriority = COMM_SERIAL_TEST_TASK_PRIORITY;
      threadConfig.useThreadPool = TaskThreadPoolManager::getInstance().isValid();
      ret = TaskCommSerialTest::comm_Serial_Test.start(threadConfig);
#else // COMM_SERIAL_USE_TASK
      ret = TaskCommSerialTest::comm_Serial_Test.start();
#endif // COMM_SERIAL_USE_TASK
      break;
    }
    case OP_CONN:
    {
      CommSerialSocketConfigTAG socketConfig = CommSerialSocketConfigTAG();
      socketConfig.enabled = true;
      socketConfig.bitPerByte = COMM_SERIAL_TEST_BYTE_SIZE;
      socketConfig.parityType = COMM_SERIAL_TEST_PARITY_TYPE;
      socketConfig.stopBitNum = COMM_SERIAL_TEST_STOP_BIT_NUM;
      socketConfig.baudrate = COMM_SERIAL_TEST_BAUDRATE;
      socketConfig.timeout = COMM_SERIAL_TEST_TIMEOUT;
      // arudino
      socketConfig.instanceIndex = COMM_SERIAL_TEST_INSTANCE_INDX;
      socketConfig.pin_TX = COMM_SERIAL_TEST_PIN_TX;
      socketConfig.pin_RX = COMM_SERIAL_TEST_PIN_RX;
      // pc
      strncpy(socketConfig.port, COMM_SERIAL_TEST_COMM_PORT, COMM_SERIAL_PORT_LEN_MAX - 1);
      ret = TaskCommSerialTest::comm_Serial_Test.connect(socketConfig);
      break;
    }
    case OP_SEND:
      ret = TaskCommSerialTest::comm_Serial_Test.send(++sequenceId);
      break;
    case OP_READ:
      ret = TaskCommSerialTest::comm_Serial_Test.read(1);
      break;
    case OP_DISC:
      ret = TaskCommSerialTest::comm_Serial_Test.disconnect();
      break;
    case OP_STOP:
      ret = TaskCommSerialTest::comm_Serial_Test.stop();
      break;
    case OP_QUIT:
      isStopped = true;
      TaskCommSerialTest::comm_Serial_Test.stop();
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
#else // 0
bool TaskCommSerialTest::cbOpCodeReceived(int opCode)
{
  return false;
}
#endif // 0
#endif // _TASK_COMM_SERIAL_TEST_H

#endif // _CONF_TEST_TASK_COMM_SERIAL_ENABLED