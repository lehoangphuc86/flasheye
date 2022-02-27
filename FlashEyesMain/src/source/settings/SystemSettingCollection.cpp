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

    ret = SettingCollection::initItemList(7);
    if (ret != 0)
    {
      break;
    }

    this->item_List[0] = &this->last_Sys_Mode;
    this->item_List[1] = &this->wifi_Mode;
    this->item_List[2] = &this->wifi_SSID;
    this->item_List[3] = &this->wifi_Pwd;
    this->item_List[4] = &this->http_Cli_Uri;
    this->item_List[5] = &this->http_Cli_Data_Type;
    this->item_List[6] = &this->http_Cli_Method;
    this->item_List[7] = &this->export_Mode;

    SettingItemConfigTAG itemConfig = SettingItemConfigTAG();
    itemConfig.dbTableId = this->db_Table_Id;

    // last sys mode
    itemConfig.dataType = SYS_DATA_T_8;
    itemConfig.dbKeyId = FEM_SET_DB_KEY_ID_SYS_LAST_SYS_MODE;
    ret = this->last_Sys_Mode.initialize(itemConfig);
    if (ret != 0)
    {
      break;
    }

    // wifi_Mode
    itemConfig.dataType = SYS_DATA_T_8;
    itemConfig.dbKeyId = FEM_SET_DB_KEY_ID_SYS_WIFI_MODE;
    ret = this->wifi_Mode.initialize(itemConfig);
#ifdef SYSTEM_SETTING_COLLECTION_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(systemSettingCollectionLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[ssc] ini %i %i", 10, ret);
#endif // SYSTEM_SETTING_COLLECTION_CONSOLE_DEBUG_ENABLE
    if (ret != 0)
    {
      break;
    }

    // wifi_SSID
    itemConfig.dataType = SYS_DATA_T_STRING;
    itemConfig.dbKeyId = FEM_SET_DB_KEY_ID_SYS_WIFI_SSID;
    itemConfig.sMaxLen = FEM_SET_DB_STR_LEN_SYS_WIFI_SSID;
    ret = this->wifi_SSID.initialize(itemConfig);
#ifdef SYSTEM_SETTING_COLLECTION_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(systemSettingCollectionLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[ssc] ini %i %i", 11, ret);
#endif // SYSTEM_SETTING_COLLECTION_CONSOLE_DEBUG_ENABLE
    if (ret != 0)
    {
      break;
    }

    // wifi_Pwd
    itemConfig.dataType = SYS_DATA_T_STRING;
    itemConfig.dbKeyId = FEM_SET_DB_KEY_ID_SYS_WIFI_PWD;
    itemConfig.sMaxLen = FEM_SET_DB_STR_LEN_SYS_WIFI_PWD;
    ret = this->wifi_Pwd.initialize(itemConfig);
#ifdef SYSTEM_SETTING_COLLECTION_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(systemSettingCollectionLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[ssc] ini %i %i", 12, ret);
#endif // SYSTEM_SETTING_COLLECTION_CONSOLE_DEBUG_ENABLE
    if (ret != 0)
    {
      break;
    }

    // http_Cli_Uri
    itemConfig.dataType = SYS_DATA_T_STRING;
    itemConfig.dbKeyId = FEM_SET_DB_KEY_ID_SYS_HTTP_CLI_URI;
    itemConfig.sMaxLen = FEM_SET_DB_STR_LEN_HTTP_CLI_URI;
    ret = this->http_Cli_Uri.initialize(itemConfig);
#ifdef SYSTEM_SETTING_COLLECTION_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(systemSettingCollectionLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[ssc] ini %i %i", 14, ret);
#endif // SYSTEM_SETTING_COLLECTION_CONSOLE_DEBUG_ENABLE
    if (ret != 0)
    {
      break;
    }

    // http_Cli_Data_Type
    itemConfig.dataType = SYS_DATA_T_8;
    itemConfig.dbKeyId = FEM_SET_DB_KEY_ID_SYS_HTTP_CLI_DATA_TYPE;
    ret = this->http_Cli_Data_Type.initialize(itemConfig);
#ifdef SYSTEM_SETTING_COLLECTION_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(systemSettingCollectionLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[ssc] ini %i %i", 15, ret);
#endif // SYSTEM_SETTING_COLLECTION_CONSOLE_DEBUG_ENABLE
    if (ret != 0)
    {
      break;
    }

    // http_Cli_Method
    itemConfig.dataType = SYS_DATA_T_8;
    itemConfig.dbKeyId = FEM_SET_DB_KEY_ID_SYS_HTTP_CLI_METHOD;
    ret = this->http_Cli_Method.initialize(itemConfig);
#ifdef SYSTEM_SETTING_COLLECTION_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(systemSettingCollectionLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[ssc] ini %i %i", 16, ret);
#endif // SYSTEM_SETTING_COLLECTION_CONSOLE_DEBUG_ENABLE
    if (ret != 0)
    {
      break;
    }

    //export_Mode
    itemConfig.dataType = SYS_DATA_T_8;
    itemConfig.dbKeyId = FEM_SET_DB_KEY_ID_SYS_EXPORT_MODE;
    ret = this->export_Mode.initialize(itemConfig);
#ifdef SYSTEM_SETTING_COLLECTION_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(systemSettingCollectionLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[ssc] ini %i %i", 13, ret);
#endif // SYSTEM_SETTING_COLLECTION_CONSOLE_DEBUG_ENABLE
    if (ret != 0)
    {
      break;
    }

    return 0;
  } while (0);
  this->clear();
  return -1;
}

int SystemSettingCollection::set(SettingParamTAG& settingParam)
{
  SystemMutexLocker locker(this->mutex_Key);
  switch (settingParam.settingId)
  {
    case FEM_SET_DB_KEY_ID_SYS_LAST_SYS_MODE:
    {
      return this->last_Sys_Mode.set(settingParam);
      break;
    }
    case FEM_SET_DB_KEY_ID_SYS_WIFI_MODE:
    {
      return this->wifi_Mode.set(settingParam);
      break;
    }
    case FEM_SET_DB_KEY_ID_SYS_WIFI_SSID:
    {
      return this->wifi_SSID.set(settingParam);
      break;
    }
    case FEM_SET_DB_KEY_ID_SYS_WIFI_PWD:
    {
      return this->wifi_Pwd.set(settingParam);
      break;
    }
    case FEM_SET_DB_KEY_ID_SYS_HTTP_CLI_URI:
    {
      return this->http_Cli_Uri.set(settingParam);
      break;
    }
    case FEM_SET_DB_KEY_ID_SYS_HTTP_CLI_DATA_TYPE:
    {
      return this->http_Cli_Data_Type.set(settingParam);
      break;
    }
    case FEM_SET_DB_KEY_ID_SYS_HTTP_CLI_METHOD:
    {
      return this->http_Cli_Method.set(settingParam);
      break;
    }
    case FEM_SET_DB_KEY_ID_SYS_EXPORT_MODE:
    {
      return this->export_Mode.set(settingParam);
      break;
    }
    default:
      return -1;
  }
}

int SystemSettingCollection::get(SettingParamTAG& settingParam)
{
  SystemMutexLocker locker(this->mutex_Key);
  switch (settingParam.settingId)
  {
    case FEM_SET_DB_KEY_ID_SYS_LAST_SYS_MODE:
    {
      return this->last_Sys_Mode.get(settingParam);
      break;
    }
    case FEM_SET_DB_KEY_ID_SYS_WIFI_MODE:
    {
      return this->wifi_Mode.get(settingParam);
      break;
    }
    case FEM_SET_DB_KEY_ID_SYS_WIFI_SSID:
    {
      return this->wifi_SSID.get(settingParam);
      break;
    }
    case FEM_SET_DB_KEY_ID_SYS_WIFI_PWD:
    {
      return this->wifi_Pwd.get(settingParam);
      break;
    }
    case FEM_SET_DB_KEY_ID_SYS_HTTP_CLI_URI:
    {
      return this->http_Cli_Uri.get(settingParam);
      break;
    }
    case FEM_SET_DB_KEY_ID_SYS_HTTP_CLI_DATA_TYPE:
    {
      return this->http_Cli_Data_Type.get(settingParam);
      break;
    }
    case FEM_SET_DB_KEY_ID_SYS_HTTP_CLI_METHOD:
    {
      return this->http_Cli_Method.get(settingParam);
      break;
    }
    case FEM_SET_DB_KEY_ID_SYS_EXPORT_MODE:
    {
      return this->export_Mode.get(settingParam);
      break;
    }
    default:
      return -1;
  }
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

uint8_t SystemSettingCollection::wifiMode(void)
{
  SystemMutexLocker locker(this->mutex_Key);
  return this->wifi_Mode.get();
}

void SystemSettingCollection::wifiMode(uint8_t newVal, bool updateDB)
{
  SystemMutexLocker locker(this->mutex_Key);
  this->wifi_Mode.set(newVal, updateDB);
}

char* SystemSettingCollection::wifiSSID(void)
{
  SystemMutexLocker locker(this->mutex_Key);
  return this->wifi_SSID.get();
}

void SystemSettingCollection::wifiSSID(char* newVal, DataSize_t newValLen, bool updateDB)
{
  SystemMutexLocker locker(this->mutex_Key);
  this->wifi_SSID.set(newVal, newValLen, updateDB);
}

char* SystemSettingCollection::wifiPwd(void)
{
  SystemMutexLocker locker(this->mutex_Key);
  return this->wifi_Pwd.get();
}

void SystemSettingCollection::wifiPwd(char* newVal, DataSize_t newValLen, bool updateDB)
{
  SystemMutexLocker locker(this->mutex_Key);
  this->wifi_Pwd.set(newVal, newValLen, updateDB);
}

char* SystemSettingCollection::httpCliUri(void)
{
  SystemMutexLocker locker(this->mutex_Key);
  return this->http_Cli_Uri.get();
}

void SystemSettingCollection::httpCliUri(char* newVal, DataSize_t newValLen, bool updateDB)
{
  SystemMutexLocker locker(this->mutex_Key);
  this->http_Cli_Uri.set(newVal, newValLen, updateDB);
}

uint8_t SystemSettingCollection::httpCliDataType(void)
{
  SystemMutexLocker locker(this->mutex_Key);
  return this->http_Cli_Data_Type.get();
}

void SystemSettingCollection::httpCliDataType(uint8_t newVal, bool updateDB)
{
  SystemMutexLocker locker(this->mutex_Key);
  this->http_Cli_Data_Type.set(newVal, updateDB);
}

uint8_t SystemSettingCollection::httpCliMethod(void)
{
  SystemMutexLocker locker(this->mutex_Key);
  return this->http_Cli_Method.get();
}

void SystemSettingCollection::httpCliMethod(uint8_t newVal, bool updateDB)
{
  SystemMutexLocker locker(this->mutex_Key);
  this->http_Cli_Method.set(newVal, updateDB);
}

uint8_t SystemSettingCollection::exportMode(void)
{
  SystemMutexLocker locker(this->mutex_Key);
  return this->export_Mode.get();
}

void SystemSettingCollection::exportMode(uint8_t newVal, bool updateDB)
{
  SystemMutexLocker locker(this->mutex_Key);
  this->export_Mode.set(newVal, updateDB);
}

#endif // _CONF_SYSTEM_SETTING_COLLECTION_ENABLED