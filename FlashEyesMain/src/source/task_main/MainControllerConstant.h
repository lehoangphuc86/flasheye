#include "../../FlashEyesMain.h"
#if (_CONF_MAIN_CONTROLLER_CONSTANT_ENABLED)
#ifndef _MAIN_CONTROLLER_CONSTANT_H
#define _MAIN_CONTROLLER_CONSTANT_H

/////////////////////////////////////////////////
// INCLUDE
#include "os_system/SystemCommon.h"
#include "data_manager/BufferDataManager.h"
#include "../task_scanner/ScanningTaskConstant.h"
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
// DATA TYPE (STRUCT)
// events
typedef struct _mainResetControllerConfigTag
{
  byte reserved;
} MainResetControllerConfigTAG;

typedef struct _mainSettingControllerConfigTag
{
  byte reserved;
} MainSettingControllerConfigTAG;

typedef struct _mainNormalControllerConfigTag
{
  byte reserved;
} MainNormalControllerConfigTAG;

typedef struct _mainControllerConfigTag
{
  BufferDataManagerConfigTAG bufferConfig;
  TaskManagerConfigTAG taskManagerConfig;
  TaskThreadConfigTAG taskThreadConfig;
  union
  {
    MainResetControllerConfigTAG resetConfig;
    MainSettingControllerConfigTAG settingConfig;
    MainNormalControllerConfigTAG normalConfig;
  } config;
} MainControllerConfigTAG;

typedef ScanningParamsTAG TrgStartScanningParamTAG;
// events
typedef struct _eventScanningControlTag
{
  Seq_t sequenceId;
  bool isStart : 1;
  byte reserved : 7;
  TrgStartScanningParamTAG trgParams; // for start
} EventScanningControlTAG;

typedef struct _eventSysPowerTag
{
  byte opCode;
  TimePoint_t delay;
} EventSysPowerTAG;

/////////////////////////////////////////////////
// STATIC DATA

/////////////////////////////////////////////////
// STATIC FUNCTIONS

/////////////////////////////////////////////////
// EXTERN

/////////////////////////////////////////////////
// CLASS DEFINITION
/*MainControllerCOnstant*/
class MainControllerConstant
{
public:
  MainControllerConstant(void) {};
  // WARNING: if inherite from this class, deconstructor must be virtual
  __ATTRIBUTE_VIRTUAL_OPTIMIZED ~MainControllerConstant(void) {};
};

#endif // _MAIN_CONTROLLER_CONSTANT_H

#endif // _CONF_MAIN_CONTROLLER_CONSTANT_ENABLED