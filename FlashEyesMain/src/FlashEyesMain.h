#ifndef _FLASH_EYE_MAIN_CONSTANT_H
#define _FLASH_EYE_MAIN_CONSTANT_H

/////////////////////////////////////////////////
// INCLUDE
#include "FlashEyesCoreLib.h"
#include "FlashEyesMainConfig.h"
#include "source/task_http/CommHttpConstant.h"
#include "timer_manager/TimerManagerConstant.h"
/////////////////////////////////////////////////
// PREPROCESSOR

/////////////////////////////////////////////////
// DEFINE
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


// Display
#define FLASH_EYE_DISPLAY_LCD_COLUMN_NO               16
#define FLASH_EYE_DISPLAY_LCD_ROW_NO                  2
#define FLASH_EYE_DISPLAY_LCD_PIN_V0                  10 //DISPLAY_DEVICE_PIN_INVALID // pwm
#define FLASH_EYE_DISPLAY_LCD_PIN_RS                  23
#define FLASH_EYE_DISPLAY_LCD_PIN_RW                  25
#define FLASH_EYE_DISPLAY_LCD_PIN_E                   27
#define FLASH_EYE_DISPLAY_LCD_PIN_D4                  29
#define FLASH_EYE_DISPLAY_LCD_PIN_D5                  31
#define FLASH_EYE_DISPLAY_LCD_PIN_D6                  33
#define FLASH_EYE_DISPLAY_LCD_PIN_D7                  35
#define FLASH_EYE_DISPLAY_LCD_PIN_A                   9   // pwm
#define FLASH_EYE_DISPLAY_LCD_INIT_BACKLIGHT          250
#define FLASH_EYE_DISPLAY_LCD_INIT_BRIGHTNESS         0

// Distance sensor
#define FLASH_EYE_DISTANCE_SENSOR_PIN_TRIGGER         47
#define FLASH_EYE_DISTANCE_SENSOR_PIN_ECHO            45


// Comm
// task
#define FEM_TASK_PRIORITY_DEFAULT                     3


// file system
#define FEM_FS_FORMAT                                 1
#define FEM_FS_BASE_PATH                              "/spiffs" // must not empty
#define FEM_FS_PARTITION_LABEL                        NULL
#define FEM_FS_MAX_FILE                               5

// DB
#define FEM_DB_PATH                                   "/spiffs/db/FEM_DB_01_01.db"

#define FEM_DB_TBL_CONFIG_SCRIPT_PATH                 "/spiffs/db/sc/DB_INIT_TBL_CONFIG.sql"

// Wifi
#define FEM_WIFI_SERVER_TASK_MEM                      2048
#define FEM_WIFI_SERVER_TASK_PRIORITY                 FEM_TASK_PRIORITY_DEFAULT
#define FEM_WIFI_SERVER_TASK_EVENT_NUM                5

// Http server
#define FEM_HTTP_SERVER_IS_HEADER_LESS                0
#define FEM_HTTP_SERVER_TASK_MEM                      4096
#define FEM_HTTP_SERVER_TASK_PRIORITY                 FEM_TASK_PRIORITY_DEFAULT
#define FEM_HTTP_SERVER_TASK_EVENT_NUM                5
#define FEM_HTTP_SERVER_BUFF_COUNT                    6
#define FEM_HTTP_SERVER_BUFF_SIZE                     200


#define FEM_HTTP_SERVER_BASE_PATH                     "/spiffs/web/public"
#define FEM_HTTP_SERVER_PORT                          80
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
#define FEM_HTTP_CLIENT_IS_HEADER_LESS                0
#define FEM_HTTP_CLIENT_TASK_MEM                      4096
#define FEM_HTTP_CLIENT_TASK_PRIORITY                 FEM_TASK_PRIORITY_DEFAULT
#define FEM_HTTP_CLIENT_TASK_EVENT_NUM                5
#define FEM_HTTP_CLIENT_BUFF_COUNT                    5
#define FEM_HTTP_CLIENT_BUFF_SIZE                     200

#define FEM_HTTP_CLIENT_PATH                          "http://192.168.137.234/api/v1/system/info"
#define FEM_HTTP_CLIENT_REQ_URI                       "http://192.168.137.234/api/v1/system/update"
#define FEM_HTTP_CLIENT_REQ_GET_URI                   "https://gorest.co.in/public/v1/users/123/todos"

#define FEM_HTTP_CLIENT_REQ_CERT                      NULL
#define FEM_HTTP_CLIENT_REQ_CERT_GOREST               "-----BEGIN CERTIFICATE-----\n\
MIIFYDCCBEigAwIBAgIQQAF3ITfU6UK47naqPGQKtzANBgkqhkiG9w0BAQsFADA/\n\
MSQwIgYDVQQKExtEaWdpdGFsIFNpZ25hdHVyZSBUcnVzdCBDby4xFzAVBgNVBAMT\n\
DkRTVCBSb290IENBIFgzMB4XDTIxMDEyMDE5MTQwM1oXDTI0MDkzMDE4MTQwM1ow\n\
TzELMAkGA1UEBhMCVVMxKTAnBgNVBAoTIEludGVybmV0IFNlY3VyaXR5IFJlc2Vh\n\
cmNoIEdyb3VwMRUwEwYDVQQDEwxJU1JHIFJvb3QgWDEwggIiMA0GCSqGSIb3DQEB\n\
AQUAA4ICDwAwggIKAoICAQCt6CRz9BQ385ueK1coHIe+3LffOJCMbjzmV6B493XC\n\
ov71am72AE8o295ohmxEk7axY/0UEmu/H9LqMZshftEzPLpI9d1537O4/xLxIZpL\n\
wYqGcWlKZmZsj348cL+tKSIG8+TA5oCu4kuPt5l+lAOf00eXfJlII1PoOK5PCm+D\n\
LtFJV4yAdLbaL9A4jXsDcCEbdfIwPPqPrt3aY6vrFk/CjhFLfs8L6P+1dy70sntK\n\
4EwSJQxwjQMpoOFTJOwT2e4ZvxCzSow/iaNhUd6shweU9GNx7C7ib1uYgeGJXDR5\n\
bHbvO5BieebbpJovJsXQEOEO3tkQjhb7t/eo98flAgeYjzYIlefiN5YNNnWe+w5y\n\
sR2bvAP5SQXYgd0FtCrWQemsAXaVCg/Y39W9Eh81LygXbNKYwagJZHduRze6zqxZ\n\
Xmidf3LWicUGQSk+WT7dJvUkyRGnWqNMQB9GoZm1pzpRboY7nn1ypxIFeFntPlF4\n\
FQsDj43QLwWyPntKHEtzBRL8xurgUBN8Q5N0s8p0544fAQjQMNRbcTa0B7rBMDBc\n\
SLeCO5imfWCKoqMpgsy6vYMEG6KDA0Gh1gXxG8K28Kh8hjtGqEgqiNx2mna/H2ql\n\
PRmP6zjzZN7IKw0KKP/32+IVQtQi0Cdd4Xn+GOdwiK1O5tmLOsbdJ1Fu/7xk9TND\n\
TwIDAQABo4IBRjCCAUIwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMCAQYw\n\
SwYIKwYBBQUHAQEEPzA9MDsGCCsGAQUFBzAChi9odHRwOi8vYXBwcy5pZGVudHJ1\n\
c3QuY29tL3Jvb3RzL2RzdHJvb3RjYXgzLnA3YzAfBgNVHSMEGDAWgBTEp7Gkeyxx\n\
+tvhS5B1/8QVYIWJEDBUBgNVHSAETTBLMAgGBmeBDAECATA/BgsrBgEEAYLfEwEB\n\
ATAwMC4GCCsGAQUFBwIBFiJodHRwOi8vY3BzLnJvb3QteDEubGV0c2VuY3J5cHQu\n\
b3JnMDwGA1UdHwQ1MDMwMaAvoC2GK2h0dHA6Ly9jcmwuaWRlbnRydXN0LmNvbS9E\n\
U1RST09UQ0FYM0NSTC5jcmwwHQYDVR0OBBYEFHm0WeZ7tuXkAXOACIjIGlj26Ztu\n\
MA0GCSqGSIb3DQEBCwUAA4IBAQAKcwBslm7/DlLQrt2M51oGrS+o44+/yQoDFVDC\n\
5WxCu2+b9LRPwkSICHXM6webFGJueN7sJ7o5XPWioW5WlHAQU7G75K/QosMrAdSW\n\
9MUgNTP52GE24HGNtLi1qoJFlcDyqSMo59ahy2cI2qBDLKobkx/J3vWraV0T9VuG\n\
WCLKTVXkcGdtwlfFRjlBz4pYg1htmf5X6DYO8A4jqv2Il9DjXA6USbW1FzXSLr9O\n\
he8Y4IWS6wY7bCkjCWDcRQJMEhg76fsO3txE+FiYruq9RUWhiF1myv4Q6W+CyBFC\n\
Dfvp7OOGAN6dEOM4+qR9sdjoSYKEBpsr6GtPAQw4dy753ec5\n\
-----END CERTIFICATE-----"


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

/*For event*/



typedef struct
{
  byte count;
  byte id;
  unsigned long time;
} EventInputReceivedTAG;

/////////////////////////////////////////////////
// STATIC DATA

/////////////////////////////////////////////////
// STATIC FUNCTIONS

/////////////////////////////////////////////////
// EXTERN

/////////////////////////////////////////////////
// CLASS DEFINITION

#endif // _FLASH_EYE_MAIN_CONSTANT_H