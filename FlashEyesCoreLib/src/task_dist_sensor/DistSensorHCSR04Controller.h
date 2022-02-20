#include "DistSensorController.h"

#if (_CONF_DIST_SENSOR_HCRS04_CONTROLLER_ENABLED)

#ifndef _DIST_SENSOR_HCRS_CONTROLLER_H
#define _DIST_SENSOR_HCRS_CONTROLLER_H

/////////////////////////////////////////////////
// INCLUDE
#include "os_system/SystemCriticalSession.h"
#include "../libs/hcsr04/HCSR04.h"
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

/*ScanningEVController*/

class DistSensorHCSR04Controller
  : public DistSensorController
{
public:
  DistSensorHCSR04Controller(void);
  virtual ~DistSensorHCSR04Controller(void);
  bool                                                          isValid(void) override;
  int                                                           start(DistSensorDeviceConfigTAG& deviceConfig) override;
  void                                                          stop(void) override;
  // controlling functions
  int                                                           measureDistanceCm(byte deviceId, double& outDistance) override;
protected:

protected:
  HCSR04*                                                       sensor_Controller;
};

#endif // _DIST_SENSOR_HCRS_CONTROLLER_H

#endif // _CONF_DIST_SENSOR_HCRS04_CONTROLLER_ENABLED