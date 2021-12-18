/////////////////////////////////////////////////
// INCLUDE
#include "DataItem.h"
#if (_CONF_DATA_ITEM_ENABLED)
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

/* DataItem*/
DataItem::DataItem(byte dataItemType, DataCount_t sequenceId, PoolId_t poolId)
  //: data_Type_Id(dataItemType)
  : sequence_Id(sequenceId)
  , pool_Id(poolId)
{

}

DataItem::~DataItem(void)
{
  this->clear();
}

int DataItem::clear(void)
{
  return 0;
}

DataCount_t DataItem::id(void)
{
  return this->sequence_Id;
}

PoolId_t DataItem::poolId(void)
{
  return this->pool_Id;
}
//
//void DataItem::poolId(PoolId_t poolId)
//{
//  this->pool_Id = poolId;
//}
//
//void DataItem::resetPoolId(void)
//{
//  this->pool_Id = DATA_POOL_ID_INVALID;
//}

/* BufferDataItem*/
BufferDataItem::BufferDataItem(DataCount_t sequenceId, PoolId_t poolId)
  : BufferDataItem((byte)DataManagerConstant::DataItemType::BufferDataItem, sequenceId, poolId)
{

}

BufferDataItem::BufferDataItem(byte dataItemType, DataCount_t sequenceId, PoolId_t poolId)
  : DataItem(dataItemType, sequenceId, poolId)
  , data_Length(0)
  , buffer_Length(0)
  , buffer_Address(NULL)
{

}

BufferDataItem::~BufferDataItem(void)
{
  this->clear();
}

bool BufferDataItem::isValid(void)
{
  do
  {
    if ( (this->bufferLength() <= 0)
      || (this->bufferAddress() == NULL)
      )
    {
      break;
    }

    return true;
  } while(0);
  
  return false;
}

DataSize_t BufferDataItem::dataLength(void)
{
  return this->data_Length;
}

bool BufferDataItem::hasData(void)
{
  do
  {
    if (this->dataLength() <= 0)
    {
      break;
    }

    return true;
  } while(0);
  return false;
}

DataSize_t BufferDataItem::bufferLength(void)
{
  return this->buffer_Length;
}

unsigned char * BufferDataItem::bufferAddress(void)
{
  return this->buffer_Address;
}

int BufferDataItem::prepare(BufferDataItemConfigTAG& initStruct)
{
  do
  {
    if (this->isValid() != false)
    {
      return -1; // already prepared before. Should not be allocated again
    }

    if (initStruct.bufferSize <= 0)
    {
      break;
    }

    this->buffer_Address = new unsigned char[initStruct.bufferSize];
    if (this->buffer_Address == NULL) // failed to malloc
    {
      break;
    }

    this->buffer_Length = initStruct.bufferSize;
    this->reset();

    return 0;
  } while(0);

  this->clear();
  return -1;
}

int BufferDataItem::setData(DataSize_t srcDataSize, unsigned char* srcData)
{
  do
  {
    if (this->isValid() == false)
    {
      break;
    }

    /*if ( (srcDataSize <= 0)
      || (srcDataSize > this->buffer_Length)
      || (srcData == NULL)
      )*/
    if ((srcDataSize < 0)
      || (srcDataSize > this->buffer_Length)
      )
    {
      break;
    }

    if (srcData != NULL)
    {
      memcpy(this->buffer_Address, srcData, srcDataSize);
    }

    this->data_Length = srcDataSize;

    return 0;
  } while (0);

  this->reset();
  return -1;
}

int BufferDataItem::setDataLen(DataSize_t dataSize)
{
  do
  {
    if (this->isValid() == false)
    {
      break;
    }

    if ((dataSize < 0)
      || (dataSize > this->buffer_Length)
      )
    {
      break;
    }

    this->data_Length = dataSize;

    return 0;
  } while (0);

  this->reset();
  return -1;
}


int BufferDataItem::reset(void)
{
  do
  {
    if (this->isValid() == false)
    {
      break;
    }

    this->data_Length = 0;

    return 0;
  } while (0);

  return -1;
}

int BufferDataItem::clear(void)
{
  do
  {
    if (this->isValid() == false)
    {
      break;
    }

    if (this->data_Length == 1)
    {
      delete this->buffer_Address;
    }
    else
    {
      delete[] this->buffer_Address;
    }

    this->data_Length = 0;
    this->reset();

    return 0;
  } while (0);


  this->buffer_Length = 0;
  this->buffer_Address = NULL;
  this->reset();

  return -1;
}

#endif//  (_CONF_DATA_ITEM_ENABLED)