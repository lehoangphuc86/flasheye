#include "NetManagerConstant.h"
#ifndef _NET_MANAGER_H
#define _NET_MANAGER_H
#if (_CONF_NET_MANAGER_ENABLED)
/////////////////////////////////////////////////
// INCLUDE
#include "NetInterface.h"
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

/*NetManager*/
class NetManager
{
private:
  NetManager(void);
  __ATTRIBUTE_VIRTUAL_OPTIMIZED ~NetManager(void);

public:
  static NetManager& getInstance(void);
  NetManager(NetManager const&) = delete;
  void operator=(NetManager const&) = delete;
  bool                                                          isValid(void);
  bool                                                          isEnabled(void);
  int                                                           getNetConfig(NetIPConfigTAG& netConfig);
  int                                                           startTask(NetManagerConfigTAG& netTaskConfig);
  int                                                           startNet(NetManagerConnectionConfigTAG& netConnConfig, bool waitCompletion = true);
  void                                                          stopNet(bool waitCompletion = true);
  void                                                          stopTask(void);
protected:
  NetInterface*                                                 net_Interface;
};
#endif // _CONF_NET_MANAGER_ENABLED

#endif // _NET_MANAGER_H