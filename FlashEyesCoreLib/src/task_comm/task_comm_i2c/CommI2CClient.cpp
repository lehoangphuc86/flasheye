/////////////////////////////////////////////////
// INCLUDE
#include "CommI2CClient.h"
#if (_CONF_COMM_I2C_CLIENT_ENABLED)
#include "../../utility/Utility.h"
/////////////////////////////////////////////////
// PREPROCESSOR
//#define COMM_I2C_CLIENT_DEBUG_CONSOLE_ENABLED
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

/* CommI2CClient*/

////////////////////Function to call from outside ISR///////////////////////
CommI2CClient::CommI2CClient(byte commId)
  : CommI2CSocket(commId)
  , registered_Instance_Index(COMM_I2C_CLIENT_INDEX_INVALID)
{
  memset(&this->sending_Info, 0, sizeof(CommI2CSendingInfoTAG));
  memset(&this->receiving_Info, 0, sizeof(CommI2CReceivingInfoTAG));
  CommI2CClient::registerInstance(this);
}

CommI2CClient::~CommI2CClient(void)
{
  this->stop();
  this->cleanUp();
}

int CommI2CClient::setConfig(CommI2CConfigTAG& commI2CConfig)
{
  do
  {
    if ((this->isInitialized() != false)
      || (this->isDisconnected() == false)
      )
    {
      return -1; // already config
    }

    if (this->registeredInstanced() == false)
    {
      break;
    }

    int ret = CommI2CSocket::setConfig(commI2CConfig);
    if (ret != 0)
    {
      break;
    }

    return 0;
  } while (0);

  this->stop();
  return -1;
}

int CommI2CClient::prepare(void)
{
  do
  {
    if (this->isDisconnected() == false)
    {
      return -1;
    }

    if (this->registeredInstanced() == false)
    {
      break;
    }

    int ret = CommI2CSocket::prepare();
    if (ret != 0)
    {
      break;
    }
    return 0;

  } while (0);
  return -1;
}

int CommI2CClient::start(CommI2CSocketConfigTAG& socketConfig)
{
  int result = 0;
  do
  {
    if (this->isDisconnected() == false)
    {
      return -1;
    }

    this->stop();
    if ((this->isInitialized() == false)
      || (this->registeredInstanced() == false)
      )
    {
      break;
    }
    result = this->prepare();
    if (result != 0)
    {
      break;
    }

    result = CommI2CSocket::start(socketConfig);
    if (result != 0)
    {
      break;
    }

    result = CommI2CClient::registerCallback(this, &this->i2c_Socket);
    if (result != 0)
    {
      break;
    }

    return 0;
  } while (0);

  this->stop();
  return -1;
}


bool CommI2CClient::registeredInstanced(void)
{
  return (this->registered_Instance_Index == COMM_I2C_CLIENT_INDEX_INVALID ? false : true);
}

void CommI2CClient::stop(void)
{
  CommI2CSocket::stop();
}

////////////////////Function to call from inside ISR///////////////////////
int CommI2CClient::sendFromISR(unsigned char* data, DataSize_t dataSize)
{
  BufferDataItem* dataItem = NULL;

  int ret = 0;
  do
  {
    if ((data == NULL)
      || (dataSize <= 0)
      )
    {
      break;
    }
    dataItem = this->getCommDataFromISR();
    if ((dataItem == NULL)
      || (dataItem->bufferAddress() == NULL)
      || (dataItem->bufferLength() < dataSize)
      )
    {
      break;
    }

    ret = dataItem->setData(dataSize, data);
    if (ret != 0)
    {
      break;
    }

    if (this->comm_Connection_Setting.isHeaderLess() != false)
    {
      // header only
      break;
    }
    ret =  this->setDataToSendFromISR(dataItem, this->comm_Sequence_Id);
    if (ret != 0)
    {
      //this->releaseCommDataFromISR(dataItem);
      break;
    }
    return 0;
  } while (0);

  if (dataItem != NULL)
  {
    this->releaseCommDataFromISR(dataItem);
  }

  return -1;
}


void CommI2CClient::onRequestFromISR(bool* woken)
{
  int result = 0;
#ifdef COMM_I2C_CLIENT_DEBUG_CONSOLE_ENABLED
  CONSOLE_LOG("[cli] reqed %i", 0);
#endif // COMM_I2C_CLIENT_DEBUG_CONSOLE_ENABLED
  do
  {
    // simple protocol
    //  callback, upper set data and this function sends data
    // advanced protocol
    //  alread set data onReceive cb, this function sends data 
    bool isHeaderLess = this->comm_Connection_Setting.isHeaderLess();
    if ( isHeaderLess != false)
    {
      CommI2CCbOnDataRequested cbOnRequest = this->comm_Connection_Setting.cbDataRequested();
      if (cbOnRequest == NULL)
      {
        break;
      }

      BufferDataItem* dataItem = this->getCommDataFromISR();
      if ((dataItem == NULL)
        || (dataItem->bufferAddress() == NULL)
        )
      {
        break;
      }
      
      // let upper know the request, upper side could use the buffer to send.
      int8_t errorCode = 0;
      CommI2CCBOnDataRequestedParams requestParams = CommI2CCBOnDataRequestedParams();
      requestParams.parentPointer = this->comm_Connection_Setting.parentPointer();
      requestParams.dataItem = dataItem;
      requestParams.errorCode = &errorCode;
      requestParams.higherTaskWoken = woken;
      cbOnRequest(&requestParams, sizeof(CommI2CCBOnDataRequestedParams));
      if (errorCode != 0) // error occurred
      {
        this->releaseCommDataFromISR(dataItem);
        break;
      }

      if ((isHeaderLess != false)
        && (dataItem->dataLength() > COMM_I2C_SYS_BUFFER_LEN)
        )
      {
        this->releaseCommDataFromISR(dataItem);
        break;
      }

      result = this->setDataToSendFromISR(dataItem, this->comm_Sequence_Id);
      if (result != 0)
      {
        this->releaseCommDataFromISR(dataItem);
        break;
      }
    }

    // sending data
    DataSize_t headerLen = (isHeaderLess == false? sizeof(CommI2CHeaderTAG) : 0);
    DataSize_t bodyLen = this->sending_Info.header.length;
    if (this->sending_Info.sentLen >= (bodyLen + headerLen))
    {
      this->resetSendingInfoFromISR();
      return;
    }

    if (this->sending_Info.sentLen < headerLen)
    {
      // have not sent header
      // check sum
      if (this->comm_Connection_Setting.useCheckSum() != false)
      {
        this->sending_Info.header.checkSum = Utility::calculateSum((uint8_t*)&this->sending_Info.header, headerLen, 0);
        this->sending_Info.header.checkSum = Utility::calculateSum((uint8_t*)this->sending_Info.dataItem->bufferAddress(), bodyLen, this->sending_Info.header.checkSum);
        this->sending_Info.header.checkSum = (uint16_t)(UTILITY_CHECK_SUM_MAX_VALUE - this->sending_Info.header.checkSum);
      }
      // send header
      result = this->sendData(COMM_I2C_ADDRESS_INVALID, (unsigned char*)&this->sending_Info.header, headerLen, this->comm_Connection_Setting.codecType());
      if (result != 0)
      {
        break;
      }

      this->sending_Info.sentLen = headerLen;
      return;
    }
    
    // send body
    DataSize_t bodySentLen = this->sending_Info.sentLen - headerLen;
    DataSize_t bodyLenToSend = SYSTEM_MIN(COMM_I2C_SYS_BUFFER_LEN, bodyLen - bodySentLen);
    result = this->sendData(COMM_I2C_ADDRESS_INVALID, this->sending_Info.dataItem->bufferAddress() + bodySentLen, bodyLenToSend, this->comm_Connection_Setting.codecType());
    if (result != 0)
    {
      break;
    }
    this->sending_Info.sentLen += bodyLenToSend; 
    if (this->sending_Info.sentLen >= (headerLen + bodyLen))
    {
      this->resetSendingInfoFromISR();
    }
    return;
  } while (0);

  this->resetSendingInfoFromISR();
  return;
}

void CommI2CClient::onReceivedFromISR(int byteNumber, bool* woken)
{
#ifdef COMM_I2C_CLIENT_DEBUG_CONSOLE_ENABLED
  CONSOLE_LOG("[cli] rev %i %i", 0, byteNumber);
#endif // COMM_I2C_CLIENT_DEBUG_CONSOLE_ENABLED
  int result = 0;
  do
  {
    DataSize_t readLen = 0;
    bool isDisconnected =  false;
    byte tryTime = 0;
    byte codecType = this->comm_Connection_Setting.codecType();
    if (this->receiving_Info.dataItem == NULL)
    {
      this->receiving_Info.dataItem = this->getCommDataFromISR();
    }

    bool isHeaderLess = this->comm_Connection_Setting.isHeaderLess();
    if (isHeaderLess != false)
    {
      // rev data and callback parent
      // return 
      readLen = this->receiveData(COMM_I2C_ADDRESS_INVALID, this->receiving_Info.dataItem->bufferAddress(), this->receiving_Info.dataItem->bufferLength(), byteNumber, codecType, tryTime, isDisconnected);
      if (isDisconnected != false)
      {
        break;
      }

      if (readLen != byteNumber)
      {
        break;
      }

      result = this->notifyParentDataReceivedFromISR(woken);
      this->resetReceivingInfoFromISR();
      if (result != 0)
      {
        break;
      }
      return;
    }
    // if no rev header
    //  rev header
    //  check header
    //  return
    DataSize_t headerLen = sizeof(CommI2CHeaderTAG);
    if (this->receiving_Info.receivedLen < headerLen)
    {
      readLen = this->receiveData(COMM_I2C_ADDRESS_INVALID, (unsigned char*)&this->receiving_Info.header, sizeof(CommI2CHeaderTAG), headerLen, codecType, tryTime, isDisconnected);
      if ( (isDisconnected != false)
        || (readLen != headerLen)
        )
      {
        break;
      }

      if (this->receiving_Info.header.getOpCode() != (byte)CommHeaderOpCode::CommHeaderOpData)
      {
        break;
      }
      this->comm_Sequence_Id = this->receiving_Info.header.commSequenceId;
      this->receiving_Info.receivedLen = headerLen;
      return;
    }
    // rev body
    //  rev data
    //  check complete
    //    check sum 
    //    cb
    DataSize_t bodyLen = this->receiving_Info.header.length;
    DataSize_t bodyRevLen = this->receiving_Info.receivedLen - headerLen;
    DataSize_t bodyLenToRev = SYSTEM_MIN(COMM_I2C_SYS_BUFFER_LEN, bodyLen - bodyRevLen);
    readLen = this->receiveData(COMM_I2C_ADDRESS_INVALID, (unsigned char*)this->receiving_Info.dataItem->bufferAddress() + bodyRevLen, bodyLenToRev, bodyLenToRev, codecType, tryTime, isDisconnected);
    if (readLen != bodyLenToRev)
    {
      break;
    }
    this->receiving_Info.receivedLen += bodyLenToRev;
    if (this->receiving_Info.receivedLen < (bodyLen + headerLen))
    {
      return; // still receiving
    }

    if (this->comm_Connection_Setting.useCheckSum() != false)
    {
      uint16_t checkSum = Utility::calculateSum((uint8_t*)&this->receiving_Info.header, sizeof(CommI2CHeaderTAG), 0);
      checkSum = Utility::calculateSum((uint8_t*)this->receiving_Info.dataItem->bufferAddress(), bodyLen, checkSum);
      if ((UTILITY_CHECK_SUM_MAX_VALUE - checkSum) != 0)
      {
        break; // error
      }
    }
    result = this->notifyParentDataReceivedFromISR(woken);
    if (result != 0)
    {
      break;
    }
    this->resetReceivingInfoFromISR();
    return;
  } while (0);
  this->resetReceivingInfoFromISR();
#ifdef COMM_I2C_CLIENT_DEBUG_CONSOLE_ENABLED
  CONSOLE_LOG("[cli] rev %i", -99);
#endif // COMM_I2C_CLIENT_DEBUG_CONSOLE_ENABLED
  return;
}

int CommI2CClient::notifyParentDataReceivedFromISR(bool* higherTaskWoken)
{
  do
  {
    if (this->receiving_Info.dataItem == NULL)
    {
      return -1;
    }

    int ret = 0;
    void* parentPointer = this->comm_Connection_Setting.parentPointer();
    CommI2CCbOnDataReceived cbOnReceived = (CommI2CCbOnDataReceived)this->comm_Connection_Setting.cbDataReceived();
    if ((parentPointer == NULL)
      || (cbOnReceived == NULL)
      )
    {
      return -1;
    }

    CommI2CCBOnDataReceivedParams receivedParams = CommI2CCBOnDataReceivedParams();
    receivedParams.parentPointer = parentPointer;
    receivedParams.dataItem = this->receiving_Info.dataItem;
    receivedParams.higherTaskWoken = higherTaskWoken;
    this->receiving_Info.dataItem = NULL; // upperside is responsible for releasing buffer
    cbOnReceived(&receivedParams, sizeof(CommI2CCBOnDataReceivedParams));
    return 0;
  } while (0);
  return -1;
}

int CommI2CClient::setDataToSendFromISR(BufferDataItem* &dataItem, uint8_t commSequenceId)
{
  do
  {
    if (dataItem == NULL)
    {
      break;
    }
    if (commSequenceId != this->comm_Sequence_Id)
    {
      break;
    }

    this->resetSendingInfoFromISR();
    this->sending_Info.dataItem = dataItem;
    this->sending_Info.header.commSequenceId = commSequenceId;
    this->sending_Info.header.setOpCode((byte)CommHeaderOpCode::CommHeaderOpData);
    this->sending_Info.header.setOpStatus(CommHeaderStatus::CommHeaderStatusOK);
    this->sending_Info.header.length = dataItem->dataLength();
    this->sending_Info.sentLen = 0;
    return 0;
  } while (0);
  return NULL;
}



void CommI2CClient::resetReceivingInfoFromISR(void)
{
  if (this->receiving_Info.dataItem != NULL)
  {
    this->releaseCommDataFromISR(this->receiving_Info.dataItem);
    this->receiving_Info.dataItem = NULL;
  }
  memset(&this->receiving_Info, 0, sizeof(CommI2CReceivingInfoTAG));
  this->receiving_Info.header.setOpCode((uint8_t)CommHeaderOpCode::CommHeaderOpCodeInvalid);
  if (this->isValidSocketFromISR() != false)
  {
    this->i2c_Socket.flushReceivedBuff();
  }
}

void CommI2CClient::resetSendingInfoFromISR(void)
{
  if (this->sending_Info.dataItem != NULL)
  {
    this->releaseCommDataFromISR(this->sending_Info.dataItem);
    this->sending_Info.dataItem = NULL;
  }
  memset(&this->sending_Info, 0, sizeof(CommI2CSendingInfoTAG));
  this->sending_Info.header.setOpCode((uint8_t)CommHeaderOpCode::CommHeaderOpCodeInvalid);
  if (this->isValidSocketFromISR() != false)
  {
    this->i2c_Socket.flush();
  }
}



// static function for callback handling
byte CommI2CClient::current_Client_Index = 0;
CommI2CClient* CommI2CClient::client_Instances[COMM_I2C_MAX_CLIENT_INSTANCE];
int CommI2CClient::registerInstance(CommI2CClient* instance)
{
  do
  {
    if (instance == NULL)
    {
      break;
    }

    instance->registered_Instance_Index = COMM_I2C_CLIENT_INDEX_INVALID;
    if (CommI2CClient::current_Client_Index == 0)
    {
      memset(CommI2CClient::client_Instances, NULL, COMM_I2C_MAX_CLIENT_INSTANCE);
    }

    if (CommI2CClient::current_Client_Index >= COMM_I2C_MAX_CLIENT_INSTANCE)
    {
      break;
    }

    instance->registered_Instance_Index = CommI2CClient::current_Client_Index;

    CommI2CClient::client_Instances[CommI2CClient::current_Client_Index] = instance;
    CommI2CClient::current_Client_Index++;
    return 0;
  } while (0);
  return -1;
}

int CommI2CClient::registerCallback(CommI2CClient* instance, CommI2CSystemSocket* i2cSocket)
{
  do
  {
    if (instance == NULL)
    {
      break;
    }

    if (instance->registeredInstanced() == false)
    {
      break;
    }

    if (i2cSocket == NULL)
    {
      break;
    }
    switch (instance->registered_Instance_Index)
    {
    case 0:
      i2cSocket->onRequest(CommI2CClient::onRequestFromISR0);
      i2cSocket->onReceive(CommI2CClient::onReceivedFromISR0);
      break;
    case 1:
      i2cSocket->onRequest(CommI2CClient::onRequestFromISR1);
      i2cSocket->onReceive(CommI2CClient::onReceivedFromISR1);
      break;
    case 2:
      i2cSocket->onRequest(CommI2CClient::onRequestFromISR2);
      i2cSocket->onReceive(CommI2CClient::onReceivedFromISR2);
      break;

    default:
      break;
    }

    return 0;
  } while (0);
  return -1;
}


// function to drive onRequest callback
void CommI2CClient::onRequestFromISR0(void)
{
  bool wakeUp = false;
  CommI2CClient::client_Instances[0]->onRequestFromISR(&wakeUp);
  SYSTEM_YEILD_FROM_ISR(wakeUp);
}

void CommI2CClient::onRequestFromISR1(void)
{
  bool wakeUp = false;
  CommI2CClient::client_Instances[1]->onRequestFromISR(&wakeUp);
  SYSTEM_YEILD_FROM_ISR(wakeUp);
}

void CommI2CClient::onRequestFromISR2(void)
{
  bool wakeUp = false;
  CommI2CClient::client_Instances[2]->onRequestFromISR(&wakeUp);
  SYSTEM_YEILD_FROM_ISR(wakeUp);
}

void CommI2CClient::onReceivedFromISR0(int byteNum)
{
  bool wakeUp = false;
  CommI2CClient::client_Instances[0]->onReceivedFromISR(byteNum, &wakeUp);
  SYSTEM_YEILD_FROM_ISR(wakeUp);
}

void CommI2CClient::onReceivedFromISR1(int byteNum)
{
  bool wakeUp = false;
  CommI2CClient::client_Instances[1]->onReceivedFromISR(byteNum, &wakeUp);
  SYSTEM_YEILD_FROM_ISR(wakeUp);
}

void CommI2CClient::onReceivedFromISR2(int byteNum)
{
  bool wakeUp = false;
  CommI2CClient::client_Instances[2]->onReceivedFromISR(byteNum, &wakeUp);
  SYSTEM_YEILD_FROM_ISR(wakeUp);
}

#endif // _CONF_COMM_I2C_CLIENT_ENABLED