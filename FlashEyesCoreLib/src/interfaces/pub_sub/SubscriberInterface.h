#include "PubSubConstant.h"

#if (_CONF_SUBSCRIBER_INTERFACE_ENABLED)

#ifndef _SUBSCRIBER_INTERFACE_H
#define _SUBSCRIBER_INTERFACE_H

/////////////////////////////////////////////////
// INCLUDE
//#include "PublisherInterface.h"
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
/*SubscriberInterface*/
class SubscriberInterface
{
public:
  SubscriberInterface(void);
  virtual ~SubscriberInterface(void);
public:
  virtual bool                                                  subIsValid(void);
  virtual int                                                   subStart(SubscriberConfigTAG& subscriberConfig);
  virtual int                                                   subSubscribeMulti(TopicIdSet_t topicIdSet, PublisherInterface* publisher, TopicIdSet_t& outErrorSet);
  virtual int                                                   subSubscribe(TopicId_t topicId, PublisherInterface* publisher);
  virtual int                                                   subUnSubscribeMulti(TopicIdSet_t topicIdSet, PublisherInterface* publisher);
  virtual int                                                   subUnSubscribe(TopicId_t topicId, PublisherInterface* publisher);
  virtual int                                                   subNotified(SubNotifiedInfoTAG& subNotifiedInfo);
  virtual void                                                  subStop(void);

private:
  SystemCriticalSession                                         sub_Critical_key;
  SubscriptionId_t                                              sub_Count;
  SubscriptionId_t                                              sub_Max_Count;
  SubscriptionInfoTAG*                                          sub_List;
};

#endif // _SUBSCRIBER_INTERFACE_H

#endif // _CONF_SUBSCRIBER_INTERFACE_ENABLED