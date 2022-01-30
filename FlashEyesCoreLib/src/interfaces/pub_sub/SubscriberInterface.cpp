/////////////////////////////////////////////////
// INCLUDE
#include "SubscriberInterface.h"
#if (_CONF_SUBSCRIBER_INTERFACE_ENABLED)
#include "PublisherInterface.h"
/////////////////////////////////////////////////
// PREPROCESSOR

/////////////////////////////////////////////////
// DEFINE
#define SUBSCRIBER_IF_CONSOLE_DEBUG_ENABLE

/////////////////////////////////////////////////
// MARCO

/////////////////////////////////////////////////
// GLOBAL VARIABLES

/////////////////////////////////////////////////
// GLOBAL FUNCTIONS

/////////////////////////////////////////////////
// DATA TYPE (ENUM)

/////////////////////////////////////////////////
// DATA TYPE (STRUCT)

/////////////////////////////////////////////////
// STATIC DATA
#ifdef SUBSCRIBER_IF_CONSOLE_DEBUG_ENABLE
char subIFLogBuf[SYSTEM_CONSOLE_OUT_BUF_LEN];
#endif // SUBSCRIBER_IF_CONSOLE_DEBUG_ENABLE
/////////////////////////////////////////////////
// STATIC FUNCTIONS

/////////////////////////////////////////////////
// EXTERN

/////////////////////////////////////////////////
// CLASS IMPLEMENTAION
//
/*SubscriberInterface*/
SubscriberInterface::SubscriberInterface(void)
  : sub_Count(0)
  , sub_Max_Count(0)
  , sub_List(NULL)
{

}

SubscriberInterface::~SubscriberInterface(void)
{
  this->subStop();
}

bool SubscriberInterface::subIsValid(void)
{
  SystemCriticalLocker locker(this->sub_Critical_key);
  return (this->sub_List == NULL ? false : true);
}

int SubscriberInterface::subStart(SubscriberConfigTAG& subscriberConfig)
{
  do
  {
    if (this->subIsValid() != false)
    {
      return -1; // already
    }

    this->subStop();
    {
      SystemCriticalLocker locker(this->sub_Critical_key);
      if ((subscriberConfig.subMaxCount < PUB_SUB_SUB_PER_SUBSCRIBER_COUNT_MIN)
        || (subscriberConfig.subMaxCount > PUB_SUB_SUB_PER_SUBSCRIBER_COUNT_MAX)
        )
      {
        break;
      }

      this->sub_Count = 0;
      this->sub_Max_Count = subscriberConfig.subMaxCount;
      this->sub_List = new SubscriptionInfoTAG[this->sub_Max_Count + 1];
      if (this->sub_List == NULL)
      {
        break;
      }
      memset(this->sub_List, 0, sizeof(SubscriptionInfoTAG) * (this->sub_Max_Count + 1));
    }

    return 0;
  } while (0);
  this->subStop();
  return -1;
}

int SubscriberInterface::subSubscribeMulti(TopicIdSet_t topicIdSet, PublisherInterface* publisher, TopicIdSet_t& outErrorSet)
{
  int ret = 0;
  outErrorSet = 0;
  do
  {
    if (this->subIsValid() == false)
    {
      break;
    }
    TopicIdSet_t wkIdx = 0;
    for (wkIdx = 0; wkIdx < PUB_SUB_SUB_PER_SUBSCRIBER_COUNT_MAX; wkIdx++)
    {
      if (SYSTEM_BITREAD(topicIdSet, wkIdx) == 0)
      {
        continue;
      }

      ret = this->pubAppend(wkIdx, subcriber);
      if (ret != 0)
      {
        SYSTEM_BITSET(outErrorSet, wkIdx);
        break;
      }
    }

    if (wkIdx < PUB_SUB_TOPIC_COUNT_MAX)
    {
      break;
    }

    return 0;
  } while (0);
  return -1;
}

int SubscriberInterface::pubAppend(TopicId_t topicId, SubscriberInterface* subcriber)
{
  int ret = 0;
  do
  {
    if (this->pubIsValid() == false)
    {
      break;
    }

    {
      SystemCriticalLocker locker(this->pub_Critical_key);
      if (this->sub_Count >= this->sub_Max_Count)
      {
        break;
      }

      if (topicId >= this->sub_Count)
      {
        break;
      }

      TopicInfoTAG& topicInfo = this->sub_List[topicId];

      TopicSubcriptionInfoTAG* foundSub = topicInfo.findSubscription(subcriber);
      if (foundSub != NULL)
      {
        return 0; //already
      }

      foundSub = topicInfo.findSubscription(NULL); // find empty slot
      if (foundSub == NULL)
      {
        break; //something wrong
      }

      foundSub->subscriber = subcriber;
      topicInfo.subCount++;
    }

    return 0;
  } while (0);
  return -1;
}

void SubscriberInterface::pubRemoveMulti(TopicIdSet_t topicIdSet, SubscriberInterface* subcriber)
{
  int ret = 0;
  do
  {
    if (this->pubIsValid() == false)
    {
      break;
    }
    TopicIdSet_t wkIdx = 0;
    for (wkIdx = 0; wkIdx < PUB_SUB_TOPIC_COUNT_MAX; wkIdx++)
    {
      if (SYSTEM_BITREAD(topicIdSet, wkIdx) == 0)
      {
        continue;
      }

      this->pubRemove(wkIdx, subcriber);
    }
    return;
  } while (0);
  return;
}

void SubscriberInterface::pubRemove(TopicId_t topicId, SubscriberInterface* subcriber)
{
  int ret = 0;
  do
  {
    if (this->pubIsValid() == false)
    {
      break;
    }

    {
      SystemCriticalLocker locker(this->pub_Critical_key);
      if (this->sub_Count <= 0)
      {
        break;
      }

      if (topicId >= this->sub_Count)
      {
        break;
      }

      TopicInfoTAG& topicInfo = this->sub_List[topicId];

      TopicSubcriptionInfoTAG* foundSub = topicInfo.findSubscription(subcriber);
      if (foundSub == NULL)
      {
        return; // not found
      }

      foundSub->subscriber = NULL;
      topicInfo.subCount--;
    }

    return;
  } while (0);
  return;
}

int SubscriberInterface::publishMulti(TopicIdSet_t topicIdSet, PublishParamsTAG& publishParams, TopicIdSet_t& outErrorSet)
{
  int ret = 0;
  outErrorSet = 0;
  do
  {
    if (this->pubIsValid() == false)
    {
      break;
    }
    TopicIdSet_t wkIdx = 0;
    for (wkIdx = 0; wkIdx < PUB_SUB_TOPIC_COUNT_MAX; wkIdx++)
    {
      if (SYSTEM_BITREAD(topicIdSet, wkIdx) == 0)
      {
        continue;
      }

      ret = this->publish(wkIdx, publishParams);
      if (ret != 0)
      {
        SYSTEM_BITSET(outErrorSet, wkIdx);
        break;
      }
    }

    if (wkIdx < PUB_SUB_TOPIC_COUNT_MAX)
    {
      break;
    }

    return 0;
  } while (0);
  return -1;
}

int SubscriberInterface::publish(TopicId_t topicId, PublishParamsTAG& publishParams)
{
  int ret = 0;
  do
  {
    if (this->pubIsValid() == false)
    {
      break;
    }

    {
      SystemCriticalLocker locker(this->pub_Critical_key);
      if (this->sub_Count <= 0)
      {
        break;
      }

      if (topicId >= this->sub_Count)
      {
        break;
      }

      TopicInfoTAG& topicInfo = this->sub_List[topicId];
      SubscriptionId_t wkidx = 0;
      SubNotifiedInfoTAG subNotifiedInfo = SubNotifiedInfoTAG();
      subNotifiedInfo.subscriptionInfo.publisher = this;
      subNotifiedInfo.subscriptionInfo.topicId = topicId;
      subNotifiedInfo.publishData.data = publishParams.data;
      subNotifiedInfo.publishData.dataSize = publishParams.dataSize;
      for (wkidx = 0; wkidx < topicInfo.subCount; wkidx++)
      {
        topicInfo.subList[wkidx].subscriber->subNotified(subNotifiedInfo);
      }
    }

    return 0;
  } while (0);
  return -1;
}

void SubscriberInterface::pubStop(void)
{
  do
  {
    SystemCriticalLocker locker(this->pub_Critical_key);
    if (this->sub_List == NULL)
    {
      this->sub_Count = 0;
      this->sub_Max_Count = 0;
      return;
    }

    for (TopicId_t wkIdx = 0; wkIdx < this->sub_Max_Count; wkIdx++)
    {
      delete[] this->sub_List[wkIdx].subList;
      this->sub_List[wkIdx].subList = NULL;
    }

    delete[] this->sub_List;
    this->sub_List = NULL;
    this->sub_Count = 0;
    this->sub_Max_Count = 0;
    return;
  } while (0);
  return;
}

#endif // _CONF_SUBSCRIBER_INTERFACE_ENABLED