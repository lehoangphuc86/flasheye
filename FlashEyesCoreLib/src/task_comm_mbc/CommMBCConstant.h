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



#define COMM_MBC_DATA_T_8                         0
#define COMM_MBC_DATA_T_16                        1
#define COMM_MBC_DATA_T_32                        2
#define COMM_MBC_DATA_T_64                        3
#define COMM_MBC_DATA_T_DOUBLE                    4
#define COMM_MBC_DATA_T_STRING                    5

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
  CommMBCPureData = 0,
  CommMbcJsonData,
  CommMbcDataTypeMax
} CommMBCDataType;
/////////////////////////////////////////////////
// DATA TYPE (STRUCT)

typedef struct __ATTRIBUTE_ALIGN _commMBCHeaderTag
{
  MbcMessageId_t messageId;
  uint8_t reserved;
  MbcDataSize_t dataLen;
} CommMBCHeaderTAG;

typedef struct _commMBCSettingParamTag
{
  bool enabled;
  byte processorType; // CommMBCProcessorType
  //uint8_t bufferSize;
  //uint8_t bufferCount;
  uint8_t reserved1;
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
  struct _bitSet1
  {
    bool isReply : 1;
    bool isUpdate : 1; // 0: get, 1: update
    byte dataType : 3; // 
    byte reserved : 3;
  } bitSet1;
  
  uint8_t errorCode; // isReply = 1
  DataSize_t sLen;
  union _data
  {
    double dVal;
    char* sVal;
  } data;
} CommMBCSystemSettingTAG;
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


//
typedef struct _commMbcPackageTAG
{
  _commMbcPackageTAG(void)
  {
    this->dataItem = NULL;
    this->dataManager = NULL;
  }

  ~_commMbcPackageTAG(void)
  {

  }

  bool isValid(void)
  {
    do
    {
      if ((this->dataItem == NULL)
        || (this->dataManager == NULL))
      {
        break;
      }

      return true;
    } while (0);
    return false;
  }

  void clear(void)
  {
    if (this->dataItem != NULL)
    {
      this->dataManager->release(this->dataItem);
    }
    this->dataItem = NULL;
    this->dataManager = NULL;
  }

  int setData(BufferDataManager* dataMgr)
  {
    do
    {
      this->clear();
      if (dataMgr == NULL)
      {
        break;
      }

      this->dataManager = dataMgr;
      this->dataItem = this->dataManager->get();
      if ((this->dataItem == NULL)
        || (this->dataItem->bufferLength() < sizeof(CommMBCHeaderTAG))
        )
      {
        break;
      }
      return this->dataItem->setDataLen(sizeof(CommMBCHeaderTAG));
    } while (0);
    this->clear();
    return -1;
  }

  CommMBCHeaderTAG* header(void)
  {
    return (CommMBCHeaderTAG*)this->dataItem->bufferAddress();
  }

  char* body(void)
  {
    return (char*)(this->dataItem->bufferAddress() + sizeof(CommMBCHeaderTAG));
  }

  DataSize_t bodyLen(void)
  {
    return this->dataItem->dataLength() - sizeof(CommMBCHeaderTAG);
  }

  int bodyLen(DataSize_t bodyLen)
  {
    return this->dataItem->setDataLen(bodyLen + sizeof(CommMBCHeaderTAG));
  }

  DataSize_t bodyMaxLen(void)
  {
    return this->dataItem->bufferLength() - sizeof(CommMBCHeaderTAG);
  }

  void releaseData(void)
  {
    this->clear();
  }

protected:
  BufferDataItem* dataItem;
  BufferDataManager* dataManager;
} CommMbcPackageTAG;
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
      return true;
    default:
      return false;
    }
  }
};

#endif // _COMM_MBC_CONSTANT_H

#endif // _CONF_COMM_MBC_CONSTANT_ENABLED