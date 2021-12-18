/////////////////////////////////////////////////
// INCLUDE
#include "CommMBCProcessor.h"
#if (_CONF_COMM_MBC_PROCESSOR_ENABLED)
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
// CLASS IMPLEMENTAION
//
/*CommMBCProcessor*/
CommMBCProcessor::CommMBCProcessor(void)
{
  
}

CommMBCProcessor::~CommMBCProcessor(void)
{

}

uint16_t CommMBCProcessor::getMaxEncodedSize(uint8_t messageId)
{
  do
  {
    uint16_t maxSize = 0;

    switch (messageId)
    {
      case (uint8_t)CommMBCConstant::CommMBCMessageId::CommMBCHeadLess:
        maxSize = 0;
        break;
      case (uint8_t)CommMBCConstant::CommMBCMessageId::CommMBCStart1:
        maxSize = sizeof(CommMBCHeaderTAG) + sizeof(CommMBCStart1PlainTAG);
        break;
      case (uint8_t)CommMBCConstant::CommMBCMessageId::CommMBCResult1:
        maxSize = sizeof(CommMBCHeaderTAG) + sizeof(CommMBCResult1PlainTAG);
        break;
      default:
        maxSize = 0;
        break;
    }

    return maxSize;
  } while (0);

  return 0;
}

uint16_t CommMBCProcessor::getMaxDecodedSize(uint8_t messageId)
{
  do
  {
    uint16_t maxSize = 0;

    switch (messageId)
    {
    case (uint8_t)CommMBCConstant::CommMBCMessageId::CommMBCHeadLess:
      maxSize = 0;
      break;
    case (uint8_t)CommMBCConstant::CommMBCMessageId::CommMBCStart1:
      maxSize = sizeof(CommMBCHeaderTAG) + sizeof(CommMBCStart1TAG) + sizeof(CommMBCStart1PlainTAG);
      break;
    case (uint8_t)CommMBCConstant::CommMBCMessageId::CommMBCResult1:
      maxSize = sizeof(CommMBCHeaderTAG) + sizeof(CommMBCResult1TAG) + sizeof(CommMBCResult1PlainTAG);
      break;
    default:
      maxSize = 0;
      break;
    }

    return maxSize;
  } while (0);

  return 0;
}

uint16_t CommMBCProcessor::getMaxProceededSize(uint8_t messageId)
{
  return SYSTEM_MAX(CommMBCProcessor::getMaxDecodedSize(messageId), CommMBCProcessor::getMaxEncodedSize(messageId));
}

int CommMBCProcessor::encodeRawData(unsigned char* inputBuffer, uint16_t inputSize, unsigned char* outputBuffer, uint16_t outputSize, uint16_t& outputUsedSize)
{
  do
  {
    if ((inputBuffer == NULL)
      || (outputBuffer == NULL)
      || (outputSize < inputSize)
      )
    {
      break;
    }

    memcpy(outputBuffer, inputBuffer, inputSize);
    outputUsedSize = inputSize;

    return 0;
  } while (0);

  return -1;
}

int CommMBCProcessor::encodeStart1(unsigned char* inputBuffer, uint16_t inputSize, unsigned char* outputBuffer, uint16_t outputSize, uint16_t& outputUsedSize)
{
  do
  {
    if ((inputBuffer == NULL)
      || (outputBuffer == NULL)
      || (inputSize < sizeof(CommMBCStart1TAG))
      )
    {
      break;
    }

    CommMBCStart1TAG* inputStart1Tag = (CommMBCStart1TAG*)inputBuffer;
    CommMBCStart1PlainTAG* outputStart1PlainTag = (CommMBCStart1PlainTAG*)outputBuffer;
    outputStart1PlainTag->param1 = inputStart1Tag->param1;
    outputStart1PlainTag->param2 = inputStart1Tag->param2;
    outputStart1PlainTag->nameLen = SYSTEM_MIN(inputStart1Tag->nameLen, (uint8_t)COMM_MBC_NAME_MAX_LEN);
    memcpy(outputStart1PlainTag->name, inputStart1Tag->name, outputStart1PlainTag->nameLen);
    outputStart1PlainTag->name[SYSTEM_MAX(0, outputStart1PlainTag->nameLen - 1)] = COMM_MBC_STRING_TERMINATOR;

    outputUsedSize = (uint16_t) (sizeof(CommMBCStart1PlainTAG) - COMM_MBC_NAME_MAX_LEN + outputStart1PlainTag->nameLen);
    return 0;
  } while (0);

  return -1;
}

int CommMBCProcessor::encodeResult1(unsigned char* inputBuffer, uint16_t inputSize, unsigned char* outputBuffer, uint16_t outputSize, uint16_t& outputUsedSize)
{
  do
  {
    if ((inputBuffer == NULL)
      || (outputBuffer == NULL)
      || (inputSize < sizeof(CommMBCResult1TAG))
      )
    {
      break;
    }

    CommMBCResult1TAG* inputResult1Tag = (CommMBCResult1TAG*)inputBuffer;
    CommMBCResult1PlainTAG* outputResult1Tag = (CommMBCResult1PlainTAG*)outputBuffer;
    outputResult1Tag->status = inputResult1Tag->status;
    outputResult1Tag->nameLen = SYSTEM_MIN(inputResult1Tag->nameLen, (uint8_t)COMM_MBC_NAME_MAX_LEN);
    memcpy(outputResult1Tag->name, inputResult1Tag->name, outputResult1Tag->nameLen);
    //CONSOLE_LOG("[mbP] enR %i %s", 1, inputResult1Tag->name);
    //CONSOLE_LOG("[mbP] enR %i %s", 2, outputResult1Tag->name);
    outputResult1Tag->name[SYSTEM_MAX(0, outputResult1Tag->nameLen - 1)] = COMM_MBC_STRING_TERMINATOR;
    //CONSOLE_LOG("[mbP] enR %i %s", 3, outputResult1Tag->name);
    outputUsedSize = (uint16_t) (sizeof(CommMBCResult1PlainTAG) - COMM_MBC_NAME_MAX_LEN + outputResult1Tag->nameLen);
    return 0;
  } while (0);

  return -1;
}

int CommMBCProcessor::decodeRawData(unsigned char* inputBuffer, uint16_t inputSize, unsigned char* outputBuffer, uint16_t outputSize, uint16_t& outputUsedSize)
{
  do
  {
    if ((inputBuffer == NULL)
      || (outputBuffer == NULL)
      || (outputSize < inputSize)
      )
    {
      break;
    }

    memcpy(outputBuffer, inputBuffer, inputSize);
    outputUsedSize = inputSize;

    return 0;
  } while (0);

  return -1;
}

int CommMBCProcessor::decodeStart1(unsigned char* inputBuffer, uint16_t inputSize, unsigned char* outputBuffer, uint16_t outputSize, uint16_t& outputUsedSize)
{
  do
  {
    if ((inputBuffer == NULL)
      || (outputBuffer == NULL)
      || (inputSize <=0)
      || (outputSize <= 0)
      )
    {
      break;
    }

    unsigned char* inputCurrentBuffer = inputBuffer;
    CommMBCStart1PlainTAG* inputStart1PlainTag = (CommMBCStart1PlainTAG*)inputCurrentBuffer;

    unsigned char* outputCurrentBuffer = outputBuffer;
    CommMBCStart1TAG* outputStart1Tag = (CommMBCStart1TAG*)outputCurrentBuffer;
    outputCurrentBuffer += sizeof(CommMBCStart1TAG);
    outputStart1Tag->param1 = inputStart1PlainTag->param1;
    outputStart1Tag->param2 = inputStart1PlainTag->param2;
    outputStart1Tag->nameLen = inputStart1PlainTag->nameLen;

    memcpy(outputCurrentBuffer, inputStart1PlainTag->name, outputStart1Tag->nameLen);
    outputStart1Tag->name = outputCurrentBuffer;

    outputCurrentBuffer += outputStart1Tag->nameLen;
      
    outputUsedSize = (uint16_t)(outputCurrentBuffer - outputBuffer);
    return 0;
  } while (0);

  return -1;
}

int CommMBCProcessor::decodeResult1(unsigned char* inputBuffer, uint16_t inputSize, unsigned char* outputBuffer, uint16_t outputSize, uint16_t& outputUsedSize)
{
  do
  {
    if ((inputBuffer == NULL)
      || (outputBuffer == NULL)
      || (inputSize <= 0)
      || (outputSize <= 0)
      )
    {
      break;
    }

    unsigned char* inputCurrentBuffer = inputBuffer;
    CommMBCResult1PlainTAG* inputResult1PlainTag = (CommMBCResult1PlainTAG*)inputCurrentBuffer;

    unsigned char* outputCurrentBuffer = outputBuffer;
    CommMBCResult1TAG* outputResult1Tag = (CommMBCResult1TAG*)outputCurrentBuffer;
    outputCurrentBuffer += sizeof(CommMBCResult1TAG);
    outputResult1Tag->status = inputResult1PlainTag->status;
    outputResult1Tag->nameLen = inputResult1PlainTag->nameLen;
    //CONSOLE_LOG("[mbP] deR %i %i %i", 1, inputSize, inputResult1PlainTag->status);
    //CONSOLE_LOG("[mbP] deR %i %i %s", 2, inputResult1PlainTag->nameLen, inputResult1PlainTag->name);
    memcpy(outputCurrentBuffer, inputResult1PlainTag->name, outputResult1Tag->nameLen);
    outputResult1Tag->name = outputCurrentBuffer;
    //CONSOLE_LOG("[mbP] deR %i %s", 2, outputCurrentBuffer);
    outputCurrentBuffer += outputResult1Tag->nameLen;

    outputUsedSize = (uint16_t)(outputCurrentBuffer - outputBuffer);
    return 0;
  } while (0);

  return -1;
}
#endif // _CONF_COMM_MBC_PROCESSOR_ENABLED