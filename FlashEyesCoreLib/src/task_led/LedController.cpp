/////////////////////////////////////////////////
// INCLUDE
#include "LedController.h"
#if (_CONF_LED_CONTROLLER_ENABLED)

#include "timer_manager/TimerManager.h"
/////////////////////////////////////////////////
// PREPROCESSOR

//#define LED_CONTROLLER_CONSOLE_DEBUG_ENABLE
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
#ifdef LED_CONTROLLER_CONSOLE_DEBUG_ENABLE
char ledControllerLogBuf[SYSTEM_CONSOLE_OUT_BUF_LEN];
#endif // LED_CONTROLLER_CONSOLE_DEBUG_ENABLE
/////////////////////////////////////////////////
// STATIC FUNCTIONS

/////////////////////////////////////////////////
// EXTERN

/////////////////////////////////////////////////
// CLASS IMPLEMENTAION

/* LedController*/
LedController::LedController(void)
{

}

LedController::~LedController(void)
{
  this->stop();
}

bool LedController::isValid(void)
{
  return true;
}

int LedController::start(LedControllerConfigTAG& controllerConfig)
{
  int ret = 0;
  byte wk_Idx = 0;
  byte deviceCount = 0;
  do
  {
    {
      SystemMutexLocker locker(this->mutex_Key);
      deviceCount = LedDeviceManager::getInstance().deviceCount();
      LedDeviceInfo* device = NULL;
      // init pin
      for (wk_Idx = 0; wk_Idx < deviceCount; wk_Idx++)
      {
        device = LedDeviceManager::getInstance().deviceByIndex(wk_Idx);
        if (device->isValid() == false)
        {
          break;
        }
        device->dev_Controller = this;
        PIN_MODE(device->led_Pin, LED_DEVICE_PIN_MODE);
      }
    }

    if (wk_Idx < deviceCount)
    {
      break;
    }

    // turn off all
    this->turnOffAll();
    return 0;
  } while (0);
  this->stop();
  return -1;
}


void LedController::stop(void)
{
  this->turnOffAll();
}

int LedController::turnOnAll(byte intensity, LedBlinkTime_t blinkTime)
{
  return this->turnOnMul(LED_DEVICE_CHANNEL_ID_SET_ALL_DEVICE, intensity, blinkTime);
}

int LedController::turnOffAll(void)
{
  return this->turnOffMul(LED_DEVICE_CHANNEL_ID_SET_ALL_DEVICE);
}

int LedController::control(LedControlParamTAG& controlParam)
{
  bool error = false;
  do
  {
    /// each channel id
    // call sub func
    switch (controlParam.cmd)
    {
      case LED_CMD_TURN_ON:
        return this->turnOnMul(controlParam.channelIdSet, controlParam.intensity, controlParam.blinkTime);
      case LED_CMD_TURN_OFF:
        return this->turnOffMul(controlParam.channelIdSet);
      /*case LED_CMD_BLINK:
        return this->blinkMul(controlParam.channelIdSet, controlParam.param.blinkTime);*/
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


int LedController::turnOnMul(LedChannelIdSet_t channelIdSet, byte intensity, LedBlinkTime_t blinkTime)
{
  int wk_idx = 0;
  do
  {
    for (wk_idx = 0; wk_idx <LED_DEVICE_CHANNEL_ID_MAX; wk_idx++)
    {
      if (((1 << wk_idx) & channelIdSet) == 0)
      {
        continue;
      }
      this->turnOn(wk_idx, intensity, blinkTime);
    }

    return 0;
  } while (0);
  return -1;
}

int LedController::turnOffMul(LedChannelIdSet_t channelIdSet)
{
  int wk_idx = 0;
  do
  {
    for (wk_idx = 0; wk_idx < LED_DEVICE_CHANNEL_ID_MAX; wk_idx++)
    {
      if (((1 << wk_idx) & channelIdSet) == 0)
      {
        continue;
      }
      this->turnOff(wk_idx);
    }

    return 0;
  } while (0);
  return -1;
}



int LedController::turnOn(LedChannelId_t channelId, byte intensity, LedBlinkTime_t blinkTime)
{
  do
  {
    LedDeviceInfo* device = LedDeviceManager::getInstance().deviceByChannel(channelId);
    if (device == NULL)
    {
      break;
    }

    this->turnOff(channelId);

    {
      SystemMutexLocker locker(this->mutex_Key);
      if ((intensity < LED_DEVICE_INTENSITY_LEVEL_MIN)
        || (intensity > LED_DEVICE_INTENSITY_LEVEL_MAX)
        )
      {
        intensity = device->default_Intensity;
      }
      
      device->cur_Intensity = intensity;
      device->is_On = true;
#ifdef LED_CONTROLLER_CONSOLE_DEBUG_ENABLE
      CONSOLE_LOG_BUF(ledControllerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[ledC] on %i %u %lu", 0, channelId, blinkTime);
#endif // LED_CONTROLLER_CONSOLE_DEBUG_ENABLE
      if (device->is_PWM == false)
      {
        SYSTEM_PIN_DIGITAL_WRITE(device->led_Pin, SYSTEM_PIN_HIGH);
      }
      else
      {
        // pwm
        SYSTEM_PIN_ANALOG_WRITE(device->led_Pin, device->cur_Intensity);
      }

      if (blinkTime >= LED_DEVICE_BLINK_TIME_MIN)
      {
        TimerInfoTAG timeConfig = TimerInfoTAG();
        timeConfig.enabled = true;
        timeConfig.handler = LedController::cbTimerFired;
        timeConfig.extraArgument = device;
        timeConfig.interval = blinkTime* TIMER_DEVICE_TIMER_MS_2_UNIT;
        timeConfig.oneShot = false;
        device->blink_Timer_Id = TimerManager::getInstance().start(timeConfig);
#ifdef LED_CONTROLLER_CONSOLE_DEBUG_ENABLE
        CONSOLE_LOG_BUF(ledControllerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[ledC] on %i %u %ld", 2, device->blink_Timer_Id, (long)timeConfig.interval);
#endif // LED_CONTROLLER_CONSOLE_DEBUG_ENABLE
        if (device->blink_Timer_Id == LED_DEVICE_BLINK_TIMER_ID_INVALID)
        {
          break;
        }
      }
    }

    return 0;
  } while (0);
  return -1;
}

int LedController::turnOff(LedChannelId_t channelId)
{
  do
  {
    LedDeviceInfo* device = LedDeviceManager::getInstance().deviceByChannel(channelId);
    if (device == NULL)
    {
      break;
    }

    {
      SystemMutexLocker locker(this->mutex_Key);
      if (device->blink_Timer_Id != LED_DEVICE_BLINK_TIMER_ID_INVALID)
      {
        TimerManager::getInstance().stop(device->blink_Timer_Id);
        device->blink_Timer_Id = LED_DEVICE_BLINK_TIMER_ID_INVALID;
      }
      
      device->cur_Intensity = 0;
      device->is_On = false;
#ifdef LED_CONTROLLER_CONSOLE_DEBUG_ENABLE
      CONSOLE_LOG_BUF(ledControllerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[ledC] of %u", channelId);
#endif // LED_CONTROLLER_CONSOLE_DEBUG_ENABLE
      if (device->is_PWM == false)
      {
        SYSTEM_PIN_DIGITAL_WRITE(device->led_Pin, SYSTEM_PIN_LOW);
        return 0;
      }
      // pwm
      SYSTEM_PIN_ANALOG_WRITE(device->led_Pin, 0);
    }

    return 0;
  } while (0);
  return -1;
}

int LedController::blinkFromISR(LedDeviceInfo* device, TimerId_t timerId, bool* woken)
{
  do
  {
    if (device == NULL)
    {
      break;
    }
#ifdef LED_CONTROLLER_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_ISR(ledControllerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[ledC] bl %d", 1);
#endif // LED_CONTROLLER_CONSOLE_DEBUG_ENABLE
    bool woken = false;
    {
      SystemMutexLockerFromISR locker(this->mutex_Key, &woken);
      if (timerId != device->blink_Timer_Id)
      {
        break;
      }
      byte dValue = SYSTEM_PIN_HIGH;
      byte aValue = device->cur_Intensity;
      if (device->is_On != false)
      {
        dValue = SYSTEM_PIN_LOW;
        aValue = 0;
      }

      device->is_On = !device->is_On;

      if (device->is_PWM == false)
      {
        SYSTEM_PIN_DIGITAL_WRITE(device->led_Pin, dValue);
      }
      else
      {
        // pwm
        SYSTEM_PIN_ANALOG_WRITE(device->led_Pin, aValue);
      }
    }

    return 0;
  } while (0);
  return -1;
}

void LedController::cbTimerFired(TimerId_t timerId, void* extraArg, bool* woken)
{
  do
  {
#ifdef LED_CONTROLLER_CONSOLE_DEBUG_ENABLE
    //CONSOLE_LOG_ISR(ledControllerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[ledC] cb %i", 0);
#endif // LED_CONTROLLER_CONSOLE_DEBUG_ENABLE

    if (extraArg == NULL)
    {
      break;
    }

    LedDeviceInfo* device = (LedDeviceInfo*)extraArg;
    if (device->dev_Controller == NULL)
    {
      break;
    }
    device->dev_Controller->blinkFromISR(device, timerId, woken);

    return;
  } while (0);
  return;
}

#endif // _CONF_LED_CONTROLLER_ENABLED