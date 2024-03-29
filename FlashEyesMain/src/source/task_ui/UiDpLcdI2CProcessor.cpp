/////////////////////////////////////////////////
// INCLUDE
#include "UiDpLcdI2CProcessor.h"
#if (_CONF_UI_DP_LCD_I2C_PROCESSOR_ENABLED)
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

/* UiDpLcdI2CProcessor*/
UiDpLcdI2CProcessor::UiDpLcdI2CProcessor(void)
  : UiDpProcessor(UIControlTypeUN::UIControlLCDI2CDevice)
{

}

UiDpLcdI2CProcessor::~UiDpLcdI2CProcessor(void)
{
  this->stop();
}

int UiDpLcdI2CProcessor::start(UiDpProcessorConfigTAG& deviceConfig)
{
  deviceConfig.deviceConfig.deviceType = DisplayDeviceTypeUN::DisplayLCDI2CDevice;
  return UiDpProcessor::start(deviceConfig);
}


int UiDpLcdI2CProcessor::uiMessRaw(UiMessRawTAG* param)
{
  //@@
  return 0;
}

int UiDpLcdI2CProcessor::uiMessMessage(UiMessMessageTAG* param)
{
  //@@
  return 0;
}

int UiDpLcdI2CProcessor::uiMessSysState(UiMessSysStateTAG* param)
{
  //@@
  return 0;
}

#endif // _CONF_UI_DP_LCD_I2C_PROCESSOR_ENABLED