
/////////////////////////////////////////////////
// INCLUDE
#include "TaskThreadManager.h"

#if (_CONF_TASK_THREAD_MANAGER_ENABLED)

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
//class TaskThreadPoolManager;
/////////////////////////////////////////////////
// STATIC DATA

/////////////////////////////////////////////////
// STATIC FUNCTIONS

/////////////////////////////////////////////////
// EXTERN

/////////////////////////////////////////////////
// CLASS IMPLEMENTAION
/*TaskThreadManager*/

TaskThreadManager::TaskThreadManager(PoolId_t poolId)
  : DataManager(poolId)
{
  TaskThreadPoolManager::getInstance();// to keep singleton construction/deconstruction in order
}

TaskThreadManager::~TaskThreadManager(void)
{
  this->clear();
}

int TaskThreadManager::prepare(TaskThreadManagerConfigTAG& poolConfig)
{
  do
  {
    if ((poolConfig.preparedDataNumber <= 0)
      || (poolConfig.dataItemConfig.isValidMem() == false)
      || (poolConfig.dataItemConfig.isValidPriority() == false)
      )
    {
      break;
    }

    return DataManager::prepare<TaskSystemThread, SystemThreadConfigTAG, TaskThreadManagerPropTAG, TaskThreadManager>(poolConfig);
  } while (0);

  return -1;
}

void TaskThreadManager::clear(void)
{
  DataManager::clearData<TaskSystemThread, SystemThreadConfigTAG, TaskThreadManagerPropTAG, TaskThreadManager>();
}

bool TaskThreadManager::isReadyQueueEmpty(void)
{
  return DataManager::isReadyQueueEmpty();
}

DataCount_t TaskThreadManager::readyQueueSize(void)
{
  return DataManager::readyQueueSize();
}

TaskSystemThread* TaskThreadManager::get(void)
{
  do
  {
    DataItem* dataItem = DataManager::get();
    if (dataItem == NULL)
    {
      break;
    }
    return (TaskSystemThread*)dataItem;
  } while (0);
  return NULL;
}

int TaskThreadManager::release(TaskSystemThread* thread)
{
  do
  {
    if (thread == NULL)
    {
      break;
    }

    //thread->reset();
    return DataManager::release(thread);
  } while (0);

  return -1;
}

#endif // _CONF_TASK_THREAD_MANAGER_ENABLED
