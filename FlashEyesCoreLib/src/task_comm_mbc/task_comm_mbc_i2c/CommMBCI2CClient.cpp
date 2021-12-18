/////////////////////////////////////////////////
// INCLUDE
#include "CommMBCI2CClient.h"
#if (_CONF_COMM_MBC_I2C_CLIENT_ENABLED)
/////////////////////////////////////////////////
// PREPROCESSOR
//#define COMM_MBC_I2C_CLIENT_CONSOLE_DEBUG_ENABLE
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

/* CommMBCI2CClient*/
////////////////////Function to call from outside ISR///////////////////////
CommMBCI2CClient::CommMBCI2CClient(byte commId)
  : CommI2CClient(commId)
{
  memset(&this->comm_Mbc_I2C_Config, 0, sizeof(CommMBCI2CConfigTAG));
}

CommMBCI2CClient::~CommMBCI2CClient(void)
{
  this->stop();
  this->cleanUp();
}

uint16_t CommMBCI2CClient::registerMessageId(uint8_t messageId, uint16_t maxRawSize)
{
  return this->comm_Mbc_Engine.registerMessageId(messageId, maxRawSize);
}

int CommMBCI2CClient::setConfig(CommMBCI2CConfigTAG& mbcI2CConfig)
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

    if ((mbcI2CConfig.parentPointer == NULL)
      || (mbcI2CConfig.cbDataReceived == NULL)
      || (mbcI2CConfig.cbDataRequested == NULL)
      )
    {
      break;
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
      commConfig.parentPointer = (void*)this;
      commConfig.cbDataReceived = (CommMBCI2CClient::cbOnDataReceivedFromISRNavigator);
      commConfig.cbDataRequested = (CommMBCI2CClient::cbOnDataRequestedFromISRNavigator);


      ret = CommI2CClient::setConfig(commConfig);
      if (ret != 0)
      {
        break;
      }
      
      memcpy(&this->comm_Mbc_I2C_Config, &mbcI2CConfig, sizeof(CommMBCI2CConfigTAG));
    }
    return 0;
  } while (0);
#ifdef COMM_MBC_I2C_CLIENT_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG("[mbcC] cfig %i", -99);
#endif // COMM_MBC_I2C_CLIENT_CONSOLE_DEBUG_ENABLE
  this->stop();
  return -1;
}

int CommMBCI2CClient::start(CommMBCI2CSocketConfigTAG& socketConfig)
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

      result = CommI2CClient::start(commI2CSocketConfig);
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


int CommMBCI2CClient::prepare(void)
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

    ret = CommI2CClient::prepare();
    if (ret != 0)
    {
      break;
    }

    return 0;

  } while (0);
  return -1;
}

void CommMBCI2CClient::cleanUp(void)
{
  do
  {
    CommI2CClient::cleanUp();
    memset(&this->comm_Mbc_I2C_Config, 0, sizeof(CommMBCI2CConfigTAG));
    return;
  } while (0);
  return;
}


void CommMBCI2CClient::stop(void)
{
  //return CommI2CClient::disconnect();
  this->comm_Mbc_Engine.clear();
  return CommI2CClient::stop();
}

////////////////////Hide parent public interfaces///////////////////////
int CommMBCI2CClient::setConfig(CommI2CConfigTAG& commI2CConfig)
{
  return CommI2CClient::setConfig(commI2CConfig);
}

int CommMBCI2CClient::sendFromISR(unsigned char* data, DataSize_t dataSize)
{
  return CommI2CClient::sendFromISR(data, dataSize);
}

////////////////////Function to call from inside ISR///////////////////////
int CommMBCI2CClient::sendFromISR(uint8_t messageId, unsigned char* inputBuffer, uint16_t inputBufferSize)
{
  //BufferDataItem* upperDataItem = NULL;
  BufferDataItem* mbcSendData = NULL;
#ifdef COMM_MBC_I2C_CLIENT_CONSOLE_DEBUG_ENABLE
  //CONSOLE_LOG("[mbcC] senisr %i %i", 0, messageId);
#endif // COMM_MBC_I2C_CLIENT_CONSOLE_DEBUG_ENABLE
  do
  {
    // this is advanced protocol only
    // call (BufferDataItem*)sendData = engine->encodeData(messageId, inputBuffer, inputBufferSize);
    // call this->sendFromISR(messageId, sendData) 
    if (this->comm_Connection_Setting.isHeaderLess() != false)
    {
      break;
    }

    mbcSendData = this->getCommDataFromISR();
    if (mbcSendData == NULL)
    {
      break;
    }

    int ret = this->comm_Mbc_Engine.encodeData(messageId, inputBuffer, inputBufferSize, mbcSendData);
    if (ret != 0)
    {
      break;
    }

    ret = CommI2CClient::sendFromISR(mbcSendData->bufferAddress(), mbcSendData->dataLength());
    if (ret != 0)
    {
      break;
    }

    if (mbcSendData != NULL)
    {
      this->releaseCommDataFromISR(mbcSendData);
    }

    return 0;
  } while (0);
  
  if (mbcSendData != NULL)
  {
    this->releaseCommDataFromISR(mbcSendData);
  }
#ifdef COMM_MBC_I2C_CLIENT_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG("[mbcC] senisr %i", -99);
#endif // COMM_MBC_I2C_CLIENT_CONSOLE_DEBUG_ENABLE
  return -1;  
}

void CommMBCI2CClient::cbOnDataRequestedFromISR(void* param, DataSize_t paramSize)
{
#ifdef COMM_MBC_I2C_CLIENT_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG("[mbcC] req %i", 0);
#endif // COMM_MBC_I2C_CLIENT_CONSOLE_DEBUG_ENABLE
  // this occur in headerless protocol only
  // parse data to CommI2CCBOnDataRequestedParams* sendRequestInfo
  // pass sendRequestInfo->dataItem to upper side
  // call cb to upper side (EventCommMBCSendRequestedTAG)
  // it is simple protocol and no need to decode/encode
  CommI2CCBOnDataRequestedParams* requestParam = (CommI2CCBOnDataRequestedParams*)param;
  do
  {
    if (this->comm_Connection_Setting.isHeaderLess() == false)
    {
      //simple only
      break;
    }

    if (requestParam->dataItem == NULL)
    {
      break;
    }
    
    CommMBCI2CCbOnDataRequested cbOnRequest = this->comm_Mbc_I2C_Config.cbDataRequested;
    if (cbOnRequest == NULL)
    {
      break;
    }

    CommMBCI2CCBOnDataRequestedParams cbMbcDataRequestParams = CommMBCI2CCBOnDataRequestedParams();
    cbMbcDataRequestParams.parentPointer = this->comm_Mbc_I2C_Config.parentPointer;
    cbMbcDataRequestParams.dataItem = requestParam->dataItem;
    cbMbcDataRequestParams.errorCode = requestParam->errorCode;
    cbMbcDataRequestParams.higherTaskWoken = requestParam->higherTaskWoken;
    cbMbcDataRequestParams.messageId = (uint8_t)CommMBCConstant::CommMBCMessageId::CommMBCHeadLess;
    cbOnRequest(&cbMbcDataRequestParams, sizeof(CommMBCI2CCBOnDataRequestedParams));

    return;
  } while (0);

  if (requestParam->errorCode != NULL)
  {
    *requestParam->errorCode = -1;
  }
#ifdef COMM_MBC_I2C_CLIENT_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG("[mbcC] req %i", -99);
#endif // COMM_MBC_I2C_CLIENT_CONSOLE_DEBUG_ENABLE
  return;
}

void CommMBCI2CClient::cbOnDataReceivedFromISR(void* param, DataSize_t paramSize)
{
#ifdef COMM_MBC_I2C_CLIENT_CONSOLE_DEBUG_ENABLE
  //CONSOLE_LOG("[mbcC] rev %i", 0);
#endif // COMM_MBC_I2C_CLIENT_CONSOLE_DEBUG_ENABLE
  // parse data to CommI2CCBOnDataReceivedParams* dataReceivedInfo
  // dataItem = (BufferDataItem*)dataReceivedInfo->dataItem;
  // translate message
  // uint8_t messageId =  invalid;
  // call (BufferDataItem*)revData = engine->decodeData((simple? true: false), sendRequestInfo->dataItem->bufferAddress, sendRequestInfo->dataItem->dataLength, );
  // call cb to upperside EventCommMBCDataReceivedTAG(messageId, dataItem, clientIdentifier)
  // release revData
  CommI2CCBOnDataReceivedParams* revParam = (CommI2CCBOnDataReceivedParams*)param;
  BufferDataItem* mbcRevDataItem = NULL;
  BufferDataItem* commRevDataItem = NULL;
  do
  {
    commRevDataItem = revParam->dataItem;
    if ((commRevDataItem == NULL)
      || (commRevDataItem->bufferAddress() == NULL)
      )
    {
      break;
    }

    mbcRevDataItem = this->getCommDataFromISR();
    if ((mbcRevDataItem == NULL)
      || (mbcRevDataItem->bufferAddress() == NULL)
      )
    {
      break;
    }

    bool isRawData = this->comm_Connection_Setting.isHeaderLess();
    uint8_t messageId = (uint8_t)CommMBCConstant::CommMBCMessageId::CommMBCMessageIdInvalid;
    int ret = this->comm_Mbc_Engine.decodeData(isRawData, commRevDataItem->bufferAddress(), commRevDataItem->dataLength(), messageId, mbcRevDataItem);
    
    this->releaseCommDataFromISR(commRevDataItem);
    commRevDataItem = NULL;
    if (ret != 0)
    {
      break;
    }
    
    CommMBCI2CCbOnDataReceived cbOnReceived = (CommMBCI2CCbOnDataReceived)this->comm_Mbc_I2C_Config.cbDataReceived;
    if (cbOnReceived == NULL)
    {
      break;
    }
    CommMBCI2CCBOnDataReceivedParams cbMbcDataRevParams = CommMBCI2CCBOnDataReceivedParams();
    cbMbcDataRevParams.parentPointer = this->comm_Mbc_I2C_Config.parentPointer;
    cbMbcDataRevParams.dataItem = mbcRevDataItem;
    cbMbcDataRevParams.higherTaskWoken = revParam->higherTaskWoken;
    cbMbcDataRevParams.messageId = messageId;
    cbOnReceived(&cbMbcDataRevParams, sizeof(CommMBCI2CCBOnDataReceivedParams));
    //this->releaseCommDataFromISR(mbcRevDataItem);
    return;
  } while (0);

  if (commRevDataItem != NULL)
  {
    this->releaseCommDataFromISR(commRevDataItem);
  }

  if (mbcRevDataItem != NULL)
  {
    this->releaseCommDataFromISR(mbcRevDataItem);
  }
#ifdef COMM_MBC_I2C_CLIENT_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG("[mbcC] rev %i", -99);
#endif // COMM_MBC_I2C_CLIENT_CONSOLE_DEBUG_ENABLE
  return;
}


void CommMBCI2CClient::cbOnDataRequestedFromISRNavigator(void* param, DataSize_t paramSize)
{
  do
  {
    //CONSOLE_LOG("reqNavi%s", "1");
    if ( (param == NULL)
      || (paramSize!= sizeof(CommI2CCBOnDataRequestedParams))
      )
    {
      break;
    }

    CommI2CCBOnDataRequestedParams* requestParam = (CommI2CCBOnDataRequestedParams*)param;

    if (requestParam->parentPointer == NULL)
    {
      break;
    }
    return ((CommMBCI2CClient*)requestParam->parentPointer)->cbOnDataRequestedFromISR(param, paramSize);
  } while (0);

  return;
}

void CommMBCI2CClient::cbOnDataReceivedFromISRNavigator(void* param, DataSize_t paramSize)
{
  do
  {
    //CONSOLE_LOG("reqNavi %i", 1);
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
    return ((CommMBCI2CClient*)revParam->parentPointer)->cbOnDataReceivedFromISR(param, paramSize);
  } while (0);

  return;
}

#endif // _CONF_COMM_MBC_I2C_CLIENT_ENABLED