/////////////////////////////////////////////////
// INCLUDE
#include "UiDpSCProcessor.h"
#if (_CONF_UI_DP_SC_PROCESSOR_ENABLED)
/////////////////////////////////////////////////
// PREPROCESSOR
//#define UI_DP_SC_PROCESSOR_CONSOLE_DEBUG_ENABLE
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
#ifdef UI_DP_SC_PROCESSOR_CONSOLE_DEBUG_ENABLE
char uiDpSCProcessorLogBuf[SYSTEM_CONSOLE_OUT_BUF_LEN];
#endif // UI_DP_SC_PROCESSOR_CONSOLE_DEBUG_ENABLE
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
  int ret = 0;
  do
  {
    deviceConfig.deviceConfig.deviceType = DisplayDeviceTypeUN::DisplaySerialConsoleDevice;
    ret =  UiDpProcessor::start(deviceConfig);

    if (this->tmp_Buff == NULL)
    {
      this->tmp_Buff = new char[UI_DP_SC_PROCESSOR_TMP_BUFF_LEN];
    }

#ifdef UI_DP_SC_PROCESSOR_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(uiDpSCProcessorLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[UI] st %i %i", 0, ret);
    this->dp_Controller->println("abc");
    CONSOLE_LOG_BUF(uiDpSCProcessorLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[UI] st %i", 99);
#endif // UI_DP_SC_PROCESSOR_CONSOLE_DEBUG_ENABLE
    if (ret != 0)
    {
      break;
    }
    return 0;
  } while (0);
  return -1;
}


int UiDpSCProcessor::uiMessRaw(UiMessRawTAG* param)
{
  SYSTEM_PRINT_BUF(this->tmp_Buff, UI_DP_SC_PROCESSOR_TMP_BUFF_LEN, "[UI] raw:%s", param->data);
  this->dp_Controller->println(this->tmp_Buff);
  return 0;
}

int UiDpSCProcessor::uiMessMessage(UiMessMessageTAG* param)
{
  SYSTEM_PRINT_BUF(this->tmp_Buff, UI_DP_SC_PROCESSOR_TMP_BUFF_LEN, "[UI] mess:%s", param->mess);
  this->dp_Controller->println(this->tmp_Buff);
  return 0;
}

int UiDpSCProcessor::uiMessSysState(UiMessSysStateTAG* param)
{
  SYSTEM_PRINT_BUF(this->tmp_Buff, UI_DP_SC_PROCESSOR_TMP_BUFF_LEN, "[UI] state:%u sub:%u", param->stateId, param->stateSubId);
  this->dp_Controller->println(this->tmp_Buff);
  return 0;
}

int UiDpSCProcessor::uiMessNetState(UiMessNetStateTAG* param)
{
  SYSTEM_PRINT_BUF(this->tmp_Buff, UI_DP_SC_PROCESSOR_TMP_BUFF_LEN, "[UI] nets:%u sub:%u", param->stateId, param->stateSubId);
  SYSTEM_PRINT_BUF(this->tmp_Buff, UI_DP_SC_PROCESSOR_TMP_BUFF_LEN, "[UI] ip:%s", param->ip4);
  this->dp_Controller->println(this->tmp_Buff);
  return 0;
}

void UiDpSCProcessor::stop(void)
{
  UiDpProcessor::stop();
  if (this->tmp_Buff != NULL)
  {
    delete[] this->tmp_Buff;
  }
  this->tmp_Buff = NULL;
}


#endif // _CONF_UI_DP_LCD_I2C_PROCESSOR_ENABLED