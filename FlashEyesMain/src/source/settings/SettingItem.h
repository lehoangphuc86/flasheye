#ifndef _SETTING_ITEM_H
#define _SETTING_ITEM_H

#include "SettingManagerConstant.h"
#if (_CONF_SETTING_ITEM_ENABLED)
/////////////////////////////////////////////////
// INCLUDE
#include "../db/DBManager.h"
/////////////////////////////////////////////////
// PREPROCESSOR
//#define SETTING_ITEM_2_CONSOLE_DEBUG_ENABLE
/////////////////////////////////////////////////
// DEFINE

/////////////////////////////////////////////////
// MARCO

/////////////////////////////////////////////////
// GLOBAL VARIABLES
#ifdef SETTING_ITEM_2_CONSOLE_DEBUG_ENABLE
static char settingItem2LogBuf[SYSTEM_CONSOLE_OUT_BUF_LEN];
#endif // SETTING_ITEM_2_CONSOLE_DEBUG_ENABLE
/////////////////////////////////////////////////
// GLOBAL FUNCTIONS

/////////////////////////////////////////////////
// DATA TYPE (TYPEDEF)

/////////////////////////////////////////////////
// DATA TYPE (ENUM)

/////////////////////////////////////////////////
// DATA TYPE (STRUCT)

typedef struct _settingItemValueTag
{
  DataSize_t sLen;
  union
  {
    double dVal;
    char* sVal;
  } data;
} SettingItemValueTAG;

typedef struct _settingItemConfigTag
{
  byte dbKeyId;
  DbTableId_t dbTableId;
  DataSize_t sMaxLen;
  byte localSettingId;
  byte dataType;
  //SettingItemValueTAG initData;
} SettingItemConfigTAG;
/////////////////////////////////////////////////
// STATIC DATA

/////////////////////////////////////////////////
// STATIC FUNCTIONS

/////////////////////////////////////////////////
// EXTERN

/////////////////////////////////////////////////
// CLASS DEFINITION
/*SettingItem*/
class SettingItem
{
public:
  SettingItem(void);
  virtual ~SettingItem(void);
  virtual int                                                   initialize(SettingItemConfigTAG& settingConfig);
  bool                                                          isInDB(void);
  int                                                           dataType(void);
  virtual int                                                   loadDB(void) = 0;
  virtual int                                                   saveDB(void) = 0;
  virtual int                                                   get(SettingParamTAG& settingParam) = 0;
  virtual int                                                   set(SettingParamTAG& settingParam) = 0;
  virtual void                                                  clear(void);
protected:
  byte                                                          db_Key_Id;
  DbTableId_t                                                   db_Table_Id;
  byte                                                          local_Setting_Id; // used for local
  byte                                                          data_Type;
  //SystemCriticalSession                                         cri_Key;
  //SystemMutex                                                   mutex_Key;
};

/*NumSettingItem*/
template <typename T>
class NumSettingItem
  : public SettingItem
{
public:
  NumSettingItem(void);
  virtual ~NumSettingItem(void);
  int                                                           initialize(SettingItemConfigTAG& settingConfig) override;
  int                                                           loadDB(void) override;
  int                                                           saveDB(void) override;
  void                                                          clear(void) override;
  int                                                           get(SettingParamTAG& settingParam) override;
  int                                                           set(SettingParamTAG& settingParam) override;
  T                                                             get(void);
  int                                                           set(T val, bool updateDb = false);
protected:
  T                                                             cur_Val;
};

/*StrSettingItem*/
class StrSettingItem
  : public SettingItem
{
public:
  StrSettingItem(void);
  virtual ~StrSettingItem(void);
  int                                                           initialize(SettingItemConfigTAG& settingConfig) override;
  int                                                           loadDB(void) override;
  int                                                           saveDB(void) override;
  void                                                          clear(void) override;
  int                                                           get(SettingParamTAG& settingParam) override;
  int                                                           set(SettingParamTAG& settingParam) override;
  char*                                                         get(void);
  DataSize_t                                                    getLen(void);
  int                                                           set(char* val, DataSize_t len, bool updateDb = false);
protected:
  char*                                                         cur_Val;
  DataSize_t                                                    max_Len;
  //DataSize_t                                                    cur_Len;
};





/*NumSettingItem*/
template <typename T>
NumSettingItem<T>::NumSettingItem(void)
  : SettingItem()
  , cur_Val(0)
{
  this->data_Type = SYS_DATA_T_DOUBLE;
}
template <typename T>
NumSettingItem<T>::~NumSettingItem(void)
{

}

template <typename T>
int NumSettingItem<T>::initialize(SettingItemConfigTAG& settingConfig)
{
  int ret = 0;
  do
  {
#ifdef SETTING_ITEM_2_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(settingItem2LogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[nSI]: ini %i", 0);
#endif // SETTING_ITEM_2_CONSOLE_DEBUG_ENABLE
    this->clear();
#ifdef SETTING_ITEM_2_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(settingItem2LogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[nSI]: ini %i", 1);
#endif // SETTING_ITEM_2_CONSOLE_DEBUG_ENABLE
    if (SettingItem::initialize(settingConfig) != 0)
    {

#ifdef SETTING_ITEM_2_CONSOLE_DEBUG_ENABLE
      CONSOLE_LOG_BUF(settingItem2LogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[nSI]: ini %i", -1);
#endif // SETTING_ITEM_2_CONSOLE_DEBUG_ENABLE
      break;
    }
#ifdef SETTING_ITEM_2_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(settingItem2LogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[nSI]: ini %i", 2);
#endif // SETTING_ITEM_2_CONSOLE_DEBUG_ENABLE
    this->cur_Val = 0;
    ret = this->loadDB();
#ifdef SETTING_ITEM_2_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(settingItem2LogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[nSI]: ini %i %i", 3, ret);
#endif // SETTING_ITEM_2_CONSOLE_DEBUG_ENABLE
    if (ret != 0)
    {
      break;
    }
#ifdef SETTING_ITEM_2_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(settingItem2LogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[nSI]: ini %i", 99);
#endif // SETTING_ITEM_2_CONSOLE_DEBUG_ENABLE
    return 0;
  } while (0);
#ifdef SETTING_ITEM_2_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(settingItem2LogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[nSI]: ini %i", -98);
#endif // SETTING_ITEM_2_CONSOLE_DEBUG_ENABLE
  this->clear();
#ifdef SETTING_ITEM_2_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(settingItem2LogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[nSI]: ini %i", -99);
#endif // SETTING_ITEM_2_CONSOLE_DEBUG_ENABLE
  return -1;
}

template <typename T>
int NumSettingItem<T>::loadDB(void)
{
  int ret = 0;
  do
  {
    if (this->isInDB() == false)
    {
      return 0; // no need to load
    }
//#ifdef SETTING_ITEM_2_CONSOLE_DEBUG_ENABLE
//    CONSOLE_LOG_BUF(settingItem2LogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[nSI]: lDB %i", 0);
//#endif // SETTING_ITEM_2_CONSOLE_DEBUG_ENABLE
    {
      //SystemCriticalLocker locker(this->cri_Key);
      ret = DBManager::getInstance().selectCellNum(
        this->db_Table_Id
        , FEM_SETTING_ITEM_DB_VAL_COLUMN_NAME
        , FEM_SETTING_ITEM_DB_ID_COLUMN_NAME
        , this->db_Key_Id
        , this->cur_Val);
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
//#ifdef SETTING_ITEM_2_CONSOLE_DEBUG_ENABLE
//    CONSOLE_LOG_BUF(settingItem2LogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[nSI]: lDB %i %i %i", 1, ret, this->cur_Val);
//#endif // SETTING_ITEM_2_CONSOLE_DEBUG_ENABLE
    if (ret != 0)
    {
      break;
    }

    return 0;
  } while (0);
  return -1;
}

template <typename T>
int NumSettingItem<T>::saveDB(void)
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
      ret = DBManager::getInstance().updateCellNum(
        this->db_Table_Id
        , FEM_SETTING_ITEM_DB_VAL_COLUMN_NAME
        , FEM_SETTING_ITEM_DB_ID_COLUMN_NAME
        , this->db_Key_Id
        , this->cur_Val);
    }
#ifdef SETTING_ITEM_2_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(settingItem2LogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[sSI]: sDB %i %i %i", 1, ret, this->cur_Val);
#endif // SETTING_ITEM_2_CONSOLE_DEBUG_ENABLE
    if (ret != 0)
    {
      break;
    }

    return 0;
  } while (0);
  return -1;
}

template <typename T>
void NumSettingItem<T>::clear(void)
{
  //SystemCriticalLocker locker(this->cri_Key);
  this->cur_Val = 0;
  SettingItem::clear();
}

template <typename T>
int NumSettingItem<T>::get(SettingParamTAG& settingParam)
{
  settingParam.errorCode = 0;
  do
  {
    if (settingParam.bitSet1.dataType != this->data_Type)
    {
      break;
    }

    settingParam.data.dVal = this->get();
    return 0;
  } while (0);
  settingParam.errorCode = -1;
  return -1;
}

template <typename T>
int NumSettingItem<T>::set(SettingParamTAG& settingParam)
{
  settingParam.errorCode = 0;
  int ret = 0;
  do
  {
    if (settingParam.bitSet1.dataType != this->data_Type)
    {
      break;
    }

    ret = this->set((T)settingParam.data.dVal, settingParam.bitSet1.isUpdateDB);
    if (ret != 0)
    {
      break;
    }

    return 0;
  } while (0);
  settingParam.errorCode = -1;
  return -1;
}

template <typename T>
T NumSettingItem<T>::get(void)
{
  //SystemCriticalLocker locker(this->cri_Key);
  return this->cur_Val;
}

template <typename T>
int NumSettingItem<T>::set(T val, bool updateDb)
{
  int ret = 0;
  do
  {
    {
      //SystemCriticalLocker locker(this->cri_Key);
      this->cur_Val = val;
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

#endif // _SETTING_ITEM_H