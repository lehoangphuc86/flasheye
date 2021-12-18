#include "TaskCodecConstant.h"

#if (_CONF_TASK_CODEC_ENABLED)

#ifndef _TASK_CODEC_H
#define _TASK_CODEC_H

/////////////////////////////////////////////////
// INCLUDE
#include "../data_manager/BufferDataManager.h"

#ifndef _CONF_BUFFER_DATA_MANAGER_ENABLED
#error Buffer data manager is required
#endif // !_CONF_BUFFER_DATA_MANAGER_ENABLED

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
typedef struct _taskCodecConfigTAG
{
  byte codecType;
  DataSize_t maxDataSize;
} TaskCodecConfigTAG;
/////////////////////////////////////////////////
// STATIC DATA

/////////////////////////////////////////////////
// STATIC FUNCTIONS

/////////////////////////////////////////////////
// EXTERN

/////////////////////////////////////////////////
// CLASS DEFINITION

/*TaskCodec*/
class TaskCodec
{
public:
  TaskCodec(byte codecType);
  // WARNING: if inherite from this class, deconstructor must be virtual
  __ATTRIBUTE_VIRTUAL_OPTIMIZED ~TaskCodec(void);
  bool                                                          isValid(void);
  BufferDataItem*                                               getCodecData(void);
  void                                                          releaseCodecData(BufferDataItem* dataItem);
  DataSize_t                                                    encodedUnitSize(void);
  DataSize_t                                                    decodedUnitSize(void);
  DataSize_t                                                    calculateEncodeSize(DataSize_t dataSize);
  DataSize_t                                                    calculateDecodedSize(DataSize_t dataSize);
  int                                                           config(TaskCodecConfigTAG& taskCodecConfig);
  int                                                           encode(unsigned char* inData, DataSize_t inDataSize, BufferDataItem*& outDataItem);
  int                                                           decode(BufferDataItem* inDataItem, unsigned char* outBuffer, DataSize_t outBufferSize, DataSize_t& outDataSize);
  void                                                          clear(void);

protected:
  byte                                                          codec_Type;
  BufferDataManager                                             data_Manager;

public:
  static DataSize_t                                             encodedUnitSize(byte codecType);
  static DataSize_t                                             decodedUnitSize(byte codecType);
  static DataSize_t                                             calculateEncodeSize(byte codecType, DataSize_t dataSize);
  static DataSize_t                                             calculateDecodedSize(byte codecType, DataSize_t dataSize);
  static int                                                    encode(byte codecType, unsigned char* inData, DataSize_t inDataSize, unsigned char* outBuffer, DataSize_t outBufferSize, DataSize_t& outDataSize);
  static int                                                    decode(byte codecType, unsigned char* inData, DataSize_t inDataSize, unsigned char* outBuffer, DataSize_t outBufferSize, DataSize_t& outDataSize);

private:
  static int                                                    encodeHex(unsigned char* inData, DataSize_t inDataSize, unsigned char* outBuffer, DataSize_t outBufferSize);
  static int                                                    decodeHex(unsigned char* inData, DataSize_t inDataSize, unsigned char* outBuffer, DataSize_t outBufferSize);
  static int8_t                                                 hexCharToByte(const char inChar, byte& outByte);
  static int8_t                                                 byteToChar(const byte inByte, unsigned char& outChar);
};

class TaskCodecFactory
{
public:
  TaskCodecFactory(void);
  // WARNING: if inherite from this class, deconstructor must be virtual
  __ATTRIBUTE_VIRTUAL_OPTIMIZED ~TaskCodecFactory(void);

  static TaskCodec*                                             create(byte codecType, TaskCodecConfigTAG& codecConfig);
  static void                                                   release(TaskCodec*& taskCodec);
};
#endif // _TASK_CODEC_H

#endif // (_CONF_COMM_SERIAL_CODEC_ENABLED)