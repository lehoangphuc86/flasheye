#include "TaskManagerConstant.h"

#ifndef _TASK_THREAD_POOL_MANAGER_H
#define _TASK_THREAD_POOL_MANAGER_H

#if (_CONF_TASK_THREAD_POOL_MANAGER_ENABLED)

/////////////////////////////////////////////////
// INCLUDE
#include "TaskSystemThread.h"
#if (!_CONF_TASK_THREAD_MANAGER_ENABLED)
#error Task thread pool is required
#endif // _CONF_TASK_THREAD_POOL_ENABLED
/////////////////////////////////////////////////
// PREPROCESSOR

#define TASK_THREAD_POOL_MANAGER_USE_TEMPLATE


#ifdef TASK_THREAD_POOL_MANAGER_USE_TEMPLATE
#include "../data_manager/DataPoolManagerTpl.h"
//#include "../data_manager/DataPoolManagerTpl.cpp"
#endif // TASK_THREAD_POOL_MANAGER_USE_TEMPLATE
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
#ifdef TASK_THREAD_POOL_MANAGER_USE_TEMPLATE
class TaskThreadManager;

typedef struct _taskThreadManagerPropTAG
{
  TaskSize_t taskSize;
  _taskThreadManagerPropTAG(void)
  {
    this->clear();
  }

  void clear(void)
  {
    this->taskSize = 0;
  }

  // example of scoring function
  void set(SystemThreadConfigTAG& config)
  {
    this->taskSize = config.usStackDepth;
  }

  int rankScoring(_taskThreadManagerPropTAG& query, RankingScore_t& score)
  {
    do
    {
      score = 0;
      if (this->taskSize < query.taskSize)
      {
        break;
      }

      score = (RankingScore_t) (this->taskSize - query.taskSize);

      return 0;
    } while (0);

    return -1;
  }
} TaskThreadManagerPropTAG;

typedef DataPoolQueryParamTAG<TaskThreadManagerPropTAG> TaskThreadPoolQueryParamTAG;
typedef DataPoolManagerTpl<TaskSystemThread, SystemThreadConfigTAG, TaskThreadManagerPropTAG, TaskThreadManager> TaskThreadPoolManager;

#else // TASK_THREAD_POOL_MANAGER_USE_TEMPLATE
typedef struct _taskThreadPoolQueryParamTAG
{
  TaskSize_t taskSize;
} TaskThreadPoolQueryParamTAG;

typedef struct _taskThreadPoolRankingTAG
{
  byte poolId;
  TaskSize_t score;
  _taskThreadPoolRankingTAG(void)
  {
    this->clear();
  }

  void clear(void)
  {
    this->poolId = TASK_THREAD_POOL_ID_INVALID;
    this->score = 0;
  }
} TaskThreadPoolRankingTAG;

typedef struct _taskThreadPoolRankingListTAG
{
  TaskThreadPoolRankingTAG rankings[TASK_THREAD_POOL_MANAGER_CANDIDATE_NUM];
  _taskThreadPoolRankingListTAG(void)
  {
    this->clear();
  }

  void clear(void)
  {
    for (byte wk_idx = 0; wk_idx < TASK_THREAD_POOL_MANAGER_CANDIDATE_NUM; wk_idx++)
    {
      this->rankings[wk_idx].clear();
    }
  }

} TaskThreadPoolRankingListTAG;


typedef struct _taskThreadPoolInfoTAG
{
  TaskThreadManager* pool;
  TaskSize_t taskSize;
} TaskThreadPoolInfoTAG;

#endif // TASK_THREAD_POOL_MANAGER_USE_TEMPLATE
/////////////////////////////////////////////////
// STATIC DATA

/////////////////////////////////////////////////
// STATIC FUNCTIONS

/////////////////////////////////////////////////
// EXTERN

/////////////////////////////////////////////////
// CLASS DEFINITION
#ifndef TASK_THREAD_POOL_MANAGER_USE_TEMPLATE
/*TaskThreadPoolManager*/
class TaskThreadPoolManager
{
  ////////////////////Function to call from outside ISR///////////////////////
public:
  static TaskThreadPoolManager& getInstance(void);
  TaskThreadPoolManager(TaskThreadPoolManager const&) = delete;
  void operator=(TaskThreadPoolManager const&) = delete;
  int                                                           prepare(byte poolCount, TaskThreadManagerConfigTAG* poolConfigs);
  void                                                          clear(void);
  bool                                                          isValid(void);
  int                                                           get(TaskThreadPoolQueryParamTAG& queryParam, byte& poolId, TaskSystemThread*& thread);
  int                                                           release(byte poolId, TaskSystemThread* thread);
protected:
  int                                                           getRanking(TaskThreadPoolQueryParamTAG& queryParam, TaskThreadPoolRankingListTAG& rankingResult);
private:
  TaskThreadPoolManager(void);
  __ATTRIBUTE_VIRTUAL_OPTIMIZED ~TaskThreadPoolManager(void);

protected:
  byte                                                          pool_Count;
  TaskThreadPoolInfoTAG*                                        pool_List;
};
#endif // TASK_THREAD_POOL_MANAGER_USE_TEMPLATE
#endif // _CONF_TASK_THREAD_POOL_MANAGER_ENABLED

#endif // !_TASK_THREAD_POOL_MANAGER_H