#include "SettingManager.h"
/////////////////////////////////////////////////
// INCLUDE
#if (_CONF_SETTING_MANAGER_ENABLED)
/////////////////////////////////////////////////
// PREPROCESSOR

/////////////////////////////////////////////////
// DEFINE
//#define SETTING_MANAGER_CONSOLE_DEBUG_ENABLE

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
#ifdef SETTING_MANAGER_CONSOLE_DEBUG_ENABLE
char settingManagerLogBuf[SYSTEM_CONSOLE_OUT_BUF_LEN];
#endif // SETTING_MANAGER_CONSOLE_DEBUG_ENABLE
/////////////////////////////////////////////////
// STATIC FUNCTIONS

/////////////////////////////////////////////////
// EXTERN

/////////////////////////////////////////////////
// CLASS IMPLEMENTAION
/*SettingManager*/
SettingManager& SettingManager::getInstance(void)
{
  static SettingManager instance;
  return instance;
}

SettingManager::SettingManager(void)
  : setting_List(NULL)
  , setting_List_Count(0)
{

}

SettingManager::~SettingManager(void)
{
  this->clear();
}

int SettingManager::initialize(SettingManagerConfigTAG& settingConfig)
{
#ifdef SETTING_MANAGER_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(settingManagerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[sm] ini %i", 0);
#endif // SETTING_MANAGER_CONSOLE_DEBUG_ENABLE
  int ret = 0;
  do
  {
    if (this->isValid() != false)
    {
      return 0; // already init
    }

#ifdef SETTING_MANAGER_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(settingManagerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[sm] ini %i", 1);
#endif // SETTING_MANAGER_CONSOLE_DEBUG_ENABLE
    ret = this->initSettingList(2); // @@
    if (ret != 0)
    {
      break;
    }
#ifdef SETTING_MANAGER_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(settingManagerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[sm] ini %i", 2);
#endif // SETTING_MANAGER_CONSOLE_DEBUG_ENABLE
    this->setting_List[0] = &this->scanner_Set;
    this->setting_List[1] = &this->system_Set;
    // @@

    SettingCollectionConfigTAG collectionConfig = SettingCollectionConfigTAG();
    // scanner
    collectionConfig.dbTableId = FEM_SET_DB_TBL_ID_SCANNER;
    ret = this->scanner_Set.initialize(collectionConfig);
    if (ret != 0)
    {
      break;
    }

    // system
    collectionConfig.dbTableId = FEM_SET_DB_TBL_ID_SYSTEM;
    ret = this->system_Set.initialize(collectionConfig);
    if (ret != 0)
    {
      break;
    }

#ifdef SETTING_MANAGER_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(settingManagerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[sm] ini %i", 99);
#endif // SETTING_MANAGER_CONSOLE_DEBUG_ENABLE
    return 0;
  } while (0);
#ifdef SETTING_MANAGER_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(settingManagerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[sm] ini %i", -98);
#endif // SETTING_MANAGER_CONSOLE_DEBUG_ENABLE
  this->clear();
#ifdef SETTING_MANAGER_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(settingManagerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[sm] ini %i", -99);
#endif // SETTING_MANAGER_CONSOLE_DEBUG_ENABLE
  return -1;
}

bool SettingManager::isValid(void)
{
  do
  {
    if ((this->setting_List_Count <= 0)
      || (this->setting_List == NULL)
      )
    {
      break;
    }

    return true;
  } while (0);
  return false;
}

int SettingManager::load(void)
{
  int ret = 0;
  do
  {
    if (this->isValid() == false)
    {
      break;
    }

    for (int wk_idx = 0; wk_idx < this->setting_List_Count; wk_idx++)
    {
      if (this->setting_List[wk_idx] == NULL)
      {
        continue;
      }

      this->setting_List[wk_idx]->load();
    }

    return 0;
  } while (0);
  return -1;
}

int SettingManager::save(void)
{
  int ret = 0;
  do
  {
    if (this->isValid() == false)
    {
      break;
    }

    for (int wk_idx = 0; wk_idx < this->setting_List_Count; wk_idx++)
    {
      if (this->setting_List[wk_idx] == NULL)
      {
        continue;
      }

      this->setting_List[wk_idx]->save();
    }

    return 0;
  } while (0);
  return -1;
}

void SettingManager::clear(void)
{
  if (this->setting_List != NULL)
  {
    for (int wk_idx = 0; wk_idx < this->setting_List_Count; wk_idx++)
    {
      if (this->setting_List == NULL)
      {
        continue;
      }

      this->setting_List[wk_idx]->clear();
      this->setting_List[wk_idx] = NULL; // not delete.
    }
  }

  this->clearSettingList();
}

int SettingManager::initSettingList(byte listCount)
{
  do
  {
    this->clearSettingList();
    if (listCount <= 0)
    {
      break;
    }

    this->setting_List_Count = listCount;
    this->setting_List = new SettingCollection * [this->setting_List_Count + 1];
    return 0;
  } while (0);
  this->clearSettingList();
  return -1;
}

void SettingManager::clearSettingList(void)
{
  if (this->setting_List != NULL)
  {
    delete[] this->setting_List;
  }
  this->setting_List_Count = 0;
  this->setting_List = NULL;
}

ScannerSettingCollection& SettingManager::scanner(void)
{
  return this->scanner_Set;
}

SystemSettingCollection& SettingManager::system(void)
{
  return this->system_Set;
}


#endif // _CONF_FILE_SYSTEM_MANAGER_ENABLED