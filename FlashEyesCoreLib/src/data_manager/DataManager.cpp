#ifndef _DATA_MANAGER_CPP
#define _DATA_MANAGER_CPP

/////////////////////////////////////////////////
// INCLUDE
#include "DataManager.h"

#if (_CONF_DATA_MANAGER_ENABLED)
/////////////////////////////////////////////////
// PREPROCESSOR

/////////////////////////////////////////////////
// DEFINE
#define DATA_MANAGER_DEBUG_EMPTY_QUEUE_ENABLE

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
/*DataManager*/
DataManager::DataManager(PoolId_t poolId)
  : pool_Id(poolId)
{

}

DataManager::~DataManager(void)
{
  this->clear();
}

void DataManager::clear(void)
{
  // for example
  return;// this->clearData<BufferDataItem, BufferDataItemConfigTAG, BufferDataManagerPropTAG>();
}

PoolId_t DataManager::poolId(void)
{
  return this->pool_Id;
}

////////////////////Function to call from outside ISR///////////////////////
bool DataManager::isValid(void)
{
  if ((this->busy_Queue.empty() == false)
    || (this->ready_Queue.empty() == false)
    )
  {
    return true;
  }

  return false;
}

DataCount_t DataManager::readyQueueSize(void)
{
  return this->ready_Queue.size();
}

bool DataManager::isUsingQueueEmpty(void)
{
  return this->busy_Queue.empty();
}

bool DataManager::isReadyQueueEmpty(void)
{
  return this->ready_Queue.empty();
}

DataItem* DataManager::get(void)
{
  DataItem* result = NULL;
  int ret = 0;
  do
  {
    result = this->ready_Queue.get();
    if (result == NULL)
    {
#ifdef DATA_MANAGER_DEBUG_EMPTY_QUEUE_ENABLE
      CONSOLE_LOG("%s", "#DM emtpy#");
#endif // DATA_MANAGER_DEBUG_EMPTY_QUEUE_ENABLE
      break;
    }

    ret = this->busy_Queue.push(result);
    if (ret != 0)
    {
      this->ready_Queue.push(result);
      break;
    }
    return result;
  } while (0);

  return NULL;
}

int DataManager::release(DataItem* val)
{
  int result = 0;
  do
  {
    result = this->busy_Queue.remove(val);
    if (result != 0)
    {
      break;
    }

    result = this->ready_Queue.push(val);
    if (result != 0)
    {
      break;
    }

    return 0;
  } while (0);

  return -1;
}

int DataManager::releaseAll(void)
{
  SystemCriticalLocker criticalLocker(this->critical_Key);
  int result = 0;
  do
  {
    if ((this->busy_Queue.empty() != false)
      && (this->ready_Queue.empty() != false)
      )
    {
      break;
    }
    while (this->busy_Queue.empty() == false)
    {
      DataItem* data = this->busy_Queue.front();
      result = this->busy_Queue.remove(data);
      if (result != 0)
      {
        goto _ErrorOccurred;
      }

      result = this->ready_Queue.push(data);
      if (result != 0)
      {
        goto _ErrorOccurred;
      }
    }

    return 0;
  } while (0);

_ErrorOccurred:
  return -1;
}

////////////////////Function to call from inside ISR///////////////////////
bool DataManager::isValidFromISR(void)
{
  if ((this->busy_Queue.emptyFromISR() == false)
    || (this->ready_Queue.emptyFromISR() == false)
    )
  {
    return true;
  }

  return false;
}

DataCount_t  DataManager::readyQueueSizeFromISR(void)
{
  return this->ready_Queue.sizeFromISR();
}

bool DataManager::isUsingQueueEmptyFromISR(void)
{
  return this->busy_Queue.emptyFromISR();
}

bool DataManager::isReadyQueueEmptyFromISR(void)
{
  return this->ready_Queue.emptyFromISR();
}

DataItem* DataManager::getFromISR(void)
{
  DataItem* result = NULL;
  int ret = 0;
  do
  {
    result = this->ready_Queue.getFromISR();
    if (result == NULL)
    {
#ifdef DATA_MANAGER_DEBUG_EMPTY_QUEUE_ENABLE
      CONSOLE_LOG("%s", "#DM emtpy#");
#endif // DATA_MANAGER_DEBUG_EMPTY_QUEUE_ENABLE
      break;
    }

    ret = this->busy_Queue.pushFromISR(result);
    if (ret != 0)
    {
      this->ready_Queue.pushFromISR(result);
      break;
    }
    return result;
  } while (0);

  return NULL;
}

int DataManager::releaseFromISR(DataItem* val)
{
  int result = 0;
  do
  {
    result = this->busy_Queue.removeFromISR(val);
    if (result != 0)
    {
      break;
    }

    result = this->ready_Queue.pushFromISR(val);
    if (result != 0)
    {
      break;
    }

    return 0;
  } while (0);

  return -1;
}

int DataManager::releaseAllFromISR(void)
{
  SystemCriticalLockerFromISR criticalLocker(this->critical_Key);
  int result = 0;
  do
  {
    if ((this->busy_Queue.emptyFromISR() != false)
      && (this->ready_Queue.emptyFromISR() != false)
      )
    {
      break;
    }
    while (this->busy_Queue.emptyFromISR() == false)
    {
      DataItem* data = this->busy_Queue.frontFromISR();
      result = this->busy_Queue.removeFromISR(data);
      if (result != 0)
      {
        goto _ErrorOccurred;
      }

      result = this->ready_Queue.pushFromISR(data);
      if (result != 0)
      {
        goto _ErrorOccurred;
      }
    }

    return 0;
  } while (0);

_ErrorOccurred:
  return -1;
}
#endif // (_CONF_DATA_MANAGER_ENABLED)

#endif // _DATA_MANAGER_CPP


