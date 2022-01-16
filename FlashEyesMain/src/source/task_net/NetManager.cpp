#include "NetManager.h"
#if (_CONF_NET_MANAGER_ENABLED)
/////////////////////////////////////////////////
// INCLUDE
#include "NetInterfaceFactory.h"
/////////////////////////////////////////////////
// PREPROCESSOR

/////////////////////////////////////////////////
// DEFINE
//#define NET_MANAGER_CONSOLE_DEBUG_ENABLE

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
#ifdef NET_MANAGER_CONSOLE_DEBUG_ENABLE
char netMgrLogBuf[SYSTEM_CONSOLE_OUT_BUF_LEN];
#endif // NET_MANAGER_CONSOLE_DEBUG_ENABLE
/////////////////////////////////////////////////
// STATIC FUNCTIONS

/////////////////////////////////////////////////
// EXTERN

/////////////////////////////////////////////////
// CLASS IMPLEMENTAION
/*NetManager*/
NetManager& NetManager::getInstance(void)
{
  static NetManager instance;
  return instance;
}

NetManager::NetManager(void)
  : net_Interface(NULL)
{

}

NetManager::~NetManager(void)
{
  this->stopTask();
}

bool NetManager::isValid(void)
{
  return (this->net_Interface == NULL ? false : true);
}

bool NetManager::isEnabled(void)
{
  do
  {
    if (this->isValid() == false)
    {
      break;
    }

    return this->net_Interface->isEnabled();
  } while (0);
  return false;
}

int NetManager::getNetConfig(NetIPConfigTAG& netConfig)
{
  do
  {
    if (this->isValid() == false)
    {
      break;
    }
    return this->net_Interface->getNetConfig(netConfig);
  } while (0);
  return -1;
}

int NetManager::startTask(NetManagerConfigTAG& netTaskConfig)
{
  int ret = 0;
  do
  {
    if (this->isValid() != false)
    {
#ifdef NET_MANAGER_CONSOLE_DEBUG_ENABLE
      CONSOLE_LOG_BUF(netMgrLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[netMgr] stT %i", -1);
#endif // NET_MANAGER_CONSOLE_DEBUG_ENABLE
      return -1; //already
    }
    this->stopTask();

    this->net_Interface = NetInterfaceFactory::generate(netTaskConfig.interfaceType);
    if (this->net_Interface == NULL)
    {
#ifdef NET_MANAGER_CONSOLE_DEBUG_ENABLE
      CONSOLE_LOG_BUF(netMgrLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[netMgr] stT %i", -2);
#endif // NET_MANAGER_CONSOLE_DEBUG_ENABLE
      break;
    }

    ret = this->net_Interface->startTask(netTaskConfig.netTaskConfig);
#ifdef NET_MANAGER_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(netMgrLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[netMgr] stT %i %i", 3, ret);
#endif // NET_MANAGER_CONSOLE_DEBUG_ENABLE
    if (ret != 0)
    {
      break;
    }
#ifdef NET_MANAGER_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(netMgrLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[netMgr] stT %i", 99);
#endif // NET_MANAGER_CONSOLE_DEBUG_ENABLE
    return 0;
  } while (0);
  this->stopTask();
#ifdef NET_MANAGER_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(netMgrLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[netMgr] stT %i",-99);
#endif // NET_MANAGER_CONSOLE_DEBUG_ENABLE
  return -1;
}

int NetManager::startNet(NetManagerConnectionConfigTAG& netConnConfig, bool waitCompletion)
{
  int ret = 0;
  do
  {
    if (this->isValid() == false)
    {
      return -1; // invalid
    }

    ret = this->net_Interface->startNet(netConnConfig.netConnConfig, waitCompletion);
    if (ret != 0)
    {
      break;
    }

    return 0;
  } while (0);
  return -1;
}

void NetManager::stopNet(bool waitCompletion)
{
  do
  {
    if (this->isValid() == false)
    {
      return; // invalid
    }

    this->net_Interface->stopNet(waitCompletion);
    
    return;
  } while (0);
  return;
}

void NetManager::stopTask(void)
{
  do
  {
    if (this->isValid() != false)
    {
      this->net_Interface->stopTask();
    }

    NetInterfaceFactory::release(this->net_Interface);
    return;
  } while (0);
  return;
}
#endif // _CONF_NET_MANAGER_ENABLED