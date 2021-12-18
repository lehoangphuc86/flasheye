#include "CommI2CConstant.h"

#if (_CONF_COMM_I2C_SERVER_ENABLED)
#ifndef _COMM_I2C_SERVER_H
#define _COMM_I2C_SERVER_H

/////////////////////////////////////////////////
// INCLUDE
#include "CommI2CSocket.h"
#include "../CommClientManager.h"

#if (!_CONF_COMM_I2C_SOCKET_ENABLED)
#error Comm I2C socket is required
#endif // _CONF_COMM_I2C_SOCKET_ENABLED

#if (!_CONF_COMM_CLIENT_MANAGER_ENABLED)
#error Comm client manager is required
#endif // _CONF_COMM_CLIENT_MANAGER_ENABLED
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

/*CommI2CClientInfo*/
class CommI2CClientInfo
  : public CommClientInfo
{
public:
  CommI2CClientInfo();
  virtual ~CommI2CClientInfo(void);
  bool                                                          compareByClientProperties(void* clientProperties) override;
  int                                                           setClientProperties(void* srcClientProperties) override;
protected:
  void                                                          initClientProperties(void) override;
};

/*CommI2CServer*/
class CommI2CServer
  : public CommI2CSocket
{
  ////////////////////Function to call from outside ISR///////////////////////
public:
  CommI2CServer(byte commId = 0);
  virtual ~CommI2CServer(void);
  virtual int                                                   setConfig(CommI2CConfigTAG& commI2CConfig) override;
  virtual int                                                   start(CommI2CSocketConfigTAG& socketConfig) override;
  int                                                           appendClient(void* clientProperties);
  CommClientInfo*                                               findClient(ClientId_t clientId, void* clientProperties);
  int                                                           sendById(ClientId_t clientId, unsigned char* data, DataSize_t dataSize);
  int                                                           sendByAddress(uint8_t clientAddress, unsigned char* data, DataSize_t dataSize);
  int                                                           send(CommI2CClientInfo* clientInfo, unsigned char* data, DataSize_t dataSize);
  int                                                           requestById(ClientId_t clientId, uint8_t requestingDataSize, byte tryTime, bool& isDisconnected, BufferDataItem*& outDataItem);
  int                                                           requestByAddress(uint8_t clientAddress, uint8_t requestingDataSize, byte tryTime, bool& isDisconnected, BufferDataItem*& outDataItem);
  int                                                           request(CommI2CClientInfo* clientInfo, uint8_t requestingDataSize, byte tryTime, bool& isDisconnected, BufferDataItem*& outDataItem);
  void                                                          stop(void) override;
protected:
  virtual int                                                   prepare(void) override;
  void                                                          cleanUp(void) override;
  ////////////////////Shared funtcion///////////////////////
protected:
  int                                                           sendPackage(CommI2CClientInfo* clientInfo, BufferDataItem*& sendingDataItem);
  int                                                           requestPackage(CommI2CClientInfo* clientInfo, uint8_t requestingDataSize, BufferDataItem* revDataItem, byte tryTime, bool& isDisconnected);
  uint8_t                                                       nextCommSequenceCore(void);
  
protected:
  CommClientManager                                             client_Manager;
};

#endif // _COMM_I2C_SERVER_H

#endif // _CONF_COMM_I2C_SERVER_ENABLED