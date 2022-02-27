#include "../../FlashEyesMain.h"
#ifndef _EXPORT_MANAGER_CONSTANT_H
#define _EXPORT_MANAGER_CONSTANT_H
#if (_CONF_EXPORT_MANAGER_CONSTANT_ENABLED)

/////////////////////////////////////////////////
// INCLUDE
#include "../task_scanner/ScanningTaskConstant.h"
#include "../task_comm_mbc/task_comm_mbc_http/CommMBCHttpClient.h"
/////////////////////////////////////////////////
// PREPROCESSOR

/////////////////////////////////////////////////
// DEFINE

//################UiMessOpModeTAG::mode#########
#define EXPORT_SCANNING_DEVICE_BARCODE_LEN_MAX                      48

/////////////////////////////////////////////////
// MARCO

/////////////////////////////////////////////////
// DATA TYPE (TYPEDEF)

/////////////////////////////////////////////////
// DATA TYPE (ENUM)
typedef enum _exportProcessorTypeUN
{
  ExportProcessorHttpClient = 0,
  ExportProcessorTypeMax
} ExportProcessorTypeUN;

/////////////////////////////////////////////////
// DATA TYPE (STRUCT)

typedef struct _exportHttpClientProcessorReqParamTag
{
  byte reqMethod;
  const char* reqUri;
  byte dataType;
} ExportHttpClientProcessorReqParamTAG;

typedef struct _exportHttpClientProcessorConfigTag
{
  CommMBCHttpClient* client;
  ExportHttpClientProcessorReqParamTAG reqParam;
} ExportHttpClientProcessorConfigTAG;

typedef struct _exportProcessorConfigTag
{
  byte exportProcType;
  union
  {
    ExportHttpClientProcessorConfigTAG httpClient;
  } spec;
} ExportProcessorConfigTAG;

typedef struct _exportManagerConfigTAG
{
  ExportProcessorConfigTAG exportProcConfig;
  TaskManagerConfigTAG taskManagerConfig;
  TaskThreadConfigTAG taskThreadConfig;
} ExportManagerConfigTAG;


typedef struct _exportBarCodeTag
{
  byte type;
  DataSize_t codeLen;
  char code[EXPORT_SCANNING_DEVICE_BARCODE_LEN_MAX + 1];
} ExportBarCodeTAG;

typedef struct _exportScanResultTag
{
  Seq_t sequenceId;
  byte scanIndex;
  ExportBarCodeTAG code;
} ExportScanResultTAG;

// event
typedef struct _eventExportScanResultTag
{
  ExportScanResultTAG result;
} EventExportScanResultTAG;

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

#endif // _CONF_EXPORT_MANAGER_CONSTANT_ENABLED

#endif // _EXPORT_MANAGER_CONSTANT_H