#include "UiManagerConstant.h"
#ifndef _UI_CONTROL_TASK_H
#define _UI_CONTROL_TASK_H
#if (_CONF_UI_CONTROL_TASK_ENABLED)
/////////////////////////////////////////////////
// INCLUDE
#include "UiDpProcessorFactory.h"
#include "task_manager/TaskManager.h"
#include "data_manager/BufferDataManager.h"
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

/*UiControlTask*/
class UiControlTask
  : public TaskManager
{
public:
  UiControlTask(void);
  virtual ~UiControlTask(void);
  bool                                                          isValid(void);
  DataSize_t                                                    regUIMessageId(UIMessageId_t messageId, DataSize_t maxRawSize = 0);
  DataSize_t                                                    getUIMessageParamSize(UIMessageId_t messageId);
  BufferDataItem*                                               getBuff(void);
  BufferDataManager*                                            dataManager(void);
  void                                                          releaseBuff(BufferDataItem* dataItem);

  int                                                           startTask(UiManagerConfigTAG& uiManagerConfig);
  void                                                          stopTask(void);
  void                                                          cleanUp(void);

protected:
  void                                                          proc(void) override;
  int                                                           prepare(void);
  int                                                           onEventUiMessage(unsigned char* data, unsigned int dataSize);

  int                                                           onUiMessRaw(EventUiMessageTAG* eventData);
  int                                                           onUiMessMessage(EventUiMessageTAG* eventData);
  int                                                           onUiMessSysState(EventUiMessageTAG* eventData);
  int                                                           onUiMessNetState(EventUiMessageTAG* eventData);
  int                                                           onUiMessOpMode(EventUiMessageTAG* eventData);
  int                                                           onUiMessScanResult(EventUiMessageTAG* eventData);
  int                                                           onUiMessBatteryStatus(EventUiMessageTAG* eventData);
protected:
  DataSize_t                                                    max_Data_Size;
  BufferDataManager                                             data_Manager;
  UiDpProcessor*                                                dp_Processor;
};

#endif // _UI_CONTROL_TASK_H

#endif // _CONF_UI_CONTROL_TASK_ENABLED