
/////////////////////////////////////////////////
// INCLUDE
#include "CommMBCI2CServer.h"
#if (_CONF_COMM_MBC_I2C_SERVER_ENABLED)
/////////////////////////////////////////////////
// PREPROCESSOR
//#define COMM_MBC_I2C_SERVER_CONSOLE_DEBUG_ENABLE
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

/* CommMBCI2CServer*/
////////////////////Function to call from outside ISR///////////////////////
CommMBCI2CServer::CommMBCI2CServer(void)
  : CommI2CServer()
{
  memset(&this->comm_Mbc_I2C_Config, 0, sizeof(CommMBCI2CConfigTAG));
}

CommMBCI2CServer::~CommMBCI2CServer(void)
{
  this->stop();
  this->cleanUp();
}

uint16_t CommMBCI2CServer::registerMessageId(uint8_t messageId, uint16_t maxRawSize)
{
  return this->comm_Mbc_Engine.registerMessageId(messageId, maxRawSize);
}

int CommMBCI2CServer::setConfig(CommMBCI2CConfigTAG& mbcI2CConfig)
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

    
    // comm settings
    {
      CommI2CConfigTAG commConfig = CommI2CConfigTAG();
      commConfig.enabled(true);
      commConfig.isHeaderless(mbcI2CConfig.isHeaderless());
      commConfig.useCheckSum(mbcI2CConfig.useCheckSum());
      commConfig.bufferCount = mbcI2CConfig.bufferCount;
      commConfig.bufferMaxSize = this->comm_Mbc_Engine.maxBufferSize();
      commConfig.bufferUsePool(mbcI2CConfig.bufferUsePool());
      commConfig.codecType = mbcI2CConfig.codecType;
      commConfig.maxClientNumber = mbcI2CConfig.maxClientNumber;

      commConfig.parentPointer = NULL; // not used
      commConfig.cbDataReceived = NULL; // not used
      commConfig.cbDataRequested = NULL; // not used
      ret = CommI2CServer::setConfig(commConfig);
      if (ret != 0)
      {
        break;
      }

      memcpy(&this->comm_Mbc_I2C_Config, &mbcI2CConfig, sizeof(CommMBCI2CConfigTAG));
    }
    return 0;

  } while (0);
#ifdef COMM_MBC_I2C_SERVER_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG("[mbcS] cfig %i", -99);
#endif // COMM_MBC_I2C_SERVER_CONSOLE_DEBUG_ENABLE
  this->stop();
  return -1;
}



int CommMBCI2CServer::start(CommMBCI2CSocketConfigTAG& socketConfig)
{
  int result = 0;
  do
  {
    result = this->comm_Mbc_Engine.start();
    if (result != 0)
    {
      break;
    }

    // socket settting
    {
      CommI2CSocketConfigTAG commI2CSocketConfig = CommI2CSocketConfigTAG();
      commI2CSocketConfig.address = socketConfig.address;
      commI2CSocketConfig.busNumer = socketConfig.busNumer;
      commI2CSocketConfig.pin_SCL = socketConfig.pin_SCL;
      commI2CSocketConfig.pin_SDA = socketConfig.pin_SDA;
      commI2CSocketConfig.frequency = socketConfig.frequency;
      commI2CSocketConfig.timeout = socketConfig.timeout;
      result = CommI2CServer::start(commI2CSocketConfig);
      if (result != 0)
      {
        break;
      }
    }
    return 0;
  } while (0);

  this->stop();
  return -1;
}

void CommMBCI2CServer::stop(void)
{
  this->comm_Mbc_Engine.clear();
  return CommI2CServer::stop();
}

int CommMBCI2CServer::sendById(ClientId_t clientId, uint8_t messageId, unsigned char* data, DataSize_t dataSize)
{
  do
  {
    // find client first
    CommI2CClientInfo* clientInfo = NULL;
    clientInfo = (CommI2CClientInfo*)this->client_Manager.findClient(clientId);
    if (clientInfo == NULL)
    {
      break;
    }
    return this->send(clientInfo, messageId, data, dataSize);
  } while (0);
  return -1;
}

int CommMBCI2CServer::sendByAddress(uint8_t clientAddress, uint8_t messageId, unsigned char* data, DataSize_t dataSize)
{
  do
  {
    // find client first
    CommI2CClientInfo* clientInfo = NULL;
    CommI2CClientPropertiesTAG keyClientProperties = CommI2CClientPropertiesTAG();
    keyClientProperties.address = clientAddress;
    clientInfo = (CommI2CClientInfo*)this->client_Manager.findClient((void*)&keyClientProperties);

    if (clientInfo == NULL)
    {
      break;
    }
    return this->send(clientInfo, messageId, data, dataSize);
  } while (0);
  return -1;
}

int CommMBCI2CServer::send(CommI2CClientInfo* clientInfo, uint8_t messageId, unsigned char* data, DataSize_t dataSize)
{
  int result = 0;
  BufferDataItem* mbcSendData = NULL;
  do
  {
    mbcSendData = this->getCommData();
    if (mbcSendData == NULL)
    {
      break;
    }

    result = this->comm_Mbc_Engine.encodeData(messageId, data, dataSize, mbcSendData);
    if (result != 0)
    {
      break;
    }

    result = CommI2CServer::send(clientInfo, mbcSendData->bufferAddress(), mbcSendData->dataLength());
    if (result != 0)
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
#ifdef COMM_MBC_I2C_SERVER_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG("[mbcS] sen %i", -99);
#endif // COMM_MBC_I2C_SERVER_CONSOLE_DEBUG_ENABLE
  return -1;
}

int CommMBCI2CServer::requestById(ClientId_t clientId, uint8_t messageId, uint8_t requestingDataSize, byte tryTime, bool& isDisconnected, BufferDataItem*& outDataItem)
{
  do
  {
    // find client first
    CommI2CClientInfo* clientInfo = NULL;
    clientInfo = (CommI2CClientInfo*)this->client_Manager.findClient(clientId);
    if (clientInfo == NULL)
    {
      break;
    }
    return this->request(clientInfo, messageId, requestingDataSize, tryTime, isDisconnected, outDataItem);
  } while (0);
  return -1;
}

int CommMBCI2CServer::requestByAddress(uint8_t clientAddress, uint8_t messageId, uint8_t requestingDataSize, byte tryTime, bool& isDisconnected, BufferDataItem*& outDataItem)
{
  do
  {
    // find client first
    CommI2CClientInfo* clientInfo = NULL;
    CommI2CClientPropertiesTAG keyClientProperties = CommI2CClientPropertiesTAG();
    keyClientProperties.address = clientAddress;
    clientInfo = (CommI2CClientInfo*)this->client_Manager.findClient((void*)&keyClientProperties);

    if (clientInfo == NULL)
    {
      break;
    }
    return this->request(clientInfo, messageId, requestingDataSize, tryTime, isDisconnected, outDataItem);
  } while (0);
  return -1;
}


int CommMBCI2CServer::request(CommI2CClientInfo* clientInfo, uint8_t messageId, uint8_t requestingDataSize, byte tryTime, bool& isDisconnected, BufferDataItem*& outDataItem)
{
  int ret = 0;
  BufferDataItem* mbcRequestDataItem = NULL;
  do
  {
    if (this->isConnected() == false)
    {
      break;
    }

    outDataItem = NULL;
    mbcRequestDataItem = this->getCommData();// (BufferDataItem*)mbcRequestParams->dataItem;
    if ((mbcRequestDataItem == NULL)
      || (mbcRequestDataItem->bufferAddress() == NULL)
      )
    {
      break;
    }

    ret = CommI2CServer::request(clientInfo, requestingDataSize, tryTime, isDisconnected, outDataItem);
    if (ret != 0)
    {
      //CONSOLE_LOG("[mbc] req %i", 2);
      break;
    }
    
    BufferDataItem* commRequestDataItem = outDataItem;
    if ( (commRequestDataItem == NULL)
      || (commRequestDataItem->bufferAddress() == NULL)
      )
    {
      break;
    }
    
    bool isHeadLess = (messageId == (uint8_t)CommMBCConstant::CommMBCMessageId::CommMBCHeadLess ? true : false);
    uint8_t revMessageId = (uint8_t)CommMBCConstant::CommMBCMessageId::CommMBCMessageIdInvalid;
    ret = this->comm_Mbc_Engine.decodeData(isHeadLess, commRequestDataItem->bufferAddress(), commRequestDataItem->dataLength(), revMessageId, mbcRequestDataItem);
    this->releaseCommData(commRequestDataItem);
    if ( (ret != 0) 
      || (revMessageId != messageId)
      )
    {
      //CONSOLE_LOG("[mbc] req %i %i %i", 5, revMessageId, mbcRequestParams->messageId);
      break;
    }
    //CONSOLE_LOG("[mbc] req %i", 6);
    // upper side is responsible for releasing mbcRequestDataItem
    outDataItem = mbcRequestDataItem;
    return 0;
  } while (0);

  if (mbcRequestDataItem != NULL)
  {
    this->releaseCommData(mbcRequestDataItem);
  }

  return -1;
}

// hide public interface from parent
int CommMBCI2CServer::setConfig(CommI2CConfigTAG& commI2CConfig)
{
  return CommI2CServer::setConfig(commI2CConfig);
}
//int CommMBCI2CServer::start(CommI2CSocketConfigTAG& socketConfig)
//{
//  return CommI2CServer::start(socketConfig);
//}

int CommMBCI2CServer::sendById(ClientId_t clientId, unsigned char* data, DataSize_t dataSize)
{
  return CommI2CServer::sendById(clientId, data, dataSize);
}

int CommMBCI2CServer::sendByAddress(uint8_t clientAddress, unsigned char* data, DataSize_t dataSize)
{
  return CommI2CServer::sendByAddress(clientAddress, data, dataSize);
}

int CommMBCI2CServer::send(CommI2CClientInfo* clientInfo, unsigned char* data, DataSize_t dataSize)
{
  return CommI2CServer::send(clientInfo, data, dataSize);
}

int CommMBCI2CServer::requestById(ClientId_t clientId, uint8_t requestingDataSize, byte tryTime, bool& isDisconnected, BufferDataItem*& outDataItem)
{
  return CommI2CServer::requestById(clientId, requestingDataSize, tryTime, isDisconnected, outDataItem);
}

int CommMBCI2CServer::requestByAddress(uint8_t clientAddress, uint8_t requestingDataSize, byte tryTime, bool& isDisconnected, BufferDataItem*& outDataItem)
{
  return CommI2CServer::requestByAddress(clientAddress, requestingDataSize, tryTime, isDisconnected, outDataItem);
}

int CommMBCI2CServer::request(CommI2CClientInfo* clientInfo, uint8_t requestingDataSize, byte tryTime, bool& isDisconnected, BufferDataItem*& outDataItem)
{
  return CommI2CServer::request(clientInfo, requestingDataSize, tryTime, isDisconnected, outDataItem);
}

// normal member methods
int CommMBCI2CServer::prepare(void)
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

    ret = CommI2CServer::prepare();
    if (ret != 0)
    {
      break;
    }

    return 0;

  } while (0);
  return -1;
}

void CommMBCI2CServer::cleanUp(void)
{
  do
  {
    CommI2CServer::cleanUp();
    memset(&this->comm_Mbc_I2C_Config, 0, sizeof(CommMBCI2CConfigTAG));
    return;
  } while (0);
  return;
}


#endif // _CONF_COMM_MBC_I2C_SERVER_ENABLED