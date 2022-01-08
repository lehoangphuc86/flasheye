#include "BuzzerDeviceInfo.h"
#ifdef _CONF_BUZZER_DEVICE_INFO_ENABLED
/////////////////////////////////////////////////
// INCLUDE

/////////////////////////////////////////////////
// PREPROCESSOR

/////////////////////////////////////////////////
// DEFINE
#define BUZZER_DEVICE_CONSOLE_DEBUG_ENABLE
/////////////////////////////////////////////////
// MARCO

/////////////////////////////////////////////////
// GLOBAL VARIABLES
#ifdef BUZZER_DEVICE_CONSOLE_DEBUG_ENABLE
char buzzerDeviceInfoLogBuf[SYSTEM_CONSOLE_OUT_BUF_LEN];
#endif // BUZZER_DEVICE_CONSOLE_DEBUG_ENABLE
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

/*BuzzerDeviceInfo*/
BuzzerDeviceInfo::BuzzerDeviceInfo(void)
  : hw_Channel_Id(BUZZER_DEVICE_HW_CHANNEL_ID_INVALID)
  , device_Id(BUZZER_DEVICE_ID_INVALID)
  , hw_Pin(BUZZER_DEVICE_ID_INVALID)
  , is_On(false)
  , dev_Controller(NULL)
{
  
}

BuzzerDeviceInfo::~BuzzerDeviceInfo(void)
{
  this->clear();
}

bool BuzzerDeviceInfo::isValid(void)
{
  return (this->device_Id == BUZZER_DEVICE_ID_INVALID ? false : true);
}

bool BuzzerDeviceInfo::isOn(void)
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

BuzzerDeviceId_t BuzzerDeviceInfo::deviceId(void)
{
  return this->device_Id;
}

int BuzzerDeviceInfo::initialize(BuzzerDeviceConfigTAG& deviceConfig)
{
  do
  {
    if (this->isValid() != false)
    {
      return 0; //already
    }

    this->clear();

    if ( (deviceConfig.deviceId < BUZZER_DEVICE_ID_MIN)
      || (deviceConfig.deviceId > BUZZER_DEVICE_ID_MAX)
      || (deviceConfig.hwChannelId < BUZZER_DEVICE_HW_CHANNEL_ID_MIN)
      || (deviceConfig.hwChannelId > BUZZER_DEVICE_HW_CHANNEL_ID_MAX)
      || (deviceConfig.pin < BUZZER_DEVICE_PIN_MIN)
      || (deviceConfig.pin > BUZZER_DEVICE_PIN_MAX)
      )
    {
#ifdef BUZZER_DEVICE_CONSOLE_DEBUG_ENABLE
      CONSOLE_LOG_BUF(buzzerDeviceInfoLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[E] ch=%u pi=%u dId=%u", deviceConfig.hwChannelId, deviceConfig.pin, deviceConfig.deviceId);
#endif // BUZZER_DEVICE_CONSOLE_DEBUG_ENABLE
      break;
    }

    {
      this->device_Id = deviceConfig.deviceId;
      this->hw_Pin = deviceConfig.pin;
      this->hw_Channel_Id = deviceConfig.hwChannelId;
      this->dev_Controller = NULL;
      this->is_On = false;
    }

    return 0;
  } while (0);
  this->clear();
  return -1;
}

void BuzzerDeviceInfo::clear(void)
{
  this->device_Id = BUZZER_DEVICE_ID_INVALID;
  this->hw_Pin = BUZZER_DEVICE_PIN_INVALID;
  this->hw_Channel_Id = BUZZER_DEVICE_HW_CHANNEL_ID_INVALID;
  this->is_On = false;
  this->dev_Controller = NULL;
}

void BuzzerDeviceInfo::dump(void)
{
#ifdef BUZZER_DEVICE_CONSOLE_DEBUG_ENABLE
  do
  {
    if (this->isValid() == false)
    {
      CONSOLE_LOG_BUF(buzzerDeviceInfoLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[bzD] %s", "Invalid");
      break;
    }
    CONSOLE_LOG_BUF(buzzerDeviceInfoLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "  ch=%u pi=%u dId=%u s=%u", this->hw_Channel_Id, this->hw_Pin, this->device_Id, this->is_On);
    return;
  } while (0);
  return;
#endif // BUZZER_DEVICE_CONSOLE_DEBUG_ENABLE
}


#endif // _CONF_BUZZER_DEVICE_INFO_ENABLED
