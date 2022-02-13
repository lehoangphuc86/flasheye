#include "UiManagerConstant.h"

#if (_CONF_UI_DP_JMD_PROCESSOR_ENABLED)

#ifndef _UI_DP_JMD_PROCESSOR_H
#define _UI_DP_JMD_PROCESSOR_H

/////////////////////////////////////////////////
// INCLUDE
#include "UiDpProcessor.h"
/////////////////////////////////////////////////
// PREPROCESSOR
//#define UI_DP_JMD_PROCESSOR_CONSOLE_DEBUG_2_ENABLE
/////////////////////////////////////////////////
// DEFINE
#define UI_CTRL_IDX_LBL_MODE                          0
#define UI_CTRL_IDX_LBL_STATE                         1
#define UI_CTRL_IDX_LBL_WIFI_STATE                    2
#define UI_CTRL_IDX_LBL_IP                            3
#define UI_CTRL_IDX_LBL_BATTERY                       4
#define UI_CTRL_IDX_TXT_BODY                          5
#define UI_CTRL_IDX_MAX                               6


#define UI_CTRL_X_SPACE                               8
#define UI_CTRL_Y_SPACE                               4

#define UI_CTRL_FONT_WIDTH_HEADER                     8
#define UI_CTRL_FONT_HEIGHT_HEADER                    8

#define UI_CTRL_FONT_WIDTH_BODY                       13
#define UI_CTRL_FONT_HEIGHT_BODY                      10
/////////////////////////////////////////////////
// MARCO

/////////////////////////////////////////////////
// GLOBAL VARIABLES

/////////////////////////////////////////////////
// GLOBAL FUNCTIONS
extern char uiDpJMDProcessorLogBuf[SYSTEM_CONSOLE_OUT_BUF_LEN];
/////////////////////////////////////////////////
// DATA TYPE (TYPEDEF)

typedef struct _uiControlPosTag
{
  uint8_t left = 0; // cursor left. U
  uint8_t top = 0; // cursor bottom
  uint8_t width = 0;
  uint8_t heigth = 0;
  void set(uint8_t left, uint8_t top, uint8_t width, uint8_t heigth)
  {
    this->left = left;
    this->top = top;
    this->width = width;
    this->heigth = heigth;
  }

  void dump(void)
  {
#ifdef UI_DP_JMD_PROCESSOR_CONSOLE_DEBUG_2_ENABLE
    CONSOLE_LOG_BUF(uiDpJMDProcessorLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[uiP] dm %d %d %d %d", this->left, this->top, this->width, this->heigth);
#endif // UI_DP_JMD_PROCESSOR_CONSOLE_DEBUG_2_ENABLE
  }

} UiControlPosTAG;

typedef struct _uiControlPropertiesTag
{
  //protected:
  UiControlPosTAG pos;
  uint8_t fontIndx = DISPLAY_FONT_IDX_PCSENIOR_8F;
  uint8_t fontWidth = 1;
  uint8_t fontHeight = 1;
} UiControlPropertiesTAG;

typedef struct _uiControlTag
{
  //protected:
  UiControlPropertiesTAG properties;
  DisplayController* controller;
public:
  _uiControlTag(void)
    : controller(NULL)
  {

  }

  bool isValid(void)
  {
    return this->controller == NULL ? false : true;
  }

  int start(DisplayController* ctr, UiControlPropertiesTAG& properties)
  {
    do
    {
      if (ctr == NULL)
      {
        break;
      }

      this->controller = ctr;
      memcpy(&this->properties, &properties, sizeof(UiControlPropertiesTAG));
      this->properties.pos.heigth = this->properties.pos.heigth * this->properties.fontHeight;
      this->properties.pos.width = this->properties.pos.width * this->properties.fontWidth;
      this->clear();
      return 0;
    } while (0);
    return -1;
  }

  void clear(void)
  {
    this->controller->setDrawColor(0);
    this->controller->drawBox(this->properties.pos.left, this->properties.pos.top, this->properties.pos.width, this->properties.pos.heigth);
    this->controller->setDrawColor(1);
  }

  int drawText(const char* data, bool clear = true)
  {
    return this->drawText((char*)data, clear);
  }

  int drawText(char* text, bool clear = true)
  {
    int ret = 0;
    do
    {
      if (clear != false)
      {
        this->clear();
      }

      if (text == NULL)
      {
        break;
      }
#ifdef UI_DP_JMD_PROCESSOR_CONSOLE_DEBUG_2_ENABLE
      CONSOLE_LOG_BUF(uiDpJMDProcessorLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[uiC] dp: %d %d %s", this->properties.pos.left, this->properties.pos.top + this->properties.pos.heigth, text);
#endif // UI_DP_JMD_PROCESSOR_CONSOLE_DEBUG_2_ENABLE
      this->controller->setFont(this->properties.fontIndx);
      this->controller->drawStr(this->properties.pos.left, this->properties.pos.top + this->properties.pos.heigth, text);
      this->controller->sendBuffer();
      return 0;
    } while (0);
    return -1;
  }
} UiControlTAG;
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

/*UiDpJMDProcessor*/
class UiDpJMDProcessor
  : public UiDpProcessor
{
public:
  UiDpJMDProcessor(void);
  virtual ~UiDpJMDProcessor(void);
  int                                                           start(UiDpProcessorConfigTAG& deviceConfig) override;
  void                                                          stop(void) override;
  int                                                           uiMessRaw(UiMessRawTAG* param) override;
  int                                                           uiMessMessage(UiMessMessageTAG* param) override;
  int                                                           uiMessSysState(UiMessSysStateTAG* param) override;
  int                                                           uiMessNetState(UiMessNetStateTAG* param) override;
  int                                                           uiMessOpMode(UiMessOpModeTAG* param) override;
  int                                                           uiMessScanResult(UiMessScanResultTAG* param) override;
  int                                                           uiMessBatteryStatus(UiMessBatteryStatusTAG* param) override;
protected:
  int                                                           addControls(UiDpProcessorConfigTAG& deviceConfig);
  void                                                          removeControls(void);
protected:
  UiControlTAG*                                                 control_List;
};

#endif // _UI_DP_JMD_PROCESSOR_H

#endif // _CONF_UI_DP_JMD_PROCESSOR_ENABLED