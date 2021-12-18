
/////////////////////////////////////////////////
// INCLUDE
#include "BufferDataManager.h"

#if (_CONF_BUFFER_DATA_MANAGER_ENABLED)

/////////////////////////////////////////////////
// PREPROCESSOR
#define BUFFER_DATA_MANAGER_DEBUG_CONSOLE_ENABLE
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
/*BufferDataManager*/
BufferDataManager::BufferDataManager(PoolId_t poolId)
  : DataManager(poolId)
{
  BufferDataPoolManager::getInstance();// to keep singleton construction/deconstruction in order
}

BufferDataManager::~BufferDataManager(void)
{
#ifdef BUFFER_DATA_MANAGER_DEBUG_CONSOLE_ENABLE
  //CONSOLE_LOG("[bDM] ~ %i %i", 1, this->pool_Id);
#endif // BUFFER_DATA_MANAGER_DEBUG_CONSOLE_ENABLE
  this->clear();
#ifdef BUFFER_DATA_MANAGER_DEBUG_CONSOLE_ENABLE
  //CONSOLE_LOG("[bDM] ~ %i %i", 2, this->pool_Id);
#endif // BUFFER_DATA_MANAGER_DEBUG_CONSOLE_ENABLE
}

int BufferDataManager::prepare(BufferDataManagerConfigTAG& poolConfig)
{
  do
  {
    if ((poolConfig.preparedDataNumber <= 0)
      || (poolConfig.dataItemConfig.bufferSize <= 0)
      )
    {
      break;
    }

    return DataManager::prepare<BufferDataItem, BufferDataItemConfigTAG, BufferDataManagerPropTAG, BufferDataManager>(poolConfig);
  } while (0);

  return -1;
}

void BufferDataManager::clear(void)
{
  DataManager::clearData<BufferDataItem, BufferDataItemConfigTAG, BufferDataManagerPropTAG, BufferDataManager>();
}

bool BufferDataManager::isReadyQueueEmpty(void)
{
  return DataManager::isReadyQueueEmpty();
}

DataCount_t BufferDataManager::readyQueueSize(void)
{
  return DataManager::readyQueueSize();
}

BufferDataItem* BufferDataManager::get(void)
{
  do
  {
    DataItem* dataItem = DataManager::get();
    if (dataItem == NULL)
    {
      break;
    }
    return (BufferDataItem*)dataItem;
  } while (0);
  return NULL;
}

int BufferDataManager::release(BufferDataItem* bufferDataItem)
{
  do
  {
    if (bufferDataItem == NULL)
    {
      break;
    }

    return DataManager::release(bufferDataItem);
  } while (0);
  return -1;
}



BufferDataItem* BufferDataManager::getFromISR(void)
{
  do
  {
    DataItem* dataItem = DataManager::getFromISR();
    if (dataItem == NULL)
    {
      break;
    }
    return (BufferDataItem*)dataItem;
  } while (0);
  return NULL;
}

int BufferDataManager::releaseFromISR(BufferDataItem* bufferDataItem)
{
  do
  {
    if (bufferDataItem == NULL)
    {
      break;
    }

    return DataManager::releaseFromISR(bufferDataItem);
  } while (0);
  return -1;
}

#endif // _CONF_BUFFER_DATA_MANAGER_ENABLED
