#include "MessBrokerConstant.h"

#if (_CONF_MESS_BROKER_INFO_ENABLED)

#ifndef _MESS_BROKER_INFO_H
#define _MESS_BROKER_INFO_H

/////////////////////////////////////////////////
// INCLUDE

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
/*MessBrokerInfo*/
class MessBrokerInfo
{
public:
  MessBrokerInfo(void);
  virtual ~MessBrokerInfo(void);
public:
  bool                                                          isValid(void);
  int                                                           start(MBMessConfigTAG& messConfig);
  int                                                           subscribe(MBSubscriberConfigTAG& subConfig);
  void                                                          unsubscribe(MBMemberId_t subId);
  int                                                           publish(MBPublishParamsTAG& publishParams, TimePoint_t resTimeoutMs = 0);
  void                                                          stop(void);
protected:
  MBSubscriberInfoTAG*                                          findSub(MBMemberId_t subId);
private:
  SystemCriticalSession                                         critical_key;
  SystemMutex                                                   res_Wait_Key;
  MBMemberId_t                                                  sub_Count;
  MBMemberId_t                                                  sub_Max_Count;
  MBSubscriberInfoTAG*                                          sub_List;
  MBMessId_t                                                    mess_Id;
};

#endif // _MESS_BROKER_INFO_H

#endif // _CONF_MESS_BROKER_INFO_ENABLED