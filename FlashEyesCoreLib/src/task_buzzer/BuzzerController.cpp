/////////////////////////////////////////////////
// INCLUDE
#include "BuzzerController.h"
#if (_CONF_BUZZER_CONTROLLER_ENABLED)
#include "../libs/buzzer/Buzzer.h"
/////////////////////////////////////////////////
// PREPROCESSOR

//#define BUZZER_CONTROLLER_CONSOLE_DEBUG_ENABLE
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
#ifdef BUZZER_CONTROLLER_CONSOLE_DEBUG_ENABLE
char buzzerControllerLogBuf[SYSTEM_CONSOLE_OUT_BUF_LEN];
#endif // BUZZER_CONTROLLER_CONSOLE_DEBUG_ENABLE
/////////////////////////////////////////////////
// STATIC FUNCTIONS

/////////////////////////////////////////////////
// EXTERN

/////////////////////////////////////////////////
// CLASS IMPLEMENTAION

/* BuzzerController*/
BuzzerController::BuzzerController(void)
{

}

BuzzerController::~BuzzerController(void)
{
  this->stop();
}

bool BuzzerController::isValid(void)
{
  return true;
}

int BuzzerController::start(BuzzerControllerConfigTAG& controllerConfig)
{
  int ret = 0;
  byte wk_Idx = 0;
  byte deviceCount = 0;
  do
  {
#ifdef BUZZER_CONTROLLER_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(buzzerControllerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[bzC] st %i", 0);
#endif // BUZZER_CONTROLLER_CONSOLE_DEBUG_ENABLE
    {
      SystemMutexLocker locker(this->mutex_Key);
      deviceCount = BuzzerDeviceManager::getInstance().deviceCount();
      BuzzerDeviceInfo* device = NULL;
      // init pin
      for (wk_Idx = 0; wk_Idx < deviceCount; wk_Idx++)
      {
        device = BuzzerDeviceManager::getInstance().deviceByIndex(wk_Idx);
        if (device->isValid() == false)
        {
          break;
        }
        device->dev_Controller = this;
        ledcAttachPin(device->hw_Pin, device->hw_Channel_Id);
      }
    }
#ifdef BUZZER_CONTROLLER_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(buzzerControllerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[bzC] st %i", 10);
#endif // BUZZER_CONTROLLER_CONSOLE_DEBUG_ENABLE
    if (wk_Idx < deviceCount)
    {
      break;
    }
#ifdef BUZZER_CONTROLLER_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(buzzerControllerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[bzC] st %i", 98);
#endif // BUZZER_CONTROLLER_CONSOLE_DEBUG_ENABLE
    // turn off all
    this->turnOffAll();
#ifdef BUZZER_CONTROLLER_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(buzzerControllerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[bzC] st %i", 99);
#endif // BUZZER_CONTROLLER_CONSOLE_DEBUG_ENABLE
    return 0;
  } while (0);
  this->stop();
  return -1;
}


void BuzzerController::stop(void)
{
  byte wk_Idx = 0;
  byte deviceCount = 0;
  do
  {
    this->turnOffAll();
    {
      SystemMutexLocker locker(this->mutex_Key);
      deviceCount = BuzzerDeviceManager::getInstance().deviceCount();
      BuzzerDeviceInfo* device = NULL;
      // detach pin
      for (wk_Idx = 0; wk_Idx < deviceCount; wk_Idx++)
      {
        device = BuzzerDeviceManager::getInstance().deviceByIndex(wk_Idx);
        if (device->isValid() == false)
        {
          continue;
        }
        ledcDetachPin(device->hw_Pin);
      }
    }

    return;
  } while (0);
  return;
}

int BuzzerController::turnOnAll(BuzzerNoteFreq_t note, BuzzerNoteLen_t duration)
{
  return this->turnOnMul(BUZZER_DEVICE_ID_SET_ALL_DEVICE, note, duration);
}

int BuzzerController::turnOnAll(BuzzerNoteFreq_t* melody, BuzzerNoteLen_t* noteLen, BuzzerMelodyLen_t molodyLen)
{
  return this->turnOnMul(BUZZER_DEVICE_ID_SET_ALL_DEVICE, melody, noteLen, molodyLen);
}

int BuzzerController::turnOffAll(void)
{
  return this->turnOffMul(BUZZER_DEVICE_ID_SET_ALL_DEVICE);
}

int BuzzerController::control(BuzzerControlParamTAG& controlParam)
{
  bool error = false;
  do
  {
    /// each channel id
    // call sub func
    switch (controlParam.cmd)
    {
      case BUZZER_CMD_TURN_ON:
        return this->turnOnMul(controlParam.deviceIdSet, controlParam.melody, controlParam.noteLen, controlParam.melodyLen);
      case BUZZER_CMD_TURN_OFF:
        return this->turnOffMul(controlParam.deviceIdSet);
      default:
        error = true;
        break;
    }

    if (error != false)
    {
      break;
    }

    return 0;
  } while (0);
  return -1;
}


int BuzzerController::turnOnMul(BuzzerDeviceIdSet_t deviceIdSet, BuzzerNoteFreq_t note, BuzzerNoteLen_t duration)
{
  int wk_idx = 0;
  do
  {
    for (wk_idx = 0; wk_idx <= BUZZER_DEVICE_ID_MAX; wk_idx++)
    {
      if (((1 << wk_idx) & deviceIdSet) == 0)
      {
        continue;
      }
      this->turnOn(wk_idx, note, duration);
    }

    return 0;
  } while (0);
  return -1;
}

int BuzzerController::turnOnMul(BuzzerDeviceIdSet_t deviceIdSet, BuzzerNoteFreq_t* melody, BuzzerNoteLen_t* noteLen, BuzzerMelodyLen_t molodyLen)
{
  int wk_idx = 0;
  do
  {
    for (wk_idx = 0; wk_idx <= BUZZER_DEVICE_ID_MAX; wk_idx++)
    {
      if (((1 << wk_idx) & deviceIdSet) == 0)
      {
        continue;
      }
      this->turnOn(wk_idx, melody, noteLen, molodyLen);
    }

    return 0;
  } while (0);
  return -1;
}

int BuzzerController::turnOffMul(BuzzerDeviceIdSet_t deviceIdSet)
{
  int wk_idx = 0;
  do
  {
    for (wk_idx = 0; wk_idx <= BUZZER_DEVICE_ID_MAX; wk_idx++)
    {
      if (((1 << wk_idx) & deviceIdSet) == 0)
      {
        continue;
      }
      this->turnOff(wk_idx);
    }

    return 0;
  } while (0);
  return -1;
}



int BuzzerController::turnOn(BuzzerDeviceId_t deviceId, BuzzerNoteFreq_t note, BuzzerNoteLen_t duration)
{
  return this->turnOn(deviceId, &note, &duration, 1);
}

int BuzzerController::turnOn(BuzzerDeviceId_t deviceId, BuzzerNoteFreq_t* melody, BuzzerNoteLen_t* noteLen, BuzzerMelodyLen_t molodyLen)
{
  byte wk_idx = 0;
  do
  {
    BuzzerDeviceInfo* device = BuzzerDeviceManager::getInstance().deviceByDeviceId(deviceId);
    if (device == NULL)
    {
      break;
    }

    this->turnOff(deviceId);

    {
      SystemMutexLocker locker(this->mutex_Key);
      if ( (melody == NULL)
        || (noteLen == NULL)
        || (molodyLen <= 0)
        )
      {
        break;
      }
      
      device->is_On = true;
      for (wk_idx = 0; wk_idx < molodyLen; wk_idx++)
      {
        SYSTEM_TONE(device->hw_Pin, melody[wk_idx], noteLen[wk_idx], device->hw_Channel_Id);
      }
    }

    return 0;
  } while (0);
  return -1;
}

int BuzzerController::turnOff(BuzzerDeviceId_t deviceId)
{
  do
  {
    BuzzerDeviceInfo* device = BuzzerDeviceManager::getInstance().deviceByDeviceId(deviceId);
    if (device == NULL)
    {
      break;
    }

#ifdef BUZZER_CONTROLLER_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(buzzerControllerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[bzC] of %i %i", 1, deviceId);
#endif // BUZZER_CONTROLLER_CONSOLE_DEBUG_ENABLE
    {
      SystemMutexLocker locker(this->mutex_Key);
      if (device->is_On == false)
      {
        return 0;
      }

      device->is_On = false;
      SYSTEM_NOTONE(device->hw_Pin, device->hw_Channel_Id);
    }
#ifdef BUZZER_CONTROLLER_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(buzzerControllerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[bzC] of %i", 99);
#endif // BUZZER_CONTROLLER_CONSOLE_DEBUG_ENABLE
    return 0;
  } while (0);
  return -1;
}

#endif // _CONF_BUZZER_CONTROLLER_ENABLED