#include "TaskCodec.h"
#if (_CONF_TASK_CODEC_ENABLED)
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

/* TaskCodec*/
TaskCodec::TaskCodec(byte codecType)
  : codec_Type(codecType)
{
  this->clear();
}

TaskCodec::~TaskCodec(void)
{
  this->clear();
}

bool TaskCodec::isValid(void)
{
  do
  {
    if ( (this->data_Manager.isValid() == false)
      || (TaskCodecConstant::isValidCodecType(this->codec_Type) == false)
      )
    {
      break;
    }

    return true;
  } while (0);
  return false;
}

BufferDataItem* TaskCodec::getCodecData(void)
{
  return this->data_Manager.get();
}

void TaskCodec::releaseCodecData(BufferDataItem* dataItem)
{
  this->data_Manager.release(dataItem);
}

DataSize_t TaskCodec::encodedUnitSize(void)
{
  return TaskCodec::encodedUnitSize(this->codec_Type);
}

DataSize_t TaskCodec::decodedUnitSize(void)
{
  return TaskCodec::decodedUnitSize(this->codec_Type);
}

DataSize_t TaskCodec::calculateEncodeSize(DataSize_t dataSize)
{
  return TaskCodec::calculateEncodeSize(this->codec_Type, dataSize);
}

DataSize_t TaskCodec::calculateDecodedSize(DataSize_t dataSize)
{
  return TaskCodec::calculateDecodedSize(this->codec_Type, dataSize);
}

int TaskCodec::config(TaskCodecConfigTAG& taskCodecConfig)
{
  int ret = 0;
  do
  {
    if (this->isValid() != false)
    {
      return 0;
    }

    this->clear();
    if ((TaskCodecConstant::isValidCodecType(this->codec_Type) == false)
      || (taskCodecConfig.maxDataSize <= 0)
      )
    {
      break;
    }
    
    {
      DataSize_t bufferSize = (DataSize_t)(TaskCodec::calculateEncodeSize(this->codec_Type, taskCodecConfig.maxDataSize));
      BufferDataManagerConfigTAG bufferManagerConfig = BufferDataManagerConfigTAG();
      bufferManagerConfig.preparedDataNumber = 2;
      bufferManagerConfig.usePool = BufferDataPoolManager::getInstance().isValid();
      bufferManagerConfig.dataItemConfig.bufferSize = bufferSize;
      ret = this->data_Manager.prepare(bufferManagerConfig);
    }
    
    if (ret != 0)
    {
      break;
    }

    return 0;
  } while (0);
  this->clear();
  return -1;
}

int TaskCodec::encode(unsigned char* inData, DataSize_t inDataSize, BufferDataItem*& outDataItem)
{
  outDataItem = NULL;
  do
  {
    if (this->isValid() == false)
    {
      break;
    }

    outDataItem = this->getCodecData();
    if (outDataItem == NULL)
    {
      break;
    }

    DataSize_t outDataSize = 0;
    int ret = TaskCodec::encode(this->codec_Type, inData, inDataSize, outDataItem->bufferAddress(), outDataItem->bufferLength(), outDataSize);
    if (ret != 0)
    {
      break;
    }

    ret = outDataItem->setDataLen(outDataSize);
    if (ret != 0)
    {
      break;
    }
    return 0;
  } while (0);

  if (outDataItem != NULL)
  {
    this->releaseCodecData(outDataItem);
  }

  outDataItem = NULL;
  return -1;
}

int TaskCodec::decode(BufferDataItem* inDataItem, unsigned char* outBuffer, DataSize_t outBufferSize, DataSize_t& outDataSize)
{
  do
  {
    if (this->isValid() == false)
    {
      break;
    }

    if (inDataItem == NULL)
    {
      break;
    }

    int ret = TaskCodec::decode(this->codec_Type, inDataItem->bufferAddress(), inDataItem->dataLength(), outBuffer, outBufferSize, outDataSize);
    this->releaseCodecData(inDataItem);
    if (ret != 0)
    {
      break;
    }

    return 0;
  } while (0);

  if (inDataItem != NULL)
  {
    this->releaseCodecData(inDataItem);
  }

  return -1;
}

void TaskCodec::clear(void)
{
  this->data_Manager.releaseAll();
}

// static functions
DataSize_t TaskCodec::encodedUnitSize(byte codecType)
{
  switch (codecType)
  {
  case (byte)TaskCodecConstant::TaskCodecType::TASK_CODEC_HEX:
    return TASK_CODEC_ENCODED_UNIT_LEN_HEX;
  default:
    return TASK_CODEC_ENCODED_UNIT_LEN_NONE;
  }
}

DataSize_t TaskCodec::decodedUnitSize(byte codecType)
{
  switch (codecType)
  {
  case (byte)TaskCodecConstant::TaskCodecType::TASK_CODEC_HEX:
    return TASK_CODEC_DECODED_UNIT_LEN;
  default:
    return TASK_CODEC_DECODED_UNIT_LEN;
  }
}

DataSize_t TaskCodec::calculateEncodeSize(byte codecType, DataSize_t dataSize)
{
  return (DataSize_t)(dataSize * TaskCodec::encodedUnitSize(codecType));
}

DataSize_t TaskCodec::calculateDecodedSize(byte codecType, DataSize_t dataSize)
{
  return (DataSize_t) (dataSize / TaskCodec::encodedUnitSize(codecType));
}

int TaskCodec::encode(byte codecType, unsigned char* inData, DataSize_t inDataSize, unsigned char* outBuffer, DataSize_t outBufferSize, DataSize_t& outDataSize)
{
  int result = 0;
  do
  {
    outDataSize = TaskCodec::calculateEncodeSize(codecType, inDataSize);
    if ((inData == NULL)
      || (inDataSize <= 0)
      || (outBuffer == NULL)
      || (outBufferSize < outDataSize)
      )
    {
      break;
    }

    switch (codecType)
    {
      case (byte)TaskCodecConstant::TaskCodecType::TASK_CODEC_HEX:
        result = TaskCodec::encodeHex(inData, inDataSize, outBuffer, outDataSize);
        break;
      default:
        result = -1;
        break;
    }
    
    if (result != 0)
    {
      break;
    }

    return 0;
  } while (0);
  outDataSize = 0;
  return -1;
}

int TaskCodec::decode(byte codecType, unsigned char* inData, DataSize_t inDataSize, unsigned char* outBuffer, DataSize_t outBufferSize, DataSize_t& outDataSize)
{
  int result = 0;
  do
  {
    outDataSize = TaskCodec::calculateDecodedSize(codecType, inDataSize);
    DataSize_t encodedUnitSize = TaskCodec::encodedUnitSize(codecType);
    if ((inData == NULL)
      || (inDataSize <= 0)
      || (outBuffer == NULL)
      || (outBufferSize < outDataSize)
      || (inDataSize % encodedUnitSize != 0)
      )
    {
      break;
    }

    switch (codecType)
    {
    case (byte)TaskCodecConstant::TaskCodecType::TASK_CODEC_HEX:
      result = TaskCodec::decodeHex(inData, inDataSize, outBuffer, outDataSize);
      break;
    default:
      result = -1;
      break;
    }

    if (result != 0)
    {
      break;
    }

    return 0;
  } while (0);
  outDataSize = 0;
  return -1;
}

int TaskCodec::encodeHex(unsigned char* inData, DataSize_t inDataSize, unsigned char* outBuffer, DataSize_t outBufferSize)
{
  do
  {
    DataSize_t inIndx = 0;
    int8_t ret = 0;
    for (inIndx = 0; inIndx < inDataSize; inIndx++)
    {
      ret = TaskCodec::byteToChar(inData[inIndx] / TASK_CODEC_BASE_HEX, outBuffer[0]);
      ret = (int8_t) (ret + TaskCodec::byteToChar(inData[inIndx] % TASK_CODEC_BASE_HEX, outBuffer[1]));
      outBuffer += TASK_CODEC_ENCODED_UNIT_LEN_HEX;
      if (ret != 0)
      {
        break;
      }
    }

    if (inIndx < inDataSize)
    {
      break;
    }

    return 0;
  } while (0);
  return -1;
}

int TaskCodec::decodeHex(unsigned char* inData, DataSize_t inDataSize, unsigned char* outBuffer, DataSize_t outBufferSize)
{
  do
  {
    int8_t ret = 0;
    DataSize_t inIndx = 0;
    byte outVal1 = 0;
    byte outVal2 = 0;
    for (inIndx = 0; inIndx <= (DataSize_t)(inDataSize - 2); inIndx= (DataSize_t)(inIndx + 2))
    {
      ret = TaskCodec::hexCharToByte(inData[inIndx], outVal1);
      ret = (int8_t)(ret + TaskCodec::hexCharToByte(inData[inIndx + 1], outVal2));
      outBuffer[inIndx / TASK_CODEC_ENCODED_UNIT_LEN_HEX] = (unsigned char) (outVal1* TASK_CODEC_BASE_HEX + outVal2);
      if (ret != 0)
      {
        break;
      }
    }

    if (inIndx < inDataSize - 2)
    {
      break;
    }

    return 0;
  } while (0);
  return -1;
}

int8_t TaskCodec::hexCharToByte(const char inChar, byte& outByte)
{
  do
  {
    switch (inChar)
    {
    case '0':
      outByte = 0;
      break;
    case '1':
      outByte = 1;
      break;
    case '2':
      outByte = 2;
      break;
    case '3':
      outByte = 3;
      break;
    case '4':
      outByte = 4;
      break;
    case '5':
      outByte = 5;
      break;
    case '6':
      outByte = 6;
      break;
    case '7':
      outByte = 7;
      break;
    case '8':
      outByte = 8;
      break;
    case '9':
      outByte = 9;
      break;
    case 'A':
      outByte = 10;
      break;
    case 'B':
      outByte = 11;
      break;
    case 'C':
      outByte = 12;
      break;
    case 'D':
      outByte = 13;
      break;
    case 'E':
      outByte = 14;
      break;
    case 'F':
      outByte = 15;
      break;
    default:
      return -1;
    }

    return 0;
  } while (0);
  return -1;
}


int8_t TaskCodec::byteToChar(const byte inByte, unsigned char& outChar)
{
  do
  {
    switch (inByte)
    {
    case 0:
      outChar = '0';
      break;
    case 1:
      outChar = '1';
      break;
    case 2:
      outChar = '2';
      break;
    case 3:
      outChar = '3';
      break;
    case 4:
      outChar = '4';
      break;
    case 5:
      outChar = '5';
      break;
    case 6:
      outChar = '6';
      break;
    case 7:
      outChar = '7';
      break;
    case 8:
      outChar = '8';
      break;
    case 9:
      outChar = '9';
      break;
    case 10:
      outChar = 'A';
      break;
    case 11:
      outChar = 'B';
      break;
    case 12:
      outChar = 'C';
      break;
    case 13:
      outChar = 'D';
      break;
    case 14:
      outChar = 'E';
      break;
    case 15:
      outChar = 'F';
      break;
    default:
      return -1;
    }

    return 0;
  } while (0);
  return -1;
}


/*TaskCodecFactory*/
TaskCodecFactory::TaskCodecFactory(void)
{

}

TaskCodecFactory::~TaskCodecFactory(void)
{

}

TaskCodec* TaskCodecFactory::create(byte codecType, TaskCodecConfigTAG& codecConfig)
{
  TaskCodec* taskCodec = NULL;
  int ret = 0;
  do
  {
    switch (codecType)
    {
    case (byte)TaskCodecConstant::TASK_CODEC_HEX:
    {
      taskCodec = new TaskCodec(codecType);
      if (taskCodec == NULL)
      {
        break;
      }

      ret = taskCodec->config(codecConfig);
      break;
    }
    // ssl or other codec could be here
    default:
      break;
    }

    if (ret != 0)
    {
      break;
    }

    return taskCodec;
  } while (0);
  TaskCodecFactory::release(taskCodec);
  return NULL;
}

void TaskCodecFactory::release(TaskCodec*& taskCodec)
{
  do
  {
    if (taskCodec == NULL)
    {
      return;
    }
    delete taskCodec;
  } while (0);
  taskCodec = NULL;
}
#endif // _CONF_TASK_CODEC_ENABLED