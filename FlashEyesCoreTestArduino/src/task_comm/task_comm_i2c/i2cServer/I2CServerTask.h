#include "../I2CTestConstant.h"

#ifdef _CONF_TEST_TASK_COMM_I2C_SERVER_ENABLED

#ifndef _I2C_SERVER_TASK_H
#define _I2C_SERVER_TASK_H

/////////////////////////////////////////////////
// INCLUDE
#include "task_comm/task_comm_i2c/CommI2CServer.h"
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
} EventCommI2CServerSendRequestedTAG;

typedef struct
{
  uint8_t clientAddress;
  uint8_t messageId;
  uint16_t requestDataSize; // for headless messageId
} EventCommI2CServerDataRequestedTAG;
/////////////////////////////////////////////////
// STATIC DATA

/////////////////////////////////////////////////
// STATIC FUNCTIONS

/////////////////////////////////////////////////
// EXTERN

/////////////////////////////////////////////////
// CLASS DEFINITION

/*I2CServerTask*/

class I2CServerTask
#ifdef I2C_COMM_TEST_SERVER_USE_TASK
  : TaskManager
#endif // I2C_COMM_TEST_SERVER_USE_TASK
{
public:
  ////////////////////Function to call from outside ISR///////////////////////
  I2CServerTask(void);
  virtual ~I2CServerTask(void);
#ifdef I2C_COMM_TEST_SERVER_USE_TASK
  int                                                           setConfig(CommI2CConfigTAG& commConfig, TaskManagerConfigTAG& taskConfigStruct);
  int                                                           start(CommI2CSocketConfigTAG& commI2CSocketConfig, TaskThreadConfigTAG& threadConfig);
#else
  int                                                           setConfig(CommI2CConfigTAG& commConfig);
  int                                                           start(CommI2CSocketConfigTAG& commI2CSocketConfig);
#endif // I2C_COMM_TEST_SERVER_USE_TASK
  
  
  int                                                           send(uint8_t clientAddress, uint8_t messageId);
  int                                                           request(uint8_t clientAddress, uint8_t messageId, uint16_t requestDataSize);
  int                                                           stop(void);
protected:
#ifdef I2C_COMM_TEST_SERVER_USE_TASK
  void                                                          proc(void) override;
#endif // I2C_COMM_TEST_SERVER_USE_TASK
  int                                                           prepare(void);
  int                                                           onEventSendRequested(unsigned char* data, unsigned int dataSize);
  int                                                           onEventDataRequested(unsigned char* data, unsigned int dataSize);

  ////////////////////Function to call from inside ISR///////////////////////
#ifdef I2C_COMM_TEST_SERVER_USE_TASK
public:
  int                                                           sendFromISR(uint8_t clientAddress, uint8_t messageId, bool* higherTaskWoken);
  int                                                           requestFromISR(uint8_t clientAddress, uint8_t messageId, uint16_t requestDataSize, bool* higherTaskWoken);
#endif // I2C_COMM_TEST_SERVER_USE_TASK
protected:
  CommI2CServer                                                 i2c_Server;
  CommI2CSocketConfigTAG                                        i2c_Socket_Config;
};

#endif // _I2C_SERVER_TASK_H

#endif // _CONF_TEST_TASK_COMM_I2C_SERVER_ENABLED