/////////////////////////////////////////////////
// INCLUDE
#include "DisplayDeviceController.h"
#if (_CONF_DISPLAY_DEVICE_CONTROLLER_ENABLED)
#include "../os_system/SystemCommon.h"
#include "Arduino.h"
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

/* DisplayDeviceController*/
DisplayDeviceController::DisplayDeviceController(void)
  : lcd_Controller(NULL)
{

}
DisplayDeviceController::~DisplayDeviceController(void)
{

}

int DisplayDeviceController::setConfig(DisplayDeviceConfig& deviceConfig)
{
  do
  {
    if (this->isTaskValid() != false)
    {
      break;
    }

    if (this->device_Info.setConfig(deviceConfig) != 0)
    {
      break;
    }
    
    return 0;
  } while (0);
  return -1;
}

int DisplayDeviceController::prepare(void)
{
  do
  {
    if (this->isTaskValid() != false)
    {
      break;
    }
    return this->device_Info.prepare();
  } while (0);
  return -1;
  
}

int DisplayDeviceController::start(void)
{
  do
  {
    if (this->isTaskValid() != false)
    {
      break;
    }

    DisplayDeviceConfig* deviceConfig = this->device_Info.deviceConfig();
#ifdef SYSTEM_AVR_PLATFORM
    this->lcd_Controller = new LcdController(
      deviceConfig->pin_RS
      , deviceConfig->pin_RW
      , deviceConfig->pin_E
      , deviceConfig->pin_D4
      , deviceConfig->pin_D5
      , deviceConfig->pin_D6
      , deviceConfig->pin_D7
    );
    this->lcd_Controller->begin(deviceConfig->columnNo, deviceConfig->rowNo);
#else // 
    this->lcd_Controller = new LcdController(
      deviceConfig->i2c_Add
      , deviceConfig->columnNo
      , deviceConfig->rowNo
    );
    this->lcd_Controller->init();
#endif // SYSTEM_AVR_PLATFORM
    
    if (this->device_Info.isBacklightControllable() != false)
    {
      PIN_MODE(this->device_Info.backlightPin(), OUTPUT);
      this->setBacklight(this->device_Info.backlightLevel());
    }

    if (this->device_Info.isBrightnessControllable() != false)
    {
      PIN_MODE(this->device_Info.brightnessPin(), OUTPUT);
      this->setBrightness(this->device_Info.brightnessLevel());
    }

    return 0;
  } while (0);
  return -1;
}

bool DisplayDeviceController::isTaskValid(void)
{
  return  (this->lcd_Controller == NULL ? false : true);
}

// controlling functions
void DisplayDeviceController::clear()
{
  this->lcd_Controller->clear();
}

void DisplayDeviceController::home()
{
  this->lcd_Controller->home();
}

void DisplayDeviceController::noDisplay()
{
  this->lcd_Controller->noDisplay();
}

void DisplayDeviceController::display()
{
  this->lcd_Controller->display();
}

void DisplayDeviceController::noBlink()
{
  this->lcd_Controller->noBlink();
}

void DisplayDeviceController::blink()
{
  this->lcd_Controller->blink();
}

void DisplayDeviceController::noCursor()
{
  this->lcd_Controller->noCursor();
}

void DisplayDeviceController::cursor()
{
  this->lcd_Controller->cursor();
}

void DisplayDeviceController::scrollDisplayLeft()
{
  this->lcd_Controller->scrollDisplayLeft();
}

void DisplayDeviceController::scrollDisplayRight()
{
  this->lcd_Controller->scrollDisplayRight();
}

void DisplayDeviceController::leftToRight()
{
  this->lcd_Controller->leftToRight();
}

void DisplayDeviceController::rightToLeft()
{
  this->lcd_Controller->rightToLeft();
}

void DisplayDeviceController::autoscroll()
{
  this->lcd_Controller->autoscroll();
}

void DisplayDeviceController::noAutoscroll()
{
  this->lcd_Controller->noAutoscroll();
}


void DisplayDeviceController::setRowOffsets(int row1, int row2, int row3, int row4)
{
#ifdef SYSTEM_AVR_PLATFORM
  this->lcd_Controller->setRowOffsets(row1, row2, row3, row4);
#else
  // nothing
#endif // SYSTEM_AVR_PLATFORM
}

void DisplayDeviceController::createChar(uint8_t charId, uint8_t charArray[])
{
  this->lcd_Controller->createChar(charId, charArray);
}

void DisplayDeviceController::setCursor(uint8_t col, uint8_t row)
{
  this->lcd_Controller->setCursor(col, row);
}

void DisplayDeviceController::setBacklight(byte level)
{
  do
  {
    if (this->device_Info.isBacklightControllable() == false)
    {
      break;
    }

    /*if (this->device_Info.backlightLevel() == level)
    {
      return; 
    }*/
    ANALOG_WRITE(this->device_Info.backlightPin(), level);
    this->device_Info.backlightLevel(level);
    return;
  } while (0);

  return;
}

void DisplayDeviceController::setBrightness(byte level)
{
  do
  {
    if (this->device_Info.isBrightnessControllable() == false)
    {
      break;
    }

    /*if (this->device_Info.brightnessLevel() == level)
    {
      return;
    }*/
    ANALOG_WRITE(this->device_Info.brightnessPin(), level);
    this->device_Info.brightnessLevel(level);
    return;
  } while (0);

  return;
}


size_t DisplayDeviceController::print(const __FlashStringHelper* param)
{
  this->lcd_Controller->print(param);
}

size_t DisplayDeviceController::print(const String& param)
{
  this->lcd_Controller->print(param);
}

size_t DisplayDeviceController::print(const char param[])
{
  this->lcd_Controller->print(param);
}

size_t DisplayDeviceController::print(char  param)
{
  this->lcd_Controller->print(param);
}

size_t DisplayDeviceController::print(unsigned char  param1, int param2)
{
  this->lcd_Controller->print(param1, param2);
}

size_t DisplayDeviceController::print(int param1, int param2)
{
  this->lcd_Controller->print(param1, param2);
}

size_t DisplayDeviceController::print(unsigned int param1, int param2)
{
  this->lcd_Controller->print(param1, param2);
}

size_t DisplayDeviceController::print(long param1, int param2)
{
  this->lcd_Controller->print(param1, param2);
}

size_t DisplayDeviceController::print(unsigned long param1, int param2)
{
  this->lcd_Controller->print(param1, param2);
}

size_t DisplayDeviceController::print(double param1, int param2)
{
  this->lcd_Controller->print(param1, param2);
}

size_t DisplayDeviceController::print(const Printable& param1)
{
  this->lcd_Controller->print(param1);
}

size_t DisplayDeviceController::println(const __FlashStringHelper* param1)
{
  this->lcd_Controller->println(param1);
}

size_t DisplayDeviceController::println(const String& s)
{
  this->lcd_Controller->println(s);
}

size_t DisplayDeviceController::println(const char param[])
{
  this->lcd_Controller->println(param);
}

size_t DisplayDeviceController::println(char param)
{
  this->lcd_Controller->println(param);
}

size_t DisplayDeviceController::println(unsigned char param1, int param2)
{
  this->lcd_Controller->println(param1, param2);
}

size_t DisplayDeviceController::println(int param1, int param2 )
{
  this->lcd_Controller->println(param1, param2);
}

size_t DisplayDeviceController::println(unsigned int param1, int param2)
{
  this->lcd_Controller->println(param1, param2);
}

size_t DisplayDeviceController::println(long param1, int param2)
{
  this->lcd_Controller->println(param1, param2);
}

size_t DisplayDeviceController::println(unsigned long param1, int param2)
{
  this->lcd_Controller->println(param1, param2);
}

size_t DisplayDeviceController::println(double param1, int param2)
{
  this->lcd_Controller->println(param1, param2);
}

size_t DisplayDeviceController::println(const Printable& param)
{
  this->lcd_Controller->println(param);
}

size_t DisplayDeviceController::println(void)
{
  this->lcd_Controller->println();
}

#endif // _CONF_DISPLAY_DEVICE_CONTROLLER_ENABLED