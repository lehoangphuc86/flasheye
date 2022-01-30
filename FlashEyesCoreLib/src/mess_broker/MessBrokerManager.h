#include "MessBrokerConstant.h"

#if (_CONF_MESS_BROKER_MANAGER_ENABLED)

#ifndef _MESS_BROKER_H
#define _MESS_BROKER_H

/////////////////////////////////////////////////
// INCLUDE
#include "MessBrokerInfo.h"
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
/*MessBrokerManager*/
class MessBrokerManager
{
public:
  static MessBrokerManager& getInstance(void);
  MessBrokerManager(MessBrokerManager const&) = delete;
  void operator=(MessBrokerManager const&) = delete;
private:
  MessBrokerManager(void);
  virtual ~MessBrokerManager(void);
public:
  bool                                                          isValid(void);
  int                                                           start(MBConfigTAG& messBrokerConfig);
  int                                                           subscribe(MBMessId_t mbMessId, MBSubscriberConfigTAG& subConfig);
  void                                                          unsubscribe(MBMessId_t mbMessId, MBMemberId_t subId);
  /*
  * @resTimeout: set 0 to return as soon as subcribers notified
  *              set x ms to wait subcribers x ms for beinging notified and responding
  */
  int                                                           publish(MBMessId_t mbMessId, MBPublishParamsTAG& publishParams, TimePoint_t resTimeoutMs = 0);
  void                                                          stop(void);

private:
  SystemCriticalSession                                         critical_Key;
  MBMessId_t                                                    mess_Count;
  MBMessId_t                                                    mess_Max_Count;
  MessBrokerInfo*                                               mess_List;
};

#endif // _MESS_BROKER_H

#endif // _CONF_MESS_BROKER_MANAGER_ENABLED