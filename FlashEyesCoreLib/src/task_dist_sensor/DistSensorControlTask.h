#include "DistSensorTaskConstant.h"

#if (_CONF_DIST_SENSOR_CONTROL_TASK_ENABLED)

#ifndef _DIST_SENSOR_CONTROL_TASK_H
#define _DIST_SENSOR_CONTROL_TASK_H

/////////////////////////////////////////////////
// INCLUDE
#include "task_manager/TaskManager.h"
#include "DistSensorControllerFactory.h"

#if (!_CONF_TASK_MANAGER_ENABLED)
#error Task manager is required
#endif // _CONF_TASK_MANAGER_ENABLED

#if (!_CONF_DIST_SENSOR_CONTROLLER_FACTORY_ENABLED)
#error Dist sensor controller factory is required
#endif // _CONF_DIST_SENSOR_CONTROLLER_FACTORY_ENABLED
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

/*DistSensorControlTask*/
class DistSensorControlTask
  : public TaskManager
{
public:
  DistSensorControlTask(void);
  virtual ~DistSensorControlTask(void);
  bool                                                          isBusy(void);
  bool                                                          isValid(void);
  bool                                                          isTriggerMode(void);
  int                                                           control(DistSensorControlParamsTAG& ctrParams);
  virtual int                                                   startTask(DistSensorTaskConfigTAG& distSensorConfig);
  virtual void                                                  stopTask(void);
protected:
  void                                                          isBusy(bool flag);
  Seq_t                                                         curSeqId(void);
  Seq_t                                                         nextSeqId(void);
  Seq_t                                                         nextSeqId(Seq_t inSeqId);
  virtual void                                                  proc(void) override;
  virtual int                                                   prepare(void);
  virtual void                                                  clear(void);
  int                                                           measureDistance(void);
  int                                                           onEventDistanceControl(unsigned char* data, unsigned int dataSize);
  void                                                          resetSequence(void);

protected:
  DistSensorController*                                         ss_Controller;
  SystemCriticalSession                                         critical_Key;
  bool                                                          is_Busy;
  Seq_t                                                         sequence_Id;
  DistSensorCtrlConfigTAG                                       ctrl_Config;
};

#endif // _SCANNING_CONTROL_TASK_H

#endif // _CONF_DIST_SENSOR_CONTROL_TASK_ENABLED