#include "SystemCommon.h"

#if (_CONF_SYSTEM_MUTEX_ENABLED)
#ifndef _SYSTEM_MUTEX_H
#define _SYSTEM_MUTEX_H

/////////////////////////////////////////////////
// INCLUDE
//#include "SystemCommon.h"
#ifndef SYSTEM_HAS_OS_SYSTEM
#error OS system is required
#endif // SYSTEM_HAS_OS_SYSTEM
/////////////////////////////////////////////////
// PREPROCESSOR
#ifdef SYSTEM_ARDUINO_BASED
#define SYSTEM_MUTEX_DATA_TYPE                            SemaphoreHandle_t
#else // SYSTEM_ARDUINO_BASED
// SYSTEM_PC_BASED
#define SYSTEM_MUTEX_DATA_TYPE                            std::timed_mutex
#endif // SYSTEM_ARDUINO_BASED

// mutex and event
#define SYSTEM_MUTEX_NO_WAIT_TIME                         0
#define SYSTEM_MUTEX_MAX_WAIT_TIME                        0xFFFF

#define SYSTEM_MUTEX_NO_WAIT_TIME_ACCURATE                0
#define SYSTEM_MUTEX_MAX_WAIT_TIME_ACCURATE               0xFFFFFFFF
/////////////////////////////////////////////////
// DEFINE
//#define SYSTEM_MUTEX_DEBUG_NABLE
/////////////////////////////////////////////////
// MARCO

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

/*ArduinoMutex*/
/*
* Wrap RTOS binary semaphore class
*
*/
class SystemMutex
{
public:
  SystemMutex(void);
  // WARNING: if inherite from this class, deconstructor must be virtual
  __ATTRIBUTE_VIRTUAL_OPTIMIZED ~SystemMutex(void);
  void                                                          lock(unsigned int timeToWait = SYSTEM_MUTEX_NO_WAIT_TIME); // ms
  void                                                          lockAccuracy(unsigned long timeToWait = SYSTEM_MUTEX_NO_WAIT_TIME_ACCURATE); // nano second
  void                                                          unlock(void);
  void                                                          lockFromISR(bool* higherTaskWoken);
  void                                                          lockAccuracyFromISR(bool* higherTaskWoken);
  void                                                          unlockFromISR(bool* higherTaskWoken);
  void*                                                         nativeHandlerCore(void);

protected:
  bool                                                          isValidCore(void);
  bool                                                          createMutexCore(void);
  int                                                           deleteMutexCore(void);

protected:
  SYSTEM_MUTEX_DATA_TYPE                                        mutex_Handler;
};



class SystemMutexLocker
{
public:
  SystemMutexLocker(SystemMutex& mutex);
  // WARNING: if inherite from this class, deconstructor must be virtual
  __ATTRIBUTE_VIRTUAL_OPTIMIZED ~SystemMutexLocker(void);

protected:
  SystemMutex*                                                  holding_Mutex;
};

class SystemMutexLockerFromISR
{
public:
  SystemMutexLockerFromISR(SystemMutex& mutex, bool* higherTaskWoken);
  // WARNING: if inherite from this class, deconstructor must be virtual
  __ATTRIBUTE_VIRTUAL_OPTIMIZED ~SystemMutexLockerFromISR(void);

protected:
  SystemMutex*                                                  holding_Mutex;
  bool*                                                         higher_Task_Woken;
};

#endif // _SYSTEM_MUTEX_H


#endif // _SYSTEM_MUTEX_ENABLED