/////////////////////////////////////////////////
// INCLUDE
#include "CommMBCEngine.h"
#if (_CONF_COMM_MBC_ENGINE_ENABLED)
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

/* CommMBCEngine*/
CommMBCEngine::CommMBCEngine(void)
  : max_Buffer_Size(0)
{
  
}

CommMBCEngine::~CommMBCEngine(void)
{
  this->clear();
}

bool CommMBCEngine::isValid(void)
{
  do
  {
    if (this->max_Buffer_Size <= 0)
    {
      break;
    }

    return true;
  } while (0);

  return false;
}

uint16_t CommMBCEngine::maxBufferSize(void)
{
  return this->max_Buffer_Size;
}

uint16_t CommMBCEngine::registerMessageId(uint8_t messageId, uint16_t maxRawSize)
{
  bool isHeadLess = (messageId == (uint8_t)CommMBCConstant::CommMBCMessageId::CommMBCHeadLess? true : false);
  uint16_t maxEncodedSize = (isHeadLess == false? CommMBCProcessor::getMaxEncodedSize(messageId): maxRawSize);
  uint16_t maxDecodedSize = (isHeadLess == false? CommMBCProcessor::getMaxDecodedSize(messageId): maxRawSize);
  if (maxEncodedSize > this->max_Buffer_Size)
  {
    this->max_Buffer_Size = maxEncodedSize;
  }

  if (maxDecodedSize > this->max_Buffer_Size)
  {
    this->max_Buffer_Size = maxDecodedSize;
  }

  return this->max_Buffer_Size;
}

int CommMBCEngine::setConfig(CommMBCSettingParamTAG& commMbcSettingParams)
{
  do
  {
    //if (this->isValid() != false)
    //{
    //  break; // already valid
    //}
    return this->mbc_Setting.setConfig(commMbcSettingParams);
  } while (0);
  return -1;
}

int CommMBCEngine::prepare(void)
{
  do
  {
    //if (this->isValid() != false)
    //{
    //  break; // already valid
    //}

    if (this->max_Buffer_Size <= 0)
    {
      break;
    }

    return 0;

  } while (0);
  return -1;
}

int CommMBCEngine::start(void)
{
  //int result = 0;
  do
  {
    if (this->isValid() == false)
    {
      break; // not config
    }
    
    return 0;
  } while (0);

  this->clear();
  return -1;
}


void CommMBCEngine::clear(void)
{
  return;
}

int CommMBCEngine::encodeData(uint8_t messageId, unsigned char* inputBuffer, int16_t inputSize, BufferDataItem* outputDataItem)
{
  do
  {
    if ((CommMBCConstant::isValidMessageId(messageId) == false)
      || (inputBuffer == NULL)
      )
    {
      break;
    }

    if ((outputDataItem == NULL)
      || (outputDataItem->bufferAddress() == NULL)
      )
    {
      break;
    }

    uint16_t outputBodyUsedSize = 0;
    uint16_t mbcHeaderSize = 0;
    CommMBCHeaderTAG* mbcHeader = NULL;
    if (messageId == (uint8_t)CommMBCConstant::CommMBCMessageId::CommMBCHeadLess)
    {
      mbcHeaderSize = 0;
    }
    else
    {
      mbcHeader = (CommMBCHeaderTAG*)outputDataItem->bufferAddress();
      mbcHeaderSize = sizeof(CommMBCHeaderTAG);
      //outputUsedSize += mbcHeaderSize;
    }
    
    unsigned char* bodyBuffer = outputDataItem->bufferAddress() + mbcHeaderSize;
    uint16_t bodyBufferSize = (uint16_t) (outputDataItem->bufferLength() - mbcHeaderSize);
    int ret = 0;   
    switch (messageId)
    {
    case (uint8_t)CommMBCConstant::CommMBCMessageId::CommMBCHeadLess:
      ret = CommMBCProcessor::encodeRawData(inputBuffer, inputSize, bodyBuffer, bodyBufferSize, outputBodyUsedSize);
      break;
    case (uint8_t)CommMBCConstant::CommMBCMessageId::CommMBCStart1:
      ret = CommMBCProcessor::encodeStart1(inputBuffer, inputSize, bodyBuffer, bodyBufferSize, outputBodyUsedSize);
      break;
    case (uint8_t)CommMBCConstant::CommMBCMessageId::CommMBCResult1:
      ret = CommMBCProcessor::encodeResult1(inputBuffer, inputSize, bodyBuffer, bodyBufferSize, outputBodyUsedSize);
      //CommMBCResult1PlainTAG* encodedPlain = (CommMBCResult1PlainTAG *)bodyBuffer;

      break;
    default:
      ret = -1;
      break;
    }

    if (ret != 0)
    {
      break;
    }

    if (mbcHeader != NULL)
    {
      mbcHeader->messageId = messageId;
      mbcHeader->dataLen = outputBodyUsedSize;
    }

    ret = outputDataItem->setDataLen((DataSize_t)(mbcHeaderSize + outputBodyUsedSize));
    if (ret != 0)
    {
      break;
    }

    return 0;
  } while (0);

  return -1;
}

int CommMBCEngine::decodeData(bool isHeadless, unsigned char* inputBuffer, int16_t inputSize, uint8_t& outMessageId, BufferDataItem* outputDataItem)
{
  do
  {
    //CONSOLE_LOG("[mbcE] de %i", 0);
    if (inputBuffer == NULL)
    {
      //CONSOLE_LOG("[mbcE] de %i", 1);
      break;
    }

    if ((outputDataItem == NULL)
      || (outputDataItem->bufferAddress() == NULL)
      )
    {
      //CONSOLE_LOG("[mbcE] de %i", 2);
      break;
    }

    uint16_t mbcHeaderSize = 0;

    if (isHeadless != false)
    {
      mbcHeaderSize = 0;
      outMessageId = (uint8_t)CommMBCConstant::CommMBCMessageId::CommMBCHeadLess;
    }
    else
    {
      mbcHeaderSize = sizeof(CommMBCHeaderTAG);
      CommMBCHeaderTAG* mbcHeader = (CommMBCHeaderTAG*)inputBuffer;
      outMessageId = mbcHeader->messageId;
    }
    //CONSOLE_LOG("[mbcE] de %i %i", 3, outMessageId);
    int ret = 0;
    uint16_t ouputUsedSize = 0;
    unsigned char* inputbodyBuffer = inputBuffer + mbcHeaderSize;
    uint16_t inputBodyBufferSize = (uint16_t)(inputSize - mbcHeaderSize);
    switch (outMessageId)
    {
    case (uint8_t)CommMBCConstant::CommMBCMessageId::CommMBCHeadLess:
      ret = CommMBCProcessor::decodeRawData(inputbodyBuffer, inputBodyBufferSize, outputDataItem->bufferAddress(), outputDataItem->bufferLength(), ouputUsedSize);
      break;
    case (uint8_t)CommMBCConstant::CommMBCMessageId::CommMBCStart1:
      ret = CommMBCProcessor::decodeStart1(inputbodyBuffer, inputBodyBufferSize, outputDataItem->bufferAddress(), outputDataItem->bufferLength(), ouputUsedSize);
      break;
    case (uint8_t)CommMBCConstant::CommMBCMessageId::CommMBCResult1:
      ret = CommMBCProcessor::decodeResult1(inputbodyBuffer, inputBodyBufferSize, outputDataItem->bufferAddress(), outputDataItem->bufferLength(), ouputUsedSize);
      break;
    default:
      ret = -1;
      break;
    }

    if (ret != 0)
    {
      //CONSOLE_LOG("[mbcE] de %i", 4);
      break;
    }

    ret = outputDataItem->setDataLen(ouputUsedSize);
    if (ret != 0)
    {
      //CONSOLE_LOG("[mbcE] de %i", 5);
      break;
    }
    //CONSOLE_LOG("[mbcE] de %i", 6);
    return 0;
  } while (0);
  //CONSOLE_LOG("[mbcE] de %i", -1);
  return -1;
}

#endif // _CONF_COMM_MBC_ENGINE_ENABLED