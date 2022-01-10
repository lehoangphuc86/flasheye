#include "ScanningTaskConstant.h"

#if (_CONF_SCANNING_CONTROLLER_ENABLED)

#ifndef _SCANNING_CONTROLLER_H
#define _SCANNING_CONTROLLER_H

/////////////////////////////////////////////////
// INCLUDE

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

/*ScanningController*/

class ScanningController
{
public:
  ScanningController(byte deviceType);
  virtual ~ScanningController(void);
  virtual bool                                                  isValid(void);
  byte                                                          deviceType(void);
  virtual int                                                   start(ScanningDeviceConfigTAG& deviceConfig) = 0;
  virtual void                                                  stop(void) = 0;
  // controlling functions
  virtual int                                                   startScan(void) = 0;
  virtual int                                                   readResult(ScanningDeviceResultTAG& scanResult) = 0;
  virtual void                                                  stopScan(void) = 0;
  virtual int                                                   setting(ScanningDeviceSettingTAG& setting, ScanningDeviceSettingResultTAG& outResult) = 0;
protected:
  byte                                                          device_Type;
};

#endif // _SCANNING_CONTROLLER_H

#endif // _CONF_SCANNING_CONTROLLER_ENABLED