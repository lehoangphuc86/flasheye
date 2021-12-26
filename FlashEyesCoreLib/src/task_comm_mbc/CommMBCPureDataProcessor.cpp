/////////////////////////////////////////////////
// INCLUDE
#include "CommMBCPureDataProcessor.h"
#if (_CONF_COMM_MBC_PURE_DATA_PROCESSOR_ENABLED)
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
/*CommMBCPureDataProcessor*/

CommMBCPureDataProcessor::CommMBCPureDataProcessor(void)
  : CommMBCProcessor((byte)CommMBCDataType::CommMBCPureData)
{

}

CommMBCPureDataProcessor::~CommMBCPureDataProcessor(void)
{

}

int CommMBCPureDataProcessor::setConfig(CommMbcProcessorConfigTAG& processorConfig)
{
  return 0; // do nothing
}

MbcDataSize_t CommMBCPureDataProcessor::getMaxEncodedSize(MbcMessageId_t messageId)
{
  do
  {
    MbcDataSize_t maxSize = 0;

    switch (messageId)
    {
      case (MbcMessageId_t)CommMBCConstant::CommMBCMessageId::CommMBCHeadLess:
        maxSize = 0;
        break;
      case (MbcMessageId_t)CommMBCConstant::CommMBCMessageId::CommMBCStart1:
        maxSize = sizeof(CommMBCHeaderTAG) + sizeof(CommMBCStart1PlainTAG);
        break;
      case (MbcMessageId_t)CommMBCConstant::CommMBCMessageId::CommMBCResult1:
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

MbcDataSize_t CommMBCPureDataProcessor::getMaxDecodedSize(MbcMessageId_t messageId)
{
  do
  {
    MbcDataSize_t maxSize = 0;

    switch (messageId)
    {
    case (MbcMessageId_t)CommMBCConstant::CommMBCMessageId::CommMBCHeadLess:
      maxSize = 0;
      break;
    case (MbcMessageId_t)CommMBCConstant::CommMBCMessageId::CommMBCStart1:
      maxSize = sizeof(CommMBCHeaderTAG) + sizeof(CommMBCStart1TAG) + sizeof(CommMBCStart1PlainTAG);
      break;
    case (MbcMessageId_t)CommMBCConstant::CommMBCMessageId::CommMBCResult1:
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

MbcDataSize_t CommMBCPureDataProcessor::getMaxProceededSize(MbcMessageId_t messageId)
{
  return SYSTEM_MAX(this->getMaxDecodedSize(messageId), this->getMaxEncodedSize(messageId));
}

int CommMBCPureDataProcessor::encodeRawData(unsigned char* inputBuffer, MbcDataSize_t inputSize, unsigned char* outputBuffer, MbcDataSize_t outputSize, MbcDataSize_t& outputUsedSize)
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

int CommMBCPureDataProcessor::encodeStart1(unsigned char* inputBuffer, MbcDataSize_t inputSize, unsigned char* outputBuffer, MbcDataSize_t outputSize, MbcDataSize_t& outputUsedSize)
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
    outputStart1PlainTag->nameLen = SYSTEM_MIN(inputStart1Tag->nameLen, (MbcMessageId_t)COMM_MBC_NAME_MAX_LEN);
    memcpy(outputStart1PlainTag->name, inputStart1Tag->name, outputStart1PlainTag->nameLen);
    outputStart1PlainTag->name[SYSTEM_MAX(0, outputStart1PlainTag->nameLen - 1)] = COMM_MBC_STRING_TERMINATOR;

    outputUsedSize = (MbcDataSize_t) (sizeof(CommMBCStart1PlainTAG) - COMM_MBC_NAME_MAX_LEN + outputStart1PlainTag->nameLen);
    return 0;
  } while (0);

  return -1;
}

int CommMBCPureDataProcessor::encodeResult1(unsigned char* inputBuffer, MbcDataSize_t inputSize, unsigned char* outputBuffer, MbcDataSize_t outputSize, MbcDataSize_t& outputUsedSize)
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
    outputResult1Tag->nameLen = SYSTEM_MIN(inputResult1Tag->nameLen, (MbcMessageId_t)COMM_MBC_NAME_MAX_LEN);
    memcpy(outputResult1Tag->name, inputResult1Tag->name, outputResult1Tag->nameLen);
    //CONSOLE_LOG("[mbP] enR %i %s", 1, inputResult1Tag->name);
    //CONSOLE_LOG("[mbP] enR %i %s", 2, outputResult1Tag->name);
    outputResult1Tag->name[SYSTEM_MAX(0, outputResult1Tag->nameLen - 1)] = COMM_MBC_STRING_TERMINATOR;
    //CONSOLE_LOG("[mbP] enR %i %s", 3, outputResult1Tag->name);
    outputUsedSize = (MbcDataSize_t) (sizeof(CommMBCResult1PlainTAG) - COMM_MBC_NAME_MAX_LEN + outputResult1Tag->nameLen);
    return 0;
  } while (0);

  return -1;
}

int CommMBCPureDataProcessor::encodeSystemSetting(unsigned char* inputBuffer, MbcDataSize_t inputSize, unsigned char* outputBuffer, MbcDataSize_t outputSize, MbcDataSize_t& outputUsedSize)
{
  do
  {
    if ((inputBuffer == NULL)
      || (outputBuffer == NULL)
      || (inputSize < sizeof(CommMBCSystemSettingTAG))
      )
    {
      break;
    }

    CommMBCSystemSettingTAG* inputTag = (CommMBCSystemSettingTAG*)inputBuffer;

    unsigned char* curOutputBuffer = outputBuffer;
    CommMBCSystemSettingTAG* outputTag = (CommMBCSystemSettingTAG*)curOutputBuffer;
    outputTag->sectionId = inputTag->sectionId;
    outputTag->settingId = inputTag->settingId;
    outputTag->bitSet1 = inputTag->bitSet1;
    outputTag->errorCode = inputTag->errorCode;
    outputTag->sLen = inputTag->sLen;
    outputTag->data = inputTag->data;

    curOutputBuffer += sizeof(CommMBCSystemSettingTAG);
    if (outputTag->bitSet1.dataType == COMM_MBC_DATA_T_STRING)
    {
      memcpy(curOutputBuffer, inputTag->data.sVal, inputTag->sLen);
    }

    curOutputBuffer += inputTag->sLen;
    outputUsedSize = (MbcDataSize_t)(curOutputBuffer - outputBuffer);
    return 0;
  } while (0);

  return -1;
}



int CommMBCPureDataProcessor::decodeRawData(unsigned char* inputBuffer, MbcDataSize_t inputSize, unsigned char* outputBuffer, MbcDataSize_t outputSize, MbcDataSize_t& outputUsedSize)
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

int CommMBCPureDataProcessor::decodeStart1(unsigned char* inputBuffer, MbcDataSize_t inputSize, unsigned char* outputBuffer, MbcDataSize_t outputSize, MbcDataSize_t& outputUsedSize)
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
      
    outputUsedSize = (MbcDataSize_t)(outputCurrentBuffer - outputBuffer);
    return 0;
  } while (0);

  return -1;
}

int CommMBCPureDataProcessor::decodeResult1(unsigned char* inputBuffer, MbcDataSize_t inputSize, unsigned char* outputBuffer, MbcDataSize_t outputSize, MbcDataSize_t& outputUsedSize)
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

    outputUsedSize = (MbcDataSize_t)(outputCurrentBuffer - outputBuffer);
    return 0;
  } while (0);

  return -1;
}

int CommMBCPureDataProcessor::decodeSystemSetting(unsigned char* inputBuffer, MbcDataSize_t inputSize, unsigned char* outputBuffer, MbcDataSize_t outputSize, MbcDataSize_t& outputUsedSize)
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
    CommMBCSystemSettingTAG* inputTag = (CommMBCSystemSettingTAG*)inputCurrentBuffer;

    unsigned char* outputCurrentBuffer = outputBuffer;
    CommMBCSystemSettingTAG* outputTag = (CommMBCSystemSettingTAG*)outputCurrentBuffer;
    outputTag->sectionId = inputTag->sectionId;
    outputTag->settingId = inputTag->settingId;
    outputTag->bitSet1 = inputTag->bitSet1;
    outputTag->errorCode = inputTag->errorCode;
    outputTag->sLen = inputTag->sLen;
    outputTag->data = inputTag->data;

    outputCurrentBuffer += sizeof(CommMBCSystemSettingTAG);
    inputCurrentBuffer += sizeof(CommMBCSystemSettingTAG);

    if (outputTag->bitSet1.dataType == COMM_MBC_DATA_T_STRING)
    {
      memcpy(outputCurrentBuffer, inputCurrentBuffer, inputTag->sLen);
      outputTag->data.sVal = (char*)outputCurrentBuffer;
    }
    
    outputCurrentBuffer += inputTag->sLen;
    inputCurrentBuffer += inputTag->sLen;
    
    outputUsedSize = (MbcDataSize_t)(outputCurrentBuffer - outputBuffer);
    return 0;
  } while (0);

  return -1;
}
#endif // _CONF_COMM_MBC_PURE_DATA_PROCESSOR_ENABLED