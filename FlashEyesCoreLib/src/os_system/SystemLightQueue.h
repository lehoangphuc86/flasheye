#include "SystemCommon.h"

#if (_CONF_SYSTEM_LIGHT_QUEUE_ENABLED)
#ifndef _SYSTEM_LIGHT_QUEUE_H
#define _SYSTEM_LIGHT_QUEUE_H

/////////////////////////////////////////////////
// INCLUDE

#if (!_CONF_SYSTEM_CRITICAL_SESSION_ENABLED)
#error Critical session is required
#endif // 

#include "SystemCriticalSession.h"
/////////////////////////////////////////////////
// PREPROCESSOR

/////////////////////////////////////////////////
// DEFINE
//#define SYSTEM_QUEUE_DUMP_ENABLE
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

/*SystemLightQueue*/
/*
* Wrap RTOS queue class
* 
*/
template <typename DataItemT>
struct LinkedNode
{
  DataItemT data;
  LinkedNode< DataItemT>* next;
};


template <typename DataItemT>
class SystemLightQueue
{
public:
  SystemLightQueue(void);
  // WARNING: if inherite from this class, deconstructor must be virtual
  __ATTRIBUTE_VIRTUAL_OPTIMIZED ~SystemLightQueue(void);
  int                                                           initialize(DataCount_t preparedQueueSize); // used for Arduino only. Remember to call this before using
  
  ////////////////////Function to call from outside ISR///////////////////////
  bool                                                          isValid(void);
  bool                                                          empty(void);
  DataCount_t                                                   size(void);
  DataItemT*                                                    front(void);
  int                                                           push(const DataItemT& val);
  int                                                           pop(void);
  DataItemT*                                                    get(void); // front then pop
  int                                                           remove(const DataItemT& val);
  int                                                           reset(void);
#ifdef SYSTEM_QUEUE_DUMP_ENABLE
  void                                                          dump(void);
#endif // SYSTEM_QUEUE_DUMP_ENABLE


  ////////////////////Function to call from inside ISR///////////////////////
  bool                                                          isValidFromISR(void);
  bool                                                          emptyFromISR(void);
  DataCount_t                                                   sizeFromISR(void);
  DataItemT*                                                    frontFromISR(void);
  int                                                           pushFromISR(const DataItemT& val);
  int                                                           popFromISR(void);
  DataItemT*                                                    getFromISR(void); // front then pop
  int                                                           removeFromISR(const DataItemT& val);
  int                                                           resetFromISR(void);
#ifdef SYSTEM_QUEUE_DUMP_ENABLE
  void                                                          dumpFromISR(void);
#endif // SYSTEM_QUEUE_DUMP_ENABLE
  
protected:
  ////////////////////Core functions without locking///////////////////////
  bool                                                          isFullCore(void);
  LinkedNode<DataItemT>*                                        getNodeCore(DataCount_t index);
  LinkedNode<DataItemT>*                                        getAvailableNodeBufferCore(void);
  int                                                           createDataCore(DataCount_t queueSize);
  int                                                           clearDataCore(void);
  bool                                                          isValidCore(void);
  bool                                                          emptyCore(void) const;
  DataCount_t                                                   sizeCore(void);
  DataItemT*                                                    frontCore(void);
  int                                                           pushCore(const DataItemT& val);
  int                                                           popCore(void);
  DataItemT*                                                    getCore(void); // front then pop
  int                                                           removeCore(const DataItemT& val);
  int                                                           resetCore(void);
#ifdef SYSTEM_QUEUE_DUMP_ENABLE
  void                                                          dumpCore(void);
#endif // SYSTEM_QUEUE_DUMP_ENABLE

protected:
  //const unsigned int                                            data_Size; // size of holding data
  //const unsigned int                                            node_Size; // size of node holding data
  DataCount_t                                                   prepared_Queue_Size; // size to allocate node buffer
  DataCount_t                                                   current_Queue_Size; // being used node number
  LinkedNode<DataItemT>*                                        node_Buffer; // holds the allocated node buffer
  LinkedNode<DataItemT>*                                        queue_Start; // points to the first used node in the queue
  LinkedNode<DataItemT>*                                        queue_End; // points to the last used node in the queue
  //SystemCriticalSession                                         critical_Key;
};
#endif // _SYSTEM_LIGHT_QUEUE_H


#endif // _CONF_SYSTEM_LIGHT_QUEUE_ENABLED