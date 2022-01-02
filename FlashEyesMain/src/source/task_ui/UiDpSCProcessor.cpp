/////////////////////////////////////////////////
// INCLUDE
#include "UiDpSCProcessor.h"
#if (_CONF_UI_DP_SC_PROCESSOR_ENABLED)
/////////////////////////////////////////////////
// PREPROCESSOR

/////////////////////////////////////////////////
// DEFINE

#define UI_DP_SC_PROCESSOR_TMP_BUFF_LEN           128
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

/* UiDpSCProcessor*/
UiDpSCProcessor::UiDpSCProcessor(void)
  : UiDpProcessor(UIControlTypeUN::UIControlSerialConsoleDevice)
  , tmp_Buff(NULL)
{

}

UiDpSCProcessor::~UiDpSCProcessor(void)
{
  this->stop();
}

int UiDpSCProcessor::start(UiDpProcessorConfigTAG& deviceConfig)
{
  do
  {
    if (this->tmp_Buff == NULL)
    {
      this->tmp_Buff = new char[UI_DP_SC_PROCESSOR_TMP_BUFF_LEN];
    }

    deviceConfig.deviceConfig.deviceType = DisplayDeviceTypeUN::DisplaySerialConsoleDevice;
    return UiDpProcessor::start(deviceConfig);
  } while (0);
  return -1;
  
}


void UiDpSCProcessor::uiMessRaw(UiMessRawTAG* param)
{
  SYSTEM_PRINT_BUF(this->tmp_Buff, UI_DP_SC_PROCESSOR_TMP_BUFF_LEN, "[UI] raw:%s", param->data);
  this->dp_Controller->println(this->tmp_Buff);
}

void UiDpSCProcessor::uiMessMessage(UiMessMessageTAG* param)
{
  SYSTEM_PRINT_BUF(this->tmp_Buff, UI_DP_SC_PROCESSOR_TMP_BUFF_LEN, "[UI] mess:%s", param->mess);
  this->dp_Controller->println(this->tmp_Buff);
}

void UiDpSCProcessor::uiMessSysState(UiMessSysStateTAG* param)
{
  SYSTEM_PRINT_BUF(this->tmp_Buff, UI_DP_SC_PROCESSOR_TMP_BUFF_LEN, "[UI] state:%u", param->state);
  this->dp_Controller->println(this->tmp_Buff);
}

void UiDpSCProcessor::stop(void)
{
  UiDpProcessor::stop();
  if (this->tmp_Buff != NULL)
  {
    delete[] this->tmp_Buff;
  }
}


#endif // _CONF_UI_DP_LCD_I2C_PROCESSOR_ENABLED