
/////////////////////////////////////////////////
// INCLUDE
#include "CommMBCSerial.h"
#if (_CONF_COMM_MBC_SERIAL_ENABLED)
/////////////////////////////////////////////////
// PREPROCESSOR
//#define COMM_MBC_SERIAL_DEBUG_CONSOLE_ENABLED
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

/* CommMBCSerial*/
////////////////////Function to call from outside ISR///////////////////////
CommMBCSerial::CommMBCSerial(void)
  : CommSerial()
{
  memset(&this->comm_Mbc_Serial_Config, 0, sizeof(CommMBCSerialConfigTAG));
}

CommMBCSerial::~CommMBCSerial(void)
{
  this->stop();
  this->cleanUp();
}

uint16_t CommMBCSerial::registerMessageId(uint8_t messageId, uint16_t maxRawSize)
{
  return this->comm_Mbc_Engine.registerMessageId(messageId, maxRawSize);
}

int CommMBCSerial::setConfig(CommMBCSerialConfigTAG& mbcSerialConfig)
{
  int ret = 0;
  do
  {
    if (this->isInitialized() != false)
    {
      return -1;
    }

    {
      CommMBCSettingParamTAG commMbcSetting = CommMBCSettingParamTAG();
      commMbcSetting.enabled = true;
      commMbcSetting.reserved1 = 1;

      ret = this->comm_Mbc_Engine.setConfig(commMbcSetting);
      if (ret != 0)
      {
        break;
      }
    }

    // socket settting
    {
      // comm settings
      CommSerialConfigTAG commConfig = CommSerialConfigTAG();
      commConfig.enabled(mbcSerialConfig.enabled());
      commConfig.isHeaderless(mbcSerialConfig.isHeaderless());
      commConfig.useCheckSum(mbcSerialConfig.useCheckSum());
      commConfig.terminator = mbcSerialConfig.terminator;
      commConfig.codecType = mbcSerialConfig.codecType;
      commConfig.bufferCount = mbcSerialConfig.bufferCount;
      commConfig.bufferMaxSize = this->comm_Mbc_Engine.maxBufferSize();
      commConfig.bufferUsePool(mbcSerialConfig.bufferUsePool());
      commConfig.cbParams = mbcSerialConfig.cbParams;
      commConfig.cbDataReceived = mbcSerialConfig.cbDataReceived;
      commConfig.cbConnectionClosed = mbcSerialConfig.cbConnectionClosed;
      commConfig.taskEventItemNumber = mbcSerialConfig.taskEventItemNumber;
      commConfig.taskEventUsePool(mbcSerialConfig.taskEventUsePool());
      commConfig.taskStackDepth = mbcSerialConfig.taskStackDepth;
      commConfig.taskPriority = mbcSerialConfig.taskPriority;
      commConfig.taskThreadUsePool(mbcSerialConfig.taskThreadUsePool());
      ret = CommSerial::setConfig(commConfig);
      if (ret != 0)
      {
        break;
      }

      memcpy(&this->comm_Mbc_Serial_Config, &mbcSerialConfig, sizeof(CommMBCSerialConfigTAG));
    }
    return 0;

  } while (0);

  CONSOLE_LOG("[mbc] cfig %i", -1);
  this->stop();
  return -1;
}

int CommMBCSerial::start(void)
{
  int result = 0;
  do
  {
    if (this->isRunning() != false)
    {
      return 0;
    }
    
    if (this->comm_Setting.isValidConfig() == false)
    {
      break;
    }

    result = this->comm_Mbc_Engine.start();
    if (result != 0)
    {
      break;
    }

    result = CommSerial::start();
    if (result != 0)
    {
      break;
    }
    return 0;
  } while (0);

  this->stop();
  return -1;
}

int CommMBCSerial::connect(CommMBCSerialSocketConfigTAG& mbcSocketConfig)
{
  int result = 0;
  do
  {
    if (this->isRunning() == false)
    {
      return -1;
    }

    if (this->isConnected() != false)
    {
      return 0;
    }


    CommSerialSocketConfigTAG socketConfig = CommSerialSocketConfigTAG();
    socketConfig.enabled = mbcSocketConfig.enabled;
    socketConfig.bitPerByte = mbcSocketConfig.bitPerByte;
    socketConfig.parityType = mbcSocketConfig.parityType;
    socketConfig.stopBitNum = mbcSocketConfig.stopBitNum;
    socketConfig.baudrate = mbcSocketConfig.baudrate;
    socketConfig.timeout = mbcSocketConfig.timeout;
    socketConfig.instanceIndex = mbcSocketConfig.instanceIndex;
    socketConfig.pin_TX = mbcSocketConfig.pin_TX;
    socketConfig.pin_RX = mbcSocketConfig.pin_RX;
    memcpy(&socketConfig.port, &mbcSocketConfig.port, COMM_SERIAL_PORT_LEN_MAX);


    result = CommSerial::connect(socketConfig);
    if (result != 0)
    {
      break;
    }
    return 0;
  } while (0);

  this->stop();
  return -1;
}

int CommMBCSerial::send(uint8_t messageId, unsigned char* inputBuffer, uint16_t inputBufferSize)
{
  //int result = 0;
  BufferDataItem* mbcSendData = NULL;
  do
  {
    if (this->isConnected() == false)
    {
      break;
    }

    if ((this->comm_Mbc_Serial_Config.isMBCHeaderless() != false)
      && (messageId != (byte)CommMBCConstant::CommMBCMessageId::CommMBCHeadLess)
      )
    {
      break;
    }

    mbcSendData = this->getCommData();
    if (mbcSendData == NULL)
    {
      break;
    }

    int ret = this->comm_Mbc_Engine.encodeData(messageId, inputBuffer, inputBufferSize, mbcSendData);
    if (ret != 0)
    {
      break;
    }
    
    ret = CommSerial::send(mbcSendData->bufferAddress(), mbcSendData->dataLength());
    if (ret != 0)
    {
      break;
    }
    
    if (mbcSendData != NULL)
    {
      this->releaseCommData(mbcSendData);
    }
    return 0;
  } while (0);

  if (mbcSendData != NULL)
  {
    this->releaseCommData(mbcSendData);
  }
  return -1;
}

void CommMBCSerial::stop(void)
{
  this->comm_Mbc_Engine.clear();
  return CommSerial::stop();
}

int CommMBCSerial::read(uint8_t inTryTime, uint8_t& messageId, BufferDataItem*& dataItem, bool& isDisconnected)
{
  int ret = 0;
  BufferDataItem* mbcRequestDataItem = NULL;
  do
  {
    if (this->isConnected() == false)
    {
      break;
    }
    
    dataItem = NULL;
    mbcRequestDataItem = this->getCommData();
    if ((mbcRequestDataItem == NULL)
      || (mbcRequestDataItem->bufferAddress() == NULL)
      )
    {
      break;
    }

    BufferDataItem* commRequestDataItem = NULL;
    ret = CommSerial::read(inTryTime, commRequestDataItem, isDisconnected);
    if (ret != 0)
    {
      break;
    }

    if ((commRequestDataItem == NULL)
      || (commRequestDataItem->bufferAddress() == NULL)
      )
    {
      break;
    }
   
    ret = this->comm_Mbc_Engine.decodeData(this->comm_Mbc_Serial_Config.isMBCHeaderless(), commRequestDataItem->bufferAddress(), commRequestDataItem->dataLength(), messageId, mbcRequestDataItem);
    this->releaseCommData(commRequestDataItem);
    if (ret != 0)
    {
      break;
    }
    // upper side is responsible for releasing mbcRequestDataItem
    dataItem = mbcRequestDataItem;
    return 0;
  } while (0);

  if (mbcRequestDataItem != NULL)
  {
    this->releaseCommData(mbcRequestDataItem);
  }

  return -1;
}


void CommMBCSerial::cleanUp(void)
{
  do
  {
    //this->comm_Mbc_Engine.clear();
    CommSerial::cleanUp();
    memset(&this->comm_Mbc_Serial_Config, 0, sizeof(CommMBCSerialConfigTAG));
    return;
  } while (0);
  return;
}

int CommMBCSerial::prepare(void)
{
  do
  {
    if (this->isInitialized() == false)
    {
      break;
    }

    int ret = this->comm_Mbc_Engine.prepare();
    if (ret != 0)
    {
      break;
    }

    ret = CommSerial::prepare();
    if (ret != 0)
    {
      break;
    }

    return 0;

  } while (0);
  return -1;
}

int CommMBCSerial::setConfig(CommSerialConfigTAG& commConfig) // hide from public
{
  return CommSerial::setConfig(commConfig);
}

//int CommMBCSerial::connect(CommSerialSocketConfigTAG& socketConfig) // hide from public
//{
//  return CommSerial::connect(socketConfig);
//}

int CommMBCSerial::send(unsigned char* buffer, DataSize_t bufferSize) // hide from public
{
  return CommSerial::send(buffer, bufferSize);
}

int CommMBCSerial::read(uint8_t inTryTime, BufferDataItem*& dataItem, bool& isDisconnected)// hide from public
{
  return CommSerial::read(inTryTime, dataItem, isDisconnected);
}

#ifdef COMM_MBC_SERIAL_USE_TASK
int CommMBCSerial::onEventDateReceived(unsigned char* data, unsigned int dataSize)
{
  int ret = 0;
  BufferDataItem* mbcRevDataItem = NULL;
#ifdef COMM_MBC_SERIAL_DEBUG_CONSOLE_ENABLED
  CONSOLE_LOG("[mbc] onRev %i", 0);
#endif // COMM_MBC_SERIAL_DEBUG_CONSOLE_ENABLED
  do
  {
    if ((dataSize != sizeof(EventCommSerialDataReceivedTAG))
      || (data == NULL))
    {
      break;
    }

    EventCommSerialDataReceivedTAG* eventData = (EventCommSerialDataReceivedTAG*)data;
    BufferDataItem* revDataItem = eventData->dataItem;
    if (revDataItem == NULL)
    {
      break;
    }

    if (this->isConnected() == false)
    {
      this->releaseCommData(revDataItem);
      break;
    }

    mbcRevDataItem = this->getCommData();
    if ((mbcRevDataItem == NULL)
      || (mbcRevDataItem->bufferAddress() == NULL)
      )
    {
      break;
    }

    uint8_t revMessageId = (uint8_t)CommMBCConstant::CommMBCMessageId::CommMBCMessageIdInvalid;
    ret = this->comm_Mbc_Engine.decodeData(this->comm_Mbc_Serial_Config.isMBCHeaderless(), revDataItem->bufferAddress(), revDataItem->dataLength(), revMessageId, mbcRevDataItem);
    this->releaseCommData(revDataItem);
    if (ret != 0)
    {
      break;
    }
    // upper side is responsible for releasing mbcRevDataItem
    CommSerialCbOnDataReceived cbOnDataRev = this->comm_Setting.cbDataReceived();
    if (cbOnDataRev == NULL)
    {
      break;
    }
    //CONSOLE_LOG("[darev] %i", 1);
    EventCommMBCSerialDataReceivedTAG cbOnDataRevParams = EventCommMBCSerialDataReceivedTAG();
    cbOnDataRevParams.messageId = revMessageId;
    cbOnDataRevParams.cbParams = this->comm_Setting.cbParams();
    cbOnDataRevParams.dataItem = mbcRevDataItem;
    cbOnDataRev(&cbOnDataRevParams, sizeof(EventCommMBCSerialDataReceivedTAG));
    return 0;
  } while (0);

  if (mbcRevDataItem != NULL)
  {
    this->releaseCommData(mbcRevDataItem);
  }
#ifdef COMM_MBC_SERIAL_DEBUG_CONSOLE_ENABLED
  CONSOLE_LOG("[mbc] onRev %i", -99);
#endif // COMM_MBC_SERIAL_DEBUG_CONSOLE_ENABLED
  return -1;
}
#endif // COMM_MBC_SERIAL_USE_TASK
#endif // _CONF_COMM_MBC_SERIAL_ENABLED