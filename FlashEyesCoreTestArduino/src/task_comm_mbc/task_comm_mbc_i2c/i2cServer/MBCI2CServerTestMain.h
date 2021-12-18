#include "../MBCI2CTestConstant.h"
#ifdef _CONF_TEST_TASK_COMM_MBC_I2C_SERVER_ENABLED

#ifndef _MBC_I2C_SERVER_TEST_MAIN_H
#define _MBC_I2C_SERVER_TEST_MAIN_H

/////////////////////////////////////////////////
// INCLUDE

#include "MBCI2CServerTask.h"
#include "../../../task_manager/OpcodeInputTask.h"
/////////////////////////////////////////////////
// PREPROCESSOR
#define MBC_I2C_SERVER_TEST_DEBUG_CONSOLE_ENABLED
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


/////////////////////////////////////////////////
// STATIC DATA

/////////////////////////////////////////////////
// STATIC FUNCTIONS

/////////////////////////////////////////////////
// EXTERN

/////////////////////////////////////////////////
// CLASS DEFINITION

class MBCI2CServerTest
{
public:
  MBCI2CServerTest(void);
  ~MBCI2CServerTest(void);
  static int setup(void);
  static void loop(void);
protected:
  static bool cbOpCodeReceived(int opCode);

protected:
#if (1)
  static MBCI2CServerTask comm_I2C_Test;
#endif //0
};

MBCI2CServerTask MBCI2CServerTest::comm_I2C_Test;

MBCI2CServerTest::MBCI2CServerTest()
{

}

MBCI2CServerTest::~MBCI2CServerTest()
{

}

int MBCI2CServerTest::setup()
{
  int ret = 0;
  //const int timePace = 5;
  do
  {
    //{
    //  bool retB = Wire.begin(); // Wire communication begin
    //  CONSOLE_LOG("[set1] w %i", retB);
    //}
    // prepare pools
    //#thread pool
#if (MBC_TEST_THREAD_POOL_COUNT)
    {
      TaskThreadManagerConfigTAG threadPoolConfig[MBC_TEST_THREAD_POOL_COUNT];
      memset(&threadPoolConfig, 0, sizeof(TaskThreadManagerConfigTAG) * MBC_TEST_THREAD_POOL_COUNT);
      threadPoolConfig[0].preparedDataNumber = 1;
      threadPoolConfig[0].dataItemConfig.enabled = true;
      threadPoolConfig[0].dataItemConfig.proc = NULL;
      threadPoolConfig[0].dataItemConfig.procParams = NULL;
      threadPoolConfig[0].dataItemConfig.usStackDepth = MBC_I2C_COMM_TEST_TASK_MEM;
      threadPoolConfig[0].dataItemConfig.uxPriority = MBC_I2C_COMM_TEST_TASK_PRIORITY;

      memcpy(&threadPoolConfig[1], &threadPoolConfig[0], sizeof(TaskThreadManagerConfigTAG));
      threadPoolConfig[1].preparedDataNumber = 1;
      threadPoolConfig[1].dataItemConfig.usStackDepth = MBC_I2C_COMM_TEST_TASK_MEM + 10;

      ret = TaskThreadPoolManager::getInstance().prepare(MBC_TEST_THREAD_POOL_COUNT, threadPoolConfig);
      if (ret != 0)
      {
        break;
      }
    }
#endif // (TEST_THREAD_POOL_COUNT)
    // #buffer pool
#if (MBC_TEST_BUFFER_POOL_COUNT)
    {
      BufferDataManagerConfigTAG bufferPoolConfig[MBC_TEST_BUFFER_POOL_COUNT];
      memset(&bufferPoolConfig, 0, sizeof(BufferDataManagerConfigTAG) * MBC_TEST_BUFFER_POOL_COUNT);
      bufferPoolConfig[0].preparedDataNumber = MBC_I2C_COMM_TEST_SERVER_BUFFER_COUNT; // i2cClient
      DataSize_t maxSize = (DataSize_t)CommMBCProcessor::getMaxProceededSize(CommMBCConstant::CommMBCMessageId::CommMBCStart1);
      maxSize = SYSTEM_MAX(maxSize, (DataSize_t)CommMBCProcessor::getMaxProceededSize(CommMBCConstant::CommMBCMessageId::CommMBCResult1));
      maxSize = SYSTEM_MAX(maxSize, (DataSize_t)sizeof(CommHeaderlesstDataTAG));
      bufferPoolConfig[0].dataItemConfig.bufferSize = maxSize;

      memcpy(&bufferPoolConfig[1], &bufferPoolConfig[0], sizeof(BufferDataManagerConfigTAG));
      bufferPoolConfig[1].preparedDataNumber = 2;
      bufferPoolConfig[1].dataItemConfig.bufferSize = TaskCodec::calculateEncodeSize((byte)MBC_I2C_COMM_TEST_SERVER_CODECTYPE, maxSize);

      ret = BufferDataPoolManager::getInstance().prepare(MBC_TEST_BUFFER_POOL_COUNT, bufferPoolConfig);
      if (ret != 0)
      {
        break;
      }
    }
#endif // (MBC_TEST_BUFFER_POOL_COUNT)
    // #event pool
#if (MBC_TEST_EVENT_POOL_COUNT)
    {
      EventManagerConfigTAG eventPoolConfig[MBC_TEST_EVENT_POOL_COUNT + 1];
      memset(&eventPoolConfig, 0, sizeof(EventManagerConfigTAG) * MBC_TEST_EVENT_POOL_COUNT);
      eventPoolConfig[0].preparedDataNumber = MBC_I2C_COMM_TEST_TASK_EVENT_NO;
      eventPoolConfig[0].dataItemConfig.eventItemSize = sizeof(EventPrepareCompletedTAG); // EventButtonPressedParamsTAG);
     
      memcpy(&eventPoolConfig[1], &eventPoolConfig[0], sizeof(EventManagerConfigTAG));
      eventPoolConfig[1].preparedDataNumber = MBC_I2C_COMM_TEST_TASK_EVENT_NO;
      eventPoolConfig[1].dataItemConfig.eventItemSize = sizeof(EventCommMBCI2CServerDataRequestedTAG);

      ret = EventPoolManager::getInstance().prepare(MBC_TEST_EVENT_POOL_COUNT, eventPoolConfig);
      if (ret != 0)
      {
        break;
      }
    }
#endif // (MBC_TEST_EVENT_POOL_COUNT)

#if defined(MBC_I2C_COMM_TEST_SERVER_USE_TASK) || defined(OPCODE_INPUT_TAS_USE_TASK)
    // Set up tasks
    TaskManagerConfigTAG taskManagerConfig = TaskManagerConfigTAG();
    memset(&taskManagerConfig, 0, sizeof(TaskManagerConfigTAG));
    taskManagerConfig.eventUsePool = TaskThreadPoolManager::getInstance().isValid();
#endif // defined(I2C_COMM_TEST_SERVER_USE_TASK) || defined(OPCODE_INPUT_TAS_USE_TASK)
    
    // set up server
    {
      CommMBCI2CConfigTAG commConfig = CommMBCI2CConfigTAG();
      memset(&commConfig, 0, sizeof(CommMBCI2CConfigTAG));
      commConfig.enabled(true);
      commConfig.bufferCount = MBC_I2C_COMM_TEST_SERVER_BUFFER_COUNT;
      commConfig.bufferMaxSize = 0;// no need
      commConfig.bufferUsePool(BufferDataPoolManager::getInstance().isValid());
      commConfig.maxClientNumber = MBC_I2C_COMM_TEST_SERVER_MAX_CLIENT_COUNT;

      commConfig.isHeaderless(MBC_I2C_COMM_TEST_SERVER_CONNECTION_HEADERLESS); // not used
      commConfig.useCheckSum(MBC_I2C_COMM_TEST_CLIENT_CHECKSUM); // not used
      commConfig.codecType = MBC_I2C_COMM_TEST_SERVER_CODECTYPE; // max mem consuming codec type
      commConfig.parentPointer = NULL; // not used
      commConfig.cbDataReceived = NULL; // not used
      commConfig.cbDataRequested = NULL; // not used
      
#if defined(MBC_I2C_COMM_TEST_SERVER_USE_TASK)
      taskManagerConfig.eventItemNumber = MBC_I2C_COMM_TEST_TASK_EVENT_NO;
      
      ret = MBCI2CServerTest::comm_I2C_Test.setConfig(commConfig, taskManagerConfig);
#else // MBC_I2C_COMM_TEST_SERVER_USE_TASK
      ret = MBCI2CServerTest::comm_I2C_Test.setConfig(commConfig);
#endif // MBC_I2C_COMM_TEST_SERVER_USE_TASK

      if (ret != 0)
      {
        break;
      }
    }


    {
#ifdef OPCODE_INPUT_TAS_USE_TASK
      taskManagerConfig.eventItemNumber = 2;
#endif // OPCODE_INPUT_TAS_USE_TASK

      OpCodeMenuItemTAG menuItem = OpCodeMenuItemTAG();
      menuItem.pin = PIN_INTERRUPT_SEND_BUTTON;
      menuItem.triggerType = BUTTON_ISR_TRIGGER_TYPE;
      menuItem.opCode = OP_SEND;
      memcpy(menuItem.description, "send", SYSTEM_MIN(OPCODE_MENU_ITEM_DESCRIPTION_LEN_MAX, strlen("send")));

      ret = OpcodeInputTask::getInstance().setConfig(
        &menuItem
        , 1
        , MBCI2CServerTest::cbOpCodeReceived
#ifdef OPCODE_INPUT_TAS_USE_TASK
        , taskManagerConfig
#endif // OPCODE_INPUT_TAS_USE_TASK
      );
      if (ret != 0)
      {
        break;
      }

      menuItem.pin = PIN_INTERRUPT_REQUEST_BUTTON;
      menuItem.triggerType = BUTTON_ISR_TRIGGER_TYPE;
      menuItem.opCode = OP_REQUEST;
      memset(menuItem.description, 0, OPCODE_MENU_ITEM_DESCRIPTION_LEN_MAX);
      memcpy(menuItem.description, "request", SYSTEM_MIN(OPCODE_MENU_ITEM_DESCRIPTION_LEN_MAX, strlen("request")));
      ret = OpcodeInputTask::getInstance().appendMenuItem(&menuItem);
      if (ret != 0)
      {
        break;
      }
    }

    {
      CommMBCI2CSocketConfigTAG commI2CSocketConfig = CommMBCI2CSocketConfigTAG();
      commI2CSocketConfig.busNumer = MBC_I2C_COMM_TEST_SERVER_SOCKET_BUS_NUMBER;
      commI2CSocketConfig.pin_SCL = MBC_I2C_COMM_TEST_SERVER_SOCKET_PIN_SCL_0;
      commI2CSocketConfig.pin_SDA = MBC_I2C_COMM_TEST_SERVER_SOCKET_PIN_SDA_0;
      commI2CSocketConfig.frequency = MBC_I2C_COMM_TEST_SERVER_SOCKET_FREQUENCY;
      commI2CSocketConfig.address = MBC_I2C_COMM_TEST_SERVER_SOCKET_ADDRESS;
      commI2CSocketConfig.timeout = MBC_I2C_COMM_TEST_SERVER_SOCKET_TIMEOUT;
#ifdef MBC_I2C_COMM_TEST_SERVER_USE_TASK
      TaskThreadConfigTAG threadConfig = TaskThreadConfigTAG();
      threadConfig.enabled = true;
      threadConfig.usStackDepth = MBC_I2C_COMM_TEST_TASK_MEM;
      threadConfig.uxPriority = MBC_I2C_COMM_TEST_TASK_PRIORITY;
      threadConfig.useThreadPool = TaskThreadPoolManager::getInstance().isValid();
      ret = MBCI2CServerTest::comm_I2C_Test.start(commI2CSocketConfig, threadConfig);
#else // I2C_COMM_TEST_SERVER_USE_TASK
      ret = MBCI2CServerTest::comm_I2C_Test.start(commI2CSocketConfig);
#endif // I2C_COMM_TEST_SERVER_USE_TASK
      if (ret != 0)
      {
        break;
      }
    }

    return 0;
  } while (0);

  CONSOLE_LOG("[seT] set %i", -99);
  return -1;
}

void MBCI2CServerTest::loop()
{
  //CONSOLE_LOG("%s","test loop");
  do
  {
    int ret = 0;
#ifdef OPCODE_INPUT_TAS_USE_TASK
    {
      TaskThreadConfigTAG threadConfig = TaskThreadConfigTAG();
      threadConfig.enabled = true;
      threadConfig.usStackDepth = MBC_I2C_COMM_TEST_TASK_MEM;
      threadConfig.uxPriority = MBC_I2C_COMM_TEST_TASK_PRIORITY;
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


bool MBCI2CServerTest::cbOpCodeReceived(int opCode)
{
#ifdef MBC_I2C_SERVER_TEST_DEBUG_CONSOLE_ENABLED
  CONSOLE_LOG("[seT] oprev %i", opCode);
#endif // MBC_I2C_SERVER_TEST_DEBUG_CONSOLE_ENABLED
  
  bool isStopped = false;
  int ret = 0;
  static byte sendMessageId = (byte)CommMBCConstant::CommMBCMessageId::CommMBCStart1;
  static byte revMessageId = (byte)CommMBCConstant::CommMBCMessageId::CommMBCResult1;

  if (MBC_I2C_COMM_TEST_SERVER_CLIENT_SOCKET_1_HEADRLESS == true)
  {
    sendMessageId = (byte)CommMBCConstant::CommMBCMessageId::CommMBCHeadLess;
    revMessageId = sendMessageId;
  }

  static uint8_t clientIndexSend = 0;
  static uint8_t clientIndexRequest = 0;
  // in arudino, there is no enough pin for all buttons
  byte clientAddress = 0;
  if (opCode == OP_SEND)
  {
    //clientIndexSend++;
    clientAddress = (clientIndexSend%2 == 0? MBC_I2C_COMM_TEST_SERVER_CLIENT_SOCKET_1_ADDRESS : MBC_I2C_COMM_TEST_SERVER_CLIENT_SOCKET_2_ADDRESS);
  }

  if (opCode == OP_REQUEST)
  {
    //clientIndexRequest++;
    clientAddress = (clientIndexRequest % 2 == 0 ? MBC_I2C_COMM_TEST_SERVER_CLIENT_SOCKET_1_ADDRESS : MBC_I2C_COMM_TEST_SERVER_CLIENT_SOCKET_2_ADDRESS);
  }

  do
  {
    switch (opCode)
    {
    case OP_SEND:
      ret = MBCI2CServerTest::comm_I2C_Test.send(clientAddress, sendMessageId);
      break;
    case OP_REQUEST:
      ret = MBCI2CServerTest::comm_I2C_Test.request(clientAddress, revMessageId, sizeof(CommHeaderlesstDataTAG));
      break;
    default:
      break;
    }
#ifdef MBC_I2C_SERVER_TEST_DEBUG_CONSOLE_ENABLED
    CONSOLE_LOG("[seT] opt %i", opCode);
#endif // MBC_I2C_SERVER_TEST_DEBUG_CONSOLE_ENABLED
    if (ret != 0)
    {
#ifdef MBC_I2C_SERVER_TEST_DEBUG_CONSOLE_ENABLED
      CONSOLE_LOG("[seT] err %i", ret);
#endif // MBC_I2C_SERVER_TEST_DEBUG_CONSOLE_ENABLED
    }
    return isStopped;
  } while (0);
  return true;
}



#endif // _MBC_I2C_SERVER_TEST_MAIN_H

#endif // _CONF_TEST_TASK_COMM_MBC_I2C_SERVER_ENABLED