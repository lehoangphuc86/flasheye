#include "I2CServerTask.h"

#ifdef _CONF_TEST_TASK_COMM_I2C_SERVER_ENABLED
/////////////////////////////////////////////////
// INCLUDE

/////////////////////////////////////////////////
// PREPROCESSOR
#define I2C_SERVER_TASK_DEBUG_CONSOLE_ENABLED
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

/* I2CServerTask*/

////////////////////Function to call from outside ISR///////////////////////
I2CServerTask::I2CServerTask(void)
#ifdef I2C_COMM_TEST_SERVER_USE_TASK
  : TaskManager()
#endif // I2C_COMM_TEST_SERVER_USE_TASK
{
  memset(&this->i2c_Socket_Config, 0, sizeof(CommI2CSocketConfigTAG));
}
I2CServerTask::~I2CServerTask(void)
{

}


#ifdef I2C_COMM_TEST_SERVER_USE_TASK
int I2CServerTask::setConfig(CommI2CConfigTAG& commConfig,TaskManagerConfigTAG& taskConfigStruct)
#else
int I2CServerTask::setConfig(CommI2CConfigTAG& commConfig)
#endif // I2C_COMM_TEST_SERVER_USE_TASK
{
  do
  {
    int result = 0;
    if (this->i2c_Server.isValidSocket() != false)
    {
      return -1; // already config
    }
    // set up server
    result = this->i2c_Server.setConfig(commConfig);
    if (result != 0)
    {
      break;
    }

    //result = this->i2c_Server.start();
    //if (result != 0)
    //{
    //  break;
    //}

#ifdef I2C_COMM_TEST_SERVER_USE_TASK
    // Set up tasks
    this->registerHanldingEventStructSize(sizeof(EventCommI2CServerSendRequestedTAG));
    this->registerHanldingEventStructSize(sizeof(EventCommI2CServerDataRequestedTAG));
    result = TaskManager::setConfig(taskConfigStruct);
    if (result != 0)
    {
      break;
    }

    result = TaskManager::prepare();
    if (result != 0)
    {
      break;
    }
#endif // I2C_COMM_TEST_SERVER_USE_TASK

    // append client
    {
      CommI2CClientPropertiesTAG clientProperties = CommI2CClientPropertiesTAG();
      clientProperties.address = I2C_COMM_TEST_SERVER_CLIENT_SOCKET_1_ADDRESS;
      clientProperties.headerless = I2C_COMM_TEST_SERVER_CLIENT_SOCKET_1_HEADRLESS;
      clientProperties.checkSum = I2C_COMM_TEST_SERVER_CLIENT_SOCKET_1_CHECKSUM;
      clientProperties.codecType = I2C_COMM_TEST_SERVER_CLIENT_SOCKET_1_CODECTPYE;
      result = this->i2c_Server.appendClient(&clientProperties);
      if (result != 0)
      {
        break;
      }
      clientProperties.address = I2C_COMM_TEST_SERVER_CLIENT_SOCKET_2_ADDRESS;
      clientProperties.headerless = I2C_COMM_TEST_SERVER_CLIENT_SOCKET_2_HEADRLESS;
      clientProperties.checkSum = I2C_COMM_TEST_SERVER_CLIENT_SOCKET_2_CHECKSUM;
      clientProperties.codecType = I2C_COMM_TEST_SERVER_CLIENT_SOCKET_2_CODECTPYE;
      result = this->i2c_Server.appendClient(&clientProperties);
      if (result != 0)
      {
        break;
      }

//      clientProperties.address = I2C_COMM_TEST_SERVER_CLIENT_SOCKET_3_ADDRESS;
//      clientProperties.protocol = I2C_COMM_TEST_SERVER_CLIENT_SOCKET_3_PROTOCOL;
//      result = this->i2c_Server.appendClient(&clientProperties);
//      if (result != 0)
//      {
//#ifdef I2C_SERVER_TASK_DEBUG_CONSOLE_ENABLED
//        CONSOLE_LOG("[i2cT] set %i", -77);
//#endif // I2C_SERVER_TASK_DEBUG_CONSOLE_ENABLED
//        break;
//      }
    }
    
    return result;
  } while (0);
  return -1;
}

int I2CServerTask::prepare(void)
{
  return this->i2c_Server.start(this->i2c_Socket_Config);
}

#ifdef I2C_COMM_TEST_SERVER_USE_TASK
int I2CServerTask::start(CommI2CSocketConfigTAG& commI2CSocketConfig, TaskThreadConfigTAG& threadConfig)
#else // I2C_COMM_TEST_SERVER_USE_TASK
int I2CServerTask::start(CommI2CSocketConfigTAG& commI2CSocketConfig)
#endif // I2C_COMM_TEST_SERVER_USE_TASK
{
  int result = 0;
  do
  {

    if (commI2CSocketConfig.isValid() == false)
    {
      break;
    }

    memcpy(&this->i2c_Socket_Config, &commI2CSocketConfig, sizeof(CommI2CSocketConfigTAG));

#ifdef I2C_COMM_TEST_SERVER_USE_TASK
    if ((this->isTaskRunning() != false)
      || (this->threadHandler() != NULL)
      )
    {
      break;
    }

    result = TaskManager::startProcess(threadConfig, true);
#else // I2C_COMM_TEST_SERVER_USE_TASK
    result = this->prepare();
#endif // I2C_COMM_TEST_SERVER_USE_TASK
    if (result != 0)
    {
      break;
    }
    return 0;
  } while (0);
  return -1;
}


int I2CServerTask::send(uint8_t clientAddress, uint8_t messageId)
{
  int result = 0;
  do
  {
#ifdef I2C_COMM_TEST_SERVER_USE_TASK
    if ( this->isTaskRunning() == false)
    {
      break;
    }
#endif // I2C_COMM_TEST_SERVER_USE_TASK
    EventCommI2CServerSendRequestedTAG sendRequestData = EventCommI2CServerSendRequestedTAG();
    sendRequestData.clientAddress = clientAddress;
    sendRequestData.messageId = messageId;
#ifdef I2C_COMM_TEST_SERVER_USE_TASK
    return this->notify(EventManagerConstant::CommI2CSendRequested, sizeof(EventCommI2CServerSendRequestedTAG), (unsigned char*)&sendRequestData);
#else // I2C_COMM_TEST_SERVER_USE_TASK
    return this->onEventSendRequested((unsigned char*)&sendRequestData, sizeof(EventCommI2CServerSendRequestedTAG));
#endif // I2C_COMM_TEST_SERVER_USE_TASK
  } while (0);
  return -1;
}

int I2CServerTask::request(uint8_t clientAddress, uint8_t messageId, uint16_t requestDataSize)
{
  int result = 0;
  do
  {
#ifdef I2C_COMM_TEST_SERVER_USE_TASK
    if (this->isTaskRunning() == false)
    {
      break;
    }
#endif // I2C_COMM_TEST_SERVER_USE_TASK

    EventCommI2CServerDataRequestedTAG dataRequestData = EventCommI2CServerDataRequestedTAG();
    dataRequestData.clientAddress = clientAddress;
    dataRequestData.messageId = messageId;
    dataRequestData.requestDataSize = requestDataSize;

#ifdef I2C_COMM_TEST_SERVER_USE_TASK
    return this->notify(EventManagerConstant::CommI2CDataRequested, sizeof(EventCommI2CServerDataRequestedTAG), (unsigned char*)&dataRequestData);
#else // I2C_COMM_TEST_SERVER_USE_TASK
    return this->onEventDataRequested((unsigned char*)&dataRequestData, sizeof(EventCommI2CServerDataRequestedTAG));
#endif // I2C_COMM_TEST_SERVER_USE_TASK
  } while (0);

  return -1;
}

int I2CServerTask::stop(void)
{
  do
  {
#ifdef I2C_COMM_TEST_SERVER_USE_TASK
    if (this->isTaskRunning() == false)
    {
      return 0;
    }
    TaskManager::stopProcess();
#else // I2C_COMM_TEST_SERVER_USE_TASK
    this->i2c_Server.stop();
#endif // I2C_COMM_TEST_SERVER_USE_TASK
    return 0;
  } while (0);

  return -1;
}

#ifdef I2C_COMM_TEST_SERVER_USE_TASK
void I2CServerTask::proc(void)
{
#ifdef I2C_SERVER_TASK_DEBUG_CONSOLE_ENABLED
  CONSOLE_LOG("[i2cT] proc %i", 0);
#endif // I2C_SERVER_TASK_DEBUG_CONSOLE_ENABLED
  int prepareRet = this->prepare();
  this->reportPrepareResult(prepareRet);
#ifdef I2C_SERVER_TASK_DEBUG_CONSOLE_ENABLED
  CONSOLE_LOG("[i2cT] proc %i %i", 1, prepareRet);
#endif // I2C_SERVER_TASK_DEBUG_CONSOLE_ENABLED
  if (prepareRet != 0)
  {
    this->waitTerminating();
    goto _procEnd;
  }

  while (this->isTaskRunning() != false) // A Task shall never return or exit.
  {
    EventDataItem* eventData = this->event_Manager.wait(5000);
    if (eventData == NULL)
    {
#ifdef I2C_SERVER_TASK_DEBUG_CONSOLE_ENABLED
      CONSOLE_LOG("[i2cT] evt NL %s", "");
#endif // I2C_SERVER_TASK_DEBUG_CONSOLE_ENABLED
      continue;
    }
    switch (eventData->messageId())
    {
    case (int)EventManagerConstant::EventMessageId::CommI2CSendRequested:
      this->onEventSendRequested(eventData->bufferAddress(), eventData->dataLength());
      break;
    case (int)EventManagerConstant::EventMessageId::CommI2CDataRequested:
      this->onEventDataRequested(eventData->bufferAddress(), eventData->dataLength());
      break;
    case (int)EventManagerConstant::EventMessageId::TerminateProcess:
      this->isTaskRunning(false);
      break;
    default:
#ifdef I2C_SERVER_TASK_DEBUG_CONSOLE_ENABLED
      CONSOLE_LOG("[i2cT] invl id=%i", eventData->messageId());
#endif // I2C_SERVER_TASK_DEBUG_CONSOLE_ENABLED
      break;
    }
    this->event_Manager.release(eventData);
  }
_procEnd:
  this->i2c_Server.stop();
#ifdef I2C_SERVER_TASK_DEBUG_CONSOLE_ENABLED
  CONSOLE_LOG("[i2cT] proc %i", 99);
#endif // I2C_SERVER_TASK_DEBUG_CONSOLE_ENABLED
}
#endif // I2C_COMM_TEST_SERVER_USE_TASK

int I2CServerTask::onEventSendRequested(unsigned char* data, unsigned int dataSize)
{
#ifdef I2C_SERVER_TASK_DEBUG_CONSOLE_ENABLED
  CONSOLE_LOG("[i2cT] sen %i", 0);
#endif // I2C_SERVER_TASK_DEBUG_CONSOLE_ENABLED
  int result = 0;
  BufferDataItem* sendDataItem = NULL;
  do
  {
    if ((dataSize != sizeof(EventCommI2CServerSendRequestedTAG))
      || (data == NULL))
    {
      break;
    }

    EventCommI2CServerSendRequestedTAG* eventData = (EventCommI2CServerSendRequestedTAG*)data;
    sendDataItem = this->i2c_Server.getCommData();
    if (sendDataItem == NULL)
    {
      break;
    }

    unsigned char* buffer = sendDataItem->bufferAddress();
    memset(buffer, 0, sizeof(CommI2CTestHeadlessDataTAG));
    CommI2CTestHeadlessDataTAG* data = (CommI2CTestHeadlessDataTAG*)(buffer);
    data->messageId = (uint8_t)eventData->messageId;
    data->nameLen = (uint8_t)5;
    data->commId = 1;
    memcpy(&data->name, "abcde", 5);
    result = sendDataItem->setDataLen(sizeof(CommI2CTestHeadlessDataTAG));
    if (result != 0)
    {
      break;
    }

#ifdef I2C_SERVER_TASK_DEBUG_CONSOLE_ENABLED
    CONSOLE_LOG("[i2cT] sen %i %i %i", eventData->clientAddress, eventData->messageId, sendDataItem->dataLength());
#endif // I2C_SERVER_TASK_DEBUG_CONSOLE_ENABLED
    /*CommI2CDataSendParams sendParam = CommI2CDataSendParams();
    sendParam.clientAddress = eventData->clientAddress;
    sendParam.inputBuffer = sendDataItem->bufferAddress();
    sendParam.inputBufferSize = sendDataItem->dataLength();*/
    result = this->i2c_Server.sendByAddress(eventData->clientAddress, sendDataItem->bufferAddress(), sendDataItem->dataLength());
    if (result != 0)
    {
#ifdef I2C_SERVER_TASK_DEBUG_CONSOLE_ENABLED
      CONSOLE_LOG("[i2cT] sen %i", -11);
#endif // I2C_SERVER_TASK_DEBUG_CONSOLE_ENABLED
      break;
    }

    if (sendDataItem != NULL)
    {
      this->i2c_Server.releaseCommData(sendDataItem);
    }
    return 0;
  } while (0);

  if (sendDataItem != NULL)
  {
    this->i2c_Server.releaseCommData(sendDataItem);
  }
#ifdef I2C_SERVER_TASK_DEBUG_CONSOLE_ENABLED
  CONSOLE_LOG("[i2cT] sen %i", -99);
#endif // I2C_SERVER_TASK_DEBUG_CONSOLE_ENABLED
  return -1;
}

int I2CServerTask::onEventDataRequested(unsigned char* data, unsigned int dataSize)
{
  int result = 0;
  //CONSOLE_LOG("[i2cT] req %i", 0);
  BufferDataItem* revDataItem = NULL;
  do
  {
    if ((dataSize != sizeof(EventCommI2CServerDataRequestedTAG))
      || (data == NULL))
    {
      break;
    }

    EventCommI2CServerDataRequestedTAG* requestEventData = (EventCommI2CServerDataRequestedTAG*)data;
#ifdef I2C_SERVER_TASK_DEBUG_CONSOLE_ENABLED
    CONSOLE_LOG("[i2cT] req %i %i %i", requestEventData->clientAddress, requestEventData->messageId, requestEventData->requestDataSize);
#endif // I2C_SERVER_TASK_DEBUG_CONSOLE_ENABLED
    {
      /*CommI2CDataRequestParams requestParam = CommI2CDataRequestParams();
      requestParam.clientAddress = requestEventData->clientAddress;
      requestParam.requestingDataSize = requestEventData->requestDataSize;
      requestParam.dataItem = NULL;*/
      byte tryTime = 1;
      bool isDisconnected = false;
      result = this->i2c_Server.requestByAddress(requestEventData->clientAddress, requestEventData->requestDataSize, tryTime, isDisconnected, revDataItem);
      if ((result != 0)
        || (revDataItem == NULL)
        )
      {
#ifdef I2C_SERVER_TASK_DEBUG_CONSOLE_ENABLED
        CONSOLE_LOG("[i2cT] req %i", -22);
#endif // I2C_SERVER_TASK_DEBUG_CONSOLE_ENABLED
        break;
      }
    }

    unsigned char* buffer = revDataItem->bufferAddress();
    CommI2CTestHeadlessDataTAG* data = (CommI2CTestHeadlessDataTAG*)(buffer);
#ifdef I2C_SERVER_TASK_DEBUG_CONSOLE_ENABLED
    CONSOLE_LOG("[i2cT] req %i %i %i", requestEventData->clientAddress, requestEventData->messageId, revDataItem->dataLength());
    CONSOLE_LOG("[i2cT] req %i %i %s", data->messageId, data->nameLen, data->name);
#endif // I2C_SERVER_TASK_DEBUG_CONSOLE_ENABLED
    if (revDataItem != NULL)
    {
      this->i2c_Server.releaseCommData(revDataItem);
    }

    return 0;
  } while (0);

  if (revDataItem != NULL)
  {
    this->i2c_Server.releaseCommData(revDataItem);
  }
#ifdef I2C_SERVER_TASK_DEBUG_CONSOLE_ENABLED
  CONSOLE_LOG("[i2cT] req %i", -99);
#endif // I2C_SERVER_TASK_DEBUG_CONSOLE_ENABLED
  return -1;
}


////////////////////Function to call from inside ISR///////////////////////
#ifdef I2C_COMM_TEST_SERVER_USE_TASK
int I2CServerTask::sendFromISR(uint8_t clientAddress, uint8_t messageId, bool* higherTaskWoken)
{
  int result = 0;
  do
  {

    if (this->isTaskRunningFromISR() == false)
    {
      break;
    }
    EventCommI2CServerSendRequestedTAG sendRequestData = EventCommI2CServerSendRequestedTAG();
    sendRequestData.clientAddress = clientAddress;
    sendRequestData.messageId = messageId;
    result = this->notifyFromISR(EventManagerConstant::CommI2CSendRequested, sizeof(EventCommI2CServerSendRequestedTAG), (unsigned char*)&sendRequestData, higherTaskWoken);
    if (result != 0)
    {
      break;
    }
    return 0;
  } while (0);

  return -1;
}

int I2CServerTask::requestFromISR(uint8_t clientAddress, uint8_t messageId, uint16_t requestDataSize, bool* higherTaskWoken)
{
  int result = 0;
  do
  {
    if (this->isTaskRunningFromISR() == false)
    {
      break;
    }
    EventCommI2CServerDataRequestedTAG dataRequestData = EventCommI2CServerDataRequestedTAG();
    dataRequestData.clientAddress = clientAddress;
    dataRequestData.messageId = messageId;
    dataRequestData.requestDataSize = requestDataSize;
    result = this->notifyFromISR(EventManagerConstant::CommI2CDataRequested, sizeof(EventCommI2CServerDataRequestedTAG), (unsigned char*)&dataRequestData, higherTaskWoken);
    if (result != 0)
    {
      break;
    }
    return 0;
  } while (0);

  return -1;
}
#endif // I2C_COMM_TEST_SERVER_USE_TASK
#endif // _CONF_TEST_TASK_COMM_I2C_SERVER_ENABLED