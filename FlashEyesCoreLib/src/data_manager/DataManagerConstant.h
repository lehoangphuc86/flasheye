#ifndef _DATA_MANAGER_CONSTANT_H
#define _DATA_MANAGER_CONSTANT_H

#include "../FlashEyesCoreLibConfig.h"

#if (_CONF_DATA_MANAGER_CONSTANT_ENABLED)
/////////////////////////////////////////////////
// INCLUDE
#include "../os_system/SystemCommon.h"
/////////////////////////////////////////////////
// PREPROCESSOR

/////////////////////////////////////////////////
// DEFINE
#define DATA_POOL_MANAGER_POOL_COUNT_MAX            255
#define DATA_POOL_MANAGER_POOL_COUNT_MIN            1
#define DATA_POOL_ID_INVALID                        DATA_POOL_MANAGER_POOL_COUNT_MAX

#define DATA_POOL_MANAGER_CANDIDATE_NUM             3
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

typedef struct _bufferDataItemConfigTAG
{
  DataSize_t bufferSize;
} BufferDataItemConfigTAG;

template<typename DataItemConfigT>
struct DataManagerConfigTAG
{
  DataCount_t preparedDataNumber;
  bool usePool;
  DataItemConfigT dataItemConfig;
};

//typedef struct _bufferDataManagerConfigTAG
//{
//  DataCount_t preparedDataNumber;
//  BufferDataItemConfigTAG dataConfig;
//} BufferDataManagerConfigTAG;


/////////////////////////////////////////////////
// STATIC DATA

/////////////////////////////////////////////////
// STATIC FUNCTIONS

/////////////////////////////////////////////////
// EXTERN

/////////////////////////////////////////////////
// CLASS DEFINITION
/*DataManagerConstant*/
class DataManagerConstant
{
public:
  enum DataItemType
  {
    InvalidDataItemType = 0
    , BufferDataItem
    , EventDataItem
    , TaskSystemThread
  };

public:
  DataManagerConstant(void) {};
  // WARNING: if inherite from this class, deconstructor must be virtual
  __ATTRIBUTE_VIRTUAL_OPTIMIZED ~DataManagerConstant(void) {};

  static bool isValidDataItemType(byte dataItemType)
  {
    switch (dataItemType)
    {
      case (byte)DataManagerConstant::DataItemType::BufferDataItem:
      case (byte)DataManagerConstant::DataItemType::EventDataItem:
      case (byte)DataManagerConstant::DataItemType::TaskSystemThread:
        return true;
      default:
        return false;
    }
  }
};

#endif // _CONF_DATA_MANAGER_CONSTANT_ENABLED

#endif // _DATA_MANAGER_CONSTANT_H

