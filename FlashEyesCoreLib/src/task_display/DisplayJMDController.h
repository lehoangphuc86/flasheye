#include "DisplayController.h"

#if (_CONF_DISPLAY_JMD_CONTROLLER_ENABLED)

#ifndef _DISPLAY_JMD_CONTROLLER_H
#define _DISPLAY_JMD_CONTROLLER_H

/////////////////////////////////////////////////
// INCLUDE
#include "../libs/lcd/U8g2/U8g2lib.h"
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

/*DisplayJMDController*/

class DisplayJMDController
  : public DisplayController
{
public:
  DisplayJMDController(void);
  virtual ~DisplayJMDController(void);
  bool                                                          isValid(void) override;
  int                                                           start(DisplayDeviceConfig& deviceConfig) override;
  
  virtual void                                                  stop(void) override;
  // controlling functions
  void                                                          clear() override;
  void                                                          home() override;

  void                                                          noDisplay() override;
  void                                                          display() override;
  void                                                          noBlink() override;
  void                                                          blink() override;
  void                                                          noCursor() override;
  void                                                          cursor() override;
  void                                                          scrollDisplayLeft() override;
  void                                                          scrollDisplayRight() override;
  void                                                          leftToRight() override;
  void                                                          rightToLeft() override;
  void                                                          autoscroll() override;
  void                                                          noAutoscroll() override;
  void                                                          setRowOffsets(int row1, int row2, int row3, int row4) override;
  void                                                          createChar(uint8_t charId, uint8_t charArray[]) override;
  void                                                          setCursor(uint8_t col, uint8_t row) override;
  void                                                          setBacklight(byte level) override;
  void                                                          setBrightness(byte level) override;

  // print
  size_t                                                        print(const __FlashStringHelper* param) override;
  size_t                                                        print(const String& param) override;
  size_t                                                        print(const char param[]) override;
  size_t                                                        print(char  param) override;
  size_t                                                        print(unsigned char  param1, int param2= DEC) override;
  size_t                                                        print(int param1, int param2= DEC) override;
  size_t                                                        print(unsigned int param1, int param2 = DEC) override;
  size_t                                                        print(long param1, int param2= DEC) override;
  size_t                                                        print(unsigned long param1, int param2 = DEC) override;
  size_t                                                        print(double param1, int param2 = 2) override;
  size_t                                                        print(const Printable& param1) override;

  size_t                                                        println(const __FlashStringHelper* param1) override;
  size_t                                                        println(const String& s) override;
  size_t                                                        println(const char param[]) override;
  size_t                                                        println(char* param) override;
  size_t                                                        println(char param) override;
  size_t                                                        println(unsigned char param1, int param2 = DEC) override;
  size_t                                                        println(int param1, int param2 = DEC) override;
  size_t                                                        println(unsigned int param1, int param2 = DEC) override;
  size_t                                                        println(long param1, int param2 = DEC) override;
  size_t                                                        println(unsigned long param1, int param2 = DEC) override;
  size_t                                                        println(double param1, int param2 = 2) override;
  size_t                                                        println(const Printable& param) override;
  size_t                                                        println(void) override;

  uint8_t                                                       getMaxCharWidth(void) override;
  uint8_t                                                       getMaxCharHeight(void) override;
  void                                                          drawPixel(uint8_t x, uint8_t y) override;
  void                                                          drawHLine(uint8_t x, uint8_t y, uint8_t w) override;
  void                                                          drawVLine(uint8_t x, uint8_t y, uint8_t h) override;
  void                                                          drawHVLine(uint8_t x, uint8_t y, uint8_t len, uint8_t dir) override;
  void                                                          drawFrame(uint8_t x, uint8_t y, uint8_t w, uint8_t h) override;
  void                                                          drawRFrame(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t r) override;
  void                                                          drawBox(uint8_t x, uint8_t y, uint8_t w, uint8_t h) override;
  void                                                          drawRBox(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t r) override;
  void                                                          drawCircle(uint8_t x0, uint8_t y0, uint8_t rad, uint8_t opt = DISPLAY_DEVICE_DRAW_ALL) override;
  void                                                          drawDisc(uint8_t x0, uint8_t y0, uint8_t rad, uint8_t opt = DISPLAY_DEVICE_DRAW_ALL) override;
  void                                                          drawEllipse(uint8_t x0, uint8_t y0, uint8_t rx, uint8_t ry, uint8_t opt = DISPLAY_DEVICE_DRAW_ALL) override;
  void                                                          drawFilledEllipse(uint8_t x0, uint8_t y0, uint8_t rx, uint8_t ry, uint8_t opt = DISPLAY_DEVICE_DRAW_ALL) override;
  void                                                          drawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2) override;
  void                                                          drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2) override;
  void                                                          setFont(uint8_t fontIndx) override;
  void                                                          setFontMode(uint8_t  is_transparent) override;
  void                                                          setFontDirection(uint8_t dir) override;
  void                                                          setDrawColor(uint8_t color) override;
  uint8_t                                                       drawGlyph(uint8_t x, uint8_t y, uint16_t encoding) override;
  uint8_t                                                       drawStr(uint8_t x, uint8_t y, const char* s) override;
  void                                                          sendBuffer(void) override;
  void                                                          clearBuffer(void) override;
protected:
  int                                                           createSocket(DisplayJMDDeviceConfigTAG& socketConfig);
  int                                                           setSocketConfig(DisplayJMDDeviceConfigTAG& socketConfig);
protected:
  U8G2*                                                         lcd_Controller;
};

#endif // _DISPLAY_JMD_CONTROLLER_H

#endif // _CONF_DISPLAY_JMD_CONTROLLER_ENABLED