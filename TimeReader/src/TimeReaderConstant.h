#ifndef _TIME_READER_CONSTANT_H
#define _TIME_READER_CONSTANT_H

/////////////////////////////////////////////////
// INCLUDE
#include "FlashEyesCoreLib.h"
#include "os_system/SystemCommon.h"
#include "task_comm_mbc/task_comm_mbc_i2c/CommMBCI2CConstant.h"
#include "task_comm_mbc/task_comm_mbc_i2c/CommMBCI2CServer.h"
/////////////////////////////////////////////////
// PREPROCESSOR

/////////////////////////////////////////////////
// DEFINE
// Server task

// I2C Socket server
#define TIME_READER_SERVER_SOCKET_BUS_NUMBER              0
#define TIME_READER_SERVER_SOCKET_PIN_SCL_0               0 // use default
#define TIME_READER_SERVER_SOCKET_PIN_SDA_0               0 // use default
#define TIME_READER_SERVER_SOCKET_PIN_SCL_1               0 // use default
#define TIME_READER_SERVER_SOCKET_PIN_SDA_1               0 // use default
#define TIME_READER_SERVER_SOCKET_FREQUENCY               0 // use default
#define TIME_READER_SERVER_SOCKET_OPMODE                  CommI2COperationMode::CommI2CServerMode
#define TIME_READER_SERVER_SOCKET_ADDRESS                 0  // not used
#define TIME_READER_SERVER_SOCKET_TIMEOUT                 1000

#define TIME_READER_SERVER_CONNECTION_OPMODE              TIME_READER_SERVER_SOCKET_OPMODE
#define TIME_READER_SERVER_CONNECTION_PROTOCOL            CommI2CProtocol::CommI2CSimple // not use
#define TIME_READER_SERVER_CONNECTION_BUFFER_COUNT        5
#define TIME_READER_SERVER_CONNECTION_MAX_CLIENT_COUNT    2

//#define COMM_I2C_TEST_HEADLESS_DATA_LEN                     50
#define TIME_READER_SERVER_CLIENT_SOCKET_1_ADDRESS        0x68

#define TIME_READER_SERVER_CLIENT_SOCKET_1_PROTOCOL       (byte)CommI2CProtocol::CommI2CSimple

#define TIME_READER_REG_DATA_SIZE                         1
#define TIME_READER_REG_SEC                               0x00
#define TIME_READER_REG_MIN                               0x01
#define TIME_READER_REG_HRS                               0x02
#define TIME_READER_REG_DAY                               0x03
#define TIME_READER_REG_DAT                               0x04
#define TIME_READER_REG_MON                               0x05
#define TIME_READER_REG_YRS                               0x06
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

#endif // _TIME_READER_CONSTANT_H