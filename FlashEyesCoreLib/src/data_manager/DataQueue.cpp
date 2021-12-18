
/////////////////////////////////////////////////
// INCLUDE
#include "DataQueue.h"

#if (_CONF_DATA_QUEUE_ENABLED)
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

/* DataQueue*/
DataQueue::DataQueue(void)
  : is_Releasable_Data(true)
{

}

DataQueue::~DataQueue(void)
{
  this->clear();
}

int DataQueue::prepare(bool isReleasableData, DataCount_t queueSize)
{
  SystemCriticalLocker criticalLocker(this->critical_Key);
  do
  {
    if (this->data_Queue.isValid() != false)
    {
      break;
    }
    this->is_Releasable_Data = isReleasableData;

    return this->data_Queue.initialize(queueSize);

  } while (0);

  return -1;
}

////////////////////Function to call from inside ISR///////////////////////
bool DataQueue::isValid(void)
{
  SystemCriticalLocker criticalLocker(this->critical_Key);
  return this->data_Queue.isValid();
}

bool DataQueue::isReleasableData(void)
{
  SystemCriticalLocker criticalLocker(this->critical_Key);
  return this->is_Releasable_Data;
}

void DataQueue::isReleasableData(bool val)
{
  SystemCriticalLocker criticalLocker(this->critical_Key);
  this->is_Releasable_Data = val;
}

bool DataQueue::empty(void)
{
  SystemCriticalLocker criticalLocker(this->critical_Key);
  return this->data_Queue.empty();
}

DataCount_t DataQueue::size(void)
{
  SystemCriticalLocker criticalLocker(this->critical_Key);
  return this->data_Queue.size();
}

DataItem* DataQueue::front(void)
{
  SystemCriticalLocker criticalLocker(this->critical_Key);
  do
  {
    DataItem** dataItemPointer = NULL;
    dataItemPointer = this->data_Queue.front();
    if (dataItemPointer == NULL)
    {
      break;
    }
    
    return *dataItemPointer;

  } while (0);

  return NULL;
}


int DataQueue::push(DataItem* val)
{
  SystemCriticalLocker criticalLocker(this->critical_Key);
  return this->data_Queue.push(val);
}

int DataQueue::pop(void)
{
  SystemCriticalLocker criticalLocker(this->critical_Key);
  return this->data_Queue.pop();
}

DataItem* DataQueue::get(void) // front then pop
{
  SystemCriticalLocker criticalLocker(this->critical_Key);
  do
  {
    DataItem** dataItemPointer = NULL;
    dataItemPointer = this->data_Queue.get();
    if (dataItemPointer == NULL)
    {
      break;
    }

    return *dataItemPointer;

  } while (0);

  return NULL;
}

int DataQueue::remove(DataItem* val)
{
  SystemCriticalLocker criticalLocker(this->critical_Key);
  return this->data_Queue.remove(val);
}


int DataQueue::clear(void)
{
  SystemCriticalLocker criticalLocker(this->critical_Key);
  return this->clearData();
}


#ifdef SYSTEM_QUEUE_DUMP_ENABLE
void DataQueue::dump(void)
{
  SystemCriticalLocker criticalLocker(this->critical_Key);
  return this->data_Queue.dump();
}
#endif // SYSTEM_QUEUE_DUMP_ENABLE


////////////////////Function to call from inside ISR///////////////////////

/// <summary>
/// 
/// </summary>
/// <param name="higherTaskWoken"></param>
/// <returns></returns>
bool DataQueue::isValidFromISR(void)
{
  return this->data_Queue.isValidFromISR();
}

bool DataQueue::isReleasableDataFromISR(void)
{
  SystemCriticalLockerFromISR criticalLocker(this->critical_Key);
  return this->is_Releasable_Data;
}

void DataQueue::isReleasableDataFromISR(bool val)
{
  SystemCriticalLockerFromISR criticalLocker(this->critical_Key);
  this->is_Releasable_Data = val;
}

bool DataQueue::emptyFromISR(void)
{
  SystemCriticalLockerFromISR criticalLocker(this->critical_Key);
  return this->data_Queue.emptyFromISR();
}

DataCount_t DataQueue::sizeFromISR(void)
{
  SystemCriticalLockerFromISR criticalLocker(this->critical_Key);
  return this->data_Queue.sizeFromISR();
}

DataItem* DataQueue::frontFromISR(void)
{
  SystemCriticalLockerFromISR criticalLocker(this->critical_Key);
  do
  {
    DataItem** dataItemPointer = NULL;
    dataItemPointer = this->data_Queue.frontFromISR();
    if (dataItemPointer == NULL)
    {
      break;
    }

    return *dataItemPointer;

  } while (0);

  return NULL;
}

int DataQueue::pushFromISR(DataItem* val)
{
  SystemCriticalLockerFromISR criticalLocker(this->critical_Key);
  return this->data_Queue.pushFromISR(val);
}

int DataQueue::popFromISR(void)
{
  SystemCriticalLockerFromISR criticalLocker(this->critical_Key);
  return this->data_Queue.popFromISR();
}

DataItem* DataQueue::getFromISR(void)
{
  SystemCriticalLockerFromISR criticalLocker(this->critical_Key);
  do
  {
    DataItem** dataItemPointer = NULL;
    dataItemPointer = this->data_Queue.getFromISR();
    if (dataItemPointer == NULL)
    {
      break;
    }

    return *dataItemPointer;

  } while (0);

  return NULL;
}

int DataQueue::removeFromISR(DataItem* val)
{
  SystemCriticalLockerFromISR criticalLocker(this->critical_Key);
  return this->data_Queue.removeFromISR(val);
}

#ifdef SYSTEM_QUEUE_DUMP_ENABLE
void DataQueue::dumpFromISR(void)
{
  SystemCriticalLockerFromISR criticalLocker(this->critical_Key);
  return this->data_Queue.dumpFromISR();
}
#endif // SYSTEM_QUEUE_DUMP_ENABLE


//################## CORE //##################
//CORE
/// <summary>
/// 
/// </summary>
/// <param name=""></param>
/// <returns></returns>
int DataQueue::clearData(void)
{
  do
  {

    if (this->data_Queue.empty() != false)
    {
      break;
    }

    if (this->is_Releasable_Data == false)
    {
      break;
    }

    //DataItem* dataItem = NULL;
    
    while (this->data_Queue.empty() == false)
    {
      /*dataItem = (*this->data_Queue.front());
      if (dataItem != NULL)
      {
        delete dataItem; //@@ should be done by data manager
      }*/
      this->data_Queue.pop();
    }

    this->data_Queue.reset();

    return 0;
  } while (0);
  
  this->data_Queue.reset();
  return -1;
}


#endif // #(_CONF_DATA_QUEUE_ENABLED)