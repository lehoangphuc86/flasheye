/////////////////////////////////////////////////
// INCLUDE
#include "ScanningControlTask.h"
#if (_CONF_SCANNING_CONTROL_TASK_ENABLED)
/////////////////////////////////////////////////
// PREPROCESSOR
//#define SCANNING_TASK_CONSOLE_DEBUG_ENABLE
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
#ifdef SCANNING_TASK_CONSOLE_DEBUG_ENABLE
char scanningTaskLogBuf[SYSTEM_CONSOLE_OUT_BUF_LEN];
#endif // SCANNING_TASK_CONSOLE_DEBUG_ENABLE
/////////////////////////////////////////////////
// STATIC FUNCTIONS

/////////////////////////////////////////////////
// EXTERN

/////////////////////////////////////////////////
// CLASS IMPLEMENTAION

/* ScanningControlTask*/
ScanningControlTask::ScanningControlTask(void)
  : TaskManager()
  , sc_Controller(NULL)
  , cur_Scan_SeqId(FEM_SCAN_SEQ_ID_INVALID)
  , cur_Scan_Index(0)
  , max_Scan_Count(0)
  , is_Busy(false)
  , parent_Eventer(NULL)
{

}
ScanningControlTask::~ScanningControlTask(void)
{
  this->cleanUp();
}

bool ScanningControlTask::isBusy(void)
{
  SystemCriticalLocker locker(this->critical_Key);
  return this->is_Busy;
}

bool ScanningControlTask::isValid(void)
{
  return (this->sc_Controller == NULL ? false : true);
}

int ScanningControlTask::inititialize(void)
{
  do
  {
    return 0;
  } while (0);
  this->cleanUp();
  return -1;
}

int ScanningControlTask::startTask(ScanningTaskConfigTAG& scanningConfig)
{
#ifdef SCANNING_TASK_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(scanningTaskLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[scCTsk] sta %i", 0);
#endif // SCANNING_TASK_CONSOLE_DEBUG_ENABLE
  do
  {
    int result = 0;
    if (this->isTaskRunning() != false)
    {
      return 0; // already running
    }

    if (scanningConfig.parentEventer == NULL)
    {
      break;
    }
    
    this->stopTask();
    this->parent_Eventer = (EventManager*)scanningConfig.parentEventer;
    this->sc_Controller = ScanningControllerFactory::generate(scanningConfig.deviceConfig.deviceType);
    if (this->sc_Controller == NULL)
    {
      break;
    }

    scanningConfig.deviceConfig.cbOnSignal = ScanningControlTask::cbScanningDeviceSignal;
    scanningConfig.deviceConfig.signalUserArg = this;

    result = this->sc_Controller->start(scanningConfig.deviceConfig);
    if (result != 0)
    {
      break;
    }

    // Set up tasks
    {
      this->registerHanldingEventStructSize(sizeof(EventScanningStartTAG));
      this->registerHanldingEventStructSize(sizeof(EventScanningStopTAG));
      this->registerHanldingEventStructSize(sizeof(EventScanningDeviceSettingTAG));
      this->registerHanldingEventStructSize(sizeof(EventScanningDeviceSignalTAG));
      result = TaskManager::setConfig(scanningConfig.taskManagerConfig);
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
      result = TaskManager::startProcess(scanningConfig.taskThreadConfig, true);
      if (result != 0)
      {
        break;
      }
    }
#ifdef SCANNING_TASK_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(scanningTaskLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[scCTsk] sta %i", 99);
#endif // SCANNING_TASK_CONSOLE_DEBUG_ENABLE
    return result;
  } while (0);

  this->stopTask();
#ifdef SCANNING_TASK_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(scanningTaskLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[scCTsk] sta %i", -99);
#endif // SCANNING_TASK_CONSOLE_DEBUG_ENABLE
  return -1;
}

void ScanningControlTask::stopTask(void)
{
  do
  {
    TaskManager::stopProcess();
    ScanningControllerFactory::release(this->sc_Controller);
    this->resetSequence();
    this->parent_Eventer = NULL;
    return;
  } while (0);
  return;
}

void ScanningControlTask::cleanUp(void)
{
  this->stopTask();
}

void ScanningControlTask::isBusy(bool flag)
{
  SystemCriticalLocker locker(this->critical_Key);
  this->is_Busy = flag;
}

void ScanningControlTask::proc(void)
{
#ifdef SCANNING_TASK_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(scanningTaskLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[scCTsk] proc %i", 0);
#endif // SCANNING_TASK_CONSOLE_DEBUG_ENABLE
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
      case (int)EventManagerConstant::EventMessageId::ScanningStart:
      {
        this->onEventScanningStart(eventData->bufferAddress(), eventData->dataLength());
        break;
      }
      case (int)EventManagerConstant::EventMessageId::ScanningStop:
      {
        this->onEventScanningStop(eventData->bufferAddress(), eventData->dataLength());
        break;
      }
      case (int)EventManagerConstant::EventMessageId::ScanningDeviceSetting:
      {
        this->onEventScanningDeviceSetting(eventData->bufferAddress(), eventData->dataLength());
        break;
      }
      case (int)EventManagerConstant::EventMessageId::ScanningDeviceSignal:
      {
        this->onEventScanningDeviceSignal(eventData->bufferAddress(), eventData->dataLength());
        break;
      }
      default:
        break;
    }
    this->event_Manager.release(eventData);
  }
#ifdef SCANNING_TASK_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(scanningTaskLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[scCTsk] proc %i", 99);
#endif // SCANNING_TASK_CONSOLE_DEBUG_ENABLE
}

int ScanningControlTask::prepare(void)
{
  this->resetSequence();
  return 0; //do nothing
}

int ScanningControlTask::onEventScanningStart(unsigned char* data, unsigned int dataSize)
{
  ScanningErr_t errorId = SCANNING_ERR_NONE;
  int ret = 0;
  do
  {
#ifdef SCANNING_TASK_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(scanningTaskLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[scCTsk] ssta %i", 0);
#endif // SCANNING_TASK_CONSOLE_DEBUG_ENABLE
    if (this->isValid() == false)
    {
      errorId = SCANNING_ERR_INVALID_OBJ;
      break;
    }

    if (this->isBusy() != false)
    {
      errorId = SCANNING_ERR_BUSY;
      break;
    }

    if ( (data == NULL)
      || (dataSize != sizeof(EventScanningStartTAG))
      )
    {
      errorId = SCANNING_ERR_INVALID_PARAM;
      break;
    }
    this->isBusy(true);
    EventScanningStartTAG* eventData = (EventScanningStartTAG*)data;
    this->cur_Scan_SeqId = eventData->seqId;
    this->max_Scan_Count = eventData->maxScanCount;
    this->cur_Scan_Index = 0;

    ret = this->sc_Controller->startScan();
    if (ret != 0)
    {
      errorId = SCANNING_ERR_DEVICE_ERROR;
      break;
    }
#ifdef SCANNING_TASK_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(scanningTaskLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[scCTsk] ssta %i", 99);
#endif // SCANNING_TASK_CONSOLE_DEBUG_ENABLE
    return 0;
  } while (0);

  if (errorId == SCANNING_ERR_NONE)
  {
    errorId = SCANNING_ERR_UNKOWN;
  }

  EventScanningCompletedTAG scanningCompleteTag = EventScanningCompletedTAG();
  scanningCompleteTag.errorId = errorId;
  this->notifyParent(EventManagerConstant::EventMessageId::ScanningCompleted, sizeof(scanningCompleteTag), (unsigned char*)&scanningCompleteTag);
#ifdef SCANNING_TASK_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(scanningTaskLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[scCTsk] ssta %i", -99);
#endif // SCANNING_TASK_CONSOLE_DEBUG_ENABLE
  return -1;
}

int ScanningControlTask::onEventScanningStop(unsigned char* data, unsigned int dataSize)
{
  int ret = 0;
  do
  {
#ifdef SCANNING_TASK_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(scanningTaskLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[scCTsk] ssto %i", 0);
#endif // SCANNING_TASK_CONSOLE_DEBUG_ENABLE
    if (this->isValid() == false)
    {
      break;
    }

    if (this->isBusy() == false)
    {
      break;
    }

    if ((data == NULL)
      || (dataSize != sizeof(EventScanningStopTAG))
      )
    {
      break;
    }

    EventScanningStopTAG* eventData = (EventScanningStopTAG*)data;
    this->resetSequence();
    this->sc_Controller->stopScan();
#ifdef SCANNING_TASK_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(scanningTaskLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[scCTsk] ssto %i", 99);
#endif // SCANNING_TASK_CONSOLE_DEBUG_ENABLE
    return 0;
  } while (0);

  this->resetSequence();
#ifdef SCANNING_TASK_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(scanningTaskLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[scCTsk] ssto %i", -99);
#endif // SCANNING_TASK_CONSOLE_DEBUG_ENABLE
  return -1;
}

int ScanningControlTask::onEventScanningDeviceSetting(unsigned char* data, unsigned int dataSize)
{
  int ret = 0;
  do
  {
#ifdef SCANNING_TASK_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(scanningTaskLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[scCTsk] sset %i", 0);
#endif // SCANNING_TASK_CONSOLE_DEBUG_ENABLE
    if (this->isValid() == false)
    {
      break;
    }

    if (this->isBusy() != false)
    {
      break;
    }

    if ((data == NULL)
      || (dataSize != sizeof(EventScanningDeviceSettingTAG))
      )
    {
      break;
    }
    EventScanningDeviceSettingTAG* eventData = (EventScanningDeviceSettingTAG*)data;
    EventScanningDeviceSettingCompletedTAG eventSettingCompleted = EventScanningDeviceSettingCompletedTAG();
    eventSettingCompleted.seqId = eventData->seqId;
    ret = this->sc_Controller->setting(eventData->setting, eventSettingCompleted.result);
    this->notifyParent(EventManagerConstant::EventMessageId::ScanningDeviceSettingCompleted, sizeof(EventScanningDeviceSettingCompletedTAG), (unsigned char*)&eventSettingCompleted);
    if (ret != 0)
    {
      break;
    }
    
#ifdef SCANNING_TASK_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(scanningTaskLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[scCTsk] sset %i", 99);
#endif // SCANNING_TASK_CONSOLE_DEBUG_ENABLE
    return 0;
  } while (0);
#ifdef SCANNING_TASK_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(scanningTaskLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[scCTsk] sset %i", -99);
#endif // SCANNING_TASK_CONSOLE_DEBUG_ENABLE
  return -1;
}

int ScanningControlTask::onEventScanningDeviceSignal(unsigned char* data, unsigned int dataSize)
{
  int ret = 0;
  ScanningErr_t errorId = SCANNING_ERR_NONE; 
  bool isCompleted = false;
  do
  {
#ifdef SCANNING_TASK_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(scanningTaskLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[scCTsk] ssig %i", 0);
#endif // SCANNING_TASK_CONSOLE_DEBUG_ENABLE
    if (this->isValid() == false)
    {
      break;
    }

    if (this->isBusy() == false)
    {
      break;
    }

    if ((data == NULL)
      || (dataSize != sizeof(EventScanningDeviceSignalTAG))
      )
    {
      break;
    }

    EventScanningDeviceSignalTAG* eventData = (EventScanningDeviceSignalTAG*)data;
#ifdef SCANNING_TASK_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(scanningTaskLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[scCTsk] ssig %i %i", 10, eventData->signalType);
#endif // SCANNING_TASK_CONSOLE_DEBUG_ENABLE
    switch (eventData->signalType)
    {
      case SCANNING_DEVICE_SIGNAL_ID_LED:
      {
        // read result
        // notify parent
        {
          EventScanningResultTAG scanResult = EventScanningResultTAG();
          scanResult.seqId = this->cur_Scan_SeqId;
          scanResult.scanIndex = this->cur_Scan_Index;
          this->cur_Scan_Index++;
          ret = this->sc_Controller->readResult(scanResult.deviceResult);
#ifdef SCANNING_TASK_CONSOLE_DEBUG_ENABLE
          CONSOLE_LOG_BUF(scanningTaskLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[scCTsk] ssig %i %i", 11, ret);
#endif // SCANNING_TASK_CONSOLE_DEBUG_ENABLE
          ret |= this->notifyParent(EventManagerConstant::EventMessageId::ScanningResult, sizeof(EventScanningResultTAG), (unsigned char*)&scanResult);
        }

        if (ret == 0)
        {
          if ((this->max_Scan_Count > 0)
            && (this->cur_Scan_Index >= this->max_Scan_Count))
          {
            isCompleted = true;
          }
          else
          {
            ret |= this->sc_Controller->startScan();
#ifdef SCANNING_TASK_CONSOLE_DEBUG_ENABLE
            CONSOLE_LOG_BUF(scanningTaskLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[scCTsk] ssig %i %i", 12, ret);
#endif // SCANNING_TASK_CONSOLE_DEBUG_ENABLE
          }
        }
        else
        {
          isCompleted = true;
        }
        
        if (ret != 0)
        {
          isCompleted = true;
        }

        if (isCompleted != false)
        {
#ifdef SCANNING_TASK_CONSOLE_DEBUG_ENABLE
          CONSOLE_LOG_BUF(scanningTaskLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[scCTsk] ssig %i", 13);
#endif // SCANNING_TASK_CONSOLE_DEBUG_ENABLE
          // enough
          EventScanningCompletedTAG scanCompleted = EventScanningCompletedTAG();
          scanCompleted.seqId = this->cur_Scan_SeqId;
          scanCompleted.errorId = (ret == 0? SCANNING_ERR_NONE: SCANNING_ERR_DEVICE_ERROR);
          scanCompleted.scannedCount = (this->cur_Scan_Index + 1);
          this->notifyParent(EventManagerConstant::EventMessageId::ScanningCompleted, sizeof(EventScanningCompletedTAG), (unsigned char*)&scanCompleted);
          this->resetSequence();
        }
        break;
      }
      case SCANNING_DEVICE_SIGNAL_ID_BUZZER:
      {
        // notify parent
        break;
      }
      default:
        ret = -1;
        break;
    }

    if (ret != 0)
    {
      break;
    }
#ifdef SCANNING_TASK_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(scanningTaskLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[scCTsk] ssig %i", 99);
#endif // SCANNING_TASK_CONSOLE_DEBUG_ENABLE
    return 0;
  } while (0);
#ifdef SCANNING_TASK_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(scanningTaskLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[scCTsk] ssig %i", -99);
#endif // SCANNING_TASK_CONSOLE_DEBUG_ENABLE
  return -1;
}

int ScanningControlTask::notifyParent(EventId_t messageId, EventSize_t eventSize, unsigned char* eventData)
{
  do
  {
    if (this->parent_Eventer == NULL)
    {
      break;
    }

    return this->parent_Eventer->notify(messageId, eventSize, eventData);
  } while (0);
  return -1;
}

void ScanningControlTask::resetSequence(void)
{
  this->isBusy(false);
  this->cur_Scan_SeqId = FEM_SCAN_SEQ_ID_INVALID;
  this->max_Scan_Count = 0;
  this->cur_Scan_Index = 0;
}

void ScanningControlTask::cbScanningDeviceSignal(void* userArg, byte signalId, void* eventData, DataSize_t eventSize, bool* woken)
{
  do
  {
    if (userArg == NULL)
    {
      break;
    }

    ScanningControlTask* ctrlTask = (ScanningControlTask*)userArg;
    switch (signalId)
    {
    case SCANNING_DEVICE_SIGNAL_ID_LED:
    case SCANNING_DEVICE_SIGNAL_ID_BUZZER:
    {
      EventScanningDeviceSignalTAG deviceSignalData = EventScanningDeviceSignalTAG();
      deviceSignalData.signalType = signalId;
      ctrlTask->notifyFromISR(EventManagerConstant::EventMessageId::ScanningDeviceSignal, sizeof(deviceSignalData), (unsigned char*)&deviceSignalData, woken);
      break;
    }
    default:
      break;
    }

    return;
  } while (0);
  return;
}
#endif // _CONF_SCANNING_CONTROL_TASK_ENABLED