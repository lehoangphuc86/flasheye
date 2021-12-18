#include "CommI2CConstant.h"

#if (_CONF_COMM_I2C_CLIENT_ENABLED)

#ifndef _COMM_I2C_CLIENT_H
#define _COMM_I2C_CLIENT_H

/////////////////////////////////////////////////
// INCLUDE
#include "CommI2CSocket.h"

#if (!_CONF_COMM_I2C_SOCKET_ENABLED)
#error Comm I2C socket is required
#endif // 

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

/*CommI2CClient*/

class CommI2CClient
  : public CommI2CSocket
{
public:
  ////////////////////Function to call from outside ISR///////////////////////
  CommI2CClient(byte commId);
  virtual ~CommI2CClient(void);
  virtual int                                                   setConfig(CommI2CConfigTAG& commI2CConfig) override;
  virtual int                                                   start(CommI2CSocketConfigTAG& socketConfig) override;
  virtual void                                                  stop(void) override;
protected:
  bool                                                          registeredInstanced(void);
  virtual int                                                   prepare(void) override;
  
  ////////////////////Function to call from inside ISR///////////////////////
public:
  virtual int                                                   sendFromISR(unsigned char* data, DataSize_t dataSize);
protected:
  void                                                          onRequestFromISR(bool* woken);
  void                                                          onReceivedFromISR(int byteNumber, bool* woken);
  int                                                           notifyParentDataReceivedFromISR(bool* higherTaskWoken);
  int                                                           setDataToSendFromISR(BufferDataItem* &dataItem, uint8_t commSequenceId);
  void                                                          resetReceivingInfoFromISR(void);
  void                                                          resetSendingInfoFromISR(void);
protected:
  byte                                                          registered_Instance_Index;
  CommI2CSendingInfoTAG                                         sending_Info;
  CommI2CReceivingInfoTAG                                       receiving_Info;

private:
  static int                                                    registerInstance(CommI2CClient* instance);
  static int                                                    registerCallback(CommI2CClient* instance, CommI2CSystemSocket* i2cSocket);

private:
  // function to drive onRequest callback
  static void                                                   onRequestFromISR0(void);
  static void                                                   onRequestFromISR1(void);
  static void                                                   onRequestFromISR2(void);
  // function to drive onReceived callback
  static void                                                   onReceivedFromISR0(int byteNum);
  static void                                                   onReceivedFromISR1(int byteNum);
  static void                                                   onReceivedFromISR2(int byteNum);

private:
  static CommI2CClient*                                         client_Instances[COMM_I2C_MAX_CLIENT_INSTANCE];
  static byte                                                   current_Client_Index;
};

#endif // _COMM_I2C_CLIENT_H

#endif // _CONF_COMM_I2C_CLIENT_ENABLED