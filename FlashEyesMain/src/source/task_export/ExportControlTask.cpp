/////////////////////////////////////////////////
// INCLUDE
#include "ExportControlTask.h"
#if (_CONF_EXPORT_CONTROL_TASK_ENABLED)
/////////////////////////////////////////////////
// PREPROCESSOR
//#define EXPORT_CONTROL_TASK_CONSOLE_DEBUG_ENABLE
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
#ifdef EXPORT_CONTROL_TASK_CONSOLE_DEBUG_ENABLE
char exportControlTaskLogBuf[SYSTEM_CONSOLE_OUT_BUF_LEN];
#endif // EXPORT_CONTROL_TASK_CONSOLE_DEBUG_ENABLE
/////////////////////////////////////////////////
// STATIC FUNCTIONS

/////////////////////////////////////////////////
// EXTERN

/////////////////////////////////////////////////
// CLASS IMPLEMENTAION

/* ExportControlTask*/
ExportControlTask::ExportControlTask(void)
  : TaskManager()
  , export_Processor(NULL)
{

}

ExportControlTask::~ExportControlTask(void)
{
  this->cleanUp();
}

bool ExportControlTask::isValid(void)
{
  do
  {
    if (this->export_Processor == NULL)
    {
      break;
    }

    return true;
  } while (0);
  return false;
}

int ExportControlTask::startTask(ExportManagerConfigTAG& exportManagerConfig)
{
  do
  {
    int result = 0;
    if (this->isTaskRunning() != false)
    {
      return 0; // already running
    }

    this->stopTask();

    this->export_Processor = ExportProcessorFactory::generate(exportManagerConfig.exportProcConfig.exportProcType);
    if (this->export_Processor == NULL)
    {
      break;
    }

    result = this->export_Processor->start(exportManagerConfig.exportProcConfig);
    if (result != 0)
    {
      break;
    }

    // Set up tasks
    {
      // self
      this->registerHanldingEventStructSize(sizeof(EventPrepareCompletedTAG));
      this->registerHanldingEventStructSize(sizeof(EventExportScanResultTAG));
      result = TaskManager::setConfig(exportManagerConfig.taskManagerConfig);
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

    {
      result = TaskManager::startProcess(exportManagerConfig.taskThreadConfig, true);
      if (result != 0)
      {
        break;
      }
    }

    return result;
  } while (0);

  this->stopTask();
  return -1;
}


void ExportControlTask::stopTask(void)
{
  TaskManager::stopProcess();
  if (this->export_Processor != NULL)
  {
    this->export_Processor->stop();
  }
  ExportProcessorFactory::release(this->export_Processor);  
  return;
}

void ExportControlTask::cleanUp(void)
{
  this->stopTask();
}

void ExportControlTask::proc(void)
{
#ifdef EXPORT_CONTROL_TASK_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(exportControlTaskLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[expCTsk] proc %i", 0);
#endif // EXPORT_CONTROL_TASK_CONSOLE_DEBUG_ENABLE
  int prepareRet = this->prepare();
  this->reportPrepareResult(prepareRet);
  if (prepareRet != 0)
  {
    this->waitTerminating();
    return;
  }
  EventDataItem* eventData = NULL;
  bool isStop = false;
  while (isStop == false) // A Task shall never return or exit.
  {
    eventData = this->event_Manager.wait(TASK_MANAGER_EVENT_WAIT_TIME_DEFAULT);
    if (eventData == NULL)
    {
      continue;
    }

    switch (eventData->messageId())
    {
      case (int)EventManagerConstant::EventMessageId::TerminateProcess:
      {
        this->isTaskRunning(false);
        isStop = true;
        break;
      }
      case (int)EventManagerConstant::EventMessageId::ExportScanResult:
      {
        this->onEventExportScanResult(eventData->bufferAddress(), eventData->dataLength());
        break;
      }
      default:
        break;
    }
    this->event_Manager.release(eventData);
  }
#ifdef EXPORT_CONTROL_TASK_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(exportControlTaskLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[expCTsk] proc %i", 99);
#endif // EXPORT_CONTROL_TASK_CONSOLE_DEBUG_ENABLE
}

int ExportControlTask::prepare(void)
{
  return 0; //do nothing
}

int ExportControlTask::onEventExportScanResult(unsigned char* data, unsigned int dataSize)
{
#ifdef EXPORT_CONTROL_TASK_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(exportControlTaskLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[expCTsk] sr %i", 0);
#endif // EXPORT_CONTROL_TASK_CONSOLE_DEBUG_ENABLE
  do
  {
    if ((dataSize != sizeof(EventExportScanResultTAG))
      || (data == NULL))
    {
      break;
    }

    if (this->isValid() == false)
    {
      break;
    }

    EventExportScanResultTAG* eventData = (EventExportScanResultTAG*)data;
    this->export_Processor->exportScanResult(&eventData->result);
#ifdef EXPORT_CONTROL_TASK_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(exportControlTaskLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[expCTsk] sr %i", 99);
#endif // EXPORT_CONTROL_TASK_CONSOLE_DEBUG_ENABLE
    return 0;
  } while (0);

#ifdef EXPORT_CONTROL_TASK_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(exportControlTaskLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[expCTsk] sr %i", -99);
#endif // EXPORT_CONTROL_TASK_CONSOLE_DEBUG_ENABLE
  return -1;
}


#endif // _CONF_EXPORT_CONTROL_TASK_ENABLED