/////////////////////////////////////////////////
// INCLUDE
#include "UiDpLcdProcessor.h"
#if (_CONF_UI_DP_LCD_PROCESSOR_ENABLED)
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

/* UiDpLcdProcessor*/
UiDpLcdProcessor::UiDpLcdProcessor(void)
  : UiDpProcessor(UIControlTypeUN::UIControlLCDDevice)
{

}

UiDpLcdProcessor::~UiDpLcdProcessor(void)
{
  this->stop();
}

int UiDpLcdProcessor::start(UiDpProcessorConfigTAG& deviceConfig)
{
  deviceConfig.deviceConfig.deviceType = DisplayDeviceTypeUN::DisplayLCDDevice;
  return UiDpProcessor::start(deviceConfig);
}


int UiDpLcdProcessor::uiMessRaw(UiMessRawTAG* param)
{
  //@@
  return 0;
}

int UiDpLcdProcessor::uiMessMessage(UiMessMessageTAG* param)
{
  //@@
  return 0;
}

int UiDpLcdProcessor::uiMessSysState(UiMessSysStateTAG* param)
{
  //@@
  return 0;
}

#endif // _CONF_UI_DP_LCD_PROCESSOR_ENABLED