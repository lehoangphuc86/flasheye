/*
 Name:		TestSimple.ino
 Created:	9/4/2021 9:30:33 AM
 Author:	My Laptop
*/
#include "FlashEyesCoreLib.h"
#include "FlashEyesCoreLibConfig.h"
#include "utility/Utility.h"

//#include "os_system/SystemMutex.h"
//#include "os_system/SystemCriticalSession.h"
//
//
//#include "../event_manager/EventManager.h"
//#include "../os_system/SystemMutex.h"
//#include "../os_system/SystemCriticalSession.h"
//#include "TaskSystemThread.h"
//#include "task_manager/TaskManager.h"
//#include "task_comm/task_comm_serial/CommSerial.h"
//#include "src/TestLightQueue.h"
//#include "src/TestLightQueue.cpp"

#include "task_comm/task_comm_i2c/CommI2CSystemSocket.h"
#if CONFIG_FREERTOS_UNICORE
#define ARDUINO_RUNNING_CORE 0
#else
#define ARDUINO_RUNNING_CORE 1
#endif

//#define MAIN_MEM_USAGE_ENABLED
void consoleMemUsage(int id)
{
#ifdef MAIN_MEM_USAGE_ENABLED
  int freeMem = Utility::getFreeMemory();
  Serial.print("[mem] id");
  Serial.print(id);
  Serial.print(" ");
  Serial.println(freeMem);
  //CONSOLE_LOG("[mem] id%i %i", id, freeMem);
#else

#endif // MAIN_MEM_USAGE_ENABLED
}

CommI2CSystemSocket i2cSocket;
#define I2C_COMM_TEST_CLIENT_SOCKET_BUS_NUMBER              0
#define I2C_COMM_TEST_CLIENT_SOCKET_PIN_SCL_0               0 // use default
#define I2C_COMM_TEST_CLIENT_SOCKET_PIN_SDA_0               0 // use default
#define I2C_COMM_TEST_CLIENT_SOCKET_PIN_SCL_1               0 // use default
#define I2C_COMM_TEST_CLIENT_SOCKET_PIN_SDA_1               0 // use default
#define I2C_COMM_TEST_CLIENT_SOCKET_FREQUENCY               10000 // 0 to use default
#define I2C_COMM_TEST_CLIENT_SOCKET_OPMODE                  CommI2COperationMode::CommI2CClientMode
#define I2C_COMM_TEST_CLIENT_SOCKET_TIMEOUT                 1000
#define I2C_COMM_TEST_CLIENT_CONNECTION_OPMODE              I2C_COMM_TEST_CLIENT_SOCKET_OPMODE
#define I2C_COMM_TEST_CLIENT_CONNECTION_PROTOCOL            CommI2CProtocol::CommI2CAdvanced
#define I2C_COMM_TEST_CLIENT_BUFFER_COUNT                   5
//#define I2C_COMM_TEST_CLIENT_CLIENT_ID                      1
#define I2C_COMM_TEST_CLIENT_ADDRESS                        0x73


// CLASS DEFINITION
void setup() {
  int freeMem = Utility::getFreeMemory();
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB, on LEONARDO, MICRO, YUN, and other 32u4 based boards.
  }

  delay(500);
  Serial.println(freeMem);
  consoleMemUsage(2);

  //Serial.println(sizeof(CommSerial));
  //Serial.println(sizeof(CommSerialConnectionSetting));
  //Serial.println(sizeof(CommSerialSender));
  //Serial.println(sizeof(CommSerialReceiver));
  //Serial.println(sizeof(CommSerialSystemSocket));

  //Serial.println(sizeof(TaskManager));
  //Serial.println(sizeof(EventManager));
  //Serial.println(sizeof(DataManager));
  //Serial.println(sizeof(DataQueue));
  //Serial.println(sizeof(SystemLightQueue<DataItem*>));
  //Serial.println(sizeof(DataItem));
  //Serial.println(sizeof(TaskManagerConfigTAG));
  //Serial.println(sizeof(TaskSystemThread));
  //Serial.println(sizeof(SystemCriticalSession));
  //Serial.println(sizeof(LinkedNode<DataItem*>));
  //Serial.println(sizeof(SystemMutex));
  ////
  //Serial.println(sizeof(Serial));
  //
  int ret = 0;
  CommI2CSocketConfigTAG commI2CSocketConfig = CommI2CSocketConfigTAG();
  commI2CSocketConfig.enabled = true;
  commI2CSocketConfig.busNumer = I2C_COMM_TEST_CLIENT_SOCKET_BUS_NUMBER;
  commI2CSocketConfig.pin_SCL = I2C_COMM_TEST_CLIENT_SOCKET_PIN_SCL_0;
  commI2CSocketConfig.pin_SDA = I2C_COMM_TEST_CLIENT_SOCKET_PIN_SDA_0;
  commI2CSocketConfig.frequency = I2C_COMM_TEST_CLIENT_SOCKET_FREQUENCY;
  commI2CSocketConfig.opMode = I2C_COMM_TEST_CLIENT_SOCKET_OPMODE;
  commI2CSocketConfig.address = I2C_COMM_TEST_CLIENT_ADDRESS;
  commI2CSocketConfig.timeout = I2C_COMM_TEST_CLIENT_SOCKET_TIMEOUT;

  ret = i2cSocket.setConfig(commI2CSocketConfig);
  if (ret != 0)
  {
    CONSOLE_LOG("[set] %i", -1);
  }

  ret = i2cSocket.prepare();
  if (ret != 0)
  {
    CONSOLE_LOG("[pre] %i", -1);
  }

  ret = i2cSocket.start();
  if (ret != 0)
  {
    CONSOLE_LOG("[sta] %i", -1);
  }

  consoleMemUsage(3);
  //ret += testTask.prepare();
  //criticalSession.end();
  consoleMemUsage(4);
  //ret += testTask.start();
  consoleMemUsage(5);
  
}
// the loop function runs over and over again until power down or reset
void loop()
{

}