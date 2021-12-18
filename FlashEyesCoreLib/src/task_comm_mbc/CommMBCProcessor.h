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
  static uint16_t                                               getMaxEncodedSize(uint8_t messageId);
  static uint16_t                                               getMaxDecodedSize(uint8_t messageId);
  static uint16_t                                               getMaxProceededSize(uint8_t messageId);
  static int                                                    encodeRawData(unsigned char* inputBuffer, uint16_t inputSize, unsigned char* outputBuffer, uint16_t outputSize, uint16_t &outputUsedSize);
  static int                                                    encodeStart1(unsigned char* inputBuffer, uint16_t inputSize, unsigned char* outputBuffer, uint16_t outputSize, uint16_t& outputUsedSize);
  static int                                                    encodeResult1(unsigned char* inputBuffer, uint16_t inputSize, unsigned char* outputBuffer, uint16_t outputSize, uint16_t& outputUsedSize);

  static int                                                    decodeRawData(unsigned char* inputBuffer, uint16_t inputSize, unsigned char* outputBuffer, uint16_t outputSize, uint16_t& outputUsedSize);
  static int                                                    decodeStart1(unsigned char* inputBuffer, uint16_t inputSize, unsigned char* outputBuffer, uint16_t outputSize, uint16_t& outputUsedSize);
  static int                                                    decodeResult1(unsigned char* inputBuffer, uint16_t inputSize, unsigned char* outputBuffer, uint16_t outputSize, uint16_t& outputUsedSize);
public:
  CommMBCProcessor(void);
  // WARNING: if inherite from this class, deconstructor must be virtual
  __ATTRIBUTE_VIRTUAL_OPTIMIZED ~CommMBCProcessor(void);
};


#endif // _COMM_MBC_PROCESSOR_H

#endif // _CONF_COMM_MBC_PROCESSOR_ENABLED