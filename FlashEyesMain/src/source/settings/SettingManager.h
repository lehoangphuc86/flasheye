#ifndef _SETTING_MANAGER_H
#define _SETTING_MANAGER_H

#include "SettingManagerConstant.h"
#if (_CONF_SETTING_MANAGER_ENABLED)
/////////////////////////////////////////////////
// INCLUDE
#include "ScannerSettingCollection.h"
#include "SystemSettingCollection.h"
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
/*SettingManager*/
class SettingManager
{
private:
  SettingManager(void);
  __ATTRIBUTE_VIRTUAL_OPTIMIZED ~SettingManager(void);

public:
  static SettingManager& getInstance(void);
  SettingManager(SettingManager const&) = delete;
  void operator=(SettingManager const&) = delete;

  int                                                           initialize(SettingManagerConfigTAG& settingConfig);
  bool                                                          isValid(void);
  int                                                           load(void);
  int                                                           save(void);
  void                                                          clear(void);
  ScannerSettingCollection&                                     scanner(void);
  SystemSettingCollection&                                      system(void);
  //MainSettingCollection&                                        main(void);
  //SensorSettingCollection&                                      sensor(void);
  //ExCommSettingCollection&                                      excomm(void);
  //UiSettingCollection&                                          ui(void);
protected:
  int                                                           initSettingList(byte listCount);
  void                                                          clearSettingList(void);

protected:
  ScannerSettingCollection                                      scanner_Set;
  SystemSettingCollection                                       system_Set;
  SettingCollection**                                           setting_List;
  byte                                                          setting_List_Count;
};
#endif // _CONF_SETTING_MANAGER_ENABLED

#endif // _SETTING_MANAGER_H