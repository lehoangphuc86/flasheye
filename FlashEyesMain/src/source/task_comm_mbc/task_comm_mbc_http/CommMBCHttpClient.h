#include "CommMBCHttpConstant.h"
#ifndef _COMM_MBC_HTTP_CLIENT_H
#define _COMM_MBC_HTTP_CLIENT_H
#if (_CONF_COMM_MBC_HTTP_ENABLED)
/////////////////////////////////////////////////
// INCLUDE
#include "task_comm_mbc/CommMBCEngine.h"
#include "../../task_http/CommHttpClient.h"

#if (!_CONF_COMM_MBC_ENGINE_ENABLED)
#error Comm MBC engine is required
#endif // _CONF_COMM_MBC_ENGINE_ENABLED

#if (!_CONF_COMM_HTTP_CLIENT_ENABLED)
#error Comm Http client is required
#endif // _CONF_COMM_HTTP_CLIENT_ENABLED
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

/*CommMBCHttpClient*/
class CommMBCHttpClient
  : public CommHttpClient
{
public:
  CommMBCHttpClient(void);
  virtual ~CommMBCHttpClient(void);
  uint16_t                                                      registerMessageId(uint8_t messageId, uint16_t maxRawSize = 0);
  int                                                           request(CommMBCHttpClientRequestParamsTAG& mbcParams, HttpReqId_t& reqId);
  int                                                           inititialize(CommMBCSettingParamTAG mbcSetting);
  int                                                           startTask(CommMBCHttpTaskConfigTAG& commMbcHttpConfig) override;
  int                                                           startHttp(CommMBCHttpConnectionConfigTAG& commMbcHttpConnConfig, bool waitCompletion = true);
  void                                                          cleanUp(void);
protected:
  void                                                          regEventSize(void) override;
  int                                                           inititialize(void) override; //hide from public
  int                                                           request(CommHttpClientRequestParamsTAG& params, HttpReqId_t& reqId) override; //hide from public
  //int                                                           startTask(CommHttpTaskConfigTAG& commHttpConfig) override; //hide from public
  int                                                           startHttp(CommHttpConnectionConfigTAG& commHttpConnConfig, bool waitCompletion = true) override; //hide from public
  int                                                           onEventCommHttpStart(unsigned char* data, unsigned int dataSize) override;
  int                                                           onEventCommHttpStop(unsigned char* data, unsigned int dataSize) override;

private:
  static int                                                    cbHttpClientResponse(void* arg, CommHttpUriRequestTAG* uriRequest);
protected:
  CommMBCEngine                                                 comm_Mbc_Engine;
};
#endif // _CONF_COMM_MBC_HTTP_ENABLED

#endif // _COMM_MBC_HTTP_CLIENT_H