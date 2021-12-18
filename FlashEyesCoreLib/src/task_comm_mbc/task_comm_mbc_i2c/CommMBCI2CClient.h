#include "CommMBCI2CConstant.h"

#if (_CONF_COMM_MBC_I2C_CLIENT_ENABLED)

#ifndef _COMM_MBC_I2C_CLIENT_H
#define _COMM_MBC_I2C_CLIENT_H

/////////////////////////////////////////////////
// INCLUDE
#include "../CommMBCEngine.h"
#include "../../task_comm/task_comm_i2c/CommI2CClient.h"

#if (!_CONF_COMM_MBC_ENGINE_ENABLED)
#error Comm MBC engine is required
#endif // _CONF_COMM_MBC_ENGINE_ENABLED

#if (!_CONF_COMM_I2C_CLIENT_ENABLED)
#error Comm I2C client is required
#endif // _CONF_COMM_I2C_CLIENT_ENABLED

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

/*CommMBCI2CClient*/

class CommMBCI2CClient
  : public CommI2CClient
{
public:
  ////////////////////Function to call from outside ISR///////////////////////
  CommMBCI2CClient(byte commId);
  virtual ~CommMBCI2CClient(void);
  uint16_t                                                      registerMessageId(uint8_t messageId, uint16_t maxRawSize = 0);
  int                                                           setConfig(CommMBCI2CConfigTAG& commMbcI2CConfig);
  int                                                           start(CommMBCI2CSocketConfigTAG& socketConfig) override;
  void                                                          stop(void) override;
protected:
  // hide public interface from parent
  int                                                           setConfig(CommI2CConfigTAG& commI2CConfig) override;
  int                                                           sendFromISR(unsigned char* data, DataSize_t dataSize) override;
  // normal member methods
  int                                                           prepare(void);
  void                                                          cleanUp(void) override;
  ////////////////////Function to call from inside ISR///////////////////////
public:
  int                                                           sendFromISR(uint8_t messageId, unsigned char* inputBuffer, uint16_t inputBufferSize);
protected:
  // callback
  void                                                          cbOnDataRequestedFromISR(void* param, DataSize_t dataSize);
  void                                                          cbOnDataReceivedFromISR(void* param, DataSize_t dataSize);
protected:
  // callback navigator
  static void                                                   cbOnDataRequestedFromISRNavigator(void* param, DataSize_t dataSize);
  static void                                                   cbOnDataReceivedFromISRNavigator(void* param, DataSize_t dataSize);

protected:
  CommMBCEngine                                                 comm_Mbc_Engine;
  CommMBCI2CConfigTAG                                           comm_Mbc_I2C_Config;
};

#endif // _COMM_MBC_I2C_CLIENT_H

#endif // _CONF_COMM_MBC_I2C_CLIENT_ENABLED