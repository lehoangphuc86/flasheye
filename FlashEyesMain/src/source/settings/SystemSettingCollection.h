#include "SettingManagerConstant.h"
#ifndef _SYSTEM_SETTING_COLLECTION_H
#define _SYSTEM_SETTING_COLLECTION_H

#if (_CONF_SYSTEM_SETTING_COLLECTION_ENABLED)
/////////////////////////////////////////////////
// INCLUDE
#include "SettingCollection.h"
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
/*SystemSettingCollection*/
class SystemSettingCollection
  : public SettingCollection
{
public:
  SystemSettingCollection(void);
  __ATTRIBUTE_VIRTUAL_OPTIMIZED ~SystemSettingCollection(void);
  int                                                           initialize(SettingCollectionConfigTAG& settingConfig) override;
  int                                                           set(SettingParamTAG& settingParam) override;
  int                                                           get(SettingParamTAG& settingParam) override;
  uint8_t                                                       lastSysMode(void);
  void                                                          lastSysMode(uint8_t newVal, bool updateDB = false);
  uint8_t                                                       wifiMode(void);
  void                                                          wifiMode(uint8_t newVal, bool updateDB = false);
  char*                                                         wifiSSID(void);
  void                                                          wifiSSID(char* newVal, DataSize_t newValLen, bool updateDB = false);
  char*                                                         wifiPwd(void);
  void                                                          wifiPwd(char* newVal, DataSize_t newValLen, bool updateDB = false);
  char*                                                         httpCliUri(void);
  void                                                          httpCliUri(char* newVal, DataSize_t newValLen, bool updateDB = false);
  uint8_t                                                       httpCliDataType(void);
  void                                                          httpCliDataType(uint8_t newVal, bool updateDB = false);
  uint8_t                                                       httpCliMethod(void);
  void                                                          httpCliMethod(uint8_t newVal, bool updateDB = false);
  uint8_t                                                       exportMode(void);
  void                                                          exportMode(uint8_t newVal, bool updateDB = false);
protected:
  NumSettingItem<uint8_t>                                       last_Sys_Mode;
  NumSettingItem<uint8_t>                                       wifi_Mode;
  StrSettingItem                                                wifi_SSID;
  StrSettingItem                                                wifi_Pwd;
  NumSettingItem<uint8_t>                                       export_Mode;
  StrSettingItem                                                http_Cli_Uri;
  NumSettingItem<uint8_t>                                       http_Cli_Data_Type;
  NumSettingItem<uint8_t>                                       http_Cli_Method;
};
#endif // _CONF_SYSTEM_SETTING_COLLECTION_ENABLED

#endif // _SYSTEM_SETTING_COLLECTION_H