#include "UiManagerConstant.h"

#if (_CONF_UI_DP_SC_PROCESSOR_ENABLED)

#ifndef _UI_DP_SC_PROCESSOR_H
#define _UI_DP_SC_PROCESSOR_H

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

/*UiDpSCProcessor*/
class UiDpSCProcessor
  : public UiDpProcessor
{
public:
  UiDpSCProcessor(void);
  virtual ~UiDpSCProcessor(void);
  int                                                           start(UiDpProcessorConfigTAG& deviceConfig) override;
  void                                                          stop(void) override;
  void                                                          uiMessRaw(UiMessRawTAG* param) override;
  void                                                          uiMessMessage(UiMessMessageTAG* param) override;
  void                                                          uiMessSysState(UiMessSysStateTAG* param) override;

protected:
  char*                                                         tmp_Buff;
};

#endif // _UI_DP_SC_PROCESSOR_H

#endif // _CONF_UI_DP_SC_PROCESSOR_ENABLED