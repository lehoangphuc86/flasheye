#include "../FlashEyesCoreLibConfig.h"
#if (_CONF_MESS_BROKER_CONSTANT_ENABLED)
#ifndef _MESS_BROKER_CONSTANT_H
#define _MESS_BROKER_CONSTANT_H

/////////////////////////////////////////////////
// INCLUDE
#include "../os_system/SystemCommon.h"
#include "../os_system/SystemCriticalSession.h"
#include "../timer_manager/TimerManagerConstant.h"
#include "../os_system/SystemMutex.h"
/////////////////////////////////////////////////
// PREPROCESSOR

/////////////////////////////////////////////////
// DEFINE

#define MB_MESS_COUNT_MIN                             1
#define MB_MESS_COUNT_MAX                             16

#define MB_MESS_ID_MIN                                0
#define MB_MESS_ID_MAX                                16
#define MB_MESS_ID_INVALID                            (MB_MESS_ID_MAX+1)

#define MB_MATCH_PATTERN_ALL                          UINT16_MAX
#define MB_MATCH_PATTERN_NONE                         0

#define MB_SUB_PER_MESS_COUNT_MIN                     1
#define MB_SUB_PER_MESS_COUNT_MAX                     16

#define MB_MEMBER_ID_INVALID                          0

#define MB_PUBLISH_RES_WAIT_TIME_MIN                  300 // ms
#define MB_PUBLISH_RES_WAIT_TIME_COFF                 100 // ms
/////////////////////////////////////////////////
// MARCO

/////////////////////////////////////////////////
// GLOBAL VARIABLES

/////////////////////////////////////////////////
// GLOBAL FUNCTIONS

/////////////////////////////////////////////////
// DATA TYPE (TYPEDEF)
struct _MBNotifiedInfoTag;
typedef void(*MessBrokerCbOnNotified)(_MBNotifiedInfoTag& params);
typedef uint8_t                                     MBMessId_t;
typedef uint8_t                                     MBMemberId_t;
typedef uint16_t                                    MBPattern_t;
/////////////////////////////////////////////////
// DATA TYPE (ENUM)

/////////////////////////////////////////////////
// DATA TYPE (STRUCT)

typedef struct _MBSubscriberInfoTag
{
  MBMemberId_t subId; // used to identify subscriber when subscribing and unsubscribing
  void* arg;
  MessBrokerCbOnNotified cb;
  MBPattern_t pattern;

  bool isValid(void)
  {
    return (this->subId == MB_MEMBER_ID_INVALID);
  }
} MBSubscriberInfoTAG;

typedef _MBSubscriberInfoTag MBSubscriberConfigTAG;

typedef struct _MBMessConfigTag
{
  MBMessId_t mbMessId; // internal use
  MBMemberId_t subMaxCount; // max sub per message
} MBMessConfigTAG;

typedef struct _MBConfigTag
{
  MBMessId_t messMaxCount; // max message count
  MBMessConfigTAG* messConfig; // length = messMaxCount
} MBConfigTAG;


typedef struct _MBPublishParamsTag
{
  MBPattern_t pattern;
  MBMemberId_t pubId;
  unsigned char* data;
  DataSize_t dataSize;
} MBPublishParamsTAG;

/*
* @expiredTime: the subcribers have to take and respond the message before this time end
*                  if it is timed out, accessing the data would be come undefined.
*                  if 0, no timeout is set.
* @mutex: the subcribers need to call mutex.unlock() when finishs
*/
typedef struct _MBPublishResSpecTag
{
  TimePoint_t expiredTime;
  SystemMutex* mutex;
} MBPublishResSpecTAG;

typedef struct _MBNotifiedInfoTag
{
  MBMessId_t mbMessId;
  MBPublishParamsTAG publishData;
  MBPublishResSpecTAG resSpec;
} MBNotifiedInfoTAG;
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
#endif // _MESS_BROKER_CONSTANT_H

#endif // _CONF_MESS_BROKER_CONSTANT_ENABLED