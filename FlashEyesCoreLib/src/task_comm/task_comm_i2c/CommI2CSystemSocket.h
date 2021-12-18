#include "CommI2CConstant.h"

#if (_CONF_COMM_I2C_SYSTEM_SOCKET_ENABLED)

#ifndef _COMM_I2C_SYSTEM_SOCKET_H
#define _COMM_I2C_SYSTEM_SOCKET_H

/////////////////////////////////////////////////
// INCLUDE
#include "Wire.h"
#include "CommI2CConstant.h"
#include "CommI2CSetting.h"
#include "../../os_system/SystemCommon.h"

#if (!_CONF_COMM_I2C_CONSTANT_ENABLED)
#error Comm I2C constant is required
#endif // 

#if (!_CONF_COMM_I2C_COMM_SETTING_ENABLED)
#error Comm I2C setting is required
#endif // 

/////////////////////////////////////////////////
// PREPROCESSOR

/////////////////////////////////////////////////
// DEFINE
#define COMM_I2C_TWO_WIRE_HANDLE                TwoWire*
#define COMM_I2C_SOCKET_HANDLE_INVALID          NULL
/////////////////////////////////////////////////
// MARCO

/////////////////////////////////////////////////
// GLOBAL VARIABLES

/////////////////////////////////////////////////
// GLOBAL FUNCTIONS

/////////////////////////////////////////////////
// DATA TYPE (TYPEDEF)
typedef COMM_I2C_TWO_WIRE_HANDLE                CommI2CSocketHandler;
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

/*CommI2CSystemSocket*/

class CommI2CSystemSocket
{
public:
  CommI2CSystemSocket(void);
  virtual ~CommI2CSystemSocket(void);
  bool                                                          isServerMode(void);
  uint8_t                                                       address(void);
  bool                                                          isValid(void);
  int                                                           start(CommI2CSocketConfigTAG& socketConfig);
  void                                                          stop(void);
  // config
  void                                                          onReceive(CommI2CSystemCbOnDataReceived cbOnReceived);
  void                                                          onRequest(CommI2CSystemCbOnRequested cbOnRequest);

  // controlling functions
  void                                                          setClock(uint32_t frequency);
  void                                                          setTimeOut(uint16_t timeOutMillis); // default timeout of i2c transactions is 50ms
  void                                                          beginTransmission(int address);
  uint8_t                                                       endTransmission(bool sendStop = true);
  uint8_t                                                       requestFrom(int address, int size, int sendStop);
  DataSize_t                                                    write(uint8_t val);
  DataSize_t                                                    write(const uint8_t* data, DataSize_t quantity);
  int                                                           writeData(uint8_t clientAddress, const uint8_t* data, DataSize_t quantity, DataSize_t& writtenLen);
  //int                                                           writeData(const uint8_t* data, DataSize_t quantity, DataSize_t& writtenLen);
  int                                                           available(void);
  //int                                                           read(void);
  //int                                                           read(uint8_t* data, DataSize_t maxToRead, DataSize_t& readLen);
  //int                                                           readData(uint8_t* data, DataSize_t maxToRead,  DataSize_t& readLen);
  int                                                           readData(uint8_t clientAddress, uint8_t* data, DataSize_t maxToRead, DataSize_t& readLen);
  int                                                           peek(void);
  void                                                          flush(void);
  DataSize_t                                                    flushReceivedBuff(void);
  DataSize_t                                                    write(const char* s);
  DataSize_t                                                    write(unsigned long n);
  DataSize_t                                                    write(long n);
  DataSize_t                                                    write(int n);

protected:
  //CommI2CSocketSetting                                          comm_Socket_Config;
  uint8_t                                                       i2c_Address;
  CommI2CSocketHandler                                          i2c_Controller;
};

/*CommI2CSocketHandlerFactory*/
class CommI2CSocketHandlerFactory
{
public:
  CommI2CSocketHandlerFactory(void);
  // WARNING: if inherite from this class, deconstructor must be virtual
  __ATTRIBUTE_VIRTUAL_OPTIMIZED ~CommI2CSocketHandlerFactory(void);
public:
  static CommI2CSocketHandler                                   generate(CommI2CSocketConfigTAG& socketConfig);
  static void                                                   release(CommI2CSocketHandler& i2cController);

};

#endif // _COMM_I2C_SYSTEM_SOCKET_H

#endif // _CONF_COMM_I2C_SYSTEM_SOCKET_ENABLED