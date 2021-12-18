#include "SystemCommon.h"

#if (_CONF_SYSTEM_CRITICAL_SESSION_ENABLED)
#ifndef _SYSTEM_CRITICAL_SESSION_H
#define _SYSTEM_CRITICAL_SESSION_H

/////////////////////////////////////////////////
// INCLUDE
#include "SystemCommon.h"

/////////////////////////////////////////////////
// PREPROCESSOR

/////////////////////////////////////////////////
// DEFINE

/////////////////////////////////////////////////
// MARCO
#ifdef SYSTEM_ARDUINO_BASED

#ifdef SYSTEM_ESP_PLATFORM

//ARDUINO_ARCH_ESP
#define SYSTEM_CRITICAL_DATA_TYPE                         portMUX_TYPE
#define SYSTEM_CRITICAL_SECTION_MUTEX_INIT(mutex)        \
do {                                                     \
   mutex = portMUX_INITIALIZER_UNLOCKED;                 \
} while(0)                                  

#define SYSTEM_CRITICAL_SECTION_BEGIN(mutex)             \
do {                                                     \
   portENTER_CRITICAL_SAFE(&mutex);                      \
} while(0)                                  

#define SYSTEM_CRITICAL_SECTION_END(mutex)               \
do {                                                     \
   portEXIT_CRITICAL_SAFE(&mutex);                       \
} while(0)                                  

// in ESP32 it auto detect fromISR.
#define SYSTEM_CRITICAL_SECTION_BEGIN_FROM_ISR(mutex)    \
do {                                                     \
    SYSTEM_CRITICAL_SECTION_BEGIN(mutex);                \
} while(0)                                  

// in ESP32 it auto detect fromISR.
#define SYSTEM_CRITICAL_SECTION_END_FROM_ISR(mutex)      \
do {                                                     \
   SYSTEM_CRITICAL_SECTION_END(mutex);                   \
} while(0)                                  
#else // SYSTEM_ESP_PLATFORM
//ARDUINO_ARCH_AVR
#if  (_CONF_FREE_RTOS_ENABLED)
// has OS
#define SYSTEM_CRITICAL_DATA_TYPE                         UBaseType_t
#define SYSTEM_ENTER_CRITICAL(mutex)                      \
do                                                        \
{                                                         \
  mutex = SREG;                                           \
  cli();                                                  \
} while (0);

#define SYSTEM_EXIT_CRITICAL(mutex)                       \
do                                                        \
{                                                         \
  SREG = mutex;                                           \
} while (0);

#define SYSTEM_ENTER_CRITICAL_FROM_ISR(mutex)             \
do {                                                      \
    mutex = portSET_INTERRUPT_MASK_FROM_ISR();            \
} while (0)
#define SYSTEM_EXIT_CRITICAL_FROM_ISR(mutex)              portCLEAR_INTERRUPT_MASK_FROM_ISR(mutex)

#else // _CONF_FREE_RTOS_ENABLED
// no OS
#define SYSTEM_CRITICAL_DATA_TYPE                         uint8_t
#define SYSTEM_ENTER_CRITICAL(mutex)                      noInterrupts()
#define SYSTEM_EXIT_CRITICAL(mutex)                       interrupts()
#define SYSTEM_ENTER_CRITICAL_FROM_ISR(mutex)
#define SYSTEM_EXIT_CRITICAL_FROM_ISR(mutex)
#endif // _CONF_FREE_RTOS_ENABLED

#define SYSTEM_CRITICAL_SECTION_MUTEX_INIT(mutex)        \
do {                                                     \
   mutex = 0;                                            \
} while(0)                                  

#define SYSTEM_CRITICAL_SECTION_BEGIN(mutex)              SYSTEM_ENTER_CRITICAL(mutex)
#define SYSTEM_CRITICAL_SECTION_END(mutex)                SYSTEM_EXIT_CRITICAL(mutex)
#define SYSTEM_CRITICAL_SECTION_BEGIN_FROM_ISR(mutex)     SYSTEM_ENTER_CRITICAL_FROM_ISR(mutex)
#define SYSTEM_CRITICAL_SECTION_END_FROM_ISR(mutex)       SYSTEM_EXIT_CRITICAL_FROM_ISR(mutex)

#endif // SYSTEM_ESP_PLATFORM

#else // SYSTEM_ARDUINO_BASED
// SYSTEM_PC_BASED
#define SYSTEM_CRITICAL_DATA_TYPE                         std::timed_mutex
#define SYSTEM_ENTER_CRITICAL(mutex)                      \
do {                                                      \
    mutex.lock();                                         \
} while (0)

#define SYSTEM_EXIT_CRITICAL(mutex)                       \
do {                                                      \
    mutex.unlock();                                       \
} while (0)

#define SYSTEM_ENTER_CRITICAL_FROM_ISR(mutex)             SYSTEM_ENTER_CRITICAL(mutex)
#define SYSTEM_EXIT_CRITICAL_FROM_ISR(mutex)              SYSTEM_EXIT_CRITICAL(mutex)
#define SYSTEM_CRITICAL_SECTION_MUTEX_INIT(mutex)
#define SYSTEM_CRITICAL_SECTION_BEGIN(mutex)              SYSTEM_ENTER_CRITICAL(mutex)
#define SYSTEM_CRITICAL_SECTION_END(mutex)                SYSTEM_EXIT_CRITICAL(mutex)
#define SYSTEM_CRITICAL_SECTION_BEGIN_FROM_ISR(mutex)     SYSTEM_ENTER_CRITICAL_FROM_ISR(mutex)
#define SYSTEM_CRITICAL_SECTION_END_FROM_ISR(mutex)       SYSTEM_EXIT_CRITICAL_FROM_ISR(mutex)

#endif // SYSTEM_ARDUINO_BASED
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

/////////////////////////////////////////////////
// EXTERN

/////////////////////////////////////////////////
// CLASS DEFINITION
class SystemCriticalSession
{
public:
  SystemCriticalSession(void);
  // WARNING: if inherite from this class, deconstructor must be virtual
  __ATTRIBUTE_VIRTUAL_OPTIMIZED ~SystemCriticalSession(void);
  void                                                          begin(void);
  void                                                          end(void);
  void                                                          beginFromISR(void);
  void                                                          endFromISR(void);
protected:
  SYSTEM_CRITICAL_DATA_TYPE                                     critical_Mutex;
};


// lock critical session
class SystemCriticalLocker
{
public:
  // WARNING: if inherite from this class, deconstructor must be virtual
  SystemCriticalLocker(SystemCriticalSession& criticalLocker);
  __ATTRIBUTE_VIRTUAL_OPTIMIZED ~SystemCriticalLocker(void);

protected:
  SystemCriticalSession*                                        critical_Locker;
};

// lock critical session
class SystemCriticalLockerFromISR
{
public:
  SystemCriticalLockerFromISR(SystemCriticalSession& criticalLocker);
  // WARNING: if inherite from this class, deconstructor must be virtual
  __ATTRIBUTE_VIRTUAL_OPTIMIZED ~SystemCriticalLockerFromISR(void);

protected:
  SystemCriticalSession*                                        critical_Locker;
};

#endif // _SYSTEM_CRITICAL_SESSION_H

#endif // _CONF_SYSTEM_CRITICAL_SESSION_ENABLED