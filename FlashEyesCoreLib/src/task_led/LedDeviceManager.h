#include "LedTaskConstant.h"

#ifdef _CONF_LED_DEVICE_MANAGER_ENABLED

#ifndef _LED_DEVICE_MANAGER_H
#define _LED_DEVICE_MANAGER_H
/////////////////////////////////////////////////
// INCLUDE
#include "LedDeviceInfo.h"
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

/*LedDeviceManager*/
class LedDeviceManager
{
public:
  static LedDeviceManager& getInstance(void);
  LedDeviceManager(LedDeviceManager const&) = delete;
  void operator=(LedDeviceManager const&) = delete;
  bool                                                          isValid(void);
  int                                                           initialize(LedDeviceManagerConfigTAG& deviceManagerConfig);
  int                                                           appendDevices(LedDeviceConfigTAG* deviceConfig, byte deviceCount);
  int                                                           appendDevice(LedDeviceConfigTAG& deviceConfig);
  byte                                                          deviceCount(void);
  LedDeviceInfo*                                                deviceByChannel(LedChannelId_t channelId);
  LedDeviceInfo*                                                deviceByIndex(byte deviceIndex);
  void                                                          clear(void);
  void                                                          dump(void);
private:
  LedDeviceManager(void);
  ~LedDeviceManager(void);
protected:
  byte                                                          device_Count;
  byte                                                          device_Max_Count;
  LedDeviceInfo*                                                device_List;
};

#endif // _LED_DEVICE_MANAGER_H

#endif // _CONF_LED_DEVICE_MANAGER_ENABLED