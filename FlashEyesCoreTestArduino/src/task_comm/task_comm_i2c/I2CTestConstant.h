#include "../../FlashEyeCoreTestConfig.h"

#if (1) //defined(_CONF_TEST_TASK_COMM_I2C_SERVER_ENABLED) || defined(_CONF_TEST_TASK_COMM_I2C_CLIENT_ENABLED)
#ifndef _COMM_I2C_TEST_2_CONSTANT_H
#define _COMM_I2C_TEST_2_CONSTANT_H

/////////////////////////////////////////////////
// INCLUDE
#include "os_system/SystemCommon.h"
#include "task_comm/task_comm_i2c/CommI2CConstant.h"
#include "task_manager/TaskManager.h"
/////////////////////////////////////////////////
// PREPROCESSOR
#if (_CONF_TASK_MANAGER_ENABLED)
#define I2C_COMM_TEST_SERVER_USE_TASK
#endif // _CONF_TASK_MANAGER_ENABLED

/////////////////////////////////////////////////
// DEFINE
// Server task

#ifdef SYSTEM_16BIT_SUPPORT
#define I2C_COMM_TEST_TASK_MEM                              2048
#else //SYSTEM_16BIT_SUPPORT
#define I2C_COMM_TEST_TASK_MEM                              256
#endif // SYSTEM_16BIT_SUPPORT
#define I2C_COMM_TEST_TASK_PRIORITY                         2
#define I2C_COMM_TEST_TASK_EVENT_NO                         5

// I2C Socket server
#define I2C_COMM_TEST_SERVER_SOCKET_BUS_NUMBER              0
#define I2C_COMM_TEST_SERVER_SOCKET_PIN_SCL_0               0 // use default
#define I2C_COMM_TEST_SERVER_SOCKET_PIN_SDA_0               0 // use default
#define I2C_COMM_TEST_SERVER_SOCKET_PIN_SCL_1               0 // use default
#define I2C_COMM_TEST_SERVER_SOCKET_PIN_SDA_1               0 // use default
#define I2C_COMM_TEST_SERVER_SOCKET_FREQUENCY               0 // use default
#define I2C_COMM_TEST_SERVER_SOCKET_ADDRESS                 COMM_I2C_ADDRESS_INVALID
#define I2C_COMM_TEST_SERVER_SOCKET_TIMEOUT                 1000

//#define I2C_COMM_TEST_SERVER_CONNECTION_OPMODE              I2C_COMM_TEST_SERVER_SOCKET_OPMODE
#define I2C_COMM_TEST_SERVER_CONNECTION_HEADERLESS          false // not use
#define I2C_COMM_TEST_SERVER_BUFFER_COUNT                   5
#define I2C_COMM_TEST_SERVER_MAX_CLIENT_COUNT               2

//#define COMM_I2C_TEST_HEADLESS_DATA_LEN                     50
#define I2C_COMM_TEST_SERVER_CLIENT_SOCKET_1_ADDRESS        0x73
#define I2C_COMM_TEST_SERVER_CLIENT_SOCKET_2_ADDRESS        0x74//74
#define I2C_COMM_TEST_SERVER_CLIENT_SOCKET_3_ADDRESS        0x68//DS clock module

#define I2C_COMM_TEST_SERVER_CLIENT_SOCKET_1_HEADRLESS      false
#define I2C_COMM_TEST_SERVER_CLIENT_SOCKET_2_HEADRLESS      true
#define I2C_COMM_TEST_SERVER_CLIENT_SOCKET_3_HEADRLESS      true

#define I2C_COMM_TEST_SERVER_CLIENT_SOCKET_1_CHECKSUM       false
#define I2C_COMM_TEST_SERVER_CLIENT_SOCKET_2_CHECKSUM       false
#define I2C_COMM_TEST_SERVER_CLIENT_SOCKET_3_CHECKSUM       false

#define I2C_COMM_TEST_SERVER_CLIENT_SOCKET_1_CODECTPYE      TaskCodecConstant::TaskCodecType::TASK_CODEC_NONE
#define I2C_COMM_TEST_SERVER_CLIENT_SOCKET_2_CODECTPYE      TaskCodecConstant::TaskCodecType::TASK_CODEC_NONE
#define I2C_COMM_TEST_SERVER_CLIENT_SOCKET_3_CODECTPYE      TaskCodecConstant::TaskCodecType::TASK_CODEC_NONE

// I2C Socket client
#define I2C_COMM_TEST_CLIENT_SOCKET_BUS_NUMBER              0
#define I2C_COMM_TEST_CLIENT_SOCKET_PIN_SCL_0               0 // use default
#define I2C_COMM_TEST_CLIENT_SOCKET_PIN_SDA_0               0 // use default
#define I2C_COMM_TEST_CLIENT_SOCKET_PIN_SCL_1               0 // use default
#define I2C_COMM_TEST_CLIENT_SOCKET_PIN_SDA_1               0 // use default
#define I2C_COMM_TEST_CLIENT_SOCKET_FREQUENCY               10000 // 0 to use default
#define I2C_COMM_TEST_CLIENT_SOCKET_TIMEOUT                 1000
#define I2C_COMM_TEST_CLIENT_CONNECTION_HEADERLESS          I2C_COMM_TEST_SERVER_CLIENT_SOCKET_1_HEADRLESS
#define I2C_COMM_TEST_CLIENT_CHECKSUM                       I2C_COMM_TEST_SERVER_CLIENT_SOCKET_1_CHECKSUM
#define I2C_COMM_TEST_CLIENT_CODECTYPE                      I2C_COMM_TEST_SERVER_CLIENT_SOCKET_1_CODECTPYE
#define I2C_COMM_TEST_CLIENT_BUFFER_COUNT                   5
//#define I2C_COMM_TEST_CLIENT_CLIENT_ID                      1
#define I2C_COMM_TEST_CLIENT_ADDRESS                        I2C_COMM_TEST_SERVER_CLIENT_SOCKET_1_ADDRESS

#define COMM_I2C_TEST_DATA_NAME_LEN                         10

#define TEST_THREAD_POOL_COUNT                        2
#define TEST_BUFFER_POOL_COUNT                        2
#define TEST_EVENT_POOL_COUNT                         2

//#define OP_QUIT                 9
//#define OP_START                1
//#define OP_CONN                 2
#define OP_SEND                   1
#define OP_REQUEST                2

#ifdef SYSTEM_ARDUINO_BASED

#define BUTTON_ISR_TRIGGER_TYPE       FALLING

#ifdef SYSTEM_ESP_PLATFORM
#define PIN_INTERRUPT_SEND_BUTTON     18
#define PIN_INTERRUPT_REQUEST_BUTTON  19
#else // SYSTEM_ESP_PLATFORM
#define PIN_INTERRUPT_SEND_BUTTON     2
#define PIN_INTERRUPT_REQUEST_BUTTON  3
#endif // SYSTEM_ESP_PLATFORM

#else // SYSTEM_ARDUINO_BASED
#define PIN_INTERRUPT_SEND_BUTTON     -1
#define PIN_INTERRUPT_REQUEST_BUTTON  -1
#endif // SYSTEM_ARDUINO_BASED
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

typedef struct __ATTRIBUTE_ALIGN _commI2CTestData
{
  uint8_t messageId;
  uint8_t nameLen;
  uint8_t commId;
  char name[COMM_I2C_TEST_DATA_NAME_LEN + 1];
} CommI2CTestHeadlessDataTAG;

/*For event*/
//
//typedef struct
//{
//  byte count;
//  byte id;
//  unsigned long time;
//} EventInputReceivedTAG;

/////////////////////////////////////////////////
// STATIC DATA

/////////////////////////////////////////////////
// STATIC FUNCTIONS

/////////////////////////////////////////////////
// EXTERN

/////////////////////////////////////////////////
// CLASS DEFINITION

#endif // _COMM_I2C_TEST_CONSTANT_H

#endif // _CONF_TEST_TASK_COMM_I2C_SERVER_ENABLED