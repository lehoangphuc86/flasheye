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
#include "mess_broker/MessBrokerManager.h"
#include "../task_excomm/ExCommConstant.h"
#include "task_dist_sensor/DistSensorControlTask.h"
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
  Seq_t                                                         nextSeqId(Seq_t inSeqId);
  virtual void                                                  regEventSize(void);
  virtual void                                                  proc(void) override;
  virtual int                                                   prepare(void);
  virtual void                                                  clear(void);
  // inherited classes determine which events/handlers to be handled/called
  virtual int                                                   onEventHandling(EventDataItem* eventData) = 0;

  //###############below functions are implemented but called inside inherited classes##############
  // start sub tasks
  virtual int                                                   startButtonTask(void);
  virtual int                                                   startMessBroker(void);
  virtual int                                                   startNetManager(void);
  virtual int                                                   startExCommManager(void);
  virtual int                                                   startScanningTask(void);
  virtual int                                                   startDistSensorTask(void);
  
  // stop sub tasks
  virtual void                                                  stopDistSensorTask(void);
  virtual void                                                  stopScanningTask(void);
  virtual void                                                  stopExCommManager(void);
  virtual void                                                  stopNetManager(void);
  virtual void                                                  stopMessBroker(void);
  virtual void                                                  stopButtonTask(void);

  // event handlers
  virtual int                                                   onEventScanningDeviceSetting(unsigned char* data, unsigned int dataSize);
  virtual int                                                   onEventScanningDeviceSettingCompleted(unsigned char* data, unsigned int dataSize);
  virtual int                                                   onEventScanningControl(unsigned char* data, unsigned int dataSize);
  virtual int                                                   onEventScanningStart(unsigned char* data, unsigned int dataSize);
  virtual int                                                   onEventScanningStop(unsigned char* data, unsigned int dataSize);
  virtual int                                                   onEventScanningResult(unsigned char* data, unsigned int dataSize);
  virtual int                                                   onEventScanningCompleted(unsigned char* data, unsigned int dataSize);
  virtual int                                                   onEventTimerFired1(unsigned char* data, unsigned int dataSize);
  virtual int                                                   onEventSysPower(unsigned char* data, unsigned int dataSize);

  // mbc comm handler
  virtual int                                                   onMbcCommMBCStart1(ExCommMBCParamTAG& mbcParams);
  virtual int                                                   onMbcCommMBCSystemSetting(ExCommMBCParamTAG& mbcParams);
  virtual int                                                   onMbcCommMBCScanningControl(ExCommMBCParamTAG& mbcParams);
  virtual int                                                   onMbcCommMBCScanningResult(ExCommMBCParamTAG& mbcParams);

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
  SystemCriticalSession                                         operation_Key;
  SystemMutex                                                   mutex_Operating;
  //###########below variables are defined but used inside inherited classes#############
  ScanningControlTask                                           scanning_Task;
  DistSensorControlTask                                         dist_Sensor_Task;
  TimerId_t                                                     timer_Id;
  
protected:
  static void                                                   cbTimerFired(TimerId_t timerId, void* extraArg, bool* woken);
  static int                                                    cbExCommRev(void* arg, ExCommMBCParamTAG& mbcParams);
  static void                                                   cbButtonPressed(void* arg, byte opCode, bool* woken);
  static void                                                   cbDistSensorTrg(void* userArg, Seq_t sequenceId, Dist_t distance, byte mode);
};

#endif // _MAIN_CONTROLLER_H

#endif // _CONF_MAIN_CONTROLLER_ENABLED