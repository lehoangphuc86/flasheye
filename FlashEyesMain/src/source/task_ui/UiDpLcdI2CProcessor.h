#include "UiManagerConstant.h"

#if (_CONF_UI_DP_LCD_I2C_PROCESSOR_ENABLED)

#ifndef _UI_DP_LCD_I2C_PROCESSOR_H
#define _UI_DP_LCD_I2C_PROCESSOR_H

/////////////////////////////////////////////////
// INCLUDE
#include "UiDpProcessor.h"
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

/*UiDpLcdI2CProcessor*/
class UiDpLcdI2CProcessor
  : public UiDpProcessor
{
public:
  UiDpLcdI2CProcessor(void);
  virtual ~UiDpLcdI2CProcessor(void);
  int                                                           start(UiDpProcessorConfigTAG& deviceConfig) override;
  void                                                          uiMessRaw(UiMessRawTAG* param) override;
  void                                                          uiMessMessage(UiMessMessageTAG* param) override;
  void                                                          uiMessSysState(UiMessSysStateTAG* param) override;
};

#endif // _UI_DP_LCD_I2C_PROCESSOR_H

#endif // _CONF_UI_DP_LCD_I2C_PROCESSOR_ENABLED