#include "../MBCI2CTestConstant.h"

#ifdef _CONF_TEST_TASK_COMM_MBC_I2C_CLIENT_ENABLED

#ifndef _MBC_I2C_CLIENT_TEST_MAIN_H
#define _MBC_I2C_CLIENT_TEST_MAIN_H

/////////////////////////////////////////////////
// INCLUDE
#include "MBCI2CClientTask.h"
/////////////////////////////////////////////////
// PREPROCESSOR

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
//
/////////////////////////////////////////////////
// STATIC DATA

/////////////////////////////////////////////////
// STATIC FUNCTIONS

/////////////////////////////////////////////////
// EXTERN

/////////////////////////////////////////////////
// CLASS DEFINITION
class MBCI2CClientTest
{
public:
  MBCI2CClientTest(void);
  ~MBCI2CClientTest(void);
  static int setup(void);
  static void loop(void);
protected:
  //static bool cbOpCodeReceived(int opCode);

protected:
  static MBCI2CClientTask comm_I2C_Test;
};

MBCI2CClientTask MBCI2CClientTest::comm_I2C_Test(MBC_I2C_COMM_TEST_CLIENT_ADDRESS);
MBCI2CClientTest::MBCI2CClientTest(void)
{

}
MBCI2CClientTest::~MBCI2CClientTest(void)
{

}

int MBCI2CClientTest::setup(void)
{
  //CONSOLE_LOG("%s", "test setup");
  int ret = 0;
  //const int timePace = 5;
  do
  {
    // prepare pools
    // #buffer pool
#if (MBC_TEST_BUFFER_POOL_COUNT)
    {
      BufferDataManagerConfigTAG bufferPoolConfig[MBC_TEST_BUFFER_POOL_COUNT];
      memset(&bufferPoolConfig, 0, sizeof(BufferDataManagerConfigTAG) * MBC_TEST_BUFFER_POOL_COUNT);
      bufferPoolConfig[0].preparedDataNumber = MBC_I2C_COMM_TEST_CLIENT_BUFFER_COUNT; // i2cClient
      DataSize_t maxSize = (DataSize_t)CommMBCProcessor::getMaxProceededSize(CommMBCConstant::CommMBCMessageId::CommMBCStart1);
      maxSize = SYSTEM_MAX(maxSize, (DataSize_t)CommMBCProcessor::getMaxProceededSize(CommMBCConstant::CommMBCMessageId::CommMBCResult1));
      maxSize = SYSTEM_MAX(maxSize, (DataSize_t)sizeof(CommHeaderlesstDataTAG));
      bufferPoolConfig[0].dataItemConfig.bufferSize = maxSize;

      memcpy(&bufferPoolConfig[1], &bufferPoolConfig[0], sizeof(BufferDataManagerConfigTAG));
      bufferPoolConfig[1].preparedDataNumber = 2; // codec
      bufferPoolConfig[1].dataItemConfig.bufferSize = TaskCodec::calculateEncodeSize((byte)MBC_I2C_COMM_TEST_CLIENT_CODECTYPE, maxSize);

      ret = BufferDataPoolManager::getInstance().prepare(MBC_TEST_BUFFER_POOL_COUNT, bufferPoolConfig);
      if (ret != 0)
      {
        CONSOLE_LOG("[clT] set %i", -11);
        break;
      }
    }
#endif // (TEST_BUFFER_POOL_COUNT)
    // set up client
    {
      CommMBCI2CConfigTAG commConfig = CommMBCI2CConfigTAG();
      memset(&commConfig, 0, sizeof(CommMBCI2CConfigTAG));
      commConfig.enabled(true);
      commConfig.bufferCount = MBC_I2C_COMM_TEST_CLIENT_BUFFER_COUNT;
      commConfig.bufferMaxSize = 0;// no need
      commConfig.bufferUsePool(BufferDataPoolManager::getInstance().isValid());

      commConfig.isHeaderless(MBC_I2C_COMM_TEST_CLIENT_CONNECTION_HEADERLESS);
      commConfig.useCheckSum(MBC_I2C_COMM_TEST_CLIENT_CHECKSUM);
      commConfig.codecType = MBC_I2C_COMM_TEST_CLIENT_CODECTYPE;
      commConfig.parentPointer = (void*)&MBCI2CClientTest::comm_I2C_Test;
      commConfig.cbDataReceived = (MBCI2CClientTask::cbOnDataReceivedNavigator);
      commConfig.cbDataRequested = (MBCI2CClientTask::cbOnDataRequestedNavigator);
      ret = MBCI2CClientTest::comm_I2C_Test.setConfig(commConfig);
      if (ret != 0)
      {
        CONSOLE_LOG("[clT] set %i", -22);
        break;
      }
    }
    
    CONSOLE_LOG("[clT] set %i", 3); 
    {
      CommI2CSocketConfigTAG commI2CSocketConfig = CommI2CSocketConfigTAG();
      commI2CSocketConfig.busNumer = MBC_I2C_COMM_TEST_CLIENT_SOCKET_BUS_NUMBER;
      commI2CSocketConfig.pin_SCL = MBC_I2C_COMM_TEST_CLIENT_SOCKET_PIN_SCL_0;
      commI2CSocketConfig.pin_SDA = MBC_I2C_COMM_TEST_CLIENT_SOCKET_PIN_SDA_0;
      commI2CSocketConfig.frequency = MBC_I2C_COMM_TEST_CLIENT_SOCKET_FREQUENCY;
      commI2CSocketConfig.address = MBC_I2C_COMM_TEST_CLIENT_ADDRESS;
      commI2CSocketConfig.timeout = MBC_I2C_COMM_TEST_CLIENT_SOCKET_TIMEOUT;
      ret = MBCI2CClientTest::comm_I2C_Test.start(commI2CSocketConfig);
      if (ret != 0)
      {
        CONSOLE_LOG("[clT] set %i", -44);
        break;
      }
    }
    //CONSOLE_LOG("set %i", 99);
    return 0;
  } while (0);

  CONSOLE_LOG("[clT] set %i", -99);
  return -1;
}

void MBCI2CClientTest::loop()
{
  //CONSOLE_LOG("%s","test loop");
  do
  {
    int ret = 0;
    if (ret != 0)
    {
      break;
    }
  } while (0);
}
#endif // _MBC_I2C_CLIENT_TEST_MAIN_H

#endif // _CONF_TEST_TASK_COMM_MBC_I2C_CLIENT_ENABLED