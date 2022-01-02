#include "ScannerSettingCollection.h"
#if (_CONF_SCANNER_SETTING_COLLECTION_ENABLED)
/////////////////////////////////////////////////
// INCLUDE

/////////////////////////////////////////////////
// PREPROCESSOR

/////////////////////////////////////////////////
// DEFINE
#define SCANNER_SETTING_COLLECTION_CONSOLE_DEBUG_ENABLE

/////////////////////////////////////////////////
// MARCO

/////////////////////////////////////////////////
// GLOBAL VARIABLES

/////////////////////////////////////////////////
// GLOBAL FUNCTIONS

/////////////////////////////////////////////////
// DATA TYPE (ENUM)

/////////////////////////////////////////////////
// DATA TYPE (STRUCT)

/////////////////////////////////////////////////
// STATIC DATA
#ifdef SCANNER_SETTING_COLLECTION_CONSOLE_DEBUG_ENABLE
char scannerSettingCollectionLogBuf[SYSTEM_CONSOLE_OUT_BUF_LEN];
#endif // SCANNER_SETTING_COLLECTION_CONSOLE_DEBUG_ENABLE
/////////////////////////////////////////////////
// STATIC FUNCTIONS

/////////////////////////////////////////////////
// EXTERN

/////////////////////////////////////////////////
// CLASS IMPLEMENTAION
/*ScannerSettingCollection*/
ScannerSettingCollection::ScannerSettingCollection(void)
  : SettingCollection()
{

}

ScannerSettingCollection::~ScannerSettingCollection(void)
{
  this->clear();
}

int ScannerSettingCollection::initialize(SettingCollectionConfigTAG& settingConfig)
{
  int ret = 0;
#ifdef SCANNER_SETTING_COLLECTION_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(scannerSettingCollectionLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[snSC] ini %i", 0);
#endif // SCANNER_SETTING_COLLECTION_CONSOLE_DEBUG_ENABLE
  do
  {
    if (SettingCollection::initialize(settingConfig) != 0)
    {
#ifdef SCANNER_SETTING_COLLECTION_CONSOLE_DEBUG_ENABLE
      CONSOLE_LOG_BUF(scannerSettingCollectionLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[snSC] ini %i", -1);
#endif // SCANNER_SETTING_COLLECTION_CONSOLE_DEBUG_ENABLE
      break;
    }

    ret = SettingCollection::initItemList(2);
#ifdef SCANNER_SETTING_COLLECTION_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(scannerSettingCollectionLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[snSC] ini %i %i", 2, ret);
#endif // SCANNER_SETTING_COLLECTION_CONSOLE_DEBUG_ENABLE
    if (ret != 0)
    {
      break;
    }

    this->item_List[0] = &this->scan_Mode;
    this->item_List[1] = &this->code_Prefix;

    SettingItemConfigTAG itemConfig = SettingItemConfigTAG();
    itemConfig.dbTableId = this->db_Table_Id;

    // scan mode
    {
#ifdef SCANNER_SETTING_COLLECTION_CONSOLE_DEBUG_ENABLE
      CONSOLE_LOG_BUF(scannerSettingCollectionLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[snSC] ini %i", 5);
#endif // SCANNER_SETTING_COLLECTION_CONSOLE_DEBUG_ENABLE
      itemConfig.dbKeyId = FEM_SET_SCANNER_DB_SCANMODE_KEY_ID;
      ret = this->scan_Mode.initialize(itemConfig);
#ifdef SCANNER_SETTING_COLLECTION_CONSOLE_DEBUG_ENABLE
      CONSOLE_LOG_BUF(scannerSettingCollectionLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[snSC] ini %i %i", 6, ret);
#endif // SCANNER_SETTING_COLLECTION_CONSOLE_DEBUG_ENABLE
      if (ret != 0)
      {
        break;
      }
    }
    
    // code prefix
    {
      itemConfig.dbKeyId = FEM_SET_SCANNER_DB_CODE_PREFIX_KEY_ID;
      itemConfig.sMaxLen = FEM_SET_SCANNER_DB_CODE_PREFIX_LEN_MAX;
#ifdef SCANNER_SETTING_COLLECTION_CONSOLE_DEBUG_ENABLE
      CONSOLE_LOG_BUF(scannerSettingCollectionLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[snSC] ini %i", 8);
#endif // SCANNER_SETTING_COLLECTION_CONSOLE_DEBUG_ENABLE
      ret = this->code_Prefix.initialize(itemConfig);
#ifdef SCANNER_SETTING_COLLECTION_CONSOLE_DEBUG_ENABLE
      CONSOLE_LOG_BUF(scannerSettingCollectionLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[snSC] ini %i %i", 9, ret);
#endif // SCANNER_SETTING_COLLECTION_CONSOLE_DEBUG_ENABLE
      if (ret != 0)
      {
        break;
      }
    }
#ifdef SCANNER_SETTING_COLLECTION_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(scannerSettingCollectionLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[snSC] ini %i", 99);
#endif // SCANNER_SETTING_COLLECTION_CONSOLE_DEBUG_ENABLE
    return 0;
  } while (0);
#ifdef SCANNER_SETTING_COLLECTION_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(scannerSettingCollectionLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[snSC] ini %i", -98);
#endif // SCANNER_SETTING_COLLECTION_CONSOLE_DEBUG_ENABLE
  this->clear();
#ifdef SCANNER_SETTING_COLLECTION_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(scannerSettingCollectionLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[snSC] ini %i", -99);
#endif // SCANNER_SETTING_COLLECTION_CONSOLE_DEBUG_ENABLE
  return -1;
}

uint8_t ScannerSettingCollection::scanMode(void)
{
  SystemMutexLocker locker(this->mutex_Key);
  return this->scan_Mode.get();
}

void ScannerSettingCollection::scanMode(uint8_t newVal, bool updateDB)
{
  SystemMutexLocker locker(this->mutex_Key);
  this->scan_Mode.set(newVal, updateDB);
}

char* ScannerSettingCollection::codePrefix(void)
{
  SystemMutexLocker locker(this->mutex_Key);
  return this->code_Prefix.get();
}

void ScannerSettingCollection::codePrefix(char* newVal, DataSize_t newValLen, bool updateDB)
{
  SystemMutexLocker locker(this->mutex_Key);
  this->code_Prefix.set(newVal, newValLen, updateDB);
}

#endif // _CONF_SCANNER_SETTING_COLLECTION_ENABLED