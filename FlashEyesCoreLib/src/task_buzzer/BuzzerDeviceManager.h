#include "BuzzerTaskConstant.h"

#ifdef _CONF_BUZZER_DEVICE_MANAGER_ENABLED

#ifndef _BUZZER_DEVICE_MANAGER_H
#define _BUZZER_DEVICE_MANAGER_H
/////////////////////////////////////////////////
// INCLUDE
#include "BuzzerDeviceInfo.h"
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

/*BuzzerDeviceManager*/
class BuzzerDeviceManager
{
public:
  static BuzzerDeviceManager& getInstance(void);
  BuzzerDeviceManager(BuzzerDeviceManager const&) = delete;
  void operator=(BuzzerDeviceManager const&) = delete;
  bool                                                          isValid(void);
  int                                                           initialize(BuzzerDeviceManagerConfigTAG& deviceManagerConfig);
  int                                                           appendDevices(BuzzerDeviceConfigTAG* deviceConfig, byte deviceCount);
  int                                                           appendDevice(BuzzerDeviceConfigTAG& deviceConfig);
  byte                                                          deviceCount(void);
  BuzzerDeviceInfo*                                             deviceByDeviceId(BuzzerDeviceId_t deviceId);
  BuzzerDeviceInfo*                                             deviceByIndex(byte deviceIndex);
  void                                                          clear(void);
  void                                                          dump(void);
private:
  BuzzerDeviceManager(void);
  ~BuzzerDeviceManager(void);
protected:
  byte                                                          device_Count;
  byte                                                          device_Max_Count;
  BuzzerDeviceInfo*                                             device_List;
};

#endif // _BUZZER_DEVICE_MANAGER_H

#endif // _CONF_BUZZER_DEVICE_MANAGER_ENABLED