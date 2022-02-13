/////////////////////////////////////////////////
// INCLUDE
#include "UiDpJMDProcessor.h"
#include "task_display/DisplayController.h"
#include "task_led/LedManager.h"
#include "task_buzzer/BuzzerManager.h"

#if (_CONF_UI_DP_JMD_PROCESSOR_ENABLED)
/////////////////////////////////////////////////
// PREPROCESSOR
//#define UI_DP_JMD_PROCESSOR_CONSOLE_DEBUG_ENABLE
/////////////////////////////////////////////////
// DEFINE

/////////////////////////////////////////////////
// MARCO

/////////////////////////////////////////////////
// GLOBAL VARIABLES
#ifdef UI_DP_JMD_PROCESSOR_CONSOLE_DEBUG_ENABLE
char uiDpJMDProcessorLogBuf[SYSTEM_CONSOLE_OUT_BUF_LEN];
#endif // UI_DP_JMD_PROCESSOR_CONSOLE_DEBUG_ENABLE
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

/* UiDpJMDProcessor*/
UiDpJMDProcessor::UiDpJMDProcessor(void)
  : UiDpProcessor(UIControlTypeUN::UIControlJMDDevice)
  , control_List(NULL)
{

}

UiDpJMDProcessor::~UiDpJMDProcessor(void)
{
  this->stop();
}

int UiDpJMDProcessor::start(UiDpProcessorConfigTAG& deviceConfig)
{
  int ret = 0;
  do
  {
    deviceConfig.deviceConfig.deviceType = DisplayDeviceTypeUN::DisplayJMDDevice;
    ret =  UiDpProcessor::start(deviceConfig);
    if (ret != 0)
    {
      break;
    }

    ret = this->dp_Controller->start(deviceConfig.deviceConfig);
#ifdef UI_DP_JMD_PROCESSOR_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(uiDpJMDProcessorLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[uijmd] sta %d %d", 0, ret);
#endif // UI_DP_JMD_PROCESSOR_CONSOLE_DEBUG_ENABLE
    if (ret != 0)
    {
      break;
    }

    this->dp_Controller->clearBuffer();
    this->dp_Controller->sendBuffer();

    ret = this->addControls(deviceConfig);
    if (ret != 0)
    {
      break;
    }

    return 0;
  } while (0);
  return -1;
}

int UiDpJMDProcessor::addControls(UiDpProcessorConfigTAG& deviceConfig)
{
  int ret = 0;
  do
  {
    if (this->isValid() == false)
    {
      break;
    }
#ifdef UI_DP_JMD_PROCESSOR_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(uiDpJMDProcessorLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[uijmd] add %d", 0);
#endif // UI_DP_JMD_PROCESSOR_CONSOLE_DEBUG_ENABLE
    this->removeControls();
#ifdef UI_DP_JMD_PROCESSOR_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(uiDpJMDProcessorLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[uijmd] add %d", 1);
#endif // UI_DP_JMD_PROCESSOR_CONSOLE_DEBUG_ENABLE
    this->control_List = new UiControlTAG[UI_CTRL_IDX_MAX];
    if (this->control_List == NULL)
    {
      break;
    }
#ifdef UI_DP_JMD_PROCESSOR_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(uiDpJMDProcessorLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[uijmd] add %d", 2);
#endif // UI_DP_JMD_PROCESSOR_CONSOLE_DEBUG_ENABLE
    memset(this->control_List, 0, sizeof(UiControlTAG) * UI_CTRL_IDX_MAX);
#ifdef UI_DP_JMD_PROCESSOR_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(uiDpJMDProcessorLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[uijmd] add %d", 3);
#endif // UI_DP_JMD_PROCESSOR_CONSOLE_DEBUG_ENABLE
    
    const uint8_t xSpace = UI_CTRL_X_SPACE;
    const uint8_t ySpace = UI_CTRL_Y_SPACE;
    
    
    UiControlPropertiesTAG prop = UiControlPropertiesTAG();
    prop.fontWidth = UI_CTRL_FONT_WIDTH_HEADER;
    prop.fontHeight = UI_CTRL_FONT_HEIGHT_HEADER;
    prop.fontIndx = DISPLAY_FONT_IDX_PCSENIOR_8F;
    // UI_CTRL_IDX_LBL_MODE
    prop.pos.set(xSpace, ySpace, 1, 1);
    this->control_List[UI_CTRL_IDX_LBL_MODE].start(this->dp_Controller, prop);
#ifdef UI_DP_JMD_PROCESSOR_CONSOLE_DEBUG_ENABLE
    this->control_List[UI_CTRL_IDX_LBL_MODE].properties.pos.dump();
#endif // UI_DP_JMD_PROCESSOR_CONSOLE_DEBUG_ENABLE

    // UI_CTRL_IDX_LBL_STATE
    prop.pos.set(this->control_List[UI_CTRL_IDX_LBL_MODE].properties.pos.left + this->control_List[UI_CTRL_IDX_LBL_MODE].properties.pos.width + xSpace, ySpace, 1, 1);
    this->control_List[UI_CTRL_IDX_LBL_STATE].start(this->dp_Controller, prop);
#ifdef UI_DP_JMD_PROCESSOR_CONSOLE_DEBUG_ENABLE
    this->control_List[UI_CTRL_IDX_LBL_STATE].properties.pos.dump();
#endif // UI_DP_JMD_PROCESSOR_CONSOLE_DEBUG_ENABLE


    // UI_CTRL_IDX_LBL_WIFI_STATE
    prop.pos.set(this->control_List[UI_CTRL_IDX_LBL_STATE].properties.pos.left + this->control_List[UI_CTRL_IDX_LBL_STATE].properties.pos.width + xSpace, ySpace, 1, 1);
    this->control_List[UI_CTRL_IDX_LBL_WIFI_STATE].start(this->dp_Controller, prop);
#ifdef UI_DP_JMD_PROCESSOR_CONSOLE_DEBUG_ENABLE
    this->control_List[UI_CTRL_IDX_LBL_WIFI_STATE].properties.pos.dump();
#endif // UI_DP_JMD_PROCESSOR_CONSOLE_DEBUG_ENABLE


    // UI_CTRL_IDX_LBL_IP
    prop.pos.set(this->control_List[UI_CTRL_IDX_LBL_WIFI_STATE].properties.pos.left + this->control_List[UI_CTRL_IDX_LBL_WIFI_STATE].properties.pos.width + xSpace, ySpace, 3, 1);
    this->control_List[UI_CTRL_IDX_LBL_IP].start(this->dp_Controller, prop);
#ifdef UI_DP_JMD_PROCESSOR_CONSOLE_DEBUG_ENABLE
    this->control_List[UI_CTRL_IDX_LBL_IP].properties.pos.dump();
#endif // UI_DP_JMD_PROCESSOR_CONSOLE_DEBUG_ENABLE

    // UI_CTRL_IDX_LBL_BATTERY
    prop.pos.set(this->control_List[UI_CTRL_IDX_LBL_IP].properties.pos.left + this->control_List[UI_CTRL_IDX_LBL_IP].properties.pos.width + xSpace, ySpace, 4, 1);
    this->control_List[UI_CTRL_IDX_LBL_BATTERY].start(this->dp_Controller, prop);
#ifdef UI_DP_JMD_PROCESSOR_CONSOLE_DEBUG_ENABLE
    this->control_List[UI_CTRL_IDX_LBL_BATTERY].properties.pos.dump();
#endif // UI_DP_JMD_PROCESSOR_CONSOLE_DEBUG_ENABLE

    // body
    prop.fontWidth = UI_CTRL_FONT_WIDTH_BODY;
    prop.fontHeight = UI_CTRL_FONT_HEIGHT_BODY;
    prop.fontIndx = DISPLAY_FONT_IDX_6x13_MF;
    
    // UI_CTRL_IDX_TXT_BODY
    prop.pos.set(xSpace, this->control_List[UI_CTRL_IDX_LBL_STATE].properties.pos.top + this->control_List[UI_CTRL_IDX_LBL_STATE].properties.pos.heigth + ySpace, 48, 1);
    this->control_List[UI_CTRL_IDX_TXT_BODY].start(this->dp_Controller, prop);
#ifdef UI_DP_JMD_PROCESSOR_CONSOLE_DEBUG_ENABLE
    this->control_List[UI_CTRL_IDX_TXT_BODY].properties.pos.dump();
#endif // UI_DP_JMD_PROCESSOR_CONSOLE_DEBUG_ENABLE
//#ifdef UI_DP_JMD_PROCESSOR_CONSOLE_DEBUG_ENABLE
//    CONSOLE_LOG_BUF(uiDpJMDProcessorLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[uijmd] add %d %d %d %d %d"
//      , 97
//      , this->control_List[UI_CTRL_IDX_TXT_BODY].properties.pos.left
//      , this->control_List[UI_CTRL_IDX_TXT_BODY].properties.pos.top
//      , this->control_List[UI_CTRL_IDX_TXT_BODY].properties.pos.width
//      , this->control_List[UI_CTRL_IDX_TXT_BODY].properties.pos.heigth);
//    this->control_List[UI_CTRL_IDX_TXT_BODY].properties.pos.dump();
//#endif // UI_DP_JMD_PROCESSOR_CONSOLE_DEBUG_ENABLE
#ifdef UI_DP_JMD_PROCESSOR_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(uiDpJMDProcessorLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[uijmd] add %d", 99);
#endif // UI_DP_JMD_PROCESSOR_CONSOLE_DEBUG_ENABLE
    return 0;
  } while (0);
#ifdef UI_DP_JMD_PROCESSOR_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(uiDpJMDProcessorLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[uijmd] add %d", -98);
#endif // UI_DP_JMD_PROCESSOR_CONSOLE_DEBUG_ENABLE
  this->removeControls();
#ifdef UI_DP_JMD_PROCESSOR_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(uiDpJMDProcessorLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[uijmd] add %d", -99);
#endif // UI_DP_JMD_PROCESSOR_CONSOLE_DEBUG_ENABLE
  return -1;
}

void UiDpJMDProcessor::removeControls(void)
{
  do
  {
    if (this->control_List == NULL)
    {
      return;
    }

    delete[] this->control_List;
    this->control_List = NULL;
    return;
  } while (0);
  return;
}

int UiDpJMDProcessor::uiMessRaw(UiMessRawTAG* param)
{
  return this->control_List[UI_CTRL_IDX_TXT_BODY].drawText(param->data);
}

int UiDpJMDProcessor::uiMessMessage(UiMessMessageTAG* param)
{
  return this->control_List[UI_CTRL_IDX_TXT_BODY].drawText(param->mess);
}

int UiDpJMDProcessor::uiMessSysState(UiMessSysStateTAG* param)
{
  int ret = 0;
  do
  {
#ifdef UI_DP_JMD_PROCESSOR_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(uiDpJMDProcessorLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[jmdP] %d %d %d", 0, param->stateId, param->stateSubId);
#endif // UI_DP_JMD_PROCESSOR_CONSOLE_DEBUG_ENABLE

    const char* mess = NULL;
    bool updateBody = true;
    bool checkSubParam = true;
    char chState[2] = { UI_MESS_SYS_STATE_ICON_IDLE, '\0'};
    switch (param->stateId)
    {
    case UI_MESS_SYS_STATE_BOOTING:
    {
      mess = UI_MESS_SYS_STATE_TXT_BOOTING;
      break;
    }
      
    case UI_MESS_SYS_STATE_RESETTING:
      mess = UI_MESS_SYS_STATE_TXT_RESETTING;
      break;
    case UI_MESS_SYS_STATE_SETTING:
      mess = UI_MESS_SYS_STATE_TXT_SETTING;
      break;
    case UI_MESS_SYS_STATE_IDLE:
      mess = UI_MESS_SYS_STATE_TXT_IDLE;
      chState[0] = UI_MESS_SYS_STATE_ICON_IDLE;
      checkSubParam = false;
      updateBody = false;
      break;
    case UI_MESS_SYS_STATE_SLEEPING:
      mess = UI_MESS_SYS_STATE_TXT_SLEEPING;
      chState[0] = UI_MESS_SYS_STATE_ICON_SLEEPING;
      checkSubParam = false;
      updateBody = false;
      break;
    case UI_MESS_SYS_STATE_SCANNING:
      mess = UI_MESS_SYS_STATE_TXT_SCANNING;
      break;
    case UI_MESS_SYS_STATE_ERROR:
      mess = UI_MESS_SYS_STATE_TXT_ERROR;
      chState[0] = UI_MESS_SYS_STATE_ICON_ERROR;
      checkSubParam = false;
      updateBody = false;
      break;
    case UI_MESS_SYS_STATE_LOADDING:
      mess = UI_MESS_SYS_STATE_TXT_LOADDING;
      break;
    default:
      updateBody = false;
      break;
    }

    if (checkSubParam != false)
    {
      switch (param->stateSubId)
      {
      case UI_MESS_SYS_STATE_SUB_START:
      case UI_MESS_SYS_STATE_SUB_RUNNING:
        chState[0] = UI_MESS_SYS_STATE_ICON_OPERATING;
        LedManager::getInstance().turnOn(FEM_LED_GREEN, LED_DEVICE_INTENSITY_LEVEL_MAX, 0);
        break;
      case UI_MESS_SYS_STATE_SUB_END:
      case UI_MESS_SYS_STATE_SUB_CANCEL:
        chState[0] = UI_MESS_SYS_STATE_ICON_IDLE;
        updateBody = false;
        LedManager::getInstance().turnOff(FEM_LED_GREEN);
        break;
      case UI_MESS_SYS_STATE_SUB_ERROR:
      default:
        chState[0] = UI_MESS_SYS_STATE_ICON_ERROR;
        updateBody = false;
        LedManager::getInstance().turnOff(FEM_LED_GREEN);
        break;
      }
    }
    else
    {
      LedManager::getInstance().turnOff(FEM_LED_GREEN);
    }

    if ((param->stateId == UI_MESS_SYS_STATE_SCANNING)
      && (param->stateSubId == UI_MESS_SYS_STATE_SUB_START)
      )
    {
      BuzzerManager::getInstance().turnOn(FEM_BUZZER_0, NOTE_C4, 100);
    }
    
    ret = this->control_List[UI_CTRL_IDX_LBL_STATE].drawText(chState);
    if (ret != 0)
    {
      break;
    }

    if (updateBody == false)
    {
      return 0;
    }

    ret = this->control_List[UI_CTRL_IDX_TXT_BODY].drawText(mess);
    if (ret != 0)
    {
      break;
    }
    return 0;
  } while (0);
  return -1;
  this->control_List[UI_CTRL_IDX_TXT_BODY].clear();
  this->control_List[UI_CTRL_IDX_LBL_STATE].clear();
  return 0;
}

int UiDpJMDProcessor::uiMessNetState(UiMessNetStateTAG* param)
{
  int ret = 0;
  do
  {
    char chwifiMode[2];
    chwifiMode[0] = UI_MESS_NET_MODE_ICON_NONE;
    chwifiMode[1] = '\0';
    if (param->stateId == UI_MESS_NET_STATE_CONNECTED)
    {
      if (param->mode == UI_MESS_NET_MODE_AP)
      {
        chwifiMode[0] = UI_MESS_NET_MODE_ICON_AP;
      }
      else
      {
        chwifiMode[0] = UI_MESS_NET_MODE_ICON_STA;
      }
    }
    
    ret = this->control_List[UI_CTRL_IDX_LBL_WIFI_STATE].drawText(chwifiMode);
    if (ret != 0)
    {
      break;
    }

    uint8_t ipLen = strlen(param->ip4);
    ret = this->control_List[UI_CTRL_IDX_LBL_IP].drawText(param->ip4 + ipLen - 3); // display 3 last characters only
    if (ret != 0)
    {
      break;
    }

    ret = this->control_List[UI_CTRL_IDX_TXT_BODY].drawText(param->ip4);
    if (ret != 0)
    {
      break;
    }

    return 0;
  } while (0);
  return -1;
  this->control_List[UI_CTRL_IDX_LBL_WIFI_STATE].clear();
  this->control_List[UI_CTRL_IDX_LBL_IP].clear();
  this->control_List[UI_CTRL_IDX_TXT_BODY].clear();
  return 0;
}

int UiDpJMDProcessor::uiMessOpMode(UiMessOpModeTAG* param)
{
  int ret = 0;
  do
  {
    char chOpMode[2];
    chOpMode[0] = UI_MESS_OP_MODE_ICON_NORMAL;
    chOpMode[1] = '\0';
    switch (param->mode)
    {
      case UI_MESS_OP_MODE_NORMAL:
        chOpMode[0] = UI_MESS_OP_MODE_ICON_NORMAL;
        break;
      case UI_MESS_OP_MODE_SETTING:
        chOpMode[0] = UI_MESS_OP_MODE_ICON_SETTING;
        break;
      case UI_MESS_OP_MODE_RESET:
        chOpMode[0] = UI_MESS_OP_MODE_ICON_RESET;
        break;
      default:
        break;
    }

    LedManager::getInstance().turnOn(FEM_LED_YELLOW, LED_DEVICE_INTENSITY_LEVEL_MAX, 0);
    BuzzerManager::getInstance().turnOn(FEM_BUZZER_0, NOTE_A4, 1000);
    LedManager::getInstance().turnOff(FEM_LED_YELLOW);

    ret = this->control_List[UI_CTRL_IDX_LBL_MODE].drawText(chOpMode);
    if (ret != 0)
    {
      break;
    }

    return 0;
  } while (0);
  return -1;
  this->control_List[UI_CTRL_IDX_LBL_MODE].clear();
  return 0;
}

int UiDpJMDProcessor::uiMessScanResult(UiMessScanResultTAG* param)
{
  int ret = 0;
  do
  {
    param->code.code[UI_SCANNING_DEVICE_BARCODE_LEN_MAX] = '\0';
    BuzzerManager::getInstance().turnOn(FEM_BUZZER_0, NOTE_A4, 100);
    ret = this->control_List[UI_CTRL_IDX_TXT_BODY].drawText(param->code.code);
#ifdef UI_DP_JMD_PROCESSOR_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(uiDpJMDProcessorLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[jmdP] sRe %d %s", 0, param->code.code);
#endif // UI_DP_JMD_PROCESSOR_CONSOLE_DEBUG_ENABLE
    if (ret != 0)
    {
      break;
    }

    return 0;
  } while (0);
  return -1;
  this->control_List[UI_CTRL_IDX_TXT_BODY].clear();
  return 0;
}

int UiDpJMDProcessor::uiMessBatteryStatus(UiMessBatteryStatusTAG* param)
{
  int ret = 0;
  do
  {
    char tmpDrawText[8];
    SYSTEM_PRINT_BUF(tmpDrawText, 8, "%d%%", param->percentage);
    ret = this->control_List[UI_CTRL_IDX_LBL_BATTERY].drawText(tmpDrawText);
    if (ret != 0)
    {
      break;
    }

    return 0;
  } while (0);
  return -1;
  this->control_List[UI_CTRL_IDX_LBL_BATTERY].clear();
  return 0;
}

void UiDpJMDProcessor::stop(void)
{
  this->removeControls();
  UiDpProcessor::stop();
}


#endif // _CONF_UI_DP_JMD_PROCESSOR_ENABLED