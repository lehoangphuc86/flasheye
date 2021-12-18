#include "DataManagerConstant.h"

#ifndef _DATA_POOL_MANAGER_H
#define _DATA_POOL_MANAGER_H

#if (_CONF_DATA_POOL_MANAGER_ENABLED)

/////////////////////////////////////////////////
// INCLUDE

/////////////////////////////////////////////////
// PREPROCESSOR
//#define DATA_POOL_MANAGER_DEBUG_CONSOLE_ENABLE


#include "../data_manager/DataPoolManagerTpl.h"
#include "../data_manager/DataPoolManagerTpl.cpp"
#include "DataItem.h"

/////////////////////////////////////////////////
// DEFINE

class BufferDataManager;
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
typedef struct _bufferDataManagerPropTAG
{
  DataSize_t bufferSize;
  _bufferDataManagerPropTAG(void)
  {
    this->clear();
  }

  void clear(void)
  {
    this->bufferSize = 0;
  }

  // example of scoring function
  void set(BufferDataItemConfigTAG& config)
  {
    this->bufferSize = config.bufferSize;
  }

  int rankScoring(_bufferDataManagerPropTAG& query, RankingScore_t& score)
  {
    do
    {
      score = 0;
      if (this->bufferSize < query.bufferSize)
      {
#ifdef DATA_POOL_MANAGER_DEBUG_CONSOLE_ENABLE
        CONSOLE_LOG("[bDMp] rk %i %i", this->bufferSize, query.bufferSize);
#endif // DATA_POOL_MANAGER_DEBUG_CONSOLE_ENABLE
        break;
      }

      score = (RankingScore_t)(this->bufferSize - query.bufferSize);

      return 0;
    } while (0);

    return -1;
  }
} BufferDataManagerPropTAG;

typedef DataPoolQueryParamTAG<BufferDataManagerPropTAG> BufferDataPoolQueryParamTAG;
typedef DataPoolManagerTpl<BufferDataItem, BufferDataItemConfigTAG, BufferDataManagerPropTAG, BufferDataManager> BufferDataPoolManager;

/////////////////////////////////////////////////
// STATIC DATA

/////////////////////////////////////////////////
// STATIC FUNCTIONS

/////////////////////////////////////////////////
// EXTERN

/////////////////////////////////////////////////
// CLASS DEFINITION

#endif // _CONF_TASK_THREAD_POOL_MANAGER_ENABLED

#endif // !_CONF_DATA_POOL_MANAGER_ENABLED