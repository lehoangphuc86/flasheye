#include "../../FlashEyesCoreLibConfig.h"
#if (_CONF_PUB_SUB_CONSTANT_ENABLED)
#ifndef _PUB_SUB_CONSTANT_H
#define _PUB_SUB_CONSTANT_H

/////////////////////////////////////////////////
// INCLUDE
#include "../os_system/SystemCommon.h"
#include "../os_system/SystemCriticalSession.h"
/////////////////////////////////////////////////
// PREPROCESSOR

/////////////////////////////////////////////////
// DEFINE
// how many topic per publisher
#define PUB_SUB_TOPIC_COUNT_MIN                   1
#define PUB_SUB_TOPIC_COUNT_MAX                   16

// how many subscriber per topic
#define PUB_SUB_SUB_PER_TOPIC_COUNT_MIN           1
#define PUB_SUB_SUB_PER_TOPIC_COUNT_MAX           8

// how many subscription per subscriber
#define PUB_SUB_SUB_PER_SUBSCRIBER_COUNT_MIN      1
#define PUB_SUB_SUB_PER_SUBSCRIBER_COUNT_MAX      16

/////////////////////////////////////////////////
// MARCO

/////////////////////////////////////////////////
// GLOBAL VARIABLES

/////////////////////////////////////////////////
// GLOBAL FUNCTIONS

/////////////////////////////////////////////////
// DATA TYPE (TYPEDEF)
typedef uint8_t                                     SubscriptionId_t;
typedef uint8_t                                     TopicId_t;
typedef uint64_t                                    TopicIdSet_t;
/////////////////////////////////////////////////
// DATA TYPE (ENUM)

/////////////////////////////////////////////////
// DATA TYPE (STRUCT)
class PublisherInterface;
class SubscriberInterface;

typedef struct _topicSubcriptionInfoTag
{
  SubscriberInterface* subscriber;
} TopicSubcriptionInfoTAG;

typedef _topicSubcriptionInfoTag TopicSubcriptionConfigTAG;

typedef struct _topicInfoTag
{
  SubscriptionId_t subCount;
  SubscriptionId_t subMaxCount;
  TopicSubcriptionInfoTAG* subList;

  TopicSubcriptionInfoTAG* findSubscription(SubscriberInterface* subscriber)
  {
    do
    {
      if (this->subList == NULL)
      {
        break;
      }

      for (SubscriptionId_t wkidx = 0; wkidx < this->subMaxCount; wkidx++)
      {
        if (this->subList[wkidx].subscriber == subscriber)
        {
          return &this->subList[wkidx];
        }
      }

      return NULL;
    } while (0);
    return NULL;
  }
} TopicInfoTAG;

typedef struct _topicConfigTag
{
  SubscriptionId_t subMaxCount; // max sub per topic
} TopicConfigTAG;

typedef struct _publisherConfigTag
{
  TopicId_t topicMaxCount; // max topic count
  TopicConfigTAG* topicConfig; // length = topicCount
} PublisherConfigTAG;

typedef struct _publishParamsTag
{
  unsigned char* data;
  DataSize_t dataSize;
} PublishParamsTAG;

typedef struct _subscriberConfigTag
{
  SubscriptionId_t subMaxCount; // max sub per subscriber
} SubscriberConfigTAG;

typedef struct _subscriptionInfoTag
{
  PublisherInterface* publisher;
  TopicId_t topicId;
} SubscriptionInfoTAG;

typedef struct _subNotifiedInfoTag
{
  SubscriptionInfoTAG subscriptionInfo;
  PublishParamsTAG publishData;
} SubNotifiedInfoTAG;

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
#endif // _PUB_SUB_CONSTANT_H

#endif // _CONF_PUB_SUB_CONSTANT_ENABLED