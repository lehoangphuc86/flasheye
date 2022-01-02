#include "SettingCollection.h"
#if (_CONF_SETTING_COLLECTION_ENABLED)
/////////////////////////////////////////////////
// INCLUDE
#include "SettingItem.h"
/////////////////////////////////////////////////
// PREPROCESSOR

/////////////////////////////////////////////////
// DEFINE
#define SETTING_COLLECTION_CONSOLE_DEBUG_ENABLE

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
#ifdef SETTING_COLLECTION_CONSOLE_DEBUG_ENABLE
char settingCollectionLogBuf[SYSTEM_CONSOLE_OUT_BUF_LEN];
#endif // SETTING_COLLECTION_CONSOLE_DEBUG_ENABLE
/////////////////////////////////////////////////
// STATIC FUNCTIONS

/////////////////////////////////////////////////
// EXTERN

/////////////////////////////////////////////////
// CLASS IMPLEMENTAION
/*SettingCollection*/
SettingCollection::SettingCollection(void)
  : db_Table_Id(FEM_DB_TBL_ID_INVALID)
  , item_List(0)
  , item_Count(0)
{

}

SettingCollection::~SettingCollection(void)
{
  this->clear();
}

int SettingCollection::initialize(SettingCollectionConfigTAG& settingConfig)
{
  do
  {
    if (settingConfig.dbTableId >= FEM_DB_TBL_ID_INVALID)
    {
      break;
    }

    this->db_Table_Id = settingConfig.dbTableId;
    return 0;
  } while (0);
  this->clear();
  return -1;
}

bool SettingCollection::isValid(void)
{
  do
  {
    if ( (this->item_Count <= 0)
      || (this->item_List == NULL)
      )
    {
      break;
    }

    return true;
  } while (0);
  return false;
}

int SettingCollection::load(void)
{
  int ret = 0;
  do
  {
    SystemMutexLocker locker(this->mutex_Key);
    if (this->isValid() == false)
    {
      break;
    }

    for (int wk_idx = 0; wk_idx < this->item_Count; wk_idx++)
    {
      if (this->item_List[wk_idx] == NULL)
      {
        continue;
      }

      this->item_List[wk_idx]->loadDB();
    }

    return 0;
  } while (0);
  return -1;
}

int SettingCollection::save(void)
{
  int ret = 0;
  do
  {
    SystemMutexLocker locker(this->mutex_Key);
    if (this->isValid() == false)
    {
      break;
    }

    for (int wk_idx = 0; wk_idx < this->item_Count; wk_idx++)
    {
      if (this->item_List[wk_idx] == NULL)
      {
        continue;
      }

      this->item_List[wk_idx]->saveDB();
    }
    
    return 0;
  } while (0);
  return -1;
}

void SettingCollection::clear(void)
{
  {
    SystemMutexLocker locker(this->mutex_Key);
    if (this->item_List != NULL)
    {
      for (int wk_idx = 0; wk_idx < this->item_Count; wk_idx++)
      {
        if (this->item_List == NULL)
        {
          continue;
        }

        this->item_List[wk_idx]->clear();
        this->item_List[wk_idx] = NULL; // not delete.
      }
    }
  }
  
  this->clearItemList();
  this->db_Table_Id = FEM_DB_TBL_ID_INVALID;
}

int SettingCollection::initItemList(byte itemCount)
{
  do
  {
    this->clearItemList();
    if (itemCount <= 0)
    {
      break;
    }

    this->item_Count = itemCount;
    this->item_List = new SettingItem * [this->item_Count + 1];
    return 0;
  } while (0);
  this->clearItemList();
  return -1;
}

void SettingCollection::clearItemList(void)
{
  if (this->item_List != NULL)
  {
    delete[] this->item_List;
  }
  this->item_Count = 0;
  this->item_List = NULL;
}

#endif // _CONF_SETTING_COLLECTION_ENABLED