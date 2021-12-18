/////////////////////////////////////////////////
// INCLUDE
#include "CommI2CServer.h"
#if (_CONF_COMM_I2C_SERVER_ENABLED)

#include "../../utility/Utility.h"
/////////////////////////////////////////////////
// PREPROCESSOR
//#define COMM_I2C_SERVER_DEBUG_CONSOLE_ENABLED
//#define COMM_I2C_CLIENT_INFOR_DEBUG_CONSOLE_ENABLED
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
/*CommI2CClientInfo*/
CommI2CClientInfo::CommI2CClientInfo()
  : CommClientInfo(CommConstant::CommType::CommTypeI2C)
{
  
}

CommI2CClientInfo::~CommI2CClientInfo(void)
{

}

bool CommI2CClientInfo::compareByClientProperties(void* clientProperties)
{
  do
  {
    if (this->client_Properties == clientProperties)
    {
      return true;
    }
    
    if ((this->client_Properties == NULL)
      || (clientProperties == NULL)
      )
    {
      break;
    }

    CommI2CClientPropertiesTAG* otherProperties = (CommI2CClientPropertiesTAG*)clientProperties;
    CommI2CClientPropertiesTAG* selfProperties = (CommI2CClientPropertiesTAG*)this->client_Properties;
    if (selfProperties->address != otherProperties->address)
    {
      break;
    }
    return true;
  } while (0);

  return false;
}

int CommI2CClientInfo::setClientProperties(void* srcClientProperties)
{
  do
  {
    if ( (this->client_Properties == NULL) 
      && (this->client_Properties == srcClientProperties)
      )
    {
      return 0;
    }

    if ((this->client_Properties == NULL)
      || (srcClientProperties == NULL)
      )
    {
      break;
    }

    CommI2CClientPropertiesTAG* srcProperties = (CommI2CClientPropertiesTAG*)srcClientProperties;
    CommI2CClientPropertiesTAG* selfProperties = (CommI2CClientPropertiesTAG*)this->client_Properties;
    selfProperties->address = srcProperties->address;
    selfProperties->headerless = srcProperties->headerless;
    selfProperties->checkSum = srcProperties->checkSum;
    selfProperties->codecType = srcProperties->codecType;
    return 0;
  } while (0);
  return -1;
}

void CommI2CClientInfo::initClientProperties(void)
{
  this->clearClientProperties();
  this->client_Properties = new CommI2CClientPropertiesTAG();
}

/* CommI2CServer*/
////////////////////Function to call from outside ISR///////////////////////
CommI2CServer::CommI2CServer(byte commId)
  : CommI2CSocket(commId)
{

}

CommI2CServer::~CommI2CServer(void)
{
  this->stop();
  this->cleanUp();
}

int CommI2CServer::setConfig(CommI2CConfigTAG& commI2CConfig)
{
  do
  {
    int ret = this->client_Manager.setConfig(commI2CConfig.maxClientNumber);
    if (ret != 0)
    {
      break;
    }

    ret = this->client_Manager.prepare<CommI2CClientInfo>();
    if (ret != 0)
    {
      break;
    }

    return CommI2CSocket::setConfig(commI2CConfig);
  } while (0);

  return -1;
}

int CommI2CServer::prepare(void)
{
  return CommI2CSocket::prepare();
}

int CommI2CServer::start(CommI2CSocketConfigTAG& socketConfig)
{
  int ret = 0;
#ifdef COMM_I2C_SERVER_DEBUG_CONSOLE_ENABLED
  CONSOLE_LOG("[sev] sta %i", 0);
#endif // COMM_I2C_SERVER_DEBUG_CONSOLE_ENABLED
  do
  {
    if ( (this->isDisconnected() == false)
      || (this->client_Manager.isValid() == false)
      )
    {
      break;
    }

    ret = this->prepare();
    if (ret != 0)
    {
      break;
    }

    ret = CommI2CSocket::start(socketConfig);
    if (ret != 0)
    {
      break;
    }
    return 0;
  } while (0);
#ifdef COMM_I2C_SERVER_DEBUG_CONSOLE_ENABLED
  CONSOLE_LOG("[sev] sta %i", -99);
#endif // COMM_I2C_SERVER_DEBUG_CONSOLE_ENABLED
  this->stop();
  return -1;
}

void CommI2CServer::stop(void)
{
#ifdef COMM_I2C_SERVER_DEBUG_CONSOLE_ENABLED
  CONSOLE_LOG("[sev] stop %i", 0);
#endif // COMM_I2C_SERVER_DEBUG_CONSOLE_ENABLED
  CommI2CSocket::stop();
}

void CommI2CServer::cleanUp(void)
{
  this->client_Manager.clear();
}

int CommI2CServer::appendClient(void* clientProperties)
{
  return this->client_Manager.appendClient(clientProperties);
}

CommClientInfo* CommI2CServer::findClient(ClientId_t clientId, void* clientProperties)
{
  return this->client_Manager.findClient(clientId, clientProperties);
}


int CommI2CServer::sendById(ClientId_t clientId, unsigned char* data, DataSize_t dataSize)
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
    return this->send(clientInfo, data, dataSize);
  } while (0);
  return -1;
}

int CommI2CServer::sendByAddress(uint8_t clientAddress, unsigned char* data, DataSize_t dataSize)
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
    return this->send(clientInfo, data, dataSize);
  } while (0);
  return -1;
}

int CommI2CServer::send(CommI2CClientInfo* clientInfo, unsigned char* data, DataSize_t dataSize)
{
  int result = 0;
  BufferDataItem* sendingDataItem = NULL;
#ifdef COMM_I2C_SERVER_DEBUG_CONSOLE_ENABLED
  CONSOLE_LOG("[sev] sen %i", 0);
#endif // COMM_I2C_SERVER_DEBUG_CONSOLE_ENABLED
  do
  {
    if ( (this->isConnected() == false)
      || (clientInfo == NULL)
      )
    {
      break;
    }

    if ((data == NULL)
      || (dataSize<= 0)
      )
    {
      break;
    }
    sendingDataItem = this->getCommData();
    if ((sendingDataItem == NULL)
      || (sendingDataItem->bufferAddress() == NULL)
      || (sendingDataItem->bufferLength() < dataSize)
      )
    {
      break;
    }

    result = sendingDataItem->setData(dataSize, data);
    if (result != 0)
    {
      break;
    }

    result = this->sendPackage(clientInfo, sendingDataItem);
    this->releaseCommData(sendingDataItem);
    if (result != 0)
    {
      break;
    }
    return 0;
  } while (0);

  if (sendingDataItem != NULL)
  {
    this->releaseCommData(sendingDataItem);
  }
  
#ifdef COMM_I2C_SERVER_DEBUG_CONSOLE_ENABLED
  CONSOLE_LOG("[sev] sen %i", -99);
#endif // COMM_I2C_SERVER_DEBUG_CONSOLE_ENABLED

  return -1;
}


int CommI2CServer::requestById(ClientId_t clientId, uint8_t requestingDataSize, byte tryTime, bool& isDisconnected, BufferDataItem*& outDataItem)
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
    return this->request(clientInfo, requestingDataSize, tryTime, isDisconnected, outDataItem);
  } while (0);
  return -1;
}

int CommI2CServer::requestByAddress(uint8_t clientAddress, uint8_t requestingDataSize, byte tryTime, bool& isDisconnected, BufferDataItem*& outDataItem)
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
    return this->request(clientInfo, requestingDataSize, tryTime, isDisconnected, outDataItem);
  } while (0);
  return -1;
}

int CommI2CServer::request(CommI2CClientInfo* clientInfo, uint8_t requestingDataSize, byte tryTime, bool& isDisconnected, BufferDataItem*& outDataItem)
{
  BufferDataItem* requestDataItem = NULL;
  int result = 0;
  do
  {
    if (this->isConnected() == false)
    {
      break;
    }

    outDataItem = NULL;
    requestDataItem = this->getCommData();
    if ( (clientInfo == NULL)
      || (requestDataItem == NULL)
      || (requestDataItem->bufferAddress() == NULL)
      )
    {
      break;
    }

    result = this->requestPackage(clientInfo, requestingDataSize, requestDataItem, tryTime, isDisconnected);
    if (result != 0)
    {
      break;
    }
    
    outDataItem = requestDataItem;
    return 0;
  } while (0);

  if (requestDataItem != NULL)
  {
    this->releaseCommData(requestDataItem);
  }
  return -1;
}

////////////////////Shared funtcion///////////////////////
int CommI2CServer::sendPackage(CommI2CClientInfo* clientInfo, BufferDataItem*& sendingDataItem)
{
  int result = 0;
  do
  {
    if ( (clientInfo == NULL)
      || (sendingDataItem == NULL)
      )
    {
      break;
    }

    CommI2CClientPropertiesTAG* clientProperties = (CommI2CClientPropertiesTAG*)clientInfo->clientProperties();
    if ((clientProperties->isHeaderless() != false)
      && (sendingDataItem->dataLength() > COMM_I2C_SYS_BUFFER_LEN)
      )
    {
      break;
    }
    
    // send header
    if (clientProperties->isHeaderless() == false)
    {
      uint8_t nextCommSequenceId = this->nextCommSequenceCore();
      // put data into sendingifo
      CommI2CHeaderTAG header = CommI2CHeaderTAG();
      header.commSequenceId = nextCommSequenceId;
      header.setOpCode((byte)CommHeaderOpCode::CommHeaderOpData);
      header.setOpStatus((byte)CommHeaderStatus::CommHeaderStatusOK);
      header.length = sendingDataItem->dataLength();
      header.checkSum = 0;

      if (clientProperties->useCheckSum() != false)
      {
        header.checkSum = Utility::calculateSum((uint8_t*)&header, sizeof(CommI2CHeaderTAG), 0);
        header.checkSum = Utility::calculateSum((uint8_t*)sendingDataItem->bufferAddress(), sendingDataItem->dataLength(), header.checkSum);
        header.checkSum = (uint16_t)(UTILITY_CHECK_SUM_MAX_VALUE - header.checkSum);
      }

      result = this->sendData(clientProperties->address, (unsigned char*)&header, sizeof(CommI2CHeaderTAG), clientProperties->codecType);
#ifdef COMM_I2C_SERVER_DEBUG_CONSOLE_ENABLED
      CONSOLE_LOG("[sev] senP hed %i %i %i", result, header.getOpCode(), header.checkSum);
#endif // COMM_I2C_SERVER_DEBUG_CONSOLE_ENABLED
      if (result != 0)
      {
        break;
      }
    }

    // send body 
    result = this->sendData(clientProperties->address, (unsigned char*)sendingDataItem->bufferAddress(), sendingDataItem->dataLength(), clientProperties->codecType);
#ifdef COMM_I2C_SERVER_DEBUG_CONSOLE_ENABLED
    CONSOLE_LOG("[sev] senP bod %i", result);
#endif // COMM_I2C_SERVER_DEBUG_CONSOLE_ENABLED
    if (result != 0)
    {
      break;
    }
    return 0;
  } while (0);
  return -1;
}

int CommI2CServer::requestPackage(CommI2CClientInfo* clientInfo, uint8_t requestingDataSize, BufferDataItem* revDataItem, byte tryTime, bool& isDisconnected)
{
  int result = 0;
  isDisconnected = false;
#ifdef COMM_I2C_SERVER_DEBUG_CONSOLE_ENABLED
  CONSOLE_LOG("[sev] reP %i", 0);
#endif // COMM_I2C_SERVER_DEBUG_CONSOLE_ENABLED
  do
  {
    //revDataItem = NULL;
    if (this->isConnected() == false)
    {
      break;
    }

    if ((clientInfo == NULL)
      || (revDataItem == NULL)
      || (revDataItem->bufferAddress() == NULL)
      )
    {
      break;
    }

    CommI2CClientPropertiesTAG* clientProperties = (CommI2CClientPropertiesTAG*)clientInfo->clientProperties();
    bool headerLess = clientProperties->isHeaderless();
    if ((headerLess != false)
      && ((requestingDataSize > COMM_I2C_SYS_BUFFER_LEN)
        || (requestingDataSize <= 0))
      )
    {
#ifdef COMM_I2C_SERVER_DEBUG_CONSOLE_ENABLED
      CONSOLE_LOG("[sev] reP %i", -22);
#endif // COMM_I2C_SERVER_DEBUG_CONSOLE_ENABLED
      break;
    }

    //CommI2CHeaderTAG header = CommI2CHeaderTAG();
    DataSize_t lenToRead = requestingDataSize;
    DataSize_t readLen = 0;
    uint16_t checkSum = 0;
    
    if (headerLess == false)
    {
      lenToRead = sizeof(CommI2CHeaderTAG);
      readLen = this->receiveData(clientProperties->address, (unsigned char*)revDataItem->bufferAddress(), revDataItem->bufferLength(), lenToRead, clientProperties->codecType, tryTime, isDisconnected);
#ifdef COMM_I2C_SERVER_DEBUG_CONSOLE_ENABLED
      CONSOLE_LOG("[sev] reP hed %i %i %i", 1, result, isDisconnected);
#endif // COMM_I2C_SERVER_DEBUG_CONSOLE_ENABLED
      if (isDisconnected != false)
      {
        break;
      }

      if (readLen != lenToRead)
      {
        break;
      }

      CommI2CHeaderTAG* header = (CommI2CHeaderTAG*)revDataItem->bufferAddress();
      // already use check sum, no need to use commCode
      /*if (Utility::isEqualString(header->commCode, COMM_PACKAGE_COMM_CODE_LEN, COMM_PACKAGE_COMM_CODE_COM, COMM_PACKAGE_COMM_CODE_LEN) == false)
      {
        break;
      }*/

      if ((header->getOpCode() != CommHeaderOpCode::CommHeaderOpData)
        || (header->getOpStatus() != CommHeaderStatus::CommHeaderStatusOK)
        )
      {
        break;
      }

      lenToRead = header->length;
      readLen = 0;
      if (clientProperties->useCheckSum() != false)
      {
        checkSum = Utility::calculateSum((uint8_t*)header, sizeof(CommI2CHeaderTAG), 0);
      }

      if ((header->commSequenceId != this->comm_Sequence_Id)
        || (header->getOpCode() != CommHeaderOpCode::CommHeaderOpData)
        )
      {
        break;
      }
    }

    readLen = this->receiveData(clientProperties->address, revDataItem->bufferAddress(), revDataItem->bufferLength(), lenToRead, clientProperties->codecType, tryTime, isDisconnected);
#ifdef COMM_I2C_SERVER_DEBUG_CONSOLE_ENABLED
    CONSOLE_LOG("[sev] reP %i %i", readLen, lenToRead);
#endif // COMM_I2C_SERVER_DEBUG_CONSOLE_ENABLED
    if (isDisconnected != false)
    {
      break;
    }

    if ((headerLess == false)
      && (readLen != lenToRead)
      )
    {
      break;
    }

    if (readLen <= 0)
    {
      break;
    }

    result = revDataItem->setDataLen((DataSize_t)readLen);
    if (result != 0)
    {
      break;
    }

    if ((headerLess != false)
      || (clientProperties->useCheckSum() == false)
      )
    {
      return 0;
    }

    checkSum = Utility::calculateSum((uint8_t*)revDataItem->bufferAddress(), revDataItem->dataLength(), checkSum);
    if ((UTILITY_CHECK_SUM_MAX_VALUE - checkSum) != 0)
    {
      break; // error
    }
    // request ok. 
#ifdef COMM_I2C_SERVER_DEBUG_CONSOLE_ENABLED
    CONSOLE_LOG("[sev] reP %i", 99);
#endif // COMM_I2C_SERVER_DEBUG_CONSOLE_ENABLED
    return 0;
  } while (0);
#ifdef COMM_I2C_SERVER_DEBUG_CONSOLE_ENABLED
  CONSOLE_LOG("[sev] reP %i", -99);
#endif // COMM_I2C_SERVER_DEBUG_CONSOLE_ENABLED
  return -1;
}

uint8_t CommI2CServer::nextCommSequenceCore(void)
{
  this->comm_Sequence_Id++;
  if (this->comm_Sequence_Id == 0)
  {
    this->comm_Sequence_Id = 1;
  }
  return this->comm_Sequence_Id;
}

#endif // _CONF_COMM_I2C_SERVER_ENABLED