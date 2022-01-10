#include "ScanningTaskConstant.h"

#if (_CONF_SCANNING_CONTROL_TASK_ENABLED)

#ifndef _SCANNING_CONTROL_TASK_H
#define _SCANNING_CONTROL_TASK_H

/////////////////////////////////////////////////
// INCLUDE
#include "task_manager/TaskManager.h"
#include "ScanningControllerFactory.h"

#if (!_CONF_TASK_MANAGER_ENABLED)
#error Task manager is required
#endif // _CONF_TASK_MANAGER_ENABLED

#if (!_CONF_SCANNING_CONTROLLER_FACTORY_ENABLED)
#error Scanning controller factory is required
#endif // _CONF_SCANNING_CONTROLLER_FACTORY_ENABLED
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

/*ScanningControlTask*/
class ScanningControlTask
  : public TaskManager
{
public:
  ScanningControlTask(void);
  virtual ~ScanningControlTask(void);
  bool                                                          isBusy(void);
  bool                                                          isValid(void);
  virtual int                                                   inititialize(void);
  virtual int                                                   startTask(ScanningTaskConfigTAG& scanningConfig);
  virtual void                                                  stopTask(void);
  virtual void                                                  cleanUp(void);
protected:
  void                                                          isBusy(bool flag);
  virtual void                                                  proc(void) override;
  virtual int                                                   prepare(void);
  int                                                           onEventScanningStart(unsigned char* data, unsigned int dataSize);
  int                                                           onEventScanningStop(unsigned char* data, unsigned int dataSize);
  int                                                           onEventScanningDeviceSetting(unsigned char* data, unsigned int dataSize);
  int                                                           onEventScanningDeviceSignal(unsigned char* data, unsigned int dataSize);
  int                                                           notifyParent(EventId_t messageId, EventSize_t eventSize, unsigned char* eventData);
  void                                                          resetSequence(void);
private:
  static void                                                   cbScanningDeviceSignal(void* userArg, byte signalId, void* eventData, DataSize_t eventSize, bool* woken);
protected:
  ScanningController*                                           sc_Controller;
  Seq_t                                                         cur_Scan_SeqId;
  byte                                                          cur_Scan_Index;
  byte                                                          max_Scan_Count;
  SystemCriticalSession                                         critical_Key;
  bool                                                          is_Busy;
  EventManager*                                                 parent_Eventer;
};

#endif // _SCANNING_CONTROL_TASK_H

#endif // _CONF_SCANNING_CONTROL_TASK_ENABLED