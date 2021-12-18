#include "CommSerialConstant.h"

#if (_CONF_COMM_SERIAL_SYSTEM_SOCKET_ENABLED)

#ifndef _COMM_SERIAL_SYSTEM_SOCKET_H
#define _COMM_SERIAL_SYSTEM_SOCKET_H

/////////////////////////////////////////////////
// INCLUDE

#ifdef SYSTEM_ARDUINO_BASED
#include "HardwareSerial.h"
#else // SYSTEM_ARDUINO_BASED
// SYSTEM_PC_BASED
#ifdef SYSTEM_WIN_PLATFORM
// already included windows.h
#elif defined(SYSTEM_LINUX_PLATFORM)
#include <unistd.h> // write(), read(), close()
#include <fcntl.h> // Contains file controls like O_RDWR
#include <errno.h> // Error integer and strerror() function
#include <sys/ioctl.h> // Used for TCGETS2, which is required for custom baud rates
#include <asm/ioctls.h>
#include <asm/termbits.h>
//#include <asm/termios.h>

//

#else // SYSTEM_WIN_PLATFORM
#error Unsupported platform type
#endif // SYSTEM_WIN_PLATFORM

#endif // SYSTEM_ARDUINO_BASED


#include "CommSerialConstant.h"
#include "CommSerialSetting.h"
#include "../../os_system/SystemCommon.h"

#if (!_CONF_COMM_SERIAL_CONSTANT_ENABLED)
#error Comm serial constant is required
#endif // _CONF_COMM_SERIAL_CONSTANT_ENABLED

#if (!_CONF_COMM_SERIAL_SETTING_ENABLED)
#error Comm serial setting is required
#endif // _CONF_COMM_SERIAL_SETTING_ENABLED

/////////////////////////////////////////////////
// PREPROCESSOR

/////////////////////////////////////////////////
// DEFINE

#ifdef SYSTEM_ARDUINO_BASED
#define COMM_SERIAL_SOCKET_HANDLE               HardwareSerial*
#define COMM_SERIAL_SOCKET_HANDLE_INVALID       NULL
#define COMM_SERIAL_THREAD_EVENT                byte // void*  // this is for placebo only
#define COMM_SERIAL_EVENT_INVALID               NULL
#else // SYSTEM_ARDUINO_BASED

#ifdef SYSTEM_WIN_PLATFORM
#define COMM_SERIAL_SOCKET_HANDLE               HANDLE
#define COMM_SERIAL_SOCKET_HANDLE_INVALID       INVALID_HANDLE_VALUE
#define COMM_SERIAL_THREAD_EVENT                HANDLE
#define COMM_SERIAL_EVENT_INVALID               NULL
#elif defined(SYSTEM_LINUX_PLATFORM)
#define COMM_SERIAL_SOCKET_HANDLE               int
#define COMM_SERIAL_SOCKET_HANDLE_INVALID       -1
#define COMM_SERIAL_THREAD_EVENT                int
#define COMM_SERIAL_EVENT_INVALID               -1
#else // SYSTEM_WIN_PLATFORM
#error Unsupported platform type
#endif // SYSTEM_WIN_PLATFORM


#endif // SYSTEM_ARDUINO_BASED


/////////////////////////////////////////////////
// MARCO

/////////////////////////////////////////////////
// GLOBAL VARIABLES

/////////////////////////////////////////////////
// GLOBAL FUNCTIONS

/////////////////////////////////////////////////
// DATA TYPE (TYPEDEF)
typedef COMM_SERIAL_SOCKET_HANDLE               SerialHandler;
typedef COMM_SERIAL_THREAD_EVENT                SerialEvent;


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

/*CommSerialSystemSocket*/
class CommSerialSystemSocket
{
public:
  CommSerialSystemSocket(void);
  // WARNING: if inherite from this class, deconstructor must be virtual
  __ATTRIBUTE_VIRTUAL_OPTIMIZED ~CommSerialSystemSocket(void);
  int                                                           setConfig(void);
  //int                                                           prepare(void);
  int                                                           start(CommSerialSocketConfigTAG& socketConfig);
  int                                                           notifyExitEvent(void);
  void                                                          stop(void);
  bool                                                          isValid(void);
  uint16_t                                                      serialTimeout(void);
  //CommSerialSocketSetting*                                      socketConfig(void);
  int                                                           writeData(const uint8_t* data, DataSize_t lenToWrite, DataSize_t& writtenLen);
  int                                                           readData(uint8_t* data, DataSize_t maxToRead, DataSize_t& readLen);
protected:
  int                                                           setSocketConfig(CommSerialSocketConfigTAG& socketConfig);
  int                                                           setSocketTimeout(CommSerialSocketConfigTAG& socketConfig);

protected:
  //CommSerialSocketSetting                                       comm_Socket_Config;
  uint16_t                                                      serial_Timeout;
  SerialHandler                                                 serial_Handler;
#ifdef SYSTEM_WIN_PLATFORM
  SerialEvent                                                   thread_Exit_Event; // for windows to handle thread exiting trigger
  SerialEvent                                                   rev_Event; // for windows to handle received trigger
  SerialEvent                                                   sen_Event; // for windows to handle sent trigger
#endif // defined(SYSTEM_WIN_PLATFORM)
};

/*CommSerialHandlerFactory*/
class CommSerialHandlerFactory
{
public:
  CommSerialHandlerFactory(void);
  // WARNING: if inherite from this class, deconstructor must be virtual
  __ATTRIBUTE_VIRTUAL_OPTIMIZED ~CommSerialHandlerFactory(void);
public:
  static SerialHandler                                          generate(CommSerialSocketConfigTAG& socketConfig);
  static void                                                   release(SerialHandler& serialController);

};


#endif // _COMM_SERIAL_SYSTEM_SOCKET_H

#endif // _CONF_COMM_SERIAL_SYSTEM_SOCKET_ENABLED