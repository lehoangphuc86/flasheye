/////////////////////////////////////////////////
// INCLUDE
#include "DisplayLCDController.h"
#if (_CONF_DISPLAY_LCD_CONTROLLER_ENABLED)
#include "../os_system/SystemCommon.h"
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

/* DisplayLCDController*/
DisplayLCDController::DisplayLCDController(void)
  : DisplayController(DisplayDeviceTypeUN::DisplayLCDDevice)
  , backlight_Pin(DISPLAY_DEVICE_PIN_INVALID)
  , brighness_Pin(DISPLAY_DEVICE_PIN_INVALID)
  , backlight_Level(DISPLAY_DEVICE_BACKLIGHT_LEVEL_DEFAULT)
  , brightness_Level(DISPLAY_DEVICE_BRIGHTNESS_LEVEL_DEFAULT)
  , lcd_Controller(NULL)
{

}
DisplayLCDController::~DisplayLCDController(void)
{

}

bool DisplayLCDController::isValid(void)
{
  do
  {
    if (DisplayController::isValid() == false)
    {
      break;
    }

    if (this->lcd_Controller == NULL)
    {
      break;
    }

    return true;
  } while (0);
  return false;
}

int DisplayLCDController::start(DisplayDeviceConfig& deviceConfig)
{
  do
  {
    if (this->isValid() != false)
    {
      return 0; //already
    }

    this->lcd_Controller = new LiquidCrystal(
      deviceConfig.spec.lcd.pin_RS
      , deviceConfig.spec.lcd.pin_RW
      , deviceConfig.spec.lcd.pin_E
      , deviceConfig.spec.lcd.pin_D4
      , deviceConfig.spec.lcd.pin_D5
      , deviceConfig.spec.lcd.pin_D6
      , deviceConfig.spec.lcd.pin_D7
    );
    this->lcd_Controller->begin(deviceConfig.columnNo, deviceConfig.rowNo);
    this->backlight_Pin = deviceConfig.spec.lcd.pin_A_Backlight;
    this->brighness_Pin = deviceConfig.spec.lcd.pin_V0_Brightness;
    this->backlight_Level = deviceConfig.spec.lcd.initBacklight;
    this->brightness_Level = deviceConfig.spec.lcd.initBrightness;

    if (this->backlight_Pin != DISPLAY_DEVICE_PIN_INVALID)
    {
      PIN_MODE(this->backlight_Pin, GPIO_OUTPUT);
      this->setBacklight(this->backlight_Level);
    }

    if (this->brighness_Pin != DISPLAY_DEVICE_PIN_INVALID)
    {
      PIN_MODE(this->brighness_Pin, GPIO_OUTPUT);
      this->setBrightness(this->brightness_Level);
    }

    return 0;
  } while (0);
  this->stop();
  return -1;
}

void DisplayLCDController::stop(void)
{
  if (this->lcd_Controller != NULL )
  {
    delete this->lcd_Controller;
  }
  this->lcd_Controller = NULL;
}

// controlling functions
void DisplayLCDController::clear()
{
  this->lcd_Controller->clear();
}

void DisplayLCDController::home()
{
  this->lcd_Controller->home();
}

void DisplayLCDController::noDisplay()
{
  this->lcd_Controller->noDisplay();
}

void DisplayLCDController::display()
{
  this->lcd_Controller->display();
}

void DisplayLCDController::noBlink()
{
  this->lcd_Controller->noBlink();
}

void DisplayLCDController::blink()
{
  this->lcd_Controller->blink();
}

void DisplayLCDController::noCursor()
{
  this->lcd_Controller->noCursor();
}

void DisplayLCDController::cursor()
{
  this->lcd_Controller->cursor();
}

void DisplayLCDController::scrollDisplayLeft()
{
  this->lcd_Controller->scrollDisplayLeft();
}

void DisplayLCDController::scrollDisplayRight()
{
  this->lcd_Controller->scrollDisplayRight();
}

void DisplayLCDController::leftToRight()
{
  this->lcd_Controller->leftToRight();
}

void DisplayLCDController::rightToLeft()
{
  this->lcd_Controller->rightToLeft();
}

void DisplayLCDController::autoscroll()
{
  this->lcd_Controller->autoscroll();
}

void DisplayLCDController::noAutoscroll()
{
  this->lcd_Controller->noAutoscroll();
}


void DisplayLCDController::setRowOffsets(int row1, int row2, int row3, int row4)
{
  this->lcd_Controller->setRowOffsets(row1, row2, row3, row4);
}

void DisplayLCDController::createChar(uint8_t charId, uint8_t charArray[])
{
  this->lcd_Controller->createChar(charId, charArray);
}

void DisplayLCDController::setCursor(uint8_t col, uint8_t row)
{
  this->lcd_Controller->setCursor(col, row);
}

void DisplayLCDController::setBacklight(byte level)
{
  do
  {
    if (this->backlight_Pin == DISPLAY_DEVICE_PIN_INVALID)
    {
      break;
    }

    ANALOG_WRITE(this->backlight_Pin, level);
    this->backlight_Level = level;
    return;
  } while (0);

  return;
}

void DisplayLCDController::setBrightness(byte level)
{
  do
  {
    if (this->brighness_Pin == DISPLAY_DEVICE_PIN_INVALID)
    {
      break;
    }

    ANALOG_WRITE(this->brighness_Pin, level);
    this->brightness_Level = level;
    return;
  } while (0);

  return;
}


size_t DisplayLCDController::print(const __FlashStringHelper* param)
{
  return this->lcd_Controller->print(param);
}

size_t DisplayLCDController::print(const String& param)
{
  return this->lcd_Controller->print(param);
}

size_t DisplayLCDController::print(const char param[])
{
  return this->lcd_Controller->print(param);
}

size_t DisplayLCDController::print(char  param)
{
  return this->lcd_Controller->print(param);
}

size_t DisplayLCDController::print(unsigned char  param1, int param2)
{
  return this->lcd_Controller->print(param1, param2);
}

size_t DisplayLCDController::print(int param1, int param2)
{
  return this->lcd_Controller->print(param1, param2);
}

size_t DisplayLCDController::print(unsigned int param1, int param2)
{
  return this->lcd_Controller->print(param1, param2);
}

size_t DisplayLCDController::print(long param1, int param2)
{
  return this->lcd_Controller->print(param1, param2);
}

size_t DisplayLCDController::print(unsigned long param1, int param2)
{
  return this->lcd_Controller->print(param1, param2);
}

size_t DisplayLCDController::print(double param1, int param2)
{
  return this->lcd_Controller->print(param1, param2);
}

size_t DisplayLCDController::print(const Printable& param1)
{
  return this->lcd_Controller->print(param1);
}

size_t DisplayLCDController::println(const __FlashStringHelper* param1)
{
  return this->lcd_Controller->println(param1);
}

size_t DisplayLCDController::println(const String& s)
{
  return this->lcd_Controller->println(s);
}

size_t DisplayLCDController::println(const char param[])
{
  return this->lcd_Controller->println(param);
}

size_t DisplayLCDController::println(char param)
{
  return this->lcd_Controller->println(param);
}

size_t DisplayLCDController::println(unsigned char param1, int param2)
{
  return this->lcd_Controller->println(param1, param2);
}

size_t DisplayLCDController::println(int param1, int param2 )
{
  return  this->lcd_Controller->println(param1, param2);
}

size_t DisplayLCDController::println(unsigned int param1, int param2)
{
  return this->lcd_Controller->println(param1, param2);
}

size_t DisplayLCDController::println(long param1, int param2)
{
  return this->lcd_Controller->println(param1, param2);
}

size_t DisplayLCDController::println(unsigned long param1, int param2)
{
  return this->lcd_Controller->println(param1, param2);
}

size_t DisplayLCDController::println(double param1, int param2)
{
  return this->lcd_Controller->println(param1, param2);
}

size_t DisplayLCDController::println(const Printable& param)
{
  return this->lcd_Controller->println(param);
}

size_t DisplayLCDController::println(void)
{
  return this->lcd_Controller->println();
}

#endif // _CONF_DISPLAY_LCD_CONTROLLER_ENABLED