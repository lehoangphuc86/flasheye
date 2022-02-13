#include "DisplayTaskConstant.h"

#if (_CONF_DISPLAY_CONTROLLER_ENABLED)

#ifndef _DISPLAY_CONTROLLER_H
#define _DISPLAY_CONTROLLER_H

/////////////////////////////////////////////////
// INCLUDE

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
// DATA TYPE (TYPEDEF)

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
// CLASS DEFINITION

/*DisplayController*/

class DisplayController
{
public:
  DisplayController(byte deviceType);
  virtual ~DisplayController(void);
  virtual bool                                                  isValid(void);
  byte                                                          deviceType(void);
  virtual int                                                   start(DisplayDeviceConfig& deviceConfig) = 0;
  virtual void                                                  stop(void) = 0;
  // controlling functions
  virtual void                                                  clear() = 0;
  virtual void                                                  home() = 0;

  virtual void                                                  noDisplay() = 0;
  virtual void                                                  display() = 0;
  virtual void                                                  noBlink() = 0;
  virtual void                                                  blink() = 0;
  virtual void                                                  noCursor() = 0;
  virtual void                                                  cursor() = 0;
  virtual void                                                  scrollDisplayLeft() = 0;
  virtual void                                                  scrollDisplayRight() = 0;
  virtual void                                                  leftToRight() = 0;
  virtual void                                                  rightToLeft() = 0;
  virtual void                                                  autoscroll() = 0;
  virtual void                                                  noAutoscroll() = 0;
  virtual void                                                  setRowOffsets(int row1, int row2, int row3, int row4) = 0;
  virtual void                                                  createChar(uint8_t charId, uint8_t charArray[]) = 0;
  virtual void                                                  setCursor(uint8_t col, uint8_t row) = 0;
  virtual void                                                  setBacklight(byte level) = 0;
  virtual void                                                  setBrightness(byte level) = 0;

  // print
  virtual size_t                                                print(const __FlashStringHelper* param) = 0;
  virtual size_t                                                print(const String& param) = 0;
  virtual size_t                                                print(const char param[]) = 0;
  virtual size_t                                                print(char  param) = 0;
  virtual size_t                                                print(unsigned char  param1, int param2= DEC) = 0;
  virtual size_t                                                print(int param1, int param2= DEC) = 0;
  virtual size_t                                                print(unsigned int param1, int param2 = DEC) = 0;
  virtual size_t                                                print(long param1, int param2= DEC) = 0;
  virtual size_t                                                print(unsigned long param1, int param2 = DEC) = 0;
  virtual size_t                                                print(double param1, int param2 = 2) = 0;
  virtual size_t                                                print(const Printable& param1) = 0;

  virtual size_t                                                println(const __FlashStringHelper* param1) = 0;
  virtual size_t                                                println(const String& s) = 0;
  virtual size_t                                                println(const char param[]) = 0;
  virtual size_t                                                println(char* param) = 0;
  virtual size_t                                                println(char param) = 0;
  virtual size_t                                                println(unsigned char param1, int param2 = DEC) = 0;
  virtual size_t                                                println(int param1, int param2 = DEC) = 0;
  virtual size_t                                                println(unsigned int param1, int param2 = DEC) = 0;
  virtual size_t                                                println(long param1, int param2 = DEC) = 0;
  virtual size_t                                                println(unsigned long param1, int param2 = DEC) = 0;
  virtual size_t                                                println(double param1, int param2 = 2) = 0;
  virtual size_t                                                println(const Printable& param) = 0;
  virtual size_t                                                println(void) = 0;

  // support u8g2
  virtual uint8_t                                               getMaxCharWidth(void) { return 1; };
  virtual uint8_t                                               getMaxCharHeight(void) { return 1; };
  virtual void                                                  drawPixel(uint8_t x, uint8_t y) {};
  virtual void                                                  drawHLine(uint8_t x, uint8_t y, uint8_t w) {};
  virtual void                                                  drawVLine(uint8_t x, uint8_t y, uint8_t h) {};
  virtual void                                                  drawHVLine(uint8_t x, uint8_t y, uint8_t len, uint8_t dir) {};
  virtual void                                                  drawFrame(uint8_t x, uint8_t y, uint8_t w, uint8_t h) {};
  virtual void                                                  drawRFrame(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t r) {};
  virtual void                                                  drawBox(uint8_t x, uint8_t y, uint8_t w, uint8_t h) {};
  virtual void                                                  drawRBox(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t r) {};
  virtual void                                                  drawCircle(uint8_t x0, uint8_t y0, uint8_t rad, uint8_t opt = DISPLAY_DEVICE_DRAW_ALL) {};
  virtual void                                                  drawDisc(uint8_t x0, uint8_t y0, uint8_t rad, uint8_t opt = DISPLAY_DEVICE_DRAW_ALL) {};
  virtual void                                                  drawEllipse(uint8_t x0, uint8_t y0, uint8_t rx, uint8_t ry, uint8_t opt = DISPLAY_DEVICE_DRAW_ALL) {};
  virtual void                                                  drawFilledEllipse(uint8_t x0, uint8_t y0, uint8_t rx, uint8_t ry, uint8_t opt = DISPLAY_DEVICE_DRAW_ALL) {};
  virtual void                                                  drawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2) {};
  virtual void                                                  drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2) {};
  virtual void                                                  setFont(uint8_t fontIndx) {};
  virtual void                                                  setFontMode(uint8_t  is_transparent) {};
  virtual void                                                  setFontDirection(uint8_t dir) {};
  virtual void                                                  setDrawColor(uint8_t color) {};
  virtual uint8_t                                               drawGlyph(uint8_t x, uint8_t y, uint16_t encoding) { return 0; };
  virtual uint8_t                                               drawStr(uint8_t x, uint8_t y, const char* s) { return 0; };
  virtual void                                                  sendBuffer(void) {};
  virtual void                                                  clearBuffer(void) {};
protected:
  byte                                                          device_Type;
};

#endif // _DISPLAY_CONTROLLER_H

#endif // _CONF_DISPLAY_CONTROLLER_ENABLED