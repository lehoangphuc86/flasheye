#include "CommSerial.h"

#if (_CONF_COMM_SERIAL_ENABLED)
/////////////////////////////////////////////////
// INCLUDE

/////////////////////////////////////////////////
// PREPROCESSOR

/////////////////////////////////////////////////
// DEFINE
//#define COMM_SERIAL_CONSOLE_DEBUG_ENABLE
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

/* CommSerial*/

////////////////////Function to call from outside ISR///////////////////////
CommSerial::CommSerial(void)
#ifdef COMM_SERIAL_USE_TASK
  : TaskManager()
#else
  : is_Running(false)
#endif // COMM_SERIAL_USE_TASK
  , connection_State(CommConnectionState::CommDisconnected)
  , task_Codec(NULL)
{

}

CommSerial::~CommSerial(void)
{
  this->stop();
  this->cleanUp();
}

bool CommSerial::isInitialized(void)
{
  return this->data_Manager.isValid();
}

bool CommSerial::isRunning(void)
{
#ifdef COMM_SERIAL_USE_TASK
  return TaskManager::isTaskRunning();
#else // COMM_SERIAL_USE_TASK
  SystemCriticalLocker criticalLocker(this->critical_Key);
  return this->is_Running;
#endif // COMM_SERIAL_USE_TASK
}

void CommSerial::isRunning(bool state)
{
#ifdef COMM_SERIAL_USE_TASK
  //return TaskManager::isTaskRunning(state);
#else
  SystemCriticalLocker criticalLocker(this->critical_Key);
  this->is_Running = state;
#endif // COMM_SERIAL_USE_TASK
}


bool CommSerial::isConnected(void)
{
  return (this->connectionState() == CommConnectionState::CommConnected ? true : false);
}

bool CommSerial::isDisconnected(void)
{
  return (this->connectionState() == CommConnectionState::CommDisconnected ? true : false);
}

bool CommSerial::isValidSocket(void)
{
  return this->system_Socket.isValid();
}

byte CommSerial::connectionState(void)
{
  SystemCriticalLocker criticalLocker(this->critical_Key);
  return this->connection_State;
}

int CommSerial::setConfig(CommSerialConfigTAG& commConfig)
{
  do
  {
    int result = 0;
    if (this->isInitialized() != false)
    {
      return -1;
    }

    result = this->comm_Setting.setConfig(commConfig);
    if (result != 0)
    {
      break;
    }

    BufferDataManagerConfigTAG bufferManagerConfig = BufferDataManagerConfigTAG();
    bufferManagerConfig.dataItemConfig.bufferSize = this->comm_Setting.bufferMaxSize();
    bufferManagerConfig.preparedDataNumber = this->comm_Setting.bufferCount();
    bufferManagerConfig.usePool = this->comm_Setting.bufferUsePool();
    result = this->data_Manager.prepare(bufferManagerConfig);
    if (result != 0)
    {
      break;
    }

#ifdef COMM_SERIAL_USE_TASK
    TaskManagerConfigTAG taskConfig = TaskManagerConfigTAG();
    memset(&taskConfig, 0, sizeof(TaskManagerConfigTAG));
    taskConfig.eventItemNumber = this->comm_Setting.taskEventItemNumer();
    taskConfig.eventUsePool = this->comm_Setting.taskEventUsePool();
    // self
    this->registerHanldingEventStructSize(sizeof(EventCommSerialSendRequestedTAG));
    this->registerHanldingEventStructSize(sizeof(EventCommSerialCloseConnectionTAG));
    this->registerHanldingEventStructSize(sizeof(EventCommSerialDataReceivedTAG));
    this->registerHanldingEventStructSize(sizeof(EventCommSerialConnectionClosedTAG));
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
#endif // COMM_SERIAL_USE_TASK

    if (this->comm_Setting.useCodec() != false)
    {
      if (this->task_Codec == NULL)
      {
        TaskCodecConfigTAG taskCodecConfig = TaskCodecConfigTAG();
        taskCodecConfig.codecType = this->comm_Setting.codecType();
        taskCodecConfig.maxDataSize = this->comm_Setting.bufferMaxSize();
        this->task_Codec = TaskCodecFactory::create(this->comm_Setting.codecType(), taskCodecConfig);
      }

      if (this->task_Codec == NULL)
      {
        break;
      }
    }
    
    result = this->comm_Sender.setConfig(
      &this->comm_Setting
      , &this->data_Manager
      , this->task_Codec
#ifdef COMM_SERIAL_USE_TASK
      , taskConfig
#endif // COMM_SERIAL_USE_TASK
    );

    if (result != 0)
    {
      break;
    }

    // receiver
#ifdef COMM_SERIAL_USE_TASK
    //taskConfig.usingEvent = false;
    taskConfig.eventItemNumber = 0;
#endif // COMM_SERIAL_USE_TASK

    result = this->comm_Receiver.setConfig(
      this,
      &this->comm_Setting
      , &this->data_Manager
      , this->task_Codec
#ifdef COMM_SERIAL_USE_TASK
      , taskConfig
#endif // COMM_SERIAL_USE_TASK
    );

    if (result != 0)
    {
      break;
    }
    
    return 0;
  } while (0);

  this->stop();
  return -1;
}

int CommSerial::prepare(void)
{
  do
  {
    //int result = 0;
    if (this->isInitialized() == false)
    {
      break;
    }

    if (this->comm_Setting.isValidConfig() == false)
    {
      break;
    }
    return 0;
  } while (0);
  return -1;
}

int CommSerial::start(void)
{
  do
  {
#ifdef COMM_SERIAL_CONSOLE_DEBUG_ENABLE
    //CONSOLE_LOG("[ser] start %i", 0);
#endif // COMM_SERIAL_CONSOLE_DEBUG_ENABLE
    int result = 0;
    if (this->isRunning() != false)
    {
      return 0;
    }
    //this->stop();
    if (this->comm_Setting.isValidConfig() == false)
    {
      break;
    }

#ifdef COMM_SERIAL_USE_TASK
    TaskThreadConfigTAG threadConfig = TaskThreadConfigTAG();
    threadConfig.enabled = true;
    threadConfig.usStackDepth = this->comm_Setting.taskStackDepth();
    threadConfig.uxPriority = this->comm_Setting.taskPriority();
    threadConfig.useThreadPool = this->comm_Setting.taskThreadUsePool();
    result = TaskManager::startProcess(threadConfig, true);
    if (result != 0)
    {
      break;
    }
//#ifdef COMM_SERIAL_CONSOLE_DEBUG_ENABLE
//    //CONSOLE_LOG("[ser] start %i", 1);
//#endif // COMM_SERIAL_CONSOLE_DEBUG_ENABLE
//    result = this->waitPrepareResult();
//    if (result != 0)
//    {
//      break;
//    }
//#ifdef COMM_SERIAL_CONSOLE_DEBUG_ENABLE
//    //CONSOLE_LOG("[ser] start %i", 2);
//#endif // COMM_SERIAL_CONSOLE_DEBUG_ENABLE
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

int CommSerial::connect(CommSerialSocketConfigTAG& socketConfig)
{
  do
  {
#ifdef COMM_SERIAL_CONSOLE_DEBUG_ENABLE
    //CONSOLE_LOG("[ser] conn %i", 0);
#endif // COMM_SERIAL_CONSOLE_DEBUG_ENABLE
    int result = 0;
    if (this->isRunning() == false)
    {
#ifdef COMM_SERIAL_CONSOLE_DEBUG_ENABLE
      //CONSOLE_LOG("[ser] conn %i", 11);
#endif // COMM_SERIAL_CONSOLE_DEBUG_ENABLE
      break;
    }

    if (this->isConnected() != false)
    {
      return 0;
    }
    
    //this->disconnect();
    /*if ( (socketConfigAdd == NULL)
      || (socketConfigSize != sizeof(CommSerialSocketConfigTAG))
      )
    {
      break;
    }*/
      
    //CommSerialSocketConfigTAG* socketConfig = (CommSerialSocketConfigTAG*)socketConfigAdd;
    result = this->system_Socket.setConfig();
    if (result != 0)
    {
      break;
    }

    result = this->system_Socket.start(socketConfig);
    if (result != 0)
    {
      break;
    }
    EventCommSerialOpenConnectionTAG eventData = EventCommSerialOpenConnectionTAG();

#ifdef COMM_SERIAL_USE_TASK
    this->connect_Key.lock();
    result = this->notify((int)EventManagerConstant::EventMessageId::CommOpenConnection, sizeof(EventCommSerialOpenConnectionTAG), (unsigned char*)&eventData);
    if (result != 0)
    {
      break;
    }
    this->connect_Key.lock(COMM_SERIAL_CONNECT_MAX_WAIT);
    if (this->isConnected() == false) // timeout
    {
#ifdef COMM_SERIAL_CONSOLE_DEBUG_ENABLE
      CONSOLE_LOG("[ser] conn %i", 33);
#endif // COMM_SERIAL_CONSOLE_DEBUG_ENABLE
      //this->disconnect();
      this->closeConnection();
      break;
    }
#else // COMM_SERIAL_USE_TASK
    result = this->onEventOpenConnection((unsigned char*)&eventData, sizeof(EventCommSerialOpenConnectionTAG));
    if (result != 0)
    {
      break;
    }
#endif // COMM_SERIAL_USE_TASK

    return 0;
  } while (0);
  return -1;
}

int CommSerial::disconnect(unsigned int timeWait)
{
  int result = 0;
  do
  {
    if (this->isConnected() == false)
    {
      return 0;
    }

    EventCommSerialCloseConnectionTAG eventData = EventCommSerialCloseConnectionTAG();
    
#ifdef COMM_SERIAL_USE_TASK
    this->disconnect_Key.lock();
    result = this->notify((int)EventManagerConstant::EventMessageId::CommCloseConnection, sizeof(EventCommSerialCloseConnectionTAG), (unsigned char*)&eventData);
    if (result != 0)
    {
      break;
    }
    //SYSTEM_SLEEP(200);
    this->disconnect_Key.lock(timeWait);
#else // COMM_SERIAL_USE_TASK
    result = this->onEventCloseConnection((unsigned char*)&eventData, sizeof(EventCommSerialCloseConnectionTAG));
#endif // COMM_SERIAL_USE_TASK
    if (result != 0)
    {
      break;
    }
    return 0;
  } while (0);
  return -1;
}

void CommSerial::stop(void)
{
    this->disconnect();
#ifdef COMM_SERIAL_USE_TASK
    TaskManager::stopProcess();
#endif // COMM_SERIAL_USE_TASK
    this->isRunning(false);
    //this->cleanUp();
    this->data_Manager.releaseAll();
}

BufferDataItem* CommSerial::getCommData(void)
{
  return (BufferDataItem*)this->data_Manager.get();
}

void CommSerial::releaseCommData(BufferDataItem* dataItem)
{
  if (dataItem != NULL)
  {
    this->data_Manager.release(dataItem);
  }
}

void CommSerial::cleanUp(void)
{
  //this->stop();
  //this->data_Manager.releaseAll();
  if (this->task_Codec != NULL)
  {
    TaskCodecFactory::release(this->task_Codec);
  }
  this->task_Codec = NULL;
}

void CommSerial::connectionState(byte state)
{
  SystemCriticalLocker criticalLocker(this->critical_Key);
  this->connection_State = state;
}

void CommSerial::closeConnection(void)
{
  do
  {
    if (this->isConnected() != false)
    {
      this->connectionState(CommConnectionState::CommDisconnecting);
    }
    
    this->system_Socket.notifyExitEvent();
    this->comm_Sender.stop();
    this->comm_Receiver.stop();
    this->system_Socket.stop();
    this->connectionState(CommConnectionState::CommDisconnected);
    return;
  } while (0);

  return;
}

//int CommSerial::send(CommSerialSendRequestedParamsTAG& sendRequestParams)
//{
//  return this->sendBuffer(sendRequestParams.inputBuffer, sendRequestParams.inputBufferSize);
//}

int CommSerial::send(unsigned char* buffer, DataSize_t bufferSize)
{
  do
  {
    if (buffer == NULL)
    {
      break;
    }

    BufferDataItem* dataItem = this->getCommData();
    if (dataItem == NULL)
    {
      break;
    }
    
    int ret = dataItem->setData((DataSize_t)bufferSize, buffer);
    if (ret != 0)
    {
      this->releaseCommData(dataItem);
      break;
    }

    return this->sendDataItem(dataItem);
  } while (0);

  return -1;
}

int CommSerial::sendDataItem(BufferDataItem* dataItem)
{
  int result = 0;
  do
  {
    if (dataItem == NULL)
    {
      break;
    }

    if (this->isConnected() == false)
    {
      this->releaseCommData(dataItem);
      break;
    }
        
    EventCommSerialSendRequestedTAG sendRequestData = EventCommSerialSendRequestedTAG();
    sendRequestData.dataItem = dataItem;
#ifdef COMM_SERIAL_USE_TASK
    result = this->notify(EventManagerConstant::EventMessageId::CommSendRequested, sizeof(EventCommSerialSendRequestedTAG), (unsigned char*)&sendRequestData);
    if (result != 0)
    {
      this->releaseCommData(dataItem);
      break;
    }
#else // COMM_SERIAL_USE_TASK
    result = this->onEventSendRequested((unsigned char*)&sendRequestData, sizeof(EventCommSerialSendRequestedTAG));
    if (result != 0)
    {
      break;
    }
#endif // COMM_SERIAL_USE_TASK
    return 0;
  } while (0);
  return -1;
}


int CommSerial::read(uint8_t inTryTime, BufferDataItem*& dataItem, bool& isDisconnected)
{
#ifdef COMM_SERIAL_USE_TASK
  return -1; // this is for non task only
#else // COMM_SERIAL_USE_TASK
  do
  {
    if (this->isConnected() == false)
    {
      break;
    }

    dataItem = this->comm_Receiver.read(isDisconnected, inTryTime);
    if (isDisconnected != false)
    {
      EventCommSerialCloseConnectionTAG eventData = EventCommSerialCloseConnectionTAG();
      this->onEventCloseConnection((unsigned char*)&eventData, sizeof(EventCommSerialCloseConnectionTAG));
    }
    return 0;
  } while (0);

  return -1;
#endif // COMM_SERIAL_USE_TASK
}

#ifdef COMM_SERIAL_USE_TASK
void CommSerial::proc(void)
{
#ifdef COMM_SERIAL_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG("%s", "[ser] start");
#endif // COMM_SERIAL_CONSOLE_DEBUG_ENABLE
  int prepareRet = this->prepare();
  this->reportPrepareResult(prepareRet);
  if (prepareRet != 0)
  {
    this->waitTerminating();
    return;
  }

  while (this->isTaskRunning() != false) // A Task shall never return or exit.
  {
    EventDataItem* eventData = this->event_Manager.wait(5000);
    if (eventData == NULL)
    {
#ifdef COMM_SERIAL_CONSOLE_DEBUG_ENABLE
      CONSOLE_LOG("%s", "[ser] NL");
#endif // COMM_SERIAL_CONSOLE_DEBUG_ENABLE
      continue;
    }
    switch (eventData->messageId())
    {
    case (int)EventManagerConstant::EventMessageId::CommOpenConnection:
      this->onEventOpenConnection(eventData->bufferAddress(), eventData->dataLength());
      break;
    case (int)EventManagerConstant::EventMessageId::CommSendRequested:
      this->onEventSendRequested(eventData->bufferAddress(), eventData->dataLength());
      break;
    case (int)EventManagerConstant::EventMessageId::CommDataReceived:
      this->onEventDateReceived(eventData->bufferAddress(), eventData->dataLength());
      break;
    case (int)EventManagerConstant::EventMessageId::CommCloseConnection:
      this->onEventCloseConnection(eventData->bufferAddress(), eventData->dataLength());
      break;
    case (int)EventManagerConstant::EventMessageId::CommConnectionClosed:
      this->onEventConnectionClosed(eventData->bufferAddress(), eventData->dataLength());
      break;
    case (int)EventManagerConstant::EventMessageId::TerminateProcess:
      this->isTaskRunning(false);
      break;
    default:
#ifdef COMM_SERIAL_CONSOLE_DEBUG_ENABLE
      CONSOLE_LOG("%s %i", "[ser] id=", eventData->messageId());
#endif // COMM_SERIAL_CONSOLE_DEBUG_ENABLE
      break;
    }
    this->event_Manager.release(eventData);
  }
#ifdef COMM_SERIAL_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG("%s", "[ser] stop");
#endif // COMM_SERIAL_CONSOLE_DEBUG_ENABLE
  //this->cleanUp();
}

int CommSerial::onEventDateReceived(unsigned char* data, unsigned int dataSize)
{
#ifdef COMM_PROC_TIME_MEA_ENABLE
  unsigned long revTime = SYSTEM_TIME_MICROS();
#endif //COMM_PROC_TIME_MEA_ENABLE
#ifdef COMM_SERIAL_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG("[ser] rev %i", 0);
#endif // COMM_SERIAL_CONSOLE_DEBUG_ENABLE
  do
  {
    if ((dataSize != sizeof(EventCommSerialDataReceivedTAG))
      || (data == NULL))
    {
      break;
    }

    EventCommSerialDataReceivedTAG* eventData = (EventCommSerialDataReceivedTAG*)data;
#ifdef COMM_PROC_TIME_MEA_ENABLE
    eventData->meaTime.procTimes[COMM_PROC_TIME_MEA_SERIAL_REV] = revTime;
#endif //COMM_PROC_TIME_MEA_ENABLE
    BufferDataItem* revDataItem = eventData->dataItem;
    if (revDataItem == NULL)
    {
      break;
    }

    if (this->isConnected() == false)
    {
      this->releaseCommData(revDataItem);
      break;
    }

    CommSerialCbOnDataReceived cbOnDataRev = this->comm_Setting.cbDataReceived();
    if (cbOnDataRev == NULL)
    {
      this->releaseCommData(revDataItem);
      break;
    }
    eventData->cbParams = this->comm_Setting.cbParams();
    cbOnDataRev(eventData, sizeof(EventCommSerialDataReceivedTAG));
    return 0;
  } while (0);
  return -1;
}

int CommSerial::onEventConnectionClosed(unsigned char* data, unsigned int dataSize)
{
#ifdef COMM_SERIAL_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG("[ser] dised %i", 0);
#endif // COMM_SERIAL_CONSOLE_DEBUG_ENABLE
  do
  {
    if ((dataSize != sizeof(EventCommSerialConnectionClosedTAG))
      || (data == NULL))
    {
      break;
    }

    if (this->isConnected() == false)
    {
      break; // this is self closing.
    }

    this->closeConnection();
    //EventCommConnectionClosedTAG* eventData = (EventCommConnectionClosedTAG*)data;
    CommSerialCbOnConnectionClosed cbOnConnectionClosed = this->comm_Setting.cbConnectionClosed();
    if (cbOnConnectionClosed == NULL)
    {
      break;
    }

    static EventCommSerialConnectionClosedTAG cbOnConnectClosedParams = EventCommSerialConnectionClosedTAG();
    cbOnConnectClosedParams.cbParams = this->comm_Setting.cbParams();
    cbOnConnectionClosed(&cbOnConnectClosedParams, sizeof(EventCommSerialConnectionClosedTAG));

    return 0;
  } while (0);
  return -1;
}
#endif // COMM_SERIAL_USE_TASK

int CommSerial::onEventSendRequested(unsigned char* data, unsigned int dataSize)
{
#ifdef COMM_SERIAL_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG("[ser] sen %i", 0);
#endif // COMM_SERIAL_CONSOLE_DEBUG_ENABLE
  int result = 0;
  do
  {
    if ((dataSize != sizeof(EventCommSerialSendRequestedTAG))
      || (data == NULL))
    {
      break;
    }

    EventCommSerialSendRequestedTAG* eventData = (EventCommSerialSendRequestedTAG*)data;
    BufferDataItem* sendDataItem = eventData->dataItem;
    if (sendDataItem == NULL)
    {
      break;
    }

    if (this->isConnected() == false)
    {
      this->releaseCommData(sendDataItem);
      break;
    }

    result = this->comm_Sender.send(sendDataItem);
    if (result != 0)
    {
      break;
    }
    return 0;
  } while (0);

  return -1;
}

int CommSerial::onEventCloseConnection(unsigned char* data, unsigned int dataSize)
{
#ifdef COMM_SERIAL_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG("[ser] dis %i", 0);
#endif // COMM_SERIAL_CONSOLE_DEBUG_ENABLE
  int result = 0;
  do
  {
    if ((dataSize != sizeof(EventCommSerialCloseConnectionTAG))
      || (data == NULL))
    {
      return -1;
    }

    //EventCommCloseConnectionTAG* eventData = (EventCommCloseConnectionTAG*)data;
    this->closeConnection();
    result = 0;
    goto _closeConnectionEnd;
  } while (0);

  result = -1;
_closeConnectionEnd:
#ifdef COMM_SERIAL_USE_TASK
  this->disconnect_Key.unlock();
#endif // COMM_SERIAL_USE_TASK
  return result;
}

int CommSerial::onEventOpenConnection(unsigned char* data, unsigned int dataSize)
{
#ifdef COMM_SERIAL_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG("[ser] open %i", 0);
#endif // COMM_SERIAL_CONSOLE_DEBUG_ENABLE
  int result = 0;
  do
  {
    if ((dataSize != sizeof(EventCommSerialOpenConnectionTAG))
      || (data == NULL))
    {
      return -1;
    }

    if (this->isDisconnected() == false)
    {
      result = -1;
      goto _openConnectionEnd;
    }
#ifdef COMM_SERIAL_USE_TASK
    TaskThreadConfigTAG threadConfig = TaskThreadConfigTAG();
    threadConfig.enabled = true;
    threadConfig.usStackDepth = this->comm_Setting.taskStackDepth();;
    threadConfig.uxPriority = this->comm_Setting.taskPriority();
    threadConfig.useThreadPool = this->comm_Setting.taskThreadUsePool();
    result = this->comm_Receiver.start(&this->system_Socket, threadConfig);
#else // COMM_SERIAL_USE_TASK
    result = this->comm_Receiver.start(&this->system_Socket);
#endif // COMM_SERIAL_USE_TASK
    if (result != 0)
    {
#ifdef COMM_SERIAL_CONSOLE_DEBUG_ENABLE
      CONSOLE_LOG("[ser] open %i", 11);
#endif // COMM_SERIAL_CONSOLE_DEBUG_ENABLE
      break;
    }
#ifdef COMM_SERIAL_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG("[ser] open %i", 2);
#endif // COMM_SERIAL_CONSOLE_DEBUG_ENABLE
#ifdef COMM_SERIAL_USE_TASK
    result = this->comm_Sender.start(&this->system_Socket, threadConfig);
#else // COMM_SERIAL_USE_TASK
    result = this->comm_Sender.start(&this->system_Socket);
#endif // COMM_SERIAL_USE_TASK
    if (result != 0)
    {
#ifdef COMM_SERIAL_CONSOLE_DEBUG_ENABLE
      CONSOLE_LOG("[ser] open %i", 22);
#endif // COMM_SERIAL_CONSOLE_DEBUG_ENABLE
      break;
    }
#ifdef COMM_SERIAL_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG("[ser] open %i", 3);
#endif // COMM_SERIAL_CONSOLE_DEBUG_ENABLE
    this->connectionState(CommConnectionState::CommConnected);
    goto _openConnectionEnd;
  } while (0);

  this->disconnect();

_openConnectionEnd:
#ifdef COMM_SERIAL_USE_TASK
  this->connect_Key.unlock();
#endif // COMM_SERIAL_USE_TASK
  return result;
}

#endif // _CONF_COMM_SERIAL_ENABLED