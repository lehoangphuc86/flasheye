#ifndef _EX_COMM_CONSTANT_H
#define _EX_COMM_CONSTANT_H

#include "../../FlashEyesMain.h"
#if (_CONF_EX_COMM_CONSTANT_ENABLED)
/////////////////////////////////////////////////
// INCLUDE
#include "../task_comm_mbc/task_comm_mbc_http/CommMBCHttpConstant.h"
#include "task_comm_mbc/task_comm_mbc_serial/CommMBCSerialConstant.h"
#include "task_comm_mbc/task_comm_mbc_i2c/CommMBCI2CConstant.h"
//#include "wifi/NetWifiConstant.h"
/////////////////////////////////////////////////
// PREPROCESSOR

/////////////////////////////////////////////////
// DEFINE
#define EX_COMM_ID_INVALID                                        0
#define EX_COMM_ID_MBC_HTTP_SERVER_0                              1
#define EX_COMM_ID_MBC_HTTP_CLIENT_0                              2

#define EX_COMM_MBC_HTTP_SERVER_WAIT_RES_TIMEOUT                  500 // ms

#define EX_COMM_REV_RES_WAIT_TIME_MIN                             300 // ms
#define EX_COMM_REV_RES_WAIT_TIME_COFF                            100 // ms

#define EX_COMM_SEND_RES_WAIT_TIME_MIN                            300 // ms
#define EX_COMM_SEND_RES_WAIT_TIME_COFF                           100 // ms

/////////////////////////////////////////////////
// MARCO

/////////////////////////////////////////////////
// DATA TYPE (TYPEDEF)

/////////////////////////////////////////////////
// DATA TYPE (ENUM)

/////////////////////////////////////////////////
// DATA TYPE (STRUCT)
typedef struct _exCommMBCPackageTag
{
  bool valid;
  MbcMessageId_t messId;
  unsigned char* data;
  MbcDataSize_t dataSize;


  BufferDataItem* dataItem; // internal use
  BufferDataManager* dataManager; // internal use

  /*
  * release data buffer. Call this only when receiving data.
  */
  void release(void)
  {
    if ( (this->dataItem != NULL)
      || (this->dataManager != NULL)
      )
    {
      this->dataManager->release(this->dataItem);
    }
    this->dataItem = NULL;
    this->dataManager = NULL;
  }
} ExCommCommMBCPackageTAG;

/*
* @flag         : to let the caller know that callee has touched the message or not
* @expiredTime  : (not used) the subcribers have to take and respond the message before this time end
*                  if it is timed out, accessing the data would be come undefined.
*                  if 0, no timeout is set.
* @mutex        : the callee need to call mutex.unlock() when finishs to stop caller from waiting
*/
typedef struct _exCommResSpecTag
{
  byte flag; // 0: noone touched, 1: someone touched
  //TimePoint_t expiredTime;
  SystemMutex* mutex;

  /*bool isValid(void)
  {
    return (this->expiredTime == 0 ? false : true);
  }*/
  bool isValid(void)
  {
    return (this->mutex == NULL ? false : true);
  }
} ExCommResSpecTAG;

class ExCommManager;
/*
* @commId       : the id of comm object which received package or is requested to send package
* @exCommMgr    : the ExCommManager which manages the comm object
* @reqPackage   : HttpServer: this contains data received from HttpClient.
*                 HttpClient: this contains data to send to HttpServer
* @resPackage   : HttpServer: not used
*                 HttpClient: this contains responded data from HttpServer
* @resSpec      : HttpServer: not used
*                 HttpClient: support timeout from sending package to receiving package from HttpServer
*/
typedef struct _exCommMBCParamTag
{
  CommId_t commId;
  ExCommManager* exCommMgr;
  ExCommCommMBCPackageTAG reqPackage; 
  ExCommCommMBCPackageTAG resPackage; // fo
  ExCommResSpecTAG resSpec; // used for client request
} ExCommMBCParamTAG;

typedef _exCommMBCParamTag EventExCommReceivedTAG;



// @param1: _exCommMBCParamTag struct contains rev and res info
// @return:
//    0: OK
//    other: error
typedef int(*ExCommCb)(void* arg, ExCommMBCParamTAG& params);

typedef struct _exCommManagerConfigTag
{
  void* cbOnCommRevArg;
  ExCommCb cbOnCommRev;
} ExCommManagerConfigTAG;
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

#endif // _CONF_EX_COMM_CONSTANT_ENABLED

#endif // _EX_COMM_CONSTANT_H