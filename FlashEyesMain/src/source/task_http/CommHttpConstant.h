#ifndef _COMM_HTTP_CONSTANT_H
#define _COMM_HTTP_CONSTANT_H

#include "../../FlashEyesMain.h"
#if (_CONF_COMM_HTTP_CONSTANT_ENABLED)
/////////////////////////////////////////////////
// INCLUDE
#include "../../drivers/esp32/http/http.h"
#include "../task_net/NetManagerConstant.h"
#include "task_manager/TaskManagerConstant.h"
#include "data_manager/DataItem.h"
#include "data_manager/BufferDataManager.h"
#include "utility/Utility.h"
#include "os_system/SystemMutex.h"
/////////////////////////////////////////////////
// PREPROCESSOR

/////////////////////////////////////////////////
// DEFINE
#define COMM_HTTP_START_WAIT_TIME                   10000 // 10 sec
#define COMM_HTTP_STOP_WAIT_TIME                    COMM_HTTP_START_WAIT_TIME
#define COMM_HTTP_CLIENT_AUTO_DIRECT_DEFAULT        false
#define COMM_HTTP_URL_MAX_LEN                       128
#define COMM_HTTP_URL_PATH_MAX_LEN                  128
#define COMM_HTTP_SERVER_BASE_PATH_MAX_LEN          64
#define COMM_HTTP_REQUEST_HANDLE_WAIT_TIME          1000 // ms
#define COMM_HTTP_REQUEST_HANDLE_WAIT_TIME_ADD      100 // ms
#define COMM_HTTP_RESPOMSE_NOTIFY_MODE_CB           0
#define COMM_HTTP_RESPOMSE_NOTIFY_MODE_MUTEX        1


#define COMM_HTTP_PACKAGE_BUFF_CHUNK_SIZE           1024 // byte
#define COMM_HTTP_PACKAGE_FILE_CHUNK_SIZE           200 // byte

#define COMM_HTTP_HEADER_CONTENT_NAME               "Content-Type"
#define COMM_HTTP_HEADER_TRANS_ENCODE_NAME          "Transfer-Encoding"
#define COMM_HTTP_HEADER_TRANS_ENCODE_CHUNKED       "chunked"
#define COMM_HTTP_TERMINATOR                        "\r\n"
#define COMM_HTTP_TERMINATOR_LEN                    2
#define COMM_HTTP_DATA_SIZE_LEN                     8

//#define COMM_HTTP_HEADER_CONTENT_JSON               "application/json"

#define COMM_HTTP_DEFAULT_URI                       "/*"
#define COMM_HTTP_DEFAULT_URI_ID                    0 // equal to http://192.168.1.5/* *

#define COMM_HTTP_METHOD_GET                        0
#define COMM_HTTP_METHOD_PUT                        1
#define COMM_HTTP_METHOD_POST                       2
#define COMM_HTTP_METHOD_MAX                        3

#define COMM_HTTP_DATA_TYPE_TEXT_PLAIN              0
#define COMM_HTTP_DATA_TYPE_TEXT_HTML               1
#define COMM_HTTP_DATA_TYPE_APP_JS                  2
#define COMM_HTTP_DATA_TYPE_TEXT_CSS                3
#define COMM_HTTP_DATA_TYPE_IMG_PNG                 4
#define COMM_HTTP_DATA_TYPE_IMG_ICON                5
#define COMM_HTTP_DATA_TYPE_TEXT_XML                6
#define COMM_HTTP_DATA_TYPE_APP_JSON                7
#define COMM_HTTP_DATA_TYPE_MAX                     8

#define COMM_HTTP_STATUS_CODE_200                   0
#define COMM_HTTP_STATUS_CODE_201                   1
#define COMM_HTTP_STATUS_CODE_202                   2
#define COMM_HTTP_STATUS_CODE_203                   3
#define COMM_HTTP_STATUS_CODE_204                   4
#define COMM_HTTP_STATUS_CODE_401                   5
#define COMM_HTTP_STATUS_CODE_404                   6
#define COMM_HTTP_STATUS_CODE_500                   7
#define COMM_HTTP_STATUS_CODE_MAX                   8

/////////////////////////////////////////////////
// MARCO

/////////////////////////////////////////////////
// DATA TYPE (TYPEDEF)
// @param1: request information
struct _commHttpUriRequestTAG;
typedef int(*CommHttpCbOnUriRequest)(void* arg, _commHttpUriRequestTAG* uriRequest);

//
typedef int(*CommHttpCbOnUriResponse)(void* arg, _commHttpUriRequestTAG* uriRequest);
/////////////////////////////////////////////////
// DATA TYPE (ENUM)

/////////////////////////////////////////////////
// DATA TYPE (UNION)

/////////////////////////////////////////////////
// DATA TYPE (STRUCT)
typedef struct _commHtttpTaskConfigTAG
{
  //size_t maxPackSize; // maximum package size
  //size_t maxTruckSize; // max size to read traffic each time
  BufferDataManagerConfigTAG bufferConfig;
  TaskManagerConfigTAG taskManagerConfig;
  TaskThreadConfigTAG taskThreadConfig;
} CommHtttpTaskConfigTAG;

typedef struct _commHttpClientConfigTAG
{
  const char* url;
  const char* host;
  uint16_t port;
  const char* certPem; //CA cert
} CommHttpClientConfigTAG;

typedef struct _commHttpServerConfigTAG
{
  const char* basePath;
  uint16_t port;
  byte uriMaxCount;
  byte connMaxCount;
  byte addHeaderMaxCount;
  byte backlogConn;
  //CommHttpUriInfoTAG* uris;
} CommHttpServerConfigTAG;

typedef union _commHttpConnectionConfigU
{
  CommHttpClientConfigTAG client;
  CommHttpServerConfigTAG server;
} CommHttpConnectionConfigU;

typedef struct _commHttpConnectionConfigTAG
{
  //CommHttpConnectionConfigU httpConfig;
  CommHttpConnectionConfigU config;
} CommHttpConnectionConfigTAG;


//typedef struct _commHttpUriRequestTAG
//{
//  uint16_t requestId; // caller generates
//  const char* reqUri;
//  CommHttpUriInfoTAG* uriInfo;
//  CommHttpPackageTAG reqData;
//  TimePoint_t expiredTime; // caller will wait for response until this time. Dont proceed if it is already expired.
//  SystemMutex* waitMutex;
//  CommHttpPackageTAG resData;
//} CommHttpUriRequestTAG;

typedef struct _commHttpUriInfoTAG
{
  const char* uri; // upper side allocate memory
  //char* basePath; // upper side allocate memory
  byte id;
  byte method;
  byte dataType;
  void* serverHandler; // for internal use
  CommHttpCbOnUriRequest reqCallback;
  //CommHttpCbOnUriRespond resCallback;
  void* userArg;

  _commHttpUriInfoTAG(void)
  {
    this->uri = NULL;
    //this->basePath = NULL;
    this->serverHandler = NULL;
    this->id = COMM_HTTP_DEFAULT_URI_ID;
    this->method = COMM_HTTP_METHOD_GET;
    this->dataType = COMM_HTTP_DATA_TYPE_TEXT_HTML;
    this->reqCallback = NULL;
    //this->resCallback = NULL;
    this->userArg = NULL;
  }

  bool isValid(void)
  {
    do
    {
      if ((this->uri == NULL)
        //|| (this->basePath == NULL)
        || (this->method >= COMM_HTTP_METHOD_MAX)
        || (this->dataType >= COMM_HTTP_DATA_TYPE_MAX)
        || (this->reqCallback == NULL)
        )
      {
        break;
      }

      return true;
    } while (0);
    return false;
  }
} CommHttpUriInfoTAG;

typedef struct _commHttpUriListTAG
{
  byte maxCount;
  byte count;
  CommHttpUriInfoTAG* uris;
  SystemMutex operatingMutex;
  _commHttpUriListTAG(void)
  {
    this->count = 0;
    this->maxCount = 0;
    this->uris = NULL;
  }

  ~_commHttpUriListTAG(void)
  {
    this->reset();
  }

  void reset(void)
  {
    SystemMutexLocker locker(this->operatingMutex);
    this->count = 0;
    this->maxCount = 0;
    if (this->uris != NULL)
    {
      delete[] this->uris;
    }
    this->uris = NULL;
  }

  int initialize(byte maxCount)
  {
    do
    {
      if (maxCount <= 0)
      {
        break;
      }
      this->reset();
      SystemMutexLocker locker(this->operatingMutex);
      this->maxCount = maxCount;
      this->uris = new CommHttpUriInfoTAG[maxCount + 1];
      return 0;
    } while (0);
    return -1;
  }

  CommHttpUriInfoTAG* append(void)
  {
    do
    {
      SystemMutexLocker locker(this->operatingMutex);
      if (this->count >= this->maxCount)
      {
        break;
      }

      this->count++;
      return &this->uris[this->count-1];
    } while (0);
    return NULL;
  }

} CommHttpUriListTAG;


typedef struct _commHttpHeaderTAG
{
  byte resStatusCode; // res
  byte reqMethod; // req
  byte dataType;
  const char* uri;
  byte uriId;
  byte fileData : 1;
  byte reserved : 6;
} CommHttpHeaderTAG;

typedef struct _commHttpPackageTAG
{
  _commHttpPackageTAG(void)
  {
    this->dataItem = NULL;
    this->dataManager = NULL;
  }

  ~_commHttpPackageTAG(void)
  {
    //this->clear();
  }

  bool isValid(void)
  {
    do
    {
      if ((this->dataItem == NULL)
        || (this->dataManager == NULL))
      {
        break;
      }

      return true;
    } while (0);
    return false;
  }

  void clear(void)
  {
    if (this->dataItem != NULL)
    {
      this->dataManager->release(this->dataItem);
    }
    this->dataItem = NULL;
    this->dataManager = NULL;
  }

  int setData(BufferDataManager* dataMgr)
  {
    do
    {
      this->clear();
      if (dataMgr == NULL)
      {
        break;
      }

      this->dataManager = dataMgr;
      this->dataItem = this->dataManager->get();
      if ((this->dataItem == NULL)
        || (this->dataItem->bufferLength() < sizeof(CommHttpHeaderTAG))
        )
      {
        break;
      }
      return this->dataItem->setDataLen(sizeof(CommHttpHeaderTAG));
    } while (0);
    this->clear();
    return -1;
  }

  CommHttpHeaderTAG* header(void)
  {
    return (CommHttpHeaderTAG*)this->dataItem->bufferAddress();
  }

  char* body(void)
  {
    return (char*)(this->dataItem->bufferAddress() + sizeof(CommHttpHeaderTAG));
  }

  DataSize_t bodyLen(void)
  {
    return this->dataItem->dataLength() - sizeof(CommHttpHeaderTAG);
  }

  int bodyLen(DataSize_t bodyLen)
  {
    return this->dataItem->setDataLen(bodyLen + sizeof(CommHttpHeaderTAG));
  }

  DataSize_t bodyMaxLen(void)
  {
    return this->dataItem->bufferLength() - sizeof(CommHttpHeaderTAG);
  }

  void releaseData(void)
  {
    this->clear();
  }

  protected:
    BufferDataItem* dataItem;
    BufferDataManager* dataManager;
} CommHttpPackageTAG;

//typedef struct _commHttpNotifierTag
//{
//  bool enabled : 1; // 0: ignore this setting
//  bool notifyMode : 1; // 0: cb, 1: mutex
//  byte reserved : 7;
//  TimePoint_t expiredTime; // caller will wait for response until this time. Dont proceed if it is already expired.
//  union
//  {
//    SystemMutex* waitMutex;
//    CommHttpCbOnUriResponse callback;
//  } notifier;
//  void* agr;
//
//  bool isValid(void)
//  {
//    do
//    {
//      if (this->enabled == 0)
//      {
//        return true;
//      }
//
//      if ( (notifyMode == COMM_HTTP_RESPOMSE_NOTIFY_MODE_CB)
//        && (this->notifier.callback == NULL)
//        )
//      {
//        break;
//      }
//
//      if ((notifyMode == COMM_HTTP_RESPOMSE_NOTIFY_MODE_MUTEX)
//        && (this->notifier.waitMutex == NULL)
//        )
//      {
//        break;
//      }
//
//      return true;
//    } while (0);
//    return false;
//  }
//} CommHttpNotifierTAG;

typedef struct _commHttpNotifierTag
{
  TimePoint_t expiredTime; // caller will wait for response until this time. Dont proceed if it is already expired.
  CommHttpCbOnUriResponse callback;
  void* agr;
  _commHttpNotifierTag(void)
  {
    this->expiredTime = 0;
    this->callback = NULL;
    this->agr = NULL;
  }
  bool isEnabled(void)
  {
    return (this->callback == NULL ? false : true);
  }
} CommHttpNotifierTAG;

typedef struct _commHttpUriRequestTAG
{
  CommHttpPackageTAG reqData;
  CommHttpNotifierTAG resNoti;
  CommHttpPackageTAG resData;
  uint16_t requestId;
} CommHttpUriRequestTAG;

typedef struct _commHttpClientRequestParamsTAG
{
  CommHttpHeaderTAG header;
  CommHttpNotifierTAG notifier;
  char* body; // file path or buffer
  DataSize_t bodyLen; // bufferLen or filePathLen
} CommHttpClientRequestParamsTAG;

// events
typedef struct _eventCommHttpStartTAG
{
  CommHttpConnectionConfigU config;
} EventCommHttpStartTAG;

typedef struct _eventCommHttpConnectionOpenedTAG
{
  byte reserved; 
} EventCommHttpConnectionOpenedTAG;

typedef struct _eventCommHttpDataReceivedTAG
{
  BufferDataItem* dataItem;
} EventCommHttpDataReceivedTAG;

typedef struct _eventCommHttpConnectionClosedTAG
{
  byte reserved;
} EventCommHttpConnectionClosedTAG;

typedef struct _eventCommHttpStopTAG
{
  byte reserved;
} EventCommHttpStopTAG;

typedef struct _eventCommHttpFinishedTAG
{
  byte reserved;
} EventCommHttpFinishedTAG;

typedef struct _eventCommHttpClientRequestTAG
{
  CommHttpUriRequestTAG param;
  //CommHttpClientRequestParamsTAG params;
  //BufferDataItem* dataItem;
} EventCommHttpClientRequestTAG;

// @param1: user arg pointer
// @param2: request information
//struct CommHttpUriRequestTAG;
//typedef int(*CommHttpCbOnUriRespond)(CommHttpUriRespondTAG* uriResponse);
/////////////////////////////////////////////////
// GLOBAL VARIABLES

/////////////////////////////////////////////////
// GLOBAL FUNCTIONS

/////////////////////////////////////////////////
// STATIC DATA
static const char* COMM_HTTP_CODE_STR_TBL[COMM_HTTP_STATUS_CODE_MAX] =
{
  "200 OK"
  , "201 201"
  , "202 202"
  , "203 203"
  , "204 204"
  , "401 401"
  , "404 Not found"
  , "500 Internal error"
};

static const char* COMM_HTTP_DATA_TYPE_STR_TBL[COMM_HTTP_DATA_TYPE_MAX] =
{
  "text/plain",
  "text/html",
  "application/javascript",
  "text/css",
  "image/png",
  "image/x-icon",
  "text/xml",
  "application/json"
};

static const httpd_method_t COMM_HTTP_SERVER_METHOD_TBL[COMM_HTTP_METHOD_MAX] =
{
  httpd_method_t::HTTP_GET,
  httpd_method_t::HTTP_PUT,
  httpd_method_t::HTTP_POST
};

static const esp_http_client_method_t COMM_HTTP_CLIENT_METHOD_TBL[COMM_HTTP_METHOD_MAX] =
{
  esp_http_client_method_t::HTTP_METHOD_GET,
  esp_http_client_method_t::HTTP_METHOD_PUT,
  esp_http_client_method_t::HTTP_METHOD_POST
};

/////////////////////////////////////////////////
// STATIC FUNCTIONS

/////////////////////////////////////////////////
// EXTERN

/////////////////////////////////////////////////
// CLASS DEFINITION
/*CommHttpConstant*/
class CommHttpConstant
{
public:
  //static httpd_method_t convert2DrvMethod(byte method)
  //{
  //  switch (method)
  //  {
  //  case (byte)CommHttpMethod::CommHttpMethodPost:
  //      return HTTP_POST;
  //  case (byte)CommHttpMethod::CommHttpMethodGet:
  //  default:
  //    return HTTP_GET;
  //  }
  //}

  static byte convert2CommMethod(int method)
  {
    switch (method)
    {
    case (int)HTTP_POST:
      return COMM_HTTP_METHOD_POST;
    case (int)HTTP_GET:
      return COMM_HTTP_METHOD_GET;
    default:
      return COMM_HTTP_METHOD_MAX;
    }
  }

  /* Set HTTP response content type according to file extension */
  static byte fileExt2DataType(const char* filePath)
  {
    if (Utility::checkFileExtension(filePath, ".html") != false)
    {
      return COMM_HTTP_DATA_TYPE_TEXT_HTML;
    }

    if (Utility::checkFileExtension(filePath, ".js") != false)
    {
      return COMM_HTTP_DATA_TYPE_APP_JS;
    }
    
    if (Utility::checkFileExtension(filePath, ".css") != false)
    {
      return COMM_HTTP_DATA_TYPE_TEXT_CSS;
    }

    if (Utility::checkFileExtension(filePath, ".png") != false)
    {
      return COMM_HTTP_DATA_TYPE_IMG_PNG;
    }

    if (Utility::checkFileExtension(filePath, ".ico") != false)
    {
      return COMM_HTTP_DATA_TYPE_IMG_ICON;
    }

    if (Utility::checkFileExtension(filePath, ".svg") != false)
    {
      return COMM_HTTP_DATA_TYPE_TEXT_XML;
    }

    if (Utility::checkFileExtension(filePath, ".json") != false)
    {
      return COMM_HTTP_DATA_TYPE_APP_JSON;
    }

    return COMM_HTTP_DATA_TYPE_TEXT_PLAIN;
  }

  static byte dataType2Code(const char* str)
  {
    for (unsigned int wk = 0; wk < COMM_HTTP_DATA_TYPE_MAX; wk++)
    {
      if (strcasecmp(str, COMM_HTTP_CODE_STR_TBL[wk]) == 0)
      {
        return wk;
      }
    }
    return COMM_HTTP_DATA_TYPE_MAX;
  }

  static byte statusCode2Code(int rawCode)
  {
    switch (rawCode)
    {
      case 200:
        return COMM_HTTP_STATUS_CODE_200;
      case 201:
        return COMM_HTTP_STATUS_CODE_201;
      case 202:
        return COMM_HTTP_STATUS_CODE_202;
      case 203:
        return COMM_HTTP_STATUS_CODE_203;
      case 204:
        return COMM_HTTP_STATUS_CODE_204;
      case 401:
        return COMM_HTTP_STATUS_CODE_401;
      case 404:
        return COMM_HTTP_STATUS_CODE_404;
      case 500:
      default:
        return COMM_HTTP_STATUS_CODE_500;
    }
  }

  //static const char* statusCode2String(byte statusCode)
  //{
  //  switch (statusCode)
  //  {
  //  case (byte)CommHttpDataType::CommHttpDataTextHtml:
  //    return COMM_HTTP_DATA_TYPE_STR_TEXT_HTML;
  //  case (byte)CommHttpDataType::CommHttpDataTextCss:
  //    return COMM_HTTP_DATA_TYPE_STR_TEXT_CSS;
  //  case (byte)CommHttpDataType::CommHttpDataTextXml:
  //    return COMM_HTTP_DATA_TYPE_STR_TEXT_XML;
  //  case (byte)CommHttpDataType::CommHttpDataImgPng:
  //    return COMM_HTTP_DATA_TYPE_STR_IMG_PNG;
  //  case (byte)CommHttpDataType::CommHttpDataImgIcon:
  //    return COMM_HTTP_DATA_TYPE_STR_IMG_ICON;
  //  case (byte)CommHttpDataType::CommHttpDataAppJs:
  //    return COMM_HTTP_DATA_TYPE_STR_APP_JS;
  //  case (byte)CommHttpDataType::CommHttpDataAppJson:
  //    return COMM_HTTP_DATA_TYPE_STR_APP_JSON;
  //  case (byte)CommHttpDataType::CommHttpDataTextPlain:
  //  default:
  //    return COMM_HTTP_DATA_TYPE_STR_TEXT_PLAIN;
  //  }
  //}
};

#endif // _CONF_COMM_HTTP_CONSTANT_ENABLED

#endif // _COMM_HTTP_CONSTANT_H