#include "SettingItem.h"
#if (_CONF_SETTING_ITEM_ENABLED)
/////////////////////////////////////////////////
// INCLUDE
#include "../db/DBManager.h"
/////////////////////////////////////////////////
// PREPROCESSOR

/////////////////////////////////////////////////
// DEFINE
//#define SETTING_ITEM_CONSOLE_DEBUG_ENABLE

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
#ifdef SETTING_ITEM_CONSOLE_DEBUG_ENABLE
char settingItemLogBuf[SYSTEM_CONSOLE_OUT_BUF_LEN];
#endif // SETTING_ITEM_CONSOLE_DEBUG_ENABLE
/////////////////////////////////////////////////
// STATIC FUNCTIONS

/////////////////////////////////////////////////
// EXTERN

/////////////////////////////////////////////////
// CLASS IMPLEMENTAION
/*SettingItem*/
SettingItem::SettingItem(void)
  : db_Key_Id(FEM_SETTING_ITEM_DB_ID_INVALID)
  , db_Table_Id(FEM_DB_TBL_ID_INVALID)
  , local_Setting_Id(FEM_SETTING_ITEM_LOCAL_SETTING_ID_INVALID)
  , data_Type(SYS_DATA_T_DOUBLE)
{

}

SettingItem::~SettingItem(void)
{
  this->clear();
}

int SettingItem::initialize(SettingItemConfigTAG& settingConfig)
{
  do
  {
    this->db_Key_Id = settingConfig.dbKeyId;
    this->db_Table_Id = settingConfig.dbTableId;
    this->local_Setting_Id = settingConfig.localSettingId;
    this->data_Type = settingConfig.dataType;
    return 0;
  } while (0);
  this->clear();
  return -1;
}


int SettingItem::dataType(void)
{
  return this->data_Type;
}

bool SettingItem::isInDB(void)
{
  return (this->db_Table_Id == FEM_DB_TBL_ID_INVALID ? false : true);
}

void SettingItem::clear(void)
{
  this->db_Key_Id = FEM_SETTING_ITEM_DB_ID_INVALID;
  this->db_Table_Id = FEM_DB_TBL_ID_INVALID;
}

/*StrSettingItem*/
StrSettingItem::StrSettingItem(void)
  : SettingItem()
  , cur_Val(NULL)
  , max_Len(0)
{
  this->data_Type = SYS_DATA_T_STRING;
}

StrSettingItem::~StrSettingItem(void)
{

}

int StrSettingItem::initialize(SettingItemConfigTAG& settingConfig)
{
  int ret = 0;
  do
  {
    this->clear();
    if (SettingItem::initialize(settingConfig) != 0)
    {
      break;
    }

    if (settingConfig.sMaxLen <=0)
    {
      break;
    }
    this->max_Len = settingConfig.sMaxLen;
    this->cur_Val = new char[this->max_Len + 1];
    this->cur_Val[this->max_Len] = '\0';
    //this->cur_Len = 0;
    ret = this->loadDB();
    if (ret != 0)
    {
      break;
    }

    return 0;
  } while (0);
  this->clear();
  return -1;
}

int StrSettingItem::loadDB(void)
{
  int ret = 0;
  do
  {
    if (this->isInDB() == false)
    {
      return 0; // no need to load
    }

    {
      //SystemCriticalLocker locker(this->cri_Key);
      if (this->cur_Val == NULL)
      {
        break;
      }
      ret = DBManager::getInstance().selectCellStr(
        this->db_Table_Id
        , FEM_SETTING_ITEM_DB_VAL_COLUMN_NAME
        , FEM_SETTING_ITEM_DB_ID_COLUMN_NAME
        , this->db_Key_Id
        , this->cur_Val
        , this->max_Len);
    }

    {
      // check data type
      byte dataType = SYS_DATA_T_STRING;
      ret = DBManager::getInstance().selectCellNum(
        this->db_Table_Id
        , FEM_SETTING_ITEM_DB_DATA_TYPE_COLUMN_NAME
        , FEM_SETTING_ITEM_DB_ID_COLUMN_NAME
        , this->db_Key_Id
        , dataType);
      if (dataType != this->data_Type)
      {
        break;
      }
    }
#ifdef SETTING_ITEM_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(settingItemLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[sSI]: lDB %i %i %s", 1, ret, this->cur_Val);
#endif // SETTING_ITEM_CONSOLE_DEBUG_ENABLE
    if (ret != 0)
    {
      break;
    }

    return 0;
  } while (0);
  return -1;
}

int StrSettingItem::saveDB(void)
{
  int ret = 0;
  do
  {
    if (this->isInDB() == false)
    {
      return 0; // no need to save
    }

    {
      //SystemCriticalLocker locker(this->cri_Key);
      if (this->cur_Val == NULL)
      {
        break;
      }

      ret = DBManager::getInstance().updateCellStr(
        this->db_Table_Id
        , FEM_SETTING_ITEM_DB_VAL_COLUMN_NAME
        , FEM_SETTING_ITEM_DB_ID_COLUMN_NAME
        , this->db_Key_Id
        , this->cur_Val
        , this->max_Len);
    }
#ifdef SETTING_ITEM_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(settingItemLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[sSI]: sDB %i %i %s", 1, ret, this->cur_Val);
#endif // SETTING_ITEM_CONSOLE_DEBUG_ENABLE
    if (ret != 0)
    {
      break;
    }

    return 0;
  } while (0);
  return -1;
}

void StrSettingItem::clear(void)
{
  //SystemCriticalLocker locker(this->cri_Key);
  if (this->cur_Val != NULL)
  {
    delete[] this->cur_Val;
  }
  this->cur_Val = NULL;
  this->max_Len = 0;
  //this->cur_Len = 0;
  SettingItem::clear();
}

char* StrSettingItem::get(void)
{
  //SystemCriticalLocker locker(this->cri_Key);
  return this->cur_Val;
}

int StrSettingItem::get(SettingParamTAG& settingParam)
{
  settingParam.errorCode = 0;
  do
  {
    if (settingParam.bitSet1.dataType != this->data_Type)
    {
      break;
    }

    settingParam.data.sVal = this->get();
    settingParam.sLen = this->getLen();
    return 0;
  } while (0);
  settingParam.errorCode = -1;
  return -1;
}

int StrSettingItem::set(SettingParamTAG& settingParam)
{
  settingParam.errorCode = 0;
  int ret = 0;
  do
  {
    if (settingParam.bitSet1.dataType != this->data_Type)
    {
      break;
    }

    ret = this->set(settingParam.data.sVal, settingParam.sLen, settingParam.bitSet1.isUpdateDB);
    if (ret != 0)
    {
      break;
    }

    return 0;
  } while (0);
  settingParam.errorCode = -1;
  return -1;
}

DataSize_t StrSettingItem::getLen(void)
{
  //SystemCriticalLocker locker(this->cri_Key);
  return (this->cur_Val  == NULL? 0: strlen(this->cur_Val));// this->cur_Len;
}

int StrSettingItem::set(char* val, DataSize_t len, bool updateDb)
{
  int ret = 0;
  do
  {
    {
      //SystemCriticalLocker locker(this->cri_Key);
      if ( (this->cur_Val == NULL)
        || (len > this->max_Len)
        )
      {
        break;
      }

      if ((val != NULL)
        && (len >0)
        )
      {
        memcpy(this->cur_Val, val, len);
      }
    
      this->cur_Val[len] = '\0';
    }

    if (updateDb != false)
    {
      ret = this->saveDB();
    }

    if (ret != 0)
    {
      break;
    }

    return 0;
  } while (0);
  return -1;
}

#endif // _CONF_SETTING_ITEM_ENABLED