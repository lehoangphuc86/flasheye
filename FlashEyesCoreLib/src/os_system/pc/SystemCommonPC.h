#ifndef _SYSTEM_COMMON_PC_H
#define _SYSTEM_COMMON_PC_H

#include "../../FlashEyesCoreLibConfig.h"
#ifdef SYSTEM_PC_BASED
/////////////////////////////////////////////////
// Preprocessor

/////////////////////////////////////////////////
// INCLUDE
#include <stdio.h>
#include "string.h"
#include <chrono>

#if defined(SYSTEM_WIN_PLATFORM)
#include "SystemCommonWin.h"
#elif defined(SYSTEM_LINUX_PLATFORM)
#include "SystemCommonLinux.h"
#else // unsupported 
#error Not supported platform
#endif // SYSTEM_WIN_PLATFORM

/////////////////////////////////////////////////
// PREPROCESSOR

/////////////////////////////////////////////////
// DEFINE
#define SYSTEM_HAS_OS_SYSTEM
/////////////////////////////////////////////////
// MARCO

// for PC based, it is no needed.
#define __ATTRIBUTE_VIRTUAL_OPTIMIZED                     virtual
#if (_CONF_SYSTEM_CONSOLE_LOG_ENABLED)
// F()
#define F(stringLiteral)                                  (stringLiteral)
// SYSTEM_CONSOLE()
#define SYSTEM_CONSOLE(format, ...)                       \
do {                                                      \
  printf(format, __VA_ARGS__);                            \
  printf("\n");                                           \
} while(0)                                  

#define SYSTEM_CONSOLE_BUF(buf, bufLen, format, ...)      SYSTEM_CONSOLE(format, __VA_ARGS__)
#define SYSTEM_CONSOLE_ISR(buf, bufLen, format, ...)      SYSTEM_CONSOLE(format, __VA_ARGS__)
#else // _CONF_SYSTEM_CONSOLE_LOG_ENABLED
#define SYSTEM_CONSOLE(format, ...)
#define SYSTEM_CONSOLE_BUF(buf, bufLen, format, ...)
#define SYSTEM_CONSOLE_ISR(buf, bufLen, format, ...)
#endif // _CONF_SYSTEM_CONSOLE_LOG_ENABLED

// SYSTEM CLOCK() 
#define SYSTEM_TIME_MILLIS()                              std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count() // current time in millis
#define SYSTEM_TIME_MICROS()                              std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count() // current time in micros
#define SYSTEM_TIME_NANOS()                               std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count() // current time in micros
#define SYSTEM_IS_OS_RUNNING()                            true
#define SYSTEM_YEILD_FROM_ISR(woken)
#define SYSTEM_IRAM_ATTR                                  
#define SYSTEM_TASK_MEM_DATA_TYPE                         uint8_t
#define SYSTEM_TASK_MEM_USAGE(taskHandler)                0

// system hardware
#define PIN_NUMBER_MAX                                    255
#define ANALOG_WRITE(channel, value)                      
#define PIN_MODE(pin, mode)                               
#define F_CPU                                             20000000 // CPU speed. dummy
/////////////////////////////////////////////////
// GLOBAL VARIABLES

/////////////////////////////////////////////////
// GLOBAL FUNCTIONS

/////////////////////////////////////////////////
// DATA TYPE (TYPEDEF)
typedef uint16_t                                          DataCount_t;
typedef uint16_t                                          DataSize_t;
typedef uint16_t                                          EventId_t;
typedef uint16_t                                          EventCount_t;
typedef uint16_t                                          EventSize_t;
typedef uint16_t                                          TaskId_t;
typedef unsigned short                                    TaskSize_t;
typedef DataSize_t                                        RankingScore_t;
typedef byte                                              PoolId_t;
typedef uint16_t                                          ClientId_t;
typedef ClientId_t                                        ClientCount_t;
typedef byte                                              TimerId_t;
typedef uint64_t                                          TimePoint_t;
typedef uint64_t                                          TimerCount_t;
typedef uint16_t                                          TimerReg_t;
typedef uint32_t                                          TimerOVFCount_t;
typedef uint8_t                                           MessageId_t;
typedef uint16_t                                          HttpReqId_t;
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

#endif // SYSTEM_PC_BASED
#endif // _SYSTEM_COMMON_PC_H