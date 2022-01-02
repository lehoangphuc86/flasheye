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
//#define FS_MGR_BASE_PATH_LEN_MAX                  8
#define FEM_SETTING_ITEM_DB_ID_INVALID              255
#define FEM_SETTING_ITEM_DB_VAL_COLUMN_NAME         "Value"
#define FEM_SETTING_ITEM_DB_ID_COLUMN_NAME          "id"

/*****************Scanner********************/
#define FEM_SET_SCANNER_DB_TABLE_ID                 FEM_DB_TBL_CONFIG_ID
// scan mode
#define FEM_SET_SCANNER_DB_SCANMODE_KEY_ID          1 // @@
// code prefix
#define FEM_SET_SCANNER_DB_CODE_PREFIX_KEY_ID       2 // @@
#define FEM_SET_SCANNER_DB_CODE_PREFIX_LEN_MAX      8 // @@

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