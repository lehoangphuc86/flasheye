/////////////////////////////////////////////////
// INCLUDE
#include "NetInterfaceFactory.h"

#if (_CONF_NET_INTERFACE_FACTORY_ENABLED)
#include "wifi/NetWifi.h"

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
// CLASS IMPLEMENTAION
/*NetInterfaceFactory*/
NetInterfaceFactory::NetInterfaceFactory(void)
{

}

NetInterfaceFactory::~NetInterfaceFactory(void)
{

}

NetInterface* NetInterfaceFactory::generate(byte deviceType)
{
  NetInterface* netInterface = NULL;
  do
  {
    switch (deviceType)
    {
    case NetInterfaceTypeUN::NetInterfaceWifi:
      netInterface = new NetWifi();
      break;
    default:
      break;
    }

    return netInterface;
  } while (0);

  NetInterfaceFactory::release(netInterface);
  return netInterface;
}

void NetInterfaceFactory::release(NetInterface*& netInterface)
{
  do
  {
    if (netInterface == NULL)
    {
      break;
    }
    delete netInterface;
    netInterface = NULL;
    return;
  } while (0);
  return;
}

#endif // _CONF_DISPLAY_CONTROLLER_FACTORY_ENABLED