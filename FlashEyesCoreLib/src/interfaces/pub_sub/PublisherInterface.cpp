/////////////////////////////////////////////////
// INCLUDE
#include "PublisherInterface.h"
#if (_CONF_PUBLISHSER_INTERFACE_ENABLED)
#include "SubscriberInterface.h"
/////////////////////////////////////////////////
// PREPROCESSOR

/////////////////////////////////////////////////
// DEFINE
#define PUBLISHER_IF_CONSOLE_DEBUG_ENABLE

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
#ifdef PUBLISHER_IF_CONSOLE_DEBUG_ENABLE
char pubIFLogBuf[SYSTEM_CONSOLE_OUT_BUF_LEN];
#endif // PUBLISHER_IF_CONSOLE_DEBUG_ENABLE
/////////////////////////////////////////////////
// STATIC FUNCTIONS

/////////////////////////////////////////////////
// EXTERN

/////////////////////////////////////////////////
// CLASS IMPLEMENTAION
//
/*PublisherInterface*/
PublisherInterface::PublisherInterface(void)
  : pub_Topic_Count(0)
  , pub_Topic_Max_Count(0)
  , pub_Topic_List(NULL)
{

}

PublisherInterface::~PublisherInterface(void)
{
  this->pubStop();
}

bool PublisherInterface::pubIsValid(void)
{
  SystemCriticalLocker locker(this->pub_Critical_key);
  return (this->pub_Topic_List == NULL ? false: true);
}

int PublisherInterface::pubStart(PublisherConfigTAG& publisherConfig)
{
  do
  {
    if (this->pubIsValid() != false)
    {
      return -1; // already
    }

    this->pubStop();
    {
      SystemCriticalLocker locker(this->pub_Critical_key);
      if ((publisherConfig.topicMaxCount < PUB_SUB_TOPIC_COUNT_MIN)
        || (publisherConfig.topicMaxCount > PUB_SUB_TOPIC_COUNT_MAX)
        || (publisherConfig.topicConfig == NULL)
        )
      {
        break;
      }
      TopicId_t wkIdx = 0;
      for (wkIdx = 0; wkIdx < publisherConfig.topicMaxCount; wkIdx++)
      {
        if ((publisherConfig.topicConfig[wkIdx].subMaxCount < PUB_SUB_SUB_PER_TOPIC_COUNT_MIN)
          || (publisherConfig.topicConfig[wkIdx].subMaxCount > PUB_SUB_SUB_PER_TOPIC_COUNT_MAX)
          )
        {
          break;
        }
      }
      this->pub_Topic_Count = 0;
      this->pub_Topic_Max_Count = publisherConfig.topicMaxCount;
      this->pub_Topic_List = new TopicInfoTAG[this->pub_Topic_Max_Count + 1];
      if (this->pub_Topic_List == NULL)
      {
        break;
      }

      wkIdx = 0;
      for (wkIdx = 0; wkIdx < this->pub_Topic_Max_Count; wkIdx++)
      {
        this->pub_Topic_List[wkIdx].subCount = 0;
        this->pub_Topic_List[wkIdx].subMaxCount = publisherConfig.topicConfig[wkIdx].subMaxCount;
        this->pub_Topic_List[wkIdx].subList = new TopicSubcriptionInfoTAG[this->pub_Topic_List[wkIdx].subMaxCount + 1];
        if (this->pub_Topic_List[wkIdx].subList == NULL)
        {
          break;
        }
        memset(this->pub_Topic_List[wkIdx].subList, 0, (this->pub_Topic_List[wkIdx].subMaxCount + 1)*sizeof(TopicSubcriptionInfoTAG));
      }
      if (wkIdx < this->pub_Topic_Max_Count)
      {
        break;
      }
    }
    
    return 0;
  } while (0);
  this->pubStop();
  return -1;
}

int PublisherInterface::pubAppendMulti(TopicIdSet_t topicIdSet, SubscriberInterface* subcriber, TopicIdSet_t& outErrorSet)
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

int PublisherInterface::pubAppend(TopicId_t topicId, SubscriberInterface* subcriber)
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
      if (this->pub_Topic_Count >= this->pub_Topic_Max_Count)
      {
        break;
      }

      if (topicId >= this->pub_Topic_Count)
      {
        break;
      }

      TopicInfoTAG& topicInfo = this->pub_Topic_List[topicId];

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

void PublisherInterface::pubRemoveMulti(TopicIdSet_t topicIdSet, SubscriberInterface* subcriber)
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

void PublisherInterface::pubRemove(TopicId_t topicId, SubscriberInterface* subcriber)
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
      if (this->pub_Topic_Count <= 0)
      {
        break;
      }

      if (topicId >= this->pub_Topic_Count)
      {
        break;
      }

      TopicInfoTAG& topicInfo = this->pub_Topic_List[topicId];

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

int PublisherInterface::publishMulti(TopicIdSet_t topicIdSet, PublishParamsTAG& publishParams, TopicIdSet_t& outErrorSet)
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

int PublisherInterface::publish(TopicId_t topicId, PublishParamsTAG& publishParams)
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
      if (this->pub_Topic_Count <= 0)
      {
        break;
      }

      if (topicId >= this->pub_Topic_Count)
      {
        break;
      }

      TopicInfoTAG& topicInfo = this->pub_Topic_List[topicId];
      SubscriptionId_t wkidx = 0;
      SubNotifiedInfoTAG subNotifiedInfo = SubNotifiedInfoTAG();
      subNotifiedInfo.subscriptionInfo.publisher = this;
      subNotifiedInfo.subscriptionInfo.topicId = topicId;
      subNotifiedInfo.publishData.data = publishParams.data;
      subNotifiedInfo.publishData.dataSize = publishParams.dataSize;
      for (wkidx = 0; wkidx< topicInfo.subCount; wkidx++)
      {
        topicInfo.subList[wkidx].subscriber->subNotified(subNotifiedInfo);
      }
    }

    return 0;
  } while (0);
  return -1;
}

void PublisherInterface::pubStop(void)
{
  do
  {
    SystemCriticalLocker locker(this->pub_Critical_key);
    if (this->pub_Topic_List == NULL)
    {
      this->pub_Topic_Count = 0;
      this->pub_Topic_Max_Count = 0;
      return;
    }
    
    for (TopicId_t wkIdx = 0; wkIdx < this->pub_Topic_Max_Count; wkIdx++)
    {
      delete[] this->pub_Topic_List[wkIdx].subList;
      this->pub_Topic_List[wkIdx].subList = NULL;
    }
    
    delete[] this->pub_Topic_List;
    this->pub_Topic_List = NULL;
    this->pub_Topic_Count = 0;
    this->pub_Topic_Max_Count = 0;
    return;
  } while (0);
  return;
}

#endif // _CONF_PUBLISHSER_INTERFACE_ENABLED