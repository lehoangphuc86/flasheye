/////////////////////////////////////////////////
// INCLUDE
#include "SystemCriticalSession.h"
#if (_CONF_SYSTEM_CRITICAL_SESSION_ENABLED)
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

// SystemCriticalSession
SystemCriticalSession::SystemCriticalSession(void)
{
  SYSTEM_CRITICAL_SECTION_MUTEX_INIT(this->critical_Mutex);
}

SystemCriticalSession::~SystemCriticalSession(void)
{

}

void SystemCriticalSession::begin(void)
{
  SYSTEM_CRITICAL_SECTION_BEGIN(this->critical_Mutex);
}

void SystemCriticalSession::end(void)
{
  SYSTEM_CRITICAL_SECTION_END(this->critical_Mutex);
}

void SystemCriticalSession::beginFromISR(void)
{
  SYSTEM_CRITICAL_SECTION_BEGIN_FROM_ISR(this->critical_Mutex);
}

void SystemCriticalSession::endFromISR(void)
{
  SYSTEM_CRITICAL_SECTION_END_FROM_ISR(this->critical_Mutex);
}

// SystemCriticalLocker
SystemCriticalLocker::SystemCriticalLocker(SystemCriticalSession& criticalLocker)
  : critical_Locker(&criticalLocker)
{
  this->critical_Locker->begin();
}

SystemCriticalLocker::~SystemCriticalLocker(void)
{
  this->critical_Locker->end();
}

// SystemCriticalLockerFromISR
SystemCriticalLockerFromISR::SystemCriticalLockerFromISR(SystemCriticalSession& criticalLocker)
  : critical_Locker(&criticalLocker)
{
  this->critical_Locker->beginFromISR();
}

SystemCriticalLockerFromISR::~SystemCriticalLockerFromISR(void)
{
  this->critical_Locker->endFromISR();
}

#endif // _CONF_SYSTEM_CRITICAL_SESSION_ENABLED