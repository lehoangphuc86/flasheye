#ifndef _SYSTEM_COMMON_AVR_H
#define _SYSTEM_COMMON_AVR_H

#include "../../FlashEyesCoreLibConfig.h"
#ifdef SYSTEM_AVR_PLATFORM
/////////////////////////////////////////////////
// Preprocessor

/////////////////////////////////////////////////
// INCLUDE
// AVR arduino
#if (_CONF_FREE_RTOS_ENABLED)
#include "Arduino_FreeRTOS.h"
#include "semphr.h"
#include "queue.h"
#include "portable.h"
#endif // _CONF_FREE_RTOS_ENABLED
//#include "Arduino.h"
//#include <stdio.h>
//#include "string.h"
/////////////////////////////////////////////////
// PREPROCESSOR
#define SYSTEM_HAS_OS_SYSTEM
/////////////////////////////////////////////////
// DEFINE

/////////////////////////////////////////////////
// MARCO

#define SYSTEM_CONSOLE_ISR(buf, bufLen, format, ...)        \
do {                                                        \
  snprintf(buf, bufLen, format, __VA_ARGS__);               \
  SYSTEM_CONSOLE_HANDLER.write(buf);                        \
  SYSTEM_CONSOLE_HANDLER.write("\r\n");                     \
} while(0)                                  

#if (_CONF_FREE_RTOS_ENABLED)
#define SYSTEM_YEILD_FROM_ISR(woken)                      \
do {                                                      \
    if (woken!= false)                                    \
    {                                                     \
      portYIELD_FROM_ISR();                               \
    }                                                     \
} while (0)

#else // _CONF_FREE_RTOS_ENABLED
#undef SYSTEM_HAS_OS_SYSTEM
#define SYSTEM_YEILD_FROM_ISR(woken)
#endif // _CONF_FREE_RTOS_ENABLED

//ARDUINO_ARCH_AVR
#define ANALOG_WRITE(channel, value)                      analogWrite(channel, value)
#define SYSTEM_IRAM_ATTR                                  //nothing
#define SYSTEM_TIME_BASE_FREQ                             F_CPU
#define SYSTEM_REBOOT()                                   \
({                                                        \
  wdt_disable();                                          \
  wdt_enable(WDTO_15MS);                                  \
  while (1) {}()                                          \
})


/////////////////////////////////////////////////
// GLOBAL VARIABLES

/////////////////////////////////////////////////
// GLOBAL FUNCTIONS

/////////////////////////////////////////////////
// DATA TYPE (TYPEDEF)
// 

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

#endif // SYSTEM_AVR_PLATFORM
#endif // _SYSTEM_COMMON_AVR_H