#ifndef _SETTING_DEFINITION_H
#define _SETTING_DEFINITION_H

#include "../../FlashEyesMain.h"
#if (_CONF_SETTING_DEFINITION_ENABLED)

/////////////////////////////////////////////////
// INCLUDE

/////////////////////////////////////////////////
// PREPROCESSOR

/////////////////////////////////////////////////
// DEFINE
//#########################Setting section id##################
#define FEM_SET_SECTION_ID_SYSTEM                       0
#define FEM_SET_SECTION_ID_SCANNER                      1
#define FEM_SET_SECTION_ID_MAX                          2
// 
//#########################scanner##################
//-------------------db table id------------------------
#define FEM_SET_DB_TBL_ID_SCANNER                       FEM_DB_TBL_ID_CF_SCAN
//-------------------db key id--------------------------
#define FEM_SET_DB_KEY_ID_SCANNER_COM_BAUDRATE          1
#define FEM_SET_DB_KEY_ID_SCANNER_COM_PARITY            2
#define FEM_SET_DB_KEY_ID_SCANNER_COM_STOPBIT           3
#define FEM_SET_DB_KEY_ID_SCANNER_COM_DATABIT           4
#define FEM_SET_DB_KEY_ID_SCANNER_COM_FLOW_CTRL         5
#define FEM_SET_DB_KEY_ID_SCANNER_CHARACTER             6
#define FEM_SET_DB_KEY_ID_SCANNER_READ_STATE            7
#define FEM_SET_DB_KEY_ID_SCANNER_AUTO_SCAN             8
#define FEM_SET_DB_KEY_ID_SCANNER_CONT_SCAN             9
#define FEM_SET_DB_KEY_ID_SCANNER_SECURITY_LEVEL        10
#define FEM_SET_DB_KEY_ID_SCANNER_CODE_PREFIX           11
#define FEM_SET_DB_KEY_ID_SCANNER_CODE_PREFIX_MOD       12
#define FEM_SET_DB_KEY_ID_SCANNER_CODE_SUFFIX_MOD       13
#define FEM_SET_DB_KEY_ID_SCANNER_CODE_CUS_FORMAT       14
#define FEM_SET_DB_KEY_ID_SCANNER_MESS_TER              15
#define FEM_SET_DB_KEY_ID_SCANNER_TRIGGER_MODE          16
#define FEM_SET_DB_KEY_ID_DIST_SS_RANGE_BEGIN           17
#define FEM_SET_DB_KEY_ID_DIST_SS_RANGE_END             18

//-------------------data len---------------------------
#define FEM_SET_DB_STR_LEN_SCANNER_CODE_PREFIX_MOD      8
#define FEM_SET_DB_STR_LEN_SCANNER_CODE_SUFFIX_MOD      8
#define FEM_SET_DB_STR_LEN_SYS_WIFI_SSID                32
#define FEM_SET_DB_STR_LEN_SYS_WIFI_PWD                 32
#define FEM_SET_DB_STR_LEN_HTTP_CLI_URI                 128

//#########################system##################
//-------------------db table id------------------------
#define FEM_SET_DB_TBL_ID_SYSTEM                        FEM_DB_TBL_ID_CF_SYS
//-------------------db key id--------------------------
#define FEM_SET_DB_KEY_ID_SYS_LAST_SYS_MODE             1
#define FEM_SET_DB_KEY_ID_SYS_WIFI_MODE                 2
#define FEM_SET_DB_KEY_ID_SYS_WIFI_SSID                 3
#define FEM_SET_DB_KEY_ID_SYS_WIFI_PWD                  4
#define FEM_SET_DB_KEY_ID_SYS_HTTP_CLI_URI              5
#define FEM_SET_DB_KEY_ID_SYS_HTTP_CLI_DATA_TYPE        6
#define FEM_SET_DB_KEY_ID_SYS_HTTP_CLI_METHOD           7
#define FEM_SET_DB_KEY_ID_SYS_EXPORT_MODE               8

/////////////////////////////////////////////////
// MARCO

/////////////////////////////////////////////////
// DATA TYPE (TYPEDEF)

/////////////////////////////////////////////////
// DATA TYPE (ENUM)

/////////////////////////////////////////////////
// DATA TYPE (STRUCT)

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


#endif // _CONF_SETTING_DEFINITION_ENABLED

#endif // _SETTING_DEFINITION_H