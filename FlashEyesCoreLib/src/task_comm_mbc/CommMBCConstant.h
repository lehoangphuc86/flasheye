#include "../FlashEyesCoreLibConfig.h"
#if (_CONF_COMM_MBC_CONSTANT_ENABLED)
#ifndef _COMM_MBC_CONSTANT_H
#define _COMM_MBC_CONSTANT_H

/////////////////////////////////////////////////
// INCLUDE
#include "../task_comm/CommConstant.h"
#include "../os_system/SystemCommon.h"
#include "../data_manager/BufferDataManager.h"
/////////////////////////////////////////////////
// PREPROCESSOR

/////////////////////////////////////////////////
// DEFINE
#define COMM_MBC_NAME_MAX_LEN                    32
#define COMM_MBC_STRING_TERMINATOR               '\0'
#define COMM_MBC_SYSTEM_SETTING_DATA_MAX_LEN     128
#define COMM_MBC_SCANNING_DEVICE_BARCODE_LEN_MAX 48

#define COMM_MBC_DATA_T_8                         SYS_DATA_T_8
#define COMM_MBC_DATA_T_DOUBLE                    SYS_DATA_T_DOUBLE
#define COMM_MBC_DATA_T_STRING                    SYS_DATA_T_STRING
#define COMM_MBC_DATA_T_16                        SYS_DATA_T_16
#define COMM_MBC_DATA_T_32                        SYS_DATA_T_32
#define COMM_MBC_DATA_T_64                        SYS_DATA_T_64


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
typedef enum _commMBCDataType
{
  CommMbcPureData = 0,
  CommMbcJsonData,
  CommMbcDataTypeMax
} CommMBCDataType;
/////////////////////////////////////////////////
// DATA TYPE (STRUCT)

typedef struct __ATTRIBUTE_ALIGN _commMBCHeaderTag
{
  MbcMessageId_t messageId;
  MbcDataSize_t dataLen;
} CommMBCHeaderTAG;

typedef struct _commMBCSettingParamTag
{
  bool enabled;
  byte processorType; // CommMbcDataType
  //uint8_t bufferSize;
  //uint8_t bufferCount;
  bool isHeaderLess : 1;
  uint8_t reserved1 : 7;
} CommMBCSettingParamTAG;


// Comm MBC message format
// Used by upper side formats
typedef struct _commMBCHeaderlessTag
{
  uint8_t nameLen;
  unsigned char* name; // COMM_MBC_NAME_MAX_LEN
} CommMBCHeaderlessTAG;

typedef struct _commMBCStart1Tag
{
  uint8_t param1;
  uint8_t param2;
  uint8_t nameLen;
  unsigned char* name; // COMM_MBC_NAME_MAX_LEN
} CommMBCStart1TAG;

typedef struct _commMBCResult1Tag
{
  uint8_t status;
  uint8_t nameLen;
  unsigned char* name; // COMM_MBC_NAME_MAX_LEN
} CommMBCResult1TAG;

typedef struct _commMBCSystemSettingTag
{
  uint8_t sectionId;
  uint8_t settingId;
  struct //_bitSet1
  {
    bool isReply : 1;
    bool isUpdate : 1; // 0: get, 1: update
    byte dataType : 3; // 
    bool isUpdateDB : 1; // 
    bool isRebootRequired : 1; // 
    byte reserved : 1;
  } bitSet1; // 0b reserved -> isReply ex:0b00001001
  
  uint8_t errorCode; // isReply = 1
  DataSize_t sLen;
  union _data
  {
    double dVal;
    char* sVal; // COMM_MBC_SYSTEM_SETTING_DATA_MAX_LEN
  } data;
} CommMBCSystemSettingTAG;

typedef struct _CommMBCScanningParamsTag
{
  uint16_t timeout; //ms
  bool enabled;
  byte trgSource;
  byte maxScanCount;
  byte reserved[1];
  uint16_t timeBtwScan;
} CommMBCScanningParamsTAG;

typedef struct _commMBCScanningControlTag
{
  Seq_t sequenceId;
  struct //_bitSet1
  {
    bool isStart : 1;
    bool isReply : 1;
    byte reserved : 6;
  } bitSet1; // 0b reserved -> isStart ex:0b00001001
  uint8_t errorCode; // isReply = 1
  byte reserved[2];
  CommMBCScanningParamsTAG trgParams; // for start
} CommMBCScanningControlTAG;


typedef struct _commMBCScanningBarCodeTag
{
  DataSize_t codeLen;
  byte type;
  char* code;// [COMM_MBC_SCANNING_DEVICE_BARCODE_LEN_MAX + 1] ;
} CommMBCScanningBarCodeTAG;

typedef struct _commMBCScanningDeviceResultTag
{
  struct //_bitSet1
  {
    bool enabled : 1;
    byte reserved : 7;
  } bitSet1; // 0b reserved -> enabled
  byte errorId;
  CommMBCScanningBarCodeTAG code;
} CommMBCScanningDeviceResultTAG;

typedef struct _commMBCscanningResultTag
{
  Seq_t sequenceId;
  struct //_bitSet1
  {
    bool enabled : 1;
    bool isReply : 1;
    byte reserved : 6;
  } bitSet1; // 0b reserved -> enabled ex:0b00001001
  byte scanIndex;
  uint8_t errorCode; // isReply = 1
  byte reserved[1];
  CommMBCScanningDeviceResultTAG deviceResult;
} CommMBCScanningResultTAG;

// Used by comm layer formats (obsolete)
// MBC processor should use dynamic encode/decode instead of this.
typedef struct __ATTRIBUTE_ALIGN _commMBCStart1PlainTag
{
  uint8_t param1;
  uint8_t param2;
  uint8_t nameLen;
  unsigned char name[COMM_MBC_NAME_MAX_LEN];
} CommMBCStart1PlainTAG;

typedef struct __ATTRIBUTE_ALIGN _commMBCResult1PlainTag
{
  uint8_t status;
  uint8_t nameLen;
  unsigned char name[COMM_MBC_NAME_MAX_LEN];
} CommMBCResult1PlainTAG;

/////////////////////////////////////////////////
// DATA TYPE (STRUCT)
// events

/////////////////////////////////////////////////
// STATIC DATA

/////////////////////////////////////////////////
// STATIC FUNCTIONS

/////////////////////////////////////////////////
// EXTERN

/////////////////////////////////////////////////
// CLASS DEFINITION
/*CommMBCConstant*/
class CommMBCConstant
{
public:
  enum CommMBCMessageId
  {
    CommMBCMessageIdInvalid = 0,
    CommMBCHeadLess,
    CommMBCStart1,
    CommMBCResult1,
    CommMBCSystemSetting,
    CommMBCScanningControl,
    CommMBCScanningResult
  };

public:
  CommMBCConstant(void) {};
  // WARNING: if inherite from this class, deconstructor must be virtual
  __ATTRIBUTE_VIRTUAL_OPTIMIZED ~CommMBCConstant(void) {};

  static bool isValidMessageId(int mbcMessageId)
  {
    switch (mbcMessageId)
    {
    case CommMBCConstant::CommMBCMessageId::CommMBCHeadLess:
    case CommMBCConstant::CommMBCMessageId::CommMBCStart1:
    case CommMBCConstant::CommMBCMessageId::CommMBCResult1:
    case CommMBCConstant::CommMBCMessageId::CommMBCSystemSetting:
    case CommMBCConstant::CommMBCMessageId::CommMBCScanningControl:
    case CommMBCConstant::CommMBCMessageId::CommMBCScanningResult:
      return true;
    default:
      return false;
    }
  }
};

#endif // _COMM_MBC_CONSTANT_H

#endif // _CONF_COMM_MBC_CONSTANT_ENABLED