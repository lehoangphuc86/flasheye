#include "EventManagerConstant.h"

#if (_CONF_EVENT_MANAGER_ENABLED)

#ifndef _EVENT_MANAGER_H
#define _EVENT_MANAGER_H

/////////////////////////////////////////////////
// INCLUDE
#include "../data_manager/DataManager.h"
#include "EventDataItem.h"
#include "../os_system/SystemMutex.h"
#include "EventPoolManager.h"
#if (!_CONF_DATA_MANAGER_ENABLED)
#error Data manager is required
#endif // _CONF_DATA_MANAGER_ENABLED

#if (!_CONF_EVENT_DATA_ITEM_ENABLED)
#error Data event item is required
#endif // _CONF_EVENT_DATA_ITEM_ENABLED

#if (!_CONF_SYSTEM_MUTEX_ENABLED)
#error System mutex is required
#endif // _CONF_SYSTEM_MUTEX_ENABLED

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
typedef DataManagerConfigTAG<EventDataItemConfigTAG> EventManagerConfigTAG;
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

/*EventManager*/
class EventManager
#ifdef SYSTEM_QUEUE_DUMP_ENABLE
  : public DataManager
#else //SYSTEM_QUEUE_DUMP_ENABLE
  : protected DataManager
#endif // SYSTEM_QUEUE_DUMP_ENABLE
{
public:
  EventManager(PoolId_t poolId = DATA_POOL_ID_INVALID);
  // WARNING: if inherite from this class, deconstructor must be virtual
  __ATTRIBUTE_VIRTUAL_OPTIMIZED ~EventManager(void);
  int                                                           prepare(EventManagerConfigTAG& poolConfig);
  int                                                           prepare(EventCount_t eventItemNumber, EventSize_t eventItemSize, bool usePool = false);
  void                                                          clear(void) override;

  ////////////////////Function to call from outside ISR///////////////////////
  bool                                                          isValid(void) override;
  bool                                                          isEmpty(void);
  bool                                                          isUsingQueueEmpty(void);
  bool                                                          isReadyQueueEmpty(void);
  DataCount_t                                                   readyQueueSize(void);
  EventDataItem*                                                get(void);
  EventDataItem*                                                wait(unsigned int timeToWait = SYSTEM_MUTEX_MAX_WAIT_TIME); // ms
  EventDataItem*                                                waitAccurate(unsigned long timeToWait = SYSTEM_MUTEX_MAX_WAIT_TIME_ACCURATE); // ns
  int                                                           notify(EventId_t messageId, EventSize_t eventSize, unsigned char* eventData);
  int                                                           release(EventDataItem* eventItem);
  ////////////////////Function to call from inside ISR///////////////////////
  bool                                                          isValidFromISR(void) override;
  bool                                                          isEmptyFromISR(void);
  bool                                                          isUsingQueueEmptyFromISR(void);
  bool                                                          isReadyQueueEmptyFromISR(void);
  DataCount_t                                                   readyQueueSizeFromISR(void);
  EventDataItem*                                                getFromISR(void);
  EventDataItem*                                                waitFromISR(bool* higherTaskWoken); // ms
  int                                                           notifyFromISR(EventId_t messageId, EventSize_t eventSize, unsigned char* eventData, bool *wakeUpTask);
  int                                                           releaseFromISR(EventDataItem* eventItem);

protected:
  SystemMutex                                                   event_Lock_Key;
};
#endif // _EVENT_MANAGER_H

#endif // _CONF_EVENT_MANAGER_ENABLED