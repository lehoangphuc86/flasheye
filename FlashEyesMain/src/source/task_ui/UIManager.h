#include "UiManagerConstant.h"
#ifndef _UI_MANAGER_H
#define _UI_MANAGER_H
#if (_CONF_UI_MANAGER_ENABLED)
/////////////////////////////////////////////////
// INCLUDE
#include "UiControlTask.h"
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

/*UiManager*/
class UiManager
{
private:
  UiManager(void);
  __ATTRIBUTE_VIRTUAL_OPTIMIZED ~UiManager(void);

public:
  static UiManager& getInstance(void);
  UiManager(UiManager const&) = delete;
  void operator=(UiManager const&) = delete;
  bool                                                          isRunning(void);
  bool                                                          isValid(void);
  DataSize_t                                                    regUIMessageId(UIMessageId_t messageId, DataSize_t maxRawSize = 0);
  DataSize_t                                                    getUIMessageParamSize(UIMessageId_t messageId);
  BufferDataItem*                                               getBuff(void);
  BufferDataManager*                                            dataManager(void);
  void                                                          releaseBuff(BufferDataItem* dataItem);
  EventManager*                                                 eventManager(void);
  int                                                           startTask(UiManagerConfigTAG& uiManagerConfig);
  void                                                          stopTask(void);
  int                                                           show(UIMessageId_t uiMessageId, DataSize_t dataSize, unsigned char* data);
  int                                                           showSysState(byte stateId, byte stateSubId);
protected:
  UiControlTask                                                 control_Task;
};
#endif // _CONF_UI_MANAGER_ENABLED

#endif // _UI_MANAGER_H