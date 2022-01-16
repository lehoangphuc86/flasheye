#include "../../FlashEyesMain.h"
#ifndef _SCANNING_TASK_CONSTANT_H
#define _SCANNING_TASK_CONSTANT_H
/////////////////////////////////////////////////
// INCLUDE
#if (_CONF_SCANNING_TASK_CONSTANT_ENABLED)
#include "os_system/SystemCommon.h"
//#include "../data_manager/BufferDataManager.h"
#include "task_manager/TaskManagerConstant.h"
#include "ScanningSettingDefinition.h"
/////////////////////////////////////////////////
// PREPROCESSOR

/////////////////////////////////////////////////
// DEFINE
//#########################HW#########################
//-------------------------HW pin---------------------
#define SCANNING_DEVICE_PIN_MIN                       PIN_NUMER_MIN
#define SCANNING_DEVICE_PIN_MAX                       PIN_NUMBER_MAX
#define SCANNING_DEVICE_PIN_INVALID                   255
#define SCANNING_DEVICE_PIN_LIB_DEFAULT               -1
//-------------------------signal ids-----------------
#define SCANNING_DEVICE_SIGNAL_ID_LED                 0
#define SCANNING_DEVICE_SIGNAL_ID_BUZZER              1
#define SCANNING_DEVICE_SIGNAL_ID_MAX                 2
#define SCANNING_DEVICE_SIGNAL_ID_INVALID             SCANNING_DEVICE_SIGNAL_ID_MAX

//#########################Error list#################
#define SCANNING_ERR_NONE                             0
#define SCANNING_ERR_BUSY                             1
#define SCANNING_ERR_INVALID_PARAM                    2
#define SCANNING_ERR_DEVICE_ERROR                     3
#define SCANNING_ERR_INVALID_OBJ                      4
#define SCANNING_ERR_UNKOWN                           99

//#########################static###########
#define SCANNING_CONTROLLER_MAX_INSTANCE              5
#define SCANNING_CONTROLLER_INSTANCE_INDX_INVALID     SCANNING_CONTROLLER_MAX_INSTANCE

//#########################other###########
#define SCANNING_DEVICE_BARCODE_LEN_MAX               48
#define SCANNING_SETTING_ID_SET_ALL                   UINT64_MAX
#define SCANNING_DEVICE_DATA_TIMEOUT_DEFAULT          500 // ms

/////////////////////////////////////////////////
// MARCO

/////////////////////////////////////////////////
// GLOBAL VARIABLES

/////////////////////////////////////////////////
// GLOBAL FUNCTIONS

/////////////////////////////////////////////////
// DATA TYPE (TYPEDEF)
// @userArg: the arg set by user
// @signalId: signal id
// @eventData: signal detail 
// @eventSize:  signal detail size
typedef void(*ScanningDeviceCbOnSignal)(void* userArg, byte signalId, void* eventData, DataSize_t eventSize, bool* woken);

typedef void(*ScanningDeviceIsrOnSignal)();


typedef uint8_t                                   ScanningErr_t;
typedef uint64_t                                  ScanningSettingIdSet_t;
typedef int16_t                                   ScanningCmdCode_t;
/////////////////////////////////////////////////
// DATA TYPE (ENUM)
typedef enum _scanningDeviceTypeUN
{
  ScanningEVDevice =0,
  ScanningDeviceTypeMax
} ScanningDeviceTypeUN;

/////////////////////////////////////////////////
// DATA TYPE (STRUCT)

typedef struct _scanningEVSerialConfigTag
{
  byte instanceIndex;
  uint32_t baudrate;
  byte bitPerByte;  // 5-8
  byte parityType; // DISPLAY_DEVICE_SC_PARITY_NONE / DISPLAY_DEVICE_SC_PARITY_ODD / DISPLAY_DEVICE_SC_PARITY_EVEN
  byte stopBitNum; // 1-2
  uint16_t timeout; // ms. set 0 to use default
  byte pin_TX;
  byte pin_RX;
} ScanningEVSerialConfigTAG;

typedef struct _scanningEVDeviceConfigTag
{
  ScanningEVSerialConfigTAG serialConfig;
  byte pin_Led;
  byte pin_Buzzer;
  byte pin_Sensor;
} ScanningEVDeviceConfigTAG;

typedef struct _scanningDeviceConfigTag
{
  byte deviceType;
  byte id;
  uint16_t dataTimeout; // reading cmd and bar code timeout. ms
  void* signalUserArg; // internal use
  ScanningDeviceCbOnSignal cbOnSignal; // internal use
  union 
  {
    ScanningEVDeviceConfigTAG evScanner;
  } spec;
} ScanningDeviceConfigTAG;

typedef struct _scanningTaskConfigTag
{
  void* parentEventer;
  ScanningDeviceConfigTAG deviceConfig;
  TaskManagerConfigTAG taskManagerConfig;
  TaskThreadConfigTAG taskThreadConfig;
} ScanningTaskConfigTAG;

typedef struct _scanningBarCodeTag
{
  byte type;
  DataSize_t codeLen;
  char code[SCANNING_DEVICE_BARCODE_LEN_MAX + 1];
} ScanningBarCodeTAG;

typedef struct _scanningDeviceResultTag
{
  ScanningErr_t errorId;
  //char code[SCANNING_DEVICE_BARCODE_LEN_MAX];
  ScanningBarCodeTAG code;
} ScanningDeviceResultTAG;

typedef struct _scanningDeviceSettingTag
{
  ScanningSettingIdSet_t settingIdSet0; // 64 setting
} ScanningDeviceSettingTAG;

typedef struct _scanningDeviceSettingResultTag
{
  ScanningSettingIdSet_t errorSet0; // 64 setting. 0: no error, 1: error
} ScanningDeviceSettingResultTAG;
//event

typedef struct _eventScanningGetReadyTag
{
  byte reserved;
} EventScanningGetReadyTAG;

typedef struct _eventScanningStartTag
{
  Seq_t seqId;
  byte maxScanCount;
} EventScanningStartTAG;

typedef struct _eventScanningStopTag
{
  byte reserved;
} EventScanningStopTAG;

typedef struct _eventScanningResultTag
{
  Seq_t seqId;
  byte scanIndex;
  ScanningDeviceResultTAG deviceResult;
} EventScanningResultTAG;

typedef struct _eventScanningCompletedTag
{
  Seq_t seqId;
  ScanningErr_t errorId;
  byte scannedCount;
} EventScanningCompletedTAG;

typedef struct _eventScanningDeviceSettingTag
{
  Seq_t seqId;
  ScanningDeviceSettingTAG setting;
} EventScanningDeviceSettingTAG;

typedef struct _eventScanningDeviceSettingCompletedTag
{
  Seq_t seqId;
  ScanningDeviceSettingResultTAG result;
} EventScanningDeviceSettingCompletedTAG;

typedef struct _eventScanningDeviceSignalTag
{
  byte signalType;
} EventScanningDeviceSignalTAG;

/////////////////////////////////////////////////
// STATIC DATA

/////////////////////////////////////////////////
// STATIC FUNCTIONS
//

/////////////////////////////////////////////////
// EXTERN

/////////////////////////////////////////////////
// CLASS DEFINITION


#endif // _CONF_SCANNING_TASK_CONSTANT_ENABLED

#endif // _SCANNING_TASK_CONSTANT_H

