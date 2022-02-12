/////////////////////////////////////////////////
// INCLUDE
#include "MainNormalController.h"
#if (_CONF_MAIN_NORMAL_CONTROLLER_ENABLED)
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

void MainNormalController::regEventSize(void)
{
  MainController::regEventSize();
  this->registerHanldingEventStructSize(sizeof(EventScanningDeviceSettingTAG));
  this->registerHanldingEventStructSize(sizeof(EventScanningDeviceSettingCompletedTAG));
  this->registerHanldingEventStructSize(sizeof(EventTimerFiredTAG));
  this->registerHanldingEventStructSize(sizeof(EventScanningResultTAG));
  this->registerHanldingEventStructSize(sizeof(EventScanningCompletedTAG));
  this->registerHanldingEventStructSize(sizeof(EventScanningControlTAG));
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

    ret = this->startButtonTask();
#ifdef MAIN_NORMAL_CONTROLLER_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(mainNormalControllerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mnTsk] pre %i %i", 2, ret);
#endif // MAIN_NORMAL_CONTROLLER_CONSOLE_DEBUG_ENABLE
    if (ret != 0)
    {
      break;
    }

    ret = this->startNetManager();
#ifdef MAIN_NORMAL_CONTROLLER_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(mainNormalControllerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mnTsk] pre %i %i", 3, ret);
#endif // MAIN_NORMAL_CONTROLLER_CONSOLE_DEBUG_ENABLE
    if (ret != 0)
    {
      break;
    }

    ret = this->startExCommManager();
#ifdef MAIN_NORMAL_CONTROLLER_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(mainNormalControllerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mnTsk] pre %i %i", 4, ret);
#endif // MAIN_NORMAL_CONTROLLER_CONSOLE_DEBUG_ENABLE
    if (ret != 0)
    {
      break;
    }

    ret = this->startScanningTask();
#ifdef MAIN_NORMAL_CONTROLLER_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(mainNormalControllerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mnTsk] pre %i %i", 5, ret);
#endif // MAIN_NORMAL_CONTROLLER_CONSOLE_DEBUG_ENABLE
    if (ret != 0)
    {
      break;
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
    this->stopScanningTask();
    this->stopExCommManager();
    this->stopNetManager();
    MainController::clear();
#ifdef MAIN_NORMAL_CONTROLLER_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(mainNormalControllerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mnTsk] clear %i", 99);
#endif // MAIN_NORMAL_CONTROLLER_CONSOLE_DEBUG_ENABLE
    return;
  } while (0);
  return;
}

int MainNormalController::onEventHandling(EventDataItem* eventData)
{
  do
  {
    if (eventData == NULL)
    {
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
      case (int)EventManagerConstant::EventMessageId::ScanningControl:
      {
        this->onEventScanningControl(eventData->bufferAddress(), eventData->dataLength());
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

#endif // _CONF_MAIN_NORMAL_CONTROLLER_ENABLED