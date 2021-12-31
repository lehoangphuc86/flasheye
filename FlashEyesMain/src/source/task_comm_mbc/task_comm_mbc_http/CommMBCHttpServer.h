#include "CommMBCHttpConstant.h"
#ifndef _COMM_MBC_HTTP_SERVER_H
#define _COMM_MBC_HTTP_SERVER_H
#if (_CONF_COMM_MBC_HTTP_SERVER_ENABLED)
/////////////////////////////////////////////////
// INCLUDE
#include "task_comm_mbc/CommMBCEngine.h"
#include "../../task_http/CommHttpServer.h"

#if (!_CONF_COMM_MBC_ENGINE_ENABLED)
#error Comm MBC engine is required
#endif // _CONF_COMM_MBC_ENGINE_ENABLED

#if (!_CONF_COMM_HTTP_SERVER_ENABLED)
#error Comm Http server is required
#endif // _CONF_COMM_HTTP_SERVER_ENABLED
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

/*CommMBCHttpServer*/
class CommMBCHttpServer
  : public CommHttpServer
{
public:
  CommMBCHttpServer(void);
  virtual ~CommMBCHttpServer(void);
  uint16_t                                                      registerMessageId(uint8_t messageId, uint16_t maxRawSize = 0);
  int                                                           regDefaultUriHandler(void) override;
  int                                                           regUriHandler(CommMBCHttpUriInfoTAG& uriInfo) override;
  int                                                           inititialize(CommMBCSettingParamTAG mbcSetting);
  int                                                           startTask(CommMBCHttpTaskConfigTAG& commMbcHttpConfig) override;
  int                                                           startHttp(CommMBCHttpConnectionConfigTAG& commMbcHttpConnConfig, bool waitCompletion = true);
  int                                                           setResponse(MbcMessageId_t messageId, unsigned char* inputBuffer, MbcDataSize_t inputSize, unsigned char* outBuffer, MbcDataSize_t outSize, MbcDataSize_t& encodedSize);
  void                                                          cleanUp(void);

protected:
  void                                                          regEventSize(void) override;
  int                                                           inititialize(void) override; //hide from public
  //int                                                           startTask(CommHttpTaskConfigTAG& commHttpConfig) override; //hide from public
  int                                                           startHttp(CommHttpConnectionConfigTAG& commHttpConnConfig, bool waitCompletion = true) override; //hide from public
  //int                                                           regUriHandler(CommHttpUriInfoTAG& uriInfo) override; //hide from public
  int                                                           onEventCommHttpStart(unsigned char* data, unsigned int dataSize) override;
  int                                                           onEventCommHttpStop(unsigned char* data, unsigned int dataSize) override;

private:
  static int                                                    cbCommHttpUriHandler(void* arg, CommHttpUriRequestTAG* httpUriReqInfo);
protected:
  CommMBCEngine                                                 comm_Mbc_Engine;
};
#endif // _CONF_COMM_MBC_HTTP_SERVER_ENABLED

#endif // _COMM_MBC_HTTP_SERVER_H