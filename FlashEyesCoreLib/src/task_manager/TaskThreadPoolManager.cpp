#include "TaskThreadPoolManager.h"
#if (_CONF_TASK_THREAD_POOL_MANAGER_ENABLED)

#ifndef TASK_THREAD_POOL_MANAGER_USE_TEMPLATE
/////////////////////////////////////////////////
// INCLUDE

/////////////////////////////////////////////////
// PREPROCESSOR

/////////////////////////////////////////////////
// DEFINE
#define TASK_THREAD_POOL_MANAGER_GET_ENABLE
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

/*TaskThreadPoolManager*/
TaskThreadPoolManager& TaskThreadPoolManager::getInstance(void)
{
  static TaskThreadPoolManager instance;
  return instance;
}

TaskThreadPoolManager::TaskThreadPoolManager(void)
  : pool_Count(0)
  , pool_List(NULL)
{
  
}

TaskThreadPoolManager::~TaskThreadPoolManager(void)
{
  this->clear();
}

int TaskThreadPoolManager::prepare(byte poolCount, TaskThreadManagerConfigTAG* poolConfigs)
{
  do
  {
    if (this->isValid() != false)
    {
      return -1; //already
    }

    this->clear();
    if ( (poolCount < TASK_THREAD_POOL_MANAGER_POOL_COUNT_MIN)
      || (poolCount > TASK_THREAD_POOL_MANAGER_POOL_COUNT_MAX)
      || (poolConfigs == NULL)
      )
    {
      break;
    }
    int ret = 0;
    this->pool_Count = poolCount;
    this->pool_List = new TaskThreadPoolInfoTAG[this->pool_Count + 1];
    
    TaskThreadPoolInfoTAG* poolInfo = NULL;
    TaskThreadManagerConfigTAG* poolConfig = NULL;
    for (byte wk_idx = 0; wk_idx < this->pool_Count; wk_idx++)
    {
      poolInfo = &this->pool_List[wk_idx];
      poolConfig = &poolConfigs[wk_idx];
      poolInfo->taskSize = poolConfig->threadConfig.usStackDepth;
      poolInfo->pool = new TaskThreadManager();
      if (poolInfo->pool == NULL)
      {
        ret = -1;
        break;
      }
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

void TaskThreadPoolManager::clear(void)
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

bool TaskThreadPoolManager::isValid(void)
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

int TaskThreadPoolManager::get(TaskThreadPoolQueryParamTAG& queryParam, byte& poolId, TaskSystemThread*& thread)
{
  poolId = TASK_THREAD_POOL_ID_INVALID;
  thread = NULL;
  do
  {
    if (this->isValid() == false)
    {
      break;
    }
    TaskThreadPoolRankingListTAG rankingResult = TaskThreadPoolRankingListTAG();
    int ret = this->getRanking(queryParam, rankingResult);
    if (ret != 0)
    {
      break;
    }
    byte candPoolId = TASK_THREAD_POOL_ID_INVALID;
    for (byte wk_idx = 0; wk_idx < TASK_THREAD_POOL_MANAGER_CANDIDATE_NUM; wk_idx++)
    {
      candPoolId = rankingResult.rankings[wk_idx].poolId;
      if (candPoolId == TASK_THREAD_POOL_ID_INVALID)
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

      thread = this->pool_List[candPoolId].pool->get();
      if (thread == NULL)
      {
        continue;
      }

      poolId = candPoolId;
      break;
    }

    if (poolId == TASK_THREAD_POOL_ID_INVALID)
    {
      break;
    }

    return 0;
  } while (0);
  poolId = TASK_THREAD_POOL_ID_INVALID;
  thread = NULL;
#ifdef TASK_THREAD_POOL_MANAGER_GET_ENABLE
  CONSOLE_LOG("[pm] get %i", -1);
#endif // TASK_THREAD_POOL_MANAGER_GET_ENABLE

  return -1;
}

int TaskThreadPoolManager::getRanking(TaskThreadPoolQueryParamTAG& queryParam, TaskThreadPoolRankingListTAG& rankingResult)
{
  do
  {
    rankingResult.clear();
    TaskSize_t score = 0;
    TaskThreadPoolInfoTAG* poolInfo = NULL;
    TaskThreadPoolRankingTAG* ranking = NULL;
    for (byte wk_idx = 0; wk_idx < this->pool_Count; wk_idx++)
    {
      poolInfo = &this->pool_List[wk_idx];
      if ((poolInfo->pool == NULL)
        || (poolInfo->pool->isReadyQueueEmpty() != false)
        || (poolInfo->taskSize < queryParam.taskSize)
        )
      {
        continue;
      }

      score = (TaskSize_t)(poolInfo->taskSize - queryParam.taskSize);
      for (byte can_idx = 0; can_idx < TASK_THREAD_POOL_MANAGER_CANDIDATE_NUM; can_idx++)
      {
        ranking = &rankingResult.rankings[can_idx];
        if ((ranking->poolId != TASK_THREAD_POOL_ID_INVALID)
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

int TaskThreadPoolManager::release(byte poolId, TaskSystemThread* thread)
{
  do
  {
    if ( (this->isValid() == false)
      || (poolId >= this->pool_Count)
      || (thread == NULL)
      || (this->pool_List[poolId].pool == NULL)
      )
    {
      break;
    }

    this->pool_List[poolId].pool->release(thread);

    return 0;
  } while (0);
  return -1;
}
#endif // TASK_THREAD_POOL_MANAGER_USE_TEMPLATE
#endif // _CONF_TASK_THREAD_POOL_MANAGER_ENABLED
