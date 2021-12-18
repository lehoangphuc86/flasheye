#include "../FlashEyesCoreLibConfig.h"
#if (_CONF_COMM_MBC_CONSTANT_ENABLED)
#ifndef _COMM_MBC_CONSTANT_H
#define _COMM_MBC_CONSTANT_H

/////////////////////////////////////////////////
// INCLUDE
#include "../task_comm/CommConstant.h"
#include "../os_system/SystemCommon.h"
/////////////////////////////////////////////////
// PREPROCESSOR

/////////////////////////////////////////////////
// DEFINE
#define COMM_MBC_NAME_MAX_LEN                    32
#define COMM_MBC_STRING_TERMINATOR               '\0'

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

/////////////////////////////////////////////////
// DATA TYPE (STRUCT)

typedef struct __ATTRIBUTE_ALIGN _commMBCHeaderTag
{
  uint8_t messageId;
  uint8_t reserved1;
  uint16_t dataLen;
} CommMBCHeaderTAG;

typedef struct _commMBCSettingParamTag
{
  bool enabled;
  //uint8_t bufferSize;
  //uint8_t bufferCount;
  uint8_t reserved1;
} CommMBCSettingParamTAG;

typedef struct _commMBCStart1Tag
{
  uint8_t param1;
  uint8_t param2;
  uint8_t nameLen;
  unsigned char* name;
} CommMBCStart1TAG;

typedef struct _commMBCHeaderlessTag
{
  uint8_t nameLen;
  unsigned char* name;
} CommMBCHeaderlessTAG;

typedef struct _commMBCResult1Tag
{
  uint8_t status;
  uint8_t nameLen;
  unsigned char* name;
} CommMBCResult1TAG;


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
    CommMBCResult1
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
      return true;
    default:
      return false;
    }
  }
};

#endif // _COMM_MBC_CONSTANT_H

#endif // _CONF_COMM_MBC_CONSTANT_ENABLED