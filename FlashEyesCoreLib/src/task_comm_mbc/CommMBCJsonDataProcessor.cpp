/////////////////////////////////////////////////
// INCLUDE
#include "CommMBCJsonDataProcessor.h"
#if (_CONF_COMM_MBC_JSON_DATA_PROCESSOR_ENABLED)
/////////////////////////////////////////////////
// PREPROCESSOR

/////////////////////////////////////////////////
// DEFINE
#define COMM_MBC_JSON_MAKEUP_LEN_START1                 42
#define COMM_MBC_JSON_MAKEUP_LEN_RESULT1                42
#define COMM_MBC_JSON_MAKEUP_LEN_SYSTEM_SETTING         52

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
/*CommMBCJsonDataProcessor*/

CommMBCJsonDataProcessor::CommMBCJsonDataProcessor(void)
  : CommMBCProcessor((byte)CommMBCDataType::CommMbcJsonData)
{

}

CommMBCJsonDataProcessor::~CommMBCJsonDataProcessor(void)
{
  this->js_Parser.finialize();
}

bool CommMBCJsonDataProcessor::isValid(void)
{
  do
  {
    if (this->js_Parser.isValid() == false)
    {
      break;
    }

    return CommMBCProcessor::isValid();
  } while (0);
  return false;
}

int CommMBCJsonDataProcessor::setConfig(CommMbcProcessorConfigTAG& processorConfig)
{
  do
  {
    if (this->js_Parser.initialize() != 0)
    {
      break;
    }

    return 0;
  } while (0);
  return -1;
}

MbcDataSize_t CommMBCJsonDataProcessor::getMaxEncodedSize(MbcMessageId_t messageId)
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
      maxSize = this->getMaxDecodedSize(messageId) + COMM_MBC_JSON_MAKEUP_LEN_START1;
      break;
    case (MbcMessageId_t)CommMBCConstant::CommMBCMessageId::CommMBCResult1:
      maxSize = this->getMaxDecodedSize(messageId) + COMM_MBC_JSON_MAKEUP_LEN_RESULT1;
      break;
    case (MbcMessageId_t)CommMBCConstant::CommMBCMessageId::CommMBCSystemSetting:
      maxSize = this->getMaxDecodedSize(messageId) + COMM_MBC_JSON_MAKEUP_LEN_SYSTEM_SETTING;
      break;
    default:
      maxSize = 0;
      break;
    }

    return maxSize;
  } while (0);

  return 0;
}

MbcDataSize_t CommMBCJsonDataProcessor::getMaxDecodedSize(MbcMessageId_t messageId)
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
      maxSize = sizeof(CommMBCHeaderTAG) + sizeof(CommMBCStart1TAG) + COMM_MBC_NAME_MAX_LEN;
      break;
    case (MbcMessageId_t)CommMBCConstant::CommMBCMessageId::CommMBCResult1:
      maxSize = sizeof(CommMBCHeaderTAG) + sizeof(CommMBCResult1TAG) + COMM_MBC_NAME_MAX_LEN;
      break;
    case (MbcMessageId_t)CommMBCConstant::CommMBCMessageId::CommMBCSystemSetting:
      maxSize = sizeof(CommMBCHeaderTAG) + sizeof(CommMBCSystemSettingTAG) + COMM_MBC_SYSTEM_SETTING_DATA_MAX_LEN;
      break;
    default:
      maxSize = 0;
      break;
    }

    return maxSize;
  } while (0);

  return 0;
}

MbcDataSize_t CommMBCJsonDataProcessor::getMaxProceededSize(MbcMessageId_t messageId)
{
  return SYSTEM_MAX(this->getMaxDecodedSize(messageId), this->getMaxEncodedSize(messageId));
}

int CommMBCJsonDataProcessor::encodeRawData(unsigned char* inputBuffer, MbcDataSize_t inputSize, unsigned char* outputBuffer, MbcDataSize_t outputSize, MbcDataSize_t& outputUsedSize)
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

int CommMBCJsonDataProcessor::encodeStart1(unsigned char* inputBuffer, MbcDataSize_t inputSize, unsigned char* outputBuffer, MbcDataSize_t outputSize, MbcDataSize_t& outputUsedSize)
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
    outputUsedSize = snprintf((char*)outputBuffer, outputSize, 
      "{\
        \"p1\":%u,\
        \"p2\":%u,\
        \"nLen\":%u,\
        \"name\":\"%s\",\
       }"
      , inputStart1Tag->param1
      , inputStart1Tag->param2
      , inputStart1Tag->nameLen
      , inputStart1Tag->name
      );
    return 0;
  } while (0);

  return -1;
}

int CommMBCJsonDataProcessor::encodeResult1(unsigned char* inputBuffer, MbcDataSize_t inputSize, unsigned char* outputBuffer, MbcDataSize_t outputSize, MbcDataSize_t& outputUsedSize)
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
    outputUsedSize = snprintf((char*)outputBuffer, outputSize,
      "{\
        \"sta\":%u,\
        \"nLen\":%u,\
        \"name\":\"%s\",\
       }"
      , inputResult1Tag->status
      , inputResult1Tag->nameLen
      , inputResult1Tag->name
    );
    return 0;
  } while (0);

  return -1;
}


int CommMBCJsonDataProcessor::encodeSystemSetting(unsigned char* inputBuffer, MbcDataSize_t inputSize, unsigned char* outputBuffer, MbcDataSize_t outputSize, MbcDataSize_t& outputUsedSize)
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
    int printedLen = snprintf((char*)outputBuffer, outputSize,
      "{\
        \"secId\":%u,\
        \"setId\":%u,\
        \"bSet1\":%u,\
        \"eCode\":%u,\
        \"sLen\":%u,\
       "
      , inputTag->sectionId
      , inputTag->settingId
      , inputTag->bitSet1
      , inputTag->errorCode
      , inputTag->sLen
    );

    if ((printedLen >= outputSize)
      || (printedLen < 0)
      )
    {
      break;
    }

    outputUsedSize = printedLen;
    outputBuffer += outputUsedSize;
    outputSize -= outputUsedSize;
    MbcDataSize_t usedSize = 0;
    switch (inputTag->bitSet1.dataType)
    {
      case COMM_MBC_DATA_T_DOUBLE:
        printedLen = snprintf((char*)outputBuffer, outputSize, "\"data\":%f}", inputTag->data.dVal);
        break;
      case COMM_MBC_DATA_T_STRING:
        printedLen = snprintf((char*)outputBuffer, outputSize, "\"data\":\"%s\"}", inputTag->data.sVal);
        break;
      default:
        break;
    }

    if ((printedLen >= outputSize)
      || (printedLen < 0)
      )
    {
      break;
    }

    return 0;
  } while (0);

  return -1;
}

int CommMBCJsonDataProcessor::decodeRawData(unsigned char* inputBuffer, MbcDataSize_t inputSize, unsigned char* outputBuffer, MbcDataSize_t outputSize, MbcDataSize_t& outputUsedSize)
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

int CommMBCJsonDataProcessor::decodeStart1(unsigned char* inputBuffer, MbcDataSize_t inputSize, unsigned char* outputBuffer, MbcDataSize_t outputSize, MbcDataSize_t& outputUsedSize)
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

int CommMBCJsonDataProcessor::decodeResult1(unsigned char* inputBuffer, MbcDataSize_t inputSize, unsigned char* outputBuffer, MbcDataSize_t outputSize, MbcDataSize_t& outputUsedSize)
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
    memcpy(outputCurrentBuffer, inputResult1PlainTag->name, outputResult1Tag->nameLen);
    outputResult1Tag->name = outputCurrentBuffer;
    outputCurrentBuffer += outputResult1Tag->nameLen;

    outputUsedSize = (MbcDataSize_t)(outputCurrentBuffer - outputBuffer);
    return 0;
  } while (0);

  return -1;
}

int CommMBCJsonDataProcessor::decodeSystemSetting(unsigned char* inputBuffer, MbcDataSize_t inputSize, unsigned char* outputBuffer, MbcDataSize_t outputSize, MbcDataSize_t& outputUsedSize)
{
  do
  {
    int ret = 0;
    if ((inputBuffer == NULL)
      || (outputBuffer == NULL)
      || (inputSize <= 0)
      || (outputSize <= 0)
      )
    {
      break;
    }

    unsigned char* outputCurrentBuffer = outputBuffer;
    CommMBCSystemSettingTAG* outputTag = (CommMBCSystemSettingTAG*)outputCurrentBuffer;
    outputCurrentBuffer += sizeof(CommMBCSystemSettingTAG);
    ret = this->js_Parser.begin((char*)inputBuffer, inputSize);
    if (ret != 0)
    {
      break;
    }

    byte bitSet = 0;
    ret |= this->js_Parser.getNumber("$.secId", outputTag->sectionId);
    ret |= this->js_Parser.getNumber("$.setId", outputTag->settingId);
    ret |= this->js_Parser.getNumber("$.bSet1", bitSet);
    ret |= this->js_Parser.getNumber("$.eCode", outputTag->errorCode);
    ret |= this->js_Parser.getNumber("$.sLen", outputTag->sLen);

    if (ret != 0)
    {
      break;
    }

    memcpy(&outputTag->bitSet1, &bitSet, sizeof(outputTag->bitSet1));

    outputTag->data.sVal = (char*)outputCurrentBuffer;
    switch (outputTag->bitSet1.dataType)
    {
      case COMM_MBC_DATA_T_DOUBLE:
        ret |= this->js_Parser.getNumber("$.data", outputTag->data.dVal);
        break;
      case COMM_MBC_DATA_T_STRING:
        ret |= this->js_Parser.getString("$.data", outputTag->data.sVal, COMM_MBC_SYSTEM_SETTING_DATA_MAX_LEN, outputTag->sLen);
        break;
      default:
        ret = -1;
        break;
    }

    if (ret != 0)
    {
      break;
    }

    outputCurrentBuffer += outputTag->sLen;
    outputUsedSize = (MbcDataSize_t)(outputCurrentBuffer - outputBuffer);
    this->js_Parser.end();
    return 0;
  } while (0);
  this->js_Parser.end();
  return -1;
}

#endif // _CONF_COMM_MBC_JSON_DATA_PROCESSOR_ENABLED