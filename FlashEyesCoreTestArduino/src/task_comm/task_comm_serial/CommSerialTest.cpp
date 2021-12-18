#include "../../FlashEyeCoreTestConfig.h"

#ifdef _CONF_TEST_TASK_COMM_SERIAL_ENABLED

/////////////////////////////////////////////////
// INCLUDE
#include "CommSerialTest.h"
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

/* CommSerialTest*/
CommSerialTest::CommSerialTest(byte sourceId)
#ifdef COMM_SERIAL_USE_TASK
  : TaskManager()
  , 
#else
  :
#endif // COMM_SERIAL_USE_TASK
  source_Id(sourceId)
{

}

CommSerialTest::~CommSerialTest(void)
{

}

#ifdef COMM_SERIAL_USE_TASK
int CommSerialTest::setConfig(CommSerialConfigTAG& commConfig, TaskManagerConfigTAG& taskConfig)
#else // COMM_SERIAL_USE_TASK
int CommSerialTest::setConfig(CommSerialConfigTAG& commConfig)
#endif // COMM_SERIAL_USE_TASK
{
  int result = 0;
  do
  {
    if (this->comm_Serial.isInitialized() != false)
    {
      return -1; // already config
    }
    result = this->comm_Serial.setConfig(commConfig);
    if (result != 0)
    {
      break;
    }
#ifdef COMM_SERIAL_USE_TASK
    TaskManager::registerHanldingEventStructSize(sizeof(EventCommSerialTestDataTAG));
    TaskManager::registerHanldingEventStructSize(sizeof(EventCommSerialDataReceivedTAG));
    TaskManager::registerHanldingEventStructSize(sizeof(EventCommSerialConnectionClosedTAG));
    
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
    return 0;
  } while (0);
  this->stop();
  return -1;
}

int CommSerialTest::prepare(void)
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

#ifdef COMM_SERIAL_USE_TASK
int CommSerialTest::start(TaskThreadConfigTAG& threadConfig)
#else // COMM_SERIAL_USE_TASK
int CommSerialTest::start(void)
#endif // COMM_SERIAL_USE_TASK
{
  int result = 0;
  do
  {

#ifdef COMM_SERIAL_USE_TASK
    if (this->isTaskRunning() != false)
    {
      return 0;
    }
    //this->stop();
    result = TaskManager::startProcess(threadConfig, true);
    if (result != 0)
    {
      //CONSOLE_LOG("[seT] sta %i", -11);
      break;
    }

    //result = this->waitPrepareResult();
#else  // COMM_SERIAL_USE_TASK
    result = this->prepare();
#endif // COMM_SERIAL_USE_TASK
    if (result != 0)
    {
      break;
    }
    return 0;
  } while (0);
  this->stop();
  return -1;
}

int CommSerialTest::connect(CommSerialSocketConfigTAG& socketConfig)
{
  do
  {
#ifdef COMM_SERIAL_USE_TASK
    if (this->isTaskRunning() == false)
    {
      break;
    }
#endif // COMM_SERIAL_USE_TASK
    return this->comm_Serial.connect(socketConfig);
  } while (0);

  return -1;
}

int CommSerialTest::disconnect(void)
{
  return this->comm_Serial.disconnect();
}

int CommSerialTest::send(byte count)
{
  do
  {
#ifdef COMM_SERIAL_USE_TASK
    if (this->isTaskRunning() == false)
    {
      break;
    }
#endif // COMM_SERIAL_USE_TASK
    EventCommSerialTestDataTAG eventSendTest = EventCommSerialTestDataTAG();
    eventSendTest.count = count;

#ifdef COMM_SERIAL_USE_TASK
    return this->notify(EventManagerConstant::EventMessageId::TestData, sizeof(EventCommSerialTestDataTAG), (unsigned char*)&eventSendTest);
#else // COMM_SERIAL_USE_TASK
    return this->onEventOnSendTestData((unsigned char*)&eventSendTest, sizeof(EventCommSerialTestDataTAG));
#endif // COMM_SERIAL_USE_TASK
  } while (0);
  return -1;
}

int CommSerialTest::read(byte tryTime)
{
#ifdef COMM_SERIAL_USE_TASK
  return -1; // this is for non task only
#endif // COMM_SERIAL_USE_TASK

  //BufferDataItem* dataItem = NULL;
  //bool isDisconnected = false;
  CommSerialDataRequestParamsTAG requestDataParams = CommSerialDataRequestParamsTAG();
  requestDataParams.inTryTime = tryTime;
  do
  {
    this->comm_Serial.read(requestDataParams);

    if (requestDataParams.isDisconnected != false)
    {
      EventCommSerialConnectionClosedTAG eventData = EventCommSerialConnectionClosedTAG();
      this->onEventOnConnectionClosed((unsigned char*)&eventData, sizeof(EventCommSerialConnectionClosedTAG));
    }

    if (requestDataParams.dataItem == NULL)
    {
      break;
    }

    EventCommSerialDataReceivedTAG dataRevEventData = EventCommSerialDataReceivedTAG();
    dataRevEventData.dataItem = requestDataParams.dataItem;
    this->onEventOnDataReceived((unsigned char*)&dataRevEventData, sizeof(EventCommSerialDataReceivedTAG));
    //this->comm_Serial.releaseCommData(requestDataParams.dataItem);

    return 0;
  } while (0);

  if (requestDataParams.dataItem != NULL)
  {
    this->comm_Serial.releaseCommData(requestDataParams.dataItem);
  }

  return -1;
}

int CommSerialTest::stop(void)
{
  do
  {
#ifdef COMM_SERIAL_USE_TASK
    if (this->isTaskRunning() == false)
    {
      return 0;
    }
    
    TaskManager::stopProcess();
#else // COMM_SERIAL_USE_TASK
    this->comm_Serial.stop();
#endif // COMM_SERIAL_USE_TASK
    return 0;
  } while (0);
  
  return -1;
}

#ifdef COMM_SERIAL_USE_TASK
void CommSerialTest::proc(void)
{
  CONSOLE_LOG("[seT] proc %i %i", 0, this->source_Id);
  int prepareRet = this->prepare();
  this->reportPrepareResult(prepareRet);
  CONSOLE_LOG("[seT] proc %i %i", 1, prepareRet);
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
      //CONSOLE_LOG("[seT] %i", this->source_Id);
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
        CONSOLE_LOG("[seT] Rev -1 id=%i", eventData->messageId());
        break;
    }
    this->event_Manager.release(eventData);
  }
_procEnd:
  this->comm_Serial.stop();
  CONSOLE_LOG("[seT] %i stopping", this->source_Id);
}
#endif // COMM_SERIAL_USE_TASK

int CommSerialTest::onEventOnSendTestData(unsigned char* data, unsigned int dataSize)
{
  //  CONSOLE_LOG("[pp] data %i", 0);
  int ret = 0;
  do
  {
    if ((dataSize != sizeof(EventCommSerialTestDataTAG))
      || (data == NULL))
    {
      break;
    }

    EventCommSerialTestDataTAG* eventData = (EventCommSerialTestDataTAG*)data;
    static CommHeaderlesstDataTAG testData = CommHeaderlesstDataTAG();
    testData.count = eventData->count;
    testData.sourceId = this->source_Id;
    testData.nameLen = COMM_HEADERLESS_NAME_LEN - 2;
    for (size_t idx = 0; idx < COMM_HEADERLESS_NAME_LEN; idx++)
    {
      testData.name[idx] = 'f';
    }
    //testData.testDump[2] = '\r';
    testData.name[COMM_HEADERLESS_NAME_LEN - 2] = 0;
    testData.name[COMM_HEADERLESS_NAME_LEN - 1] = '\n';

    CommSerialSendRequestedParamsTAG sendRequestParams = CommSerialSendRequestedParamsTAG();
    sendRequestParams.inputBuffer = (unsigned char*)&testData;
    sendRequestParams.inputBufferSize = sizeof(CommHeaderlesstDataTAG);

    ret = this->comm_Serial.send(sendRequestParams);
    CONSOLE_LOG("[seT] sen %i ret=%i", eventData->count, ret);

    return 0;
  } while (0);
  return -1;
}

void CommSerialTest::onEventOnDataReceived(unsigned char* data, unsigned int dataSize)
{
#ifdef COMM_PROC_TIME_MEA_ENABLE
  unsigned long revTime = SYSTEM_TIME_MICROS();
#endif // COMM_PROC_TIME_MEA_ENABLE
  //  CONSOLE_LOG("[pp] data %i", 0);
  BufferDataItem* dataItem = NULL;
  do
  {
    //CONSOLE_LOG("[seT] rd %i", 0);
    if ((dataSize != sizeof(EventCommSerialDataReceivedTAG))
      || (data == NULL))
    {
      CONSOLE_LOG("[seT] rd %i", 1);
      break;
    }

    EventCommSerialDataReceivedTAG* eventData = (EventCommSerialDataReceivedTAG*)data;
#ifdef COMM_PROC_TIME_MEA_ENABLE
    unsigned long startTime = eventData->meaTime.procTimes[COMM_PROC_TIME_MEA_REV_START];
    CONSOLE_LOG("[seT]t %lu %lu"
      , startTime - startTime
      , eventData->meaTime.procTimes[COMM_PROC_TIME_MEA_REV_BODY] - startTime
    );

    CONSOLE_LOG("[seT]t %lu"
      , eventData->meaTime.procTimes[COMM_PROC_TIME_MEA_SERIAL_REV] - startTime
    );
#endif //COMM_PROC_TIME_MEA_ENABLE
    dataItem = eventData->dataItem;
    //CONSOLE_LOG("[seT] rd %i %i", 11, dataItem->dataLength());
    if ((dataItem == NULL)
      || (dataItem->dataLength() != sizeof(CommHeaderlesstDataTAG))
      )
    {
      CONSOLE_LOG("[seT] rd %i %i", 2, dataItem->dataLength());
      break;
    }

    CommHeaderlesstDataTAG* testData = (CommHeaderlesstDataTAG*)dataItem->bufferAddress();
    CONSOLE_LOG("[seT] rev %i %i", testData->count, testData->sourceId);
    //CONSOLE_LOG("[seT] rev %s", testData->name);

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

void CommSerialTest::onEventOnConnectionClosed(unsigned char* data, unsigned int dataSize)
{
  //  CONSOLE_LOG("[pp] data %i", 0);
  do
  {
    if ((dataSize != sizeof(EventCommSerialConnectionClosedTAG))
      || (data == NULL))
    {
      break;
    }

    //EventCommConnectionClosedTAG* eventData = (EventCommConnectionClosedTAG*)data;
    CONSOLE_LOG("[seT] conClosed %i", 1);

    return;
  } while (0);
  return;
}

#ifdef COMM_SERIAL_USE_TASK
void CommSerialTest::cbOnDataReceivedNavigator(void* param, DataSize_t paramSize)
{
  do
  {
    if ( (param == NULL)
      || (paramSize != sizeof(EventCommSerialDataReceivedTAG))
      )
    {
      break;
    }

    EventCommSerialDataReceivedTAG* eventData = (EventCommSerialDataReceivedTAG*)param;
    CommSerialTest* receiver = (CommSerialTest *)eventData->cbParams;

    if (receiver == NULL)
    {
      break;
    }
    
    int ret = receiver->notify((EventId_t)EventManagerConstant::EventMessageId::CommDataReceived, (EventSize_t)paramSize, (unsigned char*)param);
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

void CommSerialTest::cbOnConnectionClosedNavigator(void* param, DataSize_t paramSize)
{
  do
  {
    if ((param == NULL)
      || (paramSize != sizeof(EventCommSerialConnectionClosedTAG))
      )
    {
      break;
    }

    EventCommSerialConnectionClosedTAG* eventData = (EventCommSerialConnectionClosedTAG*)param;
    CommSerialTest* receiver = (CommSerialTest*)eventData->cbParams;

    if (receiver == NULL)
    {
      break;
    }

    receiver->notify((EventId_t)EventManagerConstant::EventMessageId::CommConnectionClosed, (EventSize_t)paramSize, (unsigned char*)param);
    return;
  } while (0);
  return;
}

#endif // COMM_SERIAL_USE_TASK

#endif // _CONF_TEST_TASK_COMM_SERIAL_ENABLED