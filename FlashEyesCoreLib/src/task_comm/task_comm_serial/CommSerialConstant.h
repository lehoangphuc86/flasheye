
#ifndef _COMM_SERIAL_CONSTANT_H
#define _COMM_SERIAL_CONSTANT_H

/////////////////////////////////////////////////
// INCLUDE
#include "../../FlashEyesCoreLibConfig.h"
#include "../CommConstant.h"
#include "../../data_manager/DataItem.h"
#include "../../task_codec/TaskCodec.h"
#include "../../data_manager/DataItem.h"
#if (_CONF_COMM_SERIAL_CONSTANT_ENABLED)

#if (!_CONF_DATA_ITEM_ENABLED)
#error Data item is required
#endif // 

#if (!_CONF_TASK_CODEC_ENABLED)
#error Task codec is required
#endif // _CONF_TASK_CODEC_ENABLED

/////////////////////////////////////////////////
// PREPROCESSOR

/////////////////////////////////////////////////
// DEFINE

#ifdef SYSTEM_ARDUINO_BASED

#ifdef SYSTEM_ESP_PLATFORM
#undef COMM_SERIAL_INSTANCE_IDX_MAX
#define COMM_SERIAL_INSTANCE_IDX_MAX   2

#define COMM_SERIAL_HAVE_HWSERIAL0
#define COMM_SERIAL_HAVE_HWSERIAL1
#define COMM_SERIAL_HAVE_HWSERIAL2

#if defined(NO_GLOBAL_INSTANCES) || defined(NO_GLOBAL_SERIAL)
HardwareSerial Serial(0);
HardwareSerial Serial1(1);
HardwareSerial Serial2(2);
#endif

#else // SYSTEM_ESP_PLATFORM

// Arduino 
#if defined(UBRRH) || defined(UBRR0H)
#define COMM_SERIAL_HAVE_HWSERIAL0
#undef COMM_SERIAL_INSTANCE_IDX_MAX
#define COMM_SERIAL_INSTANCE_IDX_MAX   0
#endif // defined(UBRRH) || defined(UBRR0H)

#if defined(UBRR1H)
#define COMM_SERIAL_HAVE_HWSERIAL1
#undef COMM_SERIAL_INSTANCE_IDX_MAX
#define COMM_SERIAL_INSTANCE_IDX_MAX   1
#endif // defined(UBRR1H)

#if defined(UBRR2H)
#define COMM_SERIAL_HAVE_HWSERIAL2
#undef COMM_SERIAL_INSTANCE_IDX_MAX
#define COMM_SERIAL_INSTANCE_IDX_MAX   2
#endif // defined(UBRR2H)

#if defined(UBRR3H)
#define COMM_SERIAL_HAVE_HWSERIAL3
#undef COMM_SERIAL_INSTANCE_IDX_MAX
#define COMM_SERIAL_INSTANCE_IDX_MAX   3
#endif // defined(UBRR3H)

#endif // SYSTEM_ESP_PLATFORM

#define COMM_SERIAL_SYS_BUFFER_LEN              32
#define COMM_SERIAL_TIME_OUT_SLICE              10 // ms
#else // SYSTEM_ARDUINO_BASED
// SYSTEM_PC_BASED
#define COMM_SERIAL_SYS_BUFFER_LEN              8

#endif // SYSTEM_ARDUINO_BASED


#ifdef _CONF_COMM_SERIAL_WITH_TASK_ENABLED
#define COMM_SERIAL_USE_TASK
#endif // _CONF_COMM_SERIAL_WITH_TASK_ENABLED

#define COMM_SERIAL_PIN_INVALID                 0
#define COMM_SERIAL_PIN_MIN                     PIN_NUMER_MIN
#define COMM_SERIAL_PIN_MAX                     PIN_NUMBER_MAX
#define COMM_SERIAL_PIN_LIB_DEFAULT             -1
#define COMM_SERIAL_INSTANCE_IDX_MIN            0
#define COMM_SERIAL_BIT_PER_BYTE_MIN            5
#define COMM_SERIAL_BIT_PER_BYTE_MAX            8
#define COMM_SERIAL_STOP_BIT_NUM_MIN            1
#define COMM_SERIAL_STOP_BIT_NUM_MAX            2

//#define COMM_SERIAL_CONFIG_MIN                  SERIAL_5N1
//#define COMM_SERIAL_CONFIG_MAX                  SERIAL_8O2
#define COMM_SERIAL_BAUDRATE_MIN                600L
#define COMM_SERIAL_BAUDRATE_MAX                20000000L
#define COMM_SERIAL_TIMEOUT_MIN                 0
#define COMM_SERIAL_TIMEOUT_MAX                 1000

#define COMM_SERIAL_TERMINATOR_DEFAULT          '\r'
#define COMM_SERIAL_TERMINATOR_LEN              1
#define COMM_SERIAL_BUFFER_SIZE_MIN             1
#define COMM_SERIAL_BUFFER_COUNT_MIN            1
// pc
#define COMM_SERIAL_PORT_INVALID                ""
#define COMM_SERIAL_PORT_LEN_MAX                COMM_HOST_MAX_LEN

#ifdef COMM_SERIAL_USE_TASK
#define COMM_SERIAL_DISCONNECT_MAX_WAIT         SYSTEM_MUTEX_MAX_WAIT_TIME
#define COMM_SERIAL_CONNECT_MAX_WAIT            SYSTEM_MUTEX_MAX_WAIT_TIME
#else // COMM_SERIAL_USE_TASK
#define COMM_SERIAL_DISCONNECT_MAX_WAIT         0
#endif // COMM_SERIAL_USE_TASK

#define COMM_SERIAL_CONF_MASK_ENABLE_FLAG                   0x80
#define COMM_SERIAL_CONF_MASK_HEADLESS_FLAG                 0x40
#define COMM_SERIAL_CONF_MASK_CHECKSUM_FLAG                 0x20
#define COMM_SERIAL_CONF_MASK_RESERVED_FLAG                 0x10

#define COMM_SERIAL_CONF_MASK_USE_BUFFER_POOL_FLAG          0x08
#define COMM_SERIAL_CONF_MASK_USE_EVENT_POOL_FLAG           0x04
#define COMM_SERIAL_CONF_MASK_USE_THREAD_POOL_FLAG          0x02


/////////////////////////////////////////////////
// MARCO

/////////////////////////////////////////////////
// GLOBAL VARIABLES

/////////////////////////////////////////////////
// GLOBAL FUNCTIONS

/////////////////////////////////////////////////
// DATA TYPE (TYPEDEF)
// @param1: CommSerialCBOnDataReceivedParams pointer
// @param2: size of param1
typedef void(*CommSerialCbOnDataReceived)(void* param, DataSize_t paramSize);
typedef CommSerialCbOnDataReceived CommSerialCbOnConnectionClosed;
typedef CommSerialCbOnDataReceived CommSerialCbOnError;


/////////////////////////////////////////////////
// DATA TYPE (ENUM)

/////////////////////////////////////////////////
// DATA TYPE (STRUCT)
typedef struct _commSerialSocketConfigTag
{
  bool enabled;
  byte bitPerByte;  // 5-8
  byte parityType; // SerialParityType
  byte stopBitNum; // 1-2
  uint32_t baudrate;
  uint16_t timeout; // ms. set 0 to use default
  
  // arudino
  byte instanceIndex;
  byte pin_TX;
  byte pin_RX;
  // pc
  char port[COMM_SERIAL_PORT_LEN_MAX]; /* for COM10 and above, it must append  \\. Ex "\\COM10" */
} CommSerialSocketConfigTAG;

typedef struct _commSerialConfigTag
{
  byte boolConf; // 0th: enabled, 1st: isHeaderless, 2nd:useCheckSum, 3rd: reserved, 4h: bufferUsePool, 5th: taskEventUsePool, 6th: useThreadPool
  char terminator = COMM_SERIAL_TERMINATOR_DEFAULT;
  byte codecType; // SerialCodecType. Which codec type to encode/decode data before sending/receiving. Used for sending byte array/struct
  DataSize_t bufferMaxSize; // buffer size to prepare for sending and receiving
  DataCount_t bufferCount; // number of buffer to prepare for sending and receiving
  void* cbParams; // params specified by parent. This would be sent back to parent via callbacks
  CommSerialCbOnDataReceived cbDataReceived; // callback when data is received.
  CommSerialCbOnConnectionClosed cbConnectionClosed; // callback when connection is closed.
  EventCount_t taskEventItemNumber;
  //bool taskEventUsePool;

  // use task in arduino based. Ignored in PC based
  unsigned short taskStackDepth;
  byte taskPriority;
  //bool useThreadPool;

  bool enabled(void)
  {
    return this->boolConf & COMM_SERIAL_CONF_MASK_ENABLE_FLAG;
  }

  void enabled(bool state)
  {
    if (this->enabled() == state)
    {
      return;
    }
    this->boolConf = (this->boolConf ^ COMM_SERIAL_CONF_MASK_ENABLE_FLAG);
  }

  bool isHeaderless(void)
  {
    return this->boolConf & COMM_SERIAL_CONF_MASK_HEADLESS_FLAG;
  }

  void isHeaderless(bool state)
  {
    if (this->isHeaderless() == state)
    {
      return;
    }
    this->boolConf = (this->boolConf ^ COMM_SERIAL_CONF_MASK_HEADLESS_FLAG);
  }

  bool useCheckSum(void)
  {
    return this->boolConf & COMM_SERIAL_CONF_MASK_CHECKSUM_FLAG;
  }

  void useCheckSum(bool state)
  {
    if (this->useCheckSum() == state)
    {
      return;
    }
    this->boolConf = (this->boolConf ^ COMM_SERIAL_CONF_MASK_CHECKSUM_FLAG);
  }

  bool bufferUsePool(void)
  {
    return this->boolConf & COMM_SERIAL_CONF_MASK_USE_BUFFER_POOL_FLAG;
  }

  void bufferUsePool(bool state)
  {
    if (this->bufferUsePool() == state)
    {
      return;
    }
    this->boolConf = (this->boolConf ^ COMM_SERIAL_CONF_MASK_USE_BUFFER_POOL_FLAG);
  }

  bool taskEventUsePool(void)
  {
    return this->boolConf & COMM_SERIAL_CONF_MASK_USE_EVENT_POOL_FLAG;
  }

  void taskEventUsePool(bool state)
  {
    if (this->taskEventUsePool() == state)
    {
      return;
    }
    this->boolConf = (this->boolConf ^ COMM_SERIAL_CONF_MASK_USE_EVENT_POOL_FLAG);
  }

  bool taskThreadUsePool(void)
  {
    return this->boolConf & COMM_SERIAL_CONF_MASK_USE_THREAD_POOL_FLAG;
  }

  void taskThreadUsePool(bool state)
  {
    if (this->taskThreadUsePool() == state)
    {
      return;
    }
    this->boolConf = (this->boolConf ^ COMM_SERIAL_CONF_MASK_USE_THREAD_POOL_FLAG);
  }
} CommSerialConfigTAG;



typedef struct __ATTRIBUTE_ALIGN _commSerialHeaderTAG
{
  uint16_t checkSum; // used to check sum of both header and body
  uint16_t length; // total length of data to transmit. Header size if excluded
  uint8_t operationFlags; // 7th-4th: opCode, 3rd-1th: status, 1st: reserved
  //char commCode[COMM_PACKAGE_COMM_CODE_LEN]; // TCP | I2C | COM | BLE | BLU | 
  uint8_t getOpCode(void)
  {
    uint8_t result = (this->operationFlags & COMM_PACKAGE_OP_CODE_MASK);
    result = (uint8_t)(result >> 4);
    return result;
  }

  uint8_t getOpStatus(void)
  {
    uint8_t result = (this->operationFlags & COMM_PACKAGE_OP_STATUS_MASK);
    result = (uint8_t) (result >> 1);
    return result;
  }

  void setOpCode(uint8_t newValue)
  {
    newValue = (uint8_t) (newValue << 4);
    newValue = (newValue & COMM_PACKAGE_OP_CODE_MASK);
    this->operationFlags = (newValue | this->operationFlags);
  }

  void setOpStatus(uint8_t newValue)
  {
    newValue = (uint8_t)(newValue << 1);
    newValue = (newValue & COMM_PACKAGE_OP_STATUS_MASK);
    this->operationFlags = (newValue | this->operationFlags);
  }

  bool isValid(void)
  {
    return (this->getOpCode() == (uint8_t)CommHeaderOpCode::CommHeaderOpCodeInvalid ? false : true);
  }

} CommSerialHeaderTAG;

//typedef struct _commSerialSendRequestedParamsTAG
//{
//  uint16_t clientId;
//  unsigned char* inputBuffer; // raw buffer
//  uint16_t inputBufferSize; // raw buffer size
//} CommSerialSendRequestedParamsTAG;

//typedef struct _commSerialDataRequestParamsTAG
//{
//  //void* cbParams; // for callback case
//  //uint8_t commId;
//  uint16_t clientId;
//  BufferDataItem* dataItem;
//  bool isDisconnected;
//  uint8_t inTryTime; // input
//} CommSerialDataRequestParamsTAG;


typedef struct _eventCommSerialSendRequestedTAG
{
  uint16_t clientId;
  BufferDataItem* dataItem;
} EventCommSerialSendRequestedTAG;

typedef struct _eventCommSerialDataReceivedTAG
{
  void* cbParams; // for callback case
  uint8_t commId;
  uint16_t clientId;
  BufferDataItem* dataItem;
#ifdef COMM_PROC_TIME_MEA_ENABLE
  CommProcMeaTimeTAG meaTime;
#endif // COMM_PROC_TIME_MEA_ENABLE

} EventCommSerialDataReceivedTAG;


typedef struct _eventCommSerialOpenConnectionTAG
{
  void* connectionParam;
  DataSize_t connectionParamSize;
} EventCommSerialOpenConnectionTAG;

typedef struct _eventCommSerialCloseConnectionTAG
{
  uint16_t clientId;
} EventCommSerialCloseConnectionTAG;

typedef struct _eventCommSerialConnectionClosedTAG
{
  void* cbParams; // for callback case
  uint8_t commId;
  uint16_t clientId;
  char host[COMM_HOST_MAX_LEN]; // for tcp
  uint16_t port; // tcp: port, i2c: address
} EventCommSerialConnectionClosedTAG;

typedef struct _eventCommSerialConnectionOpenedTAG
{
  void* cbParams; // for callback cases
  uint8_t commId;
  uint16_t clientId;
  char host[COMM_HOST_MAX_LEN]; // for tcp
  uint16_t port; // tcp: port, i2c: address
} EventCommSerialConnectionOpenedTAG;

typedef struct _eventCommSerialErrorOccurredTAG
{
  void* cbParams; // for callback case
  uint8_t commId;
  uint16_t clientId;
  char message[COMM_ERROR_MESSAGE_LEN];
} EventCommSerialErrorOccurredTAG;


/////////////////////////////////////////////////
// STATIC DATA

/////////////////////////////////////////////////
// STATIC FUNCTIONS

/////////////////////////////////////////////////
// EXTERN

/////////////////////////////////////////////////
// CLASS DEFINITION
class CommSerialConstant
{
public:
  enum SerialParityType
  {
    SERIAL_PARITY_NONE = 0,
    SERIAL_PARITY_ODD,
    SERIAL_PARITY_EVEN
  };

  static bool isValidSerialParityType(byte parityType)
  {
    switch (parityType)
    {
    case (byte)SerialParityType::SERIAL_PARITY_NONE:
    case (byte)SerialParityType::SERIAL_PARITY_ODD:
    case (byte)SerialParityType::SERIAL_PARITY_EVEN:
      return true;
    default:
      return false;
    }
  }

  static bool isValidSocketConfig(CommSerialSocketConfigTAG& commConfig)
  {
    do
    {
      if (commConfig.enabled == false)
      {
        break;
      }

      // bitPerByte 
      if ((commConfig.bitPerByte < COMM_SERIAL_BIT_PER_BYTE_MIN)
        || (commConfig.bitPerByte > COMM_SERIAL_BIT_PER_BYTE_MAX)
        )
      {
        break;
      }

      // stopBitNum 
      if ((commConfig.stopBitNum < COMM_SERIAL_STOP_BIT_NUM_MIN)
        || (commConfig.stopBitNum > COMM_SERIAL_STOP_BIT_NUM_MAX)
        )
      {
        break;
      }

      // parityType 
      if (CommSerialConstant::isValidSerialParityType(commConfig.parityType) == false)
      {
        break;
      }

      // validate baudrate
      if ((commConfig.baudrate < COMM_SERIAL_BAUDRATE_MIN)
        || (commConfig.baudrate > COMM_SERIAL_BAUDRATE_MAX)
        )
      {
        break;
      }

      // validate time out
      if ((commConfig.timeout < COMM_SERIAL_TIMEOUT_MIN)
        || (commConfig.timeout > COMM_SERIAL_TIMEOUT_MAX)
        )
      {
        break;
      }

#ifdef SYSTEM_ARDUINO_BASED
      // validate TX pin
      if (commConfig.pin_TX != COMM_SERIAL_PIN_INVALID)
      {
        if ((commConfig.pin_TX < COMM_SERIAL_PIN_MIN)
          || (commConfig.pin_TX > COMM_SERIAL_PIN_MAX)
          )
        {
          break;
        }
      }

      // validate RX pin
      if (commConfig.pin_RX != COMM_SERIAL_PIN_INVALID)
      {
        if ((commConfig.pin_RX < COMM_SERIAL_PIN_MIN)
          || (commConfig.pin_RX > COMM_SERIAL_PIN_MAX)
          )
        {
          break;
        }
      }

      // validate instance index
      if ((commConfig.instanceIndex < COMM_SERIAL_INSTANCE_IDX_MIN)
        || (commConfig.instanceIndex > COMM_SERIAL_INSTANCE_IDX_MAX)
        )
      {
        break;
      }

#else // SYSTEM_ARDUINO_BASED
      // SYSTEM_PC_BASED
      // validate port
      if (commConfig.port == NULL)
      {
        break;
      }
#endif // SYSTEM_ARDUINO_BASED

      return true;
    } while (0);

    return false;
  }
};
#endif // _CONF_COMM_SERIAL_CONSTANT_ENABLED

#endif // _COMM_SERIAL_CONSTANT_H

