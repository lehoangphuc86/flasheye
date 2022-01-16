#include "NetManagerConstant.h"

#if (_CONF_NET_INTERFACE_ENABLED)

#ifndef _NET_INTERFACE_H
#define _NET_INTERFACE_H

/////////////////////////////////////////////////
// INCLUDE

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

/*NetInterface*/

class NetInterface
{
public:
  NetInterface(byte type);
  virtual ~NetInterface(void);
  virtual bool                                                  isEnabled(void) = 0;
  virtual int                                                   getNetConfig(NetIPConfigTAG& netConfig) = 0;
  virtual int                                                   startTask(NetTaskConfigTAG& netTaskConfig) = 0;
  virtual int                                                   startNet(NetConnectionConfigTAG& netConnConfig, bool waitCompletion = true) = 0;
  virtual void                                                  stopNet(bool waitCompletion = true) = 0;
  virtual void                                                  stopTask(void) = 0;

protected:
  byte                                                          interface_Type;
};

#endif // _NET_INTERFACE_H

#endif // _CONF_NET_INTERFACE_ENABLED