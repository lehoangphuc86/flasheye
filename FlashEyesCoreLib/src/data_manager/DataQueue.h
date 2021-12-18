#include "DataManagerConstant.h"

#if (_CONF_DATA_QUEUE_ENABLED)
#ifndef _DATA_QUEUE_H
#define _DATA_QUEUE_H

/////////////////////////////////////////////////
// INCLUDE

#include "DataItem.h"
#include "../os_system/SystemLightQueue.h"
#include "../os_system/SystemLightQueue.cpp"

#if (!_CONF_DATA_ITEM_ENABLED)
#error Data item is required
#endif // 

#if (!_CONF_SYSTEM_LIGHT_QUEUE_ENABLED)
#error Lighting queue is required
#endif // 
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

/*DataQueue*/

class DataQueue
{
public:
  DataQueue(void);
  // WARNING: if inherite from this class, deconstructor must be virtual
  __ATTRIBUTE_VIRTUAL_OPTIMIZED ~DataQueue(void);

  int                                                           prepare(bool isReleasableData, DataCount_t queueSize); // queueSize is used for Ardunio platform only

  ////////////////////Function to call from outside ISR///////////////////////
  bool                                                          isValid(void);
  bool                                                          isReleasableData(void);
  void                                                          isReleasableData(bool val);
  bool                                                          empty(void);
  DataCount_t                                                   size(void);

  DataItem*                                                     front(void);
  int                                                           push(DataItem* val);
  int                                                           pop(void);
  DataItem*                                                     get(void); // front then pop
  int                                                           remove(DataItem* val);
  int                                                           clear(void);
#ifdef SYSTEM_QUEUE_DUMP_ENABLE
  void                                                          dump(void);
#endif // SYSTEM_QUEUE_DUMP_ENABLE


  ////////////////////Function to call from inside ISR///////////////////////
  bool                                                          isValidFromISR(void);
  bool                                                          isReleasableDataFromISR(void);
  void                                                          isReleasableDataFromISR(bool val);
  bool                                                          emptyFromISR(void);
  DataCount_t                                                   sizeFromISR(void);

  DataItem*                                                     frontFromISR(void);
  int                                                           pushFromISR(DataItem* val);
  int                                                           popFromISR(void);
  DataItem*                                                     getFromISR(void); // front then pop
  int                                                           removeFromISR(DataItem* val);
#ifdef SYSTEM_QUEUE_DUMP_ENABLE
  void                                                          dumpFromISR(void);
#endif // SYSTEM_QUEUE_DUMP_ENABLE


protected:
  // from task
  int                                                           clearData(void);

  // core


protected:
  bool                                                          is_Releasable_Data;
  SystemLightQueue<DataItem*>                                   data_Queue;  // queue to hold data pointers
  SystemCriticalSession                                         critical_Key;
};

#endif // _DATA_QUEUE_H

#endif // _CONF_DATA_QUEUE_ENABLED