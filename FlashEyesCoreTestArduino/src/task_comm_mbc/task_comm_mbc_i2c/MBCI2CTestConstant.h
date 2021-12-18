#include "../../FlashEyeCoreTestConfig.h"

#if (defined(_CONF_TEST_TASK_COMM_MBC_I2C_SERVER_ENABLED) || defined(_CONF_TEST_TASK_COMM_MBC_I2C_CLIENT_ENABLED))
#ifndef _COMM_MBC_I2C_TEST_2_CONSTANT_H
#define _COMM_MBC_I2C_TEST_2_CONSTANT_H

/////////////////////////////////////////////////
// INCLUDE
#include "os_system/SystemCommon.h"
#include "task_comm_mbc/task_comm_mbc_i2c/CommMBCI2CConstant.h"
#include "task_manager/TaskManager.h"
/////////////////////////////////////////////////
// PREPROCESSOR
#if (_CONF_TASK_MANAGER_ENABLED)
#define MBC_I2C_COMM_TEST_SERVER_USE_TASK
#endif // _CONF_TASK_MANAGER_ENABLED

/////////////////////////////////////////////////
// DEFINE
// Server task

#ifdef SYSTEM_16BIT_SUPPORT
#define MBC_I2C_COMM_TEST_TASK_MEM                          2048
#else //SYSTEM_16BIT_SUPPORT
#define MBC_I2C_COMM_TEST_TASK_MEM                          256
#endif // SYSTEM_16BIT_SUPPORT
#define MBC_I2C_COMM_TEST_TASK_PRIORITY                     2
#define MBC_I2C_COMM_TEST_TASK_EVENT_NO                         5

// I2C Socket server
#define MBC_I2C_COMM_TEST_SERVER_SOCKET_BUS_NUMBER          0
#define MBC_I2C_COMM_TEST_SERVER_SOCKET_PIN_SCL_0           0 // use default
#define MBC_I2C_COMM_TEST_SERVER_SOCKET_PIN_SDA_0           0 // use default
#define MBC_I2C_COMM_TEST_SERVER_SOCKET_PIN_SCL_1           0 // use default
#define MBC_I2C_COMM_TEST_SERVER_SOCKET_PIN_SDA_1           0 // use default
#define MBC_I2C_COMM_TEST_SERVER_SOCKET_FREQUENCY           0 // use default
#define MBC_I2C_COMM_TEST_SERVER_SOCKET_ADDRESS             COMM_I2C_ADDRESS_INVALID
#define MBC_I2C_COMM_TEST_SERVER_SOCKET_TIMEOUT             1000

//#define I2C_COMM_TEST_SERVER_CONNECTION_OPMODE              I2C_COMM_TEST_SERVER_SOCKET_OPMODE
#define MBC_I2C_COMM_TEST_SERVER_CONNECTION_HEADERLESS      false // not use
#define MBC_I2C_COMM_TEST_SERVER_BUFFER_COUNT               5
#define MBC_I2C_COMM_TEST_SERVER_MAX_CLIENT_COUNT           2
#define MBC_I2C_COMM_TEST_SERVER_CODECTYPE                  TaskCodecConstant::TaskCodecType::TASK_CODEC_HEX

//#define COMM_I2C_TEST_HEADLESS_DATA_LEN                     50
#define MBC_I2C_COMM_TEST_SERVER_CLIENT_SOCKET_1_ADDRESS        0x73
#define MBC_I2C_COMM_TEST_SERVER_CLIENT_SOCKET_2_ADDRESS        0x74//74
#define MBC_I2C_COMM_TEST_SERVER_CLIENT_SOCKET_3_ADDRESS        0x68//DS clock module

#define MBC_I2C_COMM_TEST_SERVER_CLIENT_SOCKET_1_HEADRLESS      false
#define MBC_I2C_COMM_TEST_SERVER_CLIENT_SOCKET_2_HEADRLESS      true
#define MBC_I2C_COMM_TEST_SERVER_CLIENT_SOCKET_3_HEADRLESS      true

#define MBC_I2C_COMM_TEST_SERVER_CLIENT_SOCKET_1_CHECKSUM       true
#define MBC_I2C_COMM_TEST_SERVER_CLIENT_SOCKET_2_CHECKSUM       false
#define MBC_I2C_COMM_TEST_SERVER_CLIENT_SOCKET_3_CHECKSUM       false

#define MBC_I2C_COMM_TEST_SERVER_CLIENT_SOCKET_1_CODECTPYE      TaskCodecConstant::TaskCodecType::TASK_CODEC_HEX
#define MBC_I2C_COMM_TEST_SERVER_CLIENT_SOCKET_2_CODECTPYE      TaskCodecConstant::TaskCodecType::TASK_CODEC_NONE
#define MBC_I2C_COMM_TEST_SERVER_CLIENT_SOCKET_3_CODECTPYE      TaskCodecConstant::TaskCodecType::TASK_CODEC_NONE

// I2C Socket client
#define MBC_I2C_COMM_TEST_CLIENT_SOCKET_BUS_NUMBER              0
#define MBC_I2C_COMM_TEST_CLIENT_SOCKET_PIN_SCL_0               0 // use default
#define MBC_I2C_COMM_TEST_CLIENT_SOCKET_PIN_SDA_0               0 // use default
#define MBC_I2C_COMM_TEST_CLIENT_SOCKET_PIN_SCL_1               0 // use default
#define MBC_I2C_COMM_TEST_CLIENT_SOCKET_PIN_SDA_1               0 // use default
#define MBC_I2C_COMM_TEST_CLIENT_SOCKET_FREQUENCY               10000 // 0 to use default
#define MBC_I2C_COMM_TEST_CLIENT_SOCKET_TIMEOUT                 1000
#define MBC_I2C_COMM_TEST_CLIENT_CONNECTION_HEADERLESS          MBC_I2C_COMM_TEST_SERVER_CLIENT_SOCKET_1_HEADRLESS
#define MBC_I2C_COMM_TEST_CLIENT_CHECKSUM                       MBC_I2C_COMM_TEST_SERVER_CLIENT_SOCKET_1_CHECKSUM
#define MBC_I2C_COMM_TEST_CLIENT_CODECTYPE                      MBC_I2C_COMM_TEST_SERVER_CLIENT_SOCKET_1_CODECTPYE
#define MBC_I2C_COMM_TEST_CLIENT_BUFFER_COUNT                   5
//#define I2C_COMM_TEST_CLIENT_CLIENT_ID                      1
#define MBC_I2C_COMM_TEST_CLIENT_ADDRESS                        MBC_I2C_COMM_TEST_SERVER_CLIENT_SOCKET_1_ADDRESS

#define MBC_COMM_I2C_TEST_DATA_NAME_LEN                         10

#define MBC_TEST_THREAD_POOL_COUNT                        2
#define MBC_TEST_BUFFER_POOL_COUNT                        2
#define MBC_TEST_EVENT_POOL_COUNT                         2

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

typedef struct __ATTRIBUTE_ALIGN _commMBCI2CTestData
{
  uint8_t messageId;
  uint8_t nameLen;
  uint8_t commId;
  char name[MBC_COMM_I2C_TEST_DATA_NAME_LEN + 1];
} CommMBCI2CTestHeadlessDataTAG;

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

#endif // _COMM_MBC_I2C_TEST_2_CONSTANT_H

#endif // (defined(_CONF_TEST_TASK_COMM_MBC_I2C_SERVER_ENABLED) || defined(_CONF_TEST_TASK_COMM_MBC_I2C_CLIENT_ENABLED))