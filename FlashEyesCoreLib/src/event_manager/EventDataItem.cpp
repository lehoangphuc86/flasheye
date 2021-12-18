/////////////////////////////////////////////////
// INCLUDE
#include "EventDataItem.h"
#if (_CONF_EVENT_DATA_ITEM_ENABLED)
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


/* EventDataItem*/
EventDataItem::EventDataItem(unsigned int sequenceId, PoolId_t poolId)
  : BufferDataItem((byte)DataManagerConstant::DataItemType::EventDataItem, (DataCount_t)sequenceId, poolId)
  , message_Id((EventId_t)EventManagerConstant::EventMessageId::InvalidMessageId)
{

}

EventDataItem::~EventDataItem(void)
{
  this->clear();
}

EventId_t EventDataItem::messageId(void)
{
  return this->message_Id;
}

int EventDataItem::prepare(EventDataItemConfigTAG& initStruct)
{
  BufferDataItemConfigTAG buffInitStruct = BufferDataItemConfigTAG();
  buffInitStruct.bufferSize = (DataSize_t)initStruct.eventItemSize;
  return BufferDataItem::prepare(buffInitStruct);
}

bool EventDataItem::isValid(void)
{
  do
  {
    if (BufferDataItem::isValid() == false)
    {
      break;
    }

    if (EventManagerConstant::isValidMessageId(this->messageId()) == false)
    {
      break;
    }

    return true;
  } while (0);

  return false;
}

int EventDataItem::set(EventId_t messageId, EventSize_t srcDataSize, unsigned char* srcData)
{
  do
  {
    if (this->setMessageId(messageId) != 0)
    {
      break;
    }

    if (this->setData(srcDataSize, srcData) != 0)
    {
      break;
    }

    return 0;
  } while (0);

  this->reset();
  return -1;
}


int EventDataItem::reset(void)
{
  do
  {
    if (this->isValid() == false)
    {
      break;
    }

    int ret = BufferDataItem::reset();
    this->message_Id = (byte) EventManagerConstant::EventMessageId::InvalidMessageId;
    return ret;

  } while (0);

  return -1;
}

int EventDataItem::setMessageId(EventId_t messageId)
{
  do
  {
    if (EventManagerConstant::isValidMessageId(messageId) == false)
    {
      break;
    }

    this->message_Id = messageId;

    return 0;
  } while (0);

  return -1;
}

//int EventDataItem::setData(DataSize_t srcDataSize, unsigned char* srcData)
//{
//  return BufferDataItem::setData(srcDataSize, srcData);
//}

int EventDataItem::prepare(BufferDataItemConfigTAG& initStruct)
{
  return BufferDataItem::prepare(initStruct);
}

int EventDataItem::clear(void)
{
  BufferDataItem::clear();
  this->reset();
  return 0;
}


#endif // _CONF_EVENT_DATA_ITEM_ENABLED