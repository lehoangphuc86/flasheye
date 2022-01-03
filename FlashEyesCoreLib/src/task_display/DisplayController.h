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
protected:
  byte                                                          device_Type;
};

#endif // _DISPLAY_CONTROLLER_H

#endif // _CONF_DISPLAY_CONTROLLER_ENABLED