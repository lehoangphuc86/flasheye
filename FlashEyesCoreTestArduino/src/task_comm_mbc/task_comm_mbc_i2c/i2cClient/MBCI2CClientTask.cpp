#include "MBCI2CClientTask.h"

#ifdef _CONF_TEST_TASK_COMM_MBC_I2C_CLIENT_ENABLED

/////////////////////////////////////////////////
// INCLUDE

/////////////////////////////////////////////////
// PREPROCESSOR
#define MBC_I2C_CLIENT_TASK_DEBUG_CONSOLE_ENABLED
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

/* MBCI2CClientTask*/
MBCI2CClientTask::MBCI2CClientTask(uint8_t commAddress)
 : mbc_i2c_Client(0)
  , comm_Address(commAddress)
{

}
MBCI2CClientTask::~MBCI2CClientTask(void)
{

}

int MBCI2CClientTask::setConfig(CommMBCI2CConfigTAG& commConfig)
{
  do
  {
    int result = 0;
    if (this->mbc_i2c_Client.isInitialized() != false)
    {
      return -1; // already config
    }

    this->mbc_i2c_Client.registerMessageId(CommMBCConstant::CommMBCMessageId::CommMBCHeadLess, sizeof(CommHeaderlesstDataTAG));
    this->mbc_i2c_Client.registerMessageId(CommMBCConstant::CommMBCMessageId::CommMBCStart1);
    this->mbc_i2c_Client.registerMessageId(CommMBCConstant::CommMBCMessageId::CommMBCResult1);

    result = this->mbc_i2c_Client.setConfig(commConfig);
    if (result != 0)
    {
#ifdef MBC_I2C_CLIENT_TASK_DEBUG_CONSOLE_ENABLED
      CONSOLE_LOG("[clt] set %i", -11);
#endif // MBC_I2C_CLIENT_TASK_DEBUG_CONSOLE_ENABLED
      break;
    }
    return result;
  } while (0);
  //this->stop();
  return -1;
}

int MBCI2CClientTask::prepare(void)
{
  return  0;
}

int MBCI2CClientTask::start(CommMBCI2CSocketConfigTAG& socketConfig)
{
  do
  {
    int result = this->mbc_i2c_Client.start(socketConfig);
    if (result != 0)
    {
      //CONSOLE_LOG("%s", "[i2cCTask] sock failed.");
      break;
    }
#ifdef MBC_I2C_CLIENT_TASK_DEBUG_CONSOLE_ENABLED
    CONSOLE_LOG("[clt] sta %i", result);
#endif // MBC_I2C_CLIENT_TASK_DEBUG_CONSOLE_ENABLED
    return 0;
  } while (0);
  //this->stop();
  return -1;
}

void MBCI2CClientTask::stop(void)
{
  return this->mbc_i2c_Client.stop();
}

static uint8_t count = 0;
void MBCI2CClientTask::cbOnDataRequested(void* param, DataSize_t paramSize)
{
#ifdef MBC_I2C_CLIENT_TASK_DEBUG_CONSOLE_ENABLED
  CONSOLE_LOG("[clt] reqed %i", 0);
#endif // MBC_I2C_CLIENT_TASK_DEBUG_CONSOLE_ENABLED
  CommMBCI2CCBOnDataRequestedParams* requestParam = (CommMBCI2CCBOnDataRequestedParams*)param;
  BufferDataItem* dataItem = NULL;
  int ret = 0;
  do
  {
    dataItem = requestParam->dataItem;
    if ( (dataItem == NULL)
      || (dataItem->bufferAddress() == NULL)
      || (dataItem->bufferLength() < sizeof(CommHeaderlesstDataTAG))
      )
    {
#ifdef MBC_I2C_CLIENT_TASK_DEBUG_CONSOLE_ENABLED
      CONSOLE_LOG("[clt] reqed %i", -11);
#endif // MBC_I2C_CLIENT_TASK_DEBUG_CONSOLE_ENABLED
      break;
    }

    unsigned char* buffer = dataItem->bufferAddress();
    memset(buffer, 0, sizeof(CommHeaderlesstDataTAG));
    CommHeaderlesstDataTAG* data = (CommHeaderlesstDataTAG* )(buffer);
    data->count = (uint8_t)count++;
    data->nameLen = (uint8_t)5;
    data->sourceId = this->comm_Address;
    memcpy(&data->name, "abcde", 5);
    ret = dataItem->setDataLen(sizeof(CommHeaderlesstDataTAG));
    //CONSOLE_LOG("%s %i", "[i2cct] 2", ret);
    if (ret != 0)
    {
#ifdef MBC_I2C_CLIENT_TASK_DEBUG_CONSOLE_ENABLED
      CONSOLE_LOG("[clt] reqed %i", -22);
#endif // MBC_I2C_CLIENT_TASK_DEBUG_CONSOLE_ENABLED
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
#ifdef MBC_I2C_CLIENT_TASK_DEBUG_CONSOLE_ENABLED
  CONSOLE_LOG("[clt] reqed %i", -99);
#endif // MBC_I2C_CLIENT_TASK_DEBUG_CONSOLE_ENABLED
  return;
}

void MBCI2CClientTask::cbOnDataReceived(void* param, DataSize_t paramSize)
{
  CommMBCI2CCBOnDataReceivedParams* revParam = (CommMBCI2CCBOnDataReceivedParams*)param;
  BufferDataItem* revDataItem = NULL;
#ifdef MBC_I2C_CLIENT_TASK_DEBUG_CONSOLE_ENABLED
  //CONSOLE_LOG("[clt] rev %i %i", 0, revParam->messageId);
#endif // MBC_I2C_CLIENT_TASK_DEBUG_CONSOLE_ENABLED
  do
  {
    revDataItem = revParam->dataItem;
    if ( (revDataItem == NULL)
      || (revDataItem->bufferAddress() == NULL)
      )
    {
      break;
    }
    unsigned char* revBuffer = revDataItem->bufferAddress();
    switch (revParam->messageId)
    {
      case (int)CommMBCConstant::CommMBCMessageId::CommMBCStart1:
      {
        CommMBCStart1TAG* start1Data = (CommMBCStart1TAG*)revBuffer;
        //CONSOLE_LOG("[mCliT] revS1 %i %i", start1Data->param1, start1Data->param2);
        CONSOLE_LOG("[mCliT] senS1 %i %s", start1Data->nameLen, start1Data->name);
        
        CommMBCResult1TAG result1 = CommMBCResult1TAG();
        result1.status = start1Data->param2;
        result1.nameLen = start1Data->nameLen;
        result1.name = start1Data->name;
        this->mbc_i2c_Client.sendFromISR((uint8_t)CommMBCConstant::CommMBCMessageId::CommMBCResult1, (unsigned char*)&result1, sizeof(CommMBCResult1TAG));
        break;
      }
      case (int)CommMBCConstant::CommMBCMessageId::CommMBCResult1:
      {
        CommMBCResult1TAG* result1Data = (CommMBCResult1TAG*)revBuffer;
        CONSOLE_LOG("[mSeT] revR1 %i %i", result1Data->status, result1Data->nameLen);
        CONSOLE_LOG("[mSeT] senR1 %s", result1Data->name);

        break;
      }
      case (int)CommMBCConstant::CommMBCMessageId::CommMBCHeadLess:
      {
        CommHeaderlesstDataTAG* testData = (CommHeaderlesstDataTAG*)revBuffer;
        CONSOLE_LOG("[mSeT] rev %i %i", testData->count, testData->sourceId);
        CONSOLE_LOG("[mSeT] rev %s", testData->name);
        break;
      }

      default:
        break;
    }
    
    this->mbc_i2c_Client.releaseCommDataFromISR(revDataItem);
    return;
  } while (0);

  if (revDataItem != NULL)
  {
    this->mbc_i2c_Client.releaseCommDataFromISR(revDataItem);
  }
#ifdef MBC_I2C_CLIENT_TASK_DEBUG_CONSOLE_ENABLED
  CONSOLE_LOG("[clt] rev %i", -99);
#endif // MBC_I2C_CLIENT_TASK_DEBUG_CONSOLE_ENABLED
  return;
}

void MBCI2CClientTask::cbOnDataRequestedNavigator(void* param, DataSize_t paramSize)
{
  do
  {
    //CONSOLE_LOG("reqNavi%s", "1");
    if ((param == NULL)
      || (paramSize != sizeof(CommMBCI2CCBOnDataRequestedParams))
      )
    {
      break;
    }

    CommMBCI2CCBOnDataRequestedParams* requestParam = (CommMBCI2CCBOnDataRequestedParams*)param;

    if (requestParam->parentPointer == NULL)
    {
      break;
    }
    return ((MBCI2CClientTask*)requestParam->parentPointer)->cbOnDataRequested(param, paramSize);
  } while (0);

  return;
}

void MBCI2CClientTask::cbOnDataReceivedNavigator(void* param, DataSize_t paramSize)
{
  do
  {
    if ((param == NULL)
      || (paramSize != sizeof(CommMBCI2CCBOnDataReceivedParams))
      )
    {
      break;
    }

    CommMBCI2CCBOnDataReceivedParams* revParam = (CommMBCI2CCBOnDataReceivedParams*)param;

    if (revParam->parentPointer == NULL)
    {
      break;
    }
    
    return ((MBCI2CClientTask*)revParam->parentPointer)->cbOnDataReceived(param, paramSize);
  } while (0);

  return;
}

#endif // _CONF_TEST_TASK_COMM_MBC_I2C_CLIENT_ENABLED