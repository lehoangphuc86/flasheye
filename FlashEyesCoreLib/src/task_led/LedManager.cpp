
#include "LedManager.h"
#if (_CONF_LED_MANAGER_ENABLED)
/////////////////////////////////////////////////
// INCLUDE

/////////////////////////////////////////////////
// PREPROCESSOR

/////////////////////////////////////////////////
// DEFINE
//#define LED_MANAGER_CONSOLE_DEBUG_ENABLE

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
#ifdef LED_MANAGER_CONSOLE_DEBUG_ENABLE
char ledMgrLogBuf[SYSTEM_CONSOLE_OUT_BUF_LEN];
#endif // LED_MANAGER_CONSOLE_DEBUG_ENABLE
/////////////////////////////////////////////////
// STATIC FUNCTIONS

/////////////////////////////////////////////////
// EXTERN

/////////////////////////////////////////////////
// CLASS IMPLEMENTAION
/*LedManager*/
LedManager& LedManager::getInstance(void)
{
  static LedManager instance;
  return instance;
}

LedManager::LedManager(void)
{

}

LedManager::~LedManager(void)
{
  this->stop();
}

bool LedManager::isValid(void)
{
  do
  {
    if (this->led_Controller.isValid() == false)
    {
      break;
    }

    if (LedDeviceManager::getInstance().isValid() == false)
    {
      break;
    }

    return true;
  } while (0);
  return false;
}

int LedManager::start(LedManagerConfigTAG& ledManagerConfig)
{
  int ret = 0;
  byte wk_idx = 0;
#ifdef LED_MANAGER_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(ledMgrLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[lm] st %i", 0);
#endif // LED_MANAGER_CONSOLE_DEBUG_ENABLE
  do
  {
    if (ledManagerConfig.deviceConfig == NULL)
    {
      break;
    }
    // init device manager
    {
      LedDeviceManagerConfigTAG devMgrConfig = LedDeviceManagerConfigTAG();
      devMgrConfig.deviceMaxCount = ledManagerConfig.deviceCount;
      ret = LedDeviceManager::getInstance().initialize(devMgrConfig);
      if (ret != 0)
      {
#ifdef LED_MANAGER_CONSOLE_DEBUG_ENABLE
        CONSOLE_LOG_BUF(ledMgrLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[lm] st %i", -1);
#endif // LED_MANAGER_CONSOLE_DEBUG_ENABLE
        break;
      }
    }

    // append devices
    {
      ret = LedDeviceManager::getInstance().appendDevices(ledManagerConfig.deviceConfig, ledManagerConfig.deviceCount);
      if (ret != 0)
      {
#ifdef LED_MANAGER_CONSOLE_DEBUG_ENABLE
        CONSOLE_LOG_BUF(ledMgrLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[lm] st %i", -2);
#endif // LED_MANAGER_CONSOLE_DEBUG_ENABLE
        break;
      }
    }
    
    // start led controller
    {
      ret = this->led_Controller.start(ledManagerConfig.controllerConfig);
      if (ret != 0)
      {
#ifdef LED_MANAGER_CONSOLE_DEBUG_ENABLE
        CONSOLE_LOG_BUF(ledMgrLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[lm] st %i", -3);
#endif // LED_MANAGER_CONSOLE_DEBUG_ENABLE
        break;
      }
    }

    return 0;
  } while (0);
  this->stop();
  return -1;
}

void LedManager::stop(void)
{
  this->led_Controller.stop();
  LedDeviceManager::getInstance().clear();
}

int LedManager::control(LedControlParamTAG& controlParam)
{
  return this->led_Controller.control(controlParam);
}

int LedManager::turnOnAll(byte intensity, LedBlinkTime_t blinkTime)
{
  return this->led_Controller.turnOnAll(intensity, blinkTime);
}

int LedManager::turnOffAll(void)
{
  return this->led_Controller.turnOffAll();
}

int LedManager::turnOnMul(LedChannelIdSet_t channelIdSet, byte intensity, LedBlinkTime_t blinkTime)
{
  return this->led_Controller.turnOnMul(channelIdSet, intensity, blinkTime);
}

int LedManager::turnOffMul(LedChannelIdSet_t channelIdSet)
{
  return this->led_Controller.turnOffMul(channelIdSet);
}

int LedManager::turnOn(LedChannelId_t channelId, byte intensity, LedBlinkTime_t blinkTime)
{
  return this->led_Controller.turnOn(channelId, intensity, blinkTime);
}

int LedManager::turnOff(LedChannelId_t channelId)
{
  return this->led_Controller.turnOff(channelId);
}
#endif // _CONF_LED_MANAGER_ENABLED