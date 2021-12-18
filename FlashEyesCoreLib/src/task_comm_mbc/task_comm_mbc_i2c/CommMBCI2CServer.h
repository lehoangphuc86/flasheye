#include "CommMBCI2CConstant.h"

#if (_CONF_COMM_MBC_I2C_SERVER_ENABLED)
#ifndef _COMM_MBC_I2C_SOCKET_SERVER_H
#define _COMM_MBC_I2C_SOCKET_SERVER_H

/////////////////////////////////////////////////
// INCLUDE
#include "../CommMBCEngine.h"
#include "../../task_comm/task_comm_i2c/CommI2CServer.h"

#if (!_CONF_COMM_MBC_ENGINE_ENABLED)
#error Comm MBC engine is required
#endif // _CONF_COMM_MBC_ENGINE_ENABLED

#if (!_CONF_COMM_I2C_SERVER_ENABLED)
#error Comm I2C server is required
#endif // _CONF_COMM_I2C_SERVER_ENABLED

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

/*CommMBCI2CServer*/

class CommMBCI2CServer
  : public CommI2CServer
{
public:
  ////////////////////Function to call from outside ISR///////////////////////
  CommMBCI2CServer(void);
  virtual ~CommMBCI2CServer(void);
  uint16_t                                                      registerMessageId(uint8_t messageId, uint16_t maxRawSize = 0);
  int                                                           setConfig(CommMBCI2CConfigTAG& mbcI2CConfig);
  int                                                           start(CommMBCI2CSocketConfigTAG& socketConfig) override;
  void                                                          stop(void) override;
  
  int                                                           sendById(ClientId_t clientId, uint8_t messageId, unsigned char* data, DataSize_t dataSize);
  int                                                           sendByAddress(uint8_t clientAddress, uint8_t messageId, unsigned char* data, DataSize_t dataSize);
  int                                                           send(CommI2CClientInfo* clientInfo, uint8_t messageId, unsigned char* data, DataSize_t dataSize);
  int                                                           requestById(ClientId_t clientId, uint8_t messageId, uint8_t requestingDataSize, byte tryTime, bool& isDisconnected, BufferDataItem*& outDataItem);
  int                                                           requestByAddress(uint8_t clientAddress, uint8_t messageId, uint8_t requestingDataSize, byte tryTime, bool& isDisconnected, BufferDataItem*& outDataItem);
  int                                                           request(CommI2CClientInfo* clientInfo, uint8_t messageId, uint8_t requestingDataSize, byte tryTime, bool& isDisconnected, BufferDataItem*& outDataItem);

  ////////////////////Shared funtcion///////////////////////
protected:
  // hide public interface from parent
  int                                                           setConfig(CommI2CConfigTAG& commI2CConfig) override;
  //int                                                           start(CommI2CSocketConfigTAG& socketConfig) override;
  int                                                           sendById(ClientId_t clientId, unsigned char* data, DataSize_t dataSize);
  int                                                           sendByAddress(uint8_t clientAddress, unsigned char* data, DataSize_t dataSize);
  int                                                           send(CommI2CClientInfo* clientInfo, unsigned char* data, DataSize_t dataSize);
  int                                                           requestById(ClientId_t clientId, uint8_t requestingDataSize, byte tryTime, bool& isDisconnected, BufferDataItem*& outDataItem);
  int                                                           requestByAddress(uint8_t clientAddress, uint8_t requestingDataSize, byte tryTime, bool& isDisconnected, BufferDataItem*& outDataItem);
  int                                                           request(CommI2CClientInfo* clientInfo, uint8_t requestingDataSize, byte tryTime, bool& isDisconnected, BufferDataItem*& outDataItem);

  // normal member methods
  int                                                           prepare(void) override;
  void                                                          cleanUp(void) override;
  
protected:
  CommMBCEngine                                                 comm_Mbc_Engine;
  CommMBCI2CConfigTAG                                           comm_Mbc_I2C_Config;
};

#endif // _COMM_MBC_I2C_SOCKET_SERVER_H

#endif // _CONF_COMM_MBC_I2C_SERVER_ENABLED