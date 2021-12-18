
#ifndef _COMM_I2C_CONSTANT_H
#define _COMM_I2C_CONSTANT_H

/////////////////////////////////////////////////
// INCLUDE
#include "../../FlashEyesCoreLibConfig.h"
#include "../CommConstant.h"
#include "../../data_manager/DataItem.h"
#if (_CONF_COMM_I2C_CONSTANT_ENABLED)

#if (!_CONF_DATA_ITEM_ENABLED)
#error Data item is required
#endif // 

/////////////////////////////////////////////////
// PREPROCESSOR

/////////////////////////////////////////////////
// DEFINE

#ifdef ARDUINO_ESP32_DEV
#define COMM_I2C_MAX_CLIENT_INSTANCE                    2
#endif // ARDUINO_ESP32_DEV

#ifdef SYSTEM_AVR_PLATFORM
#define COMM_I2C_MAX_CLIENT_INSTANCE                    1
#endif // SYSTEM_AVR_PLATFORM

#define COMM_I2C_CLIENT_INDEX_INVALID                   COMM_I2C_MAX_CLIENT_INSTANCE + 1
#define COMM_I2C_SEQUENCE_ID_INVALID                    0
#define COMM_I2C_PIN_INVALID                            0
#define COMM_I2C_PIN_MIN                                PIN_NUMER_MIN
#define COMM_I2C_PIN_MAX                                PIN_NUMBER_MAX
#define COMM_I2C_BUS_NUM_INVALID                        0
#define COMM_I2C_BUS_NUM_MIN                            0
#define COMM_I2C_BUS_NUM_MAX                            COMM_I2C_MAX_CLIENT_INSTANCE - 1
#define COMM_I2C_FREQUENCY_INVALID                      0
#define COMM_I2C_FREQUENCY_MIN                          10000L
#define COMM_I2C_FREQUENCY_MAX                          340000000L
#define COMM_I2C_ENDTRANSACTION_SEND_STOP               true
#define COMM_I2C_BUFFER_SIZE_MIN                        1
#define COMM_I2C_BUFFER_COUNT_MIN                       1
#define COMM_I2C_SYS_BUFFER_LEN                         32
#define COMM_I2C_PACKAGE_OP_CODE_MASK                   0b11110000
#define COMM_I2C_PACKAGE_OP_STATUS_MASK                 0b00001110
//#define COMM_I2C_PACKAGE_RESPOND_REQUIRED_MASK  0b00000001
#define COMM_I2C_ADDRESS_MIN                            0x00
#define COMM_I2C_ADDRESS_MAX                            0x7F
#define COMM_I2C_ADDRESS_INVALID                        0xFF


#define COMM_I2C_CONF_MASK_ENABLE_FLAG                  0x80
#define COMM_I2C_CONF_MASK_HEADLESS_FLAG                0x40
#define COMM_I2C_CONF_MASK_CHECKSUM_FLAG                0x20
#define COMM_I2C_CONF_MASK_RESERVED_FLAG                0x10

#define COMM_I2C_CONF_MASK_USE_BUFFER_POOL_FLAG         0x08
/////////////////////////////////////////////////
// MARCO

/////////////////////////////////////////////////
// GLOBAL VARIABLES

/////////////////////////////////////////////////
// GLOBAL FUNCTIONS

/////////////////////////////////////////////////
// DATA TYPE (TYPEDEF)
// for system socket
typedef void(*CommI2CSystemCbOnRequested)(void);
typedef void(*CommI2CSystemCbOnDataReceived)(int);

// for upper side
// @param1: CommI2CCBOnDataRequestedParams pointer
// @param2: size of param1
typedef void(*CommI2CCbOnDataRequested)(void* param, DataSize_t paramSize);
// @param1: CommI2CCBOnDataReceivedParams pointer
// @param2: size of param1
typedef void(*CommI2CCbOnDataReceived)(void* param, DataSize_t paramSize);
/////////////////////////////////////////////////
// DATA TYPE (ENUM)
/////////////////////////////////////////////////
// DATA TYPE (STRUCT)

typedef struct _commI2CClientPropertiesTAG
{
  uint8_t address;
  bool headerless;
  bool checkSum;
  byte codecType;

  bool useCheckSum(void)
  {
    return this->checkSum;
  }

  void useCheckSum(bool flag)
  {
    this->checkSum = flag;
  }

  bool isHeaderless(void)
  {
    return this->headerless;
  }

  void isHeaderless(bool flag)
  {
    this->headerless = flag;
  }
} CommI2CClientPropertiesTAG;

typedef struct _commI2CSocketConfigTag
{
  uint8_t address; // set COMM_I2C_ADDRESS_INVALID to start as server mode
  byte busNumer;
  byte pin_SCL;
  byte pin_SDA;
  uint32_t frequency;
  uint16_t timeout; // set 0 to use default

  bool isServerMode(void)
  {
    return (this->address == COMM_I2C_ADDRESS_INVALID ? true : false);
  }

  bool isValid(void)
  {
    do
    {

      // validate address
      if (this->address != COMM_I2C_ADDRESS_INVALID)
      {
        if ((this->address < COMM_I2C_ADDRESS_MIN)
          || (this->address > COMM_I2C_ADDRESS_MAX)
          )
        {
          break;
        }
      }

      // validate scl pin
      if (this->pin_SCL != COMM_I2C_PIN_INVALID)
      {
        if ((this->pin_SCL < COMM_I2C_PIN_MIN)
          || (this->pin_SCL > COMM_I2C_PIN_MAX)
          )
        {
          break;
        }
      }

      // validate sda pin
      if (this->pin_SDA != COMM_I2C_PIN_INVALID)
      {
        if ((this->pin_SDA < COMM_I2C_PIN_MIN)
          || (this->pin_SDA > COMM_I2C_PIN_MAX)
          )
        {
          break;
        }
      }

      // validate bus number
      if (this->busNumer != COMM_I2C_BUS_NUM_INVALID)
      {
        if ((this->busNumer < COMM_I2C_BUS_NUM_MIN)
          || (this->busNumer > COMM_I2C_BUS_NUM_MAX)
          )
        {
          break;
        }
      }

      // validate frequency
      if (this->frequency != COMM_I2C_FREQUENCY_INVALID)
      {
        if ((this->frequency < COMM_I2C_FREQUENCY_MIN)
          || (this->frequency > COMM_I2C_FREQUENCY_MAX)
          )
        {
          break;
        }
      }

      
      return true;
    } while (0);

    return false;
  }
} CommI2CSocketConfigTAG;

typedef struct _commI2CConfigTag
{
  // for both
  byte boolConf; // 0th: isEnabled, 1st: isHeaderless (clinet), 2nd useCheckSum, 3rd reseved, 4th: bufferUsePool
  DataSize_t bufferMaxSize; // buffer size to prepare for sending and receiving
  DataCount_t bufferCount; // number of buffer to prepare for sending and receiving
  byte codecType; // SerialCodecType. Which codec type to encode/decode data before sending/receiving. Used for sending byte array/struct
  // for server
  ClientCount_t maxClientNumber;

  // for client
  void* parentPointer; //in case callback is used. For client only.
  CommI2CCbOnDataRequested cbDataRequested; // callback when data is requested. For client only.
  CommI2CCbOnDataReceived cbDataReceived; // callback when data is received. For client only.

  bool enabled(void)
  {
    return this->boolConf & COMM_I2C_CONF_MASK_ENABLE_FLAG;
  }

  void enabled(bool state)
  {
    if (this->enabled() == state)
    {
      return;
    }
    this->boolConf = (this->boolConf ^ COMM_I2C_CONF_MASK_ENABLE_FLAG);
  }

  bool isHeaderless(void)
  {
    return this->boolConf & COMM_I2C_CONF_MASK_HEADLESS_FLAG;
  }

  void isHeaderless(bool state)
  {
    if (this->isHeaderless() == state)
    {
      return;
    }
    this->boolConf = (this->boolConf ^ COMM_I2C_CONF_MASK_HEADLESS_FLAG);
  }

  bool useCheckSum(void)
  {
    return this->boolConf & COMM_I2C_CONF_MASK_CHECKSUM_FLAG;
  }

  void useCheckSum(bool state)
  {
    if (this->useCheckSum() == state)
    {
      return;
    }
    this->boolConf = (this->boolConf ^ COMM_I2C_CONF_MASK_CHECKSUM_FLAG);
  }

  bool bufferUsePool(void)
  {
    return this->boolConf & COMM_I2C_CONF_MASK_USE_BUFFER_POOL_FLAG;
  }

  void bufferUsePool(bool state)
  {
    if (this->bufferUsePool() == state)
    {
      return;
    }
    this->boolConf = (this->boolConf ^ COMM_I2C_CONF_MASK_USE_BUFFER_POOL_FLAG);
  }

  bool isServerMode(void)
  {
    return (this->maxClientNumber == 0 ? false : true);
  }

} CommI2CConfigTAG;

typedef struct _commI2CPackageHeaderTag
{
  uint8_t commSequenceId;
  uint8_t operationFlags; // 7th-4th: opCode, 3rd-1th: status, 1st: reserved // responseRequired
  uint16_t checkSum;
  uint16_t length; // total length of data to transmit. Header size if excluded
  //uint16_t offset; // the starting of data to transmit. Ex: length = 40. 1st package sends offset 0. 2nd one sends offset 32. (max size is 32)

  _commI2CPackageHeaderTag(void)
  {
    this->commSequenceId = COMM_I2C_SEQUENCE_ID_INVALID;
    this->operationFlags = 0;
    this->setOpCode((uint8_t)CommHeaderOpCode::CommHeaderOpCodeInvalid);
    this->setOpStatus((uint8_t)CommHeaderStatus::CommHeaderStatusInvalid);
    this->checkSum = 0;
    this->length = 0;
  }

  uint8_t getOpCode(void)
  {
    uint8_t result = (this->operationFlags & COMM_I2C_PACKAGE_OP_CODE_MASK);
    result = result >> 4;
    return result;
  }

  uint8_t getOpStatus(void)
  {
    uint8_t result = (this->operationFlags & COMM_I2C_PACKAGE_OP_STATUS_MASK);
    result = result >> 1;
    return result;
  }

  void setOpCode(uint8_t newValue)
  {
    newValue = newValue << 4;
    newValue = (newValue & COMM_I2C_PACKAGE_OP_CODE_MASK);
    this->operationFlags = (newValue | (this->operationFlags & ~COMM_I2C_PACKAGE_OP_CODE_MASK));
  }

  void setOpStatus(uint8_t newValue)
  {
    newValue = newValue << 1;
    newValue = (newValue & COMM_I2C_PACKAGE_OP_STATUS_MASK);
    this->operationFlags = (newValue | (this->operationFlags & ~COMM_I2C_PACKAGE_OP_STATUS_MASK));
  }

  bool isValid(void)
  {
    return (this->getOpCode() == (uint8_t)CommHeaderOpCode::CommHeaderOpCodeInvalid ? false : true);
  }

} CommI2CHeaderTAG;


typedef struct _commI2CSendingInfoTag
{
  CommI2CHeaderTAG header;
  uint16_t sentLen;
  BufferDataItem* dataItem; // data to send
} CommI2CSendingInfoTAG;

typedef struct _commI2CReceivingInfoTag
{
  CommI2CHeaderTAG header;
  uint16_t receivedLen;
  BufferDataItem* dataItem; // received data
} CommI2CReceivingInfoTAG;

//typedef struct _commI2CDataSendParams
//{
//  // for server
//  ClientId_t clientId = COMM_CLIENT_ID_INVALID; // set 0 to filter by clientAddress
//  uint8_t clientAddress;
//
//  // for both
//  unsigned char* inputBuffer; // raw buffer
//  DataSize_t inputBufferSize; // raw buffer size
//
//  _commI2CDataSendParams(void)
//  {
//    this->clear();
//  }
//
//  void clear(void)
//  {
//    this->clientId = COMM_CLIENT_ID_INVALID;
//    this->clientAddress = 0;
//    this->inputBuffer = NULL;
//    this->inputBufferSize = 0;
//  }
//
//  bool filterClientById(void)
//  {
//    return (this->clientId == COMM_CLIENT_ID_INVALID ? false : true);
//  }
//} CommI2CDataSendParams;


//typedef struct _commI2CDataRequestParams
//{
//  // this struct is for server only
//  ClientId_t clientId; // set 0 to filter by clientAddress
//  uint8_t clientAddress;
//  BufferDataItem* dataItem;
//  uint8_t requestingDataSize;
//
//  bool filterClientById(void)
//  {
//    return (this->clientId == COMM_CLIENT_ID_INVALID ? false : true);
//  }
//} CommI2CDataRequestParams;

typedef struct _commI2CCBOnDataRequestedParams
{
  // this struct is for client only
  void* parentPointer;
  BufferDataItem* dataItem;
  int8_t* errorCode;
  bool* higherTaskWoken;
} CommI2CCBOnDataRequestedParams;

typedef struct _commI2CCBOnDataReceivedParams
{
  // this struct is for client only
  void* parentPointer;
  BufferDataItem* dataItem;
  bool* higherTaskWoken;
} CommI2CCBOnDataReceivedParams;

/////////////////////////////////////////////////
// STATIC DATA

/////////////////////////////////////////////////
// STATIC FUNCTIONS

/////////////////////////////////////////////////
// EXTERN

/////////////////////////////////////////////////
// CLASS DEFINITION
class CommI2CConstant
{
public:
  /*static bool isValidI2CProtocol(byte protocol)
  {
    switch (protocol)
    {
      case (byte)CommI2CProtocol::CommI2CAdvanced:
      case (byte)CommI2CProtocol::CommI2CSimple:
        return true;
      default:
        return false;
    }
  }*/
};
#endif // _CONF_COMM_I2C_CONSTANT_ENABLED

#endif // _COMM_I2C_CONSTANT_H

