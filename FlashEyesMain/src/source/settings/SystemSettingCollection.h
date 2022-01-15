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
  uint8_t                                                       lastSysMode(void);
  void                                                          lastSysMode(uint8_t newVal, bool updateDB = false);
  
protected:
  NumSettingItem<uint8_t>                                       last_Sys_Mode;
};
#endif // _CONF_SYSTEM_SETTING_COLLECTION_ENABLED

#endif // _SYSTEM_SETTING_COLLECTION_H