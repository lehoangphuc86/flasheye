#include "DataManagerConstant.h"

#ifndef _BUFFER_DATA_MANAGER_H
#define _BUFFER_DATA_MANAGER_H

#if (_CONF_BUFFER_DATA_MANAGER_ENABLED)

/////////////////////////////////////////////////
// INCLUDE
#include "../data_manager/DataManager.h"
#if (!_CONF_DATA_MANAGER_ENABLED)
#error Data manager is required
#endif // _CONF_EVENT_MANAGER_ENABLED

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
typedef DataManagerConfigTAG<BufferDataItemConfigTAG> BufferDataManagerConfigTAG;
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

/*BufferDataManager*/
class BufferDataManager
  : public DataManager
{
  ////////////////////Function to call from outside ISR///////////////////////
public:
  BufferDataManager(PoolId_t poolId = DATA_POOL_ID_INVALID);
  __ATTRIBUTE_VIRTUAL_OPTIMIZED ~BufferDataManager(void);
  int                                                           prepare(BufferDataManagerConfigTAG& poolConfig);
  void                                                          clear(void) override;
  bool                                                          isReadyQueueEmpty(void);
  DataCount_t                                                   readyQueueSize(void);
  BufferDataItem*                                               get(void);
  int                                                           release(BufferDataItem* bufferDataItem);


  BufferDataItem*                                               getFromISR(void);
  int                                                           releaseFromISR(BufferDataItem* bufferDataItem);
};

#endif // _CONF_BUFFER_DATA_MANAGER_ENABLED

#endif // _BUFFER_DATA_MANAGER_H