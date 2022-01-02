#include "SettingManagerConstant.h"
#ifndef _SCANNER_SETTING_COLLECTION_H
#define _SCANNER_SETTING_COLLECTION_H

#if (_CONF_SCANNER_SETTING_COLLECTION_ENABLED)
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
/*ScannerSettingCollection*/
class ScannerSettingCollection
  : public SettingCollection
{
public:
  ScannerSettingCollection(void);
  __ATTRIBUTE_VIRTUAL_OPTIMIZED ~ScannerSettingCollection(void);
  int                                                           initialize(SettingCollectionConfigTAG& settingConfig) override;
  uint8_t                                                       scanMode(void);
  void                                                          scanMode(uint8_t newVal, bool updateDB = false);
  char*                                                         codePrefix(void);
  void                                                          codePrefix(char* newVal, DataSize_t newValLen, bool updateDB = false);
protected:
  NumSettingItem<uint8_t>                                       scan_Mode;
  StrSettingItem                                                code_Prefix;
};
#endif // _CONF_SCANNER_SETTING_COLLECTION_ENABLED

#endif // _SCANNER_SETTING_COLLECTION_H