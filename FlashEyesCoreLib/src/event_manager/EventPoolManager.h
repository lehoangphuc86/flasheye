#include "EventManagerConstant.h"

#ifndef _EVENT_POOL_MANAGER_H
#define _EVENT_POOL_MANAGER_H

#if (_CONF_EVENT_POOL_MANAGER_ENABLED)

/////////////////////////////////////////////////
// INCLUDE

#include "EventDataItem.h"

#if (!_CONF_EVENT_DATA_ITEM_ENABLED)
#error Event data item is required
#endif // _CONF_EVENT_DATA_ITEM_ENABLED
/////////////////////////////////////////////////
// PREPROCESSOR

#include "../data_manager/DataPoolManagerTpl.h"
//#include "../data_manager/DataPoolManagerTpl.cpp"
//#define EVENT_POOL_MANAGER_DEBUG_CONSOLE_ENABLE
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
class EventManager;
/////////////////////////////////////////////////
// DATA TYPE (ENUM)

/////////////////////////////////////////////////
// DATA TYPE (STRUCT)
typedef struct _eventManagerPropTAG
{
  EventSize_t eventSize;
  _eventManagerPropTAG(void)
  {
    this->clear();
  }

  void clear(void)
  {
    this->eventSize = 0;
  }

  // example of scoring function
  void set(EventDataItemConfigTAG& config)
  {
    this->eventSize = config.eventItemSize;
  }

  int rankScoring(_eventManagerPropTAG& query, RankingScore_t& score)
  {
    do
    {
      score = 0;
      if (this->eventSize < query.eventSize)
      {
#ifdef EVENT_POOL_MANAGER_DEBUG_CONSOLE_ENABLE
        CONSOLE_LOG("[eDMp] rk %i %i", this->eventSize, query.eventSize);
#endif // EVENT_POOL_MANAGER_DEBUG_CONSOLE_ENABLE
        break;
      }

      score = (RankingScore_t) (this->eventSize - query.eventSize);

      return 0;
    } while (0);

    return -1;
  }
} EventManagerPropTAG;

typedef DataPoolQueryParamTAG<EventManagerPropTAG> EventPoolQueryParamTAG;
typedef DataPoolManagerTpl<EventDataItem, EventDataItemConfigTAG, EventManagerPropTAG, EventManager> EventPoolManager;

/////////////////////////////////////////////////
// STATIC DATA

/////////////////////////////////////////////////
// STATIC FUNCTIONS

/////////////////////////////////////////////////
// EXTERN

/////////////////////////////////////////////////
// CLASS DEFINITION

#endif // _CONF_EVENT_POOL_MANAGER_ENABLED

#endif // _EVENT_POOL_MANAGER_H