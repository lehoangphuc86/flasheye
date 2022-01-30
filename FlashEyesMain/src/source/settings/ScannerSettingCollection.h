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
  int                                                           set(SettingParamTAG& settingParam) override;
  int                                                           get(SettingParamTAG& settingParam) override;
  uint8_t                                                       commBaudrate(void);
  void                                                          commBaudrate(uint8_t newVal, bool updateDB = false);
  uint8_t                                                       commParity(void);
  void                                                          commParity(uint8_t newVal, bool updateDB = false);
  uint8_t                                                       commStopbit(void);
  void                                                          commStopbit(uint8_t newVal, bool updateDB = false);
  uint8_t                                                       commDatabit(void);
  void                                                          commDatabit(uint8_t newVal, bool updateDB = false);
  uint8_t                                                       commFlowCtrl(void);
  void                                                          commFlowCtrl(uint8_t newVal, bool updateDB = false);
  uint8_t                                                       characterFormat(void);
  void                                                          characterFormat(uint8_t newVal, bool updateDB = false);
  uint8_t                                                       readState(void);
  void                                                          readState(uint8_t newVal, bool updateDB = false);
  uint8_t                                                       autoScan(void);
  void                                                          autoScan(uint8_t newVal, bool updateDB = false);
  uint8_t                                                       contScan(void);
  void                                                          contScan(uint8_t newVal, bool updateDB = false);
  uint8_t                                                       securityLevel(void);
  void                                                          securityLevel(uint8_t newVal, bool updateDB = false);
  uint8_t                                                       enablePrefix(void);
  void                                                          enablePrefix(uint8_t newVal, bool updateDB = false);
  char*                                                         customPrefix(void);
  void                                                          customPrefix(char* newVal, DataSize_t newValLen, bool updateDB = false);
  char*                                                         customSuffix(void);
  void                                                          customSuffix(char* newVal, DataSize_t newValLen, bool updateDB = false);
  uint8_t                                                       customCodeFormat(void);
  void                                                          customCodeFormat(uint8_t newVal, bool updateDB = false);
  uint8_t                                                       messTerminator(void);
  void                                                          messTerminator(uint8_t newVal, bool updateDB = false);

protected:
  NumSettingItem<uint8_t>                                       comm_Baudrate;
  NumSettingItem<uint8_t>                                       comm_Parity;
  NumSettingItem<uint8_t>                                       comm_Stopbit;
  NumSettingItem<uint8_t>                                       comm_Databit;
  NumSettingItem<uint8_t>                                       comm_FlowCtrl;
  NumSettingItem<uint8_t>                                       character_Format;
  NumSettingItem<uint8_t>                                       read_State;
  NumSettingItem<uint8_t>                                       auto_Scan;
  NumSettingItem<uint8_t>                                       cont_Scan;
  NumSettingItem<uint8_t>                                       security_Level;
  NumSettingItem<uint8_t>                                       enable_Prefix;
  StrSettingItem                                                custom_Prefix;
  StrSettingItem                                                custom_Suffix;
  NumSettingItem<uint8_t>                                       custom_Code_Format;
  NumSettingItem<uint8_t>                                       mess_Terminator;
};
#endif // _CONF_SCANNER_SETTING_COLLECTION_ENABLED

#endif // _SCANNER_SETTING_COLLECTION_H