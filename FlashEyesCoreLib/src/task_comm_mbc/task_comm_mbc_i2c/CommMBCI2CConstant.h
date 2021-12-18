#include "../../FlashEyesCoreLibConfig.h"
#if (_CONF_COMM_MBC_I2C_CONSTANT_ENABLED)
#ifndef _COMM_MBC_I2C_CONSTANT_H
#define _COMM_MBC_I2C_CONSTANT_H

/////////////////////////////////////////////////
// INCLUDE
#include "../CommMBCConstant.h"
#include "../../task_comm/task_comm_i2c/CommI2CConstant.h"
#if (!_CONF_COMM_MBC_CONSTANT_ENABLED)
#error Comm MBC constant is required
#endif // _CONF_COMM_MBC_CONSTANT_ENABLED
/////////////////////////////////////////////////
// PREPROCESSOR

/////////////////////////////////////////////////
// DEFINE

#define COMM_MBC_I2C_CONF_MASK_ENABLE_FLAG              0x80
#define COMM_MBC_I2C_CONF_MASK_HEADLESS_FLAG            0x40
#define COMM_MBC_I2C_CONF_MASK_CHECKSUM_FLAG            0x20
#define COMM_MBC_I2C_CONF_MASK_RESERVED_FLAG            0x10

#define COMM_MBC_I2C_CONF_MASK_USE_BUFFER_POOL_FLAG     0x08

/////////////////////////////////////////////////
// MARCO

/////////////////////////////////////////////////
// GLOBAL VARIABLES

/////////////////////////////////////////////////
// GLOBAL FUNCTIONS

/////////////////////////////////////////////////
// DATA TYPE (TYPEDEF)

// for upper side
// @param1: CommMBCI2CCBOnDataRequestedParams pointer
// @param2: size of param1
typedef void(*CommMBCI2CCbOnDataRequested)(void* param, DataSize_t paramSize);
// @param1: CommMBCI2CCBOnDataReceivedParams pointer
// @param2: size of param1
typedef void(*CommMBCI2CCbOnDataReceived)(void* param, DataSize_t paramSize);

/////////////////////////////////////////////////
// DATA TYPE (ENUM)

/////////////////////////////////////////////////
// DATA TYPE (STRUCT)

typedef struct _commMBCI2CConfigTag
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
  CommMBCI2CCbOnDataRequested cbDataRequested; // callback when data is requested. For client only. 
  CommMBCI2CCbOnDataReceived cbDataReceived; // callback when data is received. For client only.

  bool enabled(void)
  {
    return this->boolConf & COMM_MBC_I2C_CONF_MASK_ENABLE_FLAG;
  }

  void enabled(bool state)
  {
    if (this->enabled() == state)
    {
      return;
    }
    this->boolConf = (this->boolConf ^ COMM_MBC_I2C_CONF_MASK_ENABLE_FLAG);
  }

  bool isHeaderless(void)
  {
    return this->boolConf & COMM_MBC_I2C_CONF_MASK_HEADLESS_FLAG;
  }

  void isHeaderless(bool state)
  {
    if (this->isHeaderless() == state)
    {
      return;
    }
    this->boolConf = (this->boolConf ^ COMM_MBC_I2C_CONF_MASK_HEADLESS_FLAG);
  }

  bool useCheckSum(void)
  {
    return this->boolConf & COMM_MBC_I2C_CONF_MASK_CHECKSUM_FLAG;
  }

  void useCheckSum(bool state)
  {
    if (this->useCheckSum() == state)
    {
      return;
    }
    this->boolConf = (this->boolConf ^ COMM_MBC_I2C_CONF_MASK_CHECKSUM_FLAG);
  }

  bool bufferUsePool(void)
  {
    return this->boolConf & COMM_MBC_I2C_CONF_MASK_USE_BUFFER_POOL_FLAG;
  }

  void bufferUsePool(bool state)
  {
    if (this->bufferUsePool() == state)
    {
      return;
    }
    this->boolConf = (this->boolConf ^ COMM_MBC_I2C_CONF_MASK_USE_BUFFER_POOL_FLAG);
  }

  bool isServerMode(void)
  {
    return (this->maxClientNumber == 0 ? false : true);
  }
} CommMBCI2CConfigTAG;

typedef CommI2CSocketConfigTAG CommMBCI2CSocketConfigTAG;

typedef struct _commMBCI2CCBOnDataRequestedParams
{
  // this struct is for client only
  uint8_t messageId;
  void* parentPointer;
  BufferDataItem* dataItem;
  int8_t* errorCode;
  bool* higherTaskWoken;
} CommMBCI2CCBOnDataRequestedParams;

typedef struct _commMBCI2CCBOnDataReceivedParams
{
  // this struct is for client only
  uint8_t messageId;
  void* parentPointer;
  BufferDataItem* dataItem;
  bool* higherTaskWoken;
} CommMBCI2CCBOnDataReceivedParams;


//typedef struct _commMBCI2CDataSendParams
//{
//  // for server
//  ClientId_t clientId; // set 0 to filter by clientAddress
//  uint8_t clientAddress;
//
//  // for both
//  uint8_t messageId;
//  unsigned char* inputBuffer; // raw buffer
//  uint16_t inputBufferSize; // raw buffer size
//
//  // functions
//  bool filterClientById(void)
//  {
//    return (this->clientId == COMM_CLIENT_ID_INVALID ? false : true);
//  }
//} CommMBCI2CDataSendParams;


//typedef struct _commMBCI2CDataRequestParams
//{
//  // this struct is for server only
//  uint8_t messageId;
//  ClientId_t clientId; // set 0 to filter by clientAddress
//  uint8_t clientAddress;
//  BufferDataItem* dataItem;
//  uint8_t requestingDataSize;
//
//  bool filterClientById(void)
//  {
//    return (this->clientId == COMM_CLIENT_ID_INVALID ? false : true);
//  }
//} CommMBCI2CDataRequestParams;

/////////////////////////////////////////////////
// STATIC DATA

/////////////////////////////////////////////////
// STATIC FUNCTIONS

/////////////////////////////////////////////////
// EXTERN

/////////////////////////////////////////////////
// CLASS DEFINITION
/*CommMBCI2CConstant*/

#endif // _COMM_MBC_I2C_CONSTANT_H

#endif // _CONF_COMM_MBC_I2C_CONSTANT_ENABLED