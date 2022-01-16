#ifndef _NET_IP_CONSTANT_H
#define _NET_IP_CONSTANT_H

#include "../../FlashEyesMain.h"
#if (_CONF_NET_IP_CONSTANT_ENABLED)
/////////////////////////////////////////////////
// INCLUDE
// 
/////////////////////////////////////////////////
// PREPROCESSOR

/////////////////////////////////////////////////
// DEFINE

/////////////////////////////////////////////////
// MARCO

/////////////////////////////////////////////////
// DATA TYPE (TYPEDEF)

/////////////////////////////////////////////////
// DATA TYPE (ENUM)

/////////////////////////////////////////////////
// DATA TYPE (STRUCT)

typedef struct _netIPConfigTAG
{
  bool enabledStaticIP : 1;
  byte reserverd : 7;
  uint32_t IPV4;
  uint32_t subnetMask;
  uint32_t defaultGW;
  _netIPConfigTAG(void)
  {
    this->enabledStaticIP = 0;
    this->IPV4 = 0;;
    this->subnetMask = 0;
    this->defaultGW = 0;
  }

  bool useStaticIP(void)
  {
    return this->enabledStaticIP;
  }


  bool isValidStaticIP(void)
  {
    do
    {
      if (this->enabledStaticIP == 0)
      {
        return true;
      }

      if ((this->IPV4 == 0)
        || (this->subnetMask == 0)
        || (this->defaultGW == 0)
        )
      {
        break;
      }

      return true;
    } while (0);
    return false;
  }
} NetIPConfigTAG;

/////////////////////////////////////////////////
// GLOBAL VARIABLES

/////////////////////////////////////////////////
// GLOBAL FUNCTIONS

/////////////////////////////////////////////////
// STATIC DATA

/////////////////////////////////////////////////
// STATIC FUNCTIONS

/////////////////////////////////////////////////
// EXTERN

/////////////////////////////////////////////////
// CLASS DEFINITION

#endif // _CONF_NET_IP_CONSTANT_ENABLED

#endif // _NET_IP_CONSTANT_H