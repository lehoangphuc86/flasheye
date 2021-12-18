/////////////////////////////////////////////////
// INCLUDE
#include "TimeReaderTask.h"

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

/* TimeReaderTask*/

////////////////////Function to call from outside ISR///////////////////////
TimeReaderTask::TimeReaderTask(void)
{
}
TimeReaderTask::~TimeReaderTask(void)
{

}

int TimeReaderTask::setConfig(void)
{
  do
  {
    int result = 0;

    CommMBCI2CSocketConfigTAG commI2CSocketConfig = CommMBCI2CSocketConfigTAG();
    commI2CSocketConfig.enabled = true;
    commI2CSocketConfig.busNumer = TIME_READER_SERVER_SOCKET_BUS_NUMBER;
    commI2CSocketConfig.pin_SCL = TIME_READER_SERVER_SOCKET_PIN_SCL_0;
    commI2CSocketConfig.pin_SDA = TIME_READER_SERVER_SOCKET_PIN_SDA_0;
    commI2CSocketConfig.frequency = TIME_READER_SERVER_SOCKET_FREQUENCY;
    commI2CSocketConfig.opMode = TIME_READER_SERVER_SOCKET_OPMODE;
    commI2CSocketConfig.address = TIME_READER_SERVER_SOCKET_ADDRESS;
    commI2CSocketConfig.timeout = TIME_READER_SERVER_SOCKET_TIMEOUT;
    // set up server
    {
      this->i2c_Server.registerMessageId(CommMBCConstant::CommMBCMessageId::CommMBCHeadLess, sizeof(TIME_READER_REG_DATA_SIZE));
      
      CommMBCI2CConfigTAG commConfig = CommMBCI2CConfigTAG();
      memset(&commConfig, 0, sizeof(CommMBCI2CConfigTAG));
      commConfig.enabled = true;
      
      commConfig.i2cProtocol = TIME_READER_SERVER_CONNECTION_PROTOCOL; // not used
      commConfig.parentPointer = NULL; // not used
      commConfig.cbDataReceived = NULL; // not used
      commConfig.cbDataRequested = NULL; // not used
      commConfig.maxBufferCount = TIME_READER_SERVER_CONNECTION_BUFFER_COUNT;
      commConfig.maxClientNumber = TIME_READER_SERVER_CONNECTION_MAX_CLIENT_COUNT;
      result = this->i2c_Server.setConfig(&commConfig, sizeof(CommMBCI2CConfigTAG), &commI2CSocketConfig, sizeof(CommMBCI2CSocketConfigTAG));
      if (result != 0)
      {
        CONSOLE_LOG("%s", "[i2cSev] con set -1");
        break;
      }
    }
   
    return result;
  } while (0);
  return -1;
}

int TimeReaderTask::prepare(void)
{
  do
  {

    int result = this->i2c_Server.prepare();
    if (result != 0)
    {
      CONSOLE_LOG("%s", "[i2cSev] prep con -1");
      break;
    }
    return 0;
  } while (0);
  return -1;
}

int TimeReaderTask::start(void)
{
  do
  {
    int result = this->i2c_Server.start();
    if (result != 0)
    {
      CONSOLE_LOG("[timeR] sta %i", 1);
      break;
    }

    // append client
    {
      CommI2CClientPropertiesTAG clientProperties = CommI2CClientPropertiesTAG();
      clientProperties.address = TIME_READER_SERVER_CLIENT_SOCKET_1_ADDRESS;
      clientProperties.protocol = TIME_READER_SERVER_CLIENT_SOCKET_1_PROTOCOL;
      result = this->i2c_Server.appendClient(&clientProperties);
      if (result != 0)
      {
        CONSOLE_LOG("[timeR] sta %i", 2);
        break;
      }
    }

    return 0;
  } while (0);
  return -1;
}

byte TimeReaderTask::readTimerReg(uint8_t clientAddress, byte regAddress)
{
  BufferDataItem* revDataItem = NULL;
  do
  {
    CommMBCI2CDataSendParams sendParam = CommMBCI2CDataSendParams();
    sendParam.clientAddress = clientAddress;
    sendParam.messageId = CommMBCConstant::CommMBCMessageId::CommMBCHeadLess;
    sendParam.inputBuffer = (unsigned char*)&regAddress;
    sendParam.inputBufferSize = sizeof(regAddress);
    int ret = this->i2c_Server.send(&sendParam, sizeof(CommMBCI2CDataSendParams));
    if (ret != 0)
    {
      break;
    }

    CommMBCI2CDataRequestParams requestParam = CommMBCI2CDataRequestParams();
    requestParam.clientAddress = clientAddress;
    requestParam.messageId = CommMBCConstant::CommMBCMessageId::CommMBCHeadLess;
    requestParam.requestingDataSize = TIME_READER_REG_DATA_SIZE;
    requestParam.dataItem = NULL;
    ret = this->i2c_Server.request(&requestParam, sizeof(CommMBCI2CDataRequestParams));
    revDataItem = requestParam.dataItem;
    if ((ret != 0)
      || (revDataItem == NULL)
      )
    {
      //CONSOLE_LOG("[regRed] %i", 2);
      break;
    }

    unsigned char* buffer = revDataItem->bufferAddress();
    byte revValue = *(byte*)(buffer);
    //CONSOLE_LOG("regRed %i %i", 3, revValue);
   
    if (revDataItem != NULL)
    {
      this->i2c_Server.releaseCommData(revDataItem);
    }

    return revValue;
  } while (0);
  return 0;
  
  if (revDataItem != NULL)
  {
    this->i2c_Server.releaseCommData(revDataItem);
  }
  return 0;
}

byte TimeReaderTask::readTimerData(uint8_t clientAddress, byte regAddress)
{
  byte result = 0;
  byte regData = this->readTimerReg(clientAddress, regAddress);
  switch (regAddress)
  {
  case TIME_READER_REG_SEC:
  {
    byte sec10 = regData >> 4;
    byte sec1 = regData << 4;
    sec1 = sec1 >> 4;
    result = sec10 * 10 + sec1;
  }
  break;
  case TIME_READER_REG_MIN:
  {
    byte min10 = regData >> 4;
    byte min1 = regData << 4;
    min1 = min1 >> 4;
    result = min10 * 10 + min1;
  }
  break;
  case TIME_READER_REG_HRS:
  {
    byte hour10 = regData << 3;
    hour10 = hour10 >> 7;
    byte hour1 = regData << 4;
    hour1 = hour1 >> 4;
    result = hour10 * 10 + hour1;
  }
  break;
  case TIME_READER_REG_DAY:
  {
    byte day = regData << 5;
    day = day >> 5;
    result = day;
  }
  break;
  case TIME_READER_REG_DAT:
  {
    byte date10 = regData << 2;
    date10 = date10 >> 6;
    byte date1 = regData << 4;
    date1 = date1 >> 4;
    result = date10 * 10 + date1;
  }
  break;
  case TIME_READER_REG_MON:
  {
    byte month10 = regData << 3;
    month10 = month10 >> 7;
    byte month1 = regData << 4;
    month1 = month1 >> 4;
    result = month10 * 10 + month1;
  }
  break;
  case TIME_READER_REG_YRS:
  {
    byte year10 = regData >> 4;
    byte year1 = regData << 4;
    year1 = year1 >> 4;
    result = year10 * 10 + year1;
  }
  break;
  default:
    break;
  }
  return result;
}

int TimeReaderTask::getTime(uint8_t clientAddress)
{
  int result = 0;
  do
  {
    byte sec = this->readTimerData(clientAddress, TIME_READER_REG_SEC);
    byte minute = this->readTimerData(clientAddress, TIME_READER_REG_MIN);
    byte hour = this->readTimerData(clientAddress, TIME_READER_REG_HRS);
    byte day = this->readTimerData(clientAddress, TIME_READER_REG_DAY);
    byte date = this->readTimerData(clientAddress, TIME_READER_REG_DAT);
    byte mon = this->readTimerData(clientAddress, TIME_READER_REG_MON);
    byte year = this->readTimerData(clientAddress, TIME_READER_REG_YRS);

    CONSOLE_LOG("[gT] %i/%i/%i(%i) %i:%i:%i",year, mon, date, day, hour, minute,sec);

    return 0;
  } while (0);

  return -1;
}
