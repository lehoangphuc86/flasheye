#include "EventManagerConstant.h"

#if (_CONF_EVENT_DATA_ITEM_ENABLED)
#ifndef _EVENT_DATA_ITEM_H
#define _EVENT_DATA_ITEM_H

/////////////////////////////////////////////////
// INCLUDE
#include "../data_manager/DataItem.h"

#if (!_CONF_DATA_ITEM_ENABLED)
#error Data item is required
#endif // _CONF_DATA_ITEM_ENABLED

#if (!_CONF_EVENT_MANAGER_CONSTANT_ENABLED)
#error Event manager constant is required
#endif // _CONF_EVENT_MANAGER_CONSTANT_ENABLED

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
/*EventDataItem*/
class EventDataItem
  : public BufferDataItem
{
public:
  EventDataItem(unsigned int sequenceId, PoolId_t poolId);
  // WARNING: if inherite from this class, deconstructor must be virtual
  __ATTRIBUTE_VIRTUAL_OPTIMIZED ~EventDataItem(void);
  bool                                                          isValid(void) override;
  EventId_t                                                     messageId(void);
  int                                                           prepare(EventDataItemConfigTAG& initStruct);
  int                                                           set(EventId_t messageId, EventSize_t srcDataSize, unsigned char* srcData);
  int                                                           reset(void) override; // reset data for the next use
protected:
  int                                                           setMessageId(EventId_t messageId);
  //virtual int                                                   setData(DataSize_t srcDataSize, unsigned char* srcData) override;
  int                                                           clear(void) override; // clear buffer
  int                                                           prepare(BufferDataItemConfigTAG& initStruct) override; // to hide this from public
protected:
  EventId_t                                                     message_Id;  // message id
};
#endif // _EVENT_DATA_ITEM_H

#endif // _CONF_EVENT_DATA_ITEM_ENABLED