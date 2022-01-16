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
//#define DB_QUERY_LEN_MAX                                          128
#define UI_MESS_RAW_DATA_LEN_MAX                                    32

#define UI_MESS_MESSAGE_LEN_MAX                                     32

#define UI_MESS_IP4_LEN_MAX                                         18

//##############UiMessSysState::stateId##############
#define UI_MESS_SYS_STATE_BOOTING                                   0
#define UI_MESS_SYS_STATE_RESETTING                                 1
#define UI_MESS_SYS_STATE_SETTING                                   2
#define UI_MESS_SYS_STATE_IDLE                                      3
#define UI_MESS_SYS_STATE_SLEEPING                                  4
#define UI_MESS_SYS_STATE_SCANNING                                  5
#define UI_MESS_SYS_STATE_ERROR                                     6
#define UI_MESS_SYS_STATE_LOADDING                                  7
#define UI_MESS_SYS_STATE_MAX                                       8

//################UiMessSysState::stateSubId#########
//--------------------
#define UI_MESS_SYS_STATE_SUB_NONE                                  0
#define UI_MESS_SYS_STATE_SUB_START                                 1
#define UI_MESS_SYS_STATE_SUB_RUNNING                               2
#define UI_MESS_SYS_STATE_SUB_END                                   3
#define UI_MESS_SYS_STATE_SUB_CANCEL                                4
#define UI_MESS_SYS_STATE_SUB_ERROR                                 5
//----------------UI_MESS_SYS_STATE_RESETTING--------
#define UI_MESS_SYS_STATE_SUB_RESETTING_DB_RESETING                 0
#define UI_MESS_SYS_STATE_SUB_RESETTING_DB_ERROR                    1


//################UiMessNetState::stateId############
#define UI_MESS_NET_STATE_DISCONNECTED                              0
#define UI_MESS_NET_STATE_CONNECTED                                 1
#define UI_MESS_NET_STATE_ERROR                                     2
#define UI_MESS_NET_STATE_MAX                                       3
//################UiMessNetState::stateSubId#########
#define UI_MESS_NET_STATE_SUB_NONE                                  0

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
  byte stateId; //
  byte stateSubId; //
} UiMessSysStateTAG;

typedef struct _uiMessNetStateTag
{
  byte stateId; //
  byte stateSubId; //
  char ip4[UI_MESS_IP4_LEN_MAX];
} UiMessNetStateTAG;

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
    UiMessNetState,
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
      case UIConstant::UIMessageId::UiMessNetState:
        return true;
      default:
        return false;
    }
  }
};

#endif // _CONF_UI_MANAGER_CONSTANT_ENABLED

#endif // _UI_MANAGER_CONSTANT_H