#include "CommSerialSender.h"

#if (_CONF_COMM_SERIAL_SENDER_ENABLED)
/////////////////////////////////////////////////
// INCLUDE
#include "../../utility/Utility.h"
/////////////////////////////////////////////////
// PREPROCESSOR

/////////////////////////////////////////////////
// DEFINE
#define COMM_SERIAL_SENDER_CONSOLE_DEBUG_ENABLE
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

/* CommSerialSender*/

////////////////////Function to call from outside ISR///////////////////////
CommSerialSender::CommSerialSender(void)
#ifdef COMM_SERIAL_USE_TASK
  : TaskManager()
#else
  : is_Running(false)
#endif // COMM_SERIAL_USE_TASK
  , system_Socket(NULL)
  , comm_Setting(NULL)
  , data_Manager(NULL)
  , task_Codec(NULL)
{

}

CommSerialSender::~CommSerialSender(void)
{
  this->stop();
  this->cleanUp();
}

bool CommSerialSender::isInitialized(void)
{
  return (this->comm_Setting == NULL ? false : true);
}

bool CommSerialSender::isRunning(void)
{
#ifdef COMM_SERIAL_USE_TASK
  return TaskManager::isTaskRunning();
#else // COMM_SERIAL_USE_TASK
  SystemCriticalLocker criticalLocker(this->critical_Session);
  return this->is_Running;
#endif // COMM_SERIAL_USE_TASK
}


void CommSerialSender::isRunning(bool state)
{
#ifdef COMM_SERIAL_USE_TASK
  //return TaskManager::isTaskRunning(state);
#else
  SystemCriticalLocker criticalLocker(this->critical_Session);
  this->is_Running = state;
#endif // COMM_SERIAL_USE_TASK
}


int CommSerialSender::setConfig(
  CommSerialConnectionSetting* commSettings
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

    if ( (commSettings == NULL)
      || (commSettings->isValidConfig() == false)
      || (dataManager == NULL)
      //|| (dataManager->isValid() == false)
      )
    {
      break;
    }

    this->comm_Setting = commSettings;
    this->data_Manager = dataManager;
#ifdef COMM_SERIAL_USE_TASK
    // Set up tasks
    {
      this->registerHanldingEventStructSize(sizeof(EventCommSerialSendRequestedTAG));
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

int CommSerialSender::prepare(void)
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
int CommSerialSender::start(TaskThreadConfigTAG& threadConfig)
{
  return this->start(this->system_Socket, threadConfig);
}
#else // COMM_SERIAL_USE_TASK
int CommSerialSender::start(void)
{
  return this->start(this->system_Socket);
}
#endif // COMM_SERIAL_USE_TASK


#ifdef COMM_SERIAL_USE_TASK
int CommSerialSender::start(CommSerialSystemSocket* systemSocket, TaskThreadConfigTAG& threadConfig)
#else // COMM_SERIAL_USE_TASK
int CommSerialSender::start(CommSerialSystemSocket* systemSocket)
#endif // COMM_SERIAL_USE_TASK
{
  do
  {
    int result = 0;
    if (this->isRunning() != false)
    {
      return 0;
    }
    //this->stop();
    if ((systemSocket == NULL)
      || (systemSocket->isValid() == false)
      )
    {
      break;
    }
    
    this->system_Socket = systemSocket;

#ifdef COMM_SERIAL_USE_TASK
    result = TaskManager::startProcess(threadConfig, true);
    if (result != 0)
    {
      break;
    }

    //result = this->waitPrepareResult();
    //if (result != 0)
    //{
    //  break;
    //}
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

void CommSerialSender::stop(void)
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

void CommSerialSender::cleanUp(void)
{
  //this->stop();
  this->system_Socket = NULL;
  this->comm_Setting = NULL;
  this->data_Manager = NULL;
}

int CommSerialSender::send(BufferDataItem* dataItem)
{
  int result = 0;
  do
  {
    if (dataItem == NULL)
    {
      break;
    }

    if (this->isRunning() == false)
    {
      break;
    }
    
    EventCommSerialSendRequestedTAG sendRequestData = EventCommSerialSendRequestedTAG();
    sendRequestData.dataItem = dataItem;
#ifdef COMM_SERIAL_USE_TASK
    result = this->notify(EventManagerConstant::EventMessageId::CommSendRequested, sizeof(EventCommSerialSendRequestedTAG), (unsigned char*)&sendRequestData);
#else // COMM_SERIAL_USE_TASK
    result = this->onEventSendRequested((unsigned char*)&sendRequestData, sizeof(EventCommSerialSendRequestedTAG));
    dataItem = NULL;
#endif // COMM_SERIAL_USE_TASK
    if (result != 0)
    {
      break;
    }
    return 0;

  } while (0);

  if ( (dataItem != NULL)
    && (this->data_Manager!= NULL)
    )
  {
    this->data_Manager->release(dataItem);
  }
  return -1;
}

#ifdef COMM_SERIAL_USE_TASK
void CommSerialSender::proc(void)
{
#ifdef COMM_SERIAL_SENDER_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG("%s", "[sen] start");
#endif // COMM_SERIAL_SENDER_CONSOLE_DEBUG_ENABLE
  int prepareRet = this->prepare();
  this->reportPrepareResult(prepareRet);
  if (prepareRet != 0)
  {
    this->waitTerminating();
    return;
  }

  while (this->isTaskRunning() != false)
  {
    EventDataItem* eventData = this->event_Manager.wait(5000);
    if (eventData == NULL)
    {
#ifdef COMM_SERIAL_SENDER_CONSOLE_DEBUG_ENABLE
      CONSOLE_LOG("%s", "[sen] NL");
#endif // COMM_SERIAL_SENDER_CONSOLE_DEBUG_ENABLE
      continue;
    }
    switch (eventData->messageId())
    {
    case (int)EventManagerConstant::EventMessageId::CommSendRequested:
      this->onEventSendRequested(eventData->bufferAddress(), eventData->dataLength());
      break;
    case (int)EventManagerConstant::EventMessageId::TerminateProcess:
      this->isTaskRunning(false);
      break;
    default:
#ifdef COMM_SERIAL_SENDER_CONSOLE_DEBUG_ENABLE
      CONSOLE_LOG("%s %i", "[sen] id=", eventData->messageId());
#endif // COMM_SERIAL_SENDER_CONSOLE_DEBUG_ENABLE
      break;
    }
    this->event_Manager.release(eventData);
  }
#ifdef COMM_SERIAL_SENDER_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG("[sen] stop %i", this->thread_Handler->id());
#endif // COMM_SERIAL_SENDER_CONSOLE_DEBUG_ENABLE
  //this->cleanUp();
}
#endif // COMM_SERIAL_USE_TASK

int CommSerialSender::onEventSendRequested(unsigned char* data, unsigned int dataSize)
{
  int result = 0;
  BufferDataItem* sendDataItem = NULL;
  do
  {
    if ((dataSize != sizeof(EventCommSerialSendRequestedTAG))
      || (data == NULL))
    {
      break;
    }

    EventCommSerialSendRequestedTAG* eventData = (EventCommSerialSendRequestedTAG*)data;
    sendDataItem = eventData->dataItem;
    if (sendDataItem == NULL)
    {
      break;
    }

    if (this->comm_Setting->isHeaderless() == false)
    {
      CommSerialHeaderTAG serialHeader = CommSerialHeaderTAG();
      serialHeader.checkSum = 0; // not used now
      serialHeader.length = (uint16_t)sendDataItem->dataLength();
      serialHeader.setOpCode(CommHeaderOpCode::CommHeaderOpData);
      serialHeader.setOpStatus(CommHeaderStatus::CommHeaderStatusOK);
      //memcpy(&serialHeader.commCode, COMM_PACKAGE_COMM_CODE_COM, COMM_PACKAGE_COMM_CODE_LEN);
      if (this->comm_Setting->useCheckSum() != false)
      {
        serialHeader.checkSum = Utility::calculateSum((uint8_t*)&serialHeader, sizeof(CommSerialHeaderTAG), 0);
        serialHeader.checkSum = Utility::calculateSum((uint8_t*)sendDataItem->bufferAddress(), sendDataItem->dataLength(), serialHeader.checkSum);
        serialHeader.checkSum = (uint16_t) (UTILITY_CHECK_SUM_MAX_VALUE - serialHeader.checkSum);
      }

      //send header first
      result = this->sendData((unsigned char*)&serialHeader, sizeof(CommSerialHeaderTAG));
      if (result != 0)
      {
        break;
      }
    }

    // send body
    result = this->sendData(sendDataItem->bufferAddress(), sendDataItem->dataLength());
    if (result != 0)
    {
      break;
    }
    this->data_Manager->release(sendDataItem);
    return 0;
  } while (0);

  if (sendDataItem != NULL)
  {
    this->data_Manager->release(sendDataItem);
  }
  return -1;
}

////////////////////Shared funtcion///////////////////////
// send 1 package
// send terminator following each package
int CommSerialSender::sendData(unsigned char* data, DataSize_t dataSize)
{
  int result = 0;
  BufferDataItem* codecDataItem = NULL;
  do
  {
    if (data == NULL)
    {
      break;
    }

    if (this->isRunning() == false)
    {
      break;
    }

    unsigned char* encodedData = data;
    DataSize_t encodedDataSize = dataSize;

    if (this->task_Codec != NULL)
    {
      
      result = this->task_Codec->encode(data, dataSize, codecDataItem);
      if (result != 0)
      {
        break;
      }

      encodedData = codecDataItem->bufferAddress();
      encodedDataSize = codecDataItem->dataLength();
    }

    DataSize_t truckSize = 0;
    DataSize_t totalSentSize = 0;
    DataSize_t sentTruckSize = 0;
    while (totalSentSize < encodedDataSize)
    {
      truckSize = (DataSize_t)SYSTEM_MIN(encodedDataSize - totalSentSize, COMM_SERIAL_SYS_BUFFER_LEN);
      result = this->system_Socket->writeData((uint8_t*)(encodedData + totalSentSize), truckSize, sentTruckSize);
      if ( (result != 0)
        || (sentTruckSize <= 0)
        )
      {
        break;
      }
      totalSentSize = (DataSize_t)(totalSentSize + sentTruckSize);
    }
    if (totalSentSize != encodedDataSize)
    {
      break;
    }

    uint8_t terminator = this->comm_Setting->terminator();
    if (this->comm_Setting->isHeaderless() == false)
    {
      goto _sendData_End;
    }

    result = this->system_Socket->writeData(&terminator, COMM_SERIAL_TERMINATOR_LEN, sentTruckSize);
    if ( (result != 0)
      || (sentTruckSize != COMM_SERIAL_TERMINATOR_LEN)
      )
    {
      break;
    }

  _sendData_End:
    if (codecDataItem != NULL)
    {
      this->task_Codec->releaseCodecData(codecDataItem);
    }

    return 0;
  } while (0);

  if (codecDataItem != NULL)
  {
    this->task_Codec->releaseCodecData(codecDataItem);
  }
  return -1;
}

#endif // _CONF_COMM_SERIAL_SENDER_ENABLED