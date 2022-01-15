/////////////////////////////////////////////////
// INCLUDE
#include "MainNormalController.h"
#if (_CONF_MAIN_NORMAL_CONTROLLER_ENABLED)

#include "timer_manager/TimerManager.h"
/////////////////////////////////////////////////
// PREPROCESSOR
#define MAIN_NORMAL_CONTROLLER_CONSOLE_DEBUG_ENABLE
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
#ifdef MAIN_NORMAL_CONTROLLER_CONSOLE_DEBUG_ENABLE
char mainNormalControllerLogBuf[SYSTEM_CONSOLE_OUT_BUF_LEN];
#endif // MAIN_NORMAL_CONTROLLER_CONSOLE_DEBUG_ENABLE
/////////////////////////////////////////////////
// STATIC FUNCTIONS

/////////////////////////////////////////////////
// EXTERN

/////////////////////////////////////////////////
// CLASS IMPLEMENTAION
/*MainNormalController*/

MainNormalController::MainNormalController(void)
  : MainController(SYS_MODE_NORMAL)
  , timer_Id(TIMER_MANAGER_TIMER_ID_INVALID)
{

}

MainNormalController::~MainNormalController(void)
{

}

bool MainNormalController::isValid(void)
{
  do
  {
    
    return MainController::isValid();
  } while (0);
  return false;
}

int MainNormalController::prepare(void)
{
  int ret = 0;
  do
  {
#ifdef MAIN_NORMAL_CONTROLLER_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(mainNormalControllerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mnTsk] pre %i", 0);
#endif // MAIN_NORMAL_CONTROLLER_CONSOLE_DEBUG_ENABLE
    ret = MainController::prepare();
#ifdef MAIN_NORMAL_CONTROLLER_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(mainNormalControllerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mnTsk] pre %i %i", 1, ret);
#endif // MAIN_NORMAL_CONTROLLER_CONSOLE_DEBUG_ENABLE
    if (ret != 0)
    {
      break;
    }

    ret = this->scanning_Task.inititialize();
#ifdef MAIN_NORMAL_CONTROLLER_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(mainNormalControllerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mnTsk] pre %i %i", 2, ret);
#endif // MAIN_NORMAL_CONTROLLER_CONSOLE_DEBUG_ENABLE
    if (ret != 0)
    {
      break;
    }

    {
      ScanningTaskConfigTAG scanningConfig = ScanningTaskConfigTAG();
      scanningConfig.parentEventer = this->eventManager();
    
      scanningConfig.taskManagerConfig.eventItemNumber = FEM_SCAN_EM_EVENT_NUM;
      scanningConfig.taskManagerConfig.eventUsePool = FEM_SCAN_EM_USE_POOL;

      scanningConfig.taskThreadConfig.enabled = true;
      scanningConfig.taskThreadConfig.useThreadPool = FEM_SCAN_TM_USE_POOL;
      scanningConfig.taskThreadConfig.usStackDepth = FEM_SCAN_TM_MEM;
      scanningConfig.taskThreadConfig.uxPriority = FEM_SCAN_TM_PRIORITY;

      scanningConfig.deviceConfig.dataTimeout = FEM_SCAN_DEV_DEVICE_DATA_TIMEOUT;
      scanningConfig.deviceConfig.deviceType = FEM_SCAN_DEV_DEVICE_TYPE;
      scanningConfig.deviceConfig.id = FEM_SCAN_DEV_DEVICE_ID;
      scanningConfig.deviceConfig.spec.evScanner.pin_Buzzer = FEM_SCAN_DEV_PIN_BUZZER;
      scanningConfig.deviceConfig.spec.evScanner.pin_Led = FEM_SCAN_DEV_PIN_LED;
      scanningConfig.deviceConfig.spec.evScanner.pin_Sensor = FEM_SCAN_DEV_PIN_SENSOR;
      scanningConfig.deviceConfig.spec.evScanner.serialConfig.instanceIndex = FEM_SCAN_DEV_DEVICE_SC_INSTANCE_IDX;
      scanningConfig.deviceConfig.spec.evScanner.serialConfig.baudrate = FEM_SCAN_DEV_DEVICE_SC_BAUDRATE;
      scanningConfig.deviceConfig.spec.evScanner.serialConfig.bitPerByte = FEM_SCAN_DEV_DEVICE_SC_BIT_PER_BYTE;
      
      scanningConfig.deviceConfig.spec.evScanner.serialConfig.parityType = FEM_SCAN_DEV_DEVICE_SC_PARITY_TYPE;
      scanningConfig.deviceConfig.spec.evScanner.serialConfig.pin_RX = FEM_SCAN_DEV_DEVICE_SC_PIN_RX;
      scanningConfig.deviceConfig.spec.evScanner.serialConfig.pin_TX = FEM_SCAN_DEV_DEVICE_SC_PIN_TX;
      scanningConfig.deviceConfig.spec.evScanner.serialConfig.stopBitNum = FEM_SCAN_DEV_DEVICE_SC_STOP_BIT;
      scanningConfig.deviceConfig.spec.evScanner.serialConfig.timeout = FEM_SCAN_DEV_DEVICE_SC_TIMEOUT;

      ret = this->scanning_Task.startTask(scanningConfig);
#ifdef MAIN_NORMAL_CONTROLLER_CONSOLE_DEBUG_ENABLE
      CONSOLE_LOG_BUF(mainNormalControllerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mnTsk] pre %i %i", 5, ret);
#endif // MAIN_NORMAL_CONTROLLER_CONSOLE_DEBUG_ENABLE
      if (ret != 0)
      {
        break;
      }
    }
#ifdef MAIN_NORMAL_CONTROLLER_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(mainNormalControllerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mnTsk] pre %i", 99);
#endif // MAIN_NORMAL_CONTROLLER_CONSOLE_DEBUG_ENABLE
    return 0;
  } while (0);
#ifdef MAIN_NORMAL_CONTROLLER_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(mainNormalControllerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mnTsk] pre %i", -99);
#endif // MAIN_NORMAL_CONTROLLER_CONSOLE_DEBUG_ENABLE
  return -1;
}

void MainNormalController::clear(void)
{
  do
  {
    this->scanning_Task.cleanUp();
    MainController::clear();
    return;
  } while (0);
  return;
}

void MainNormalController::regEventSize(void)
{
  
  this->registerHanldingEventStructSize(sizeof(EventTimerFiredTAG));
  this->registerHanldingEventStructSize(sizeof(EventScanningResultTAG));
  this->registerHanldingEventStructSize(sizeof(EventScanningCompletedTAG));
  this->registerHanldingEventStructSize(sizeof(EventTriggerStartScanningTAG));
  
}

int MainNormalController::onEventHandling(EventDataItem* eventData)
{
  do
  {
    if (eventData == NULL)
    {
      static int count = 0;
      count++;
      //@@
      if (count == 1)
      {
        EventTriggerStartScanningTAG trgStart = EventTriggerStartScanningTAG();
        trgStart.source = 1;
        this->notify(
          (int)EventManagerConstant::EventMessageId::TriggerStartScanning
          , sizeof(EventTriggerStartScanningTAG)
          , (unsigned char*)&trgStart);

        //EventScanningDeviceSettingTAG devSetting = EventScanningDeviceSettingTAG();
        //devSetting.setting.settingIdSet0 = (1UL << SCANNING_SET_ID_COM_BAUDRATE);
        //this->notify(
        //  (int)EventManagerConstant::EventMessageId::ScanningDeviceSetting
        //  , sizeof(EventScanningDeviceSettingTAG)
        //  , (unsigned char*)&devSetting);
      }

      //if (count == 2)
      //{
      //  EventTriggerStartScanningTAG trgStart = EventTriggerStartScanningTAG();
      //  trgStart.source = 1;
      //  this->notify(
      //    (int)EventManagerConstant::EventMessageId::TriggerStartScanning
      //    , sizeof(EventTriggerStartScanningTAG)
      //    , (unsigned char*)&trgStart);

      //  //EventScanningDeviceSettingTAG devSetting = EventScanningDeviceSettingTAG();
      //  //devSetting.setting.settingIdSet0 = (1UL << SCANNING_SET_ID_COM_BAUDRATE);
      //  //this->notify(
      //  //  (int)EventManagerConstant::EventMessageId::ScanningDeviceSetting
      //  //  , sizeof(EventScanningDeviceSettingTAG)
      //  //  , (unsigned char*)&devSetting);
      //}
      break;
    }

    switch (eventData->messageId())
    {
      case (int)EventManagerConstant::EventMessageId::ScanningDeviceSetting:
      {
        this->onEventScanningDeviceSetting(eventData->bufferAddress(), eventData->dataLength());
        break;
      }
      case (int)EventManagerConstant::EventMessageId::ScanningDeviceSettingCompleted:
      {
        this->onEventScanningDeviceSettingCompleted(eventData->bufferAddress(), eventData->dataLength());
        break;
      }
      case (int)EventManagerConstant::EventMessageId::TriggerStartScanning:
      {
        this->onEventTriggerStartScanning(eventData->bufferAddress(), eventData->dataLength());
        break;
      }
      case (int)EventManagerConstant::EventMessageId::ScanningResult:
      {
        this->onEventScanningResult(eventData->bufferAddress(), eventData->dataLength());
        break;
      }
      case (int)EventManagerConstant::EventMessageId::ScanningCompleted:
      {
        this->onEventScanningCompleted(eventData->bufferAddress(), eventData->dataLength());
        break;
      }
      case (int)EventManagerConstant::EventMessageId::TimerFired1:
      {
        this->onEventTimerFired1(eventData->bufferAddress(), eventData->dataLength());
        break;
      }
      default:
        break;
    }

    return 0;
  } while (0);
  return -1;
}

int MainNormalController::onEventScanningDeviceSetting(unsigned char* data, unsigned int dataSize)
{
  int ret = 0;
  do
  {
#ifdef MAIN_NORMAL_CONTROLLER_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(mainNormalControllerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mnTsk] setS %i", 0);
#endif // MAIN_NORMAL_CONTROLLER_CONSOLE_DEBUG_ENABLE
    if (this->isBusy() != false)
    {
      return -1;
    }

    if ((data == NULL)
      || (dataSize != sizeof(EventScanningDeviceSettingTAG))
      )
    {
      return -1;
    }

    EventScanningDeviceSettingTAG* eventData = (EventScanningDeviceSettingTAG*)data;
    eventData->seqId = this->nextSeqId();
    ret = this->scanning_Task.notify(
      EventManagerConstant::EventMessageId::ScanningDeviceSetting
      , sizeof(EventScanningDeviceSettingTAG)
      , (unsigned char*)eventData
    );
#ifdef MAIN_NORMAL_CONTROLLER_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(mainNormalControllerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mnTsk] setS %i %i %i", 5, ret, eventData->seqId);
#endif // MAIN_NORMAL_CONTROLLER_CONSOLE_DEBUG_ENABLE
    if (ret != 0)
    {
      break;
    }

    this->isBusy(true);
    ret = this->startTimer(FEM_SCAN_OP_TIMEOUT);
    if (ret != 0)
    {
      break;
    }
#ifdef MAIN_NORMAL_CONTROLLER_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(mainNormalControllerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mnTsk] setS %i %i", 99, eventData->seqId);
#endif // MAIN_NORMAL_CONTROLLER_CONSOLE_DEBUG_ENABLE
    return 0;
  } while (0);
  this->resetSequence();
#ifdef MAIN_NORMAL_CONTROLLER_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(mainNormalControllerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mnTsk] setS %i", -99);
#endif // MAIN_NORMAL_CONTROLLER_CONSOLE_DEBUG_ENABLE
  return -1;
}

int MainNormalController::onEventScanningDeviceSettingCompleted(unsigned char* data, unsigned int dataSize)
{
  int ret = 0;
  do
  {
#ifdef MAIN_NORMAL_CONTROLLER_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(mainNormalControllerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mnTsk] seC %i", 0);
#endif // MAIN_NORMAL_CONTROLLER_CONSOLE_DEBUG_ENABLE
    if (this->isBusy() == false)
    {
      break;
    }

    if ((data == NULL)
      || (dataSize != sizeof(EventScanningDeviceSettingCompletedTAG))
      )
    {
      break;
    }

    EventScanningDeviceSettingCompletedTAG* eventData = (EventScanningDeviceSettingCompletedTAG*)data;
    if (eventData->seqId != this->curSeqId())
    {
      break;
    }
#ifdef MAIN_NORMAL_CONTROLLER_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(mainNormalControllerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mnTsk] seC %i %ld", 97, (long)(eventData->result.errorSet0 >> 32));
    CONSOLE_LOG_BUF(mainNormalControllerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mnTsk] seC %i %ld", 98, (long)eventData->result.errorSet0);
#endif // MAIN_NORMAL_CONTROLLER_CONSOLE_DEBUG_ENABLE
    this->resetSequence();
    return 0;
  } while (0);

#ifdef MAIN_NORMAL_CONTROLLER_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(mainNormalControllerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mnTsk] seC %i", -99);
#endif // MAIN_NORMAL_CONTROLLER_CONSOLE_DEBUG_ENABLE
  return -1;
}


int MainNormalController::onEventTriggerStartScanning(unsigned char* data, unsigned int dataSize)
{
  int ret = 0;
  do
  {
#ifdef MAIN_NORMAL_CONTROLLER_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(mainNormalControllerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mnTsk] trg %i", 0);
#endif // MAIN_NORMAL_CONTROLLER_CONSOLE_DEBUG_ENABLE
    if (this->isBusy() != false)
    {
#ifdef MAIN_NORMAL_CONTROLLER_CONSOLE_DEBUG_ENABLE
      CONSOLE_LOG_BUF(mainNormalControllerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mnTsk] trg %i", -1);
#endif // MAIN_NORMAL_CONTROLLER_CONSOLE_DEBUG_ENABLE
      return -1;
    }

    if ((data == NULL)
      || (dataSize != sizeof(EventTriggerStartScanningTAG))
      )
    {
#ifdef MAIN_NORMAL_CONTROLLER_CONSOLE_DEBUG_ENABLE
      CONSOLE_LOG_BUF(mainNormalControllerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mnTsk] trg %i", -2);
#endif // MAIN_NORMAL_CONTROLLER_CONSOLE_DEBUG_ENABLE
      return -1;
    }

    //EventTriggerStartScanningTAG* eventData = (EventTriggerStartScanningTAG*)data;
    EventScanningStartTAG scanningStartEvent = EventScanningStartTAG();
    scanningStartEvent.maxScanCount = FEM_SCAN_OP_MAX_SCAN_COUNT;
    scanningStartEvent.seqId = this->nextSeqId();
    ret = this->scanning_Task.notify(
        EventManagerConstant::EventMessageId::ScanningStart
        , sizeof(EventScanningStartTAG)
        , (unsigned char*)&scanningStartEvent
      );
#ifdef MAIN_NORMAL_CONTROLLER_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(mainNormalControllerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mnTsk] trg %i %i", 5, ret);
#endif // MAIN_NORMAL_CONTROLLER_CONSOLE_DEBUG_ENABLE
    if (ret != 0)
    {
      break;
    }

    this->isBusy(true);
    ret = this->startTimer(FEM_SCAN_OP_TIMEOUT);
    if (ret != 0)
    {
      break;
    }
#ifdef MAIN_NORMAL_CONTROLLER_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(mainNormalControllerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mnTsk] trg %i %i %i", 99, scanningStartEvent.seqId, scanningStartEvent.maxScanCount);
#endif // MAIN_NORMAL_CONTROLLER_CONSOLE_DEBUG_ENABLE
    return 0;
  } while (0);
  this->resetSequence();
#ifdef MAIN_NORMAL_CONTROLLER_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(mainNormalControllerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mnTsk] trg %i", -99);
#endif // MAIN_NORMAL_CONTROLLER_CONSOLE_DEBUG_ENABLE
  return -1;
}

int MainNormalController::onEventScanningResult(unsigned char* data, unsigned int dataSize)
{
  int ret = 0;
  do
  {
#ifdef MAIN_NORMAL_CONTROLLER_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(mainNormalControllerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mnTsk] sre %i", 0);
#endif // MAIN_NORMAL_CONTROLLER_CONSOLE_DEBUG_ENABLE
    if (this->isBusy() == false)
    {
      break;
    }

    if ((data == NULL)
      || (dataSize != sizeof(EventScanningResultTAG))
      )
    {
      break;
    }

    EventScanningResultTAG* eventData = (EventScanningResultTAG*)data;
    if (eventData->seqId != this->curSeqId())
    {
      break;
    }
    
#ifdef MAIN_NORMAL_CONTROLLER_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(mainNormalControllerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mnTsk] sre %i %i %i %i", 10, eventData->seqId, eventData->scanIndex, eventData->deviceResult.errorId);
    CONSOLE_LOG_BUF(mainNormalControllerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mnTsk] sre %i %i %i", 11, eventData->deviceResult.code.type, eventData->deviceResult.code.codeLen);
    CONSOLE_LOG_BUF(mainNormalControllerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mnTsk] sre %i %s", 12, eventData->deviceResult.code.code);
#endif // MAIN_NORMAL_CONTROLLER_CONSOLE_DEBUG_ENABLE
    return 0;
  } while (0);

#ifdef MAIN_NORMAL_CONTROLLER_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(mainNormalControllerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mnTsk] ssto %i", -99);
#endif // MAIN_NORMAL_CONTROLLER_CONSOLE_DEBUG_ENABLE
  return -1;
}

int MainNormalController::onEventScanningCompleted(unsigned char* data, unsigned int dataSize)
{
  int ret = 0;
  do
  {
#ifdef MAIN_NORMAL_CONTROLLER_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(mainNormalControllerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mnTsk] sco %i", 0);
#endif // MAIN_NORMAL_CONTROLLER_CONSOLE_DEBUG_ENABLE
    if (this->isBusy() == false)
    {
#ifdef MAIN_NORMAL_CONTROLLER_CONSOLE_DEBUG_ENABLE
      CONSOLE_LOG_BUF(mainNormalControllerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mnTsk] sco %i", -1);
#endif // MAIN_NORMAL_CONTROLLER_CONSOLE_DEBUG_ENABLE
      break;
    }

    if ((data == NULL)
      || (dataSize != sizeof(EventScanningCompletedTAG))
      )
    {
#ifdef MAIN_NORMAL_CONTROLLER_CONSOLE_DEBUG_ENABLE
      CONSOLE_LOG_BUF(mainNormalControllerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mnTsk] sco %i", -2);
#endif // MAIN_NORMAL_CONTROLLER_CONSOLE_DEBUG_ENABLE
      break;
    }

    EventScanningCompletedTAG* eventData = (EventScanningCompletedTAG*)data;
    if (eventData->seqId != this->curSeqId())
    {
#ifdef MAIN_NORMAL_CONTROLLER_CONSOLE_DEBUG_ENABLE
      CONSOLE_LOG_BUF(mainNormalControllerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mnTsk] sco %d %d %d", -3, eventData->seqId, this->curSeqId());
#endif // MAIN_NORMAL_CONTROLLER_CONSOLE_DEBUG_ENABLE
      break;
    }
    this->resetSequence();
#ifdef MAIN_NORMAL_CONTROLLER_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(mainNormalControllerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mnTsk] sco %i %i %i %i", 10, eventData->seqId, eventData->errorId, eventData->scannedCount);
#endif // MAIN_NORMAL_CONTROLLER_CONSOLE_DEBUG_ENABLE
    return 0;
  } while (0);

#ifdef MAIN_NORMAL_CONTROLLER_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(mainNormalControllerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mnTsk] sco %i", -99);
#endif // MAIN_NORMAL_CONTROLLER_CONSOLE_DEBUG_ENABLE
  return -1;
}

int MainNormalController::onEventTimerFired1(unsigned char* data, unsigned int dataSize)
{
  int ret = 0;
  do
  {
#ifdef MAIN_NORMAL_CONTROLLER_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(mainNormalControllerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mnTsk] tim %i", 0);
#endif // MAIN_NORMAL_CONTROLLER_CONSOLE_DEBUG_ENABLE
    
    if ((data == NULL)
      || (dataSize != sizeof(EventTimerFiredTAG))
      )
    {
      break;
    }

    EventTimerFiredTAG* eventData = (EventTimerFiredTAG*)data;
    if (eventData->timerId != this->timer_Id)
    {
#ifdef MAIN_NORMAL_CONTROLLER_CONSOLE_DEBUG_ENABLE
      CONSOLE_LOG_BUF(mainNormalControllerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mnTsk] tim %d %d %d", -3, eventData->timerId, this->timer_Id);
#endif // MAIN_NORMAL_CONTROLLER_CONSOLE_DEBUG_ENABLE
      break;
    }
    //this->stopScanning();
    this->resetSequence();
#ifdef MAIN_NORMAL_CONTROLLER_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(mainNormalControllerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mnTsk] tim %i", 10);
#endif // MAIN_NORMAL_CONTROLLER_CONSOLE_DEBUG_ENABLE
    return 0;
  } while (0);

#ifdef MAIN_NORMAL_CONTROLLER_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(mainNormalControllerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mnTsk] tim %i", -99);
#endif // MAIN_NORMAL_CONTROLLER_CONSOLE_DEBUG_ENABLE
  return -1;
}

void MainNormalController::resetSequence(void)
{
  this->stopTimer();
  this->stopScanning();
  this->isBusy(false);
}

int MainNormalController::startTimer(TimePoint_t timeout)
{
  do
  {
    this->stopTimer();
    if (timeout <= 0)
    {
      return 0; // no need to set
    }

    this->timer_Id = TimerManager::getInstance().start(MainNormalController::cbTimerFired, this, timeout, true);
    if (this->timer_Id == TIMER_MANAGER_TIMER_ID_INVALID)
    {
      break;
    }

    return 0;
  } while (0);
  this->stopTimer();
  return -1;
}

void MainNormalController::stopTimer(void)
{
  do
  {
    if (this->timer_Id == TIMER_MANAGER_TIMER_ID_INVALID)
    {
      return;
    }

    TimerManager::getInstance().stop(this->timer_Id);
    this->timer_Id = TIMER_MANAGER_TIMER_ID_INVALID;
    return;
  } while (0);
  return;
}

void MainNormalController::stopScanning(void)
{
  int ret = 0;
  do
  {
    if (this->isBusy() == false)
    {
      return;
    }

    EventScanningStopTAG scanningStopEvent = EventScanningStopTAG();
    scanningStopEvent.reserved = 0;
    ret = this->scanning_Task.notify(
      EventManagerConstant::EventMessageId::ScanningStop
      , sizeof(EventScanningStopTAG)
      , (unsigned char*)&scanningStopEvent
    );
    if (ret != 0)
    {
      break;
    }

    return;
  } while (0);
  return;
}

// static
void MainNormalController::cbTimerFired(TimerId_t timerId, void* extraArg, bool* woken)
{
  do
  {
    if (extraArg == NULL)
    {
      break;
    }
    EventTimerFiredTAG eventData = EventTimerFiredTAG();
    eventData.timerId = timerId;
    eventData.extraArg = extraArg;
    ((MainNormalController*)extraArg)->notifyFromISR(
      EventManagerConstant::EventMessageId::TimerFired1
      , sizeof(EventTimerFiredTAG)
      , (unsigned char*)&eventData
      , woken);

    return;
  } while (0);
  return;
}
#endif // _CONF_MAIN_NORMAL_CONTROLLER_ENABLED