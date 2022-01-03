#include "UiManagerConstant.h"

#if (_CONF_UI_DP_PROCESSOR_ENABLED)

#ifndef _UI_DP_PROCESSOR_H
#define _UI_DP_PROCESSOR_H

/////////////////////////////////////////////////
// INCLUDE
#include "task_display/DisplayControllerFactory.h"
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

/*UiDpProcessor*/
class UiDpProcessor
{
public:
  UiDpProcessor(byte procType);
  virtual ~UiDpProcessor(void);
  virtual bool                                                  isValid(void);
  byte                                                          procType(void);
  virtual int                                                   start(UiDpProcessorConfigTAG& deviceConfig);
  virtual void                                                  stop(void);
  virtual int                                                   uiMessRaw(UiMessRawTAG* param) = 0;
  virtual int                                                   uiMessMessage(UiMessMessageTAG* param) = 0;
  virtual int                                                   uiMessSysState(UiMessSysStateTAG* param) = 0;

protected:
  byte                                                          proc_Type;
  DisplayController*                                            dp_Controller;
};

#endif // _UI_DP_PROCESSOR_H

#endif // _CONF_UI_DP_PROCESSOR_ENABLED