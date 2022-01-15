#include "SystemSettingCollection.h"
#if (_CONF_SYSTEM_SETTING_COLLECTION_ENABLED)
/////////////////////////////////////////////////
// INCLUDE

/////////////////////////////////////////////////
// PREPROCESSOR

/////////////////////////////////////////////////
// DEFINE
//#define SYSTEM_SETTING_COLLECTION_CONSOLE_DEBUG_ENABLE

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
#ifdef SYSTEM_SETTING_COLLECTION_CONSOLE_DEBUG_ENABLE
char systemSettingCollectionLogBuf[SYSTEM_CONSOLE_OUT_BUF_LEN];
#endif // SYSTEM_SETTING_COLLECTION_CONSOLE_DEBUG_ENABLE
/////////////////////////////////////////////////
// STATIC FUNCTIONS

/////////////////////////////////////////////////
// EXTERN

/////////////////////////////////////////////////
// CLASS IMPLEMENTAION
/*SystemSettingCollection*/
SystemSettingCollection::SystemSettingCollection(void)
  : SettingCollection()
{

}

SystemSettingCollection::~SystemSettingCollection(void)
{
  this->clear();
}

int SystemSettingCollection::initialize(SettingCollectionConfigTAG& settingConfig)
{
  int ret = 0;
  do
  {
    if (SettingCollection::initialize(settingConfig) != 0)
    {
      break;
    }

    ret = SettingCollection::initItemList(1);
    if (ret != 0)
    {
      break;
    }

    this->item_List[0] = &this->last_Sys_Mode;
    SettingItemConfigTAG itemConfig = SettingItemConfigTAG();
    itemConfig.dbTableId = this->db_Table_Id;

    // comm_Baudrate
    itemConfig.dbKeyId = FEM_SET_DB_KEY_ID_SYS_LAST_SYS_MODE;
    ret = this->last_Sys_Mode.initialize(itemConfig);
    if (ret != 0)
    {
      break;
    }

    return 0;
  } while (0);
  this->clear();
  return -1;
}

uint8_t SystemSettingCollection::lastSysMode(void)
{
  SystemMutexLocker locker(this->mutex_Key);
  return this->last_Sys_Mode.get();
}

void SystemSettingCollection::lastSysMode(uint8_t newVal, bool updateDB)
{
  SystemMutexLocker locker(this->mutex_Key);
  this->last_Sys_Mode.set(newVal, updateDB);
}

#endif // _CONF_SYSTEM_SETTING_COLLECTION_ENABLED