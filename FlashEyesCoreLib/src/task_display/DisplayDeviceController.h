#include "DisplayTaskCommon.h"

#if (_CONF_DISPLAY_DEVICE_CONTROLLER_ENABLED)

#ifndef _DISPLAY_DEVICE_CONTROLLER_H
#define _DISPLAY_DEVICE_CONTROLLER_H

/////////////////////////////////////////////////
// INCLUDE
#include "DisplayDeviceInfo.h"
#ifdef SYSTEM_AVR_PLATFORM
#include <LiquidCrystal.h>
typedef LiquidCrystal LcdController;
#endif // SYSTEM_AVR_PLATFORM

#ifdef SYSTEM_ESP_PLATFORM
#include <LiquidCrystal_I2C.h>
typedef LiquidCrystal_I2C LcdController;
#endif // SYSTEM_AVR_PLATFORM

#if (!_CONF_DISPLAY_DEVICE_INFO_ENABLED)
#error Display device infor is required
#endif // _CONF_FREE_RTOS_ENABLED
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

/*DisplayDeviceController*/

class DisplayDeviceController
{
public:
  DisplayDeviceController(void);
  virtual ~DisplayDeviceController(void);
  virtual int                                                   setConfig(DisplayDeviceConfig& deviceConfig);
  virtual int                                                   prepare(void);
  virtual int                                                   start(void);
  virtual bool                                                  isTaskValid(void);
  // @@@ delete lcd_Controller
  // controlling functions
  virtual void                                                  clear();
  virtual void                                                  home();

  virtual void                                                  noDisplay();
  virtual void                                                  display();
  virtual void                                                  noBlink();
  virtual void                                                  blink();
  virtual void                                                  noCursor();
  virtual void                                                  cursor();
  virtual void                                                  scrollDisplayLeft();
  virtual void                                                  scrollDisplayRight();
  virtual void                                                  leftToRight();
  virtual void                                                  rightToLeft();
  virtual void                                                  autoscroll();
  virtual void                                                  noAutoscroll();
  virtual void                                                  setRowOffsets(int row1, int row2, int row3, int row4);
  virtual void                                                  createChar(uint8_t charId, uint8_t charArray[]);
  virtual void                                                  setCursor(uint8_t col, uint8_t row);
  virtual void                                                  setBacklight(byte level);
  virtual void                                                  setBrightness(byte level);

  // print
  virtual size_t                                                print(const __FlashStringHelper* param);
  virtual size_t                                                print(const String& param);
  virtual size_t                                                print(const char param[]);
  virtual size_t                                                print(char  param);
  virtual size_t                                                print(unsigned char  param1, int param2= DEC);
  virtual size_t                                                print(int param1, int param2= DEC);
  virtual size_t                                                print(unsigned int param1, int param2 = DEC);
  virtual size_t                                                print(long param1, int param2= DEC);
  virtual size_t                                                print(unsigned long param1, int param2 = DEC);
  virtual size_t                                                print(double param1, int param2 = 2);
  virtual size_t                                                print(const Printable& param1);

  virtual size_t                                                println(const __FlashStringHelper* param1);
  virtual size_t                                                println(const String& s);
  virtual size_t                                                println(const char param[]);
  virtual size_t                                                println(char param);
  virtual size_t                                                println(unsigned char param1, int param2 = DEC);
  virtual size_t                                                println(int param1, int param2 = DEC);
  virtual size_t                                                println(unsigned int param1, int param2 = DEC);
  virtual size_t                                                println(long param1, int param2 = DEC);
  virtual size_t                                                println(unsigned long param1, int param2 = DEC);
  virtual size_t                                                println(double param1, int param2 = 2);
  virtual size_t                                                println(const Printable& param);
  virtual size_t                                                println(void);
protected:
  DisplayDeviceInfo                                             device_Info;
  LcdController*                                                lcd_Controller; // for lcd device only
};

#endif // _DISPLAY_DEVICE_CONTROLLER_H

#endif // _CONF_DISPLAY_DEVICE_CONTROLLER_ENABLED