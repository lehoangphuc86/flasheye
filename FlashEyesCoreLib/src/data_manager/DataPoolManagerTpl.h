#include "DataManagerConstant.h"

#ifndef _DATA_POOL_MANAGER_TPL_H
#define _DATA_POOL_MANAGER_TPL_H

#if (_CONF_DATA_POOL_MANAGER_TPL_ENABLED)

/////////////////////////////////////////////////
// INCLUDE

#if (!_CONF_DATA_MANAGER_ENABLED)
#error Data Manager is required
#endif // _CONF_DATA_MANAGER_ENABLED
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
/// tempaltes
template<typename DataPoolPropT>
struct DataPoolQueryParamTAG
{
  DataPoolPropT poolProperties;
};

template <typename DataPoolPropT, typename DataPoolT>
struct DataPoolInfoTAG
{
  DataPoolT* pool;
  DataPoolPropT properties;
};

// structs
typedef struct _dataPoolRankingTAG
{
  byte poolId;
  RankingScore_t score;
  _dataPoolRankingTAG(void)
  {
    this->clear();
  }

  void clear(void)
  {
    this->poolId = DATA_POOL_ID_INVALID;
    this->score = 0;
  }
} DataPoolRankingTAG;

typedef struct _dataPoolRankingListTAG
{
  DataPoolRankingTAG rankings[DATA_POOL_MANAGER_CANDIDATE_NUM];
  _dataPoolRankingListTAG(void)
  {
    this->clear();
  }

  void clear(void)
  {
    for (byte wk_idx = 0; wk_idx < DATA_POOL_MANAGER_CANDIDATE_NUM; wk_idx++)
    {
      this->rankings[wk_idx].clear();
    }
  }

} DataPoolRankingListTAG;
/////////////////////////////////////////////////
// TEMPLATES


/////////////////////////////////////////////////
// STATIC DATA

/////////////////////////////////////////////////
// STATIC FUNCTIONS

/////////////////////////////////////////////////
// EXTERN

/////////////////////////////////////////////////
// CLASS DEFINITION

/*DataPoolManagerTpl*/
template<typename DataItemT, typename DataItemConfigT, typename DataPoolPropT, typename DataPoolT>
class DataPoolManagerTpl
{
  ////////////////////Function to call from outside ISR///////////////////////
public:
  static DataPoolManagerTpl& getInstance(void);
  DataPoolManagerTpl(DataPoolManagerTpl const&) = delete;
  void operator=(DataPoolManagerTpl const&) = delete;
  int                                                           prepare(byte poolCount, DataManagerConfigTAG<DataItemConfigT>* poolConfigs);
  void                                                          clear(void);
  bool                                                          isValid(void);
  DataCount_t                                                   getPoolCapacity(PoolId_t poolId);
  int                                                           request(DataPoolQueryParamTAG<DataPoolPropT>& queryParam, DataItemT*& dataItem);
  int                                                           request(PoolId_t poolId, DataItemT*& dataItem);
  int                                                           release(DataItemT* dataItem);
protected:
  int                                                           getRanking(DataPoolQueryParamTAG<DataPoolPropT>& queryParam, DataPoolRankingListTAG& rankingResult);
private:
  DataPoolManagerTpl(void);
  __ATTRIBUTE_VIRTUAL_OPTIMIZED ~DataPoolManagerTpl(void);

protected:
  byte                                                          pool_Count;
  DataPoolInfoTAG<DataPoolPropT, DataPoolT>*                    pool_List;
};

#endif // _CONF_DATA_POOL_MANAGER_TPL_ENABLED

#endif // _DATA_POOL_MANAGER_TPL_H