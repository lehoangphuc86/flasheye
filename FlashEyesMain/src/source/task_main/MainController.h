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
#include "../settings/SettingManager.h"
#include "../task_scanner/ScanningControlTask.h"
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
/*MainController*/
class MainController
  : public TaskManager
{
public:
  MainController(byte systemMode);
  virtual ~MainController(void);
  bool                                                          isBusy(void);
  virtual bool                                                  isValid(void);
  byte                                                          systemMode(void);
  BufferDataItem*                                               getCommData(void);
  BufferDataManager*                                            dataManager(void);
  void                                                          releaseCommData(BufferDataItem* dataItem);
  virtual int                                                   start(MainControllerConfigTAG& controllerConfig);
  virtual void                                                  stop(void);
protected:
  void                                                          isBusy(bool flag);
  Seq_t                                                         curSeqId(void);
  Seq_t                                                         nextSeqId(void);
  virtual void                                                  regEventSize(void) = 0;
  virtual void                                                  proc(void) override;
  virtual int                                                   prepare(void);
  virtual void                                                  clear(void);
  // inherited classes determine which events/handlers to be handled/called
  virtual int                                                   onEventHandling(EventDataItem* eventData) = 0;

  //###############below functions are implemented but called inside inherited classes##############
  // start sub tasks
  virtual int                                                   startNetManager(void);
  virtual int                                                   startScanningTask(void);
  
  // stop sub tasks
  virtual void                                                  stopNetManager(void);
  virtual void                                                  stopScanningTask(void);
  // event handlers
  virtual int                                                   onEventScanningDeviceSetting(unsigned char* data, unsigned int dataSize);
  virtual int                                                   onEventScanningDeviceSettingCompleted(unsigned char* data, unsigned int dataSize);
  virtual int                                                   onEventTriggerStartScanning(unsigned char* data, unsigned int dataSize);
  virtual int                                                   onEventScanningResult(unsigned char* data, unsigned int dataSize);
  virtual int                                                   onEventScanningCompleted(unsigned char* data, unsigned int dataSize);
  virtual int                                                   onEventTimerFired1(unsigned char* data, unsigned int dataSize);
  // common functions
  virtual void                                                  resetSequence(void);
  virtual int                                                   startTimer(TimePoint_t timeout);
  virtual void                                                  stopTimer(void);
  virtual void                                                  stopScanning(void);

protected:
  byte                                                          system_Mode;
  BufferDataManager                                             data_Manager;
  byte                                                          is_Busy;
  Seq_t                                                         sequence_Id;
  SystemCriticalSession                                         is_Busy_Key;
  SystemMutex                                                   mutex_Operating;
  //###########below variables are defined but used inside inherited classes#############
  ScanningControlTask                                           scanning_Task;
  TimerId_t                                                     timer_Id;
  
protected:
  static void                                                   cbTimerFired(TimerId_t timerId, void* extraArg, bool* woken);
};

#endif // _MAIN_CONTROLLER_H

#endif // _CONF_MAIN_CONTROLLER_ENABLED