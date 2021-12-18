#include "DataPoolManagerTpl.h"
#ifndef _DATA_POOL_MANAGER_TPL_CPP
#define _DATA_POOL_MANAGER_TPL_CPP

#if (_CONF_DATA_POOL_MANAGER_TPL_ENABLED)
/////////////////////////////////////////////////
// INCLUDE

/////////////////////////////////////////////////
// PREPROCESSOR

/////////////////////////////////////////////////
// DEFINE
#define DATA_POOL_MANAGER_DEBUG_CONSOLE_ENABLE
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

/*DataPoolManagerTpl*/
template<typename DataItemT, typename DataItemConfigT, typename DataPoolPropT, typename DataPoolT>
DataPoolManagerTpl<DataItemT, DataItemConfigT, DataPoolPropT, DataPoolT>& DataPoolManagerTpl<DataItemT, DataItemConfigT, DataPoolPropT, DataPoolT>::getInstance(void)
{
  static DataPoolManagerTpl<DataItemT, DataItemConfigT, DataPoolPropT, DataPoolT> instance;
  return instance;
}

template<typename DataItemT, typename DataItemConfigT, typename DataPoolPropT, typename DataPoolT>
DataPoolManagerTpl<DataItemT, DataItemConfigT, DataPoolPropT, DataPoolT>::DataPoolManagerTpl(void)
  : pool_Count(0)
  , pool_List(NULL)
{
  
}

template<typename DataItemT, typename DataItemConfigT, typename DataPoolPropT, typename DataPoolT>
DataPoolManagerTpl<DataItemT, DataItemConfigT, DataPoolPropT, DataPoolT>::~DataPoolManagerTpl(void)
{
#ifdef DATA_POOL_MANAGER_DEBUG_CONSOLE_ENABLE
  //CONSOLE_LOG("[dpm] ~ %i a %i", 1, this->isValid());
#endif // DATA_POOL_MANAGER_DEBUG_CONSOLE_ENABLE
  this->clear();
#ifdef DATA_POOL_MANAGER_DEBUG_CONSOLE_ENABLE
  //CONSOLE_LOG("[dpm] ~ %i b %i", 2, this->isValid());
#endif // DATA_POOL_MANAGER_DEBUG_CONSOLE_ENABLE
}

template<typename DataItemT, typename DataItemConfigT, typename DataPoolPropT, typename DataPoolT>
int DataPoolManagerTpl<DataItemT, DataItemConfigT, DataPoolPropT, DataPoolT>::prepare(byte poolCount, DataManagerConfigTAG<DataItemConfigT>* poolConfigs)
{
  do
  {
    if (this->isValid() != false)
    {
      return -1; //already
    }

    this->clear();
    if ( (poolCount < DATA_POOL_MANAGER_POOL_COUNT_MIN)
      || (poolCount > DATA_POOL_MANAGER_POOL_COUNT_MAX)
      || (poolConfigs == NULL)
      )
    {
      break;
    }
    int ret = 0;
    this->pool_Count = poolCount;
    this->pool_List = new DataPoolInfoTAG<DataPoolPropT, DataPoolT>[this->pool_Count + 1];
    
    DataPoolInfoTAG<DataPoolPropT, DataPoolT>* poolInfo = NULL;
    DataManagerConfigTAG<DataItemConfigT>* poolConfig = NULL;
    for (byte wk_idx = 0; wk_idx < this->pool_Count; wk_idx++)
    {
      poolInfo = &this->pool_List[wk_idx];
      poolConfig = &poolConfigs[wk_idx];
      poolInfo->properties.set(poolConfig->dataItemConfig);
      poolInfo->pool = new DataPoolT(wk_idx);// new DataPoolT(wk_idx);
      if (poolInfo->pool == NULL)
      {
        ret = -1;
        break;
      }
      poolConfig->usePool = false; // self create data
      //ret = poolInfo->pool->prepare<DataItemT, DataItemConfigT, DataPoolPropT>(*poolConfig);
      ret = poolInfo->pool->prepare(*poolConfig);
      if (ret != 0)
      {
        break;
      }
    }

    if (ret != 0)
    {
      break;
    }

    return 0;
  } while (0);

  this->clear();
  return -1;
}

template<typename DataItemT, typename DataItemConfigT, typename DataPoolPropT, typename DataPoolT>
void DataPoolManagerTpl<DataItemT, DataItemConfigT, DataPoolPropT, DataPoolT>::clear(void)
{
  do
  {
    if (this->isValid() == false)
    {
      // not created yet
      break;
    }

    for (byte wk_idx = 0; wk_idx < this->pool_Count; wk_idx++)
    {
      if (this->pool_List[wk_idx].pool == NULL)
      {
        continue;
      }
      delete this->pool_List[wk_idx].pool;
      this->pool_List[wk_idx].pool = NULL;
    }

    delete[] this->pool_List;
    this->pool_List = NULL;
    this->pool_Count = 0;
    return;
  } while (0);

  this->pool_List = NULL;
  this->pool_Count = 0;
  return;
}

template<typename DataItemT, typename DataItemConfigT, typename DataPoolPropT, typename DataPoolT>
bool DataPoolManagerTpl<DataItemT, DataItemConfigT, DataPoolPropT, DataPoolT>::isValid(void)
{
  do
  {
    if ((this->pool_Count <=0)
     || (this->pool_List == NULL)
      )
    {
      break;
    }

    return true;
  } while (0);
  return false;
}

template<typename DataItemT, typename DataItemConfigT, typename DataPoolPropT, typename DataPoolT>
DataCount_t DataPoolManagerTpl<DataItemT, DataItemConfigT, DataPoolPropT, DataPoolT>::getPoolCapacity(PoolId_t poolId)
{
  do
  {
    if (this->isValid() == false)
    {
      break;
    }

    if ((poolId >= this->pool_Count)
      || (this->pool_List[poolId].pool == NULL)
      )
    {
      break;
    }

    return (DataCount_t)this->pool_List[poolId].pool->readyQueueSize();
  } while (0);
  return 0;
}


template<typename DataItemT, typename DataItemConfigT, typename DataPoolPropT, typename DataPoolT>
int DataPoolManagerTpl<DataItemT, DataItemConfigT, DataPoolPropT, DataPoolT>::request(DataPoolQueryParamTAG<DataPoolPropT>& queryParam, DataItemT*& dataItem)
{
  //poolId = DATA_POOL_ID_INVALID;
  dataItem = NULL;
  do
  {
    if (this->isValid() == false)
    {
      break;
    }
    DataPoolRankingListTAG rankingResult = DataPoolRankingListTAG();
    int ret = this->getRanking(queryParam, rankingResult);
    if (ret != 0)
    {
      break;
    }
    byte candPoolId = DATA_POOL_ID_INVALID;
    for (byte wk_idx = 0; wk_idx < DATA_POOL_MANAGER_CANDIDATE_NUM; wk_idx++)
    {
      candPoolId = rankingResult.rankings[wk_idx].poolId;
      if (candPoolId == DATA_POOL_ID_INVALID)
      {
        break;
      }
      if ((candPoolId >= this->pool_Count)
        || (this->pool_List[candPoolId].pool == NULL)
        || (this->pool_List[candPoolId].pool->isReadyQueueEmpty() != false)
        )
      {
        continue;
      }

      dataItem = (DataItemT*)this->pool_List[candPoolId].pool->get();
      if (dataItem == NULL)
      {
        continue;
      }

      //poolId = candPoolId;
      break;
    }

    if (dataItem == NULL)
    {
      break;
    }
    //dataItem->poolId(candPoolId);
    return 0;
  } while (0);
  dataItem = NULL;
#ifdef DATA_POOL_MANAGER_DEBUG_CONSOLE_ENABLE
  CONSOLE_LOG("[dpm] req %i", -1);
#endif // DATA_POOL_MANAGER_DEBUG_CONSOLE_ENABLE

  return -1;
}

template<typename DataItemT, typename DataItemConfigT, typename DataPoolPropT, typename DataPoolT>
int DataPoolManagerTpl<DataItemT, DataItemConfigT, DataPoolPropT, DataPoolT>::request(PoolId_t poolId, DataItemT*& dataItem)
{
  dataItem = NULL;
  do
  {
    if (this->isValid() == false)
    {
      break;
    }

    if ((poolId >= this->pool_Count)
      || (this->pool_List[poolId].pool == NULL)
      )
    {
      break;
    }

    dataItem = (DataItemT*)this->pool_List[poolId].pool->get();
    if (dataItem == NULL)
    {
      break;
    }
    return 0;
  } while (0);
  dataItem = NULL;
#ifdef DATA_POOL_MANAGER_DEBUG_CONSOLE_ENABLE
  CONSOLE_LOG("[dpm] req2 %i", -1);
#endif // DATA_POOL_MANAGER_DEBUG_CONSOLE_ENABLE
  return -1;
}

template<typename DataItemT, typename DataItemConfigT, typename DataPoolPropT, typename DataPoolT>
int DataPoolManagerTpl<DataItemT, DataItemConfigT, DataPoolPropT, DataPoolT>::getRanking(DataPoolQueryParamTAG<DataPoolPropT>& queryParam, DataPoolRankingListTAG& rankingResult)
{
  do
  {
    rankingResult.clear();
    //TaskSize_t score = 0;
    RankingScore_t score = 0;
    DataPoolInfoTAG<DataPoolPropT, DataPoolT>* poolInfo = NULL;
    DataPoolRankingTAG* ranking = NULL;
    int ret = 0;
    for (byte wk_idx = 0; wk_idx < this->pool_Count; wk_idx++)
    {
      poolInfo = &this->pool_List[wk_idx];
      if ((poolInfo->pool == NULL)
        || (poolInfo->pool->isReadyQueueEmpty() != false)
        //|| (poolInfo->taskSize < queryParam.taskSize)
        )
      {
        continue;
      }

      //score = (TaskSize_t)(poolInfo->taskSize - queryParam.taskSize);
      ret = poolInfo->properties.rankScoring(queryParam.poolProperties, score);
      if (ret != 0)
      {
        continue;
      }
      for (byte can_idx = 0; can_idx < DATA_POOL_MANAGER_CANDIDATE_NUM; can_idx++)
      {
        ranking = &rankingResult.rankings[can_idx];
        if ((ranking->poolId != DATA_POOL_ID_INVALID)
          && (ranking->score <= score)
          )
        {
          continue;
        }
        
        ranking->poolId = (byte)wk_idx;
        ranking->score = score;
        break;
      }
    }
    return 0;
  } while (0);
  return -1;
}

template<typename DataItemT, typename DataItemConfigT, typename DataPoolPropT, typename DataPoolT>
int DataPoolManagerTpl<DataItemT, DataItemConfigT, DataPoolPropT, DataPoolT>::release(DataItemT* dataItem)
{
  do
  {
    if ( (this->isValid() == false)
      || (dataItem == NULL)
      )
    {
      break;
    }

    PoolId_t poolId = dataItem->poolId();

    if ((poolId >= this->pool_Count)
      || (this->pool_List[poolId].pool == NULL)
      )
    {
      break;
    }

    //dataItem->resetPoolId();
    this->pool_List[poolId].pool->release(dataItem);

    return 0;
  } while (0);
  return -1;
}

#endif // _CONF_DATA_POOL_MANAGER_TPL_ENABLED

#endif // _DATA_POOL_MANAGER_TPL_CPP