#include "CommMBCConstant.h"

#if (_CONF_COMM_MBC_PROCESSOR_ENABLED)

#ifndef _COMM_MBC_PROCESSOR_H
#define _COMM_MBC_PROCESSOR_H

/////////////////////////////////////////////////
// INCLUDE
#include "CommMBCConstant.h"
#if (!_CONF_COMM_MBC_CONSTANT_ENABLED)
#error Comm MBC constant is required
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
typedef struct _commMbcPureDataProcessorConfigTag
{
  byte reserved;
} CommMbcPureDataProcessorConfigTAG;

typedef struct _commMbcJsonDataProcessorConfigTag
{
  uint16_t reserved;
} CommMbcJsonDataProcessorConfigTAG;

typedef struct _commMbcProcessorConfigTag
{
  union 
  {
    CommMbcPureDataProcessorConfigTAG pureData;
    CommMbcJsonDataProcessorConfigTAG jsonData;
  } config;

} CommMbcProcessorConfigTAG;
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
/*CommMBCProcessor*/
class CommMBCProcessor
{
public:
  CommMBCProcessor(byte processorType);
  virtual ~CommMBCProcessor(void);
public:
  virtual bool                                                  isValid(void);
  virtual MbcDataSize_t                                         encodedHeaderSize(void) = 0;
  virtual MbcDataSize_t                                         encodedBodyStartPoint(bool isHeadless) = 0;
  virtual int                                                   setConfig(CommMbcProcessorConfigTAG& processorConfig) = 0;
  virtual MbcDataSize_t                                         getMaxEncodedSize(MbcMessageId_t messageId) = 0;
  virtual MbcDataSize_t                                         getMaxDecodedSize(MbcMessageId_t messageId) = 0;
  virtual MbcDataSize_t                                         getMaxProceededSize(MbcMessageId_t messageId) = 0;
  virtual int                                                   encodeHeader(CommMBCHeaderTAG& inMbcHeader, unsigned char* outputBuffer, MbcDataSize_t outputSize, MbcDataSize_t& outputUsedSize) = 0;
  virtual int                                                   encodeRawData(unsigned char* inputBuffer, MbcDataSize_t inputSize, unsigned char* outputBuffer, MbcDataSize_t outputSize, MbcDataSize_t& outputUsedSize) = 0;
  virtual int                                                   encodeStart1(unsigned char* inputBuffer, MbcDataSize_t inputSize, unsigned char* outputBuffer, MbcDataSize_t outputSize, MbcDataSize_t& outputUsedSize) = 0;
  virtual int                                                   encodeResult1(unsigned char* inputBuffer, MbcDataSize_t inputSize, unsigned char* outputBuffer, MbcDataSize_t outputSize, MbcDataSize_t& outputUsedSize) = 0;
  virtual int                                                   encodeSystemSetting(unsigned char* inputBuffer, MbcDataSize_t inputSize, unsigned char* outputBuffer, MbcDataSize_t outputSize, MbcDataSize_t& outputUsedSize) = 0;

  virtual int                                                   decodeHeader(unsigned char* inputBuffer, MbcDataSize_t inputSize, CommMBCHeaderTAG& mbcHeader) = 0;
  virtual int                                                   decodeRawData(unsigned char* inputBuffer, MbcDataSize_t inputSize, unsigned char* outputBuffer, MbcDataSize_t outputSize, MbcDataSize_t& outputUsedSize) = 0;
  virtual int                                                   decodeStart1(unsigned char* inputBuffer, MbcDataSize_t inputSize, unsigned char* outputBuffer, MbcDataSize_t outputSize, MbcDataSize_t& outputUsedSize) = 0;
  virtual int                                                   decodeResult1(unsigned char* inputBuffer, MbcDataSize_t inputSize, unsigned char* outputBuffer, MbcDataSize_t outputSize, MbcDataSize_t& outputUsedSize) = 0;
  virtual int                                                   decodeSystemSetting(unsigned char* inputBuffer, MbcDataSize_t inputSize, unsigned char* outputBuffer, MbcDataSize_t outputSize, MbcDataSize_t& outputUsedSize) = 0;

protected:
  byte                                                          processor_Type;
};

#endif // _COMM_MBC_PROCESSOR_H

#endif // _CONF_COMM_MBC_PROCESSOR_ENABLED