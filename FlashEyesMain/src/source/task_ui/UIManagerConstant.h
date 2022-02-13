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
#define UI_SCANNING_DEVICE_BARCODE_LEN_MAX                          48
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
//################UiMessNetState::mode#########
#define UI_MESS_NET_MODE_AP                                         0
#define UI_MESS_NET_MODE_STA                                        1

//################UiMessOpModeTAG::mode#########
#define UI_MESS_OP_MODE_NORMAL                                      SYS_MODE_NORMAL
#define UI_MESS_OP_MODE_RESET                                       SYS_MODE_RESET
#define UI_MESS_OP_MODE_SETTING                                     SYS_MODE_SETTING

//##########MEssage for state#################
#define UI_MESS_SYS_STATE_TXT_BOOTING                               "Booting.."
#define UI_MESS_SYS_STATE_TXT_RESETTING                             "Reseting.."
#define UI_MESS_SYS_STATE_TXT_SETTING                               "Setting.."
#define UI_MESS_SYS_STATE_TXT_IDLE                                  " "
#define UI_MESS_SYS_STATE_TXT_SLEEPING                              " "
#define UI_MESS_SYS_STATE_TXT_SCANNING                              " "
#define UI_MESS_SYS_STATE_TXT_ERROR                                 " "
#define UI_MESS_SYS_STATE_TXT_LOADDING                              "Loading.."

#define UI_MESS_SYS_STATE_ICON_OPERATING                            'O'
#define UI_MESS_SYS_STATE_ICON_IDLE                                 'I'
#define UI_MESS_SYS_STATE_ICON_ERROR                                'E'
#define UI_MESS_SYS_STATE_ICON_SLEEPING                             'S'

#define UI_MESS_NET_MODE_ICON_AP                                    'A'
#define UI_MESS_NET_MODE_ICON_STA                                   'S'
#define UI_MESS_NET_MODE_ICON_NONE                                  'N'

#define UI_MESS_OP_MODE_ICON_NORMAL                                 'N'
#define UI_MESS_OP_MODE_ICON_RESET                                  'R'
#define UI_MESS_OP_MODE_ICON_SETTING                                'S'
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
  UIControlJMDDevice,
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

typedef struct _uiMessOpModeTag
{
  byte mode;
} UiMessOpModeTAG;

typedef struct _uiMessNetStateTag
{
  byte stateId; //
  byte stateSubId; //
  byte mode; // 0: AP 1: STA
  char ip4[UI_MESS_IP4_LEN_MAX];
} UiMessNetStateTAG;

typedef struct _uiMessageBarCodeTag
{
  byte type;
  DataSize_t codeLen;
  char code[UI_SCANNING_DEVICE_BARCODE_LEN_MAX + 1];
} UiMessageBarCodeTAG;

typedef struct _uiMessScanResultTag
{
  Seq_t sequenceId;
  byte scanIndex;
  UiMessageBarCodeTAG code;
} UiMessScanResultTAG;

typedef struct _uiMessBatteryStatusTag
{
  byte percentage;
  byte warningLevel;
} UiMessBatteryStatusTAG;

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
    UiMessOpMode,
    UiMessScanResult,
    UiMessBatteryStatus,
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
      case UIConstant::UIMessageId::UiMessOpMode:
      case UIConstant::UIMessageId::UiMessScanResult:
      case UIConstant::UIMessageId::UiMessBatteryStatus:
        return true;
      default:
        return false;
    }
  }
};

#endif // _CONF_UI_MANAGER_CONSTANT_ENABLED

#endif // _UI_MANAGER_CONSTANT_H