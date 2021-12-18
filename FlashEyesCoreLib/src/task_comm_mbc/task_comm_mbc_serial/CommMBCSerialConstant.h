#include "../../FlashEyesCoreLibConfig.h"
#if (_CONF_COMM_MBC_SERIAL_CONSTANT_ENABLED)
#ifndef _COMM_MBC_SERIAL_CONSTANT_H
#define _COMM_MBC_SERIAL_CONSTANT_H

/////////////////////////////////////////////////
// INCLUDE
#include "../CommMBCConstant.h"
#include "../../task_comm/task_comm_serial/CommSerialConstant.h"
#if (!_CONF_COMM_MBC_CONSTANT_ENABLED)
#error Comm MBC constant is required
#endif // _CONF_COMM_MBC_CONSTANT_ENABLED
/////////////////////////////////////////////////
// PREPROCESSOR

/////////////////////////////////////////////////
// DEFINE

#define COMM_MBC_SERIAL_CONF_MASK_ENABLE_FLAG                 0x80
#define COMM_MBC_SERIAL_CONF_MASK_HEADLESS_FLAG               0x40
#define COMM_MBC_SERIAL_CONF_MASK_CHECKSUM_FLAG               0x20
#define COMM_MBC_SERIAL_CONF_MASK_MBC_HEADLESS_FLAG           0x10

#define COMM_MBC_SERIAL_CONF_MASK_USE_BUFFER_POOL_FLAG        0x08
#define COMM_MBC_SERIAL_CONF_MASK_USE_EVENT_POOL_FLAG         0x04
#define COMM_MBC_SERIAL_CONF_MASK_USE_THREAD_POOL_FLAG        0x02

#define COMM_MBC_SERIAL_PORT_LEN_MAX                          COMM_SERIAL_PORT_LEN_MAX
/////////////////////////////////////////////////
// MARCO

/////////////////////////////////////////////////
// GLOBAL VARIABLES

/////////////////////////////////////////////////
// GLOBAL FUNCTIONS

/////////////////////////////////////////////////
// DATA TYPE (TYPEDEF)
typedef void(*CommMBCSerialCbOnDataReceived)(void* param, DataSize_t paramSize);
typedef CommMBCSerialCbOnDataReceived CommMBCSerialCbOnConnectionClosed;
typedef CommMBCSerialCbOnDataReceived CommMBCSerialCbOnError;
/////////////////////////////////////////////////
// DATA TYPE (ENUM)

/////////////////////////////////////////////////
// DATA TYPE (STRUCT)

typedef struct _commMBCSerialConfigTag
{
  byte boolConf; // 0th: enabled, 1st: isHeaderless, 2nd:useCheckSum, 3rd: isMBCHeaderless, 4h: bufferUsePool, 5th: taskEventUsePool, 6th: useThreadPool
  char terminator = COMM_SERIAL_TERMINATOR_DEFAULT;
  byte codecType; // SerialCodecType. Which codec type to encode/decode data before sending/receiving. Used for sending byte array/struct
  //DataSize_t bufferMaxSize; // buffer size to prepare for sending and receiving
  DataCount_t bufferCount; // number of buffer to prepare for sending and receiving
  void* cbParams; // params specified by parent. This would be sent back to parent via callbacks
  CommMBCSerialCbOnDataReceived cbDataReceived; // callback when data is received.
  CommMBCSerialCbOnConnectionClosed cbConnectionClosed; // callback when connection is closed.
  // use task in arduino based. Ignored in PC based
  EventCount_t taskEventItemNumber;
  unsigned short taskStackDepth;
  byte taskPriority;

  bool enabled(void)
  {
    return this->boolConf & COMM_MBC_SERIAL_CONF_MASK_ENABLE_FLAG;
  }

  void enabled(bool state)
  {
    if (this->enabled() == state)
    {
      return;
    }
    this->boolConf = (this->boolConf ^ COMM_MBC_SERIAL_CONF_MASK_ENABLE_FLAG);
  }

  bool isHeaderless(void)
  {
    return this->boolConf & COMM_MBC_SERIAL_CONF_MASK_HEADLESS_FLAG;
  }

  void isHeaderless(bool state)
  {
    if (this->isHeaderless() == state)
    {
      return;
    }
    this->boolConf = (this->boolConf ^ COMM_MBC_SERIAL_CONF_MASK_HEADLESS_FLAG);
  }

  bool isMBCHeaderless(void)
  {
    return this->boolConf & COMM_MBC_SERIAL_CONF_MASK_MBC_HEADLESS_FLAG;
  }

  void isMBCHeaderless(bool state)
  {
    if (this->isHeaderless() == state)
    {
      return;
    }
    this->boolConf = (this->boolConf ^ COMM_MBC_SERIAL_CONF_MASK_MBC_HEADLESS_FLAG);
  }

  bool useCheckSum(void)
  {
    return this->boolConf & COMM_MBC_SERIAL_CONF_MASK_CHECKSUM_FLAG;
  }

  void useCheckSum(bool state)
  {
    if (this->useCheckSum() == state)
    {
      return;
    }
    this->boolConf = (this->boolConf ^ COMM_MBC_SERIAL_CONF_MASK_CHECKSUM_FLAG);
  }

  bool bufferUsePool(void)
  {
    return this->boolConf & COMM_MBC_SERIAL_CONF_MASK_USE_BUFFER_POOL_FLAG;
  }

  void bufferUsePool(bool state)
  {
    if (this->bufferUsePool() == state)
    {
      return;
    }
    this->boolConf = (this->boolConf ^ COMM_MBC_SERIAL_CONF_MASK_USE_BUFFER_POOL_FLAG);
  }

  bool taskEventUsePool(void)
  {
    return this->boolConf & COMM_MBC_SERIAL_CONF_MASK_USE_EVENT_POOL_FLAG;
  }

  void taskEventUsePool(bool state)
  {
    if (this->taskEventUsePool() == state)
    {
      return;
    }
    this->boolConf = (this->boolConf ^ COMM_MBC_SERIAL_CONF_MASK_USE_EVENT_POOL_FLAG);
  }

  bool taskThreadUsePool(void)
  {
    return this->boolConf & COMM_MBC_SERIAL_CONF_MASK_USE_THREAD_POOL_FLAG;
  }

  void taskThreadUsePool(bool state)
  {
    if (this->taskThreadUsePool() == state)
    {
      return;
    }
    this->boolConf = (this->boolConf ^ COMM_MBC_SERIAL_CONF_MASK_USE_THREAD_POOL_FLAG);
  }

} CommMBCSerialConfigTAG;

typedef CommSerialSocketConfigTAG CommMBCSerialSocketConfigTAG;

typedef struct _eventCommMBCSerialDataReceivedTAG
{
  uint8_t messageId;
  void* cbParams; // for callback case
  uint8_t commId;
  BufferDataItem* dataItem;
} EventCommMBCSerialDataReceivedTAG;

typedef EventCommSerialOpenConnectionTAG EventCommMBCSerialOpenConnectionTAG;
typedef EventCommSerialCloseConnectionTAG EventCommMBCSerialCloseConnectionTAG;
typedef EventCommSerialConnectionClosedTAG EventCommMBCSerialConnectionClosedTAG;
typedef EventCommSerialConnectionOpenedTAG EventCommMBCSerialConnectionOpenedTAG;
typedef EventCommSerialErrorOccurredTAG EventCommMBCSerialErrorOccurredTAG;

/////////////////////////////////////////////////
// STATIC DATA

/////////////////////////////////////////////////
// STATIC FUNCTIONS

/////////////////////////////////////////////////
// EXTERN

/////////////////////////////////////////////////
// CLASS DEFINITION
#endif // _COMM_MBC_SERIAL_CONSTANT_H

#endif // _CONF_COMM_MBC_SERIAL_CONSTANT_ENABLED