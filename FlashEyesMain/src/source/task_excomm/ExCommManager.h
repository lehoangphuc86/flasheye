#include "ExCommConstant.h"
#ifndef _EX_COMM_MANAGER_H
#define _NET_MANAGER_H
#if (_CONF_EX_COMM_MANAGER_ENABLED)
/////////////////////////////////////////////////
// INCLUDE

#include "../task_comm_mbc/task_comm_mbc_http/CommMBCHttpClient.h"
#include "../task_comm_mbc/task_comm_mbc_http/CommMBCHttpServer.h"
#include "task_comm_mbc/task_comm_mbc_serial/CommMBCSerial.h"
#include "task_comm_mbc/task_comm_mbc_i2c/CommMBCI2CClient.h"
#include "task_comm_mbc/task_comm_mbc_i2c/CommMBCI2CServer.h"
#include "mess_broker/MessBrokerManager.h"
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

/*ExCommManager*/
class ExCommManager
{
private:
  ExCommManager(void);
  __ATTRIBUTE_VIRTUAL_OPTIMIZED ~ExCommManager(void);

public:
  static ExCommManager& getInstance(void);
  ExCommManager(ExCommManager const&) = delete;
  void operator=(ExCommManager const&) = delete;
  bool                                                          isValid(void);
  CommMBCHttpClient*                                            httpClient0(void);
  CommMBCHttpServer*                                            httpServer0(void);
  int                                                           startTask(ExCommManagerConfigTAG& exCommConfig);
  int                                                           startComm(bool waitCompletion);
  int                                                           sendComm(CommId_t commId, ExCommMBCParamTAG& mbcParams, TimePoint_t resWaitTimeMs);
  void                                                          stopComm(bool waitCompletion);
  void                                                          stopTask(void);
protected:
  //int                                                           notifyParent(EventId_t messageId, EventSize_t eventSize, unsigned char* eventData);

  int                                                           startTaskHttpClient0(void);
  int                                                           startCommHttpClient0(bool waitCompletion);
  void                                                          stopCommHttpClient0(bool waitCompletion);
  void                                                          stopTaskHttpClient0(void);

  int                                                           startTaskHttpServer0(void);
  int                                                           startCommHttpServer0(bool waitCompletion);
  void                                                          stopCommHttpServer0(bool waitCompletion);
  void                                                          stopTaskHttpServer0(void);

  
  int                                                           cbHttpUriHandlerServer(CommMBCHttpServer* mbcHttpServer, CommMBCHttpUriRequestTAG* uriReqInfo);

  int                                                           sendCommHttpClient(CommMBCHttpClient* httpClient, ExCommMBCParamTAG& mbcParams, TimePoint_t resWaitTimeMs);
  int                                                           cbHttpClientResponseClient(CommMBCHttpClient* mbcHttpClient, ExCommMBCParamTAG* mbcParams, CommMBCHttpUriRequestTAG* uriRequest);

  //int                                                           mbcMessageProceed(ExCommMBCParamTAG& mbcParams, TimePoint_t resWaitTimeMs);
protected:
  ExCommCb                                                      cb_Comm_Rev;
  void*                                                         cb_Comm_Rev_Arg;
  CommMBCHttpClient*                                            http_Client_0;
  CommMBCHttpServer*                                            http_Server_0;
  //CommMBCSerial*

private:
  static int                                                    cbHttpUriHandlerServer0(void* arg, CommMBCHttpUriRequestTAG* uriReqInfo);
  static int                                                    cbHttpClientResponseClient0(void* arg, CommMBCHttpUriRequestTAG* uriRequest);
};
#endif // _CONF_EX_COMM_MANAGER_ENABLED

#endif // _NET_MANAGER_H