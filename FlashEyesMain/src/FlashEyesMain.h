#ifndef _FLASH_EYE_MAIN_CONSTANT_H
#define _FLASH_EYE_MAIN_CONSTANT_H

/////////////////////////////////////////////////
// INCLUDE
#include "FlashEyesCoreLib.h"
#include "FlashEyesMainConfig.h"
#include "source/task_http/CommHttpConstant.h"
#include "timer_manager/TimerManagerConstant.h"
#include "source/task_ui/UiManagerConstant.h"
#include "source/task_net/NetManagerConstant.h"
#include "source/task_net/wifi/NetWifiConstant.h"
/////////////////////////////////////////////////
// PREPROCESSOR

/////////////////////////////////////////////////
// DEFINE

// system mode
#define SYS_MODE_RESET                                0
#define SYS_MODE_SETTING                              1
#define SYS_MODE_NORMAL                               2
#define SYS_MODE_MAX                                  3
#define SYS_MODE_INVALID                              SYS_MODE_MAX

// Common
#define FEM_DEBUG_SERIAL_BAUDRATE                     56000
#define FEM_DEBUG_SERIAL_INIT_WAIT                    1000 // ms
// task
#define FEM_TASK_PRIORITY_DEFAULT                     3

// SW settings
#define FEM_SW_SYS_MODE_TRY_TIME                      5
#define FEM_SW_SYS_MODE_WAIT_TIME                     1000
#define FEM_SW_BUTTON_COUNT                           2
#define FEM_SW_BUTTON_BOUNCE_TIME                     (200) // ms
#define FEM_SW_ITEM_1_OPCODE                          (0x01)
#define FEM_SW_ITEM_2_OPCODE                          (0x02)
#define FEM_SW_SYS_MODE_RESET_BIT                     (FEM_SW_ITEM_1_OPCODE | FEM_SW_ITEM_2_OPCODE)
#define FEM_SW_SYS_MODE_SCANING_BIT                   (FEM_SW_ITEM_1_OPCODE)
#define FEM_SW_SYS_MODE_SETTING_BIT                   (FEM_SW_ITEM_2_OPCODE)
#define FEM_SW_PRESS_TIME_REQUIRED                    2

#define FEM_PIN_SW_1                                  32
#define FEM_ISR_TYPE_SW_1                             ISR_FALLING
#define FEM_ISR_TYPE_SYSMODE_SW_1                     ISR_ONLOW
#define FEM_GPIO_FUNC_SW_1                            GPIO_INPUT_PULLUP
#define FEM_OPCODE_SW_1                               FEM_SW_ITEM_1_OPCODE
#define FEM_DESC_SW_1                                 "Scan"

#define FEM_PIN_SW_2                                  33
#define FEM_ISR_TYPE_SW_2                             ISR_FALLING
#define FEM_ISR_TYPE_SYSMODE_SW_2                     ISR_ONLOW
#define FEM_GPIO_FUNC_SW_2                            GPIO_INPUT_PULLUP
#define FEM_OPCODE_SW_2                               FEM_SW_ITEM_2_OPCODE
#define FEM_DESC_SW_2                                 "Reset"

// Btn settings
#define FEM_BTN_SYS_MODE_TRY_TIME                     5
#define FEM_BTN_SYS_MODE_WAIT_TIME                    1000
#define FEM_BTN_BUTTON_COUNT                          2
#define FEM_BTN_BUTTON_BOUNCE_TIME                    (300) // ms

#define FEM_BTN_ITEM_2_OPCODE                         (0x02)
#define FEM_BTN_SYS_MODE_RESET_BIT                    (FEM_SW_ITEM_1_OPCODE | FEM_SW_ITEM_2_OPCODE)
#define FEM_BTN_SYS_MODE_SCANING_BIT                  (FEM_SW_ITEM_1_OPCODE)
#define FEM_BTN_SYS_MODE_SETTING_BIT                  (FEM_SW_ITEM_2_OPCODE)
#define FEM_BTN_PRESS_TIME_REQUIRED                   2

#define FEM_PIN_SCAN_BUTTON                           32
#define FEM_BTN_OPCODE_SCAN_BTN                       (0x01)
#define FEM_ISR_TYPE_SCAN_BUTTON                      ISR_ONLOW
#define FEM_GPIO_FUNC_SCAN_BUTTON                     GPIO_INPUT_PULLUP
#define FEM_OPCODE_SCAN_BUTTON                        FEM_SW_ITEM_1_OPCODE
#define FEM_DESC_SCAN_BUTTON                          "Scan"

#define FEM_PIN_RESET_BUTTON                          33
#define FEM_BTN_OPCODE_RESET_BTN                      (0x02)
#define FEM_ISR_TYPE_RESET_BUTTON                     ISR_ONLOW
#define FEM_GPIO_FUNC_RESET_BUTTON                    GPIO_INPUT_PULLUP
#define FEM_OPCODE_RESET_BUTTON                       FEM_SW_ITEM_2_OPCODE
#define FEM_DESC_RESET_BUTTON                         "Reset"

///
#define FLASH_EYE_TASK_MEM_MAIN_CONTROLLER            526
#define FLASH_EYE_TASK_MEM_INPUT_HANDLER              526
#define FLASH_EYE_TASK_MEM_DISPLAY_CONTROL            526
#define FLASH_EYE_TASK_MEM_DISTANCE_SEN_CONTROL       526

#define FLASH_EYE_TASK_PRIORITY_MAIN_CONTROLLER       2
#define FLASH_EYE_TASK_PRIORITY_INPUT_HANDLER         2
#define FLASH_EYE_TASK_PRIORITY_DISPLAY_CONTROL       2
#define FLASH_EYE_TASK_PRIORITY_DISTANCE_SEN_CONTROL  2

#define FLASH_EYE_TASK_EVENT_NO_MAIN_CONTROLLER       5
#define FLASH_EYE_TASK_EVENT_NO_INPUT_HANDLER         3
#define FLASH_EYE_TASK_EVENT_NO_DISPLAY_CONTROL       3
#define FLASH_EYE_TASK_EVENT_NO_DISTANCE_SEN_CONTROL  3

#define FLASH_EYE_INPUT_SETTER_INTERRUPT_PIN          2
#define FLASH_EYE_INPUT_SETTER_BUTTON_0               4
#define FLASH_EYE_INPUT_SETTER_BUTTON_1               5

// buzzer manager
#define FEM_BUZZER_DEVICE_COUNT                       1
#define FEM_BUZZER_DEVICE_0_DEV_ID                    0
#define FEM_BUZZER_DEVICE_0_PIN                       15
#define FEM_BUZZER_DEVICE_0_HW_CHANNEL                0

#define FEM_BUZZER_0                                  FEM_BUZZER_DEVICE_0_DEV_ID

// led manager
#define FEM_LED_DEVICE_COUNT                          3
#define FEM_LED_DEVICE_0_CHANNEL_ID                   0
#define FEM_LED_DEVICE_0_PIN                          12
#define FEM_LED_DEVICE_0_DEFAULT_INTENSITY            252
#define FEM_LED_DEVICE_0_IS_PWM                       false
#define FEM_LED_DEVICE_1_CHANNEL_ID                   1
#define FEM_LED_DEVICE_1_PIN                          13
#define FEM_LED_DEVICE_1_DEFAULT_INTENSITY            253
#define FEM_LED_DEVICE_1_IS_PWM                       false
#define FEM_LED_DEVICE_2_CHANNEL_ID                   2
#define FEM_LED_DEVICE_2_PIN                          14
#define FEM_LED_DEVICE_2_DEFAULT_INTENSITY            254
#define FEM_LED_DEVICE_2_IS_PWM                       false

#define FEM_LED_GREEN                                 FEM_LED_DEVICE_0_CHANNEL_ID
#define FEM_LED_RED                                   FEM_LED_DEVICE_1_CHANNEL_ID
#define FEM_LED_YELLOW                                FEM_LED_DEVICE_2_CHANNEL_ID

// ui manager
#define FEM_UI_CONTROL_TYPE                           UIControlTypeUN::UIControlJMDDevice
#define FEM_UI_DM_BUFF_SIZE                           0
#define FEM_UI_DM_BUFF_COUNT                          5
#define FEM_UI_DM_USE_POOL                            false

#define FEM_UI_EM_EVENT_NUM                           5
#define FEM_UI_EM_USE_POOL                            false

#define FEM_UI_TM_USE_POOL                            false
#define FEM_UI_TM_MEM                                 2096
#define FEM_UI_TM_PRIORITY                            FEM_TASK_PRIORITY_DEFAULT

#define FEM_UI_DEV_DEVICE_TYPE                        DisplayDeviceTypeUN::DisplayJMDDevice
#define FEM_UI_DEV_DEVICE_COL                         128
#define FEM_UI_DEV_DEVICE_ROW                         32
#define FEM_UI_DEV_DEVICE_ID                          0

#define FEM_UI_DEV_DEVICE_JMD_PIN_SCL                 22
#define FEM_UI_DEV_DEVICE_JMD_PIN_SDA                 21

#define FEM_UI_DEV_DEVICE_SC_INSTANCE_IDX             1
#define FEM_UI_DEV_DEVICE_SC_BAUDRATE                 DISPLAY_SET_COMM_BAUD_9600
#define FEM_UI_DEV_DEVICE_SC_BIT_PER_BYTE             DISPLAY_SET_COMM_DATABIT_8
#define FEM_UI_DEV_DEVICE_SC_PARITY_TYPE              DISPLAY_SET_COMM_PARITY_NONE
#define FEM_UI_DEV_DEVICE_SC_STOP_BIT                 DISPLAY_SET_COMM_STOPBIT_1
#define FEM_UI_DEV_DEVICE_SC_TIMEOUT                  500
#define FEM_UI_DEV_DEVICE_SC_PIN_TX                   18
#define FEM_UI_DEV_DEVICE_SC_PIN_RX                   19


// Distance sensor
#define FLASH_EYE_DISTANCE_SENSOR_PIN_TRIGGER         47
#define FLASH_EYE_DISTANCE_SENSOR_PIN_ECHO            45


// Comm

// file system
#define FEM_FS_FORMAT                                 1
#define FEM_FS_BASE_PATH                              "/spiffs" // must not empty
#define FEM_FS_PARTITION_LABEL                        NULL
#define FEM_FS_MAX_FILE                               5

// DB
#define FEM_DB_PATH                                   "/spiffs/db/FEM_DB_01_01.db"
#define FEM_DB_TBL_ID_SCR_PATH_CF_SCAN                "/spiffs/db/sc/DB_INIT_TBL_CF_SCAN.sql"
#define FEM_DB_TBL_ID_SCR_PATH_CF_SYS                 "/spiffs/db/sc/DB_INIT_TBL_CF_SYS.sql"

#define FEM_DB_TBL_ID_CF_SYS                          0
#define FEM_DB_TBL_ID_CF_SCAN                         1
#define FEM_DB_TBL_ID_MAX                             2

#define FEM_DB_TBL_ID_INVALID                         FEM_DB_TBL_ID_MAX

// Net
#define FEM_NET_INTERFACE_TYPE                        NetInterfaceTypeUN::NetInterfaceWifi
#define FEM_NET_TASK_MEM                              2048
#define FEM_NET_TASK_PRIORITY                         FEM_TASK_PRIORITY_DEFAULT
#define FEM_NET_TASK_EVENT_NUM                        5

// Wifi
#define FEM_WIFI_AP_CHANNEL                           WIFI_MGR_CHANNEL_DEFAULT
#define FEM_WIFI_AP_MAX_CONN                          WIFI_MGR_MAX_CONN_DEFAULT

#define FEM_WIFI_CONN_RETRY                           WIFI_MGR_CONN_RETRY_MAX_DEFAULT
#define FEM_WIFI_AUTH_MODE                            WIFI_MGR_AUTH_MODE_DEFAULT

#define FEM_WIFI_STA_NET_STATIC_IP                    NET_IP4_INIT(192, 168, 137, 100)
#define FEM_WIFI_STA_NET_STATIC_NETMARK               NET_IP4_INIT(255, 255, 255, 0)
#define FEM_WIFI_STA_NET_STATIC_GW                    NET_IP4_INIT(192, 168, 137, 1)

#define FEM_WIFI_AP_NET_STATIC_IP                     NET_IP4_INIT(192, 169, 0, 100)
#define FEM_WIFI_AP_NET_STATIC_NETMARK                NET_IP4_INIT(255, 255, 255, 0)
#define FEM_WIFI_AP_NET_STATIC_GW                     NET_IP4_INIT(192, 169, 0, 1)

// Http server
#define FEM_HTTP_SERVER_IS_HEADER_LESS                0
#define FEM_HTTP_SERVER_TASK_USE_POOL                 false
#define FEM_HTTP_SERVER_TASK_MEM                      4096
#define FEM_HTTP_SERVER_TASK_PRIORITY                 FEM_TASK_PRIORITY_DEFAULT
#define FEM_HTTP_SERVER_TASK_EVENT_USE_POOL           false
#define FEM_HTTP_SERVER_TASK_EVENT_NUM                5
#define FEM_HTTP_SERVER_BUFF_USE_POOL                 false
#define FEM_HTTP_SERVER_BUFF_COUNT                    6
#define FEM_HTTP_SERVER_BUFF_SIZE                     200


#define FEM_HTTP_SERVER_BASE_PATH                     "/spiffs/web/public"
#define FEM_HTTP_SERVER_PORT                          80
#define FEM_HTTP_SERVER_STACK_SIZE                    5120
#define FEM_HTTP_SERVER_LRU_PURGE                     false
#define FEM_HTTP_SERVER_URI_COUNT_MAX                 5
#define FEM_HTTP_SERVER_CONN_COUNT_MAX                2
#define FEM_HTTP_SERVER_ADD_HEADER_COUNT_MAX          2
#define FEM_HTTP_SERVER_BACKLOG_CONN                  2

  // uri 1
#define FEM_HTTP_SERVER_URI_1_ID                      1
#define FEM_HTTP_SERVER_URI_1_URI                     "/api/v1/system/info"
#define FEM_HTTP_SERVER_URI_1_METHOD                  COMM_HTTP_METHOD_GET
#define FEM_HTTP_SERVER_URI_1_DATATYPE                COMM_HTTP_DATA_TYPE_APP_JSON
  // uri 2
#define FEM_HTTP_SERVER_URI_2_ID                      2
#define FEM_HTTP_SERVER_URI_2_URI                     "/api/v1/system/update"
#define FEM_HTTP_SERVER_URI_2_METHOD                  COMM_HTTP_METHOD_POST
#define FEM_HTTP_SERVER_URI_2_DATATYPE                COMM_HTTP_DATA_TYPE_APP_JSON

// Http client
#define FEM_HTTP_CLIENT_IS_HEADER_LESS                true
#define FEM_HTTP_CLIENT_TASK_MEM                      4096
#define FEM_HTTP_CLIENT_TASK_USE_POOL                 false
#define FEM_HTTP_CLIENT_TASK_PRIORITY                 FEM_TASK_PRIORITY_DEFAULT
#define FEM_HTTP_CLIENT_TASK_EVENT_NUM                5
#define FEM_HTTP_CLIENT_TASK_EVENT_USE_POOL           false
#define FEM_HTTP_CLIENT_BUFF_USE_POOL                 false
#define FEM_HTTP_CLIENT_BUFF_COUNT                    6
#define FEM_HTTP_CLIENT_BUFF_SIZE                     200


#define FEM_HTTP_CLIENT_PATH_DEFAULT                  "http://localhost/"
#define FEM_HTTP_CLIENT_REQ_URI_ID                    1
#define FEM_HTTP_CLIENT_REQ_DATATYPE                  COMM_HTTP_DATA_TYPE_APP_JSON
#define FEM_HTTP_CLIENT_REQ_METHOD                    COMM_HTTP_METHOD_POST


#define FEM_HTTP_CLIENT_REQ_CERT                      NULL


#define FEM_HTTP_CLIENT_REQ_BODY "{\"h\":{\"id\":4,\"sz\":73},\"b\":{\"secId\":1,\"setId\":2,\"bSet1\":16,\"eCode\":4,\"sLen\":0,\"data\":99.000000}}"
// Timer manager

// comm
#define FEM_TM_TASK_EVENT_NO                          5
#define FEM_TM_TASK_EVENT_NO                          5
#define FEM_TM_USE_POOL                               false
#define FEM_TM_TASK_MEM                               2048
#define FEM_TM_TASK_PRIORITY                          FEM_TASK_PRIORITY_DEFAULT

#define FEM_TM_HW_GROUP_ID                            TIMER_DEVICE_TIMER_HW_GROUP_ID_DEFAULT
#define FEM_TM_HW_INDEX_ID                            TIMER_DEVICE_TIMER_HW_INDEX_ID_DEFAULT
#define FEM_TM_HW_DEVIDER                             8

// export manager
#define FEM_EXPORT_EM_EVENT_NUM                       5
#define FEM_EXPORT_EM_USE_POOL                        false

#define FEM_EXPORT_TM_USE_POOL                        false
#define FEM_EXPORT_TM_MEM                             2096
#define FEM_EXPORT_TM_PRIORITY                        FEM_TASK_PRIORITY_DEFAULT

#define FEM_EXPORT_MODE_NONE                          0
#define FEM_EXPORT_MODE_HTTP_CLI                      1

// scanning task
#define FEM_SCAN_SEQ_ID_INVALID                       0
#define FEM_SCAN_EM_EVENT_NUM                         5
#define FEM_SCAN_EM_USE_POOL                          false

#define FEM_SCAN_TM_USE_POOL                          false
#define FEM_SCAN_TM_MEM                               2096
#define FEM_SCAN_TM_PRIORITY                          FEM_TASK_PRIORITY_DEFAULT

#define FEM_SCAN_DEV_DEVICE_TYPE                      ScanningDeviceTypeUN::ScanningEVDevice
#define FEM_SCAN_DEV_DEVICE_DATA_TIMEOUT              1000 // ms
#define FEM_SCAN_DEV_DEVICE_ID                        0
#define FEM_SCAN_DEV_PIN_LED                          5
#define FEM_SCAN_DEV_PIN_BUZZER                       18 // no used
#define FEM_SCAN_DEV_PIN_SENSOR                       19 // no used

#define FEM_SCAN_DEV_DEVICE_SC_INSTANCE_IDX           2
#define FEM_SCAN_DEV_DEVICE_SC_BAUDRATE               SCANNING_SET_COMM_BAUD_9600
#define FEM_SCAN_DEV_DEVICE_SC_BIT_PER_BYTE           SCANNING_SET_COMM_DATABIT_8
#define FEM_SCAN_DEV_DEVICE_SC_PARITY_TYPE            SCANNING_SET_COMM_PARITY_NONE
#define FEM_SCAN_DEV_DEVICE_SC_STOP_BIT               SCANNING_SET_COMM_STOPBIT_1
#define FEM_SCAN_DEV_DEVICE_SC_TIMEOUT                500
#define FEM_SCAN_DEV_DEVICE_SC_PIN_TX                 SCANNING_DEVICE_PIN_LIB_DEFAULT
#define FEM_SCAN_DEV_DEVICE_SC_PIN_RX                 SCANNING_DEVICE_PIN_LIB_DEFAULT

#define FEM_SCAN_OP_TIMEOUT                           10000 // ms
#define FEM_SCAN_OP_MAX_SCAN_COUNT                    1
#define FEM_SCAN_OP_BREAK_TIME                        100

#define FEM_SCAN_TRG_SRC_BUTTON                       0
#define FEM_SCAN_TRG_SRC_HTTP                         1
#define FEM_SCAN_TRG_SRC_SERIAL                       2
//#define FEM_SCAN_TRG_SRC_BUTTON                       3
#define FEM_SCAN_TRG_SRC_SENSOR                       3

#define FEM_SCAN_ERR_CODE_NONE                        0
#define FEM_SCAN_ERR_CODE_NOT_AVAI                    1
#define FEM_SCAN_ERR_CODE_NOT_VALID                   2
#define FEM_SCAN_ERR_CODE_OP_ERR                      3


// dist sensor task
#define FEM_DIST_SS_EM_EVENT_NUM                      5
#define FEM_DIST_SS_EM_USE_POOL                       false

#define FEM_DIST_SS_TM_USE_POOL                       false
#define FEM_DIST_SS_TM_MEM                            2096
#define FEM_DIST_SS_TM_PRIORITY                       FEM_TASK_PRIORITY_DEFAULT

#define FEM_DIST_SS_DEV_ID                            0
#define FEM_DIST_SS_DEV_DEVICE_TYPE                   DistSensorDeviceTypeUN::DistSensorHRCS04Device
#define FEM_DIST_SS_DEV_DEVICE_HRC04_PIN_TRG          27
#define FEM_DIST_SS_DEV_DEVICE_HRC04_PIN_ECHO         26

#define FEM_DIST_SS_MEAS_INTERVAL                     200 // ms
#define FEM_DIST_SS_OP_MODE                           DistSensorOpModeUN::DistSensorRangeMode


// main 
#define FEM_MAIN_DM_BUFF_SIZE                         200
#define FEM_MAIN_DM_BUFF_COUNT                        5
#define FEM_MAIN_DM_USE_POOL                          false

#define FEM_MAIN_EM_EVENT_NUM                         10
#define FEM_MAIN_EM_USE_POOL                          false

#define FEM_MAIN_TM_USE_POOL                          false
#define FEM_MAIN_TM_MEM                               4096
#define FEM_MAIN_TM_PRIORITY                          FEM_TASK_PRIORITY_DEFAULT

#define FEM_MAIN_RESET_CONFIRM_TIME                   5000 // ms


// Mess broker
// mess id
#define FEM_MB_MESS_ID_HEADLESS                       0
#define FEM_MB_MESS_ID_START1                         1
#define FEM_MB_MESS_ID_RESULT1                        2
#define FEM_MB_MESS_ID_SYSTEMSETTING                  3
#define FEM_MB_MESS_ID_MAX                            4
// mess sub max count
#define FEM_MB_SUB_MAX_HEADLESS                       1
#define FEM_MB_SUB_MAX_START1                         1
#define FEM_MB_SUB_MAX_RESULT1                        1
#define FEM_MB_SUB_MAX_SYSTEMSETTING                  1

// member id
#define FEM_MB_MEM_ID_MAIN_CONTROLLER                 1
#define FEM_MB_MEM_ID_EXCOMM_MANAGER                  2
#define FEM_MB_MEM_ID_NET_MANAGER                     3

// pattern
#define FEM_MB_MATCH_PATTERN_NONE                     MB_MATCH_PATTERN_NONE
#define FEM_MB_MATCH_PATTERN_ALL                      MB_MATCH_PATTERN_ALL
#define FEM_MB_MATCH_PATTERN_SEND                     0x0001
#define FEM_MB_MATCH_PATTERN_REV                      0x0002

// Sys com
#define FEM_SYS_POWER_WAKEUP                          0
#define FEM_SYS_POWER_SLEEP                           1
#define FEM_SYS_POWER_SLEEP_FORCE                     2
#define FEM_SYS_POWER_RESTART                         3
#define FEM_SYS_POWER_RESTART_FORCE                   4
#define FEM_SYS_POWER_SHUTDOWN                        5
#define FEM_SYS_POWER_SHUTDOWN_FORCE                  6

#define FEM_SYS_POWER_DELAY                           1000 //sc
/////////////////////////////////////////////////
// MARCO


/////////////////////////////////////////////////
// GLOBAL VARIABLES

/////////////////////////////////////////////////
// GLOBAL FUNCTIONS

/////////////////////////////////////////////////
// DATA TYPE (TYPEDEF)
typedef uint8_t                                       SysMode_t;
/////////////////////////////////////////////////
// DATA TYPE (ENUM)
//typedef enum _systemMode
//{
//  ResetMode = 0,
//  SettingMode,
//  NormalMode,
//  SystemModeMax
//} SystemMode;
/////////////////////////////////////////////////
// DATA TYPE (STRUCT)

/*For event*/



typedef struct
{
  byte count;
  byte id;
  unsigned long time;
} EventInputReceivedTAG;

/////////////////////////////////////////////////
// STATIC DATA
static const char* g_FEM_Db_Tbl_Names[] = {
                            "TBL_CF_SYS",
                            "TBL_CF_SCAN"
};

static const char* g_FEM_Db_Tbl_Scripts[] = {
                            FEM_DB_TBL_ID_SCR_PATH_CF_SYS,
                            FEM_DB_TBL_ID_SCR_PATH_CF_SCAN
};

/////////////////////////////////////////////////
// STATIC FUNCTIONS

/////////////////////////////////////////////////
// EXTERN

/////////////////////////////////////////////////
// CLASS DEFINITION
//
///*FEMConstant*/
//class FEMConstant
//{
//public:
//  FEMConstant(void) {};
//  // WARNING: if inherite from this class, deconstructor must be virtual
//  __ATTRIBUTE_VIRTUAL_OPTIMIZED ~FEMConstant(void) {};
//
//  static MBMessId_t MBC2MBMessId(MbcMessageId_t mbcMessId)
//  {
//      MBMessId_t mbMessId = MB_MESS_ID_INVALID;
//      switch (mbcMessId)
//      {
//        case (int)CommMBCConstant::CommMBCMessageId::CommMBCHeadLess:
//          mbMessId = FEM_MB_MESS_ID_HEADLESS;
//          break;
//        case (int)CommMBCConstant::CommMBCMessageId::CommMBCStart1:
//          mbMessId = FEM_MB_MESS_ID_START1;
//          break;
//        case (int)CommMBCConstant::CommMBCMessageId::CommMBCResult1:
//          mbMessId = FEM_MB_MESS_ID_RESULT1;
//          break;
//        case (int)CommMBCConstant::CommMBCMessageId::CommMBCSystemSetting:
//          mbMessId = FEM_MB_MESS_ID_SYSTEMSETTING;
//          break;
//        default:
//          break;
//      }
//      return mbMessId;
//  }
//
//  static MbcMessageId_t MB2MBCMessId(MBMessId_t mbMessId)
//  {
//    MbcMessageId_t mbcMessId= CommMBCConstant::CommMBCMessageId::CommMBCMessageIdInvalid;
//    switch (mbMessId)
//    {
//      case FEM_MB_MESS_ID_HEADLESS:
//        mbcMessId = (int)CommMBCConstant::CommMBCMessageId::CommMBCHeadLess;
//        break;
//      case FEM_MB_MESS_ID_START1:
//        mbcMessId = (int)CommMBCConstant::CommMBCMessageId::CommMBCStart1;
//        break;
//      case FEM_MB_MESS_ID_RESULT1:
//        mbcMessId = (int)CommMBCConstant::CommMBCMessageId::CommMBCResult1;
//        break;
//      case FEM_MB_MESS_ID_SYSTEMSETTING:
//        mbcMessId = (int)CommMBCConstant::CommMBCMessageId::CommMBCSystemSetting;
//        break;
//      default:
//        break;
//    }
//    return mbcMessId;
//  }
//};






#endif // _FLASH_EYE_MAIN_CONSTANT_H