#include "CommSerialReceiver.h"

#if (_CONF_COMM_SERIAL_RECEIVER_ENABLED)
/////////////////////////////////////////////////
// INCLUDE
#include "../../utility/Utility.h"
/////////////////////////////////////////////////
// PREPROCESSOR
//#define COMM_SERIAL_REV_CONSOLE_DEBUG_ENABLE
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

/* CommSerialReceiver*/

////////////////////Function to call from outside ISR///////////////////////
CommSerialReceiver::CommSerialReceiver(void)
#ifdef COMM_SERIAL_USE_TASK
  : TaskManager()
#else
  : is_Running(false)
#endif // COMM_SERIAL_USE_TASK
  , comm_Parent(NULL)
  , system_Socket(NULL)
  , comm_Setting(NULL)
  , data_Manager(NULL)
  , task_Codec(NULL)
{
  
}

CommSerialReceiver::~CommSerialReceiver(void)
{
  this->stop();
  this->cleanUp();
  //this->clearSaveBuffer();
}

bool CommSerialReceiver::isInitialized(void)
{
  return (this->comm_Parent == NULL ? false : true);
}

bool CommSerialReceiver::isRunning(void)
{
#ifdef COMM_SERIAL_USE_TASK
  return TaskManager::isTaskRunning();
#else // COMM_SERIAL_USE_TASK
  SystemCriticalLocker criticalLocker(this->critical_Session);
  return this->is_Running;
#endif // COMM_SERIAL_USE_TASK
}


void CommSerialReceiver::isRunning(bool state)
{
#ifdef COMM_SERIAL_USE_TASK
  //return TaskManager::isTaskRunning(state);
#else
  SystemCriticalLocker criticalLocker(this->critical_Session);
  this->is_Running = state;
#endif // COMM_SERIAL_USE_TASK
}

int CommSerialReceiver::setConfig(
  void* commParent
  , CommSerialConnectionSetting* commSettings
  , BufferDataManager* dataManager
  , TaskCodec* taskCodec
#ifdef COMM_SERIAL_USE_TASK
  , TaskManagerConfigTAG& taskConfig
#endif // COMM_SERIAL_USE_TASK
)
{
  do
  {
    int result = 0;
    if (this->isInitialized() != false)
    {
      return -1; // already started
    }

    if ( (commParent == NULL)
      || (commSettings == NULL)
      || (commSettings->isValidConfig() == false)
      || (dataManager == NULL)
      //|| (dataManager->isValid() == false)
      )
    {
      break;
    }
    this->comm_Parent = commParent;
    this->comm_Setting = commSettings;
    this->data_Manager = dataManager;
#ifdef COMM_SERIAL_USE_TASK
    // Set up tasks
    {
      //taskConfig.usingEvent = false;
      taskConfig.eventItemNumber = 0;
      result = TaskManager::setConfig(taskConfig);
      if (result != 0)
      {
        break;
      }

      result = TaskManager::prepare();
      if (result != 0)
      {
        break;
      }
    }
#endif // COMM_SERIAL_USE_TASK
    
    if ((this->comm_Setting->useCodec())
      && (taskCodec == NULL)
      )
    {
      break;
    }

    this->task_Codec = taskCodec;

    return result;
  } while (0);
  this->stop();
  return -1;
}

int CommSerialReceiver::prepare(void)
{
  do
  {
    if (this->data_Manager->isValid() == false)
    {
      break;
    }
    return 0;
  } while (0);
  return -1;
}

#ifdef COMM_SERIAL_USE_TASK
int CommSerialReceiver::start(TaskThreadConfigTAG& threadConfig)
{
  return this->start(this->system_Socket, threadConfig);
}
#else // COMM_SERIAL_USE_TASK
int CommSerialReceiver::start(void)
{
  return this->start(this->system_Socket);
}
#endif // COMM_SERIAL_USE_TASK

#ifdef COMM_SERIAL_USE_TASK
int CommSerialReceiver::start(CommSerialSystemSocket* systemSocket, TaskThreadConfigTAG& threadConfig)
#else // COMM_SERIAL_USE_TASK
int CommSerialReceiver::start(CommSerialSystemSocket* systemSocket)
#endif // COMM_SERIAL_USE_TASK
{
  do
  {
    int result = 0;
    if (this->isRunning() != false)
    {
      return 0;
    }

    if ((systemSocket == NULL)
      || (systemSocket->isValid() == false)
      )
    {
      break;
    }

    this->system_Socket = systemSocket;
#ifdef COMM_SERIAL_USE_TASK
    //this->isWaitingPrepare(true);
    result = TaskManager::startProcess(threadConfig, true);
    if (result != 0)
    {
      break;
    }
#ifdef COMM_SERIAL_REV_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG("[rev] start %i", 1);
#endif // COMM_SERIAL_REV_CONSOLE_DEBUG_ENABLE

#else // COMM_SERIAL_USE_TASK
    result = this->prepare();
    if (result != 0)
    {
      break;
    }
    this->isRunning(true);
#endif // COMM_SERIAL_USE_TASK
    
    return result;
  } while (0);

  this->stop();
  return -1;
}

void CommSerialReceiver::stop(void)
{
  if (this->isRunning() != false)
  {
    this->system_Socket->notifyExitEvent();
#ifdef COMM_SERIAL_USE_TASK
    TaskManager::stopProcess();
#endif // COMM_SERIAL_USE_TASK
  }

  this->isRunning(false);
  //this->cleanUp();
}

void CommSerialReceiver::cleanUp(void)
{
  //this->stop();
  this->system_Socket = NULL;
  this->comm_Setting = NULL;
  this->data_Manager = NULL;
}

BufferDataItem* CommSerialReceiver::read(bool& isDisconnected, byte tryTime)
{
#ifdef COMM_SERIAL_USE_TASK
  return NULL; // this is for non task only
#endif // COMM_SERIAL_USE_TASK

  BufferDataItem* dataItem = NULL;
  do
  {
    if (this->isRunning() == false)
    {
      break;
    }
    dataItem = (BufferDataItem*)this->data_Manager->get();
    if (dataItem == NULL)
    {
      break;
    }
#ifdef COMM_PROC_TIME_MEA_ENABLE
    CommProcMeaTimeTAG procTime = CommProcMeaTimeTAG();
    int ret = this->readCore(dataItem, isDisconnected, procTime, tryTime);
#else // COMM_PROC_TIME_MEA_ENABLE
    int ret = this->readCore(dataItem, isDisconnected, tryTime);
#endif // COMM_PROC_TIME_MEA_ENABLE
    if ((ret!=0)
      || (isDisconnected != false)
      )
    {
      break;
    }
    return dataItem;
  } while (0);

  if ((this->data_Manager!=NULL)
    && (dataItem!= NULL))
  {
    this->data_Manager->release(dataItem);
  }

  return NULL;
}

#ifdef COMM_SERIAL_USE_TASK
void CommSerialReceiver::proc(void)
{
#ifdef COMM_SERIAL_REV_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG("[rev] proc %i", 0);
#endif // COMM_SERIAL_REV_CONSOLE_DEBUG_ENABLE
  int ret = 0;
  bool isDisconnected = false;
  BufferDataItem* dataItem = NULL;
  EventCommSerialConnectionClosedTAG eventConnectionClosed = EventCommSerialConnectionClosedTAG();
  EventCommSerialDataReceivedTAG eventDataRev = EventCommSerialDataReceivedTAG();

  int prepareRet = this->prepare();
  this->reportPrepareResult(prepareRet);
  if (prepareRet != 0)
  {
#ifdef COMM_SERIAL_REV_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG("[rev] proc %i", 11);
#endif // COMM_SERIAL_REV_CONSOLE_DEBUG_ENABLE
    this->waitTerminating();
    goto _procEnd;
  }
#ifdef COMM_SERIAL_REV_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG("[rev] proc %i", 2);
#endif // COMM_SERIAL_REV_CONSOLE_DEBUG_ENABLE

  while (this->isTaskRunning() != false) // A Task shall never return or exit.
  {
    if (isDisconnected != false)
    {
      SYSTEM_SLEEP(TASK_MANAGER_CHECK_TERMINATOR_INTERVAL); // sleep and wait for terminator
      continue;
    }

    if (dataItem == NULL)
    {
      dataItem = (BufferDataItem*)this->data_Manager->get();
    }
    if (dataItem == NULL)
    {
      continue;
    }
#ifdef COMM_PROC_TIME_MEA_ENABLE
    memset(&eventDataRev.meaTime, 0, sizeof(CommProcMeaTimeTAG));
    eventDataRev.meaTime.procTimes[COMM_PROC_TIME_MEA_REV_START] = SYSTEM_TIME_MICROS();
    ret = this->readCore(dataItem, isDisconnected, eventDataRev.meaTime);
#else // COMM_PROC_TIME_MEA_ENABLE
    ret = this->readCore(dataItem, isDisconnected);
#endif //COMM_PROC_TIME_MEA_ENABLE
    if (isDisconnected != false)
    {
      // notify to parent connection closed
      ((TaskManager*)this->comm_Parent)->notify(EventManagerConstant::EventMessageId::CommConnectionClosed, sizeof(EventCommSerialConnectionClosedTAG), (unsigned char*)&eventConnectionClosed);
    }

    if (ret != 0)
    {
      continue;
    }

    // notify to parent data received
    if (dataItem->hasData() != false)
    {
      eventDataRev.dataItem = dataItem;
      ret = ((TaskManager*)this->comm_Parent)->notify(EventManagerConstant::EventMessageId::CommDataReceived, sizeof(EventCommSerialDataReceivedTAG), (unsigned char*)&eventDataRev);
      if (ret != 0)
      {
        //this->data_Manager->release(dataItem);
        continue;
      }

      dataItem = NULL; // let upper side release the dataItem
    }
    
  }

_procEnd:
  if (dataItem != NULL)
  {
    this->data_Manager->release(dataItem);
  }
#ifdef COMM_SERIAL_REV_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG("%s", "[rev] stop");
#endif // COMM_SERIAL_REV_CONSOLE_DEBUG_ENABLE
  //this->cleanUp();
}
#endif // COMM_SERIAL_USE_TASK

////////////////////Shared funtcion///////////////////////
// read header and body
#ifdef COMM_PROC_TIME_MEA_ENABLE
int CommSerialReceiver::readCore(BufferDataItem* dataItem, bool& isDisconnected, CommProcMeaTimeTAG& procTime, byte tryTime)
#else // COMM_PROC_TIME_MEA_ENABLE
int CommSerialReceiver::readCore(BufferDataItem* dataItem, bool& isDisconnected, byte tryTime)
#endif // COMM_PROC_TIME_MEA_ENABLE
{
  //BufferDataItem* dataItem = NULL;
  isDisconnected = false;
  do
  {
    int ret = 0;
    if (this->isRunning() == false)
    {
      break;
    }

    //dataItem = (BufferDataItem*)this->data_Manager->get();
    if (dataItem == NULL)
    {
      break;
    }

    bool headerLess = this->comm_Setting->isHeaderless();
    DataSize_t lenToRead = this->comm_Setting->bufferMaxSize();
    DataSize_t readLen = 0;
    uint16_t checkSum = 0;
    //CONSOLE_LOG("[rer] rc %i", 1);
    if (headerLess == false)
    {
      // read header 
      lenToRead = sizeof(CommSerialHeaderTAG);
      readLen = this->readData(dataItem->bufferAddress(), dataItem->bufferLength(), lenToRead, isDisconnected, tryTime);
#ifdef COMM_PROC_TIME_MEA_ENABLE
      procTime.procTimes[COMM_PROC_TIME_MEA_REV_HEADER] = SYSTEM_TIME_MICROS();
#endif //COMM_PROC_TIME_MEA_ENABLE
      //CONSOLE_LOG("[rer] rc %i %i %i", 2, readLen, lenToRead);
      if (isDisconnected != false)
      {
        break;
      }

      if (readLen != lenToRead)
      {
        break;
      }
      CommSerialHeaderTAG* serialHeader = (CommSerialHeaderTAG*)dataItem->bufferAddress();

      /*if (Utility::isEqualString(serialHeader->commCode, COMM_PACKAGE_COMM_CODE_LEN, COMM_PACKAGE_COMM_CODE_COM, COMM_PACKAGE_COMM_CODE_LEN) == false)
      {
        break;
      }*/

      if ((serialHeader->getOpCode() != CommHeaderOpCode::CommHeaderOpData)
        || (serialHeader->getOpStatus() != CommHeaderStatus::CommHeaderStatusOK)
        )
      {
        break;
      }

      lenToRead = serialHeader->length;
      readLen = 0;
      if (this->comm_Setting->useCheckSum() != false)
      {
        checkSum = Utility::calculateSum((uint8_t*)serialHeader, sizeof(CommSerialHeaderTAG), 0);
      }
    }
    
    readLen = this->readData(dataItem->bufferAddress(), dataItem->bufferLength(), lenToRead, isDisconnected, tryTime);
#ifdef COMM_PROC_TIME_MEA_ENABLE
    procTime.procTimes[COMM_PROC_TIME_MEA_REV_BODY] = SYSTEM_TIME_MICROS();
#endif //COMM_PROC_TIME_MEA_ENABLE
    //CONSOLE_LOG("[rer] rc %i %i %i", 3, readLen, lenToRead);
    if (isDisconnected != false)
    {
      //CONSOLE_LOG("[rer] rc %i", 4);
      break;
    }

    if ( (headerLess == false)
      && (readLen != lenToRead)
      )
    {
      //CONSOLE_LOG("[rer] rc %i", 5);
      break;
    }

    if (readLen <= 0)
    {
      //CONSOLE_LOG("[rer] rc %i", 6);
      break;
    }
    //CONSOLE_LOG("[rer] rc %i", 61);
    ret = dataItem->setDataLen((DataSize_t)readLen);
    if (ret != 0)
    {
      //CONSOLE_LOG("[rer] rc %i", 7);
      break;
    }

    if ( (headerLess != false)
      || (this->comm_Setting->useCheckSum() == false)
      )
    {
      //CONSOLE_LOG("[rer] rc %i", 8);
      return 0;
    }
    
    checkSum = Utility::calculateSum((uint8_t*)dataItem->bufferAddress(), dataItem->dataLength(), checkSum);
    if ((UTILITY_CHECK_SUM_MAX_VALUE - checkSum) != 0)
    {
      break; // error
    }

    return 0;
  } while (0);

  return -1;
}

// read 1 package
// 1 package is decided by a terminator
DataSize_t CommSerialReceiver::readData(unsigned char* buffer, DataSize_t bufferSize, DataSize_t maxLenToRead, bool& isDisconnected, byte tryTime)
{
  byte remainTryTime = tryTime;
  isDisconnected = false;
  //int result = 0;
  BufferDataItem* codecDataItem = NULL;
  int ret = 0;
  do
  {
    if (maxLenToRead > bufferSize)
    {
      break;
    }

    DataSize_t encodedReadLen = 0;
    unsigned char* encodedData = buffer;
    DataSize_t encodedDataSize = maxLenToRead;
    
    if (this->task_Codec != NULL)
    {
      codecDataItem = this->task_Codec->getCodecData();
      if (codecDataItem == NULL)
      {
        break;
      }
      encodedData = codecDataItem->bufferAddress();
      encodedDataSize = this->task_Codec->calculateEncodeSize(maxLenToRead);
      if (encodedDataSize > codecDataItem->bufferLength())
      {
        break;
      }
    }

    if (encodedData == NULL)
    {
      break;
    }

    unsigned char* readBuffer = encodedData;
    char terminator = this->comm_Setting->terminator();
    bool useTerminator = this->comm_Setting->isHeaderless();
    bool isCompleted = false;
    DataSize_t readTruckSize = 0;
    DataSize_t truckSizeToRead = 0;
    
    
    while ((isCompleted == false) 
      && (this->isRunning() != false)
      )
    {
      if (encodedReadLen >= encodedDataSize)
      {
        break;
      }

      truckSizeToRead = (DataSize_t)SYSTEM_MIN( (useTerminator == false? encodedDataSize - encodedReadLen : COMM_SERIAL_TERMINATOR_LEN), COMM_SERIAL_SYS_BUFFER_LEN);
      ret = this->system_Socket->readData((uint8_t*)readBuffer, truckSizeToRead, readTruckSize);
      if (ret != 0)
      {
        isDisconnected = true;
        break;
      }

      if (readTruckSize == 0)
      {
        if (remainTryTime <= 0)
        {
#ifdef COMM_SERIAL_REV_CONSOLE_DEBUG_ENABLE
          if (encodedReadLen > 0)
          {
            CONSOLE_LOG("##[rd] bk %i %i", truckSizeToRead, (unsigned int)encodedReadLen);
          }
#endif // COMM_SERIAL_REV_CONSOLE_DEBUG_ENABLE
          break;
        }
        remainTryTime--;
        continue;
      }
      else
      {
#ifdef COMM_SERIAL_REV_CONSOLE_DEBUG_ENABLE
        //CONSOLE_LOG("[rev] red %i %i", 2, readTruckSize);
#endif // COMM_SERIAL_REV_CONSOLE_DEBUG_ENABLE
        remainTryTime = tryTime;
      }

      //SYSTEM_CONSOLE("rd %i %i %i %s", remainTryTime, truckSizeToRead, readTruckSize, buffer);


      if (useTerminator != false) // headerless
      {
        if (readBuffer[0] != terminator) // not found
        {
          encodedReadLen = (DataSize_t)(encodedReadLen + readTruckSize);
          readBuffer += readTruckSize;
          continue;
        }
      }
      else // header
      {
        encodedReadLen = (DataSize_t)(encodedReadLen + readTruckSize);
        readBuffer += readTruckSize;
        if (encodedReadLen < encodedDataSize) // not enough
        {
          continue;
        }
      }

      isCompleted = true;
      break;
    }
    
    if ( (isDisconnected != false)
      || (isCompleted == false))
    {
      break;
    }
    
    if (this->task_Codec == NULL)
    {
      return encodedReadLen;
    }

    DataSize_t decodedReadLen = encodedReadLen;
    codecDataItem->setDataLen(encodedReadLen);
    ret = this->task_Codec->decode(codecDataItem, buffer, bufferSize, decodedReadLen);
    //TaskCodec::decode(this->comm_Setting->codecType(), encodedData, encodedReadLen, buffer, bufferSize, decodedReadLen);
    if (ret != 0)
    {
      break;
    }

    if (codecDataItem != NULL)
    {
      this->task_Codec->releaseCodecData(codecDataItem);
    }
    return decodedReadLen;

  } while (0);

  if (codecDataItem != NULL)
  {
    this->task_Codec->releaseCodecData(codecDataItem);
  }

  return 0;
}

#endif // _CONF_COMM_SERIAL_RECEIVER_ENABLED