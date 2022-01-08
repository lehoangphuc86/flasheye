
#include "BuzzerManager.h"
#if (_CONF_BUZZER_MANAGER_ENABLED)
/////////////////////////////////////////////////
// INCLUDE

/////////////////////////////////////////////////
// PREPROCESSOR

/////////////////////////////////////////////////
// DEFINE
//#define BUZZER_MANAGER_CONSOLE_DEBUG_ENABLE

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
#ifdef BUZZER_MANAGER_CONSOLE_DEBUG_ENABLE
char buzzerMgrLogBuf[SYSTEM_CONSOLE_OUT_BUF_LEN];
#endif // BUZZER_MANAGER_CONSOLE_DEBUG_ENABLE
/////////////////////////////////////////////////
// STATIC FUNCTIONS

/////////////////////////////////////////////////
// EXTERN

/////////////////////////////////////////////////
// CLASS IMPLEMENTAION
/*BuzzerManager*/
BuzzerManager& BuzzerManager::getInstance(void)
{
  static BuzzerManager instance;
  return instance;
}

BuzzerManager::BuzzerManager(void)
{

}

BuzzerManager::~BuzzerManager(void)
{
  this->stop();
}

bool BuzzerManager::isValid(void)
{
  do
  {
    if (this->buzzer_Controller.isValid() == false)
    {
      break;
    }

    if (BuzzerDeviceManager::getInstance().isValid() == false)
    {
      break;
    }

    return true;
  } while (0);
  return false;
}

int BuzzerManager::start(BuzzerManagerConfigTAG& buzzerManagerConfig)
{
  int ret = 0;
  byte wk_idx = 0;
#ifdef BUZZER_MANAGER_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(buzzerMgrLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[bzm] st %i", 0);
#endif // BUZZER_MANAGER_CONSOLE_DEBUG_ENABLE
  do
  {
    if (buzzerManagerConfig.deviceConfig == NULL)
    {
      break;
    }
    // init device manager
    {
      BuzzerDeviceManagerConfigTAG devMgrConfig = BuzzerDeviceManagerConfigTAG();
      devMgrConfig.deviceMaxCount = buzzerManagerConfig.deviceCount;
      ret = BuzzerDeviceManager::getInstance().initialize(devMgrConfig);
      if (ret != 0)
      {
#ifdef BUZZER_MANAGER_CONSOLE_DEBUG_ENABLE
        CONSOLE_LOG_BUF(buzzerMgrLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[bzm] st %i", -1);
#endif // BUZZER_MANAGER_CONSOLE_DEBUG_ENABLE
        break;
      }
    }

    // append devices
    {
      ret = BuzzerDeviceManager::getInstance().appendDevices(buzzerManagerConfig.deviceConfig, buzzerManagerConfig.deviceCount);
      if (ret != 0)
      {
#ifdef BUZZER_MANAGER_CONSOLE_DEBUG_ENABLE
        CONSOLE_LOG_BUF(buzzerMgrLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[bzm] st %i", -2);
#endif // BUZZER_MANAGER_CONSOLE_DEBUG_ENABLE
        break;
      }
    }
    
    // start buzzer controller
    {
#ifdef BUZZER_MANAGER_CONSOLE_DEBUG_ENABLE
      CONSOLE_LOG_BUF(buzzerMgrLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[bzm] st %i", 10);
#endif // BUZZER_MANAGER_CONSOLE_DEBUG_ENABLE
      ret = this->buzzer_Controller.start(buzzerManagerConfig.controllerConfig);
      if (ret != 0)
      {
#ifdef BUZZER_MANAGER_CONSOLE_DEBUG_ENABLE
        CONSOLE_LOG_BUF(buzzerMgrLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[bzm] st %i", -3);
#endif // BUZZER_MANAGER_CONSOLE_DEBUG_ENABLE
        break;
      }
    }
#ifdef BUZZER_MANAGER_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(buzzerMgrLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[bzm] st %i", 99);
#endif // BUZZER_MANAGER_CONSOLE_DEBUG_ENABLE
    return 0;
  } while (0);
  this->stop();
  return -1;
}

void BuzzerManager::stop(void)
{
  this->buzzer_Controller.stop();
  BuzzerDeviceManager::getInstance().clear();
}

int BuzzerManager::control(BuzzerControlParamTAG& controlParam)
{
  return this->buzzer_Controller.control(controlParam);
}

int BuzzerManager::turnOnAll(BuzzerNoteFreq_t note, BuzzerNoteLen_t duration)
{
  return this->buzzer_Controller.turnOnAll(note, duration);
}

int BuzzerManager::turnOnAll(BuzzerNoteFreq_t* melody, BuzzerNoteLen_t* noteLen, BuzzerMelodyLen_t molodyLen)
{
  return this->buzzer_Controller.turnOnAll(melody, noteLen, molodyLen);
}

int BuzzerManager::turnOffAll(void)
{
  return this->buzzer_Controller.turnOffAll();
}

int BuzzerManager::turnOnMul(BuzzerDeviceIdSet_t deviceIdSet, BuzzerNoteFreq_t note, BuzzerNoteLen_t duration)
{
  return this->buzzer_Controller.turnOnMul(deviceIdSet, note, duration);
}

int BuzzerManager::turnOnMul(BuzzerDeviceIdSet_t deviceIdSet, BuzzerNoteFreq_t* melody, BuzzerNoteLen_t* noteLen, BuzzerMelodyLen_t molodyLen)
{
  return this->buzzer_Controller.turnOnMul(deviceIdSet, melody, noteLen, molodyLen);
}

int BuzzerManager::turnOffMul(BuzzerDeviceIdSet_t deviceIdSet)
{
  return this->buzzer_Controller.turnOffMul(deviceIdSet);
}

int BuzzerManager::turnOn(BuzzerDeviceId_t deviceId, BuzzerNoteFreq_t note, BuzzerNoteLen_t duration)
{
  return this->buzzer_Controller.turnOn(deviceId, note, duration);
}

int BuzzerManager::turnOn(BuzzerDeviceId_t deviceId, BuzzerNoteFreq_t* melody, BuzzerNoteLen_t* noteLen, BuzzerMelodyLen_t molodyLen)
{
  return this->buzzer_Controller.turnOn(deviceId, melody, noteLen, molodyLen);
}

int BuzzerManager::turnOff(BuzzerDeviceId_t deviceId)
{
  return this->buzzer_Controller.turnOff(deviceId);
}

#endif // _CONF_BUZZER_MANAGER_ENABLED