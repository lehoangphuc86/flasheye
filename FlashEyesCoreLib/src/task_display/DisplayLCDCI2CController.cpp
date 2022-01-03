/////////////////////////////////////////////////
// INCLUDE
#include "DisplayLCDCI2CController.h"
#if (_CONF_DISPLAY_LCD_I2C_CONTROLLER_ENABLED)
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

/* DisplayLCDI2CController*/
DisplayLCDI2CController::DisplayLCDI2CController(void)
  : DisplayController(DisplayDeviceTypeUN::DisplayLCDI2CDevice)
  , backlight_Pin(DISPLAY_DEVICE_PIN_INVALID)
  , brighness_Pin(DISPLAY_DEVICE_PIN_INVALID)
  , backlight_Level(DISPLAY_DEVICE_BACKLIGHT_LEVEL_DEFAULT)
  , brightness_Level(DISPLAY_DEVICE_BRIGHTNESS_LEVEL_DEFAULT)
  , lcd_Controller(NULL)
{

}
DisplayLCDI2CController::~DisplayLCDI2CController(void)
{

}

bool DisplayLCDI2CController::isValid(void)
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

int DisplayLCDI2CController::start(DisplayDeviceConfig& deviceConfig)
{
  do
  {
    if (this->isValid() != false)
    {
      return 0; //already
    }

    this->lcd_Controller = new LiquidCrystal_I2C(
      deviceConfig.spec.lcdI2C.i2c_Add
      , deviceConfig.columnNo
      , deviceConfig.rowNo
    );
    this->lcd_Controller->init();
    this->backlight_Pin = deviceConfig.spec.lcdI2C.pin_A_Backlight;
    this->brighness_Pin = deviceConfig.spec.lcdI2C.pin_V0_Brightness;
    this->backlight_Level = deviceConfig.spec.lcdI2C.initBacklight;
    this->brightness_Level = deviceConfig.spec.lcdI2C.initBrightness;

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

void DisplayLCDI2CController::stop(void)
{
  if (this->lcd_Controller != NULL )
  {
    delete this->lcd_Controller;
  }
  this->lcd_Controller = NULL;
}

// controlling functions
void DisplayLCDI2CController::clear()
{
  this->lcd_Controller->clear();
}

void DisplayLCDI2CController::home()
{
  this->lcd_Controller->home();
}

void DisplayLCDI2CController::noDisplay()
{
  this->lcd_Controller->noDisplay();
}

void DisplayLCDI2CController::display()
{
  this->lcd_Controller->display();
}

void DisplayLCDI2CController::noBlink()
{
  this->lcd_Controller->noBlink();
}

void DisplayLCDI2CController::blink()
{
  this->lcd_Controller->blink();
}

void DisplayLCDI2CController::noCursor()
{
  this->lcd_Controller->noCursor();
}

void DisplayLCDI2CController::cursor()
{
  this->lcd_Controller->cursor();
}

void DisplayLCDI2CController::scrollDisplayLeft()
{
  this->lcd_Controller->scrollDisplayLeft();
}

void DisplayLCDI2CController::scrollDisplayRight()
{
  this->lcd_Controller->scrollDisplayRight();
}

void DisplayLCDI2CController::leftToRight()
{
  this->lcd_Controller->leftToRight();
}

void DisplayLCDI2CController::rightToLeft()
{
  this->lcd_Controller->rightToLeft();
}

void DisplayLCDI2CController::autoscroll()
{
  this->lcd_Controller->autoscroll();
}

void DisplayLCDI2CController::noAutoscroll()
{
  this->lcd_Controller->noAutoscroll();
}

void DisplayLCDI2CController::setRowOffsets(int row1, int row2, int row3, int row4)
{
  // do nothing
}

void DisplayLCDI2CController::createChar(uint8_t charId, uint8_t charArray[])
{
  this->lcd_Controller->createChar(charId, charArray);
}

void DisplayLCDI2CController::setCursor(uint8_t col, uint8_t row)
{
  this->lcd_Controller->setCursor(col, row);
}

void DisplayLCDI2CController::setBacklight(byte level)
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

void DisplayLCDI2CController::setBrightness(byte level)
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


size_t DisplayLCDI2CController::print(const __FlashStringHelper* param)
{
  return this->lcd_Controller->print(param);
}

size_t DisplayLCDI2CController::print(const String& param)
{
  return this->lcd_Controller->print(param);
}

size_t DisplayLCDI2CController::print(const char param[])
{
  return this->lcd_Controller->print(param);
}

size_t DisplayLCDI2CController::print(char  param)
{
  return this->lcd_Controller->print(param);
}

size_t DisplayLCDI2CController::print(unsigned char  param1, int param2)
{
  return this->lcd_Controller->print(param1, param2);
}

size_t DisplayLCDI2CController::print(int param1, int param2)
{
  return this->lcd_Controller->print(param1, param2);
}

size_t DisplayLCDI2CController::print(unsigned int param1, int param2)
{
  return this->lcd_Controller->print(param1, param2);
}

size_t DisplayLCDI2CController::print(long param1, int param2)
{
  return this->lcd_Controller->print(param1, param2);
}

size_t DisplayLCDI2CController::print(unsigned long param1, int param2)
{
  return this->lcd_Controller->print(param1, param2);
}

size_t DisplayLCDI2CController::print(double param1, int param2)
{
  return this->lcd_Controller->print(param1, param2);
}

size_t DisplayLCDI2CController::print(const Printable& param1)
{
  return this->lcd_Controller->print(param1);
}

size_t DisplayLCDI2CController::println(const __FlashStringHelper* param1)
{
  return this->lcd_Controller->println(param1);
}

size_t DisplayLCDI2CController::println(const String& s)
{
  return this->lcd_Controller->println(s);
}

size_t DisplayLCDI2CController::println(char* param)
{
  return this->lcd_Controller->println(param);
}

size_t DisplayLCDI2CController::println(const char param[])
{
  return this->lcd_Controller->println(param);
}

size_t DisplayLCDI2CController::println(char param)
{
  return this->lcd_Controller->println(param);
}

size_t DisplayLCDI2CController::println(unsigned char param1, int param2)
{
  return this->lcd_Controller->println(param1, param2);
}

size_t DisplayLCDI2CController::println(int param1, int param2 )
{
  return  this->lcd_Controller->println(param1, param2);
}

size_t DisplayLCDI2CController::println(unsigned int param1, int param2)
{
  return this->lcd_Controller->println(param1, param2);
}

size_t DisplayLCDI2CController::println(long param1, int param2)
{
  return this->lcd_Controller->println(param1, param2);
}

size_t DisplayLCDI2CController::println(unsigned long param1, int param2)
{
  return this->lcd_Controller->println(param1, param2);
}

size_t DisplayLCDI2CController::println(double param1, int param2)
{
  return this->lcd_Controller->println(param1, param2);
}

size_t DisplayLCDI2CController::println(const Printable& param)
{
  return this->lcd_Controller->println(param);
}

size_t DisplayLCDI2CController::println(void)
{
  return this->lcd_Controller->println();
}

#endif // _CONF_DISPLAY_LCD_I2C_CONTROLLER_ENABLED