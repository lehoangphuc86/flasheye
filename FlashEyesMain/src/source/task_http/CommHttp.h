#ifndef _COMM_HTTP_H
#define _COMM_HTTP_H
/////////////////////////////////////////////////
// INCLUDE
#include "CommHttpConstant.h"
#if (_CONF_COMM_HTTP_ENABLED)

#include "task_manager/TaskManager.h"
#include "data_manager/BufferDataManager.h"
#include "os_system/SystemCriticalSession.h"
/////////////////////////////////////////////////
// PREPROCESSOR
/////////////////////////////////////////////////
// DEFINE

/////////////////////////////////////////////////
// MARCO

/////////////////////////////////////////////////
// DATA TYPE (TYPEDEF)

/////////////////////////////////////////////////
// DATA TYPE (ENUM)

/////////////////////////////////////////////////
// DATA TYPE (STRUCT)

/////////////////////////////////////////////////
// GLOBAL VARIABLES

/////////////////////////////////////////////////
// GLOBAL FUNCTIONS

/////////////////////////////////////////////////
// STATIC DATA

/////////////////////////////////////////////////
// STATIC FUNCTIONS

/////////////////////////////////////////////////
// EXTERN

/////////////////////////////////////////////////
// CLASS IMPLEMENTAION

/*CommHttp*/
class CommHttp
  : public TaskManager
{
public:
  CommHttp(bool isServer);
  virtual ~CommHttp(void);
  virtual int                                                   inititialize(void);
  bool                                                          isStarted(void);
  bool                                                          isServer(void);
  virtual int                                                   startTask(CommHttpTaskConfigTAG& commHttpConfig);
  virtual int                                                   startHttp(CommHttpConnectionConfigTAG& commHttpConnConfig, bool waitCompletion = true);
  BufferDataItem*                                               getCommData(void);
  BufferDataManager*                                            dataManager(void);
  void                                                          releaseCommData(BufferDataItem* dataItem);
  virtual void                                                  stopHttp(bool waitCompletion = true);
  virtual void                                                  stopTask(void);
  virtual void                                                  cleanUp(void);
protected:
  HttpReqId_t                                                   nextReqId(void);
  virtual void                                                  regEventSize(void) = 0;
  virtual int                                                   prepare(void);
  virtual int                                                   onEventCommHttpStart(unsigned char* data, unsigned int dataSize) = 0;
  virtual int                                                   onEventCommHttpStop(unsigned char* data, unsigned int dataSize) = 0;
protected:
  bool                                                          is_Server : 1;
  bool                                                          is_Started : 1;
  byte                                                          reserved_Bit : 6;
  HttpReqId_t                                                   current_Req_Id;
  SystemMutex                                                   mutex_Operating;
  SystemMutex                                                   mutex_wait_Start;
  SystemMutex                                                   mutex_wait_Stop;
  int                                                           ret_Start;
  int                                                           ret_Stop;
  BufferDataManager                                             data_Manager;
  CommHttpConnectionConfigTAG                                   conn_Config;
};
#endif // _CONF_COMM_HTTP_ENABLED

#endif // _COMM_HTTP_H