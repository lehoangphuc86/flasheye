#include "PingpongTask.h"
#ifdef _CONF_TEST_TASK_MANAGER_ENABLED

/////////////////////////////////////////////////
// INCLUDE
#include "PingpongTask.h"
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

/* PingpongTask*/

PingpongTask::PingpongTask(void)
  : TaskManager()
  , is_Source(false)
  , base_Number(1)
  , time_Pace(1000)
  , partner_Task(NULL)
  , expected_SeqId(0)
{

}

PingpongTask::~PingpongTask(void)
{

}

int PingpongTask::setConfig(bool isSource, byte baseNumber, unsigned int timePace, PingpongTask* partner, TaskManagerConfigTAG& taskConfig)
{
  int result = 0;
  do
  {
    if (this->partner_Task != NULL)
    {
      return -1; // already config
    }

    if ((baseNumber == 0)
      || (timePace == 0)
      || (partner == NULL)
      )
    {
      break;
    }
    this->is_Source = isSource;
    this->base_Number = baseNumber;
    this->time_Pace = timePace;
    this->partner_Task = partner;

    //TaskManagerConfigTAG taskConfig = TaskManagerConfigTAG();
    //taskConfig.eventItemNumber = 5;
    //taskConfig.usingEvent = true;
    //taskConfig.threadConfig.enabled = true;
    //taskConfig.threadConfig.usStackDepth = 2048;
    //taskConfig.threadConfig.uxPriority = 2;

    TaskManager::registerHanldingEventStructSize(sizeof(EventPingpongTestDataTAG));

    result = TaskManager::setConfig(taskConfig);
    if (result != 0)
    {
      break;
    }

    return 0;
  } while (0);
  this->stop();
  return -1;
}

int PingpongTask::prepare(void)
{
  int result = 0;
  do
  {
    result = TaskManager::prepare();
    if (result != 0)
    {
      break;
    }

    return 0;
  } while (0);
  this->stop();
  return -1;
}

int PingpongTask::start(void)
{
  int result = 0;
  do
  {
    if (this->isTaskRunning() != false)
    {
      return -1;
    }

    if (this->partner_Task == NULL)
    {
      break;
    }
    result = TaskManager::startProcess();
    if (result != 0)
    {
      break;
    }
    return 0;
  } while (0);
  this->stop();
  return -1;
}

int PingpongTask::stop(void)
{
  do
  {
    if (this->isTaskRunning() == false)
    {
      return 0;
    }

    TaskManager::stopProcess();
    return 0;
  } while (0);
  
  return -1;
}

void PingpongTask::proc(void)
{
  //SYSTEM_TASK_MEM_DATA_TYPE usedMem = SYSTEM_TASK_MEM_USAGE(this->thread_Handler->nativeHandler());;
  //CONSOLE_LOG("[mem] proc0 %i %i", this->base_Number, usedMem);
  CONSOLE_LOG("[pp] %i starting", this->base_Number);
  byte seqId = 0;
  this->expected_SeqId = 0;
  int ret = 0;
  EventPingpongTestDataTAG testData = EventPingpongTestDataTAG();

  SYSTEM_TASK_MEM_DATA_TYPE usedMem = SYSTEM_TASK_MEM_USAGE(this->thread_Handler->nativeHandler());;
  CONSOLE_LOG("[mem] proc1 %i %i", this->base_Number, usedMem);
  while (this->isTaskRunning() != false) // A Task shall never return or exit.
  {
    EventDataItem* eventData = this->event_Manager.wait(this->time_Pace);
    if (eventData == NULL)
    {
      if (this->is_Source == false)
      {
        CONSOLE_LOG("[pp] Nul %i", this->base_Number);
        //SYSTEM_TASK_MEM_DATA_TYPE usedMem = SYSTEM_TASK_MEM_USAGE(this->thread_Handler->nativeHandler());;
        //CONSOLE_LOG("[mem] pp%i %i", this->base_Number, usedMem);
        continue;
      }
      testData.count = seqId;
      testData.sourceId = this->base_Number;
      ret = this->partner_Task->notify(EventManagerConstant::EventMessageId::TestData, sizeof(EventPingpongTestDataTAG), (unsigned char*)&testData);
      if (ret == 0)
      {
        CONSOLE_LOG("[pp] %i %i", this->base_Number, seqId);
        //SYSTEM_TASK_MEM_DATA_TYPE usedMem = SYSTEM_TASK_MEM_USAGE(this->thread_Handler->nativeHandler());;
        //CONSOLE_LOG("[mem] pp%i %i", this->base_Number, usedMem);
        seqId++;
      }
      continue;
    }

    //CONSOLE_LOG("[pp] Rev seqid=%i", eventData->id());
    switch (eventData->messageId())
    {
      case (int)EventManagerConstant::EventMessageId::TerminateProcess:
        this->isTaskRunning(false);
        break;
      case (int)EventManagerConstant::EventMessageId::TestData:
        this->onEventTestData(eventData->bufferAddress(), eventData->dataLength());
        break;
      default:
        CONSOLE_LOG("[pp] Rev -1 id=%i", eventData->messageId());
        break;
    }
    this->event_Manager.release(eventData);
  }
  CONSOLE_LOG("[pp] %i stopping", this->base_Number);
}

void PingpongTask::onEventTestData(unsigned char* data, unsigned int dataSize)
{
//  CONSOLE_LOG("[pp] data %i", 0);
  do
  {
    if ((dataSize != sizeof(EventPingpongTestDataTAG))
      || (data == NULL))
    {
      break;
    }
    SYSTEM_TASK_MEM_DATA_TYPE usedMem = SYSTEM_TASK_MEM_USAGE(this->thread_Handler->nativeHandler());;
    CONSOLE_LOG("[mem] pp te%i %i", this->base_Number, usedMem);
    EventPingpongTestDataTAG* eventData = (EventPingpongTestDataTAG*)data;
    if (eventData->sourceId == 4)
    {
      if (this->expected_SeqId == 0)
      {
        CONSOLE_LOG("[pp] data1st %i %i", eventData->sourceId, eventData->count);
        this->expected_SeqId = eventData->count;
      }
      
      if (this->expected_SeqId != eventData->count)
      {
        CONSOLE_LOG("[pp] dataError %i %i %i", this->expected_SeqId, eventData->sourceId, eventData->count);
        this->isTaskRunning(false);
      }
      else
      {
        CONSOLE_LOG("[pp] data %i %i", eventData->sourceId, eventData->count);
      }

      this->expected_SeqId++;
    }
    
    //CONSOLE_LOG("[pp] data %i %i", eventData->sourceId, eventData->count);
    
    return;
  } while (0);
  return;
}

#endif // _CONF_TEST_TASK_MANAGER_ENABLED