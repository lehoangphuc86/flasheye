#ifndef _COMM_MBC_HTTP_CONSTANT_H
#define _COMM_MBC_HTTP_CONSTANT_H

#include "../../../FlashEyesMain.h"
#if (_CONF_COMM_MBC_HTTP_CONSTANT_ENABLED)
/////////////////////////////////////////////////
// INCLUDE
#include "task_comm_mbc/CommMBCConstant.h"
#include "../../task_http/CommHttpConstant.h"
/////////////////////////////////////////////////
// PREPROCESSOR

/////////////////////////////////////////////////
// DEFINE
#define COMM_MBC_HTTP_REQUEST_HANDLE_TIME             500 // ms
/////////////////////////////////////////////////
// MARCO

/////////////////////////////////////////////////
// DATA TYPE (TYPEDEF)
// @param1: request information
struct _commMBCHttpUriRequestTAG;
typedef int(*CommMBCHttpCbOnUriRequest)(void* arg, _commMBCHttpUriRequestTAG* uriRequest);

//
typedef int(*CommMBCHttpCbOnUriResponse)(void* arg, _commMBCHttpUriRequestTAG* uriRequest);
/////////////////////////////////////////////////
// DATA TYPE (ENUM)

/////////////////////////////////////////////////
// DATA TYPE (UNION)

/////////////////////////////////////////////////
// DATA TYPE (STRUCT)
typedef _commHttpTaskConfigTAG CommMBCHttpTaskConfigTAG;
typedef _commHttpClientConfigTAG CommMBCHttpClientConfigTAG;
typedef _commHttpServerConfigTAG CommMBCHttpServerConfigTAG;

typedef union _commMBCHttpConnectionConfigU
{
  CommMBCHttpClientConfigTAG client;
  CommMBCHttpServerConfigTAG server;
} CommMBCHttpConnectionConfigU;

typedef struct _commMBCHttpConnectionConfigTAG
{
  CommMBCHttpConnectionConfigU config;
} CommMBCHttpConnectionConfigTAG;

typedef _commHttpUriInfoTAG CommMBCHttpUriInfoTAG;
typedef _commHttpHeaderTAG CommMBCHttpHeaderTAG;
typedef _commHttpPackageTAG CommMBCHttpPackageTAG;

typedef struct _commMBCHttpNotifierTag
{
  TimePoint_t expiredTime; // caller will wait for response until this time. Dont proceed if it is already expired.
  CommMBCHttpCbOnUriResponse callback;
  void* agr;
  _commMBCHttpNotifierTag(void)
  {
    this->expiredTime = 0;
    this->callback = NULL;
    this->agr = NULL;
  }
  bool isEnabled(void)
  {
    return (this->callback == NULL ? false : true);
  }
} CommMBCHttpNotifierTAG;

// internal use
typedef struct _commMBCHttpClientArgTag
{
  void* mbcHandler;
  CommMBCHttpNotifierTAG mbcNotifier;
  MbcMessageId_t reqMessageId;
} CommMBCHttpClientArgTAG;

typedef struct _commMBCHttpServerArgTag
{
  void* mbcHandler;
  void* mbcArg;
  CommMBCHttpCbOnUriResponse reqCallback;
} CommMBCHttpServerArgTAG;

typedef struct _commMBCHttpUriRequestTAG
{
  CommMBCHttpPackageTAG reqData;
  MbcMessageId_t reqMessageId;
  CommMBCHttpNotifierTAG resNoti;
  CommMBCHttpPackageTAG resData;
  MbcMessageId_t resMessageId;
  uint16_t requestId;
} CommMBCHttpUriRequestTAG;

typedef struct _commMBCHttpClientRequestParamsTAG
{
  MbcMessageId_t reqMessageId;
  CommMBCHttpHeaderTAG header;
  CommMBCHttpNotifierTAG notifier;
  char* body; // file path or buffer
  DataSize_t bodyLen; // bufferLen or filePathLen
} CommMBCHttpClientRequestParamsTAG;

// events
typedef struct _eventCommMBCHttpStartTAG
{
  CommMBCHttpConnectionConfigU config;
} EventCommMBCHttpStartTAG;

typedef _eventCommHttpConnectionOpenedTAG EventCommMBCHttpConnectionOpenedTAG;
typedef _eventCommHttpDataReceivedTAG EventCommMBCHttpDataReceivedTAG;
typedef _eventCommHttpConnectionClosedTAG EventCommMBCHttpConnectionClosedTAG;
typedef _eventCommHttpStopTAG EventCommMBCHttpStopTAG;
typedef _eventCommHttpFinishedTAG EventCommMBCHttpFinishedTAG;

typedef struct _eventCommMBCHttpClientRequestTAG
{
  CommMBCHttpUriRequestTAG param;
} EventCommMBCHttpClientRequestTAG;

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
// CLASS DEFINITION
/**/


#endif // _CONF_COMM_MBC_HTTP_CONSTANT_ENABLED

#endif // _COMM_MBC_HTTP_CONSTANT_H