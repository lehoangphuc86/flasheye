/*
 Name:		FlashEyesCoreTest.ino
 Created:	7/5/2021 8:17:07 PM
 Author:	My Laptop
*/


#define __DEBUG_MODE
#if CONFIG_FREERTOS_UNICORE
#define ARDUINO_RUNNING_CORE 0
#else
#define ARDUINO_RUNNING_CORE 1
#endif

//#define MAIN_MEM_USAGE_ENABLED

#include "src/FlashEyeCoreTestConfig.h"
#include "FlashEyesCoreLib.h" // include library
#include "utility/Utility.h"

#ifdef _CONF_TEST_CRITICAL_SESSION_ENABLED
#include "src/os_system/TestCriticalSession.h"
TestCriticalSession testObject;
#endif // _CONF_TEST_CRITICAL_SESSION_ENABLED

#ifdef _CONF_TEST_SYSTEM_LIGHT_QUEUE_ENABLED
#include "src/os_system/TestSystemLightQueue.h"
TestSystemLightQueue testObject;
#endif // _CONF_TEST_SYSTEM_LIGHT_QUEUE_ENABLED

#ifdef _CONF_TEST_SYSTEM_MUTEX_LOCKER_ENABLED
#include "src/os_system/TestSystemMutexLocker.h"
TestSystemMutexLocker testObject;
#endif // _CONF_TEST_SYSTEM_MUTEX_LOCKER_ENABLED

#ifdef _CONF_TEST_SYSTEM_MUTEX_NOTIFY_ENABLED
#include "src/os_system/TestSystemMutexNotify.h"
TestSystemMutexNotify testObject;
#endif // _CONF_TEST_SYSTEM_MUTEX_NOTIFY_ENABLED

#ifdef __CONF_TEST_TASK_MANAGER_FUNC_ENABLED
#include "src/task_manager/TaskManagerTestMain.h"
TaskManagerTest testObject;
#endif // _CONF_TEST_SYSTEM_MUTEX_NOTIFY_ENABLED

#ifdef __CONF_TEST_TASK_COMM_SERIAL_FUNC_ENABLED
#include "src/task_comm/task_comm_serial/TaskCommSerialTestMain.h"
TaskCommSerialTest testObject;
#endif // _CONF_TEST_SYSTEM_MUTEX_NOTIFY_ENABLED

#ifdef __CONF_TEST_TASK_COMM_MBC_SERIAL_FUNC_ENABLED
#include "src/task_comm_mbc/task_comm_mbc_serial/TaskCommMBCSerialTestMain.h"
TaskCommMBCSerialTest testObject;
#endif // __CONF_TEST_TASK_COMM_MBC_SERIAL_FUNC_ENABLED


//#ifdef __CONF_TEST_TASK_COMM_I2C_CLIENT_FUNC_ENABLED
#if defined (__CONF_TEST_TASK_COMM_I2C_CLIENT_FUNC_ENABLED) && defined (SYSTEM_AVR_PLATFORM)
#include "src/task_comm/task_comm_i2c/i2cClient/I2CClientTestMain.h"
I2CClientTest testObject;
#endif // _CONF_TEST_SYSTEM_MUTEX_NOTIFY_ENABLED

//#ifdef __CONF_TEST_TASK_COMM_I2C_SERVER_FUNC_ENABLED
#if defined (__CONF_TEST_TASK_COMM_I2C_SERVER_FUNC_ENABLED) && defined (SYSTEM_ESP_PLATFORM)
#include "src/task_comm/task_comm_i2c/i2cServer/I2CServerTestMain.h"
I2CServerTest testObject;
#endif // _CONF_TEST_SYSTEM_MUTEX_NOTIFY_ENABLED


#if defined (__CONF_TEST_TASK_COMM_MBC_I2C_CLIENT_FUNC_ENABLED) && defined (SYSTEM_AVR_PLATFORM)
#include "src/task_comm_mbc/task_comm_mbc_i2c/i2cClient/MBCI2CClientTestMain.h"
MBCI2CClientTest testObject;
#endif // _CONF_TEST_SYSTEM_MUTEX_NOTIFY_ENABLED

#if defined (__CONF_TEST_TASK_COMM_MBC_I2C_SERVER_FUNC_ENABLED) && defined (SYSTEM_ESP_PLATFORM)
#include "src/task_comm_mbc/task_comm_mbc_i2c/i2cServer/MBCI2CServerTestMain.h"
MBCI2CServerTest testObject;
#endif // _CONF_TEST_SYSTEM_MUTEX_NOTIFY_ENABLED

#if defined (__CONF_TEST_TIMER_MANAGER_FUNC_ENABLED)
#include "src/timer_manager/TimerManagerTestMain.h"
TimerManagerMainTest testObject;
#endif // _CONF_TEST_SYSTEM_MUTEX_NOTIFY_ENABLED


void consoleMemUsage(int no)
{
#ifdef MAIN_MEM_USAGE_ENABLED
  int freeMem = Utility::getFreeMemory();
  Serial.print("[mem] id");
  Serial.print(no);
  Serial.print(" ");
  Serial.println(freeMem);
  //CONSOLE_LOG("[mem] id%i %i", id, freeMem);
#else

#endif // MAIN_MEM_USAGE_ENABLED
}


// the setup function runs once when you press reset or power the board
void setup() {
  int initMem = Utility::getFreeMemory();
  // initialize serial communication at 9600 bits per second:
  //SYSTEM_CONSOLE_HANDLER.begin(9600);// , SERIAL_8N1, 21, 22);
  //while (!SYSTEM_CONSOLE_HANDLER) {
  Serial.begin(56000);
  while(!Serial) {
    ; // wait for serial port to connect. Needed for native USB, on LEONARDO, MICRO, YUN, and other 32u4 based boards.
  }
  
  delay(1000);
  Serial.println(initMem);
  //
  //Serial.println(sizeof(CommSerialTest));
  //Serial.println(sizeof(OpcodeInputTask));
  //Serial.println(sizeof(TaskCommSerialTest));
  //CONSOLE_LOG("[mem] initMem", initMem);
  //delay(1000);
  consoleMemUsage(2);
  //CONSOLE_LOG("aa %i", 1);
  int result = 0;
  testObject.setup();
  consoleMemUsage(3);

  
#ifdef SYSTEM_HAS_OS_SYSTEM
  testObject.loop();
#endif // SYSTEM_HAS_OS_SYSTEM
  //CONSOLE_LOG("%s", "[setup] done");

  consoleMemUsage(4);
}
// the loop function runs over and over again until power down or reset
uint8_t count = 0;
void loop()
{
#ifdef MAIN_MEM_USAGE_ENABLED
  if (count <2)
  {
    consoleMemUsage(5);
    count++;
  }
#endif // MAIN_MEM_USAGE_ENABLED
#ifndef SYSTEM_HAS_OS_SYSTEM
  testObject.loop();
#endif // !SYSTEM_HAS_OS_SYSTEM
}
