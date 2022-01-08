#include "BuzzerTaskConstant.h"

#ifdef _CONF_BUZZER_DEVICE_INFO_ENABLED

#ifndef _BUZZER_DEVICE_INFO_H
#define _BUZZER_DEVICE_INFO_H
/////////////////////////////////////////////////
// INCLUDE
#include "os_system/SystemMutex.h"
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
class BuzzerController;
/*BuzzerDeviceInfo*/
class BuzzerDeviceInfo
{
  friend BuzzerController;
public:
  BuzzerDeviceInfo(void);
  __ATTRIBUTE_VIRTUAL_OPTIMIZED ~BuzzerDeviceInfo(void);
  bool                                                          isValid(void);
  bool                                                          isOn(void);
  BuzzerDeviceId_t                                              deviceId(void);
  int                                                           initialize(BuzzerDeviceConfigTAG& deviceConfig);
  void                                                          clear(void);
  void                                                          dump(void);
protected:
  BuzzerHWChannelId_t                                           hw_Channel_Id;
  BuzzerDeviceId_t                                              device_Id;
  byte                                                          hw_Pin;
  bool                                                          is_On;
  BuzzerController*                                             dev_Controller;
};


#endif // _BUZZER_DEVICE_INFO_H

#endif // _CONF_BUZZER_DEVICE_INFO_ENABLED