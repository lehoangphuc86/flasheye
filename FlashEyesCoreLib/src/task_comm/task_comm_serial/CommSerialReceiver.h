#include "CommSerialConstant.h"

#if (_CONF_COMM_SERIAL_RECEIVER_ENABLED)

#ifndef _COMM_SERIAL_RECEIVER_H
#define _COMM_SERIAL_RECEIVER_H

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

/*CommSerialReceiver*/
class CommSerialReceiver
#ifdef COMM_SERIAL_USE_TASK
  : TaskManager
#endif // COMM_SERIAL_USE_TASK
{
public:
  ////////////////////Function to call from outside ISR///////////////////////
  CommSerialReceiver(void);
  // WARNING: if inherite from this class, deconstructor must be virtual
  __ATTRIBUTE_VIRTUAL_OPTIMIZED ~CommSerialReceiver(void);
  bool                                                          isInitialized(void);
  bool                                                          isRunning(void);
  int                                                           setConfig(
                                                                  void* commParent
                                                                  , CommSerialConnectionSetting* commSettings
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
  BufferDataItem*                                               read(bool& isDisconnected, byte tryTime);
  void                                                          cleanUp(void);
protected:
  int                                                           prepare(void);
#ifdef COMM_SERIAL_USE_TASK
  void                                                          proc(void) override;
#endif // COMM_SERIAL_USE_TASK
  
protected:
#ifdef COMM_PROC_TIME_MEA_ENABLE
  int                                                           readCore(BufferDataItem* dataItem, bool& isDisconnected, CommProcMeaTimeTAG& procTime, byte tryTime = 0xFF);
#else // COMM_PROC_TIME_MEA_ENABLE
  int                                                           readCore(BufferDataItem* dataItem, bool& isDisconnected, byte tryTime = 0xFF);
#endif // COMM_PROC_TIME_MEA_ENABLE
  DataSize_t                                                    readData(unsigned char* buffer, DataSize_t bufferSize, DataSize_t maxLenToRead, bool& isDisconnected, byte tryTime);
  void                                                          isRunning(bool state);
  /*void                                                          initializeSaveBuffer(void);
  void                                                          clearSaveBuffer(void);*/
protected:
#ifndef COMM_SERIAL_USE_TASK
  bool                                                          is_Running;
  SystemCriticalSession                                         critical_Session;
#endif // COMM_SERIAL_USE_TASK
  void*                                                         comm_Parent;
  CommSerialSystemSocket*                                       system_Socket;
  CommSerialConnectionSetting*                                  comm_Setting;
  BufferDataManager*                                            data_Manager;
//  CodecBufferInfo*                                              codec_Buffer_Info;
  TaskCodec*                                                    task_Codec;
};

#endif // _COMM_SERIAL_RECEIVER_H

#endif // (_CONF_COMM_SERIAL_RECEIVER_ENABLED)