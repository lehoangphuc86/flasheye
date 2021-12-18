#include "CommSerialConstant.h"

#if (_CONF_COMM_SERIAL_SETTING_ENABLED)

#ifndef _COMM_SERIAL_SETTING_H
#define _COMM_SERIAL_SETTING_H

/////////////////////////////////////////////////
// INCLUDE

#if (!_CONF_COMM_SERIAL_CONSTANT_ENABLED)
#error Comm Serial constant is required
#endif // _CONF_COMM_SERIAL_CONSTANT_ENABLED
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
/*CommSerialCommConnectionSetting*/
class CommSerialConnectionSetting
{
public:
  static bool                                                   isValidConfig(CommSerialConfigTAG& commConnectionConfig);

public:
  CommSerialConnectionSetting(void);
  virtual ~CommSerialConnectionSetting(void);
  bool                                                          isValidConfig(void);
  bool                                                          enabled(void);
  bool                                                          isHeaderless(void);
  bool                                                          useCheckSum(void);
  bool                                                          useCodec(void);
  byte                                                          codecType(void);
  void*                                                         cbParams(void);
  CommSerialCbOnDataReceived                                    cbDataReceived(void);
  CommSerialCbOnConnectionClosed                                cbConnectionClosed(void);
  DataSize_t                                                    bufferMaxSize(void);
  DataCount_t                                                   bufferCount(void);
  bool                                                          bufferUsePool(void);
  char                                                          terminator(void);
  EventCount_t                                                  taskEventItemNumer(void);
  bool                                                          taskEventUsePool(void);
  unsigned short                                                taskStackDepth(void);
  byte                                                          taskPriority(void);
  bool                                                          taskThreadUsePool(void);
  int                                                           setConfig(CommSerialConfigTAG& commConfig);

protected:
  CommSerialConfigTAG                                           comm_Connection_Config;
};

#endif // _COMM_SERIAL_SETTING_H

#endif // _CONF_COMM_SERIAL_SETTING_ENABLED

