#include "CommSerialConstant.h"

#if (_CONF_COMM_SERIAL_ENABLED)

#ifndef _COMM_SERIAL_H
#define _COMM_SERIAL_H

/////////////////////////////////////////////////
// INCLUDE
#include "CommSerialSystemSocket.h"
#include "CommSerialSetting.h"
#include "../../data_manager/BufferDataManager.h"
#include "CommSerialSender.h"
#include "CommSerialReceiver.h"

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

#if (!_CONF_COMM_SERIAL_SENDER_ENABLED)
#error Comm serial sender is required
#endif // _CONF_COMM_SERIAL_SENDER_ENABLED


#if (!_CONF_COMM_SERIAL_SENDER_ENABLED)
#error Comm serial receiver is required
#endif // _CONF_COMM_SERIAL_SENDER_ENABLED

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

/*CommSerial*/
class CommSerial
#ifdef COMM_SERIAL_USE_TASK
  : public TaskManager
#endif // COMM_SERIAL_USE_TASK
{
public:
  ////////////////////Function to call from outside ISR///////////////////////
  CommSerial(void);
  virtual ~CommSerial(void);
  bool                                                          isInitialized(void);
  bool                                                          isRunning(void);
  bool                                                          isConnected(void);
  bool                                                          isDisconnected(void);
  bool                                                          isValidSocket(void);
  byte                                                          connectionState(void);
  virtual int                                                   setConfig(CommSerialConfigTAG& commConfig);
  virtual int                                                   start(void);
  //virtual int                                                   connect(void);
  virtual int                                                   connect(CommSerialSocketConfigTAG& socketConfig);
  int                                                           disconnect(unsigned int timeWait = COMM_SERIAL_DISCONNECT_MAX_WAIT);
  virtual void                                                  stop(void);
  BufferDataItem*                                               getCommData(void);
  void                                                          releaseCommData(BufferDataItem* dataItem);
  virtual int                                                   send(unsigned char* buffer, DataSize_t bufferSize);
  virtual int                                                   read(uint8_t inTryTime, BufferDataItem*& dataItem, bool& isDisconnected);
  virtual void                                                  cleanUp(void);
protected:
  void                                                          isRunning(bool state);
  void                                                          connectionState(byte state);
  void                                                          closeConnection(void);
  virtual int                                                   prepare(void);
  //int                                                           sendBuffer(unsigned char* buffer, size_t bufferSize);
  int                                                           sendDataItem(BufferDataItem* dataItem);
#ifdef COMM_SERIAL_USE_TASK
  void                                                          proc(void) override;
  virtual int                                                   onEventDateReceived(unsigned char* data, unsigned int dataSize);
  int                                                           onEventConnectionClosed(unsigned char* data, unsigned int dataSize);
#endif // COMM_SERIAL_USE_TASK
  int                                                           onEventSendRequested(unsigned char* data, unsigned int dataSize);
  int                                                           onEventCloseConnection(unsigned char* data, unsigned int dataSize);
  int                                                           onEventOpenConnection(unsigned char* data, unsigned int dataSize);

protected:
  CommSerialSender                                              comm_Sender;
  CommSerialReceiver                                            comm_Receiver;
  CommSerialSystemSocket                                        system_Socket;
  CommSerialConnectionSetting                                   comm_Setting;
  BufferDataManager                                             data_Manager;
#ifdef COMM_SERIAL_USE_TASK
  SystemMutex                                                   connect_Key; // wait for connecting
  SystemMutex                                                   disconnect_Key; // wait for connecting
#else // COMM_SERIAL_USE_TASK
  bool                                                          is_Running;
  SystemCriticalSession                                         critical_Key;
#endif // COMM_SERIAL_USE_TASK
  byte                                                          connection_State;
  TaskCodec*                                                    task_Codec;
};

#endif // _COMM_SERIAL_H

#endif // (_CONF_COMM_SERIAL_ENABLED)