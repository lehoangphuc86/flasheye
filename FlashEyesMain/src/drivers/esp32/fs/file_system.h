#ifndef _DRIVER_FILE_SYSTEM_ESP32_H
#define _DRIVER_FILE_SYSTEM_ESP32_H

/////////////////////////////////////////////////
// INCLUDE
//#include "../../../libs/SPIFFS/SPIFFS.h"
#include "esp_spiffs.h"
#include "esp_vfs.h"
/////////////////////////////////////////////////
// PREPROCESSOR

/////////////////////////////////////////////////
// DEFINE
//#define FS_FILE_READ                                ""
//#define FS_FILE_WRITE                               FILE_WRITE
//#define FS_FILE_APPEND                              FILE_APPEND
#define FS_FILE_HANDLER_INVALID                     NULL
#define FS_FILE_PATH_MAX_LEN                        128

#define FS_HANDLER_INVALID                          NULL
#define FS_RET_OK                                   ESP_OK
#define FS_RET_ERR                                  ESP_FAIL
/////////////////////////////////////////////////
// MARCO

/////////////////////////////////////////////////
// GLOBAL VARIABLES

/////////////////////////////////////////////////
// GLOBAL FUNCTIONS

/////////////////////////////////////////////////
// DATA TYPE (TYPEDEF)
typedef FILE*                                       FileHandler_t;
typedef esp_err_t                                   FsRet_t;
typedef mode_t                                      FileMode_t;
/////////////////////////////////////////////////
// DATA TYPE (ENUM)

/////////////////////////////////////////////////
// DATA TYPE (STRUCT)

/////////////////////////////////////////////////
// STATIC DATA

/////////////////////////////////////////////////
// STATIC FUNCTIONS

/////////////////////////////////////////////////
// EXTERN

/////////////////////////////////////////////////
// CLASS DEFINITION


#endif // _DRIVER_FILE_SYSTEM_ESP32_H