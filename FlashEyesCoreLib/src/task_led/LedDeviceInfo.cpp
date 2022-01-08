#include "LedDeviceInfo.h"
#ifdef _CONF_LED_DEVICE_INFO_ENABLED
/////////////////////////////////////////////////
// INCLUDE

/////////////////////////////////////////////////
// PREPROCESSOR

/////////////////////////////////////////////////
// DEFINE
//#define LED_DEVICE_CONSOLE_DEBUG_ENABLE
/////////////////////////////////////////////////
// MARCO

/////////////////////////////////////////////////
// GLOBAL VARIABLES
#ifdef LED_DEVICE_CONSOLE_DEBUG_ENABLE
char ledDeviceInfoLogBuf[SYSTEM_CONSOLE_OUT_BUF_LEN];
#endif // LED_DEVICE_CONSOLE_DEBUG_ENABLE
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

/*LedDeviceInfo*/
LedDeviceInfo::LedDeviceInfo(void)
  : channel_Id(LED_DEVICE_CHANNEL_ID_INVALID)
  , led_Pin(LED_DEVICE_PIN_INVALID)
  , cur_Intensity(0)
  , default_Intensity(LED_DEVICE_INTENSITY_LEVEL_DEFAULT)
  , blink_Timer_Id(LED_DEVICE_BLINK_TIMER_ID_INVALID)
  , is_PWM(false)
  , is_On(false)
  , dev_Controller(NULL)
{
  
}

LedDeviceInfo::~LedDeviceInfo(void)
{
  this->clear();
}

bool LedDeviceInfo::isValid(void)
{
  return (this->channel_Id == LED_DEVICE_CHANNEL_ID_INVALID ? false : true);
}

bool LedDeviceInfo::isOn(void)
{
  do
  {
    if ( (this->isValid() == false)
      || (this->is_On == false)
      )
    {
      break;
    }

    return true;
  } while (0);
  return false;
}

LedChannelId_t LedDeviceInfo::channelId(void)
{
  return this->channel_Id;
}

int LedDeviceInfo::initialize(LedDeviceConfigTAG& deviceConfig)
{
  do
  {
    if (this->isValid() != false)
    {
      return 0; //already
    }

    this->clear();

    if ( (deviceConfig.channelId < LED_DEVICE_CHANNEL_ID_MIN)
      || (deviceConfig.channelId > LED_DEVICE_CHANNEL_ID_MAX)
      || (deviceConfig.defaultIntensity < LED_DEVICE_INTENSITY_LEVEL_MIN)
      || (deviceConfig.defaultIntensity > LED_DEVICE_INTENSITY_LEVEL_MAX)
      //|| (deviceConfig.devManager == NULL)
      || (deviceConfig.pin < LED_DEVICE_PIN_MIN)
      || (deviceConfig.pin > LED_DEVICE_PIN_MAX)
      )
    {
#ifdef LED_DEVICE_CONSOLE_DEBUG_ENABLE
      CONSOLE_LOG_BUF(ledDeviceInfoLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[E] ch=%u pi=%u dIn=%u", deviceConfig.channelId, deviceConfig.pin, deviceConfig.defaultIntensity);
      //CONSOLE_LOG_BUF(ledDeviceInfoLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[E] dv=%s", (deviceConfig.devManager == NULL? "n" : "v"));
#endif // LED_DEVICE_CONSOLE_DEBUG_ENABLE
      break;
    }

    {
      this->channel_Id = deviceConfig.channelId;
      this->led_Pin = deviceConfig.pin;
      this->default_Intensity = deviceConfig.defaultIntensity;
      this->cur_Intensity = 0;
      this->blink_Timer_Id = LED_DEVICE_BLINK_TIMER_ID_INVALID;
      this->is_PWM = deviceConfig.isPWM;
      this->dev_Controller = NULL;// deviceConfig.devManager;
      this->is_On = false;
    }

    return 0;
  } while (0);
  this->clear();
  return -1;
}

void LedDeviceInfo::clear(void)
{
  this->channel_Id = LED_DEVICE_CHANNEL_ID_INVALID;
  this->led_Pin = LED_DEVICE_PIN_INVALID;
  this->default_Intensity = LED_DEVICE_INTENSITY_LEVEL_DEFAULT;
  this->cur_Intensity = 0;
  this->blink_Timer_Id = LED_DEVICE_BLINK_TIMER_ID_INVALID;
  this->is_PWM = false;
  this->is_On = false;
  this->dev_Controller = NULL;
}

void LedDeviceInfo::dump(void)
{
#ifdef LED_DEVICE_CONSOLE_DEBUG_ENABLE
  do
  {
    if (this->isValid() == false)
    {
      CONSOLE_LOG_BUF(ledDeviceInfoLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[ledD] %s", "Invalid");
      break;
    }
    CONSOLE_LOG_BUF(ledDeviceInfoLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "  ch=%u pi=%u dIn=%u", this->channel_Id, this->led_Pin, this->default_Intensity);
    CONSOLE_LOG_BUF(ledDeviceInfoLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "  cIn=%u pwm=%u on=%u", this->cur_Intensity, this->is_PWM, this->is_On);
    return;
  } while (0);
  return;
#endif // LED_DEVICE_CONSOLE_DEBUG_ENABLE
}


#endif // _CONF_LED_DEVICE_INFO_ENABLED
