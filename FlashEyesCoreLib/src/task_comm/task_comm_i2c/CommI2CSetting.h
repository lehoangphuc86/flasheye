#include "CommI2CConstant.h"

#if (_CONF_COMM_I2C_COMM_SETTING_ENABLED)

#ifndef _COMM_I2C_COMM_SETTING_H
#define _COMM_I2C_COMM_SETTING_H

/////////////////////////////////////////////////
// INCLUDE

#if (!_CONF_COMM_I2C_CONSTANT_ENABLED)
#error Comm I2C constant is required
#endif // _CONF_COMM_I2C_CONSTANT_ENABLED
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

/*CommI2CSocketSetting*/
//class CommI2CSocketSetting
//{
//public:
//  static bool                                                   isValidConfig(CommI2CSocketConfigTAG& commConnectionConfig);
//
//public:
//  CommI2CSocketSetting(void);
//  virtual ~CommI2CSocketSetting(void);
//  bool                                                          isValidConfig(void);
//  int                                                           setConfig(CommI2CSocketConfigTAG& commConfig);
//  bool                                                          enabled(void);
//  byte                                                          busNumer(void);
//  byte                                                          pinSCL(void);
//  byte                                                          pinSDA(void);
//  uint32_t                                                      frequency(void);
//  byte                                                          opMode(void);
//  uint8_t                                                       address(void);
//  uint16_t                                                      timeout(void);
//protected:
//  CommI2CSocketConfigTAG                                        comm_Socket_Config;
//};

/*CommI2CCommConnectionSetting*/
class CommI2CConnectionSetting
{
public:
  static bool                                                   isValidConfig(CommI2CConfigTAG& commConnectionConfig);

public:
  CommI2CConnectionSetting(void);
  virtual ~CommI2CConnectionSetting(void);
  bool                                                          isValidConfig(void);
  bool                                                          enabled(void);
  bool                                                          useOnrequestCallback(void);
  //byte                                                          i2cProtocol(void);
  bool                                                          useCheckSum(void);
  bool                                                          useCodec(void);
  byte                                                          codecType(void);
  bool                                                          isServerMode(void);
  bool                                                          isHeaderLess(void);
  void*                                                         parentPointer(void);
  CommI2CCbOnDataReceived                                       cbDataReceived(void);
  CommI2CCbOnDataRequested                                      cbDataRequested(void);
  DataSize_t                                                    bufferMaxSize(void);
  DataCount_t                                                   bufferCount(void);
  bool                                                          bufferUsePool(void);
  int                                                           setConfig(CommI2CConfigTAG& commConfig);

protected:
  CommI2CConfigTAG                                              comm_Connection_Config;
};

#endif // _COMM_I2C_COMM_SETTING_H

#endif // _CONF_COMM_I2C_COMM_SETTING_ENABLED