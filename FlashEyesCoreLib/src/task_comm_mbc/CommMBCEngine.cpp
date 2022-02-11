/////////////////////////////////////////////////
// INCLUDE
#include "CommMBCEngine.h"
#if (_CONF_COMM_MBC_ENGINE_ENABLED)
#include "CommMBCProcessorFactory.h"
/////////////////////////////////////////////////
// PREPROCESSOR
#define COMM_MBC_ENGINE_CONSOLE_DEBUG_ENABLE
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
#ifdef COMM_MBC_ENGINE_CONSOLE_DEBUG_ENABLE
char commMbcEngineLogBuf[SYSTEM_CONSOLE_OUT_BUF_LEN];
#endif // COMM_MBC_ENGINE_CONSOLE_DEBUG_ENABLE
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

bool CommMBCEngine::isHeaderLess(void)
{
  return this->mbc_Setting.isHeaderless();
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
  MbcDataSize_t encodedSize = 0;
  int ret = 0;
  do
  {
    if ((outputDataItem == NULL)
      || (outputDataItem->bufferAddress() == NULL)
      )
    {
      break;
    }

    ret = this->encodeData(messageId, inputBuffer, inputSize, outputDataItem->bufferAddress(), outputDataItem->bufferLength(), encodedSize);
    if (ret != 0)
    {
      break;
    }

    ret = outputDataItem->setDataLen(encodedSize);
    if (ret != 0)
    {
      break;
    }

    return 0;
  } while (0);

  return -1;
}

int CommMBCEngine::encodeData(MbcMessageId_t messageId, unsigned char* inputBuffer, MbcDataSize_t inputSize, unsigned char* outBuffer, MbcDataSize_t outSize, MbcDataSize_t& encodedSize)
{
  int ret = 0;
#ifdef COMM_MBC_ENGINE_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(commMbcEngineLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mE] en %i hl=%i", 0, messageId);
#endif // COMM_MBC_ENGINE_CONSOLE_DEBUG_ENABLE
  do
  {
    if (this->isValidProcessor() == false)
    {
      break;
    }

    if ((CommMBCConstant::isValidMessageId(messageId) == false)
      //|| (inputBuffer == NULL)
      )
    {
      break;
    }

    if ( (messageId != CommMBCConstant::CommMBCMessageId::CommMBCHeadLess)
      && ((outBuffer == NULL)
      || (outSize < sizeof(CommMBCHeaderTAG))
        )
      )
    {
      break;
    }

    MbcDataSize_t outputBodyUsedSize = 0;
    MbcDataSize_t mbcHeaderSize = 0;
    unsigned char* mbcEncodedHeaderBuf = NULL;
    encodedSize = 0;
    if (messageId == (MbcMessageId_t)CommMBCConstant::CommMBCMessageId::CommMBCHeadLess)
    {
      mbcHeaderSize = 0;
    }
    else
    {
      mbcEncodedHeaderBuf = outBuffer;
      mbcHeaderSize = this->mbc_Processor->encodedHeaderSize();
    }

    unsigned char* bodyBuffer = outBuffer + mbcHeaderSize;
    MbcDataSize_t bodyBufferSize = (MbcDataSize_t)(outSize - mbcHeaderSize);
    
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
      case (MbcMessageId_t)CommMBCConstant::CommMBCMessageId::CommMBCScanningControl:
        ret = this->mbc_Processor->encodeScanningControl(inputBuffer, inputSize, bodyBuffer, bodyBufferSize, outputBodyUsedSize);
        break;
      case (MbcMessageId_t)CommMBCConstant::CommMBCMessageId::CommMBCScanningResult:
        ret = this->mbc_Processor->encodeScanningResult(inputBuffer, inputSize, bodyBuffer, bodyBufferSize, outputBodyUsedSize);
        break;
      default:
        ret = -1;
        break;
    }

//#ifdef COMM_MBC_ENGINE_CONSOLE_DEBUG_ENABLE
//    CONSOLE_LOG_BUF(commMbcEngineLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mE] en %i %i", 50, ret);
//#endif // COMM_MBC_ENGINE_CONSOLE_DEBUG_ENABLE
    if (ret != 0)
    {
      break;
    }

    if (mbcEncodedHeaderBuf != NULL)
    {
      CommMBCHeaderTAG mbcHeader = CommMBCHeaderTAG();
      mbcHeader.messageId = messageId;
      mbcHeader.dataLen = outputBodyUsedSize;
      MbcDataSize_t usedHeaderSize = 0;
      ret = this->mbc_Processor->encodeHeader(mbcHeader, mbcEncodedHeaderBuf, this->mbc_Processor->encodedHeaderSize(), usedHeaderSize);
//#ifdef COMM_MBC_ENGINE_CONSOLE_DEBUG_ENABLE
//      CONSOLE_LOG_BUF(commMbcEngineLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mE] en %i %i", 81, ret);
//#endif // COMM_MBC_ENGINE_CONSOLE_DEBUG_ENABLE
      if (ret != 0)
      {
        break;
      }
    }

    encodedSize = (DataSize_t)(mbcHeaderSize + outputBodyUsedSize);
#ifdef COMM_MBC_ENGINE_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(commMbcEngineLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mE] en %i", 99);
#endif // COMM_MBC_ENGINE_CONSOLE_DEBUG_ENABLE
    return 0;
  } while (0);
#ifdef COMM_MBC_ENGINE_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(commMbcEngineLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mE] en %i", -99);
#endif // COMM_MBC_ENGINE_CONSOLE_DEBUG_ENABLE
  return -1;
}

int CommMBCEngine::decodeData(bool isHeadless, unsigned char* inputBuffer, MbcDataSize_t inputSize, MbcMessageId_t& outMessageId, BufferDataItem* outputDataItem)
{
  int ret = 0;
  MbcDataSize_t decodedSize = 0;
  do
  {
    if ((outputDataItem == NULL)
      || (outputDataItem->bufferAddress() == NULL)
      )
    {
      break;
    }

    ret = this->decodeData(isHeadless, inputBuffer, inputSize, outMessageId, outputDataItem->bufferAddress(), outputDataItem->bufferLength(), decodedSize);
    if (ret != 0)
    {
      break;
    }

    ret = outputDataItem->setDataLen(decodedSize);
    if (ret != 0)
    {     
      break;
    }    
    return 0;
  } while (0);  
  return -1;
}

int CommMBCEngine::decodeData(bool isHeadless, unsigned char* inputBuffer, MbcDataSize_t inputSize, MbcMessageId_t& outMessageId, unsigned char* outBuffer, MbcDataSize_t outSize, MbcDataSize_t& decodedSize)
{
  int ret = 0;
  MbcDataSize_t mbcHeaderSize = 0;
  MbcDataSize_t mbcBodySize = 0;
#ifdef COMM_MBC_ENGINE_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(commMbcEngineLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mE] de %i hl=%i", 0, isHeadless);
#endif // COMM_MBC_ENGINE_CONSOLE_DEBUG_ENABLE
  do
  {
    if (this->isValidProcessor() == false)
    {
      break;
    }

    if ((inputBuffer == NULL)
      || (outBuffer == NULL)
      )
    {
      break;
    }

    if (isHeadless != false)
    {
      mbcHeaderSize = 0;
      outMessageId = (MbcMessageId_t)CommMBCConstant::CommMBCMessageId::CommMBCHeadLess;
      mbcBodySize = inputSize;
    }
    else
    {
      mbcHeaderSize = this->mbc_Processor->encodedHeaderSize();
      CommMBCHeaderTAG mbcHeader = CommMBCHeaderTAG();
      ret = this->mbc_Processor->decodeHeader(inputBuffer, mbcHeaderSize, mbcHeader);
      if (ret != 0)
      {
        break;
      }
      outMessageId = mbcHeader.messageId;
      mbcBodySize = mbcHeader.dataLen;
    }
    decodedSize = 0;
    unsigned char* inputbodyBuffer = inputBuffer + this->mbc_Processor->encodedBodyStartPoint(isHeadless);
    MbcDataSize_t inputBodyBufferSize = (MbcDataSize_t)(inputSize - this->mbc_Processor->encodedBodyStartPoint(isHeadless));
//#ifdef COMM_MBC_ENGINE_CONSOLE_DEBUG_ENABLE
//    CONSOLE_LOG_BUF(commMbcEngineLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mE] de %i %i %i", 50, outMessageId, mbcBodySize);
//#endif // COMM_MBC_ENGINE_CONSOLE_DEBUG_ENABLE
    switch (outMessageId)
    {
      case (MbcMessageId_t)CommMBCConstant::CommMBCMessageId::CommMBCHeadLess:
        ret = this->mbc_Processor->decodeRawData(inputbodyBuffer, inputBodyBufferSize, outBuffer, outSize, decodedSize);
        break;
      case (MbcMessageId_t)CommMBCConstant::CommMBCMessageId::CommMBCStart1:
        ret = this->mbc_Processor->decodeStart1(inputbodyBuffer, inputBodyBufferSize, outBuffer, outSize, decodedSize);
        break;
      case (MbcMessageId_t)CommMBCConstant::CommMBCMessageId::CommMBCResult1:
        ret = this->mbc_Processor->decodeResult1(inputbodyBuffer, inputBodyBufferSize, outBuffer, outSize, decodedSize);
        break;
      case (MbcMessageId_t)CommMBCConstant::CommMBCMessageId::CommMBCSystemSetting:
        ret = this->mbc_Processor->decodeSystemSetting(inputbodyBuffer, inputBodyBufferSize, outBuffer, outSize, decodedSize);
        break;
      case (MbcMessageId_t)CommMBCConstant::CommMBCMessageId::CommMBCScanningControl:
        ret = this->mbc_Processor->decodeScanningControl(inputbodyBuffer, inputBodyBufferSize, outBuffer, outSize, decodedSize);
        break;
      case (MbcMessageId_t)CommMBCConstant::CommMBCMessageId::CommMBCScanningResult:
        ret = this->mbc_Processor->decodeScanningResult(inputbodyBuffer, inputBodyBufferSize, outBuffer, outSize, decodedSize);
        break;
      default:
        ret = -1;
        break;
    }
//#ifdef COMM_MBC_ENGINE_CONSOLE_DEBUG_ENABLE
//    CONSOLE_LOG_BUF(commMbcEngineLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mE] de %i %i", 60, ret);
//#endif // COMM_MBC_ENGINE_CONSOLE_DEBUG_ENABLE
    if (ret != 0)
    {
      break;
    }
#ifdef COMM_MBC_ENGINE_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(commMbcEngineLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mE] de %i", 99);
#endif // COMM_MBC_ENGINE_CONSOLE_DEBUG_ENABLE
    return 0;
  } while (0);
#ifdef COMM_MBC_ENGINE_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(commMbcEngineLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mE] de %i", -99);
#endif // COMM_MBC_ENGINE_CONSOLE_DEBUG_ENABLE
  return -1;
}
#endif // _CONF_COMM_MBC_ENGINE_ENABLED