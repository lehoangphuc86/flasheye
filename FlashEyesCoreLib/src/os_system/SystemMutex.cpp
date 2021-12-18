/////////////////////////////////////////////////
// INCLUDE
#include "SystemMutex.h"
#if (_CONF_SYSTEM_MUTEX_ENABLED)

#ifdef SYSTEM_PC_BASED
#include <chrono>
#endif // SYSTEM_PC_BASED
/////////////////////////////////////////////////
// PREPROCESSOR

/////////////////////////////////////////////////
// DEFINE

/////////////////////////////////////////////////
// MARCO

/////////////////////////////////////////////////
// GLOBAL VARIABLES

/////////////////////////////////////////////////
// GLOBAL FUNCTIONS

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
// CLASS IMPLEMENTAION

/* SystemMutex*/

#ifdef SYSTEM_ARDUINO_BASED
SystemMutex::SystemMutex(void)
  : mutex_Handler(NULL)
{
  this->createMutexCore();
}

SystemMutex::~SystemMutex(void)
{
  this->deleteMutexCore();
}

void SystemMutex::lock(unsigned int timeToWait)
{
  if (this->isValidCore() == false)
  {
    return;
  }
  BaseType_t ret = pdFALSE;
  if (timeToWait == SYSTEM_MUTEX_MAX_WAIT_TIME)
  {
    ret = xSemaphoreTake(this->mutex_Handler, pdMS_TO_TICKS(portMAX_DELAY));
  }
  else
  {
    ret = xSemaphoreTake(this->mutex_Handler, pdMS_TO_TICKS(timeToWait));
  }
}

void SystemMutex::lockAccuracy(unsigned long timeToWait) // nano second
{
  unsigned long waitInMs = timeToWait / SYSTEM_CONST_MILISEC_2_NANOSEC;
  if (timeToWait == SYSTEM_MUTEX_NO_WAIT_TIME_ACCURATE)
  {
    waitInMs = SYSTEM_MUTEX_NO_WAIT_TIME;
  } 
  else if (timeToWait == SYSTEM_MUTEX_MAX_WAIT_TIME_ACCURATE)
  {
    waitInMs = SYSTEM_MUTEX_MAX_WAIT_TIME;
  }
  return this->lock((unsigned int)waitInMs);
}

void SystemMutex::unlock(void)
{
  if (this->isValidCore() == false)
  {
    return;
  }
  BaseType_t ret = xSemaphoreGive(this->mutex_Handler);
}

void SystemMutex::lockFromISR(bool* higherTaskWoken)
{
  if (this->isValidCore() == false)
  {
    return;
  }

  BaseType_t taskWoken = pdFALSE;
  BaseType_t ret = xSemaphoreTakeFromISR(this->mutex_Handler, &taskWoken);

  if (higherTaskWoken != NULL)
  {
    (*higherTaskWoken) |= (taskWoken == pdFALSE ? 0 : 1);
  }
}

void SystemMutex::lockAccuracyFromISR(bool* higherTaskWoken)
{
  return this->lockFromISR(higherTaskWoken);
}

void SystemMutex::unlockFromISR(bool* higherTaskWoken)
{
  if (this->isValidCore() == false)
  {
    return;
  }
  BaseType_t taskWoken = pdFALSE;
  BaseType_t ret = xSemaphoreGiveFromISR(this->mutex_Handler, &taskWoken);
  if (higherTaskWoken != NULL)
  {
    (*higherTaskWoken) |= (taskWoken == pdFALSE ? 0 : 1);
  }
}

void* SystemMutex::nativeHandlerCore(void)
{
  return this->mutex_Handler;
}


bool SystemMutex::isValidCore(void)
{
  return (this->mutex_Handler == NULL ? false : true);
}

bool SystemMutex::createMutexCore(void)
{
  do
  {
    if (this->isValidCore() != false)
    {
      // Already intialized
      break;
    }

    this->mutex_Handler = xSemaphoreCreateBinary();
    if (this->mutex_Handler == NULL)
    {
      break;
    }

    xSemaphoreGive(this->mutex_Handler);
    return true;
  } while (0);

  this->deleteMutexCore();
  return false;
}

int SystemMutex::deleteMutexCore(void)
{
  do
  {
    if (this->isValidCore() == false)
    {
      // not created yet
      break;
    }
    vSemaphoreDelete(this->mutex_Handler);
    this->mutex_Handler = NULL;

    return 0;
  } while (0);
  
  this->mutex_Handler = NULL;
  return -1;
}
#elif defined(SYSTEM_PC_BASED)
SystemMutex::SystemMutex(void)
{
  this->createMutexCore();
}

SystemMutex::~SystemMutex(void)
{
  this->deleteMutexCore();
}

void SystemMutex::lock(unsigned int timeToWait)
{
  if (this->isValidCore() == false)
  {
    return;
  }
  
  bool ret = this->mutex_Handler.try_lock_for(std::chrono::milliseconds(timeToWait));
  if (ret != false)
  {

  }
}

void SystemMutex::lockAccuracy(unsigned long timeToWait) // nano second
{
  if (this->isValidCore() == false)
  {
    return;
  }

  bool ret = this->mutex_Handler.try_lock_for(std::chrono::nanoseconds(timeToWait));
  if (ret != false)
  {

  }
}

void SystemMutex::unlock(void)
{
  if (this->isValidCore() == false)
  {
    return;
  }
  this->mutex_Handler.unlock();
}

void SystemMutex::lockFromISR(bool* higherTaskWoken)
{
  return this->lock(SYSTEM_MUTEX_MAX_WAIT_TIME);
}

void SystemMutex::lockAccuracyFromISR(bool* higherTaskWoken)
{
  return this->lockAccuracy(SYSTEM_MUTEX_MAX_WAIT_TIME_ACCURATE);
}

void SystemMutex::unlockFromISR(bool* higherTaskWoken)
{
  return this->unlock();
}

void* SystemMutex::nativeHandlerCore(void)
{
  return &this->mutex_Handler;
}


bool SystemMutex::isValidCore(void)
{
  return true;
}

bool SystemMutex::createMutexCore(void)
{
  return true;
}

int SystemMutex::deleteMutexCore(void)
{
  return 0;
}
#endif // SYSTEM_ARDUINO_BASED


//
///* SystemMutexLocker*/
SystemMutexLocker::SystemMutexLocker(SystemMutex& mutex)
  : holding_Mutex(&mutex)
{
  this->holding_Mutex->lock(SYSTEM_MUTEX_MAX_WAIT_TIME);
}

SystemMutexLocker::~SystemMutexLocker(void)
{
  this->holding_Mutex->unlock();
}

///* SystemMutexLockerFromISR*/
SystemMutexLockerFromISR::SystemMutexLockerFromISR(SystemMutex& mutex, bool* higherTaskWoken)
  : holding_Mutex(&mutex)
  , higher_Task_Woken(higherTaskWoken)
{
  this->holding_Mutex->lockFromISR(higherTaskWoken);
}

SystemMutexLockerFromISR::~SystemMutexLockerFromISR(void)
{
  this->holding_Mutex->unlockFromISR(this->higher_Task_Woken);
}

#endif // _CONF_SYSTEM_MUTEX_ENABLED