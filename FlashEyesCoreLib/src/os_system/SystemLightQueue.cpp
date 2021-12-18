#ifndef _SYSTEM_LIGHT_QUEUE_CPP
#define _SYSTEM_LIGHT_QUEUE_CPP
/////////////////////////////////////////////////
// INCLUDE
#include "SystemLightQueue.h"
#if (_CONF_SYSTEM_LIGHT_QUEUE_ENABLED)
#include "SystemCommon.h"
//#include "portable.h"
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

/* SystemLightQueue*/
template <typename DataItemT>
SystemLightQueue<DataItemT>::SystemLightQueue(void)
  : //data_Size(sizeof(DataItemT))
  //, node_Size(sizeof(LinkedNode<DataItemT>))
   prepared_Queue_Size(0)
  , current_Queue_Size(0)
  , node_Buffer(NULL)
  , queue_Start(NULL)
  , queue_End(NULL)
{
  
}

template <typename DataItemT>
SystemLightQueue<DataItemT>::~SystemLightQueue(void)
{
  this->clearDataCore();
}

template <typename DataItemT>
int SystemLightQueue<DataItemT>::initialize(DataCount_t preparedQueueSize)
{
  //SystemCriticalLocker criticalLocker(this->critical_Key); //@@
  return this->createDataCore(preparedQueueSize);
}

////////////////////Function to call from outside ISR///////////////////////
template <typename DataItemT>
bool SystemLightQueue<DataItemT>::isValid(void)
{
  //SystemCriticalLocker criticalLocker(this->critical_Key); //@@
  return this->isValidCore();
}

template <typename DataItemT>
bool SystemLightQueue<DataItemT>::empty(void)
{
  //SystemCriticalLocker criticalLocker(this->critical_Key); //@@
  return this->emptyCore();
}

template <typename DataItemT>
DataCount_t SystemLightQueue<DataItemT>::size(void)
{
  //SystemCriticalLocker criticalLocker(this->critical_Key); //@@
  return this->sizeCore();
}
 
template <typename DataItemT>
DataItemT* SystemLightQueue<DataItemT>::front(void)
{
  //SystemCriticalLocker criticalLocker(this->critical_Key); //@@
  return this->frontCore();
}

template <typename DataItemT>
int SystemLightQueue<DataItemT>::push(const DataItemT& val)
{
  //SystemCriticalLocker criticalLocker(this->critical_Key); //@@
  return this->pushCore(val);
}

template <typename DataItemT>
int SystemLightQueue<DataItemT>::pop(void)
{
  //SystemCriticalLocker criticalLocker(this->critical_Key); //@@
  return this->popCore();
}

template <typename DataItemT>
DataItemT* SystemLightQueue<DataItemT>::get(void)
{
  //SystemCriticalLocker criticalLocker(this->critical_Key); //@@
  return this->getCore();
}

template <typename DataItemT>
int SystemLightQueue<DataItemT>::remove(const DataItemT& val)
{
  //SystemCriticalLocker criticalLocker(this->critical_Key); //@@
  return this->removeCore(val);
}

template <typename DataItemT>
int SystemLightQueue<DataItemT>::reset(void)
{
  //SystemCriticalLocker criticalLocker(this->critical_Key); //@@
  return this->resetCore();
}

#ifdef SYSTEM_QUEUE_DUMP_ENABLE
template <typename DataItemT>
void SystemLightQueue<DataItemT>::dump(void)
{
  //SystemCriticalLocker criticalLocker(this->critical_Key); //@@
  return this->dumpCore();
}
#endif // SYSTEM_QUEUE_DUMP_ENABLE


////////////////////Function to call from inside ISR///////////////////////
// fromISR
template <typename DataItemT>
bool SystemLightQueue<DataItemT>::isValidFromISR(void)
{
  //SystemCriticalLockerFromISR criticalLocker(this->critical_Key); //@@
  return this->isValidCore();
}

template <typename DataItemT>
bool SystemLightQueue<DataItemT>::emptyFromISR(void)
{
  //SystemCriticalLockerFromISR criticalLocker(this->critical_Key); //@@
  return this->emptyCore();
}

template <typename DataItemT>
DataCount_t SystemLightQueue<DataItemT>::sizeFromISR(void)
{
  //SystemCriticalLockerFromISR criticalLocker(this->critical_Key); //@@
  return this->sizeCore();
}

template <typename DataItemT>
DataItemT* SystemLightQueue<DataItemT>::frontFromISR(void)
{
  //SystemCriticalLockerFromISR criticalLocker(this->critical_Key); //@@
  return this->frontCore();
}

template <typename DataItemT>
int SystemLightQueue<DataItemT>::pushFromISR(const DataItemT& val)
{
  //SystemCriticalLockerFromISR criticalLocker(this->critical_Key); //@@
  return this->pushCore(val);
}

template <typename DataItemT>
int SystemLightQueue<DataItemT>::popFromISR(void)
{
  //SystemCriticalLockerFromISR criticalLocker(this->critical_Key); //@@
  return this->popCore();
}

template <typename DataItemT>
DataItemT* SystemLightQueue<DataItemT>::getFromISR(void)
{
  //SystemCriticalLockerFromISR criticalLocker(this->critical_Key); //@@
  return this->getCore();
}

template <typename DataItemT>
int SystemLightQueue<DataItemT>::removeFromISR(const DataItemT& val)
{
  //SystemCriticalLockerFromISR criticalLocker(this->critical_Key); //@@
  return this->removeCore(val);
}

template <typename DataItemT>
int SystemLightQueue<DataItemT>::resetFromISR(void)
{
  //SystemCriticalLockerFromISR criticalLocker(this->critical_Key); //@@
  return this->resetCore();
}

#ifdef SYSTEM_QUEUE_DUMP_ENABLE
template <typename DataItemT>
void SystemLightQueue<DataItemT>::dumpFromISR(void)
{
  //SystemCriticalLockerFromISR criticalLocker(this->critical_Key); //@@
  return this->dumpCore();
}
#endif // SYSTEM_QUEUE_DUMP_ENABLE


////////////////////Core functions without locking///////////////////////
template <typename DataItemT>
bool SystemLightQueue<DataItemT>::isFullCore(void)
{
  // if it is not prepared, it returns true as well.
  return (this->current_Queue_Size >= this->prepared_Queue_Size ? true : false);
}

template <typename DataItemT>
LinkedNode<DataItemT>* SystemLightQueue<DataItemT>::getNodeCore(DataCount_t index)
{
  LinkedNode<DataItemT>* candidateNode = NULL;
  do
  {
    // check empty and index out of range
    if (index >= this->current_Queue_Size)
    {
      break;
    }

    // it is not empty, so the start node is not null.
    candidateNode = this->queue_Start;

    DataCount_t count = 0;
    while (count < index)
    {
      count++;
      candidateNode++;
    }

    return candidateNode;
  } while (0);
  
  return NULL;
}

template <typename DataItemT>
LinkedNode<DataItemT>* SystemLightQueue<DataItemT>::getAvailableNodeBufferCore(void)
{
  LinkedNode<DataItemT>* candidateNode = NULL;
  do
  {
    if (this->isFullCore() != false)
    {
      break;
    }
    // it is not full, so there is surely an available node.
    candidateNode = ((LinkedNode<DataItemT>*) (this->node_Buffer));
    while (candidateNode->next != NULL)
    {
      candidateNode++;
    }

    return candidateNode;
  } while (0);

  return NULL;
}


template <typename DataItemT>
int SystemLightQueue<DataItemT>::createDataCore(DataCount_t queueSize)
{
  do
  {
    this->clearDataCore();
    if (queueSize <= 0)
    {
      break;
    }
    //this->node_Buffer = new LinkedNode<DataItemT>[queueSize];
    //size_t memSize = queueSize * this->node_Size;
    size_t memSize = queueSize * sizeof(LinkedNode<DataItemT>);
    //this->node_Buffer = (LinkedNode<DataItemT>*) pvPortMalloc(memSize); // (LinkedNode<DataItemT>*) new unsigned char[memSize];// 
    this->node_Buffer = (LinkedNode<DataItemT>*) new unsigned char[memSize];// 
    if (this->node_Buffer == NULL)
    {
      break;
    }
    this->prepared_Queue_Size = queueSize;

    return this->resetCore();
  } while (0);
  
  this->clearDataCore();
  return -1;
}

template <typename DataItemT>
int SystemLightQueue<DataItemT>::clearDataCore(void)
{
  do
  {
    if (this->isValidCore() == false)
    {
      // not created yet
      break;
    }

    free(this->node_Buffer);
    
    this->node_Buffer = NULL;
    this->prepared_Queue_Size = 0;
    this->current_Queue_Size = 0;
    this->queue_Start = NULL;
    this->queue_End = NULL;

    return 0;
  } while (0);
  
  this->node_Buffer = NULL;
  this->prepared_Queue_Size = 0;
  this->current_Queue_Size = 0;
  this->queue_Start = NULL;
  this->queue_End = NULL;

  return -1;

}

template <typename DataItemT>
bool SystemLightQueue<DataItemT>::isValidCore(void)
{
  return (this->node_Buffer == NULL ? false : true);
}

template <typename DataItemT>
bool SystemLightQueue<DataItemT>::emptyCore(void) const
{
  return (this->current_Queue_Size <= 0 ? true : false);
}


template <typename DataItemT>
DataCount_t SystemLightQueue<DataItemT>::sizeCore(void)
{
  return this->current_Queue_Size;
}

template <typename DataItemT>
DataItemT* SystemLightQueue<DataItemT>::frontCore(void)
{
  do
  {
    if (this->emptyCore() != false)
    {
      break;
    }

    return &this->queue_Start->data;
  } while (0);

  return NULL;
}

template <typename DataItemT>
int SystemLightQueue<DataItemT>::pushCore(const DataItemT& val)
{
  do
  {
    // find the available buffer for this new node. if it is full, it returns null
    LinkedNode<DataItemT>* availableNode = this->getAvailableNodeBufferCore();
    if (availableNode == NULL)
    {
      break;
    }

    //memcpy(&availableNode->data, &val, this->data_Size);
    memcpy(&availableNode->data, &val, sizeof(DataItemT));
    if (this->emptyCore() != false)
    {
      this->queue_Start = availableNode;
      this->queue_End = this->queue_Start;
    }
    this->queue_End->next = availableNode;
    this->queue_End = availableNode;
    this->queue_End->next = this->queue_End;
    this->current_Queue_Size++;
    return 0;
  } while (0);
  return -1;
}

template <typename DataItemT>
int SystemLightQueue<DataItemT>::popCore(void)
{
  do
  {
    if (this->emptyCore() != false)
    {
      break;
    }
    LinkedNode<DataItemT>* nextNode = this->queue_Start->next;
    this->queue_Start->next = NULL;
    this->queue_Start = nextNode;
    this->current_Queue_Size--;

    return 0;
  } while (0);

  return -1;
}

template <typename DataItemT>
DataItemT* SystemLightQueue<DataItemT>::getCore(void)
{
  do
  {
    DataItemT* result = this->frontCore();
    if (result == NULL)
    {
      break;
    }

    this->popCore();
    
    return result;
  } while (0);

  return NULL;
}

template <typename DataItemT>
int SystemLightQueue<DataItemT>::removeCore(const DataItemT& val)
{
  int result = 0;
  do
  {
    if (this->emptyCore() != false)
    {
      break;
    }

    if (this->queue_Start->data == val)
    {
      result = this->popCore();
      return result;
    }

    // it is not empty, so the start node is not null.
    LinkedNode<DataItemT>* previousNode = this->queue_Start;
    LinkedNode<DataItemT>* candidateNode = NULL;
    LinkedNode<DataItemT>* foundNode = NULL;

    do
    {
      candidateNode = previousNode->next;
      if (candidateNode->data == val)
      {
        foundNode = candidateNode;
        break;
      }
      previousNode = candidateNode;
    } while (candidateNode != this->queue_End);

    if (foundNode == NULL)
    {
      break;
    }

    if (foundNode == this->queue_End)
    {
      previousNode->next = previousNode;
      this->queue_End = previousNode;
    }
    else
    {
      previousNode->next = candidateNode->next;
    }

    candidateNode->next = NULL;
    this->current_Queue_Size--;
    return 0;
  } while (0);

  return -1;
}

template <typename DataItemT>
int SystemLightQueue<DataItemT>::resetCore(void)
{
  do
  {
    if (this->isValidCore() == false)
    {
      break;
    }

    memset(this->node_Buffer, 0, sizeof(LinkedNode<DataItemT>) * this->prepared_Queue_Size);
    this->current_Queue_Size = 0;
    this->queue_Start = (LinkedNode<DataItemT>*)this->node_Buffer;
    this->queue_End = queue_Start;

    return 0;
  } while (0);

  return -1;
}



#ifdef SYSTEM_QUEUE_DUMP_ENABLE
template <typename DataItemT>
void SystemLightQueue<DataItemT>::dumpCore(void)
{
  LinkedNode<DataItemT>* candidateNode = NULL;
  SYSTEM_CONSOLE("s=%i/%i", this->current_Queue_Size, this->prepared_Queue_Size);
  candidateNode = this->queue_Start;
  unsigned int count = 0;
  while (count < this->current_Queue_Size)
  {
    SYSTEM_CONSOLE("%i", (int)candidateNode->data);
    candidateNode = candidateNode->next;
    count++;
  }
}
#endif // SYSTEM_QUEUE_DUMP_ENABLE

#endif // _CONF_SYSTEM_LIGHT_QUEUE_ENABLED

#endif // _SYSTEM_LIGHT_QUEUE_CPP

