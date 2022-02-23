#include "ExportManagerConstant.h"

#if (_CONF_EXPORT_HTTP_CLIENT_PROCESSOR_ENABLED)

#ifndef _EXPORT_HTTP_CLIENT_PROCESSOR_H
#define _EXPORT_HTTP_CLIENT_PROCESSOR_H

/////////////////////////////////////////////////
// INCLUDE
#include "ExportProcessor.h"
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

/*ExportHttpClientProcessor*/
class ExportHttpClientProcessor
  : public ExportProcessor
{
public:
  ExportHttpClientProcessor(void);
  virtual ~ExportHttpClientProcessor(void);
  bool                                                          isValid(void) override;
  int                                                           start(ExportProcessorConfigTAG& procConfig) override;
  void                                                          stop(void) override;
  int                                                           exportScanResult(ExportScanResultTAG* param) override;
protected:
  CommMBCHttpClient*                                            http_Client;
  ExportHttpClientProcessorReqParamTAG                          req_Param;
};

#endif // _EXPORT_HTTP_CLIENT_PROCESSOR_H

#endif // _CONF_EXPORT_HTTP_CLIENT_PROCESSOR_ENABLED