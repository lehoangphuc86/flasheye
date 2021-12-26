#include "CommMBCConstant.h"

#if (_CONF_COMM_MBC_ENGINE_ENABLED)

#ifndef _COMM_MBC_ENGINE_H
#define _COMM_MBC_ENGINE_H

/////////////////////////////////////////////////
// INCLUDE

#include "CommMBCSetting.h"
#include "CommMBCProcessor.h"
#include "../data_manager/BufferDataManager.h"
#if (!_CONF_COMM_MBC_SETTING_ENABLED)
#error Comm MBC setting is required
#endif // _CONF_COMM_MBC_SETTING_ENABLED

#if (!_CONF_COMM_MBC_PROCESSOR_ENABLED)
#error Comm MBC processor is required
#endif // _CONF_COMM_MBC_PROCESSOR_ENABLED

#if (!_CONF_DATA_MANAGER_ENABLED)
#error Data manager is required
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

/////////////////////////////////////////////////
// STATIC DATA

/////////////////////////////////////////////////
// STATIC FUNCTIONS

/////////////////////////////////////////////////
// EXTERN

/////////////////////////////////////////////////
// CLASS DEFINITION

/*CommMBCEngine*/
class CommMBCEngine
{
public:
  CommMBCEngine(byte dataType);
  // WARNING: if inherite from this class, deconstructor must be virtual
  __ATTRIBUTE_VIRTUAL_OPTIMIZED ~CommMBCEngine(void);
  bool                                                          isValid(void);
  bool                                                          isValidProcessor(void);
  MbcDataSize_t                                                 maxBufferSize(void);
  MbcDataSize_t                                                 registerMessageId(MbcMessageId_t messageId, MbcDataSize_t maxRawSize = 0);
  int                                                           setConfig(CommMBCSettingParamTAG& commMbcSettingParams);
  int                                                           prepare(void);
  int                                                           start(void);
  void                                                          clear(void);
  int                                                           encodeData(MbcMessageId_t messageId, unsigned char* inputBuffer, MbcDataSize_t inputSize, BufferDataItem* outputDataItem);
  int                                                           decodeData(bool isHeadless, unsigned char* inputBuffer, MbcDataSize_t inputSize, MbcMessageId_t& outMessageId, BufferDataItem* outputDataItem);
  
protected:
  MbcDataSize_t                                                 max_Buffer_Size;
  CommMBCSetting                                                mbc_Setting;
  CommMBCProcessor*                                             mbc_Processor;
  byte                                                          mbc_Data_Type;
};

#endif // _COMM_MBC_ENGINE_H

#endif // _CONF_COMM_MBC_ENGINE_ENABLED