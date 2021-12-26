/////////////////////////////////////////////////
// INCLUDE
#include "CommMBCEngine.h"
#if (_CONF_COMM_MBC_ENGINE_ENABLED)
#include "CommMBCProcessorFactory.h"
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
CommMBCEngine::CommMBCEngine(byte dataType)
  : max_Buffer_Size(0)
  , mbc_Processor(NULL)
  , mbc_Data_Type(dataType)
{
 this->mbc_Processor = CommMBCProcessorFactory::generate(dataType);
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

bool CommMBCEngine::isValidProcessor(void)
{
  return (this->mbc_Processor == NULL ? false : true);
}

MbcDataSize_t CommMBCEngine::maxBufferSize(void)
{
  return this->max_Buffer_Size;
}

MbcDataSize_t CommMBCEngine::registerMessageId(MbcMessageId_t messageId, MbcDataSize_t maxRawSize)
{
  do
  {
    if (this->isValidProcessor() == false)
    {
      break;
    }
    bool isHeadLess = (messageId == (MbcMessageId_t)CommMBCConstant::CommMBCMessageId::CommMBCHeadLess ? true : false);
    MbcDataSize_t maxEncodedSize = (isHeadLess == false ? this->mbc_Processor->getMaxEncodedSize(messageId) : maxRawSize);
    MbcDataSize_t maxDecodedSize = (isHeadLess == false ? this->mbc_Processor->getMaxDecodedSize(messageId) : maxRawSize);
    if (maxEncodedSize > this->max_Buffer_Size)
    {
      this->max_Buffer_Size = maxEncodedSize;
    }

    if (maxDecodedSize > this->max_Buffer_Size)
    {
      this->max_Buffer_Size = maxDecodedSize;
    }

  } while (0);
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
    if (this->isValidProcessor() == false)
    {
      break;
    }
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
    if (this->isValidProcessor() == false)
    {
      break;
    }

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
    if (this->isValidProcessor() == false)
    {
      break;
    }

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
  CommMBCProcessorFactory::release(this->mbc_Processor);
  return;
}

int CommMBCEngine::encodeData(MbcMessageId_t messageId, unsigned char* inputBuffer, MbcDataSize_t inputSize, BufferDataItem* outputDataItem)
{
  do
  {
    if (this->isValidProcessor() == false)
    {
      break;
    }

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

    MbcDataSize_t outputBodyUsedSize = 0;
    MbcDataSize_t mbcHeaderSize = 0;
    CommMBCHeaderTAG* mbcHeader = NULL;
    if (messageId == (MbcMessageId_t)CommMBCConstant::CommMBCMessageId::CommMBCHeadLess)
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
    MbcDataSize_t bodyBufferSize = (MbcDataSize_t) (outputDataItem->bufferLength() - mbcHeaderSize);
    int ret = 0;   
    switch (messageId)
    {
    case (MbcMessageId_t)CommMBCConstant::CommMBCMessageId::CommMBCHeadLess:
      ret = this->mbc_Processor->encodeRawData(inputBuffer, inputSize, bodyBuffer, bodyBufferSize, outputBodyUsedSize);
      break;
    case (MbcMessageId_t)CommMBCConstant::CommMBCMessageId::CommMBCStart1:
      ret = this->mbc_Processor->encodeStart1(inputBuffer, inputSize, bodyBuffer, bodyBufferSize, outputBodyUsedSize);
      break;
    case (MbcMessageId_t)CommMBCConstant::CommMBCMessageId::CommMBCResult1:
      ret = this->mbc_Processor->encodeResult1(inputBuffer, inputSize, bodyBuffer, bodyBufferSize, outputBodyUsedSize);
      break;
    case (MbcMessageId_t)CommMBCConstant::CommMBCMessageId::CommMBCSystemSetting:
      ret = this->mbc_Processor->encodeSystemSetting(inputBuffer, inputSize, bodyBuffer, bodyBufferSize, outputBodyUsedSize);
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

int CommMBCEngine::decodeData(bool isHeadless, unsigned char* inputBuffer, MbcDataSize_t inputSize, MbcMessageId_t& outMessageId, BufferDataItem* outputDataItem)
{
  do
  {
    if (this->isValidProcessor() == false)
    {
      break;
    }

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

    MbcDataSize_t mbcHeaderSize = 0;

    if (isHeadless != false)
    {
      mbcHeaderSize = 0;
      outMessageId = (MbcMessageId_t)CommMBCConstant::CommMBCMessageId::CommMBCHeadLess;
    }
    else
    {
      mbcHeaderSize = sizeof(CommMBCHeaderTAG);
      CommMBCHeaderTAG* mbcHeader = (CommMBCHeaderTAG*)inputBuffer;
      outMessageId = mbcHeader->messageId;
    }
    //CONSOLE_LOG("[mbcE] de %i %i", 3, outMessageId);
    int ret = 0;
    MbcDataSize_t ouputUsedSize = 0;
    unsigned char* inputbodyBuffer = inputBuffer + mbcHeaderSize;
    MbcDataSize_t inputBodyBufferSize = (MbcDataSize_t)(inputSize - mbcHeaderSize);
    switch (outMessageId)
    {
    case (MbcMessageId_t)CommMBCConstant::CommMBCMessageId::CommMBCHeadLess:
      ret = this->mbc_Processor->decodeRawData(inputbodyBuffer, inputBodyBufferSize, outputDataItem->bufferAddress(), outputDataItem->bufferLength(), ouputUsedSize);
      break;
    case (MbcMessageId_t)CommMBCConstant::CommMBCMessageId::CommMBCStart1:
      ret = this->mbc_Processor->decodeStart1(inputbodyBuffer, inputBodyBufferSize, outputDataItem->bufferAddress(), outputDataItem->bufferLength(), ouputUsedSize);
      break;
    case (MbcMessageId_t)CommMBCConstant::CommMBCMessageId::CommMBCResult1:
      ret = this->mbc_Processor->decodeResult1(inputbodyBuffer, inputBodyBufferSize, outputDataItem->bufferAddress(), outputDataItem->bufferLength(), ouputUsedSize);
      break;
    case (MbcMessageId_t)CommMBCConstant::CommMBCMessageId::CommMBCSystemSetting:
      ret = this->mbc_Processor->decodeSystemSetting(inputbodyBuffer, inputBodyBufferSize, outputDataItem->bufferAddress(), outputDataItem->bufferLength(), ouputUsedSize);
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