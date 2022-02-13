/////////////////////////////////////////////////
// INCLUDE
#include "DisplayJMDController.h"
#if (_CONF_DISPLAY_SC_CONTROLLER_ENABLED)
#include "../os_system/SystemCommon.h"
/////////////////////////////////////////////////
// PREPROCESSOR

//#define DP_JMD_CONTROLLER_CONSOLE_DEBUG_ENABLE
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
#ifdef DP_JMD_CONTROLLER_CONSOLE_DEBUG_ENABLE
char dpJMDControllerLogBuf[SYSTEM_CONSOLE_OUT_BUF_LEN];
#endif // DP_JMD_CONTROLLER_CONSOLE_DEBUG_ENABLE
/////////////////////////////////////////////////
// STATIC FUNCTIONS

/////////////////////////////////////////////////
// EXTERN

/////////////////////////////////////////////////
// CLASS IMPLEMENTAION

/* DisplayJMDController*/
DisplayJMDController::DisplayJMDController(void)
  : DisplayController(DisplayDeviceTypeUN::DisplayJMDDevice)
  , lcd_Controller(NULL)
{

}
DisplayJMDController::~DisplayJMDController(void)
{

}

bool DisplayJMDController::isValid(void)
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

int DisplayJMDController::start(DisplayDeviceConfig& deviceConfig)
{
  int ret = 0;
  do
  {
    if (this->isValid() != false)
    {
      return 0; //already
    }

    ret = this->createSocket(deviceConfig.spec.jmd);
#ifdef DP_JMD_CONTROLLER_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(dpJMDControllerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[jmdC] sta %d %d", 0, ret);
#endif // DP_JMD_CONTROLLER_CONSOLE_DEBUG_ENABLE
    if (ret != 0)
    {
      break;
    }

    ret = this->setSocketConfig(deviceConfig.spec.jmd);
#ifdef DP_JMD_CONTROLLER_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(dpJMDControllerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[jmdC] sta %d %d", 1, ret);
#endif // DP_JMD_CONTROLLER_CONSOLE_DEBUG_ENABLE
    if (ret != 0)
    {
      break;
    }

#ifdef DP_JMD_CONTROLLER_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(dpJMDControllerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[jmdC] sta %d", 99);
#endif //DP_JMD_CONTROLLER_CONSOLE_DEBUG_ENABLE

    return 0;
  } while (0);
  this->stop();
#ifdef DP_JMD_CONTROLLER_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(dpJMDControllerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[jmdC] sta %d", -99);
#endif // DP_JMD_CONTROLLER_CONSOLE_DEBUG_ENABLE
  return -1;
}

int DisplayJMDController::createSocket(DisplayJMDDeviceConfigTAG& socketConfig)
{
  this->lcd_Controller = NULL;
  do
  {
    //this->lcd_Controller = new U8G2_SSD1306_128X64_NONAME_1_HW_I2C(U8G2_R0, U8X8_PIN_NONE, socketConfig.pin_SCL, socketConfig.pin_SDA);
    /*
    * Currently, there is a bug in ESP32 and U8g2lib, it could not work in specified pins even it is 21 and 22
    */
    this->lcd_Controller = new U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C(U8G2_R0, U8X8_PIN_NONE, U8X8_PIN_NONE, U8X8_PIN_NONE);
    if (this->lcd_Controller == NULL)
    {
#ifdef DP_JMD_CONTROLLER_CONSOLE_DEBUG_ENABLE
      CONSOLE_LOG_BUF(dpJMDControllerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[jmdC] creSoc %d", -10);
#endif // DP_JMD_CONTROLLER_CONSOLE_DEBUG_ENABLE
      break;
    }
#ifdef DP_JMD_CONTROLLER_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(dpJMDControllerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[jmdC] creSoc %d", 99);
#endif // DP_JMD_CONTROLLER_CONSOLE_DEBUG_ENABLE
    return 0;
  } while (0);

  this->stop();
#ifdef DP_JMD_CONTROLLER_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(dpJMDControllerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[jmdC] creSoc %d", -99);
#endif // DP_JMD_CONTROLLER_CONSOLE_DEBUG_ENABLE
  return -1;
}

int DisplayJMDController::setSocketConfig(DisplayJMDDeviceConfigTAG& socketConfig)
{
  bool error = false;
  do
  {
    if (this->isValid() == false)
    {
#ifdef DP_JMD_CONTROLLER_CONSOLE_DEBUG_ENABLE
      CONSOLE_LOG_BUF(dpJMDControllerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[jmdC] setSoc %d", -10);
#endif // DP_JMD_CONTROLLER_CONSOLE_DEBUG_ENABLE
      break;
    }
    this->lcd_Controller->begin();
    return 0;
  } while (0);

  return -1;
}

void DisplayJMDController::stop(void)
{
  if (this->lcd_Controller != NULL)
  {
    //this->lcd_Controller->end();
    delete this->lcd_Controller;
  }

  this->lcd_Controller = NULL;
}

// controlling functions
void DisplayJMDController::clear()
{
  return this->lcd_Controller->clear();
}

void DisplayJMDController::home()
{
  return this->lcd_Controller->home();
}

void DisplayJMDController::noDisplay()
{
  return this->lcd_Controller->noDisplay();
}

void DisplayJMDController::display()
{
  return this->lcd_Controller->display();
}

void DisplayJMDController::noBlink()
{
  // not supported
}

void DisplayJMDController::blink()
{
  // not supported
}

void DisplayJMDController::noCursor()
{
  // not supported
}

void DisplayJMDController::cursor()
{
  // not supported
}

void DisplayJMDController::scrollDisplayLeft()
{
  // not supported
}

void DisplayJMDController::scrollDisplayRight()
{
  // not supported
}

void DisplayJMDController::leftToRight()
{
  // not supported
}

void DisplayJMDController::rightToLeft()
{
  // not supported
}

void DisplayJMDController::autoscroll()
{
  // not supported
}

void DisplayJMDController::noAutoscroll()
{
  // not supported
}

void DisplayJMDController::setRowOffsets(int row1, int row2, int row3, int row4)
{
  // not supported
}

void DisplayJMDController::createChar(uint8_t charId, uint8_t charArray[])
{
  // not supported
}

void DisplayJMDController::setCursor(uint8_t col, uint8_t row)
{
  return this->lcd_Controller->setCursor(col, row);
}

void DisplayJMDController::setBacklight(byte level)
{
  // not supported
}

void DisplayJMDController::setBrightness(byte level)
{
  // not supported
}


size_t DisplayJMDController::print(const __FlashStringHelper* param)
{
  return this->lcd_Controller->print(param);
}

size_t DisplayJMDController::print(const String& param)
{
  return this->lcd_Controller->print(param);
}

size_t DisplayJMDController::print(const char param[])
{
  return this->lcd_Controller->print(param);
}

size_t DisplayJMDController::print(char  param)
{
  return this->lcd_Controller->print(param);
}

size_t DisplayJMDController::print(unsigned char  param1, int param2)
{
  return this->lcd_Controller->print(param1, param2);
}

size_t DisplayJMDController::print(int param1, int param2)
{
  return this->lcd_Controller->print(param1, param2);
}

size_t DisplayJMDController::print(unsigned int param1, int param2)
{
  return this->lcd_Controller->print(param1, param2);
}

size_t DisplayJMDController::print(long param1, int param2)
{
  return this->lcd_Controller->print(param1, param2);
}

size_t DisplayJMDController::print(unsigned long param1, int param2)
{
  return this->lcd_Controller->print(param1, param2);
}

size_t DisplayJMDController::print(double param1, int param2)
{
  return this->lcd_Controller->print(param1, param2);
}

size_t DisplayJMDController::print(const Printable& param1)
{
  return this->lcd_Controller->print(param1);
}

size_t DisplayJMDController::println(const __FlashStringHelper* param1)
{
  return this->lcd_Controller->println(param1);
}

size_t DisplayJMDController::println(const String& s)
{
  return this->lcd_Controller->println(s);
}

size_t DisplayJMDController::println(char* param)
{
  return this->lcd_Controller->println(param);
}

size_t DisplayJMDController::println(const char param[])
{
  return this->lcd_Controller->print(param);
}

size_t DisplayJMDController::println(char param)
{
  return this->lcd_Controller->println(param);
}

size_t DisplayJMDController::println(unsigned char param1, int param2)
{
  return this->lcd_Controller->println(param1, param2);
}

size_t DisplayJMDController::println(int param1, int param2 )
{
  return  this->lcd_Controller->println(param1, param2);
}

size_t DisplayJMDController::println(unsigned int param1, int param2)
{
  return this->lcd_Controller->println(param1, param2);
}

size_t DisplayJMDController::println(long param1, int param2)
{
  return this->lcd_Controller->println(param1, param2);
}

size_t DisplayJMDController::println(unsigned long param1, int param2)
{
  return this->lcd_Controller->println(param1, param2);
}

size_t DisplayJMDController::println(double param1, int param2)
{
  return this->lcd_Controller->println(param1, param2);
}

size_t DisplayJMDController::println(const Printable& param)
{
  return this->lcd_Controller->println(param);
}

size_t DisplayJMDController::println(void)
{
  return this->lcd_Controller->println();
}


uint8_t DisplayJMDController::getMaxCharWidth(void)
{
  return this->lcd_Controller->getMaxCharWidth();
}

uint8_t DisplayJMDController::getMaxCharHeight(void)
{
  return this->lcd_Controller->getMaxCharHeight();
}

void DisplayJMDController::drawPixel(uint8_t x, uint8_t y)
{
  return this->lcd_Controller->drawPixel(x, y);
}

void DisplayJMDController::drawHLine(uint8_t x, uint8_t y, uint8_t w)
{
  return this->lcd_Controller->drawHLine(x, y, w);
}

void DisplayJMDController::drawVLine(uint8_t x, uint8_t y, uint8_t h)
{
  return this->lcd_Controller->drawVLine(x, y, h);
}

void DisplayJMDController::drawHVLine(uint8_t x, uint8_t y, uint8_t len, uint8_t dir)
{
  return this->lcd_Controller->drawHVLine(x, y, len, dir);
}

void DisplayJMDController::drawFrame(uint8_t x, uint8_t y, uint8_t w, uint8_t h)
{
  return this->lcd_Controller->drawFrame(x, y, w, h);
}

void DisplayJMDController::drawRFrame(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t r)
{
  return this->lcd_Controller->drawRFrame(x, y, w, h, r);
}

void DisplayJMDController::drawBox(uint8_t x, uint8_t y, uint8_t w, uint8_t h)
{
  return this->lcd_Controller->drawBox(x, y, w, h);
}

void DisplayJMDController::drawRBox(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t r)
{
  return this->lcd_Controller->drawRBox(x, y, w, h, r);
}

void DisplayJMDController::drawCircle(uint8_t x0, uint8_t y0, uint8_t rad, uint8_t opt)
{
  return this->lcd_Controller->drawCircle(x0, y0, rad, opt);
}

void DisplayJMDController::drawDisc(uint8_t x0, uint8_t y0, uint8_t rad, uint8_t opt)
{
  return this->lcd_Controller->drawDisc(x0, y0, rad, opt);
}

void DisplayJMDController::drawEllipse(uint8_t x0, uint8_t y0, uint8_t rx, uint8_t ry, uint8_t opt)
{
  return this->lcd_Controller->drawEllipse(x0, y0, rx, ry, opt);
}

void DisplayJMDController::drawFilledEllipse(uint8_t x0, uint8_t y0, uint8_t rx, uint8_t ry, uint8_t opt)
{
  return this->lcd_Controller->drawFilledEllipse(x0, y0, rx, ry, opt);
}

void DisplayJMDController::drawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2)
{
  return this->lcd_Controller->drawLine(x1, y1, x2, y2);
}

void DisplayJMDController::drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2)
{
  return this->lcd_Controller->drawTriangle(x0, y0, x1, y1, x2, y2);
}

void DisplayJMDController::setFont(uint8_t fontIndx)
{
  switch (fontIndx)
  {
  case DISPLAY_FONT_IDX_PCSENIOR_8F:
    return this->lcd_Controller->setFont(u8g2_font_pcsenior_8f);
  case DISPLAY_FONT_IDX_6x13_MF:
  default:
    return this->lcd_Controller->setFont(u8g2_font_6x13_mf);
  }
}

void DisplayJMDController::setFontMode(uint8_t  is_transparent)
{
  return this->lcd_Controller->setFontMode(is_transparent);
}

void DisplayJMDController::setFontDirection(uint8_t dir)
{
  return this->lcd_Controller->setFontDirection(dir);
}

void DisplayJMDController::setDrawColor(uint8_t color)
{
  return this->lcd_Controller->setDrawColor(color);
}

uint8_t DisplayJMDController::drawGlyph(uint8_t x, uint8_t y, uint16_t encoding)
{
  return this->lcd_Controller->drawGlyph(x, y, encoding);
}

uint8_t DisplayJMDController::drawStr(uint8_t x, uint8_t y, const char* s)
{
  return this->lcd_Controller->drawStr(x, y, s);
}

void DisplayJMDController::sendBuffer(void)
{
  return this->lcd_Controller->sendBuffer();
}

void DisplayJMDController::clearBuffer(void)
{
  return this->lcd_Controller->clearBuffer();
}
#endif // _CONF_DISPLAY_SC_CONTROLLER_ENABLED