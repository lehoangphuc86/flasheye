#include "DataManagerConstant.h"

#if (_CONF_DATA_ITEM_ENABLED)
#ifndef _DATA_ITEM_H
#define _DATA_ITEM_H

/////////////////////////////////////////////////
// INCLUDE
#if (!_CONF_DATA_MANAGER_CONSTANT_ENABLED)
#error Data manager constant is required
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

/*DataItem*/
class DataItem
{
public:
  DataItem(byte dataItemType, DataCount_t sequenceId, PoolId_t poolId);
  virtual ~DataItem(void);
  virtual bool                                                  isValid(void) = 0; // object is valid for further operation
  virtual int                                                   reset(void) = 0; // reset data for the next use
  DataCount_t                                                   id(void);
  PoolId_t                                                      poolId(void);
  //void                                                          poolId(PoolId_t poolId);
  //void                                                          resetPoolId(void);
protected:
  virtual int                                                   clear(void); // clear data
 
protected:
  // save memory
  //byte                                                          data_Type_Id; // to identify the inherited children types
  DataCount_t                                                   sequence_Id; // to identify this data item
  PoolId_t                                                      pool_Id;
};

/*BufferDataItem*/
class BufferDataItem
  : public DataItem
{
public:
  BufferDataItem(byte dataItemType, DataCount_t sequenceId, PoolId_t poolId);
  BufferDataItem(DataCount_t sequenceId, PoolId_t poolId);
  virtual ~BufferDataItem(void);
  virtual bool                                                  isValid(void) override;
  DataSize_t                                                    dataLength(void);
  bool                                                          hasData(void);
  DataSize_t                                                    bufferLength(void);
  unsigned char*                                                bufferAddress(void);
  virtual int                                                   prepare(BufferDataItemConfigTAG& initStruct);
  int                                                           setData(DataSize_t srcDataSize, unsigned char* srcData);
  int                                                           setDataLen(DataSize_t dataSize);
  virtual int                                                   reset(void) override; // reset data for the next use
protected:
  virtual int                                                   clear(void) override; // clear buffer

protected:
  DataSize_t                                                    data_Length;  // length of holding data
  DataSize_t                                                    buffer_Length; // length of whole buffer
  unsigned char*                                                buffer_Address; // buffer address
};

#endif // _DATA_ITEM_H

#endif // _CONF_DATA_ITEM_ENABLED