#include "CommI2CConstant.h"

#if (_CONF_COMM_I2C_SOCKET_ENABLED)

#ifndef _COMM_I2C_SOCKET_H
#define _COMM_I2C_SOCKET_CLIENT_H

/////////////////////////////////////////////////
// INCLUDE
#include "CommI2CConstant.h"
#include "CommI2CSetting.h"
#include "../../data_manager/BufferDataManager.h"
#include "CommI2CSystemSocket.h"
#include "task_codec/TaskCodec.h"

#if (!_CONF_COMM_I2C_COMM_SETTING_ENABLED)
#error Comm I2C setting is is required
#endif // _CONF_COMM_I2C_COMM_SETTING_ENABLED

#if (!_CONF_COMM_I2C_CONSTANT_ENABLED)
#error Comm I2C constant is is required
#endif // _CONF_COMM_I2C_CONSTANT_ENABLED

#if (!_CONF_DATA_MANAGER_ENABLED)
#error Data manager is is required
#endif // _CONF_DATA_MANAGER_ENABLED

#if (!_CONF_COMM_I2C_SYSTEM_SOCKET_ENABLED)
#error Comm I2C system socket is is required
#endif // _CONF_COMM_I2C_SYSTEM_SOCKET_ENABLED

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

/*CommI2CSocket*/

class CommI2CSocket
{
  ////////////////////Function to call from outside ISR///////////////////////
public:
  CommI2CSocket(uint8_t commId);
  virtual ~CommI2CSocket(void);
  uint8_t                                                       commId(void);
  virtual int                                                   setConfig(CommI2CConfigTAG& commI2CConfig);
  virtual int                                                   start(CommI2CSocketConfigTAG& socketConfig);
  //int                                                           disconnect(void);
  bool                                                          isInitialized(void);
  bool                                                          isConnected(void);
  bool                                                          isDisconnected(void);
  bool                                                          isValidSocket(void);
  byte                                                          connectionState(void);
  //CommI2CConnectionSetting*                                     connectionSetting(void);
  bool                                                          isHeaderless(void);
  BufferDataItem*                                               getCommData(void);
  void                                                          releaseCommData(BufferDataItem* dataItem);
protected:
  virtual int                                                   prepare(void);
  void                                                          connectionState(byte state);
  //void                                                          resetReceivingInfo(void);
  //void                                                          resetSendingInfo(void);
  void                                                          closeConnection(void);
  virtual void                                                  stop(void);
  virtual void                                                  cleanUp(void);
  ////////////////////Function to call from inside ISR///////////////////////
public:
  bool                                                          isConnectedFromISR(void);
  bool                                                          isDisconnectedFromISR(void);
  bool                                                          isValidSocketFromISR(void);
  byte                                                          connectionStateFromISR(void);
  CommI2CConnectionSetting*                                     connectionSettingFromISR(void);
  BufferDataItem*                                               getCommDataFromISR(void);
  void                                                          releaseCommDataFromISR(BufferDataItem* dataItem);
protected:
  void                                                          connectionStateFromISR(byte state);
  //void                                                          resetReceivingInfoFromISR(void);
  //void                                                          resetSendingInfoFromISR(void);

  ////////////////////Shared funtcion///////////////////////
protected:
//  int                                                           receiveData(bool isHeaderLess, uint8_t sizeToReceive, uint16_t& remainingDataLen);
  DataSize_t                                                    receiveData(byte clientAddress, unsigned char* buffer, DataSize_t bufferSize, DataSize_t maxLenToRead, byte codecType, byte tryTime, bool& isDisconnected);
//  int                                                           receiveHeader(bool isHeaderLess, byte sizeToReceive);
  //int                                                           sendData(bool isHeaderLess, uint16_t& remainingDataLen);
  //int                                                           sendHeader(bool isHeaderLess, uint16_t& sentLen);
  int                                                           sendData(byte clientAddress, unsigned char* data, DataSize_t dataSize, byte codecType);

protected:
  CommI2CSystemSocket                                           i2c_Socket;
  uint8_t                                                       comm_Id; // in a system with multiple I2C, this is to identify
  //uint8_t                                                       operation_Mode;
  byte                                                          connection_State;
  uint8_t                                                       comm_Sequence_Id;
  CommI2CConnectionSetting                                      comm_Connection_Setting;

  SystemCriticalSession                                         critical_Key;
  BufferDataManager                                             data_Manager;
  TaskCodec*                                                    task_Codec;
};

#endif // _COMM_I2C_SOCKET_CLIENT_H

#endif // _CONF_COMM_I2C_SOCKET_ENABLED