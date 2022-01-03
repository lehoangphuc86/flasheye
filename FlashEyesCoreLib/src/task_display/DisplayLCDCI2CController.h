#include "DisplayController.h"

#if (_CONF_DISPLAY_LCD_I2C_CONTROLLER_ENABLED)

#ifndef _DISPLAY_LCD_I2C_CONTROLLER_H
#define _DISPLAY_LCD_I2C_CONTROLLER_H

/////////////////////////////////////////////////
// INCLUDE
#include "../libs/lcd/LiquidCrystal_I2C/LiquidCrystal_I2C.h"

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

/*DisplayLCDI2CController*/

class DisplayLCDI2CController
  : public DisplayController
{
public:
  DisplayLCDI2CController(void);
  virtual ~DisplayLCDI2CController(void);
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
protected:
  byte                                                          backlight_Pin;
  byte                                                          brighness_Pin;
  byte                                                          backlight_Level;
  byte                                                          brightness_Level;
  LiquidCrystal_I2C*                                            lcd_Controller; // for lcd device only
};

#endif // _DISPLAY_LCD_I2C_CONTROLLER_H

#endif // _CONF_DISPLAY_LCD_I2C_CONTROLLER_ENABLED