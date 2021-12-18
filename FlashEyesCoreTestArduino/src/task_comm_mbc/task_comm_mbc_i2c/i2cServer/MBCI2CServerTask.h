#include "../MBCI2CTestConstant.h"

#ifdef _CONF_TEST_TASK_COMM_MBC_I2C_SERVER_ENABLED

#ifndef _MBC_I2C_SERVER_TASK_H
#define _MBC_I2C_SERVER_TASK_H

/////////////////////////////////////////////////
// INCLUDE
#include "task_comm_mbc/task_comm_mbc_i2c/CommMBCI2CServer.h"
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
// DATA TYPE (TYPEDEF)

/////////////////////////////////////////////////
// DATA TYPE (ENUM)

/////////////////////////////////////////////////
// DATA TYPE (STRUCT)
typedef struct
{
  uint8_t clientAddress;
  uint8_t messageId;
} EventCommMBCI2CServerSendRequestedTAG;

typedef struct
{
  uint8_t clientAddress;
  uint8_t messageId;
  uint16_t requestDataSize; // for headless messageId
} EventCommMBCI2CServerDataRequestedTAG;
/////////////////////////////////////////////////
// STATIC DATA

/////////////////////////////////////////////////
// STATIC FUNCTIONS

/////////////////////////////////////////////////
// EXTERN

/////////////////////////////////////////////////
// CLASS DEFINITION

/*MBCI2CServerTask*/

class MBCI2CServerTask
#ifdef MBC_I2C_COMM_TEST_SERVER_USE_TASK
  : TaskManager
#endif // MBC_I2C_COMM_TEST_SERVER_USE_TASK
{
public:
  ////////////////////Function to call from outside ISR///////////////////////
  MBCI2CServerTask(void);
  virtual ~MBCI2CServerTask(void);
#ifdef MBC_I2C_COMM_TEST_SERVER_USE_TASK
  int                                                           setConfig(CommMBCI2CConfigTAG& commConfig, TaskManagerConfigTAG& taskConfigStruct);
  int                                                           start(CommMBCI2CSocketConfigTAG& commI2CSocketConfig, TaskThreadConfigTAG& threadConfig);
#else
  int                                                           setConfig(CommMBCI2CConfigTAG& commConfig);
  int                                                           start(CommMBCI2CSocketConfigTAG& commI2CSocketConfig);
#endif // MBC_I2C_COMM_TEST_SERVER_USE_TASK
  
  
  int                                                           send(uint8_t clientAddress, uint8_t messageId);
  int                                                           request(uint8_t clientAddress, uint8_t messageId, uint16_t requestDataSize);
  int                                                           stop(void);
protected:
#ifdef MBC_I2C_COMM_TEST_SERVER_USE_TASK
  void                                                          proc(void) override;
#endif // MBC_I2C_COMM_TEST_SERVER_USE_TASK
  int                                                           prepare(void);
  int                                                           onEventSendRequested(unsigned char* data, unsigned int dataSize);
  int                                                           onEventDataRequested(unsigned char* data, unsigned int dataSize);

  ////////////////////Function to call from inside ISR///////////////////////
#ifdef MBC_I2C_COMM_TEST_SERVER_USE_TASK
public:
  int                                                           sendFromISR(uint8_t clientAddress, uint8_t messageId, bool* higherTaskWoken);
  int                                                           requestFromISR(uint8_t clientAddress, uint8_t messageId, uint16_t requestDataSize, bool* higherTaskWoken);
#endif // MBC_I2C_COMM_TEST_SERVER_USE_TASK
protected:
  CommMBCI2CServer                                              mbc_i2c_Server;
  CommMBCI2CSocketConfigTAG                                     mbc_i2c_Socket_Config;
};

#endif // _MBC_I2C_SERVER_TASK_H

#endif // _CONF_TEST_TASK_COMM_MBC_I2C_SERVER_ENABLED