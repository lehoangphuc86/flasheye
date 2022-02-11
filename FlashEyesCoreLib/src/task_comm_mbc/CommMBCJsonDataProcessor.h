#include "CommMBCProcessor.h"

#if (_CONF_COMM_MBC_JSON_DATA_PROCESSOR_ENABLED)

#ifndef _COMM_MBC_JSON_DATA_PROCESSOR_H
#define _COMM_MBC_JSON_DATA_PROCESSOR_H

/////////////////////////////////////////////////
// INCLUDE
#include "../utility/JsonParser.h"
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
/*CommMBCJsonDataProcessor*/
class CommMBCJsonDataProcessor
  : public CommMBCProcessor
{
public:
  CommMBCJsonDataProcessor(void);
  virtual ~CommMBCJsonDataProcessor(void);
public:
  bool                                                          isValid(void) override;
  MbcDataSize_t                                                 encodedHeaderSize(void) override;
  MbcDataSize_t                                                 encodedBodyStartPoint(bool isHeadless) override;
  int                                                           setConfig(CommMbcProcessorConfigTAG& processorConfig) override;
  MbcDataSize_t                                                 getMaxEncodedSize(MbcMessageId_t messageId) override;
  MbcDataSize_t                                                 getMaxDecodedSize(MbcMessageId_t messageId) override;
  MbcDataSize_t                                                 getMaxProceededSize(MbcMessageId_t messageId) override;
  int                                                           encodeHeader(CommMBCHeaderTAG& inMbcHeader, unsigned char* outputBuffer, MbcDataSize_t outputSize, MbcDataSize_t& outputUsedSize) override;
  int                                                           encodeRawData(unsigned char* inputBuffer, MbcDataSize_t inputSize, unsigned char* outputBuffer, MbcDataSize_t outputSize, MbcDataSize_t& outputUsedSize) override;
  int                                                           encodeStart1(unsigned char* inputBuffer, MbcDataSize_t inputSize, unsigned char* outputBuffer, MbcDataSize_t outputSize, MbcDataSize_t& outputUsedSize) override;
  int                                                           encodeResult1(unsigned char* inputBuffer, MbcDataSize_t inputSize, unsigned char* outputBuffer, MbcDataSize_t outputSize, MbcDataSize_t& outputUsedSize) override;
  int                                                           encodeSystemSetting(unsigned char* inputBuffer, MbcDataSize_t inputSize, unsigned char* outputBuffer, MbcDataSize_t outputSize, MbcDataSize_t& outputUsedSize) override;
  int                                                           encodeScanningControl(unsigned char* inputBuffer, MbcDataSize_t inputSize, unsigned char* outputBuffer, MbcDataSize_t outputSize, MbcDataSize_t& outputUsedSize) override;
  int                                                           encodeScanningResult(unsigned char* inputBuffer, MbcDataSize_t inputSize, unsigned char* outputBuffer, MbcDataSize_t outputSize, MbcDataSize_t& outputUsedSize) override;

  int                                                           decodeHeader(unsigned char* inputBuffer, MbcDataSize_t inputSize, CommMBCHeaderTAG& mbcHeader) override;
  int                                                           decodeRawData(unsigned char* inputBuffer, MbcDataSize_t inputSize, unsigned char* outputBuffer, MbcDataSize_t outputSize, MbcDataSize_t& outputUsedSize) override;
  int                                                           decodeStart1(unsigned char* inputBuffer, MbcDataSize_t inputSize, unsigned char* outputBuffer, MbcDataSize_t outputSize, MbcDataSize_t& outputUsedSize) override;
  int                                                           decodeResult1(unsigned char* inputBuffer, MbcDataSize_t inputSize, unsigned char* outputBuffer, MbcDataSize_t outputSize, MbcDataSize_t& outputUsedSize) override;
  int                                                           decodeSystemSetting(unsigned char* inputBuffer, MbcDataSize_t inputSize, unsigned char* outputBuffer, MbcDataSize_t outputSize, MbcDataSize_t& outputUsedSize) override;
  int                                                           decodeScanningControl(unsigned char* inputBuffer, MbcDataSize_t inputSize, unsigned char* outputBuffer, MbcDataSize_t outputSize, MbcDataSize_t& outputUsedSize) override;
  int                                                           decodeScanningResult(unsigned char* inputBuffer, MbcDataSize_t inputSize, unsigned char* outputBuffer, MbcDataSize_t outputSize, MbcDataSize_t& outputUsedSize) override;

protected:
  JsonParser                                                    js_Parser;
};
#endif // _COMM_MBC_JSON_DATA_PROCESSOR_H

#endif // _CONF_COMM_MBC_JSON_DATA_PROCESSOR_ENABLED