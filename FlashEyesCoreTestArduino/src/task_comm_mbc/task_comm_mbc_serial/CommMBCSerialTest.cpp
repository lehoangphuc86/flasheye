#include "../../FlashEyeCoreTestConfig.h"

#ifdef _CONF_TEST_TASK_COMM_MBC_SERIAL_ENABLED

/////////////////////////////////////////////////
// INCLUDE
#include "CommMBCSerialTest.h"
/////////////////////////////////////////////////
// PREPROCESSOR
#define COMM_MBC_SERIAL_TEST_DEBUG_CONSOLE_ENABLED
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

/* CommMBCSerialTest*/
CommMBCSerialTest::CommMBCSerialTest(byte sourceId)
#ifdef COMM_MBC_SERIAL_USE_TASK
  : TaskManager()
  , 
#else
  :
#endif // COMM_MBC_SERIAL_USE_TASK
  source_Id(sourceId)
{

}

CommMBCSerialTest::~CommMBCSerialTest(void)
{

}

#ifdef COMM_MBC_SERIAL_USE_TASK
int CommMBCSerialTest::setConfig(CommMBCSerialConfigTAG& commConfig, TaskManagerConfigTAG& taskConfig)
#else // COMM_MBC_SERIAL_USE_TASK
int CommMBCSerialTest::setConfig(CommMBCSerialConfigTAG& commConfig)
#endif // COMM_MBC_SERIAL_USE_TASK
{
  int result = 0;
  do
  {
    if (this->comm_Serial.isInitialized() != false)
    {
      return -1; // already config
    }
    
    this->comm_Serial.registerMessageId(CommMBCConstant::CommMBCMessageId::CommMBCHeadLess, sizeof(CommHeaderlesstDataTAG));
    this->comm_Serial.registerMessageId(CommMBCConstant::CommMBCMessageId::CommMBCStart1);
    this->comm_Serial.registerMessageId(CommMBCConstant::CommMBCMessageId::CommMBCResult1);
    result = this->comm_Serial.setConfig(commConfig);
    if (result != 0)
    {
      break;
    }
#ifdef COMM_MBC_SERIAL_USE_TASK
    TaskManager::registerHanldingEventStructSize(sizeof(EventCommMBCSerialStart1TAG));
    TaskManager::registerHanldingEventStructSize(sizeof(EventCommMBCSerialDataReceivedTAG));
    TaskManager::registerHanldingEventStructSize(sizeof(EventCommMBCSerialConnectionClosedTAG));
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
#endif // COMM_MBC_SERIAL_USE_TASK
    return 0;
  } while (0);
  this->stop();
  return -1;
}

int CommMBCSerialTest::prepare(void)
{
  int result = 0;
  do
  {
    result = this->comm_Serial.start();
    if (result != 0)
    {
      break;
    }
    return 0;
  } while (0);
  return -1;
}
#ifdef COMM_MBC_SERIAL_USE_TASK
int CommMBCSerialTest::start(TaskThreadConfigTAG& threadConfig)
#else // COMM_MBC_SERIAL_USE_TASK
int CommMBCSerialTest::start(void)
#endif // COMM_MBC_SERIAL_USE_TASK
{
  int result = 0;
  do
  {

#ifdef COMM_MBC_SERIAL_USE_TASK
    if (this->isTaskRunning() != false)
    {
      return 0;
    }
    //this->stop();
    result = TaskManager::startProcess(threadConfig, true);
    if (result != 0)
    {
      break;
    }
#else  // COMM_MBC_SERIAL_USE_TASK
    result = this->prepare();
#endif // COMM_MBC_SERIAL_USE_TASK
    if (result != 0)
    {
      break;
    }
    return 0;
  } while (0);
  this->stop();
  return -1;
}

int CommMBCSerialTest::connect(CommMBCSerialSocketConfigTAG& socketConfig)
{
  do
  {
#ifdef COMM_MBC_SERIAL_USE_TASK
    if (this->isTaskRunning() == false)
    {
      break;
    }
#endif // COMM_MBC_SERIAL_USE_TASK
    return this->comm_Serial.connect(socketConfig);
  } while (0);

  return -1;
}

int CommMBCSerialTest::disconnect(void)
{
  return this->comm_Serial.disconnect();
}

int CommMBCSerialTest::sendStart1(uint8_t param1, uint8_t param2, uint8_t nameLen, unsigned char* name)
{
  do
  {
#ifdef COMM_MBC_SERIAL_USE_TASK
    if (this->isTaskRunning() == false)
    {
      break;
    }
#endif // COMM_MBC_SERIAL_USE_TASK
    EventCommMBCSerialStart1TAG eventSendTest = EventCommMBCSerialStart1TAG();
    eventSendTest.param1 = param1;
    eventSendTest.param2 = param2;
    eventSendTest.nameLen = nameLen;
    memcpy(&eventSendTest.name, name, SYSTEM_MIN(nameLen, COMM_MBC_NAME_MAX_LEN));

#ifdef COMM_MBC_SERIAL_USE_TASK
    return this->notify(EventManagerConstant::EventMessageId::TestData, sizeof(EventCommMBCSerialStart1TAG), (unsigned char*)&eventSendTest);
#else // COMM_MBC_SERIAL_USE_TASK
    return this->onEventOnSendTestData((unsigned char*)&eventSendTest, sizeof(EventCommMBCSerialStart1TAG));
#endif // COMM_MBC_SERIAL_USE_TASK
  } while (0);
  return -1;
}

int CommMBCSerialTest::read(byte tryTime)
{
#ifdef COMM_MBC_SERIAL_USE_TASK
  return -1; // this is for non task only
#endif // COMM_MBC_SERIAL_USE_TASK

  BufferDataItem* dataItem = NULL;
  bool isDisconnected = false;
  byte messageId = CommMBCConstant::CommMBCMessageId::CommMBCMessageIdInvalid;

  do
  {
    this->comm_Serial.read(tryTime, messageId, dataItem, isDisconnected);
    if (isDisconnected != false)
    {
      EventCommMBCSerialConnectionClosedTAG eventData = EventCommMBCSerialConnectionClosedTAG();
      this->onEventOnConnectionClosed((unsigned char*)&eventData, sizeof(EventCommMBCSerialConnectionClosedTAG));
    }

    if (dataItem == NULL)
    {
      break;
    }

    EventCommMBCSerialDataReceivedTAG dataRevEventData = EventCommMBCSerialDataReceivedTAG();
    dataRevEventData.dataItem = dataItem;
    dataRevEventData.messageId = messageId;
    this->onEventOnDataReceived((unsigned char*)&dataRevEventData, sizeof(EventCommMBCSerialDataReceivedTAG));
    //this->comm_Serial.releaseCommData(requestDataParams.dataItem);

    return 0;
  } while (0);

  if (dataItem != NULL)
  {
    this->comm_Serial.releaseCommData(dataItem);
  }

  return -1;
}

int CommMBCSerialTest::stop(void)
{
  do
  {
#ifdef COMM_MBC_SERIAL_USE_TASK
    if (this->isTaskRunning() == false)
    {
      return 0;
    }
    
    TaskManager::stopProcess();
#else // COMM_MBC_SERIAL_USE_TASK
    this->comm_Serial.stop();
#endif // COMM_MBC_SERIAL_USE_TASK
    return 0;
  } while (0);
  
  return -1;
}

#ifdef COMM_MBC_SERIAL_USE_TASK
void CommMBCSerialTest::proc(void)
{
#ifdef COMM_MBC_SERIAL_TEST_DEBUG_CONSOLE_ENABLED
  CONSOLE_LOG("[mSeT] %i start", this->source_Id);
#endif // COMM_MBC_SERIAL_TEST_DEBUG_CONSOLE_ENABLED
  int prepareRet = this->prepare();
  this->reportPrepareResult(prepareRet);
  if (prepareRet != 0)
  {
    this->waitTerminating();
    goto _procEnd;
  }
  while (this->isTaskRunning() != false) // A Task shall never return or exit.
  {
    EventDataItem* eventData = this->event_Manager.wait(100);
    if (eventData == NULL)
    {
      //CONSOLE_LOG("[mSeT] %i", this->source_Id);
      //this->send(++seqId);
      continue;
    }

    switch (eventData->messageId())
    {
      case (int)EventManagerConstant::EventMessageId::TerminateProcess:
        this->isTaskRunning(false);
        break;
      case (int)EventManagerConstant::EventMessageId::TestData:
        this->onEventOnSendTestData(eventData->bufferAddress(), eventData->dataLength());
        break;
      case (int)EventManagerConstant::EventMessageId::CommDataReceived:
        this->onEventOnDataReceived(eventData->bufferAddress(), eventData->dataLength());
        break;
      case (int)EventManagerConstant::EventMessageId::CommConnectionClosed:
        this->onEventOnConnectionClosed(eventData->bufferAddress(), eventData->dataLength());
        break;
      default:
#ifdef COMM_MBC_SERIAL_TEST_DEBUG_CONSOLE_ENABLED
        CONSOLE_LOG("[mSeT] Rev -1 id=%i", eventData->messageId());
#endif // COMM_MBC_SERIAL_TEST_DEBUG_CONSOLE_ENABLED
        break;
    }
    this->event_Manager.release(eventData);
  }
_procEnd:
  this->comm_Serial.stop();
#ifdef COMM_MBC_SERIAL_TEST_DEBUG_CONSOLE_ENABLED
  CONSOLE_LOG("[mSeT] %i stopping", this->source_Id);
#endif // COMM_MBC_SERIAL_TEST_DEBUG_CONSOLE_ENABLED
}
#endif // COMM_MBC_SERIAL_USE_TASK

int CommMBCSerialTest::onEventOnSendTestData(unsigned char* data, unsigned int dataSize)
{
  //  CONSOLE_LOG("[pp] data %i", 0);
  int ret = 0;
  do
  {
    if ((dataSize != sizeof(EventCommMBCSerialStart1TAG))
      || (data == NULL))
    {
      break;
    }

    EventCommMBCSerialStart1TAG* eventData = (EventCommMBCSerialStart1TAG*)data;

    if (COMM_SERIAL_MBC_HEADERLESS == false)
    {
      CommMBCStart1TAG start1Data = CommMBCStart1TAG();
      start1Data.param1 = eventData->param1;
      start1Data.param2 = eventData->param2;
      start1Data.nameLen = eventData->nameLen;
      start1Data.name = eventData->name;

      ret = this->comm_Serial.send((uint8_t)CommMBCConstant::CommMBCMessageId::CommMBCStart1, (unsigned char*)&start1Data, sizeof(CommMBCStart1TAG));
      CONSOLE_LOG("[mSeT] sen %i %i %i", start1Data.param1, start1Data.param2, ret);
      CONSOLE_LOG("[mSeT] sen %i %s", start1Data.nameLen, start1Data.name);
      if (ret != 0)
      {
        break;
      }
      return 0;
    }
    
    CommHeaderlesstDataTAG headerlessData = CommHeaderlesstDataTAG();
    headerlessData.count = eventData->param1;
    headerlessData.sourceId = eventData->param2;
    headerlessData.nameLen = SYSTEM_MIN(COMM_HEADERLESS_NAME_LEN - 1, eventData->nameLen);
    memcpy(headerlessData.name, eventData->name, headerlessData.nameLen);
    headerlessData.nameLen = eventData->nameLen;
    
    ret = this->comm_Serial.send((uint8_t)CommMBCConstant::CommMBCMessageId::CommMBCHeadLess, (unsigned char*)&headerlessData, sizeof(CommHeaderlesstDataTAG));
    CONSOLE_LOG("[mSeT] senHL %i %i %i", headerlessData.count, headerlessData.sourceId, ret);
    CONSOLE_LOG("[mSeT] senHL %i %s", headerlessData.nameLen, headerlessData.name);
    if (ret != 0)
    {
      break;
    }
    return 0;
  } while (0);
  return -1;
}

void CommMBCSerialTest::onEventOnDataReceived(unsigned char* data, unsigned int dataSize)
{
//  CONSOLE_LOG("[mSeT] data %i", 0);
  BufferDataItem* dataItem = NULL;
//#ifdef COMM_MBC_SERIAL_TEST_DEBUG_CONSOLE_ENABLED
//  CONSOLE_LOG("[mSeT] rev %i", 0);
//#endif // COMM_MBC_SERIAL_TEST_DEBUG_CONSOLE_ENABLED
  do
  {
    //CONSOLE_LOG("[mSeT] rd %i", 0);
    if ((dataSize != sizeof(EventCommMBCSerialDataReceivedTAG))
      || (data == NULL))
    {
      break;
    }

    EventCommMBCSerialDataReceivedTAG* eventData = (EventCommMBCSerialDataReceivedTAG*)data;
    dataItem = eventData->dataItem;
    if ( (dataItem == NULL)
      || (dataItem->bufferAddress() == NULL)
      )
    {
      break;
    }

    unsigned char* revBuffer = dataItem->bufferAddress();
//#ifdef COMM_MBC_SERIAL_TEST_DEBUG_CONSOLE_ENABLED
//    CONSOLE_LOG("[mSeT] rev %i %i", 1, eventData->messageId);
//#endif // COMM_MBC_SERIAL_TEST_DEBUG_CONSOLE_ENABLED
    switch (eventData->messageId)
    {
      case (int)CommMBCConstant::CommMBCMessageId::CommMBCStart1:
      {
        CommMBCStart1TAG* start1Data = (CommMBCStart1TAG * )revBuffer;
        CONSOLE_LOG("[mSeT] revS1 %i %i", start1Data->param1, start1Data->param2);
        CONSOLE_LOG("[mSeT] senS1 %i %s", start1Data->nameLen, start1Data->name);


        CommMBCResult1TAG result1 = CommMBCResult1TAG();
        result1.status = start1Data->param2;
        result1.nameLen = start1Data->nameLen;
        result1.name = start1Data->name;

        this->comm_Serial.send((uint8_t)CommMBCConstant::CommMBCMessageId::CommMBCResult1, (unsigned char*)&result1, sizeof(CommMBCResult1TAG));
        break;
      }
      case (int)CommMBCConstant::CommMBCMessageId::CommMBCResult1:
      {
        CommMBCResult1TAG* result1Data = (CommMBCResult1TAG*)revBuffer;
        CONSOLE_LOG("[mSeT] revR1 %i %i", result1Data->status, result1Data->nameLen);
        CONSOLE_LOG("[mSeT] senR1 %s", result1Data->name);

        break;
      }
      case (int)CommMBCConstant::CommMBCMessageId::CommMBCHeadLess:
      {
        CommHeaderlesstDataTAG* testData = (CommHeaderlesstDataTAG*)revBuffer;
        CONSOLE_LOG("[mSeT] rev %i %i", testData->count, testData->sourceId);
        CONSOLE_LOG("[mSeT] rev %s", testData->name);
        break;
      }

      default:
        break;
    }
    
    if (dataItem != NULL)
    {
      this->comm_Serial.releaseCommData(dataItem);
    }
    return;
  } while (0);

  if (dataItem != NULL)
  {
    this->comm_Serial.releaseCommData(dataItem);
  }

  return;
}

void CommMBCSerialTest::onEventOnConnectionClosed(unsigned char* data, unsigned int dataSize)
{
  //  CONSOLE_LOG("[mSeT] data %i", 0);
  do
  {
    if ((dataSize != sizeof(EventCommMBCSerialConnectionClosedTAG))
      || (data == NULL))
    {
      break;
    }

    //EventCommMBCConnectionClosedTAG* eventData = (EventCommMBCConnectionClosedTAG*)data;
    CONSOLE_LOG("[mSeT] conClosed %i", 1);

    return;
  } while (0);
  return;
}

#ifdef COMM_MBC_SERIAL_USE_TASK
void CommMBCSerialTest::cbOnDataReceivedNavigator(void* param, DataSize_t paramSize)
{
  do
  {
    if ( (param == NULL)
      || (paramSize != sizeof(EventCommMBCSerialDataReceivedTAG))
      )
    {
      break;
    }

    EventCommMBCSerialDataReceivedTAG* eventData = (EventCommMBCSerialDataReceivedTAG*)param;
    CommMBCSerialTest* receiver = (CommMBCSerialTest*)eventData->cbParams;

    if (receiver == NULL)
    {
      break;
    }
    
    int ret = receiver->notify(EventManagerConstant::EventMessageId::CommDataReceived, (EventSize_t)paramSize, (unsigned char*)param);
    if ( (ret != 0)
      &&(eventData->dataItem != NULL)
      )
    {
      receiver->comm_Serial.releaseCommData(eventData->dataItem);
    }
    return;
  } while (0);
  return;
}

void CommMBCSerialTest::cbOnConnectionClosedNavigator(void* param, DataSize_t paramSize)
{
  do
  {
    if ((param == NULL)
      || (paramSize != sizeof(EventCommMBCSerialConnectionClosedTAG))
      )
    {
      break;
    }

    EventCommMBCSerialConnectionClosedTAG* eventData = (EventCommMBCSerialConnectionClosedTAG*)param;
    CommMBCSerialTest* receiver = (CommMBCSerialTest*)eventData->cbParams;

    if (receiver == NULL)
    {
      break;
    }

    receiver->notify(EventManagerConstant::EventMessageId::CommConnectionClosed, (EventSize_t)paramSize, (unsigned char*)param);
    return;
  } while (0);
  return;
}

#endif // COMM_MBC_SERIAL_USE_TASK

#endif // _CONF_TEST_TASK_COMM_MBC_SERIAL_ENABLED