#include "../MBCI2CTestConstant.h"

#ifdef _CONF_TEST_TASK_COMM_MBC_I2C_CLIENT_ENABLED
#ifndef _MBC_I2C_CLIENT_TASK_2_H
#define _MBC_I2C_CLIENT_TASK_2_H
/////////////////////////////////////////////////
// INCLUDE
#include "task_comm_mbc/task_comm_mbc_i2c/CommMBCI2CClient.h"
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

/*MBCI2CClientTask*/

class MBCI2CClientTask
{
public:
  MBCI2CClientTask(uint8_t commAddress);
  virtual ~MBCI2CClientTask(void);
  int                                                           setConfig(CommMBCI2CConfigTAG& commConfig);
  int                                                           start(CommMBCI2CSocketConfigTAG& socketConfig);
  void                                                          stop(void);
protected:
  int                                                           prepare(void);
  // for callback
  void                                                          cbOnDataRequested(void* param, DataSize_t paramSize);
  void                                                          cbOnDataReceived(void* param, DataSize_t paramSize);
protected:
  CommMBCI2CClient                                              mbc_i2c_Client;
  uint8_t                                                       comm_Address;
public:
  static void                                                   cbOnDataRequestedNavigator(void* param, DataSize_t paramSize);
  static void                                                   cbOnDataReceivedNavigator(void* param, DataSize_t paramSize);
};

#endif // _MBC_I2C_CLIENT_TASK_2_H

#endif // _CONF_TEST_TASK_COMM_MBC_I2C_CLIENT_ENABLED