#ifndef _SETTING_MANAGER_CONSTANT_H
#define _SETTING_MANAGER_CONSTANT_H

#include "../../FlashEyesMain.h"
#if (_CONF_SETTING_MANAGER_CONSTANT_ENABLED)

/////////////////////////////////////////////////
// INCLUDE

/////////////////////////////////////////////////
// PREPROCESSOR

/////////////////////////////////////////////////
// DEFINE
//#########################common###################
//#define FS_MGR_BASE_PATH_LEN_MAX                      8
#define FEM_SETTING_ITEM_DB_ID_INVALID                  255
#define FEM_SETTING_ITEM_DB_VAL_COLUMN_NAME             "Value"
#define FEM_SETTING_ITEM_DB_ID_COLUMN_NAME              "Id"
#define FEM_SETTING_ITEM_LOCAL_SETTING_ID_INVALID       255
//#########################scanner##################
//-------------------db table id------------------------
#define FEM_SET_DB_TBL_ID_SCANNER                       FEM_DB_TBL_ID_SCANNER_SETTING
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

//-------------------data len---------------------------
#define FEM_SET_DB_STR_LEN_SCANNER_CODE_PREFIX_MOD      8
#define FEM_SET_DB_STR_LEN_SCANNER_CODE_SUFFIX_MOD      8

/////////////////////////////////////////////////
// MARCO

/////////////////////////////////////////////////
// DATA TYPE (TYPEDEF)

/////////////////////////////////////////////////
// DATA TYPE (ENUM)

/////////////////////////////////////////////////
// DATA TYPE (STRUCT)
typedef struct _settingManagerConfigTAG
{
  byte reserved : 8;
} SettingManagerConfigTAG;
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


#endif // _CONF_SETTING_MANAGER_CONSTANT_ENABLED

#endif // _SETTING_MANAGER_CONSTANT_H