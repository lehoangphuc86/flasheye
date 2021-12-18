#include "../I2CTestConstant.h"

#ifdef _CONF_TEST_TASK_COMM_I2C_CLIENT_ENABLED
#ifndef _I2C_CLIENT_TASK_2_H
#define _I2C_CLIENT_TASK_2_H
/////////////////////////////////////////////////
// INCLUDE
#include "task_comm/task_comm_i2c/CommI2CClient.h"
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

/////////////////////////////////////////////////
// STATIC DATA

/////////////////////////////////////////////////
// STATIC FUNCTIONS

/////////////////////////////////////////////////
// EXTERN

/////////////////////////////////////////////////
// CLASS DEFINITION

/*I2CClientTask*/

class I2CClientTask
{
public:
  I2CClientTask(uint8_t commAddress);
  virtual ~I2CClientTask(void);
  int                                                           setConfig(CommI2CConfigTAG& commConfig);
  int                                                           start(CommI2CSocketConfigTAG& commI2CSocketConfig);
  void                                                          stop(void);
protected:
  int                                                           prepare(void);
  // for callback
  void                                                          cbOnDataRequested(void* param, DataSize_t paramSize);
  void                                                          cbOnDataReceived(void* param, DataSize_t paramSize);
protected:
  CommI2CClient                                                 i2c_Client;
  uint8_t                                                       comm_Address;
public:
  static void                                                   cbOnDataRequestedNavigator(void* param, DataSize_t paramSize);
  static void                                                   cbOnDataReceivedNavigator(void* param, DataSize_t paramSize);
};

#endif // _I2C_CLIENT_TASK_H

#endif // _CONF_TEST_TASK_COMM_I2C_CLIENT_ENABLED