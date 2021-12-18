
/////////////////////////////////////////////////
// INCLUDE
#include "CommI2CSocket.h"
#if (_CONF_COMM_I2C_SOCKET_ENABLED)

/////////////////////////////////////////////////
// PREPROCESSOR
#define COMM_I2C_SOCKET_REV_CONSOLE_DEBUG_ENABLE
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

/* CommI2CSocket*/
////////////////////Function to call from outside ISR///////////////////////
CommI2CSocket::CommI2CSocket(uint8_t commId)
  : comm_Id(commId)
  , connection_State((byte)CommConnectionState::CommDisconnected)
  , comm_Sequence_Id(COMM_I2C_SEQUENCE_ID_INVALID)
  , task_Codec(NULL)
{

}

CommI2CSocket::~CommI2CSocket(void)
{
  this->stop();
  this->cleanUp();
}

uint8_t CommI2CSocket::commId(void)
{
  return this->comm_Id;
}



int CommI2CSocket::setConfig(CommI2CConfigTAG& commI2CConfig)
{
  do
  {
    int ret = 0;
    if ( (this->isInitialized() != false)
      || (this->isDisconnected() == false)
      )
    {
      return -1; // already config
    }

    ret = this->comm_Connection_Setting.setConfig(commI2CConfig);
    if (ret != 0)
    {
      break;
    }

    if (this->comm_Connection_Setting.useCodec() != false)
    {
      if (this->task_Codec == NULL)
      {
        TaskCodecConfigTAG taskCodecConfig = TaskCodecConfigTAG();
        taskCodecConfig.codecType = this->comm_Connection_Setting.codecType();
        taskCodecConfig.maxDataSize = this->comm_Connection_Setting.bufferMaxSize();
        this->task_Codec = TaskCodecFactory::create(this->comm_Connection_Setting.codecType(), taskCodecConfig);
      }

      if (this->task_Codec == NULL)
      {
        break;
      }
    }

    BufferDataManagerConfigTAG bufferManagerConfig = BufferDataManagerConfigTAG();
    bufferManagerConfig.preparedDataNumber = this->comm_Connection_Setting.bufferCount();
    bufferManagerConfig.dataItemConfig.bufferSize = this->comm_Connection_Setting.bufferMaxSize();
    bufferManagerConfig.usePool = this->comm_Connection_Setting.bufferUsePool();
    ret = this->data_Manager.prepare(bufferManagerConfig);
    //CONSOLE_LOG("[sock] pre %i %i %i", initStruct.bufferSize, this->comm_Connection_Setting.maxBufferCount(), result);
    if (ret != 0)
    {
      break;
    }
    
    return 0;

  } while (0);

  this->stop();
  return -1;
}

int CommI2CSocket::prepare(void)
{
  int result = 0;
  do
  {
    if (this->isInitialized() == false)
    {
      break;
    }

    if (this->comm_Connection_Setting.isValidConfig() == false)
    {
      break;
    }

    return 0;
  } while (0);
  return -1;
}

int CommI2CSocket::start(CommI2CSocketConfigTAG& socketConfig)
{
  int result = 0;
  do
  {

    if (this->isDisconnected() == false)
    {
      return -1;
    }

    if (this->isInitialized() == false)
    {
      break;
    }

    this->stop();
    result = this->i2c_Socket.start(socketConfig);
    if (result != 0)
    {
      break;
    }
    this->connectionState(CommConnectionState::CommConnected);
    return 0;
  } while (0);
  this->stop();
  return -1;
}

//int CommI2CSocket::disconnect(void)
//{
//  this->closeConnection();
//}

bool CommI2CSocket::isInitialized(void)
{
  return this->data_Manager.isValid();
}

bool CommI2CSocket::isConnected(void)
{
  return (this->connectionState() == CommConnectionState::CommConnected ? true : false);
}

bool CommI2CSocket::isDisconnected(void)
{
  return (this->connectionState() == CommConnectionState::CommDisconnected ? true : false);
}

bool CommI2CSocket::isValidSocket(void)
{
  return this->i2c_Socket.isValid();
}

byte CommI2CSocket::connectionState(void)
{
  SystemCriticalLocker criticalLocker(this->critical_Key);
  return this->connection_State;
}

//CommI2CConnectionSetting* CommI2CSocket::connectionSetting(void)
//{
//  return &this->comm_Connection_Setting;
//}

bool CommI2CSocket::isHeaderless(void)
{
  return this->comm_Connection_Setting.isHeaderLess();
}

BufferDataItem* CommI2CSocket::getCommData(void)
{
  return this->data_Manager.get();
}

void CommI2CSocket::releaseCommData(BufferDataItem* dataItem)
{
  this->data_Manager.release(dataItem);
}

void CommI2CSocket::connectionState(byte state)
{
  SystemCriticalLocker criticalLocker(this->critical_Key);
  this->connection_State = state;
}

void CommI2CSocket::closeConnection(void)
{
  do
  {
    if (this->isConnected() != false)
    {
      this->connectionState(CommConnectionState::CommDisconnecting);
    }

    this->i2c_Socket.stop();
    this->connectionState(CommConnectionState::CommDisconnected);
    return;
  } while (0);

  return;
}

void CommI2CSocket::stop(void)
{
  do
  {
    this->closeConnection();
    this->data_Manager.releaseAll();
    this->comm_Sequence_Id = COMM_I2C_SEQUENCE_ID_INVALID;
    //this->resetReceivingInfo();
    //this->resetSendingInfo();
    this->i2c_Socket.flush();
    this->i2c_Socket.flushReceivedBuff();
    return;
  } while (0);

  return;
}

void CommI2CSocket::cleanUp(void)
{
  this->data_Manager.releaseAll();
  if (this->task_Codec != NULL)
  {
    TaskCodecFactory::release(this->task_Codec);
  }
  this->task_Codec = NULL;
}

////////////////////Function to call from inside ISR///////////////////////
bool CommI2CSocket::isConnectedFromISR(void)
{
  return (this->connectionStateFromISR() == CommConnectionState::CommConnected ? true : false);
}

bool CommI2CSocket::isDisconnectedFromISR(void)
{
  return (this->connectionStateFromISR() == CommConnectionState::CommDisconnected ? true : false);
}

bool CommI2CSocket::isValidSocketFromISR(void)
{
  return this->i2c_Socket.isValid();
}

byte CommI2CSocket::connectionStateFromISR(void)
{
  SystemCriticalLockerFromISR criticalLocker(this->critical_Key);
  return this->connection_State;
}

CommI2CConnectionSetting* CommI2CSocket::connectionSettingFromISR(void)
{
  return &this->comm_Connection_Setting;
}

BufferDataItem* CommI2CSocket::getCommDataFromISR(void)
{
  return this->data_Manager.getFromISR();
}

void CommI2CSocket::releaseCommDataFromISR(BufferDataItem* dataItem)
{
  this->data_Manager.releaseFromISR(dataItem);
}

void CommI2CSocket::connectionStateFromISR(byte state)
{
  SystemCriticalLockerFromISR criticalLocker(this->critical_Key);
  this->connection_State = state;
}

DataSize_t CommI2CSocket::receiveData(byte clientAddress, unsigned char* buffer, DataSize_t bufferSize, DataSize_t maxLenToRead, byte codecType, byte tryTime, bool& isDisconnected)
{
  byte remainTryTime = tryTime;
  isDisconnected = false;
  BufferDataItem* codecDataItem = NULL;
  int ret = 0;
  do
  {
    if (maxLenToRead > bufferSize)
    {
      break;
    }

    DataSize_t encodedReadLen = 0;
    unsigned char* encodedData = buffer;
    DataSize_t encodedDataSize = maxLenToRead;

    if ( (TaskCodecConstant::isValidCodecType(codecType) != false)
      && (this->task_Codec != NULL)
      )
    {
      codecDataItem = this->task_Codec->getCodecData();
      if (codecDataItem == NULL)
      {
        break;
      }
      encodedData = codecDataItem->bufferAddress();
      encodedDataSize = this->task_Codec->calculateEncodeSize(maxLenToRead);
      if (encodedDataSize > codecDataItem->bufferLength())
      {
        break;
      }
    }

    if (encodedData == NULL)
    {
      break;
    }

    unsigned char* readBuffer = encodedData;
    bool isCompleted = false;
    DataSize_t readTruckSize = 0;
    DataSize_t truckSizeToRead = 0;
    while ((isCompleted == false)
      && (this->isConnected() != false)
      )
    {
      if (encodedReadLen >= encodedDataSize)
      {
        break;
      }
      
      truckSizeToRead = (DataSize_t)SYSTEM_MIN(encodedDataSize - encodedReadLen, COMM_I2C_SYS_BUFFER_LEN);
      ret = this->i2c_Socket.readData(clientAddress, (uint8_t*)readBuffer, truckSizeToRead, readTruckSize);
      if (ret != 0)
      {
        isDisconnected = true;
#ifdef COMM_I2C_SOCKET_REV_CONSOLE_DEBUG_ENABLE
//        CONSOLE_LOG("[soc] red %i %i %i", -22, truckSizeToRead, readTruckSize);
#endif // COMM_I2C_SOCKET_REV_CONSOLE_DEBUG_ENABLE
        break;
      }
      
      if (readTruckSize == 0)
      {
        if (remainTryTime <= 0)
        {
#ifdef COMM_I2C_SOCKET_REV_CONSOLE_DEBUG_ENABLE
          if (encodedReadLen > 0)
          {
            CONSOLE_LOG("##[rd] bk %i %i", truckSizeToRead, (unsigned int)encodedReadLen);
          }
#endif // COMM_I2C_SOCKET_REV_CONSOLE_DEBUG_ENABLE
          break;
        }
        remainTryTime--;
        continue;
      }
      else
      {
        remainTryTime = tryTime;
      }

      //SYSTEM_CONSOLE("rd %i %i %i %s", remainTryTime, truckSizeToRead, readTruckSize, buffer);
      encodedReadLen = (DataSize_t)(encodedReadLen + readTruckSize);
      readBuffer += readTruckSize;
      if (encodedReadLen < encodedDataSize) // not enough
      {
        continue;
      }
      
      isCompleted = true;
      break;
    }

    if ((isDisconnected != false)
      || (isCompleted == false))
    {
      break;
    }

    if ( (this->task_Codec == NULL)
      || (TaskCodecConstant::isValidCodecType(codecType) == false)
      )
    {
      return encodedReadLen;
    }
//#ifdef COMM_I2C_SOCKET_REV_CONSOLE_DEBUG_ENABLE
//    CONSOLE_LOG("[soc] red %i %i %i", 11, truckSizeToRead, readTruckSize);
//#endif // COMM_I2C_SOCKET_REV_CONSOLE_DEBUG_ENABLE
    DataSize_t decodedReadLen = encodedReadLen;
    codecDataItem->setDataLen(encodedReadLen);
    ret = this->task_Codec->decode(codecDataItem, buffer, bufferSize, decodedReadLen);
    if (ret != 0)
    {
      break;
    }

    if (codecDataItem != NULL)
    {
      this->task_Codec->releaseCodecData(codecDataItem);
    }
    return decodedReadLen;

  } while (0);

  if (codecDataItem != NULL)
  {
    this->task_Codec->releaseCodecData(codecDataItem);
  }

  return 0;
}

int CommI2CSocket::sendData(byte clientAddress, unsigned char* data, DataSize_t dataSize, byte codecType)
{
  int result = 0;
  BufferDataItem* codecDataItem = NULL;
  do
  {
    if (data == NULL)
    {
      break;
    }

    if (this->isConnected() == false)
    {
      break;
    }

    unsigned char* encodedData = data;
    DataSize_t encodedDataSize = dataSize;

    if ((TaskCodecConstant::isValidCodecType(codecType) != false)
      && (this->task_Codec != NULL)
      )
    {
      result = this->task_Codec->encode(data, dataSize, codecDataItem);
      if (result != 0)
      {
        break;
      }

      encodedData = codecDataItem->bufferAddress();
      encodedDataSize = codecDataItem->dataLength();
    }

    DataSize_t truckSize = 0;
    DataSize_t totalSentSize = 0;
    DataSize_t sentTruckSize = 0;
    while (totalSentSize < encodedDataSize)
    {
      truckSize = (DataSize_t)SYSTEM_MIN(encodedDataSize - totalSentSize, COMM_I2C_SYS_BUFFER_LEN);
      result = this->i2c_Socket.writeData(clientAddress, (uint8_t*)(encodedData + totalSentSize), truckSize, sentTruckSize);
      if ((result != 0)
        || (sentTruckSize <= 0)
        )
      {
        break;
      }
      totalSentSize = (DataSize_t)(totalSentSize + sentTruckSize);
    }
    if (totalSentSize != encodedDataSize)
    {
      break;
    }

    if (codecDataItem != NULL)
    {
      this->task_Codec->releaseCodecData(codecDataItem);
    }
    return 0;
  } while (0);

  if (codecDataItem != NULL)
  {
    this->task_Codec->releaseCodecData(codecDataItem);
  }
  return -1;
}

#endif // _CONF_COMM_I2C_SOCKET_ENABLED