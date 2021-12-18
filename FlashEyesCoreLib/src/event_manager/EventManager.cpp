/////////////////////////////////////////////////
// INCLUDE
#include "EventManager.h"

#if (_CONF_EVENT_MANAGER_ENABLED)

#include "EventPoolManager.h"
/////////////////////////////////////////////////
// PREPROCESSOR

/////////////////////////////////////////////////
// DEFINE
#define EVENT_MANAGER_DEBUG_EMPTY_QUEUE_ENABLE

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

/////////////////////////////////////////////////
// STATIC FUNCTIONS

/////////////////////////////////////////////////
// EXTERN

/////////////////////////////////////////////////
// CLASS IMPLEMENTAION
/*EventManager*/
EventManager::EventManager(PoolId_t poolId)
  : DataManager(poolId)
{
  EventPoolManager::getInstance();// to keep singleton construction/deconstruction in order
}

EventManager::~EventManager(void)
{
  this->clear();
}

int EventManager::prepare(EventManagerConfigTAG& poolConfig)
{
  do
  {
    if ((poolConfig.preparedDataNumber <= 0)
      || (poolConfig.dataItemConfig.eventItemSize <= 0)
      )
    {
      break;
    }

    return DataManager::prepare<EventDataItem, EventDataItemConfigTAG, EventManagerPropTAG, EventManager>(poolConfig);
  } while (0);

  return -1;
}

int EventManager::prepare(EventCount_t eventItemNumber, EventSize_t eventItemSize, bool usePool)
{
  EventManagerConfigTAG poolConfig = EventManagerConfigTAG();
  poolConfig.dataItemConfig.eventItemSize = eventItemSize;
  poolConfig.preparedDataNumber = eventItemNumber;
  poolConfig.usePool = usePool;
  return this->prepare(poolConfig);
}

void EventManager::clear(void)
{
  //DataManager::clear();
  return DataManager::clearData<EventDataItem, EventDataItemConfigTAG, EventManagerPropTAG, EventManager>();
}

////////////////////Function to call from outside ISR///////////////////////
bool EventManager::isValid(void)
{
  return DataManager::isValid();
}

bool EventManager::isEmpty(void)
{
  return DataManager::isUsingQueueEmpty();
}

bool EventManager::isUsingQueueEmpty(void)
{
  return DataManager::isUsingQueueEmpty();
}

bool EventManager::isReadyQueueEmpty(void)
{
  return DataManager::isReadyQueueEmpty();
}

DataCount_t EventManager::readyQueueSize(void)
{
  return DataManager::readyQueueSize();
}

EventDataItem* EventManager::get(void)
{
  return (EventDataItem*)DataManager::get();
}

EventDataItem* EventManager::wait(unsigned int timeToWait)
{
  do
  {
    this->event_Lock_Key.lock(); // lock if it is unlock
    if (this->busy_Queue.empty() != false)
    {
      this->event_Lock_Key.lock(timeToWait); // lock and wait for specified time
    }
   
    if (this->busy_Queue.empty() != false)
    {
      break;
    }

    return (EventDataItem*)this->busy_Queue.get();
  } while (0);
  return NULL;
}

EventDataItem* EventManager::waitAccurate(unsigned long timeToWait)
{
  do
  {
    this->event_Lock_Key.lock(); // lock if it is unlock
    if (this->busy_Queue.empty() != false)
    {
      this->event_Lock_Key.lockAccuracy(timeToWait); // lock and wait for specified time
    }

    if (this->busy_Queue.empty() != false)
    {
      break;
    }

    return (EventDataItem*)this->busy_Queue.get();
  } while (0);
  return NULL;
}

int EventManager::notify(EventId_t messageId, EventSize_t eventSize, unsigned char* eventData)
{
  do
  {
    if (this->isValid() == false)
    {
      break;
    }
    
    EventDataItem* eventDataItem = (EventDataItem*)this->ready_Queue.get();
    
    if (eventDataItem == NULL)
    {
#ifdef EVENT_MANAGER_DEBUG_EMPTY_QUEUE_ENABLE
      CONSOLE_LOG("%s", "#EM emtpy#");
#endif // EVENT_MANAGER_DEBUG_EMPTY_QUEUE_ENABLE
      break;
    }

    int result = eventDataItem->set(messageId, eventSize, eventData);
    if (result != 0)
    {
      this->ready_Queue.push(eventDataItem); // return back
      break;
    }

    result = this->busy_Queue.push(eventDataItem);
    if (result != 0)
    {
      this->ready_Queue.push(eventDataItem); // return back
      break;
    }

    this->event_Lock_Key.unlock();
    return 0;
  } while (0);

  return -1;
}

int EventManager::release(EventDataItem* eventItem)
{
  do
  {
    if (eventItem == NULL)
    {
      break;
    }
    eventItem->reset();

    return this->ready_Queue.push(eventItem);
  } while (0);

  return -1;
}


////////////////////Function to call from inside ISR///////////////////////
bool EventManager::isValidFromISR(void)
{
  return DataManager::isValidFromISR();
}

bool EventManager::isEmptyFromISR(void)
{
  return DataManager::isUsingQueueEmptyFromISR();
}

bool EventManager::isUsingQueueEmptyFromISR(void)
{
  return DataManager::isUsingQueueEmptyFromISR();
}

bool EventManager::isReadyQueueEmptyFromISR(void)
{
  return DataManager::isReadyQueueEmptyFromISR();
}

DataCount_t EventManager::readyQueueSizeFromISR(void)
{
  return DataManager::readyQueueSizeFromISR();
}

EventDataItem* EventManager::getFromISR(void)
{
  return (EventDataItem*)DataManager::get();
}

EventDataItem* EventManager::waitFromISR(bool* higherTaskWoken)
{
  do
  {
    this->event_Lock_Key.lockFromISR(higherTaskWoken); // lock if it is unlock
    if (this->busy_Queue.emptyFromISR() != false)
    {
      //this->event_Lock_Key.lock(); // lock if it is unlock
      this->event_Lock_Key.lockFromISR(higherTaskWoken); // lock and wait for specified time
    }

    if (this->busy_Queue.emptyFromISR() != false)
    {
      break;
    }
    return (EventDataItem*)this->busy_Queue.getFromISR();

  } while (0);

  return NULL;
}

int EventManager::notifyFromISR(EventId_t messageId, EventSize_t eventSize, unsigned char* eventData, bool* higherTaskWoken)
{
  do
  {
    if (this->isValidFromISR() == false)
    {
      break;
    }
    EventDataItem* eventDataItem = (EventDataItem*)this->ready_Queue.getFromISR();
    if (eventDataItem == NULL)
    {
#ifdef EVENT_MANAGER_DEBUG_EMPTY_QUEUE_ENABLE
      CONSOLE_LOG("%s", "#EM emtpy#");
#endif // EVENT_MANAGER_DEBUG_EMPTY_QUEUE_ENABLE
      break;
    }
    int result = eventDataItem->set(messageId, eventSize, eventData);
    if (result != 0)
    {
      this->ready_Queue.pushFromISR(eventDataItem); // return back
      break;
    }
    result = this->busy_Queue.pushFromISR(eventDataItem);
    if (result != 0)
    {
      this->ready_Queue.pushFromISR(eventDataItem); // return back
      break;
    }
    this->event_Lock_Key.unlockFromISR(higherTaskWoken);
    return 0;
  } while (0);

  return -1;
}

int EventManager::releaseFromISR(EventDataItem* eventItem)
{
  do
  {
    if (eventItem == NULL)
    {
      break;
    }
    eventItem->reset();
    return this->ready_Queue.pushFromISR(eventItem);
  } while (0);

  return -1;
}


#endif // _CONF_EVENT_MANAGER_ENABLED