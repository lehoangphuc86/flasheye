#include "CommConstant.h"

#if (_CONF_COMM_CLIENT_MANAGER_ENABLED)

#ifndef _COMM_CLIENT_MANAGER_H
#define _COMM_CLIENT_MANAGER_H

/////////////////////////////////////////////////
// INCLUDE
#include "../os_system/SystemCriticalSession.h"

#if (!_CONF_COMM_CONSTANT_ENABLED)
#error Comm constant is required
#endif // _CONF_COMM_CONSTANT_ENABLED

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

/*CommClientInfo*/
class CommClientInfo
{
public:
  CommClientInfo(uint8_t commType);
  virtual ~CommClientInfo(void);
  uint8_t                                                       commType(void);
  bool                                                          compareByClientId(ClientId_t clientId);
  virtual void                                                  initClientProperties(void); // need to be overrided by children
  virtual int                                                   setClientProperties(void* clientProperties); // need to be overrided by children
  virtual bool                                                  compareByClientProperties(void* clientProperties); // need to be overrided by children
  bool                                                          isValid(void);
  void                                                          remove(void);
  void                                                          setClientId(ClientId_t clientId);
  ClientId_t                                                    clientId(void);
  void*                                                         clientProperties(void);
  void                                                          clear(void);
protected:
  void                                                          clearClientProperties(void);
protected:
  uint8_t                                                       comm_Type;
  ClientId_t                                                    client_Id;
  void*                                                         client_Properties;
};

/*CommClientManager*/
class CommClientManager
{
public:
  ////////////////////Function to call from outside ISR///////////////////////
  CommClientManager(void);
  virtual ~CommClientManager(void);
  int                                                           setConfig(ClientCount_t maxClientNumber);
  template<typename CommClientInfoType>
  int                                                           prepare(void);
  int                                                           clear(void);
  bool                                                          isValid(void);
  ClientCount_t                                                 maxClientNumber(void);
  ClientCount_t                                                 clientCount(void);
  ClientCount_t                                                 remainingClientCount(void);
  bool                                                          isEmpty(void);
  bool                                                          isFull(void);
  int                                                           appendClient(void* clientProperties);
  CommClientInfo*                                               findClient(ClientId_t clientId);
  CommClientInfo*                                               findClient(void* clientProperties);
  CommClientInfo*                                               findClient(ClientId_t clientId, void* clientProperties);
  int                                                           removeClient(ClientId_t clientId);
  int                                                           removeClient(void* clientProperties);
  int                                                           removeClient(CommClientInfo* clientInfo); 
  int                                                           removeAll(void);

  ////////////////////Function to call from inside ISR///////////////////////
  bool                                                          isValidFromISR(void);
  ClientCount_t                                                 maxClientNumberFromISR(void);
  ClientCount_t                                                 clientCountFromISR(void);
  ClientCount_t                                                 remainingClientCountFromISR(void);
  bool                                                          isEmptyFromISR(void);
  bool                                                          isFullFromISR(void);
  int                                                           appendClientFromISR(void* clientProperties);
  CommClientInfo*                                               findClientFromISR(ClientId_t clientId);
  CommClientInfo*                                               findClientFromISR(void* clientProperties);
  CommClientInfo*                                               findClientFromISR(ClientId_t clientId, void* clientProperties);
  int                                                           removeClientFromISR(ClientId_t clientId);
  int                                                           removeClientFromISR(void* clientProperties);
  int                                                           removeClientFromISR(CommClientInfo* clientInfo);
  int                                                           removeAllFromISR(void);
protected:
  ////////////////////Shared funtcion///////////////////////
  bool                                                          isValidCore(void);
  ClientCount_t                                                 maxClientNumberCore(void);
  ClientCount_t                                                 clientCountCore(void);
  ClientCount_t                                                 remainingClientCountCore(void);
  bool                                                          isEmptyCore(void);
  bool                                                          isFullCore(void);
  CommClientInfo*                                               findAvailableSlotCore(void);
  int                                                           appendClientCore(void* clientProperties);
  CommClientInfo*                                               findClientCore(ClientId_t clientId);
  CommClientInfo*                                               findClientCore(void* clientProperties);
  CommClientInfo*                                               findClientCore(ClientId_t clientId, void* clientProperties);
  int                                                           removeClientCore(ClientId_t clientId);
  int                                                           removeClientCore(void* clientProperties);
  int                                                           removeClientCore(CommClientInfo* clientInfo);
  int                                                           removeAllCore(void);
  ////void                                                          dump(void);
protected:
  ClientCount_t                                                 max_Client_Number;
  CommClientInfo*                                               client_List;
  ClientCount_t                                                 client_Count;
  ClientId_t                                                    next_Client_Id;
  SystemCriticalSession                                         critical_Session;
};

template<typename CommClientInfoType>
int CommClientManager::prepare(void)
{
  do
  {
    if (this->isValidCore() != false)
    {
      break; //already prepared
    }

    if (this->max_Client_Number <= 0)
    {
      break;
    }

    this->clear();

    this->client_List = new CommClientInfoType[this->max_Client_Number + 1];
    if (this->client_List == NULL)
    {
      break;
    }
    for (ClientCount_t wk_idx = 0; wk_idx < this->max_Client_Number; wk_idx++)
    {
      this->client_List[wk_idx].initClientProperties();
    }
    return 0;
  } while (0);
  return -1;
}


#endif // _COMM_CLIENT_MANAGER_H

#endif // _CONF_COMM_CLIENT_MANAGER_ENABLED