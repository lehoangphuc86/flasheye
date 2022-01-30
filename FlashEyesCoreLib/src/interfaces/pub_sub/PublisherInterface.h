#include "PubSubConstant.h"

#if (_CONF_PUBLISHSER_INTERFACE_ENABLED)

#ifndef _PUBLISHSER_INTERFACE_H
#define _PUBLISHSER_INTERFACE_H

/////////////////////////////////////////////////
// INCLUDE
//#include "SubscriberInterface.h"
/////////////////////////////////////////////////
// PREPROCESSOR

/////////////////////////////////////////////////
// DEFINE

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

/////////////////////////////////////////////////
// STATIC DATA

/////////////////////////////////////////////////
// STATIC FUNCTIONS

/////////////////////////////////////////////////
// EXTERN

/////////////////////////////////////////////////
// CLASS DEFINITION
/*PublisherInterface*/
class PublisherInterface
{
public:
  PublisherInterface(void);
  virtual ~PublisherInterface(void);
public:
  virtual bool                                                  pubIsValid(void);
  virtual int                                                   pubStart(PublisherConfigTAG& publisherConfig);
  virtual int                                                   pubAppendMulti(TopicIdSet_t topicIdSet, SubscriberInterface* subcriber, TopicIdSet_t& outErrorSet);
  virtual int                                                   pubAppend(TopicId_t topicId, SubscriberInterface* subcriber);
  virtual void                                                  pubRemoveMulti(TopicIdSet_t topicIdSet, SubscriberInterface* subcriber);
  virtual void                                                  pubRemove(TopicId_t topicId, SubscriberInterface* subcriber);
  virtual int                                                   publishMulti(TopicIdSet_t topicIdSet, PublishParamsTAG& publishParams, TopicIdSet_t& outErrorSet);
  virtual int                                                   publish(TopicId_t topicId, PublishParamsTAG& publishParams);
  virtual void                                                  pubStop(void);
private:
  //TopicSubcriptionInfoTAG*                                      pubFindSubscription(SubscriberInterface* subcriber, TopicId_t topicId);
private:
  SystemCriticalSession                                         pub_Critical_key;
  TopicId_t                                                     pub_Topic_Count;
  TopicId_t                                                     pub_Topic_Max_Count;
  TopicInfoTAG*                                                 pub_Topic_List;
};

#endif // _PUBLISHSER_INTERFACE_H

#endif // _CONF_PUBLISHSER_INTERFACE_ENABLED