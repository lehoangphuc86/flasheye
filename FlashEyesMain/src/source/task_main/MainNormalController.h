#include "MainController.h"

#if (_CONF_MAIN_NORMAL_CONTROLLER_ENABLED)

#ifndef _MAIN_NORMAL_CONTROLLER_H
#define _MAIN_NORMAL_CONTROLLER_H

/////////////////////////////////////////////////
// INCLUDE
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
/*MainNormalController*/
class MainNormalController
  : public MainController
{
public:
  MainNormalController(void);
  virtual ~MainNormalController(void);
public:
  bool                                                          isValid(void) override;  
protected:
  void                                                          regEventSize(void) override;
  int                                                           prepare(void) override;
  void                                                          clear(void) override;
  int                                                           onEventHandling(EventDataItem* eventData) override;
  int                                                           onEventScanningDeviceSetting(unsigned char* data, unsigned int dataSize);
  int                                                           onEventScanningDeviceSettingCompleted(unsigned char* data, unsigned int dataSize);
  int                                                           onEventTriggerStartScanning(unsigned char* data, unsigned int dataSize);
  int                                                           onEventScanningResult(unsigned char* data, unsigned int dataSize);
  int                                                           onEventScanningCompleted(unsigned char* data, unsigned int dataSize);
  int                                                           onEventTimerFired1(unsigned char* data, unsigned int dataSize);

  void                                                          resetSequence(void);
  int                                                           startTimer(TimePoint_t timeout);
  void                                                          stopTimer(void);
  void                                                          stopScanning(void);
protected:
  static void                                                   cbTimerFired(TimerId_t timerId, void* extraArg, bool* woken);
protected:
  ScanningControlTask                                           scanning_Task;
  TimerId_t                                                     timer_Id;
};
#endif // _MAIN_NORMAL_CONTROLLER_H

#endif // _CONF_MAIN_NORMAL_CONTROLLER_ENABLED