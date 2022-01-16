/////////////////////////////////////////////////
// INCLUDE
#include "MainSettingController.h"
#if (_CONF_MAIN_SETTING_CONTROLLER_ENABLED)
/////////////////////////////////////////////////
// PREPROCESSOR
#define MAIN_SETTING_CONTROLLER_CONSOLE_DEBUG_ENABLE
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
#ifdef MAIN_SETTING_CONTROLLER_CONSOLE_DEBUG_ENABLE
char mainSettingControllerLogBuf[SYSTEM_CONSOLE_OUT_BUF_LEN];
#endif // MAIN_SETTING_CONTROLLER_CONSOLE_DEBUG_ENABLE
/////////////////////////////////////////////////
// STATIC FUNCTIONS

/////////////////////////////////////////////////
// EXTERN

/////////////////////////////////////////////////
// CLASS IMPLEMENTAION
/*MainSettingController*/

MainSettingController::MainSettingController(void)
  : MainController(SYS_MODE_SETTING)
{

}

MainSettingController::~MainSettingController(void)
{
  
}

bool MainSettingController::isValid(void)
{
  do
  {
    return MainController::isValid();
  } while (0);
  return false;
}


void MainSettingController::regEventSize(void)
{
  this->registerHanldingEventStructSize(sizeof(EventScanningDeviceSettingTAG));
  this->registerHanldingEventStructSize(sizeof(EventScanningDeviceSettingCompletedTAG));
  this->registerHanldingEventStructSize(sizeof(EventTimerFiredTAG));
}

int MainSettingController::prepare(void)
{
  int ret = 0;
  do
  {
#ifdef MAIN_SETTING_CONTROLLER_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(mainSettingControllerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[msTsk] pre %i", 0);
#endif // MAIN_SETTING_CONTROLLER_CONSOLE_DEBUG_ENABLE
    ret = MainController::prepare();
#ifdef MAIN_SETTING_CONTROLLER_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(mainSettingControllerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[msTsk] pre %i %i", 1, ret);
#endif // MAIN_SETTING_CONTROLLER_CONSOLE_DEBUG_ENABLE
    if (ret != 0)
    {
      break;
    }

    ret = this->startScanningTask();
#ifdef MAIN_SETTING_CONTROLLER_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(mainSettingControllerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[msTsk] pre %i %i", 2, ret);
#endif // MAIN_SETTING_CONTROLLER_CONSOLE_DEBUG_ENABLE
    if (ret != 0)
    {
      break;
    }

#ifdef MAIN_SETTING_CONTROLLER_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(mainSettingControllerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[msTsk] pre %i", 99);
#endif // MAIN_SETTING_CONTROLLER_CONSOLE_DEBUG_ENABLE
    return 0;
  } while (0);
#ifdef MAIN_SETTING_CONTROLLER_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(mainSettingControllerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[msTsk] pre %i", -99);
#endif // MAIN_SETTING_CONTROLLER_CONSOLE_DEBUG_ENABLE
  return -1;
}

void MainSettingController::clear(void)
{
  do
  {
    this->stopScanningTask();
    MainController::clear();
    return;
  } while (0);
  return;
}

int MainSettingController::onEventHandling(EventDataItem* eventData)
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
        EventScanningDeviceSettingTAG devSetting = EventScanningDeviceSettingTAG();
        devSetting.setting.settingIdSet0 = SCANNING_SETTING_ID_SET_ALL;
        this->notify(
          (int)EventManagerConstant::EventMessageId::ScanningDeviceSetting
          , sizeof(EventScanningDeviceSettingTAG)
          , (unsigned char*)&devSetting);
      }
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

int MainSettingController::onEventScanningDeviceSettingCompleted(unsigned char* data, unsigned int dataSize)
{
  int ret = 0;
  do
  {
#ifdef MAIN_SETTING_CONTROLLER_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(mainSettingControllerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[msTsk] seC %i", 0);
#endif // MAIN_SETTING_CONTROLLER_CONSOLE_DEBUG_ENABLE
    ret = MainController::onEventScanningDeviceSettingCompleted(data, dataSize);
#ifdef MAIN_SETTING_CONTROLLER_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(mainSettingControllerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[msTsk] seC %i %i", 98, ret);
#endif // MAIN_SETTING_CONTROLLER_CONSOLE_DEBUG_ENABLE
    if (ret != 0)
    {
      break;
    }
    // restart
    SYSTEM_SLEEP(1000);// wait 1 sec
    SYSTEM_REBOOT();
#ifdef MAIN_SETTING_CONTROLLER_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(mainSettingControllerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[msTsk] seC %i", 99);
#endif // MAIN_SETTING_CONTROLLER_CONSOLE_DEBUG_ENABLE
    return 0;
  } while (0);

#ifdef MAIN_SETTING_CONTROLLER_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(mainSettingControllerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[msTsk] seC %i", -99);
#endif // MAIN_SETTING_CONTROLLER_CONSOLE_DEBUG_ENABLE
  return -1;
}
#endif // _CONF_MAIN_SETTING_CONTROLLER_ENABLED