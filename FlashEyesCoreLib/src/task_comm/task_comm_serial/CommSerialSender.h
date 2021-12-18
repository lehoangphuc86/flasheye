#include "CommSerialConstant.h"

#if (_CONF_COMM_SERIAL_SENDER_ENABLED)

#ifndef _COMM_SERIAL_SENDER_H
#define _COMM_SERIAL_SENDER_H

/////////////////////////////////////////////////
// INCLUDE
#include "CommSerialSystemSocket.h"
#include "CommSerialSetting.h"
#include "../../data_manager/BufferDataManager.h"

#ifdef COMM_SERIAL_USE_TASK
#include "../../task_manager/TaskManager.h"
#endif // COMM_SERIAL_USE_TASK

#if (!_CONF_COMM_SERIAL_SYSTEM_SOCKET_ENABLED)
#error Comm serial socket is required
#endif // _CONF_COMM_SERIAL_SYSTEM_SOCKET_ENABLED

#if (!_CONF_COMM_SERIAL_SETTING_ENABLED)
#error Comm serial setting is required
#endif // _CONF_COMM_SERIAL_SETTING_ENABLED

#if (!_CONF_DATA_QUEUE_ENABLED)
#error Data manager is required
#endif // _CONF_DATA_QUEUE_ENABLED

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

/*CommSerialSender*/
class CommSerialSender
#ifdef COMM_SERIAL_USE_TASK
  : TaskManager
#endif // COMM_SERIAL_USE_TASK
{
public:
  ////////////////////Function to call from outside ISR///////////////////////
  CommSerialSender(void);
  // WARNING: if inherite from this class, deconstructor must be virtual
  __ATTRIBUTE_VIRTUAL_OPTIMIZED ~CommSerialSender(void);
  bool                                                          isInitialized(void);
  bool                                                          isRunning(void);
  int                                                           setConfig(
                                                                  CommSerialConnectionSetting* commSettings
                                                                  , BufferDataManager* dataManager
                                                                  , TaskCodec* taskCodec
#ifdef COMM_SERIAL_USE_TASK
                                                                  , TaskManagerConfigTAG& taskConfig
#endif // COMM_SERIAL_USE_TASK
                                                                 );
#ifdef COMM_SERIAL_USE_TASK
  int                                                           start(TaskThreadConfigTAG& threadConfig);
  int                                                           start(CommSerialSystemSocket* systemSocket, TaskThreadConfigTAG& threadConfig);
#else // COMM_SERIAL_USE_TASK
  int                                                           start(void);
  int                                                           start(CommSerialSystemSocket* systemSocket);
#endif // COMM_SERIAL_USE_TASK
  void                                                          stop(void);
  int                                                           send(BufferDataItem* dataItem);
  void                                                          cleanUp(void);
protected:
  int                                                           prepare(void);
#ifdef COMM_SERIAL_USE_TASK
  void                                                          proc(void) override;
#endif // COMM_SERIAL_USE_TASK
  int                                                           onEventSendRequested(unsigned char* data, unsigned int dataSize);

  ////////////////////Shared funtcion///////////////////////
protected:
  int                                                           sendData(unsigned char* data, DataSize_t dataSize);
  void                                                          isRunning(bool state);
protected:
#ifndef COMM_SERIAL_USE_TASK
  bool                                                          is_Running;
  SystemCriticalSession                                         critical_Session;
#endif // COMM_SERIAL_USE_TASK
  CommSerialSystemSocket*                                       system_Socket;
  CommSerialConnectionSetting*                                  comm_Setting;
  BufferDataManager*                                            data_Manager;
  TaskCodec*                                                    task_Codec;
  
};

#endif // _COMM_SERIAL_SENDER_H

#endif // (_CONF_COMM_SERIAL_SENDER_ENABLED)