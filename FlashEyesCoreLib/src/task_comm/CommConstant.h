#include "../FlashEyesCoreLibConfig.h"

#if (_CONF_COMM_CONSTANT_ENABLED)
#ifndef _COMM_CONSTANT_H
#define _COMM_CONSTANT_H

/////////////////////////////////////////////////
// INCLUDE

#include "../os_system/SystemCommon.h"
#include "../data_manager/DataItem.h"
/////////////////////////////////////////////////
// PREPROCESSOR

/////////////////////////////////////////////////
// DEFINE
#define COMM_HOST_MAX_LEN                       24
#define COMM_ERROR_MESSAGE_LEN                  16
#define COMM_PACKAGE_CHECK_SUM_LEN              2 // byte
#define COMM_CLIENT_ID_INVALID                  0


#define COMM_PACKAGE_OP_CODE_MASK               0b11110000
#define COMM_PACKAGE_OP_STATUS_MASK             0b00001110

#define COMM_PACKAGE_COMM_CODE_LEN              3
#define COMM_PACKAGE_COMM_CODE_TCP              "TCP"
#define COMM_PACKAGE_COMM_CODE_I2C              "I2C"
#define COMM_PACKAGE_COMM_CODE_COM              "COM" // serial
#define COMM_PACKAGE_COMM_CODE_BLE              "BLE"
#define COMM_PACKAGE_COMM_CODE_BLU              "BLU" // bluetooth


#define COMM_HEADERLESS_NAME_LEN                8

//#define COMM_PROC_TIME_MEA_ENABLE
#ifdef COMM_PROC_TIME_MEA_ENABLE
#define COMM_PROC_TIME_MEA_NUM                  5
#define COMM_PROC_TIME_MEA_REV_START            0
#define COMM_PROC_TIME_MEA_REV_HEADER           1
#define COMM_PROC_TIME_MEA_REV_BODY             2
#define COMM_PROC_TIME_MEA_REV_DECODED          3
#define COMM_PROC_TIME_MEA_SERIAL_REV           4
//#define COMM_PROC_TIME_MEA_MAIN_REV             5
#endif // COMM_PROC_TIME_MEA_ENABLE
/////////////////////////////////////////////////
// MARCO

/////////////////////////////////////////////////
// GLOBAL VARIABLES

/////////////////////////////////////////////////
// GLOBAL FUNCTIONS

/////////////////////////////////////////////////
// DATA TYPE (TYPEDEF)

/////////////////////////////////////////////////
// DATA TYPE (ENUM)

enum CommConnectionState
{
  CommConnectionStateInvalid = 0,
  CommConnected,
  CommDisconnecting,
  CommDisconnected
};

enum CommHeaderOpCode
{
  CommHeaderOpCodeInvalid = 0,
  CommHeaderOpDataRequest,
  CommHeaderOpData

  // max count 16
};

enum CommHeaderStatus
{
  CommHeaderStatusInvalid = 0,
  CommHeaderStatusOK,
  CommHeaderStatusErrUnknown,
  CommHeaderStatusErrTimeout,
  CommHeaderStatusErrBusy,
  CommHeaderStatusErrReceiving,
  //CommHeaderStatusRetry

  // max count: 8
};

typedef struct __ATTRIBUTE_ALIGN _commHeaderlesstDataTag
{
  byte count;
  byte sourceId;
  byte nameLen;
  byte reserved;
  char name[COMM_HEADERLESS_NAME_LEN];
} CommHeaderlesstDataTAG;

/////////////////////////////////////////////////
// DATA TYPE (STRUCT)
//
#ifdef COMM_PROC_TIME_MEA_ENABLE
typedef struct _commProcMeaTimeTAG
{
  unsigned long procTimes[COMM_PROC_TIME_MEA_NUM];
} CommProcMeaTimeTAG;

#endif // COMM_PROC_TIME_MEA_ENABLE


/////////////////////////////////////////////////
// STATIC DATA

/////////////////////////////////////////////////
// STATIC FUNCTIONS

/////////////////////////////////////////////////
// EXTERN

/////////////////////////////////////////////////
// CLASS DEFINITION
/*CommConstant*/
class CommConstant
{
public:
  enum CommType
  {
    CommTypeInvalid = 0
    , CommTypeI2C
    , CommTypeSerial
    , CommTypeSPI
    , CommTypeTCP
    , CommTypeBLE
  };

public:
  CommConstant(void) {};
  // WARNING: if inherite from this class, deconstructor must be virtual
  __ATTRIBUTE_VIRTUAL_OPTIMIZED ~CommConstant(void) {};

  static bool isValidCommClientInfoType(int commClientInfoType)
  {
    switch (commClientInfoType)
    {
    case CommConstant::CommType::CommTypeI2C:
    case CommConstant::CommType::CommTypeSerial:
    case CommConstant::CommType::CommTypeSPI:
    case CommConstant::CommType::CommTypeTCP:
    case CommConstant::CommType::CommTypeBLE:
      return true;
    default:
      return false;
    }
  }
};


#endif // _COMM_CONSTANT_H

#endif // _CONF_COMM_CONSTANT_ENABLED