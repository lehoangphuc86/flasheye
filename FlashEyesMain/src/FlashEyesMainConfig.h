#ifndef _FLASH_EYES_MAIN_CONFIG_H
#define _FLASH_EYES_MAIN_CONFIG_H

/////////////////////////////////////////////////
// INCLUDE
// 
/////////////////////////////////////////////////
// PREPROCESSOR
// 
//################DEFINE SYSTEM TYPE#######################
#define __CONF_FILE_SYSTEM_MANAGER_FUNC_ENABLED
#define __CONF_DB_MANAGER_FUNC_ENABLED
#define __CONF_NET_MANAGER_FUNC_ENABLED
#define __CONF_WIFI_MANAGER_FUNC_ENABLED
#define __CONF_COMM_HTTP_FUNC_ENABLED
#define __CONF_COMM_MBC_HTTP_FUNC_ENABLED

// file system
#ifdef __CONF_FILE_SYSTEM_MANAGER_FUNC_ENABLED
#define CONF_FILE_SYSTEM_MANAGER_FUNC_ENABLED_VAL         1
#else // __CONF_FILE_SYSTEM_MANAGER_FUNC_ENABLED
#define CONF_FILE_SYSTEM_MANAGER_FUNC_ENABLED_VAL         0
#endif // __CONF_FILE_SYSTEM_MANAGER_FUNC_ENABLED

#define _CONF_FILE_SYSTEM_MANAGER_CONSTANT_ENABLED        CONF_FILE_SYSTEM_MANAGER_FUNC_ENABLED_VAL
#define _CONF_FILE_SYSTEM_MANAGER_ENABLED                 CONF_FILE_SYSTEM_MANAGER_FUNC_ENABLED_VAL


// db
#ifdef __CONF_DB_MANAGER_FUNC_ENABLED
#define CONF_DB_MANAGER_FUNC_ENABLED_VAL                  1
#else // __CONF_DB_MANAGER_FUNC_ENABLED
#define CONF_DB_MANAGER_FUNC_ENABLED_VAL                  0
#endif // __CONF_DB_MANAGER_FUNC_ENABLED

#define _CONF_DB_MANAGER_CONSTANT_ENABLED                 CONF_DB_MANAGER_FUNC_ENABLED_VAL
#define _CONF_DB_MANAGER_ENABLED                          CONF_DB_MANAGER_FUNC_ENABLED_VAL


// net
#ifdef __CONF_NET_MANAGER_FUNC_ENABLED
#define CONF_NET_MANAGER_FUNC_ENABLED_VAL                 1
#else // __CONF_NET_MANAGER_FUNC_ENABLED
#define CONF_NET_MANAGER_FUNC_ENABLED_VAL                 0
#endif // __CONF_NET_MANAGER_FUNC_ENABLED

#define _CONF_NET_MANAGER_CONSTANT_ENABLED               CONF_NET_MANAGER_FUNC_ENABLED_VAL

// wifi
#ifdef __CONF_WIFI_MANAGER_FUNC_ENABLED
#define CONF_WIFI_MANAGER_FUNC_ENABLED_VAL                1
#else // __CONF_WIFI_MANAGER_FUNC_ENABLED
#define CONF_WIFI_MANAGER_FUNC_ENABLED_VAL                0
#endif // __CONF_WIFI_MANAGER_FUNC_ENABLED

#define _CONF_WIFI_MANAGER_CONSTANT_ENABLED               CONF_WIFI_MANAGER_FUNC_ENABLED_VAL
#define _CONF_WIFI_CONTROLLER_TASK_ENABLED                CONF_WIFI_MANAGER_FUNC_ENABLED_VAL

// http
#ifdef __CONF_COMM_HTTP_FUNC_ENABLED
#define CONF_COMM_HTTP_FUNC_ENABLED_VAL                   1
#else // __CONF_COMM_HTTP_FUNC_ENABLED
#define CONF_COMM_HTTP_FUNC_ENABLED_VAL                   0
#endif // __CONF_COMM_HTTP_FUNC_ENABLED

#define _CONF_COMM_HTTP_CONSTANT_ENABLED                  CONF_COMM_HTTP_FUNC_ENABLED_VAL
#define _CONF_COMM_HTTP_ENABLED                           CONF_COMM_HTTP_FUNC_ENABLED_VAL
#define _CONF_COMM_HTTP_CLIENT_ENABLED                    CONF_COMM_HTTP_FUNC_ENABLED_VAL
#define _CONF_COMM_HTTP_SERVER_ENABLED                    CONF_COMM_HTTP_FUNC_ENABLED_VAL

// mbc http
#ifdef __CONF_COMM_MBC_HTTP_FUNC_ENABLED
#define CONF_COMM_MBC_HTTP_FUNC_ENABLED_VAL               1
#else // __CONF_COMM_MBC_HTTP_FUNC_ENABLED
#define CONF_COMM_MBC_HTTP_FUNC_ENABLED_VAL               0
#endif // __CONF_COMM_MBC_HTTP_FUNC_ENABLED

#define _CONF_COMM_MBC_HTTP_CONSTANT_ENABLED              CONF_COMM_MBC_HTTP_FUNC_ENABLED_VAL
#define _CONF_COMM_MBC_HTTP_ENABLED                       CONF_COMM_MBC_HTTP_FUNC_ENABLED_VAL
#define _CONF_COMM_MBC_HTTP_CLIENT_ENABLED                CONF_COMM_MBC_HTTP_FUNC_ENABLED_VAL
#define _CONF_COMM_MBC_HTTP_SERVER_ENABLED                CONF_COMM_MBC_HTTP_FUNC_ENABLED_VAL

/////////////////////////////////////////////////
// DEFINE

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
//

/////////////////////////////////////////////////
// EXTERN

/////////////////////////////////////////////////
// CLASS DEFINITION



#endif // _FLASH_EYES_MAIN_CONFIG_H