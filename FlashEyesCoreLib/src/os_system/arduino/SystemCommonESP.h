#ifndef _SYSTEM_COMMON_ESP_H
#define _SYSTEM_COMMON_ESP_H

#include "../../FlashEyesCoreLibConfig.h"
#ifdef SYSTEM_ESP_PLATFORM
/////////////////////////////////////////////////
// Preprocessor

/////////////////////////////////////////////////
// INCLUDE
// esp arduino
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"
#include "freertos/portable.h"

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
// SYSTEM_CONSOLE()

#define SYSTEM_YEILD_FROM_ISR(woken)                     \
do {                                                     \
    if (woken!= false)                                   \
    {                                                    \
      portYIELD_FROM_ISR();                              \
    }                                                    \
} while (0)


#define ANALOG_WRITE_GENERIC(channel, value, valueMax)    \
do {                                                      \
  uint32_t minValue = value;                              \
  if (minValue<valueMax)                                  \
  {                                                       \
    minValue = valueMax;                                  \
  }                                                       \
  uint32_t duty = (8191 / valueMax) * minValue;           \
  ledcWrite(channel, duty);                               \
} while(0)

#define ANALOG_WRITE(channel, value)                      ANALOG_WRITE_GENERIC(channel, value , 255)
#define SYSTEM_IRAM_ATTR                                  IRAM_ATTR

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

#endif // SYSTEM_ESP_PLATFORM
#endif // _SYSTEM_COMMON_ESP_H