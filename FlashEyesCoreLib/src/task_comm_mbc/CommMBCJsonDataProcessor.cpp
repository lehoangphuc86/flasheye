/////////////////////////////////////////////////
// INCLUDE
#include "CommMBCJsonDataProcessor.h"
#if (_CONF_COMM_MBC_JSON_DATA_PROCESSOR_ENABLED)
/////////////////////////////////////////////////
// PREPROCESSOR
#define COMM_MBC_JSON_PROCESSOR_CONSOLE_DEBUG_ENABLE
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
typedef struct __ATTRIBUTE_ALIGN _commMBCJsonEncodedHeaderTag
{
  char reserved[28] = "{\"h\":{\"id\":256,\"sz\":65535},";
} CommMBCJsonEncodedHeaderTAG;
/////////////////////////////////////////////////
// STATIC DATA
#ifdef COMM_MBC_JSON_PROCESSOR_CONSOLE_DEBUG_ENABLE
char commMbcJsonProcessorLogBuf[SYSTEM_CONSOLE_OUT_BUF_LEN];
#endif // COMM_MBC_JSON_PROCESSOR_CONSOLE_DEBUG_ENABLE
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

MbcDataSize_t CommMBCJsonDataProcessor::encodedHeaderSize(void)
{
  return sizeof(CommMBCJsonEncodedHeaderTAG);
}

MbcDataSize_t CommMBCJsonDataProcessor::encodedBodyStartPoint(bool isHeadless)
{
  return 0; // from json, whole message is required to decode.
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
      maxSize = this->encodedHeaderSize() + sizeof(CommMBCStart1TAG) + COMM_MBC_NAME_MAX_LEN + COMM_MBC_JSON_MAKEUP_LEN_START1;
      break;
    case (MbcMessageId_t)CommMBCConstant::CommMBCMessageId::CommMBCResult1:
      maxSize = this->encodedHeaderSize() + sizeof(CommMBCResult1TAG) + COMM_MBC_NAME_MAX_LEN + COMM_MBC_JSON_MAKEUP_LEN_RESULT1;
      break;
    case (MbcMessageId_t)CommMBCConstant::CommMBCMessageId::CommMBCSystemSetting:
      maxSize = this->encodedHeaderSize() + sizeof(CommMBCSystemSettingTAG) + COMM_MBC_SYSTEM_SETTING_DATA_MAX_LEN + COMM_MBC_JSON_MAKEUP_LEN_SYSTEM_SETTING;
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

int CommMBCJsonDataProcessor::encodeHeader(CommMBCHeaderTAG& inMbcHeader, unsigned char* outputBuffer, MbcDataSize_t outputSize, MbcDataSize_t& outputUsedSize)
{
  outputUsedSize = 0;
//#ifdef COMM_MBC_JSON_PROCESSOR_CONSOLE_DEBUG_ENABLE
//  CONSOLE_LOG_BUF(commMbcJsonProcessorLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mJP] enH %i %i %i", 0, inMbcHeader.messageId, inMbcHeader.dataLen);
//#endif // COMM_MBC_JSON_PROCESSOR_CONSOLE_DEBUG_ENABLE
  do
  {
    if ((outputBuffer == NULL)
      || (outputSize < this->encodedHeaderSize())
      )
    {
      break;
    }
    memset(outputBuffer, 0, outputSize);
    outputUsedSize = JsonSnprintf((char*)outputBuffer, outputSize,
      "{\"h\":{\"id\":%d,\"sz\":%d},"
      , inMbcHeader.messageId
      , inMbcHeader.dataLen
    );

//#ifdef COMM_MBC_JSON_PROCESSOR_CONSOLE_DEBUG_ENABLE
//    CONSOLE_LOG_BUF(commMbcJsonProcessorLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mJP] enH %i %i %i", 1, usedSize, outputSize);
//#endif // COMM_MBC_JSON_PROCESSOR_CONSOLE_DEBUG_ENABLE
    if ((outputUsedSize < 0)
      || (outputUsedSize >= outputSize)
      )
    {
      break;
    }
//#ifdef COMM_MBC_JSON_PROCESSOR_CONSOLE_DEBUG_ENABLE
//    CONSOLE_LOG_BUF(commMbcJsonProcessorLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mJP] enH %i", 99);
//#endif // COMM_MBC_JSON_PROCESSOR_CONSOLE_DEBUG_ENABLE
    return 0;
  } while (0);

//#ifdef COMM_MBC_JSON_PROCESSOR_CONSOLE_DEBUG_ENABLE
//  CONSOLE_LOG_BUF(commMbcJsonProcessorLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mJP] enH %i", -99);
//#endif // COMM_MBC_JSON_PROCESSOR_CONSOLE_DEBUG_ENABLE
  return -1;
}


int CommMBCJsonDataProcessor::encodeRawData(unsigned char* inputBuffer, MbcDataSize_t inputSize, unsigned char* outputBuffer, MbcDataSize_t outputSize, MbcDataSize_t& outputUsedSize)
{
  do
  {
    if (inputSize <= 0)
    {
      if ( (outputSize < 2)
        || (outputBuffer == NULL)
        )
      {
        return 0;
      }
      
      JsonSnprintf((char*)outputBuffer, outputSize, "%s", "{}");
      return 0;
    }

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
    outputUsedSize = JsonSnprintf((char*)outputBuffer, outputSize,
        "\"b\":{\"p1\":%d,\"p2\":%d,\"nLen\":%d,\"name\":%Q}}"
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
    outputUsedSize = JsonSnprintf((char*)outputBuffer, outputSize,
        "\"b\":{\"sta\":%d,\"nLen\":%d,\"name\":%Q}}"
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
//#ifdef COMM_MBC_JSON_PROCESSOR_CONSOLE_DEBUG_ENABLE
//  CONSOLE_LOG_BUF(commMbcJsonProcessorLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mJP] enSS %i", 0);
//#endif // COMM_MBC_JSON_PROCESSOR_CONSOLE_DEBUG_ENABLE
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
    int printedLen = JsonSnprintf((char*)outputBuffer, outputSize,
      "\"b\":{\"secId\":%d,\"setId\":%d,\"bSet1\":%d,\"eCode\":%d,\"sLen\":%d,"
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
    printedLen = 0;

//#ifdef COMM_MBC_JSON_PROCESSOR_CONSOLE_DEBUG_ENABLE
//    CONSOLE_LOG_BUF(commMbcJsonProcessorLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mJP] enSS %i %i %i", 30, inputTag->bitSet1, inputTag->bitSet1.dataType);
//#endif // COMM_MBC_JSON_PROCESSOR_CONSOLE_DEBUG_ENABLE
    switch (inputTag->bitSet1.dataType)
    {
      case COMM_MBC_DATA_T_DOUBLE:
        printedLen = JsonSnprintf((char*)outputBuffer, outputSize, "\"data\":%g}}", inputTag->data.dVal);
        break;
      case COMM_MBC_DATA_T_STRING:
        printedLen = JsonSnprintf((char*)outputBuffer, outputSize, "\"data\":%Q}}", inputTag->data.sVal);
        break;
      default:
        break;
    }

//#ifdef COMM_MBC_JSON_PROCESSOR_CONSOLE_DEBUG_ENABLE
//    CONSOLE_LOG_BUF(commMbcJsonProcessorLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mJP] enSS %i %i", 40, printedLen);
//    CONSOLE_LOG_BUF(commMbcJsonProcessorLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mJP] %s", outputBuffer);
//#endif // COMM_MBC_JSON_PROCESSOR_CONSOLE_DEBUG_ENABLE
    if ((printedLen >= outputSize)
      || (printedLen < 0)
      )
    {
      break;
    }
    outputUsedSize += printedLen;
//#ifdef COMM_MBC_JSON_PROCESSOR_CONSOLE_DEBUG_ENABLE
//    CONSOLE_LOG_BUF(commMbcJsonProcessorLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mJP] enSS %i", 99);
//#endif // COMM_MBC_JSON_PROCESSOR_CONSOLE_DEBUG_ENABLE
    return 0;
  } while (0);
//#ifdef COMM_MBC_JSON_PROCESSOR_CONSOLE_DEBUG_ENABLE
//  CONSOLE_LOG_BUF(commMbcJsonProcessorLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mJP] enSS %i", -99);
//#endif // COMM_MBC_JSON_PROCESSOR_CONSOLE_DEBUG_ENABLE
  return -1;
}


int CommMBCJsonDataProcessor::decodeHeader(unsigned char* inputBuffer, MbcDataSize_t inputSize, CommMBCHeaderTAG& mbcHeader)
{
  do
  {
    int ret = 0;
    if ((inputBuffer == NULL)
      || (inputSize < this->encodedHeaderSize())
      )
    {
      break;
    }

    ret = this->js_Parser.begin((char*)inputBuffer, inputSize);
    if (ret != 0)
    {
      break;
    }

    ret |= this->js_Parser.getNumber("$.h.id", mbcHeader.messageId);
    ret |= this->js_Parser.getNumber("$.h.sz", mbcHeader.dataLen);
    if (ret != 0)
    {
      break;
    }

    this->js_Parser.end();
    return 0;
  } while (0);
  this->js_Parser.end();
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
    CommMBCStart1TAG* outputTag = (CommMBCStart1TAG*)outputCurrentBuffer;
    outputCurrentBuffer += sizeof(CommMBCStart1TAG);
    ret = this->js_Parser.begin((char*)inputBuffer, inputSize);
    if (ret != 0)
    {
      break;
    }

    ret |= this->js_Parser.getNumber("$.b.p1", outputTag->param1);
    ret |= this->js_Parser.getNumber("$.b.p2", outputTag->param2);
    ret |= this->js_Parser.getNumber("$.b.nLen", outputTag->nameLen);
    if (ret != 0)
    {
      break;
    }

    outputTag->name = outputCurrentBuffer;
    DataSize_t tmpNameLen = 0;
    ret |= this->js_Parser.getString("$.b.name", (char*)outputTag->name, COMM_MBC_NAME_MAX_LEN, tmpNameLen);
    if (ret != 0)
    {
      break;
    }
    outputTag->nameLen = (uint8_t)tmpNameLen;
    outputCurrentBuffer += outputTag->nameLen;
    outputUsedSize = (MbcDataSize_t)(outputCurrentBuffer - outputBuffer);
    this->js_Parser.end();
    return 0;
  } while (0);
  this->js_Parser.end();
  return -1;
}

int CommMBCJsonDataProcessor::decodeResult1(unsigned char* inputBuffer, MbcDataSize_t inputSize, unsigned char* outputBuffer, MbcDataSize_t outputSize, MbcDataSize_t& outputUsedSize)
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
    CommMBCResult1TAG* outputTag = (CommMBCResult1TAG*)outputCurrentBuffer;
    outputCurrentBuffer += sizeof(CommMBCResult1TAG);
    ret = this->js_Parser.begin((char*)inputBuffer, inputSize);
    if (ret != 0)
    {
      break;
    }

    ret |= this->js_Parser.getNumber("$.b.sta", outputTag->status);
    ret |= this->js_Parser.getNumber("$.b.nLen", outputTag->nameLen);
    if (ret != 0)
    {
      break;
    }

    outputTag->name = outputCurrentBuffer;
    DataSize_t tmpNameLen = 0;
    ret |= this->js_Parser.getString("$.b.name", (char*)outputTag->name, COMM_MBC_NAME_MAX_LEN, tmpNameLen);
    if (ret != 0)
    {
      break;
    }
    outputTag->nameLen = (uint8_t)tmpNameLen;
    outputCurrentBuffer += outputTag->nameLen;
    outputUsedSize = (MbcDataSize_t)(outputCurrentBuffer - outputBuffer);
    this->js_Parser.end();
    return 0;
  } while (0);
  this->js_Parser.end();
  return -1;
}

int CommMBCJsonDataProcessor::decodeSystemSetting(unsigned char* inputBuffer, MbcDataSize_t inputSize, unsigned char* outputBuffer, MbcDataSize_t outputSize, MbcDataSize_t& outputUsedSize)
{
#ifdef COMM_MBC_JSON_PROCESSOR_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(commMbcJsonProcessorLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mJP] dSS %i", 0);
#endif // COMM_MBC_JSON_PROCESSOR_CONSOLE_DEBUG_ENABLE
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
//#ifdef COMM_MBC_JSON_PROCESSOR_CONSOLE_DEBUG_ENABLE
//    CONSOLE_LOG_BUF(commMbcJsonProcessorLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mJP] dSS %i %i", 2, ret);
//#endif // COMM_MBC_JSON_PROCESSOR_CONSOLE_DEBUG_ENABLE
    if (ret != 0)
    {
      break;
    }

    byte bitSet = 0;
    ret |= this->js_Parser.getNumber("$.b.secId", outputTag->sectionId);
    ret |= this->js_Parser.getNumber("$.b.setId", outputTag->settingId);
    ret |= this->js_Parser.getNumber("$.b.bSet1", bitSet);
    ret |= this->js_Parser.getNumber("$.b.eCode", outputTag->errorCode);
    ret |= this->js_Parser.getNumber("$.b.sLen", outputTag->sLen);

//#ifdef COMM_MBC_JSON_PROCESSOR_CONSOLE_DEBUG_ENABLE
//    CONSOLE_LOG_BUF(commMbcJsonProcessorLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mJP] dSS %i %i", 30, ret);
//#endif // COMM_MBC_JSON_PROCESSOR_CONSOLE_DEBUG_ENABLE
    if (ret != 0)
    {
      break;
    }

    memcpy(&outputTag->bitSet1, &bitSet, sizeof(outputTag->bitSet1));

    outputTag->data.sVal = (char*)outputCurrentBuffer;
//#ifdef COMM_MBC_JSON_PROCESSOR_CONSOLE_DEBUG_ENABLE
//    CONSOLE_LOG_BUF(commMbcJsonProcessorLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mJP] dSS %i %i %i", 40, outputTag->bitSet1, outputTag->bitSet1.dataType);
//#endif // COMM_MBC_JSON_PROCESSOR_CONSOLE_DEBUG_ENABLE
    switch (outputTag->bitSet1.dataType)
    {
      case COMM_MBC_DATA_T_DOUBLE:
        ret |= this->js_Parser.getNumber("$.b.data", outputTag->data.dVal);
        break;
      case COMM_MBC_DATA_T_STRING:
        ret |= this->js_Parser.getString("$.b.data", outputTag->data.sVal, COMM_MBC_SYSTEM_SETTING_DATA_MAX_LEN, outputTag->sLen);
        break;
      default:
        ret = -1;
        break;
    }

//#ifdef COMM_MBC_JSON_PROCESSOR_CONSOLE_DEBUG_ENABLE
//    CONSOLE_LOG_BUF(commMbcJsonProcessorLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mJP] dSS %i %i", 70, ret);
//#endif // COMM_MBC_JSON_PROCESSOR_CONSOLE_DEBUG_ENABLE
    if (ret != 0)
    {
      break;
    }

    outputCurrentBuffer += outputTag->sLen;
    outputUsedSize = (MbcDataSize_t)(outputCurrentBuffer - outputBuffer);
    this->js_Parser.end();
//#ifdef COMM_MBC_JSON_PROCESSOR_CONSOLE_DEBUG_ENABLE
//    CONSOLE_LOG_BUF(commMbcJsonProcessorLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mJP] dSS %i", 99, outputUsedSize);
//#endif // COMM_MBC_JSON_PROCESSOR_CONSOLE_DEBUG_ENABLE
    return 0;
  } while (0);
  this->js_Parser.end();
//#ifdef COMM_MBC_JSON_PROCESSOR_CONSOLE_DEBUG_ENABLE
//  CONSOLE_LOG_BUF(commMbcJsonProcessorLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mJP] dSS %i", -99);
//#endif // COMM_MBC_JSON_PROCESSOR_CONSOLE_DEBUG_ENABLE
  return -1;
}

#endif // _CONF_COMM_MBC_JSON_DATA_PROCESSOR_ENABLED