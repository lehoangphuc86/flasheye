#include "CommMBCProcessor.h"

#if (_CONF_COMM_MBC_PURE_DATA_PROCESSOR_ENABLED)

#ifndef _COMM_MBC_PURE_DATA_PROCESSOR_H
#define _COMM_MBC_PURE_DATA_PROCESSOR_H

/////////////////////////////////////////////////
// INCLUDE

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
/*CommMBCPureDataProcessor*/
class CommMBCPureDataProcessor
  : public CommMBCProcessor
{
public:
  CommMBCPureDataProcessor(void);
  virtual ~CommMBCPureDataProcessor(void);
public:
  int                                                           setConfig(CommMbcProcessorConfigTAG& processorConfig) override;
  MbcDataSize_t                                                 getMaxEncodedSize(MbcMessageId_t messageId) override;
  MbcDataSize_t                                                 getMaxDecodedSize(MbcMessageId_t messageId) override;
  MbcDataSize_t                                                 getMaxProceededSize(MbcMessageId_t messageId) override;
  int                                                           encodeRawData(unsigned char* inputBuffer, MbcDataSize_t inputSize, unsigned char* outputBuffer, MbcDataSize_t outputSize, MbcDataSize_t& outputUsedSize) override;
  int                                                           encodeStart1(unsigned char* inputBuffer, MbcDataSize_t inputSize, unsigned char* outputBuffer, MbcDataSize_t outputSize, MbcDataSize_t& outputUsedSize) override;
  int                                                           encodeResult1(unsigned char* inputBuffer, MbcDataSize_t inputSize, unsigned char* outputBuffer, MbcDataSize_t outputSize, MbcDataSize_t& outputUsedSize) override;
  int                                                           encodeSystemSetting(unsigned char* inputBuffer, MbcDataSize_t inputSize, unsigned char* outputBuffer, MbcDataSize_t outputSize, MbcDataSize_t& outputUsedSize) override;

  int                                                           decodeRawData(unsigned char* inputBuffer, MbcDataSize_t inputSize, unsigned char* outputBuffer, MbcDataSize_t outputSize, MbcDataSize_t& outputUsedSize) override;
  int                                                           decodeStart1(unsigned char* inputBuffer, MbcDataSize_t inputSize, unsigned char* outputBuffer, MbcDataSize_t outputSize, MbcDataSize_t& outputUsedSize) override;
  int                                                           decodeResult1(unsigned char* inputBuffer, MbcDataSize_t inputSize, unsigned char* outputBuffer, MbcDataSize_t outputSize, MbcDataSize_t& outputUsedSize) override;
  int                                                           decodeSystemSetting(unsigned char* inputBuffer, MbcDataSize_t inputSize, unsigned char* outputBuffer, MbcDataSize_t outputSize, MbcDataSize_t& outputUsedSize) override;
};
#endif // _COMM_MBC_PURE_DATA_PROCESSOR_H

#endif // _CONF_COMM_MBC_PURE_DATA_PROCESSOR_ENABLED