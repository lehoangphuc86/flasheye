#include "I2CClientTask.h"

#ifdef _CONF_TEST_TASK_COMM_I2C_CLIENT_ENABLED

/////////////////////////////////////////////////
// INCLUDE

/////////////////////////////////////////////////
// PREPROCESSOR
#define I2C_CLIENT_TASK_DEBUG_CONSOLE_ENABLED
/////////////////////////////////////////////////
// DEFINE

/////////////////////////////////////////////////
// MARCO

/////////////////////////////////////////////////
// GLOBAL VARIABLES

/////////////////////////////////////////////////
// GLOBAL FUNCTIONS

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
// CLASS IMPLEMENTAION

/* I2CClientTask*/
I2CClientTask::I2CClientTask(uint8_t commAddress)
 : i2c_Client(0)
  , comm_Address(commAddress)
{

}
I2CClientTask::~I2CClientTask(void)
{

}

int I2CClientTask::setConfig(CommI2CConfigTAG& commConfig)
{
  do
  {
    int result = 0;
    result = this->i2c_Client.setConfig(commConfig);
    if (result != 0)
    {
#ifdef I2C_CLIENT_TASK_DEBUG_CONSOLE_ENABLED
      CONSOLE_LOG("[clt] set %i", -11);
#endif // I2C_CLIENT_TASK_DEBUG_CONSOLE_ENABLED
      break;
    }
    return result;
  } while (0);
  //this->stop();
  return -1;
}

int I2CClientTask::prepare(void)
{
  return  0;
}

int I2CClientTask::start(CommI2CSocketConfigTAG& commI2CSocketConfig)
{
  do
  {
    int result = this->i2c_Client.start(commI2CSocketConfig);
    if (result != 0)
    {
      //CONSOLE_LOG("%s", "[i2cCTask] sock failed.");
      break;
    }
#ifdef I2C_CLIENT_TASK_DEBUG_CONSOLE_ENABLED
    CONSOLE_LOG("[clt] sta %i", result);
#endif // I2C_CLIENT_TASK_DEBUG_CONSOLE_ENABLED
    return 0;
  } while (0);
  //this->stop();
  return -1;
}

void I2CClientTask::stop(void)
{
  return this->i2c_Client.stop();
}

static uint8_t count = 0;
void I2CClientTask::cbOnDataRequested(void* param, DataSize_t paramSize)
{
  // simple and headless only
  //CONSOLE_LOG("%s", "[i2cct] 1");
#ifdef I2C_CLIENT_TASK_DEBUG_CONSOLE_ENABLED
  CONSOLE_LOG("[clt] reqed %i", 0);
#endif // I2C_CLIENT_TASK_DEBUG_CONSOLE_ENABLED
  CommI2CCBOnDataRequestedParams* requestParam = (CommI2CCBOnDataRequestedParams*)param;
  BufferDataItem* dataItem = NULL;
  int ret = 0;
  do
  {
    dataItem = requestParam->dataItem;
    if ( (dataItem == NULL)
      || (dataItem->bufferAddress() == NULL)
      || (dataItem->bufferLength() < sizeof(CommI2CTestHeadlessDataTAG))
      )
    {
#ifdef I2C_CLIENT_TASK_DEBUG_CONSOLE_ENABLED
      CONSOLE_LOG("[clt] reqed %i", -11);
#endif // I2C_CLIENT_TASK_DEBUG_CONSOLE_ENABLED
      break;
    }

    unsigned char* buffer = dataItem->bufferAddress();
    memset(buffer, 0, sizeof(CommI2CTestHeadlessDataTAG));
    CommI2CTestHeadlessDataTAG* data = (CommI2CTestHeadlessDataTAG* )(buffer);
    data->messageId = (uint8_t)count++;
    data->nameLen = (uint8_t)5;
    data->commId = this->comm_Address;
    memcpy(&data->name, "abcde", 5);
    ret = dataItem->setDataLen(sizeof(CommI2CTestHeadlessDataTAG));
    //CONSOLE_LOG("%s %i", "[i2cct] 2", ret);
    if (ret != 0)
    {
#ifdef I2C_CLIENT_TASK_DEBUG_CONSOLE_ENABLED
      CONSOLE_LOG("[clt] reqed %i", -22);
#endif // I2C_CLIENT_TASK_DEBUG_CONSOLE_ENABLED
      break;
    }
    //CONSOLE_LOG("%s %i", "[i2cct] 3", dataItem->dataLength());
    if (requestParam->errorCode != NULL)
    {
      *requestParam->errorCode = ret;
    }
    
    return;
  } while (0);

  if (requestParam->errorCode != NULL)
  {
    *requestParam->errorCode = ret;
  }
#ifdef I2C_CLIENT_TASK_DEBUG_CONSOLE_ENABLED
  CONSOLE_LOG("[clt] reqed %i", -99);
#endif // I2C_CLIENT_TASK_DEBUG_CONSOLE_ENABLED
  return;
}

void I2CClientTask::cbOnDataReceived(void* param, DataSize_t paramSize)
{
  CommI2CCBOnDataReceivedParams* revParam = (CommI2CCBOnDataReceivedParams*)param;
  BufferDataItem* revDataItem = NULL;
  //CONSOLE_LOG("[clTa] cbRev %i", 0);
  BufferDataItem* respondDataItem = NULL;
  do
  {
    revDataItem = revParam->dataItem;
    if ( (revDataItem == NULL)
      || (revDataItem->bufferAddress() == NULL)
      )
    {
      break;
    }

    CommI2CTestHeadlessDataTAG* data = (CommI2CTestHeadlessDataTAG*)(revDataItem->bufferAddress());
    CONSOLE_LOG("[clt] rev %i %i %i", data->messageId, data->commId, data->nameLen);
    CONSOLE_LOG("[clt] rev %s", data->name);

    if (this->i2c_Client.isHeaderless() == false)
    {
      respondDataItem = this->i2c_Client.getCommDataFromISR();
      if ((respondDataItem == NULL)
        || (respondDataItem->bufferAddress() == NULL)
        )
      {
        break;
      }
      unsigned char* buffer = respondDataItem->bufferAddress();
      memset(buffer, 0, sizeof(CommI2CTestHeadlessDataTAG));
      CommI2CTestHeadlessDataTAG* resData = (CommI2CTestHeadlessDataTAG*)(buffer);
      resData->messageId = data->messageId;
      resData->commId = data->commId;
      resData->nameLen = data->nameLen;
      memcpy(resData->name, data->name, data->nameLen);
      resData->name[data->nameLen - 1] = '!';
      int ret = respondDataItem->setDataLen(sizeof(CommI2CTestHeadlessDataTAG));
      if (ret != 0)
      {
#ifdef I2C_CLIENT_TASK_DEBUG_CONSOLE_ENABLED
        CONSOLE_LOG("[clt] rev %i", -22);
#endif // I2C_CLIENT_TASK_DEBUG_CONSOLE_ENABLED
        break;
      }

      /*CommI2CDataSendParams sendParams = CommI2CDataSendParams();
      sendParams.clientAddress = this->comm_Address;
      sendParams.inputBuffer = respondDataItem->bufferAddress();
      sendParams.inputBufferSize = respondDataItem->dataLength();*/

      ret = this->i2c_Client.sendFromISR(respondDataItem->bufferAddress(), respondDataItem->dataLength());
      if (ret != 0)
      {
#ifdef I2C_CLIENT_TASK_DEBUG_CONSOLE_ENABLED
        CONSOLE_LOG("[clt] rev %i", -33);
#endif // I2C_CLIENT_TASK_DEBUG_CONSOLE_ENABLED
        break;
      }
      this->i2c_Client.releaseCommDataFromISR(respondDataItem);
    }

    this->i2c_Client.releaseCommDataFromISR(revDataItem);
    return;
  } while (0);

  if (respondDataItem != NULL)
  {
    this->i2c_Client.releaseCommDataFromISR(respondDataItem);
  }

  if (revDataItem != NULL)
  {
    this->i2c_Client.releaseCommDataFromISR(revDataItem);
  }
  return;
}

void I2CClientTask::cbOnDataRequestedNavigator(void* param, DataSize_t paramSize)
{
  do
  {
    //CONSOLE_LOG("reqNavi%s", "1");
    if ((param == NULL)
      || (paramSize != sizeof(CommI2CCBOnDataRequestedParams))
      )
    {
      break;
    }

    CommI2CCBOnDataRequestedParams* requestParam = (CommI2CCBOnDataRequestedParams*)param;

    if (requestParam->parentPointer == NULL)
    {
      break;
    }
    return ((I2CClientTask*)requestParam->parentPointer)->cbOnDataRequested(param, paramSize);
  } while (0);

  return;
}

void I2CClientTask::cbOnDataReceivedNavigator(void* param, DataSize_t paramSize)
{
  do
  {
    if ((param == NULL)
      || (paramSize != sizeof(CommI2CCBOnDataReceivedParams))
      )
    {
      break;
    }

    CommI2CCBOnDataReceivedParams* revParam = (CommI2CCBOnDataReceivedParams*)param;

    if (revParam->parentPointer == NULL)
    {
      break;
    }
    
    return ((I2CClientTask*)revParam->parentPointer)->cbOnDataReceived(param, paramSize);
  } while (0);

  return;
}

#endif // _CONF_TEST_TASK_COMM_I2C_CLIENT_ENABLED