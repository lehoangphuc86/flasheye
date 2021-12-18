#include "DataManagerConstant.h"

#if (_CONF_DATA_MANAGER_ENABLED)

#ifndef _DATA_MANAGER_H
#define _DATA_MANAGER_H

/////////////////////////////////////////////////
// INCLUDE
#include "DataQueue.h"
#include "DataPoolManager.h"
#if (!_CONF_DATA_QUEUE_ENABLED)
#error Data queue is required
#endif // 

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

/*DataManager*/

class DataManager
{
public:
  DataManager(PoolId_t poolId = DATA_POOL_ID_INVALID);
  virtual ~DataManager(void);
  PoolId_t                                                      poolId(void);


  template<typename DataItemT, typename DataItemConfigT, typename DataPoolPropT, typename DataPoolT>
  int                                                           prepare(DataCount_t preparedDataNumber, DataItemConfigT& initStruct, bool usePool = false);
  template<typename DataItemT, typename DataItemConfigT, typename DataPoolPropT, typename DataPoolT>
  int                                                           prepare(DataManagerConfigTAG<DataItemConfigT> dataManagerConfig);
  virtual void                                                  clear(void); // must be overrided by children classes
  ////////////////////Function to call from outside ISR///////////////////////
  virtual bool                                                  isValid(void);
  DataCount_t                                                   readyQueueSize(void);
  bool                                                          isUsingQueueEmpty(void);
  bool                                                          isReadyQueueEmpty(void);
  int                                                           releaseAll(void);
  int                                                           release(DataItem* val);
  virtual DataItem*                                             get(void);
  
  ////////////////////Function to call from inside ISR///////////////////////
  virtual bool                                                  isValidFromISR(void);
  DataCount_t                                                   readyQueueSizeFromISR(void);
  bool                                                          isUsingQueueEmptyFromISR(void);
  bool                                                          isReadyQueueEmptyFromISR(void);
  int                                                           releaseAllFromISR(void);
  int                                                           releaseFromISR(DataItem* val);
  virtual DataItem*                                             getFromISR(void);
  
protected:
  template<typename DataItemT, typename DataItemConfigT, typename DataPoolPropT, typename DataPoolT>
  void                                                          clearData(void);

#ifdef SYSTEM_QUEUE_DUMP_ENABLE
public:
#else // SYSTEM_QUEUE_DUMP_ENABLE
protected:
#endif // SYSTEM_QUEUE_DUMP_ENABLE
  DataQueue                                                     ready_Queue;  // queue to hold ready to use data
  DataQueue                                                     busy_Queue;  // queue to hold being used data
  SystemCriticalSession                                         critical_Key;
  PoolId_t                                                      pool_Id;
};

template<typename DataItemT, typename DataItemConfigT, typename DataPoolPropT, typename DataPoolT>
int DataManager::prepare(DataCount_t preparedDataNumber, DataItemConfigT& initStruct, bool usePool)
{
  int result = 0;
  do
  {
    if (this->isValid() != false)
    {
      return -1;
    }

    result = this->ready_Queue.prepare(true, preparedDataNumber);
    if (result != 0)
    {
      break;
    }

    result = this->busy_Queue.prepare(false, preparedDataNumber);
    if (result != 0)
    {
      break;
    }
  
    DataItemT* dataItem = NULL;
    if (usePool != false)
    {
      // request from pool manager
      DataPoolQueryParamTAG<DataPoolPropT> queryParam = DataPoolQueryParamTAG<DataPoolPropT>();
      queryParam.poolProperties.set(initStruct);
      DataCount_t retQuantity = 0;
      DataCount_t candPoolCapacity = 0;
      while (retQuantity < preparedDataNumber)
      {
        if (candPoolCapacity <=0)
        {
          result = DataPoolManagerTpl<DataItemT, DataItemConfigT, DataPoolPropT, DataPoolT>::getInstance().request(queryParam, dataItem);
        }
        else
        {
          // keep requesting same pool if it still has data
          result = DataPoolManagerTpl<DataItemT, DataItemConfigT, DataPoolPropT, DataPoolT>::getInstance().request(dataItem->poolId(), dataItem);
        }

        if ((result != 0)
          && (candPoolCapacity > 0))
        {
          // the candidate pool is empty
          candPoolCapacity = 0;
          continue;
        }
        
        if ((result != 0)
          || (dataItem == NULL)
          )
        {
          break;
        }

        result = this->ready_Queue.push(dataItem);
        if (result != 0)
        {
          DataPoolManagerTpl<DataItemT, DataItemConfigT, DataPoolPropT, DataPoolT>::getInstance().release(dataItem);
          break;
        }
        candPoolCapacity = DataPoolManagerTpl<DataItemT, DataItemConfigT, DataPoolPropT, DataPoolT>::getInstance().getPoolCapacity(dataItem->poolId());
        retQuantity++;
      }

      if (retQuantity < preparedDataNumber)
      {
        break;
      }
      return 0;
    }

    for (DataCount_t wkIndex = 0; wkIndex < preparedDataNumber; wkIndex++)
    {
      dataItem = new DataItemT(wkIndex, this->pool_Id); // sequenceId
      if (dataItem == NULL)
      {
        goto _ExceptionOccurred;
      }
      result = dataItem->prepare(initStruct);
      if (result != 0)
      {
        delete dataItem;
        goto _ExceptionOccurred;
      }

      result = this->ready_Queue.push(dataItem);
      if (result != 0)
      {
        delete dataItem;
        goto _ExceptionOccurred;
      }
    }

    return 0;
  } while (0);

_ExceptionOccurred:
  this->clear();
  return -1;
}

template<typename DataItemT, typename DataItemConfigT, typename DataPoolPropT, typename DataPoolT>
int DataManager::prepare(DataManagerConfigTAG<DataItemConfigT> dataManagerConfig)
{
  return this->prepare<DataItemT, DataItemConfigT, DataPoolPropT, DataPoolT>(dataManagerConfig.preparedDataNumber, dataManagerConfig.dataItemConfig, dataManagerConfig.usePool);
}


template<typename DataItemT, typename DataItemConfigT, typename DataPoolPropT, typename DataPoolT>
void DataManager::clearData(void)
{
  do
  {
    this->busy_Queue.isReleasableData(true);
    this->ready_Queue.isReleasableData(true);
    this->releaseAll();
    DataItem* dataItem = NULL;
    while (this->ready_Queue.empty() == false)
    {
      dataItem = (this->ready_Queue.get());
      if (dataItem == NULL)
      {
        continue;
      }

      if (dataItem->poolId() == this->poolId())
      {
        delete dataItem;
        dataItem = NULL;
        continue;
      }

      DataPoolManagerTpl<DataItemT, DataItemConfigT, DataPoolPropT, DataPoolT>::getInstance().release((DataItemT*)dataItem);
    }
    
    this->busy_Queue.clear();
    this->ready_Queue.clear();
  } while (0);
}


#endif // _DATA_MANAGER_H


#endif // _CONF_DATA_MANAGER_ENABLED