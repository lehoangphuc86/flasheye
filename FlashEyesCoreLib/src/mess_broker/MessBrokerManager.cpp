/////////////////////////////////////////////////
// INCLUDE
#include "MessBrokerManager.h"
#if (_CONF_MESS_BROKER_MANAGER_ENABLED)
/////////////////////////////////////////////////
// PREPROCESSOR

/////////////////////////////////////////////////
// DEFINE
#define MESS_BROKER_MANAGER_CONSOLE_DEBUG_ENABLE

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
#ifdef MESS_BROKER_MANAGER_CONSOLE_DEBUG_ENABLE
char messBrokerMgrLogBuf[SYSTEM_CONSOLE_OUT_BUF_LEN];
#endif // MESS_BROKER_MANAGER_CONSOLE_DEBUG_ENABLE
/////////////////////////////////////////////////
// STATIC FUNCTIONS

/////////////////////////////////////////////////
// EXTERN

/////////////////////////////////////////////////
// CLASS IMPLEMENTAION
//
/*MessBrokerManager*/

MessBrokerManager& MessBrokerManager::getInstance(void)
{
  static MessBrokerManager instance;
  return instance;
}

MessBrokerManager::MessBrokerManager(void)
  : mess_Count(0)
  , mess_Max_Count(0)
  , mess_List(NULL)
{

}

MessBrokerManager::~MessBrokerManager(void)
{
  this->stop();
}

bool MessBrokerManager::isValid(void)
{
  SystemCriticalLocker locker(this->critical_Key);
  return (this->mess_List == NULL ? false: true);
}

int MessBrokerManager::start(MBConfigTAG& messBrokerConfig)
{
  int ret = 0;
  do
  {
    if (this->isValid() != false)
    {
      return -1; // already
    }

    this->stop();
    {
      SystemCriticalLocker locker(this->critical_Key);
      if ((messBrokerConfig.messMaxCount < MB_MESS_COUNT_MIN)
        || (messBrokerConfig.messMaxCount > MB_MESS_COUNT_MAX)
        || (messBrokerConfig.messConfig == NULL)
        )
      {
        break;
      }

      MBMessId_t wkIdx = 0;
      this->mess_Count = 0;
      this->mess_Max_Count = messBrokerConfig.messMaxCount;
      this->mess_List = new MessBrokerInfo[this->mess_Max_Count + 1];
      if (this->mess_List == NULL)
      {
        break;
      }

      wkIdx = 0;
      for (wkIdx = 0; wkIdx < this->mess_Max_Count; wkIdx++)
      {
        messBrokerConfig.messConfig[wkIdx].mbMessId = (wkIdx);
        ret = this->mess_List[wkIdx].start(messBrokerConfig.messConfig[wkIdx]);
        if (ret != 0)
        {
          break;
        }
      }

      if (wkIdx < this->mess_Max_Count)
      {
        break;
      }
    }
    
    return 0;
  } while (0);
  this->stop();
  return -1;
}

int MessBrokerManager::subscribe(MBMessId_t mbMessId, MBSubscriberConfigTAG& subConfig)
{
  int ret = 0;
  do
  {
    if (this->isValid() == false)
    {
      break;
    }

    if (mbMessId >= this->mess_Max_Count)
    {
      break;
    }

    return this->mess_List[mbMessId].subscribe(subConfig);
  } while (0);
  return -1;
}

void MessBrokerManager::unsubscribe(MBMessId_t mbMessId, MBMemberId_t subId)
{
  int ret = 0;
  do
  {
    if (this->isValid() == false)
    {
      break;
    }

    if (mbMessId >= this->mess_Max_Count)
    {
      break;
    }

    return this->mess_List[mbMessId].unsubscribe(subId);
  } while (0);
  return;
}

int MessBrokerManager::publish(MBMessId_t mbMessId, MBPublishParamsTAG& publishParams, TimePoint_t resTimeoutMs)
{
  int ret = 0;
  do
  {
    if (this->isValid() == false)
    {
      break;
    }

    if (mbMessId >= this->mess_Max_Count)
    {
      break;
    }

    return this->mess_List[mbMessId].publish(publishParams, resTimeoutMs);
  } while (0);
  return -1;
}

void MessBrokerManager::stop(void)
{
  do
  {
    SystemCriticalLocker locker(this->critical_Key);
    if (this->mess_List == NULL)
    {
      this->mess_Count = 0;
      this->mess_Max_Count = 0;
      return;
    }
    
    for (MBMessId_t wkIdx = 0; wkIdx < this->mess_Max_Count; wkIdx++)
    {
      this->mess_List[wkIdx].stop();
    }
    
    delete[] this->mess_List;
    this->mess_List = NULL;
    this->mess_Count = 0;
    this->mess_Max_Count = 0;
    return;
  } while (0);
  return;
}

#endif // _CONF_MESS_BROKER_ENABLED