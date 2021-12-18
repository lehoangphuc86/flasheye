#include "MBCI2CServerTask.h"

#ifdef _CONF_TEST_TASK_COMM_MBC_I2C_SERVER_ENABLED
/////////////////////////////////////////////////
// INCLUDE

/////////////////////////////////////////////////
// PREPROCESSOR
#define MBC_I2C_SERVER_TASK_DEBUG_CONSOLE_ENABLED
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

/* MBCI2CServerTask*/

////////////////////Function to call from outside ISR///////////////////////
MBCI2CServerTask::MBCI2CServerTask(void)
#ifdef MBC_I2C_COMM_TEST_SERVER_USE_TASK
  : TaskManager()
#endif // MBC_I2C_COMM_TEST_SERVER_USE_TASK
{
  memset(&this->mbc_i2c_Socket_Config, 0, sizeof(CommMBCI2CSocketConfigTAG));
}
MBCI2CServerTask::~MBCI2CServerTask(void)
{

}


#ifdef MBC_I2C_COMM_TEST_SERVER_USE_TASK
int MBCI2CServerTask::setConfig(CommMBCI2CConfigTAG& commConfig,TaskManagerConfigTAG& taskConfigStruct)
#else
int MBCI2CServerTask::setConfig(CommMBCI2CConfigTAG& commConfig)
#endif // MBC_I2C_COMM_TEST_SERVER_USE_TASK
{
  do
  {
    int result = 0;
    if (this->mbc_i2c_Server.isValidSocket() != false)
    {
      return -1; // already config
    }

    // set up server
    this->mbc_i2c_Server.registerMessageId(CommMBCConstant::CommMBCMessageId::CommMBCHeadLess, sizeof(CommHeaderlesstDataTAG));
    this->mbc_i2c_Server.registerMessageId(CommMBCConstant::CommMBCMessageId::CommMBCStart1);
    this->mbc_i2c_Server.registerMessageId(CommMBCConstant::CommMBCMessageId::CommMBCResult1);

    result = this->mbc_i2c_Server.setConfig(commConfig);
    if (result != 0)
    {
      break;
    }

    //result = this->mbc_i2c_Server.start();
    //if (result != 0)
    //{
    //  break;
    //}

#ifdef MBC_I2C_COMM_TEST_SERVER_USE_TASK
    // Set up tasks
    this->registerHanldingEventStructSize(sizeof(EventCommMBCI2CServerSendRequestedTAG));
    this->registerHanldingEventStructSize(sizeof(EventCommMBCI2CServerDataRequestedTAG));
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
#endif // MBC_I2C_COMM_TEST_SERVER_USE_TASK

    // append client
    {
      CommI2CClientPropertiesTAG clientProperties = CommI2CClientPropertiesTAG();
      clientProperties.address = MBC_I2C_COMM_TEST_SERVER_CLIENT_SOCKET_1_ADDRESS;
      clientProperties.headerless = MBC_I2C_COMM_TEST_SERVER_CLIENT_SOCKET_1_HEADRLESS;
      clientProperties.checkSum = MBC_I2C_COMM_TEST_SERVER_CLIENT_SOCKET_1_CHECKSUM;
      clientProperties.codecType = MBC_I2C_COMM_TEST_SERVER_CLIENT_SOCKET_1_CODECTPYE;
      result = this->mbc_i2c_Server.appendClient(&clientProperties);
      if (result != 0)
      {
        break;
      }
      clientProperties.address = MBC_I2C_COMM_TEST_SERVER_CLIENT_SOCKET_2_ADDRESS;
      clientProperties.headerless = MBC_I2C_COMM_TEST_SERVER_CLIENT_SOCKET_2_HEADRLESS;
      clientProperties.checkSum = MBC_I2C_COMM_TEST_SERVER_CLIENT_SOCKET_2_CHECKSUM;
      clientProperties.codecType = MBC_I2C_COMM_TEST_SERVER_CLIENT_SOCKET_2_CODECTPYE;
      result = this->mbc_i2c_Server.appendClient(&clientProperties);
      if (result != 0)
      {
        break;
      }

    }
    
    return result;
  } while (0);
  return -1;
}

int MBCI2CServerTask::prepare(void)
{
  return this->mbc_i2c_Server.start(this->mbc_i2c_Socket_Config);
}

#ifdef MBC_I2C_COMM_TEST_SERVER_USE_TASK
int MBCI2CServerTask::start(CommMBCI2CSocketConfigTAG& commI2CSocketConfig, TaskThreadConfigTAG& threadConfig)
#else // MBC_I2C_COMM_TEST_SERVER_USE_TASK
int MBCI2CServerTask::start(CommMBCI2CSocketConfigTAG& commI2CSocketConfig)
#endif // MBC_I2C_COMM_TEST_SERVER_USE_TASK
{
  int result = 0;
  do
  {

    if (commI2CSocketConfig.isValid() == false)
    {
      break;
    }

    memcpy(&this->mbc_i2c_Socket_Config, &commI2CSocketConfig, sizeof(CommMBCI2CSocketConfigTAG));

#ifdef MBC_I2C_COMM_TEST_SERVER_USE_TASK
    if ((this->isTaskRunning() != false)
      || (this->threadHandler() != NULL)
      )
    {
      break;
    }

    result = TaskManager::startProcess(threadConfig, true);
#else // MBC_I2C_COMM_TEST_SERVER_USE_TASK
    result = this->prepare();
#endif // MBC_I2C_COMM_TEST_SERVER_USE_TASK
    if (result != 0)
    {
      break;
    }
    return 0;
  } while (0);
  return -1;
}


int MBCI2CServerTask::send(uint8_t clientAddress, uint8_t messageId)
{
  int result = 0;
  do
  {
#ifdef MBC_I2C_COMM_TEST_SERVER_USE_TASK
    if ( this->isTaskRunning() == false)
    {
      break;
    }
#endif // MBC_I2C_COMM_TEST_SERVER_USE_TASK
    EventCommMBCI2CServerSendRequestedTAG sendRequestData = EventCommMBCI2CServerSendRequestedTAG();
    sendRequestData.clientAddress = clientAddress;
    sendRequestData.messageId = messageId;
#ifdef MBC_I2C_COMM_TEST_SERVER_USE_TASK
    return this->notify(EventManagerConstant::CommI2CSendRequested, sizeof(EventCommMBCI2CServerSendRequestedTAG), (unsigned char*)&sendRequestData);
#else // MBC_I2C_COMM_TEST_SERVER_USE_TASK
    return this->onEventSendRequested((unsigned char*)&sendRequestData, sizeof(EventCommMBCI2CServerSendRequestedTAG));
#endif // MBC_I2C_COMM_TEST_SERVER_USE_TASK
  } while (0);
  return -1;
}

int MBCI2CServerTask::request(uint8_t clientAddress, uint8_t messageId, uint16_t requestDataSize)
{
  int result = 0;
  do
  {
#ifdef MBC_I2C_COMM_TEST_SERVER_USE_TASK
    if (this->isTaskRunning() == false)
    {
      break;
    }
#endif // MBC_I2C_COMM_TEST_SERVER_USE_TASK

    EventCommMBCI2CServerDataRequestedTAG dataRequestData = EventCommMBCI2CServerDataRequestedTAG();
    dataRequestData.clientAddress = clientAddress;
    dataRequestData.messageId = messageId;
    dataRequestData.requestDataSize = requestDataSize;

#ifdef MBC_I2C_COMM_TEST_SERVER_USE_TASK
    return this->notify(EventManagerConstant::CommI2CDataRequested, sizeof(EventCommMBCI2CServerDataRequestedTAG), (unsigned char*)&dataRequestData);
#else // MBC_I2C_COMM_TEST_SERVER_USE_TASK
    return this->onEventDataRequested((unsigned char*)&dataRequestData, sizeof(EventCommMBCI2CServerDataRequestedTAG));
#endif // MBC_I2C_COMM_TEST_SERVER_USE_TASK
  } while (0);

  return -1;
}

int MBCI2CServerTask::stop(void)
{
  do
  {
#ifdef MBC_I2C_COMM_TEST_SERVER_USE_TASK
    if (this->isTaskRunning() == false)
    {
      return 0;
    }
    TaskManager::stopProcess();
#else // MBC_I2C_COMM_TEST_SERVER_USE_TASK
    this->mbc_i2c_Server.stop();
#endif // MBC_I2C_COMM_TEST_SERVER_USE_TASK
    return 0;
  } while (0);

  return -1;
}

#ifdef MBC_I2C_COMM_TEST_SERVER_USE_TASK
void MBCI2CServerTask::proc(void)
{
#ifdef MBC_I2C_SERVER_TASK_DEBUG_CONSOLE_ENABLED
  CONSOLE_LOG("[i2cT] proc %i", 0);
#endif // MBC_I2C_SERVER_TASK_DEBUG_CONSOLE_ENABLED
  int prepareRet = this->prepare();
  this->reportPrepareResult(prepareRet);
#ifdef MBC_I2C_SERVER_TASK_DEBUG_CONSOLE_ENABLED
  CONSOLE_LOG("[i2cT] proc %i %i", 1, prepareRet);
#endif // MBC_I2C_SERVER_TASK_DEBUG_CONSOLE_ENABLED
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
#ifdef MBC_I2C_SERVER_TASK_DEBUG_CONSOLE_ENABLED
      CONSOLE_LOG("[i2cT] evt NL %s", "");
#endif // MBC_I2C_SERVER_TASK_DEBUG_CONSOLE_ENABLED
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
  this->mbc_i2c_Server.stop();
#ifdef MBC_I2C_SERVER_TASK_DEBUG_CONSOLE_ENABLED
  CONSOLE_LOG("[i2cT] proc %i", 99);
#endif // MBC_I2C_SERVER_TASK_DEBUG_CONSOLE_ENABLED
}
#endif // MBC_I2C_COMM_TEST_SERVER_USE_TASK

int MBCI2CServerTask::onEventSendRequested(unsigned char* data, unsigned int dataSize)
{
//#ifdef MBC_I2C_SERVER_TASK_DEBUG_CONSOLE_ENABLED
//  CONSOLE_LOG("[i2cT] sen %i", 0);
//#endif // MBC_I2C_SERVER_TASK_DEBUG_CONSOLE_ENABLED
  int result = 0;
  do
  {
    if ((dataSize != sizeof(EventCommMBCI2CServerSendRequestedTAG))
      || (data == NULL))
    {
      break;
    }
    EventCommMBCI2CServerSendRequestedTAG* eventData = (EventCommMBCI2CServerSendRequestedTAG*)data;
#ifdef MBC_I2C_SERVER_TASK_DEBUG_CONSOLE_ENABLED
    CONSOLE_LOG("[i2cT] sen %i %i", 1, eventData->messageId);
#endif // MBC_I2C_SERVER_TASK_DEBUG_CONSOLE_ENABLED
    switch (eventData->messageId)
    {
      case (int)CommMBCConstant::CommMBCMessageId::CommMBCStart1:
      {
        CommMBCStart1TAG start1Data = CommMBCStart1TAG();
        unsigned char name[] = "abcde";
        start1Data.name = name;
        start1Data.nameLen = 5;
        start1Data.param1 = 1;
        start1Data.param2 = 2;
        this->mbc_i2c_Server.sendByAddress(eventData->clientAddress, eventData->messageId, (unsigned char*)&start1Data, sizeof(CommMBCStart1TAG));
        break;
      }
      case (int)CommMBCConstant::CommMBCMessageId::CommMBCResult1:
      {
        CommMBCResult1TAG result1Data = CommMBCResult1TAG();
        unsigned char name[] = "abcde";
        result1Data.name = name;
        result1Data.nameLen = 5;
        result1Data.status = 1;
        this->mbc_i2c_Server.sendByAddress(eventData->clientAddress, eventData->messageId, (unsigned char*)&result1Data, sizeof(CommMBCResult1TAG));
        break;
      }
      case (int)CommMBCConstant::CommMBCMessageId::CommMBCHeadLess:
      {
        CommHeaderlesstDataTAG headerlessData = CommHeaderlesstDataTAG();
        unsigned char name[] = "abcde";
        headerlessData.sourceId = 0;
        headerlessData.nameLen = 5;
        memcpy(headerlessData.name, name, 5);
        headerlessData.count = 2;
        this->mbc_i2c_Server.sendByAddress(eventData->clientAddress, eventData->messageId, (unsigned char*)&headerlessData, sizeof(CommHeaderlesstDataTAG));
        break;
      }
      default:
        break;
    }

    return 0;
  } while (0);

#ifdef MBC_I2C_SERVER_TASK_DEBUG_CONSOLE_ENABLED
  CONSOLE_LOG("[i2cT] sen %i", -99);
#endif // MBC_I2C_SERVER_TASK_DEBUG_CONSOLE_ENABLED
  return -1;
}

int MBCI2CServerTask::onEventDataRequested(unsigned char* data, unsigned int dataSize)
{
  int result = 0;
  //CONSOLE_LOG("[i2cT] req %i", 0);
  BufferDataItem* revDataItem = NULL;
  do
  {
    if ((dataSize != sizeof(EventCommMBCI2CServerDataRequestedTAG))
      || (data == NULL))
    {
      break;
    }

    EventCommMBCI2CServerDataRequestedTAG* requestEventData = (EventCommMBCI2CServerDataRequestedTAG*)data;
    DataSize_t requestSize = 0;
    


#ifdef MBC_I2C_SERVER_TASK_DEBUG_CONSOLE_ENABLED
    CONSOLE_LOG("[i2cT] req %i %i %i", requestEventData->clientAddress, requestEventData->messageId, requestEventData->requestDataSize);
#endif // MBC_I2C_SERVER_TASK_DEBUG_CONSOLE_ENABLED
    {
      byte tryTime = 1;
      bool isDisconnected = false;
      result = this->mbc_i2c_Server.requestByAddress(requestEventData->clientAddress, requestEventData->messageId, requestEventData->requestDataSize, tryTime, isDisconnected, revDataItem);
      if ((result != 0)
        || (revDataItem == NULL)
        )
      {
#ifdef MBC_I2C_SERVER_TASK_DEBUG_CONSOLE_ENABLED
        CONSOLE_LOG("[i2cT] req %i", -22);
#endif // MBC_I2C_SERVER_TASK_DEBUG_CONSOLE_ENABLED
        break;
      }
    }
    unsigned char* revBuffer = revDataItem->bufferAddress();
    switch (requestEventData->messageId)
    {
      case (int)CommMBCConstant::CommMBCMessageId::CommMBCStart1:
      {
        CommMBCStart1TAG* start1Data = (CommMBCStart1TAG*)revBuffer;
        CONSOLE_LOG("[i2cT] revS1 %i %i", start1Data->param1, start1Data->param2);
        CONSOLE_LOG("[i2cT] senS1 %i %s", start1Data->nameLen, start1Data->name);
        break;
      }
      case (int)CommMBCConstant::CommMBCMessageId::CommMBCResult1:
      {
        CommMBCResult1TAG* result1Data = (CommMBCResult1TAG*)revBuffer;
        CONSOLE_LOG("[i2cT] revR1 %i %i", result1Data->status, result1Data->nameLen);
        CONSOLE_LOG("[i2cT] senR1 %s", result1Data->name);
        break;
      }
      case (int)CommMBCConstant::CommMBCMessageId::CommMBCHeadLess:
      {
        CommHeaderlesstDataTAG* testData = (CommHeaderlesstDataTAG*)revBuffer;
        CONSOLE_LOG("[i2cT] rev %i %i", testData->count, testData->sourceId);
        CONSOLE_LOG("[i2cT] rev %s", testData->name);
        break;
      }

      default:
        break;
    }

    if (revDataItem != NULL)
    {
      this->mbc_i2c_Server.releaseCommData(revDataItem);
    }
    return 0;
  } while (0);

  if (revDataItem != NULL)
  {
    this->mbc_i2c_Server.releaseCommData(revDataItem);
  }
#ifdef MBC_I2C_SERVER_TASK_DEBUG_CONSOLE_ENABLED
  CONSOLE_LOG("[i2cT] req %i", -99);
#endif // MBC_I2C_SERVER_TASK_DEBUG_CONSOLE_ENABLED
  return -1;
}


////////////////////Function to call from inside ISR///////////////////////
#ifdef MBC_I2C_COMM_TEST_SERVER_USE_TASK
int MBCI2CServerTask::sendFromISR(uint8_t clientAddress, uint8_t messageId, bool* higherTaskWoken)
{
  int result = 0;
  do
  {

    if (this->isTaskRunningFromISR() == false)
    {
      break;
    }
    EventCommMBCI2CServerSendRequestedTAG sendRequestData = EventCommMBCI2CServerSendRequestedTAG();
    sendRequestData.clientAddress = clientAddress;
    sendRequestData.messageId = messageId;
    result = this->notifyFromISR(EventManagerConstant::CommI2CSendRequested, sizeof(EventCommMBCI2CServerSendRequestedTAG), (unsigned char*)&sendRequestData, higherTaskWoken);
    if (result != 0)
    {
      break;
    }
    return 0;
  } while (0);

  return -1;
}

int MBCI2CServerTask::requestFromISR(uint8_t clientAddress, uint8_t messageId, uint16_t requestDataSize, bool* higherTaskWoken)
{
  int result = 0;
  do
  {
    if (this->isTaskRunningFromISR() == false)
    {
      break;
    }
    EventCommMBCI2CServerDataRequestedTAG dataRequestData = EventCommMBCI2CServerDataRequestedTAG();
    dataRequestData.clientAddress = clientAddress;
    dataRequestData.messageId = messageId;
    dataRequestData.requestDataSize = requestDataSize;
    result = this->notifyFromISR(EventManagerConstant::CommI2CDataRequested, sizeof(EventCommMBCI2CServerDataRequestedTAG), (unsigned char*)&dataRequestData, higherTaskWoken);
    if (result != 0)
    {
      break;
    }
    return 0;
  } while (0);

  return -1;
}
#endif // MBC_I2C_COMM_TEST_SERVER_USE_TASK
#endif // _CONF_TEST_TASK_COMM_MBC_I2C_SERVER_ENABLED