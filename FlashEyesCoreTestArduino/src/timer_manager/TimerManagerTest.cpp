#include "../FlashEyeCoreTestConfig.h"

#ifdef __CONF_TEST_TIMER_MANAGER_FUNC_ENABLED

/////////////////////////////////////////////////
// INCLUDE
#include "TimerManagerTest.h"
/////////////////////////////////////////////////
// PREPROCESSOR
#define TIMER_MANAGER_TEST_TASK_CONSOLE_DEBUG_ENABLED
/////////////////////////////////////////////////
// DEFINE

/////////////////////////////////////////////////
// MARCO

/////////////////////////////////////////////////
// GLOBAL VARIABLES
#ifdef TIMER_MANAGER_TEST_TASK_CONSOLE_DEBUG_ENABLED
char logBuf[SYSTEM_CONSOLE_OUT_BUF_LEN];
#endif // TIMER_MANAGER_TEST_TASK_CONSOLE_DEBUG_ENABLED
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

/* TimerManagerTest*/
TimerManagerTest::TimerManagerTest(void)
  : TaskManager()
{

}

TimerManagerTest::~TimerManagerTest(void)
{

}

int TimerManagerTest::setConfig(TaskManagerConfigTAG& taskConfig)
{
  int result = 0;
  do
  {
    TaskManager::registerHanldingEventStructSize(sizeof(EventTimerFiredTAG));
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
    return 0;
  } while (0);
  this->stop();
  return -1;
}

int TimerManagerTest::prepare(void)
{
  //int result = 0;
  do
  {
    return 0;
  } while (0);
  return -1;
}


int TimerManagerTest::start(TaskThreadConfigTAG& threadConfig)
{
  int result = 0;
  do
  {
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
    if (result != 0)
    {
      break;
    }
    return 0;
  } while (0);
  this->stop();
  return -1;
}

int TimerManagerTest::stop(void)
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

#ifdef TIMER_MANAGER_TEST_NO_BUTTON
extern void startTestTimers(void);
#endif // TIMER_MANAGER_TEST_NO_BUTTON
void TimerManagerTest::proc(void)
{
  CONSOLE_LOG("[timT] proc %i", 0);
  int prepareRet = this->prepare();
  this->reportPrepareResult(prepareRet);
#ifdef TIMER_MANAGER_TEST_NO_BUTTON
  bool started = false;
#endif // TIMER_MANAGER_TEST_NO_BUTTON
  if (prepareRet != 0)
  {
    this->waitTerminating();
    goto _procEnd;
  }
  while (this->isTaskRunning() != false) // A Task shall never return or exit.
  {
    EventDataItem* eventData = this->event_Manager.wait(1000);
#ifdef TIMER_MANAGER_TEST_NO_BUTTON
    if (started == false)
    {
      started = true;
      startTestTimers();
    }
#endif // TIMER_MANAGER_TEST_NO_BUTTON
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
      case (int)EventManagerConstant::EventMessageId::TimerFired1:
        this->onEventTimerFired(eventData->bufferAddress(), eventData->dataLength());
        break;
      case (int)EventManagerConstant::EventMessageId::TimerFired2:
        this->onEventTimerFiredResult(eventData->bufferAddress(), eventData->dataLength());
        break;
      default:
        CONSOLE_LOG("[timT] Rev -1 id=%i", eventData->messageId());
        break;
    }
    this->event_Manager.release(eventData);
  }
_procEnd:

  CONSOLE_LOG("[timT] %i", 99);
}

unsigned int countT[TEST_TIMER_COUNT] = {0, 0};
TimePoint_t startPointT[TEST_TIMER_COUNT] = {0, 0};

#define TEST_TIME_T 10
TimePoint_t lapPointT[2 * TEST_TIME_T] = {};
byte lapEnd[2] = {0, 0};

typedef struct _eventTimerFiredResultTAG
{
  TimerId_t timerId;
  TimePoint_t procTime;
} EventTimerFiredResultTAG;
void TimerManagerTest::cbTimerFired(TimerId_t timerId, void* extraArg, bool* woken)
{
  do
  {
    int index = timerId - 1;// reinterpret_cast<intptr_t>(extraArg);
    CONSOLE_LOG_ISR(logBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[t-] %u", 0);
    if (countT[index] == 0)
    {
      startPointT[index] = TimerManager::getInstance().nowFromISR();// TIMER_MANAGER_CURRENT_TIMEPOINT();
      countT[index]++;
    }
    else if (countT[index] >= (TEST_TIME_T))
    {
      TimePoint_t current = TimerManager::getInstance().nowFromISR();
      TimePoint_t procTime = (current - startPointT[index])/ TEST_TIME_T;
      if (procTime > 20000)
      {
      //  //CONSOLE_LOG_ISR(logBuf, SYSTEM_CONSOLE_OUT_BUF_LEN,"[t-] %lu", micros());
      //  //CONSOLE_LOG_ISR(logBuf, SYSTEM_CONSOLE_OUT_BUF_LEN,"[t0] %lu", startPointT[index] >> 32);
      //  CONSOLE_LOG_ISR(logBuf, SYSTEM_CONSOLE_OUT_BUF_LEN,"[t1] %lu", startPointT[index]);
      //  //CONSOLE_LOG_ISR(logBuf, SYSTEM_CONSOLE_OUT_BUF_LEN,"[t2] %lu", current >> 32);
      //  CONSOLE_LOG_ISR(logBuf, SYSTEM_CONSOLE_OUT_BUF_LEN,"[t3] %lu", (uint32_t)current);
      //  //CONSOLE_LOG_ISR(logBuf, SYSTEM_CONSOLE_OUT_BUF_LEN,"[timT]4 %lu", procTime >> 32);
      //  CONSOLE_LOG_ISR(logBuf, SYSTEM_CONSOLE_OUT_BUF_LEN,"[t5] %lu", (uint32_t)procTime);

          //@@CONSOLE_LOG_ISR(logBuf, SYSTEM_CONSOLE_OUT_BUF_LEN,"[timT]5 %lu", (uint32_t)startPointT[index]);
      //  //TimerManager::getInstance().stop(timerId);
      }
      if (timerId == 1)
      {
        //TimerManager::getInstance().stopFromISR(timerId, woken);
        //TimerManager::getInstance().stop(timerId);
      }

      /*if (extraArg != NULL)
      {
        EventTimerFiredResultTAG resultEventData = EventTimerFiredResultTAG();
        resultEventData.timerId = timerId;
        resultEventData.procTime = procTime;
        ((TimerManagerTest*)extraArg)->notifyFromISR((int)EventManagerConstant::EventMessageId::TimerFired2, sizeof(EventTimerFiredResultTAG), (unsigned char*)&resultEventData, woken);

      }*/
      //CONSOLE_LOG_ISR(logBuf, SYSTEM_CONSOLE_OUT_BUF_LEN,"cb0 %lu", procTime >> 32);
      //CONSOLE_LOG_ISR(logBuf, SYSTEM_CONSOLE_OUT_BUF_LEN,"cb1 %i", timerId);
      //CONSOLE_LOG_ISR(logBuf, SYSTEM_CONSOLE_OUT_BUF_LEN,"cb0 %lu", (uint32_t)current);
      //SYSTEM_CONSOLE_ISR(logBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "cb0 %i %lu", index, (uint32_t)procTime);
      SYSTEM_CONSOLE_ISR(logBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "cb0 %d", 2345);
      //SYSTEM_CONSOLE_ISR(logBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "cb0 %i", index);
      //Serial.write("a\n");
      //SYSTEM_CONSOLE_ISR(logBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "%i", index);
      //device_Timer.setAlarmValueFromISR(10000 + (intervalStep%10 * 1000));
      countT[index] = 0;
      if (lapEnd[index] == 0)
      {
        lapEnd[index] = 1;
      }

      if ((lapEnd[0] + lapEnd[1]) < 2)
      {
        return;
      }

      /*for (unsigned int wk_indx = 0; wk_indx < TEST_TIME_T; wk_indx++)
      {
        CONSOLE_LOG_ISR(logBuf, SYSTEM_CONSOLE_OUT_BUF_LEN,"cb2 %i %lu", wk_indx, lapPointT[0* TEST_TIME_T + wk_indx]);
      }

      for (unsigned int wk_indx = 0; wk_indx < TEST_TIME_T; wk_indx++)
      {
        CONSOLE_LOG_ISR(logBuf, SYSTEM_CONSOLE_OUT_BUF_LEN,"cb3 %i %lu", wk_indx, lapPointT[1 * TEST_TIME_T + wk_indx]);
      }*/
    }
    else
    {
      //CONSOLE_LOG_ISR(logBuf, SYSTEM_CONSOLE_OUT_BUF_LEN,"[timT] fired %i %i", timerId, countT[index]);
      if (lapEnd[index] == 0)
      {
        //lapPointT[index * TEST_TIME_T + countT[index]] = TimerManager::getInstance().nowFromISR() - startPointT[index];
      }
      countT[index]++;
    }

    return;
  } while (0);
  return;
}

int TimerManagerTest::onEventTimerFired(unsigned char* data, unsigned int dataSize)
{
  //  CONSOLE_LOG("[pp] data %i", 0);
  //int ret = 0;
  do
  {
    if ((dataSize != sizeof(EventTimerFiredTAG))
      || (data == NULL))
    {
      break;
    }

    EventTimerFiredTAG* eventData = (EventTimerFiredTAG*)data;
    //int index = (int)(eventData->extraArg);
    int index = reinterpret_cast<intptr_t>(eventData->extraArg);
    countT[index]++;
    if (countT[index] == 1)
    {
      startPointT[index] = TimerManager::getInstance().now();
      CONSOLE_LOG("[timT] fired %i %i", eventData->timerId, countT[index]);
    }
    else if(countT[index] >= (TEST_TIME_T+1))
    {
      //unsigned long long procTime = TimerManager::getInstance().now() - startPointT[index];
      //TimerManager::getInstance().stop(eventData->timerId);
      //CONSOLE_LOG("[timT] fired %i %i", eventData->timerId, countT2);
      //CONSOLE_LOG("[timT] EvAvg %i %lli", eventData->timerId, (procTime / TEST_TIME_T));
      countT[index] = 0;
    }
    else
    {
      CONSOLE_LOG("[timT] fired %i %i", eventData->timerId, countT[index]);
    }
    
    return 0;
  } while (0);
  return -1;
}


int TimerManagerTest::onEventTimerFiredResult(unsigned char* data, unsigned int dataSize)
{
  do
  {
    if ((dataSize != sizeof(EventTimerFiredResultTAG))
      || (data == NULL))
    {
      break;
    }

    EventTimerFiredResultTAG* eventData = (EventTimerFiredResultTAG*)data;
    SYSTEM_CONSOLE("eventTimer %i %lu", eventData->timerId, (uint32_t)eventData->procTime);
    return 0;
  } while (0);
  return -1;
}

#endif // __CONF_TEST_TIMER_MANAGER_FUNC_ENABLED