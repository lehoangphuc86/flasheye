
/////////////////////////////////////////////////
// INCLUDE
#include "DisplayDeviceInfo.h"
#if (_CONF_DISPLAY_DEVICE_INFO_ENABLED)
/////////////////////////////////////////////////
// PREPROCESSOR

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

/////////////////////////////////////////////////
// STATIC FUNCTIONS

/////////////////////////////////////////////////
// EXTERN

/////////////////////////////////////////////////
// CLASS IMPLEMENTAION

/* DisplayDeviceInfo*/
DisplayDeviceInfo::DisplayDeviceInfo(void)
  : backlight_Level(DISPLAY_DEVICE_BACKLIGHT_LEVEL_DEFAULT)
  , brightness_Level(DISPLAY_DEVICE_BRIGHTNESS_LEVEL_DEFAULT)
{
  memset(&this->device_Config, 0, sizeof(DisplayDeviceConfig));
}
DisplayDeviceInfo::~DisplayDeviceInfo(void)
{

}

bool DisplayDeviceInfo::isValidConfig(void)
{
  return this->isValidConfig(this->device_Config);
}

bool DisplayDeviceInfo::isBacklightControllable(void)
{
  return (this->device_Config.pin_A != DISPLAY_DEVICE_PIN_INVALID ? true : false);
}

bool DisplayDeviceInfo::isBrightnessControllable(void)
{
  return (this->device_Config.pin_V0 != DISPLAY_DEVICE_PIN_INVALID ? true : false);
}

byte DisplayDeviceInfo::backlightPin(void)
{
  return this->device_Config.pin_A;
}

byte DisplayDeviceInfo::brightnessPin(void)
{
  return this->device_Config.pin_V0;
}

DisplayDeviceConfig* DisplayDeviceInfo::deviceConfig(void)
{
  return &this->device_Config;
}

byte DisplayDeviceInfo::backlightLevel(void)
{
  return this->backlight_Level;
}

byte DisplayDeviceInfo::brightnessLevel(void)
{
  return this->brightness_Level;
}

void DisplayDeviceInfo::backlightLevel(byte val)
{
  this->backlight_Level = val;
}

void DisplayDeviceInfo::brightnessLevel(byte val)
{
  this->brightness_Level = val;
}

int DisplayDeviceInfo::setConfig(DisplayDeviceConfig& deviceConfig)
{
  do
  {
    if (this->isValidConfig(deviceConfig) == false)
    {
      break;
    }
    memcpy(&this->device_Config, &deviceConfig, sizeof(DisplayDeviceConfig));
    this->backlight_Level = deviceConfig.initBacklight;
    this->brightness_Level = deviceConfig.initBrightness;
    return 0;
  } while (0);
  return -1;
}

int DisplayDeviceInfo::prepare(void)
{
  return 0;
}

bool DisplayDeviceInfo::isValidConfig(DisplayDeviceConfig& deviceConfig)
{
  do
  {
    if ( (deviceConfig.rowNo < DISPLAY_DEVICE_ROW_MIN)
      || (deviceConfig.columnNo < DISPLAY_DEVICE_COLUMN_MIN)
      || (deviceConfig.pin_RS < DISPLAY_DEVICE_PIN_MIN)
      || (deviceConfig.pin_RS > DISPLAY_DEVICE_PIN_MAX)
      || (deviceConfig.pin_RW < DISPLAY_DEVICE_PIN_MIN)
      || (deviceConfig.pin_RW > DISPLAY_DEVICE_PIN_MAX)
      || (deviceConfig.pin_E < DISPLAY_DEVICE_PIN_MIN)
      || (deviceConfig.pin_E > DISPLAY_DEVICE_PIN_MAX)
      || (deviceConfig.pin_D4 < DISPLAY_DEVICE_PIN_MIN)
      || (deviceConfig.pin_D4 > DISPLAY_DEVICE_PIN_MAX)
      || (deviceConfig.pin_D5 < DISPLAY_DEVICE_PIN_MIN)
      || (deviceConfig.pin_D5 > DISPLAY_DEVICE_PIN_MAX)
      || (deviceConfig.pin_D6 < DISPLAY_DEVICE_PIN_MIN)
      || (deviceConfig.pin_D6 > DISPLAY_DEVICE_PIN_MAX)
      || (deviceConfig.pin_D7 < DISPLAY_DEVICE_PIN_MIN)
      || (deviceConfig.pin_D7 > DISPLAY_DEVICE_PIN_MAX)
      )
    {
      break;
    }
    // validate pin V0 if it is valid pin. 
    if ((deviceConfig.pin_V0 != DISPLAY_DEVICE_PIN_INVALID)
        &&
        ( (deviceConfig.pin_V0 < DISPLAY_DEVICE_PIN_MIN)
          || (deviceConfig.pin_V0 > DISPLAY_DEVICE_PIN_MAX)
        )
      )
    {
      break;
    }
    // validate pin A if it is valid pin. 
    if ((deviceConfig.pin_A != DISPLAY_DEVICE_PIN_INVALID)
      &&
      ( (deviceConfig.pin_A < DISPLAY_DEVICE_PIN_MIN)
        || (deviceConfig.pin_A > DISPLAY_DEVICE_PIN_MAX)
      )
    )
    {
      break;
    }


    return true;
  } while (0);

  return false;
}
#endif // _CONF_DISPLAY_DEVICE_INFO_ENABLED