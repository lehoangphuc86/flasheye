#include "../../FlashEyesMain.h"
#ifndef _UI_MANAGER_CONSTANT_H
#define _UI_MANAGER_CONSTANT_H
#if (_CONF_UI_MANAGER_CONSTANT_ENABLED)

/////////////////////////////////////////////////
// INCLUDE
#include "task_display/DisplayTaskConstant.h"
/////////////////////////////////////////////////
// PREPROCESSOR

/////////////////////////////////////////////////
// DEFINE
//#define DB_QUERY_LEN_MAX                          128
#define UI_MESS_RAW_DATA_LEN_MAX                    32

#define UI_MESS_MESSAGE_LEN_MAX                     32
/////////////////////////////////////////////////
// MARCO

/////////////////////////////////////////////////
// DATA TYPE (TYPEDEF)

/////////////////////////////////////////////////
// DATA TYPE (ENUM)
typedef enum _uiControlTypeUN
{
  UIControlLCDDevice = 0,
  UIControlLCDI2CDevice,
  UIControlSerialConsoleDevice,
  UIControlDeviceTypeMax
} UIControlTypeUN;

/////////////////////////////////////////////////
// DATA TYPE (STRUCT)


typedef struct _uiDpProcessorConfigTAG
{
  DisplayDeviceConfig deviceConfig;
} UiDpProcessorConfigTAG;

typedef struct _uiManagerConfigTAG
{
  byte dpProcType;
  UiDpProcessorConfigTAG dpProcConfig;
  // buzzer
  // lcd
  BufferDataManagerConfigTAG bufferConfig;
  TaskManagerConfigTAG taskManagerConfig;
  TaskThreadConfigTAG taskThreadConfig;
} UiManagerConfigTAG;


// ui message
typedef struct _uiMessRawTag
{
  char data[UI_MESS_RAW_DATA_LEN_MAX];
  DataSize_t len;
} UiMessRawTAG;

typedef struct _uiMessMessageTag
{
  char mess[UI_MESS_MESSAGE_LEN_MAX];
  DataSize_t messLen;
  byte messType; // 
} UiMessMessageTAG;

typedef struct _uiMessSysStateTag
{
  byte state; //
} UiMessSysStateTAG;


// event
typedef struct _eventUiMessageTag
{
  UIMessageId_t uiMessId;
  BufferDataItem* buffItem;
} EventUiMessageTAG;
/////////////////////////////////////////////////
// GLOBAL VARIABLES

/////////////////////////////////////////////////
// GLOBAL FUNCTIONS

/////////////////////////////////////////////////
// STATIC DATA

/////////////////////////////////////////////////
// STATIC FUNCTIONS

/////////////////////////////////////////////////
// EXTERN

/////////////////////////////////////////////////
// CLASS DEFINITION
/*UIConstant*/
class UIConstant
{
public:
  enum UIMessageId
  {
    UiMessRaw,
    UiMessMessage,
    UiMessSysState,
    UiMessMax
  };

public:
  UIConstant(void) {};
  // WARNING: if inherite from this class, deconstructor must be virtual
  __ATTRIBUTE_VIRTUAL_OPTIMIZED ~UIConstant(void) {};

  static bool isValidMessageId(int uiMessageId)
  {
    switch (uiMessageId)
    {
      case UIConstant::UIMessageId::UiMessRaw:
      case UIConstant::UIMessageId::UiMessMessage:
      case UIConstant::UIMessageId::UiMessSysState:
        return true;
      default:
        return false;
    }
  }
};

#endif // _CONF_UI_MANAGER_CONSTANT_ENABLED

#endif // _UI_MANAGER_CONSTANT_H