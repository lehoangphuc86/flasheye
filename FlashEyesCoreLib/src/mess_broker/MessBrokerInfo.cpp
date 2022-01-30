/////////////////////////////////////////////////
// INCLUDE
#include "MessBrokerInfo.h"
#if (_CONF_MESS_BROKER_INFO_ENABLED)
#include "../timer_manager/TimerManager.h"
/////////////////////////////////////////////////
// PREPROCESSOR

/////////////////////////////////////////////////
// DEFINE
#define MESS_BROKER_INFO_CONSOLE_DEBUG_ENABLE

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
#ifdef MESS_BROKER_INFO_CONSOLE_DEBUG_ENABLE
char messBrokerInfoLogBuf[SYSTEM_CONSOLE_OUT_BUF_LEN];
#endif // MESS_BROKER_INFO_CONSOLE_DEBUG_ENABLE
/////////////////////////////////////////////////
// STATIC FUNCTIONS

/////////////////////////////////////////////////
// EXTERN

/////////////////////////////////////////////////
// CLASS IMPLEMENTAION
//
/*MessBrokerInfo*/
MessBrokerInfo::MessBrokerInfo(void)
  : sub_Count(0)
  , sub_Max_Count(0)
  , sub_List(NULL)
{

}

MessBrokerInfo::~MessBrokerInfo(void)
{
  this->stop();
}

bool MessBrokerInfo::isValid(void)
{
  SystemCriticalLocker locker(this->critical_key);
  return (this->sub_List == NULL ? false: true);
}

int MessBrokerInfo::start(MBMessConfigTAG& messConfig)
{
  do
  {
    if (this->isValid() != false)
    {
      return -1; // already
    }

    this->stop();
    {
      SystemCriticalLocker locker(this->critical_key);
      if ((messConfig.subMaxCount < MB_SUB_PER_MESS_COUNT_MIN)
        || (messConfig.subMaxCount > MB_SUB_PER_MESS_COUNT_MAX)
        || (messConfig.mbMessId < MB_MESS_ID_MIN)
        || (messConfig.mbMessId > MB_MESS_ID_MAX)
        )
      {
        break;
      }

      this->sub_Count = 0;
      this->sub_Max_Count = messConfig.subMaxCount;
      this->mess_Id = messConfig.mbMessId;
      this->sub_List = new MBSubscriberInfoTAG[this->sub_Max_Count + 1];
      if (this->sub_List == NULL)
      {
        break;
      }
      memset(this->sub_List, 0, sizeof(MBSubscriberInfoTAG) * (this->sub_Max_Count + 1));
    }
    
    return 0;
  } while (0);
  this->stop();
  return -1;
}

int MessBrokerInfo::subscribe(MBSubscriberConfigTAG& subConfig)
{
  int ret = 0;
  do
  {
    if (this->isValid() == false)
    {
      break;
    }

    {
      SystemCriticalLocker locker(this->critical_key);
      if (this->sub_Count >= this->sub_Max_Count)
      {
        break;
      }

      if (subConfig.cb == NULL)
      {
        break;
      }

      
      MBSubscriberInfoTAG* foundSub = this->findSub(subConfig.subId);
      if (foundSub != NULL)
      {
        return 0; //already
      }

      foundSub = this->findSub(MB_MEMBER_ID_INVALID); // find empty slot
      if (foundSub == NULL)
      {
        break; //something wrong
      }

      foundSub->subId = subConfig.subId;
      foundSub->cb = subConfig.cb;
      foundSub->arg = subConfig.arg;
      foundSub->pattern = subConfig.pattern;
      this->sub_Count++;
    }

    return 0;
  } while (0);
  return -1;
}


void MessBrokerInfo::unsubscribe(MBMemberId_t subId)
{
  int ret = 0;
  do
  {
    if (this->isValid() == false)
    {
      break;
    }

    {
      SystemCriticalLocker locker(this->critical_key);
      if (this->sub_Count <= 0)
      {
        break;
      }

      MBSubscriberInfoTAG* foundSub = this->findSub(subId);
      if (foundSub == NULL)
      {
        return; // not found
      }

      foundSub->subId = MB_MEMBER_ID_INVALID;
      foundSub->arg = NULL;
      foundSub->cb = NULL;
      foundSub->pattern = MB_MATCH_PATTERN_ALL;
      this->sub_Count--;
    }

    return;
  } while (0);
  return;
}


int MessBrokerInfo::publish(MBPublishParamsTAG& publishParams, TimePoint_t resTimeoutMs)
{
  int ret = 0;
  do
  {
    if (this->isValid() == false)
    {
      break;
    }

    this->res_Wait_Key.unlock();
    this->res_Wait_Key.lock();
    MBMemberId_t subscriberNum = 0;
    {
      SystemCriticalLocker locker(this->critical_key);
      if (this->sub_Count <= 0)
      {
        break;
      }

      MBMemberId_t wkidx = 0; 
      MBNotifiedInfoTAG subNotifiedInfo = MBNotifiedInfoTAG();
      memcpy(&subNotifiedInfo.publishData, &publishParams, sizeof(MBPublishParamsTAG));
      subNotifiedInfo.mbMessId = this->mess_Id;
      subNotifiedInfo.resSpec.expiredTime = 0;
      subNotifiedInfo.resSpec.mutex = NULL;
      if (resTimeoutMs > 0)
      {
        resTimeoutMs = SYSTEM_MAX(MB_PUBLISH_RES_WAIT_TIME_MIN, resTimeoutMs);
        subNotifiedInfo.resSpec.expiredTime = TimerManager::getInstance().nowMs() + resTimeoutMs - MB_PUBLISH_RES_WAIT_TIME_COFF;
        subNotifiedInfo.resSpec.mutex = &this->res_Wait_Key;
      }

      for (wkidx = 0; wkidx< this->sub_Max_Count; wkidx++)
      {
        if (this->sub_List[wkidx].isValid() == false)
        {
          continue;
        }

        if ( (this->sub_List[wkidx].pattern & publishParams.pattern) == 0)
        {
          continue;
        }
        this->sub_List[wkidx].cb(subNotifiedInfo);
        subscriberNum++;
      }
    }

    if ( (subscriberNum >0 )
      && (resTimeoutMs > 0))
    {
      this->res_Wait_Key.lock(resTimeoutMs);
    }

    return 0;
  } while (0);
  return -1;
}

void MessBrokerInfo::stop(void)
{
  do
  {
    SystemCriticalLocker locker(this->critical_key);
    if (this->sub_List == NULL)
    {
      this->sub_Count = 0;
      this->sub_Max_Count = 0;
      this->mess_Id = MB_MESS_ID_INVALID;
      return;
    }
    
    delete[] this->sub_List;
    this->sub_List = NULL;
    this->sub_Count = 0;
    this->sub_Max_Count = 0;
    this->mess_Id = MB_MESS_ID_INVALID;
    return;
  } while (0);
  return;
}

MBSubscriberInfoTAG* MessBrokerInfo::findSub(MBMemberId_t subId)
{
  do
  {
    if (this->sub_List == NULL)
    {
      break;
    }

    for (MBMemberId_t wkidx = 0; wkidx < this->sub_Max_Count; wkidx++)
    {
      if (this->sub_List[wkidx].subId == subId)
      {
        return &this->sub_List[wkidx];
      }
    }

    return NULL;
  } while (0);
  return NULL;
}

#endif // _CONF_MESS_BROKER_INFO_ENABLED