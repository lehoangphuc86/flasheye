#ifndef _COMM_HTTP_SERVER_H
#define _COMM_HTTP_SERVER_H
/////////////////////////////////////////////////
// INCLUDE
#include "CommHttp.h"
#if (_CONF_COMM_HTTP_SERVER_ENABLED)
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

/*CommHttpServer*/
class CommHttpServer
  : public CommHttp
{
public:
  CommHttpServer(void);
  virtual ~CommHttpServer(void);
  int                                                           regUriHandler(CommHttpUriInfoTAG& uriInfo);
  int                                                           regDefaultUriHandler(void);

protected:
  const char*                                                   basePath(void);
  void                                                          regEventSize(void) override;
  void                                                          proc(void) override;
  int                                                           onEventCommHttpStart(unsigned char* data, unsigned int dataSize);
  int                                                           onEventCommHttpDataReceived(unsigned char* data, unsigned int dataSize);
  int                                                           onEventCommHttpStop(unsigned char* data, unsigned int dataSize);  
  DataSize_t                                                    readRequest(HttpReq_t* httpReq, char* buffer, DataSize_t bufferSize, DataSize_t maxLenToRead);
  int                                                           sendResponse(HttpReq_t* httpReq, CommHttpPackageTAG& package);
  int                                                           sendResponseBuff(HttpReq_t* httpReq, char* buffer, DataSize_t bufferSize);
  int                                                           sendResponseFileData(HttpReq_t* httpReq, char* buffer, DataSize_t bufferSize);
  virtual int                                                   cbCommHttpDefaultUriHandler(CommHttpUriRequestTAG* uriReqInfo);
  virtual int                                                   cbCommHttpRequestHandler(HttpReq_t* httpReq);

private:
  static HttpRet_t                                              cbCommHttpRequestHandlerNavigator(HttpReq_t* httpReq);
  static int                                                    cbCommHttpRequestDefaultUriHandlerNavigator(void* arg, CommHttpUriRequestTAG* uriReqInfo);
protected:
  SystemMutex                                                   data_Mutex;
  HttpServerHandler_t                                           http_Handler;
  char                                                          base_Path[COMM_HTTP_SERVER_BASE_PATH_MAX_LEN + 1];
  CommHttpUriListTAG                                            uri_List;
};
#endif // _CONF_COMM_HTTP_SERVER_ENABLED

#endif // _COMM_HTTP_SERVER_H