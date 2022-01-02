#include "MainControllerConstant.h"

#if (_CONF_MAIN_CONTROLLER_ENABLED)

#ifndef _MAIN_CONTROLLER_H
#define _MAIN_CONTROLLER_H

/////////////////////////////////////////////////
// INCLUDE
#include "task_manager/TaskManager.h"
#if (!_CONF_TASK_MANAGER_ENABLED)
#error Task manager is required
#endif // !_CONF_TASK_MANAGER_ENABLED
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
  union 
  {
    MainResetControllerConfigTAG resetConfig;
    MainSettingControllerConfigTAG settingConfig;
    MainNormalControllerConfigTAG normalConfig;
  } config;
} MainControllerConfigTAG;
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
/*MainController*/
class MainController
  : public TaskManager
{
public:
  MainController(byte systemMode);
  virtual ~MainController(void);
  virtual bool                                                  isValid(void);
  byte                                                          systemMode(void);
  bool                                                          isBusy(void);
  void                                                          isBusy(bool flag);
  BufferDataItem*                                               getCommData(void);
  BufferDataManager*                                            dataManager(void);
  void                                                          releaseCommData(BufferDataItem* dataItem);

  virtual int                                                   setConfig(MainControllerConfigTAG& controllerConfig) = 0;
  virtual int                                                   start(void);
  virtual void                                                  stop(void);
  virtual void                                                  cleanUp(void);

protected:
  Seq_t                                                         nextSeqId(void);
  virtual void                                                  regEventSize(void) = 0;
  virtual int                                                   prepare(void);

protected:
  byte                                                          system_Mode;
  MainControllerConfigTAG                                       controller_Config;
  BufferDataManager                                             data_Manager;
  byte                                                          is_Busy;
  Seq_t                                                         sequence_Id;
  SystemCriticalSession                                         is_Busy_Key;
  SystemMutex                                                   mutex_Operating;
};

#endif // _MAIN_CONTROLLER_H

#endif // _CONF_MAIN_CONTROLLER_ENABLED