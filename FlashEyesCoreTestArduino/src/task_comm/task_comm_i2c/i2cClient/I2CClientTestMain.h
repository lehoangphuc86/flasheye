#include "../I2CTestConstant.h"

#ifdef _CONF_TEST_TASK_COMM_I2C_CLIENT_ENABLED

#ifndef _I2C_CLIENT_TEST_MAIN_H
#define _I2C_CLIENT_TEST_MAIN_H

/////////////////////////////////////////////////
// INCLUDE
#include "I2CClientTask.h"
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
class I2CClientTest
{
public:
  I2CClientTest(void);
  ~I2CClientTest(void);
  static int setup(void);
  static void loop(void);
protected:
  //static bool cbOpCodeReceived(int opCode);

protected:
  static I2CClientTask comm_I2C_Test;
};

I2CClientTask I2CClientTest::comm_I2C_Test(I2C_COMM_TEST_CLIENT_ADDRESS);
I2CClientTest::I2CClientTest(void)
{

}
I2CClientTest::~I2CClientTest(void)
{

}

int I2CClientTest::setup(void)
{
  //CONSOLE_LOG("%s", "test setup");
  int ret = 0;
  //const int timePace = 5;
  do
  {
    // prepare pools
    // #buffer pool
#if (TEST_BUFFER_POOL_COUNT)
    {
      BufferDataManagerConfigTAG bufferPoolConfig[TEST_BUFFER_POOL_COUNT];
      memset(&bufferPoolConfig, 0, sizeof(BufferDataManagerConfigTAG) * TEST_BUFFER_POOL_COUNT);
      bufferPoolConfig[0].preparedDataNumber = I2C_COMM_TEST_CLIENT_BUFFER_COUNT; // i2cClient
      bufferPoolConfig[0].dataItemConfig.bufferSize = sizeof(CommI2CHeaderTAG) +  sizeof(CommI2CTestHeadlessDataTAG);

      memcpy(&bufferPoolConfig[1], &bufferPoolConfig[0], sizeof(BufferDataManagerConfigTAG));
      bufferPoolConfig[1].preparedDataNumber = 2; // codec
      bufferPoolConfig[1].dataItemConfig.bufferSize = TaskCodec::calculateEncodeSize(TaskCodecConstant::TaskCodecType::TASK_CODEC_HEX, sizeof(CommI2CTestHeadlessDataTAG));//  bufferPoolConfig[0].dataItemConfig.bufferSize + 10;

      ret = BufferDataPoolManager::getInstance().prepare(TEST_BUFFER_POOL_COUNT, bufferPoolConfig);
      if (ret != 0)
      {
        CONSOLE_LOG("[clT] set %i", -11);
        break;
      }
    }
#endif // (TEST_BUFFER_POOL_COUNT)
    // set up client
    {
      CommI2CConfigTAG commConfig = CommI2CConfigTAG();
      memset(&commConfig, 0, sizeof(CommI2CConfigTAG));
      commConfig.enabled(true);
      commConfig.bufferCount = I2C_COMM_TEST_CLIENT_BUFFER_COUNT;
      commConfig.bufferMaxSize = sizeof(CommI2CTestHeadlessDataTAG);
      commConfig.bufferUsePool(BufferDataPoolManager::getInstance().isValid());

      commConfig.isHeaderless(I2C_COMM_TEST_CLIENT_CONNECTION_HEADERLESS);
      commConfig.useCheckSum(I2C_COMM_TEST_CLIENT_CHECKSUM);
      commConfig.codecType = I2C_COMM_TEST_CLIENT_CODECTYPE;
      commConfig.parentPointer = (void*)&I2CClientTest::comm_I2C_Test;
      commConfig.cbDataReceived = (I2CClientTask::cbOnDataReceivedNavigator);
      commConfig.cbDataRequested = (I2CClientTask::cbOnDataRequestedNavigator);
      ret = I2CClientTest::comm_I2C_Test.setConfig(commConfig);
      if (ret != 0)
      {
        CONSOLE_LOG("[clT] set %i", -22);
        break;
      }
    }
    
    CONSOLE_LOG("[clT] set %i", 3); 
    {
      CommI2CSocketConfigTAG commI2CSocketConfig = CommI2CSocketConfigTAG();
      commI2CSocketConfig.busNumer = I2C_COMM_TEST_CLIENT_SOCKET_BUS_NUMBER;
      commI2CSocketConfig.pin_SCL = I2C_COMM_TEST_CLIENT_SOCKET_PIN_SCL_0;
      commI2CSocketConfig.pin_SDA = I2C_COMM_TEST_CLIENT_SOCKET_PIN_SDA_0;
      commI2CSocketConfig.frequency = I2C_COMM_TEST_CLIENT_SOCKET_FREQUENCY;
      commI2CSocketConfig.address = I2C_COMM_TEST_CLIENT_ADDRESS;
      commI2CSocketConfig.timeout = I2C_COMM_TEST_CLIENT_SOCKET_TIMEOUT;
      ret = I2CClientTest::comm_I2C_Test.start(commI2CSocketConfig);
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

void I2CClientTest::loop()
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
#endif // _I2C_CLIENT_TEST_MAIN_H

#endif // _CONF_TEST_TASK_COMM_I2C_CLIENT_ENABLED