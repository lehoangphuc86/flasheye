#ifndef _SYSTEM_COMMON_LINUX_H
#define _SYSTEM_COMMON_LINUX_H

#include "../../FlashEyesCoreLibConfig.h"
#ifdef SYSTEM_LINUX_PLATFORM
/////////////////////////////////////////////////
// Preprocessor

/////////////////////////////////////////////////
// INCLUDE
// linux
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <mutex>
/////////////////////////////////////////////////
// PREPROCESSOR

/////////////////////////////////////////////////
// DEFINE

/////////////////////////////////////////////////
// MARCO
#define SYSTEM_SLEEP(x)                                   usleep(x*1000)
#define SYSTEM_REBOOT()                                   
/////////////////////////////////////////////////
// GLOBAL VARIABLES

/////////////////////////////////////////////////
// GLOBAL FUNCTIONS

/////////////////////////////////////////////////
// DATA TYPE (TYPEDEF)
typedef uint8_t                                           byte;
typedef timer_t                                           TimerHanlder_t;

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

#endif // SYSTEM_LINUX_PLATFORM
#endif // _SYSTEM_COMMON_LINUX_H