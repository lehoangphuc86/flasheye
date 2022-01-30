#ifndef _SETTING_MANAGER_CONSTANT_H
#define _SETTING_MANAGER_CONSTANT_H

#include "../../FlashEyesMain.h"
#if (_CONF_SETTING_MANAGER_CONSTANT_ENABLED)
#include "SettingDefinition.h"
#include "task_comm_mbc/CommMBCConstant.h"
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
#define FEM_SETTING_ITEM_DB_DATA_TYPE_COLUMN_NAME       "Type"
#define FEM_SETTING_ITEM_DB_ID_COLUMN_NAME              "Id"
#define FEM_SETTING_ITEM_LOCAL_SETTING_ID_INVALID       255

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

typedef _commMBCSystemSettingTag SettingParamTAG;
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