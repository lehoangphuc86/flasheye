/////////////////////////////////////////////////
// INCLUDE
#include "CommClientManager.h"
#if (_CONF_COMM_CLIENT_MANAGER_ENABLED)
/////////////////////////////////////////////////
// PREPROCESSOR
//#define COMM_CLIENT_MANAGER_DEBUG_CONSOLE_ENABLED
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

/* CommClientInfo*/
CommClientInfo::CommClientInfo(uint8_t commType)
  : comm_Type(commType)
  , client_Id(COMM_CLIENT_ID_INVALID)
  , client_Properties(NULL)
{
  //this->initClientProperties();
}

CommClientInfo::~CommClientInfo(void)
{
  this->clear();
}

uint8_t CommClientInfo::commType(void)
{
  return this->comm_Type;
}

bool CommClientInfo::compareByClientId(ClientId_t clientId)
{
  return (this->client_Id == clientId ? true : false);
}

bool CommClientInfo::compareByClientProperties(void* clientProperties)
{
  return true; // (this->client_Properties == clientProperties ? true : false); // need to be implemented by children
}

bool CommClientInfo::isValid(void)
{
  do
  {
    if (this->client_Id == COMM_CLIENT_ID_INVALID)
    {
      break;
    }

    return true;
  } while (0);

  return false;
}

void CommClientInfo::remove(void)
{
  do
  {
    if (this->isValid() == false)
    {
      return;
    }

    this->setClientId(COMM_CLIENT_ID_INVALID);

    return;
  } while (0);

  return;
}

void CommClientInfo::setClientId(ClientId_t clientId)
{
  this->client_Id = clientId;
}

int CommClientInfo::setClientProperties(void* clientProperties)
{
  return 0; // need to be implemented by children
}

ClientId_t CommClientInfo::clientId(void)
{
  return this->client_Id;
}

void* CommClientInfo::clientProperties(void)
{
  return this->client_Properties;
}

void CommClientInfo::clear(void)
{
  this->clearClientProperties();
  this->client_Id = COMM_CLIENT_ID_INVALID;
}

void CommClientInfo::initClientProperties(void)
{
  // need to be implemented by children
}

void CommClientInfo::clearClientProperties(void)
{
  if (this->client_Properties != NULL)
  {
    delete this->client_Properties;
  }
  this->client_Properties = NULL;
}

/*CommClientManager*/
CommClientManager::CommClientManager(void)
  : max_Client_Number(0)
  , client_List(NULL)
  , client_Count(0)
  , next_Client_Id(1)
{

}

CommClientManager::~CommClientManager(void)
{
  this->clear();
}

int CommClientManager::setConfig(ClientCount_t maxClientNumber)
{
  do
  {
    if (maxClientNumber <= 0)
    {
      break;
    }

    this->clear();
    this->max_Client_Number = maxClientNumber;
    return 0;
  } while (0);

  return -1;
}

int CommClientManager::clear(void)
{
  do
  {
    if (this->isValidCore() == false)
    {
      break;
    }

    for (ClientCount_t wk_indx = 0; wk_indx < this->max_Client_Number; wk_indx++)
    {
      this->client_List[wk_indx].clear();
    }

    delete[] this->client_List;
    this->client_List = NULL;
    //this->max_Client_Number = 0;
    this->client_Count = 0;

    return 0;
  } while (0);

  this->client_List = NULL;
  //this->max_Client_Number = 0;
  this->client_Count = 0;
  return -1;
}


////////////////////Function to call from outside ISR///////////////////////
bool CommClientManager::isValid()
{
  SystemCriticalLocker criticalLocker(this->critical_Session);
  return this->isValidCore();
}

ClientCount_t CommClientManager::maxClientNumber(void)
{
  SystemCriticalLocker criticalLocker(this->critical_Session);
  return this->maxClientNumberCore();
}

ClientCount_t CommClientManager::clientCount(void)
{
  SystemCriticalLocker criticalLocker(this->critical_Session);
  return this->clientCountCore();
}

ClientCount_t CommClientManager::remainingClientCount(void)
{
  SystemCriticalLocker criticalLocker(this->critical_Session);
  return this->remainingClientCountCore();
}

bool CommClientManager::isEmpty(void)
{
  SystemCriticalLocker criticalLocker(this->critical_Session);
  return this->isEmptyCore();
}

bool CommClientManager::isFull(void)
{
  SystemCriticalLocker criticalLocker(this->critical_Session);
  return this->isFullCore();
}


int CommClientManager::appendClient(void* clientProperties)
{
  SystemCriticalLocker criticalLocker(this->critical_Session);
  return this->appendClientCore(clientProperties);
}

CommClientInfo* CommClientManager::findClient(ClientId_t clientId)
{
  SystemCriticalLocker criticalLocker(this->critical_Session);
  return this->findClientCore(clientId);
}

CommClientInfo* CommClientManager::findClient(void* clientProperties)
{
  SystemCriticalLocker criticalLocker(this->critical_Session);
  return this->findClientCore(clientProperties);
}

CommClientInfo* CommClientManager::findClient(ClientId_t clientId, void* clientProperties)
{
  SystemCriticalLocker criticalLocker(this->critical_Session);
  return this->findClientCore(clientId, clientProperties);
}

int CommClientManager::removeClient(ClientId_t clientId)
{
  SystemCriticalLocker criticalLocker(this->critical_Session);
  return this->removeClientCore(clientId);
}

int CommClientManager::removeClient(void* clientProperties)
{
  SystemCriticalLocker criticalLocker(this->critical_Session);
  return this->removeClientCore(clientProperties);
}

int CommClientManager::removeClient(CommClientInfo* clientInfo)
{
  SystemCriticalLocker criticalLocker(this->critical_Session);
  return this->removeClientCore(clientInfo);
}

int CommClientManager::removeAll(void)
{
  SystemCriticalLocker criticalLocker(this->critical_Session);
  return this->removeAllCore();
}

////////////////////Function to call from inside ISR///////////////////////
bool CommClientManager::isValidFromISR()
{
  SystemCriticalLockerFromISR criticalLocker(this->critical_Session);
  return this->isValidCore();
}

ClientCount_t CommClientManager::maxClientNumberFromISR(void)
{
  SystemCriticalLockerFromISR criticalLocker(this->critical_Session);
  return this->maxClientNumberCore();
}

ClientCount_t CommClientManager::clientCountFromISR(void)
{
  SystemCriticalLockerFromISR criticalLocker(this->critical_Session);
  return this->clientCountCore();
}

ClientCount_t CommClientManager::remainingClientCountFromISR(void)
{
  SystemCriticalLockerFromISR criticalLocker(this->critical_Session);
  return this->remainingClientCountCore();
}

bool CommClientManager::isEmptyFromISR(void)
{
  SystemCriticalLockerFromISR criticalLocker(this->critical_Session);
  return this->isEmptyCore();
}

bool CommClientManager::isFullFromISR(void)
{
  SystemCriticalLockerFromISR criticalLocker(this->critical_Session);
  return this->isFullCore();
}


int CommClientManager::appendClientFromISR(void* clientProperties)
{
  SystemCriticalLockerFromISR criticalLocker(this->critical_Session);
  return this->appendClientCore(clientProperties);
}

CommClientInfo* CommClientManager::findClientFromISR(ClientId_t clientId)
{
  SystemCriticalLockerFromISR criticalLocker(this->critical_Session);
  return this->findClientCore(clientId);
}

CommClientInfo* CommClientManager::findClientFromISR(void* clientProperties)
{
  SystemCriticalLockerFromISR criticalLocker(this->critical_Session);
  return this->findClientCore(clientProperties);
}

CommClientInfo* CommClientManager::findClientFromISR(ClientId_t clientId, void* clientProperties)
{
  SystemCriticalLockerFromISR criticalLocker(this->critical_Session);
  return this->findClientCore(clientId, clientProperties);
}

int CommClientManager::removeClientFromISR(ClientId_t clientId)
{
  SystemCriticalLockerFromISR criticalLocker(this->critical_Session);
  return this->removeClientCore(clientId);
}

int CommClientManager::removeClientFromISR(void* clientProperties)
{
  SystemCriticalLockerFromISR criticalLocker(this->critical_Session);
  return this->removeClientCore(clientProperties);
}

int CommClientManager::removeClientFromISR(CommClientInfo* clientInfo)
{
  SystemCriticalLockerFromISR criticalLocker(this->critical_Session);
  return this->removeClientCore(clientInfo);
}

int CommClientManager::removeAllFromISR(void)
{
  SystemCriticalLockerFromISR criticalLocker(this->critical_Session);
  return this->removeAllCore();
}

////////////////////Shared funtcion///////////////////////
bool CommClientManager::isValidCore()
{
  do
  {
    if ((this->max_Client_Number <= 0)
      || (this->client_List == NULL)
      )
    {
      break;
    }

    return true;
  } while (0);
  return false;
}

ClientCount_t CommClientManager::maxClientNumberCore(void)
{
  return this->max_Client_Number;
}

ClientCount_t CommClientManager::clientCountCore(void)
{
  return this->client_Count;
}

ClientCount_t CommClientManager::remainingClientCountCore(void)
{
  return this->max_Client_Number - this->client_Count;
}

bool CommClientManager::isEmptyCore(void)
{
  return (this->client_Count == 0 ? true : false);
}

bool CommClientManager::isFullCore(void)
{
  return (this->client_Count == this->max_Client_Number ? true : false);
}

CommClientInfo* CommClientManager::findAvailableSlotCore(void)
{
  do
  {
    if (this->isValidCore() == false)
    {
      break;
    }

    for (ClientCount_t wk_indx = 0; wk_indx < this->max_Client_Number; wk_indx++)
    {
      if (this->client_List[wk_indx].isValid() == false)
      {
        return &this->client_List[wk_indx];
      }
    }

    return NULL;
  } while (0);

  return NULL;
}

int CommClientManager::appendClientCore(void* clientProperties)
{
  do
  {
    if (this->isValidCore() == false)
    {
      break;
    }

    if (this->isFullCore() != false)
    {
      break;
    }

    if (this->findClientCore(clientProperties) != NULL)
    {
      // already existing
      break;
    }

    CommClientInfo* availableSlot = this->findAvailableSlotCore();
    if (availableSlot == NULL)
    {
      break;
    }

    int ret = availableSlot->setClientProperties(clientProperties);
    if (ret != 0)
    {
      break;
    }

    availableSlot->setClientId(this->next_Client_Id);
    this->client_Count++;
    this->next_Client_Id++;
    return 0;

  } while (0);
  return -1;
}

CommClientInfo* CommClientManager::findClientCore(ClientId_t clientId)
{
  do
  {
    if (this->isValidCore() == false)
    {
      break;
    }

    CommClientInfo* candidate = NULL;
    for (ClientCount_t wk_indx = 0; wk_indx < this->max_Client_Number; wk_indx++)
    {
      candidate = &this->client_List[wk_indx];
      if (candidate->isValid() == false)
      {
        continue;
      }

      if (candidate->compareByClientId(clientId) != false)
      {
        return candidate;
      }
    }

    return NULL;
  } while (0);

  return NULL;
}

CommClientInfo* CommClientManager::findClientCore(void* clientProperties)
{
  do
  {
    if (this->isValidCore() == false)
    {
      break;
    }
    CommClientInfo* candidate = NULL;
    for (ClientCount_t wk_indx = 0; wk_indx < this->max_Client_Number; wk_indx++)
    {
      candidate = &this->client_List[wk_indx];
#ifdef COMM_CLIENT_MANAGER_DEBUG_CONSOLE_ENABLED
      CONSOLE_LOG("[clM] fid %i %i", wk_indx, candidate->clientId());
#endif // COMM_CLIENT_MANAGER_DEBUG_CONSOLE_ENABLED
      if (candidate->isValid() == false)
      {
        continue;
      }
      if (candidate->compareByClientProperties(clientProperties) != false)
      {
        return candidate;
      }
    }

    return NULL;
  } while (0);

  return NULL;
}

CommClientInfo* CommClientManager::findClientCore(ClientId_t clientId, void* clientProperties)
{
  do
  {
    if (this->isValidCore() == false)
    {
      break;
    }
    if (clientId != COMM_CLIENT_ID_INVALID)
    {
      return this->findClientCore(clientId);
    }

    return this->findClientCore(clientProperties);
  } while (0);

  return NULL;
}

int CommClientManager::removeClientCore(ClientId_t clientId)
{
  do
  {
    if (this->isValidCore() == false)
    {
      break;
    }

    CommClientInfo* clientInfo = this->findClientCore(clientId);
    if (clientInfo == NULL)
    {
      break;
    }

    clientInfo->remove();
    this->client_Count--;
    
    return 0;
  } while (0);

  return -1;
}

int CommClientManager::removeClientCore(void* clientProperties)
{
  do
  {
    if (this->isValidCore() == false)
    {
      break;
    }

    CommClientInfo* clientInfo = this->findClientCore(clientProperties);
    if (clientInfo == NULL)
    {
      break;
    }

    clientInfo->remove();
    this->client_Count--;

    return 0;
  } while (0);

  return -1;
}

int CommClientManager::removeClientCore(CommClientInfo* clientInfo)
{
  do
  {
    if (clientInfo == NULL)
    {
      break;
    }

    return this->removeClientCore(clientInfo->clientId());
  } while (0);

  return -1;
}

int CommClientManager::removeAllCore(void)
{
  do
  {
    if (this->isValidCore() == false)
    {
      break;
    }

    if (this->isEmptyCore() != false)
    {
      return 0;
    }

    CommClientInfo* candidate = NULL;
    for (ClientCount_t wk_indx = 0; wk_indx < this->max_Client_Number; wk_indx++)
    {
      candidate = &this->client_List[wk_indx];
      if (candidate->isValid() == false)
      {
        continue;
      }
      candidate->remove();
    }
    return 0;
  } while (0);

  return -1;
}
#endif // _CONF_COMM_CLIENT_MANAGER_ENABLED