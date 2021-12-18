#ifndef _COMM_HTTP_CLIENT_H
#define _COMM_HTTP_CLIENT_H
/////////////////////////////////////////////////
// INCLUDE
#include "CommHttp.h"
#if (_CONF_COMM_HTTP_ENABLED)
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

/*CommHttpClient*/
class CommHttpClient
  : public CommHttp
{
public:
  CommHttpClient(void);
  virtual ~CommHttpClient(void);
  int                                                           request(CommHttpClientRequestParamsTAG& params, HttpReqId_t& reqId);
protected:
  //void                                                          cleanUp(void) override;
  void                                                          regEventSize(void) override;
  DataSize_t                                                    readResponse(CommHttpUriRequestTAG* httpReq, char* buffer, DataSize_t bufferLen);
  int                                                           sendBuff(CommHttpUriRequestTAG* httpReq, char* buffer, DataSize_t bufferLen);
  int                                                           sendFileData(CommHttpUriRequestTAG* httpReq, char* buffer, DataSize_t bufferLen);
  void                                                          proc(void) override;
  int                                                           onEventCommHttpStart(unsigned char* data, unsigned int dataSize);
  int                                                           onEventCommHttpConnectionOpened(unsigned char* data, unsigned int dataSize);
  int                                                           onEventCommHttpDataReceived(unsigned char* data, unsigned int dataSize);
  int                                                           onEventCommHttpClientRequest(unsigned char* data, unsigned int dataSize);
  int                                                           onEventCommHttpConnectionClosed(unsigned char* data, unsigned int dataSize);
  int                                                           onEventCommHttpStop(unsigned char* data, unsigned int dataSize);
  int                                                           onEventCommHttpFinished(unsigned char* data, unsigned int dataSize);
  int                                                           cbEventCommHttpHandler(void* eventData);
private:
  static HttpRet_t                                              cbHttpEventHandlerNavigator(HttpClientEvent_t* evt);
protected:
  HttpClientHandler_t                                           http_Handler;
  //CommHttpPackageTAG*                                           cur_Package;
};
#endif // _CONF_COMM_HTTP_ENABLED

#endif // _COMM_HTTP_CLIENT_H