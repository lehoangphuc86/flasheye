#include "ScannerSettingCollection.h"
#if (_CONF_SCANNER_SETTING_COLLECTION_ENABLED)
/////////////////////////////////////////////////
// INCLUDE

/////////////////////////////////////////////////
// PREPROCESSOR

/////////////////////////////////////////////////
// DEFINE
//#define SCANNER_SETTING_COLLECTION_CONSOLE_DEBUG_ENABLE

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

    ret = SettingCollection::initItemList(18);
#ifdef SCANNER_SETTING_COLLECTION_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(scannerSettingCollectionLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[snSC] ini %i %i", 2, ret);
#endif // SCANNER_SETTING_COLLECTION_CONSOLE_DEBUG_ENABLE
    if (ret != 0)
    {
      break;
    }

    this->item_List[0] = &this->comm_Baudrate;
    this->item_List[1] = &this->comm_Parity;
    this->item_List[2] = &this->comm_Stopbit;
    this->item_List[3] = &this->comm_Databit;
    this->item_List[4] = &this->comm_FlowCtrl;
    this->item_List[5] = &this->character_Format;
    this->item_List[6] = &this->read_State;
    this->item_List[7] = &this->auto_Scan;
    this->item_List[8] = &this->cont_Scan;
    this->item_List[9] = &this->security_Level;
    this->item_List[10] = &this->enable_Prefix;
    this->item_List[11] = &this->custom_Prefix;
    this->item_List[12] = &this->custom_Suffix;
    this->item_List[13] = &this->custom_Code_Format;
    this->item_List[14] = &this->mess_Terminator;
    this->item_List[15] = &this->trigger_Mode;
    this->item_List[16] = &this->dist_SS_Range_Begin;
    this->item_List[17] = &this->dist_SS_Range_End;

    SettingItemConfigTAG itemConfig = SettingItemConfigTAG();
    itemConfig.dbTableId = this->db_Table_Id;

    // comm_Baudrate
    itemConfig.dataType = SYS_DATA_T_8;
    itemConfig.dbKeyId = FEM_SET_DB_KEY_ID_SCANNER_COM_BAUDRATE;
    ret = this->comm_Baudrate.initialize(itemConfig);
    if (ret != 0)
    {
      break;
    }

    // comm_Parity
    itemConfig.dataType = SYS_DATA_T_8;
    itemConfig.dbKeyId = FEM_SET_DB_KEY_ID_SCANNER_COM_PARITY;
    ret = this->comm_Parity.initialize(itemConfig);
    if (ret != 0)
    {
      break;
    }

    // comm_Stopbit
    itemConfig.dataType = SYS_DATA_T_8;
    itemConfig.dbKeyId = FEM_SET_DB_KEY_ID_SCANNER_COM_STOPBIT;
    ret = this->comm_Stopbit.initialize(itemConfig);
    if (ret != 0)
    {
      break;
    }

    // comm_Databit
    itemConfig.dataType = SYS_DATA_T_8;
    itemConfig.dbKeyId = FEM_SET_DB_KEY_ID_SCANNER_COM_DATABIT;
    ret = this->comm_Databit.initialize(itemConfig);
    if (ret != 0)
    {
      break;
    }

    // comm_FlowCtrl
    itemConfig.dataType = SYS_DATA_T_8;
    itemConfig.dbKeyId = FEM_SET_DB_KEY_ID_SCANNER_COM_FLOW_CTRL;
    ret = this->comm_FlowCtrl.initialize(itemConfig);
    if (ret != 0)
    {
      break;
    }

    // character_Format
    itemConfig.dataType = SYS_DATA_T_8;
    itemConfig.dbKeyId = FEM_SET_DB_KEY_ID_SCANNER_CHARACTER;
    ret = this->character_Format.initialize(itemConfig);
    if (ret != 0)
    {
      break;
    }

    // read_State
    itemConfig.dataType = SYS_DATA_T_8;
    itemConfig.dbKeyId = FEM_SET_DB_KEY_ID_SCANNER_READ_STATE;
    ret = this->read_State.initialize(itemConfig);
    if (ret != 0)
    {
      break;
    }

    // auto_Scan
    itemConfig.dataType = SYS_DATA_T_8;
    itemConfig.dbKeyId = FEM_SET_DB_KEY_ID_SCANNER_AUTO_SCAN;
    ret = this->auto_Scan.initialize(itemConfig);
    if (ret != 0)
    {
      break;
    }

    // cont_Scan
    itemConfig.dataType = SYS_DATA_T_8;
    itemConfig.dbKeyId = FEM_SET_DB_KEY_ID_SCANNER_CONT_SCAN;
    ret = this->cont_Scan.initialize(itemConfig);
    if (ret != 0)
    {
      break;
    }

    // security_Level
    itemConfig.dataType = SYS_DATA_T_8;
    itemConfig.dbKeyId = FEM_SET_DB_KEY_ID_SCANNER_SECURITY_LEVEL;
    ret = this->security_Level.initialize(itemConfig);
    if (ret != 0)
    {
      break;
    }

    // enable_Prefix
    itemConfig.dataType = SYS_DATA_T_8;
    itemConfig.dbKeyId = FEM_SET_DB_KEY_ID_SCANNER_CODE_PREFIX;
    ret = this->enable_Prefix.initialize(itemConfig);
    if (ret != 0)
    {
      break;
    }
    // custom_Code_Format
    itemConfig.dataType = SYS_DATA_T_8;
    itemConfig.dbKeyId = FEM_SET_DB_KEY_ID_SCANNER_CODE_CUS_FORMAT;
    ret = this->custom_Code_Format.initialize(itemConfig);
    if (ret != 0)
    {
      break;
    }
    // mess_Terminator
    itemConfig.dataType = SYS_DATA_T_8;
    itemConfig.dbKeyId = FEM_SET_DB_KEY_ID_SCANNER_MESS_TER;
    ret = this->mess_Terminator.initialize(itemConfig);
    if (ret != 0)
    {
      break;
    }


    // custom_Prefix
    itemConfig.dataType = SYS_DATA_T_STRING;
    itemConfig.dbKeyId = FEM_SET_DB_KEY_ID_SCANNER_CODE_PREFIX_MOD;
    itemConfig.sMaxLen = FEM_SET_DB_STR_LEN_SCANNER_CODE_PREFIX_MOD;
    ret = this->custom_Prefix.initialize(itemConfig);
    if (ret != 0)
    {
      break;
    }

    // custom_Suffix
    itemConfig.dataType = SYS_DATA_T_STRING;
    itemConfig.dbKeyId = FEM_SET_DB_KEY_ID_SCANNER_CODE_SUFFIX_MOD;
    itemConfig.sMaxLen = FEM_SET_DB_STR_LEN_SCANNER_CODE_SUFFIX_MOD;
    ret = this->custom_Suffix.initialize(itemConfig);
    if (ret != 0)
    {
      break;
    }

    // trigger_Mode
    itemConfig.dataType = SYS_DATA_T_8;
    itemConfig.dbKeyId = FEM_SET_DB_KEY_ID_SCANNER_TRIGGER_MODE;
    ret = this->trigger_Mode.initialize(itemConfig);
#ifdef SCANNER_SETTING_COLLECTION_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(scannerSettingCollectionLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[scc] ini %i %i", 10, ret);
#endif // SCANNER_SETTING_COLLECTION_CONSOLE_DEBUG_ENABLE
    if (ret != 0)
    {
      break;
    }

    // dist_SS_Range_Begin
    itemConfig.dataType = SYS_DATA_T_8;
    itemConfig.dbKeyId = FEM_SET_DB_KEY_ID_DIST_SS_RANGE_BEGIN;
    ret = this->dist_SS_Range_Begin.initialize(itemConfig);
#ifdef SCANNER_SETTING_COLLECTION_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(scannerSettingCollectionLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[scc] ini %i %i", 10, ret);
#endif // SCANNER_SETTING_COLLECTION_CONSOLE_DEBUG_ENABLE
    if (ret != 0)
    {
      break;
    }

    // dist_SS_Range_End
    itemConfig.dataType = SYS_DATA_T_8;
    itemConfig.dbKeyId = FEM_SET_DB_KEY_ID_DIST_SS_RANGE_END;
    ret = this->dist_SS_Range_End.initialize(itemConfig);
#ifdef SCANNER_SETTING_COLLECTION_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(scannerSettingCollectionLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[scc] ini %i %i", 10, ret);
#endif // SCANNER_SETTING_COLLECTION_CONSOLE_DEBUG_ENABLE
    if (ret != 0)
    {
      break;
    }
#ifdef SCANNER_SETTING_COLLECTION_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(scannerSettingCollectionLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[snSC] ini %i", 99);
#endif // SCANNER_SETTING_COLLECTION_CONSOLE_DEBUG_ENABLE
    return 0;
  } while (0);
  this->clear();
#ifdef SCANNER_SETTING_COLLECTION_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(scannerSettingCollectionLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[snSC] ini %i", -99);
#endif // SCANNER_SETTING_COLLECTION_CONSOLE_DEBUG_ENABLE
  return -1;
}


int ScannerSettingCollection::get(SettingParamTAG& settingParam)
{
  SystemMutexLocker locker(this->mutex_Key);
  switch (settingParam.settingId)
  {
    case FEM_SET_DB_KEY_ID_SCANNER_COM_BAUDRATE:
    {
      return this->comm_Baudrate.get(settingParam);
      break;
    }
    case FEM_SET_DB_KEY_ID_SCANNER_COM_PARITY:
    {
      return this->comm_Parity.get(settingParam);
      break;
    }
    case FEM_SET_DB_KEY_ID_SCANNER_COM_STOPBIT:
    {
      return this->comm_Stopbit.get(settingParam);
      break;
    }
    case FEM_SET_DB_KEY_ID_SCANNER_COM_DATABIT:
    {
      return this->comm_Databit.get(settingParam);
      break;
    }
    case FEM_SET_DB_KEY_ID_SCANNER_COM_FLOW_CTRL:
    {
      return this->comm_FlowCtrl.get(settingParam);
      break;
    }
    case FEM_SET_DB_KEY_ID_SCANNER_CHARACTER:
    {
      return this->character_Format.get(settingParam);
      break;
    }
    case FEM_SET_DB_KEY_ID_SCANNER_READ_STATE:
    {
      return this->read_State.get(settingParam);
      break;
    }
    case FEM_SET_DB_KEY_ID_SCANNER_AUTO_SCAN:
    {
      return this->auto_Scan.get(settingParam);
      break;
    }
    case FEM_SET_DB_KEY_ID_SCANNER_CONT_SCAN:
    {
      return this->cont_Scan.get(settingParam);
      break;
    }
    case FEM_SET_DB_KEY_ID_SCANNER_SECURITY_LEVEL:
    {
      return this->security_Level.get(settingParam);
      break;
    }
    case FEM_SET_DB_KEY_ID_SCANNER_CODE_PREFIX:
    {
      return this->enable_Prefix.get(settingParam);
      break;
    }
    case FEM_SET_DB_KEY_ID_SCANNER_CODE_PREFIX_MOD:
    {
      return this->custom_Prefix.get(settingParam);
      break;
    }
    case FEM_SET_DB_KEY_ID_SCANNER_CODE_SUFFIX_MOD:
    {
      return this->custom_Suffix.get(settingParam);
      break;
    }
    case FEM_SET_DB_KEY_ID_SCANNER_CODE_CUS_FORMAT:
    {
      return this->custom_Code_Format.get(settingParam);
      break;
    }
    case FEM_SET_DB_KEY_ID_SCANNER_MESS_TER:
    {
      return this->mess_Terminator.get(settingParam);
      break;
    }
    case FEM_SET_DB_KEY_ID_SCANNER_TRIGGER_MODE:
    {
      return this->trigger_Mode.get(settingParam);
      break;
    }
    case FEM_SET_DB_KEY_ID_DIST_SS_RANGE_BEGIN:
    {
      return this->dist_SS_Range_Begin.get(settingParam);
      break;
    }
    case FEM_SET_DB_KEY_ID_DIST_SS_RANGE_END:
    {
      return this->dist_SS_Range_End.get(settingParam);
      break;
    }
    default:
      return -1;
  }
}

int ScannerSettingCollection::set(SettingParamTAG& settingParam)
{
  SystemMutexLocker locker(this->mutex_Key);
  switch (settingParam.settingId)
  {
    case FEM_SET_DB_KEY_ID_SCANNER_COM_BAUDRATE:
    {
      return this->comm_Baudrate.set(settingParam);
      break;
    }
    case FEM_SET_DB_KEY_ID_SCANNER_COM_PARITY:
    {
      return this->comm_Parity.set(settingParam);
      break;
    }
    case FEM_SET_DB_KEY_ID_SCANNER_COM_STOPBIT:
    {
      return this->comm_Stopbit.set(settingParam);
      break;
    }
    case FEM_SET_DB_KEY_ID_SCANNER_COM_DATABIT:
    {
      return this->comm_Databit.set(settingParam);
      break;
    }
    case FEM_SET_DB_KEY_ID_SCANNER_COM_FLOW_CTRL:
    {
      return this->comm_FlowCtrl.set(settingParam);
      break;
    }
    case FEM_SET_DB_KEY_ID_SCANNER_CHARACTER:
    {
      return this->character_Format.set(settingParam);
      break;
    }
    case FEM_SET_DB_KEY_ID_SCANNER_READ_STATE:
    {
      return this->read_State.set(settingParam);
      break;
    }
    case FEM_SET_DB_KEY_ID_SCANNER_AUTO_SCAN:
    {
      return this->auto_Scan.set(settingParam);
      break;
    }
    case FEM_SET_DB_KEY_ID_SCANNER_CONT_SCAN:
    {
      return this->cont_Scan.set(settingParam);
      break;
    }
    case FEM_SET_DB_KEY_ID_SCANNER_SECURITY_LEVEL:
    {
      return this->security_Level.set(settingParam);
      break;
    }
    case FEM_SET_DB_KEY_ID_SCANNER_CODE_PREFIX:
    {
      return this->enable_Prefix.set(settingParam);
      break;
    }
    case FEM_SET_DB_KEY_ID_SCANNER_CODE_PREFIX_MOD:
    {
      return this->custom_Prefix.set(settingParam);
      break;
    }
    case FEM_SET_DB_KEY_ID_SCANNER_CODE_SUFFIX_MOD:
    {
      return this->custom_Suffix.set(settingParam);
      break;
    }
    case FEM_SET_DB_KEY_ID_SCANNER_CODE_CUS_FORMAT:
    {
      return this->custom_Code_Format.set(settingParam);
      break;
    }
    case FEM_SET_DB_KEY_ID_SCANNER_MESS_TER:
    {
      return this->mess_Terminator.set(settingParam);
      break;
    }
    case FEM_SET_DB_KEY_ID_SCANNER_TRIGGER_MODE:
    {
      return this->trigger_Mode.set(settingParam);
      break;
    }
    case FEM_SET_DB_KEY_ID_DIST_SS_RANGE_BEGIN:
    {
      return this->dist_SS_Range_Begin.set(settingParam);
      break;
    }
    case FEM_SET_DB_KEY_ID_DIST_SS_RANGE_END:
    {
      return this->dist_SS_Range_End.set(settingParam);
      break;
    }
    default:
      return -1;
  }
}

uint8_t ScannerSettingCollection::commBaudrate(void)
{
  SystemMutexLocker locker(this->mutex_Key);
  return this->comm_Baudrate.get();
}

void ScannerSettingCollection::commBaudrate(uint8_t newVal, bool updateDB)
{
  SystemMutexLocker locker(this->mutex_Key);
  this->comm_Baudrate.set(newVal, updateDB);
}

uint8_t ScannerSettingCollection::commParity(void)
{
  SystemMutexLocker locker(this->mutex_Key);
  return this->comm_Parity.get();
}

void ScannerSettingCollection::commParity(uint8_t newVal, bool updateDB)
{
  SystemMutexLocker locker(this->mutex_Key);
  this->comm_Parity.set(newVal, updateDB);
}

uint8_t ScannerSettingCollection::commStopbit(void)
{
  SystemMutexLocker locker(this->mutex_Key);
  return this->comm_Stopbit.get();
}

void ScannerSettingCollection::commStopbit(uint8_t newVal, bool updateDB)
{
  SystemMutexLocker locker(this->mutex_Key);
  this->comm_Stopbit.set(newVal, updateDB);
}

uint8_t ScannerSettingCollection::commDatabit(void)
{
  SystemMutexLocker locker(this->mutex_Key);
  return this->comm_Databit.get();
}

void ScannerSettingCollection::commDatabit(uint8_t newVal, bool updateDB)
{
  SystemMutexLocker locker(this->mutex_Key);
  this->comm_Databit.set(newVal, updateDB);
}

uint8_t ScannerSettingCollection::commFlowCtrl(void)
{
  SystemMutexLocker locker(this->mutex_Key);
  return this->comm_FlowCtrl.get();
}

void ScannerSettingCollection::commFlowCtrl(uint8_t newVal, bool updateDB)
{
  SystemMutexLocker locker(this->mutex_Key);
  this->comm_FlowCtrl.set(newVal, updateDB);
}

uint8_t ScannerSettingCollection::characterFormat(void)
{
  SystemMutexLocker locker(this->mutex_Key);
  return this->character_Format.get();
}

void ScannerSettingCollection::characterFormat(uint8_t newVal, bool updateDB)
{
  SystemMutexLocker locker(this->mutex_Key);
  this->character_Format.set(newVal, updateDB);
}

uint8_t ScannerSettingCollection::readState(void)
{
  SystemMutexLocker locker(this->mutex_Key);
  return this->read_State.get();
}

void ScannerSettingCollection::readState(uint8_t newVal, bool updateDB)
{
  SystemMutexLocker locker(this->mutex_Key);
  this->read_State.set(newVal, updateDB);
}

uint8_t ScannerSettingCollection::autoScan(void)
{
  SystemMutexLocker locker(this->mutex_Key);
  return this->auto_Scan.get();
}

void ScannerSettingCollection::autoScan(uint8_t newVal, bool updateDB)
{
  SystemMutexLocker locker(this->mutex_Key);
  this->auto_Scan.set(newVal, updateDB);
}

uint8_t ScannerSettingCollection::contScan(void)
{
  SystemMutexLocker locker(this->mutex_Key);
  return this->cont_Scan.get();
}

void ScannerSettingCollection::contScan(uint8_t newVal, bool updateDB)
{
  SystemMutexLocker locker(this->mutex_Key);
  this->cont_Scan.set(newVal, updateDB);
}

uint8_t ScannerSettingCollection::securityLevel(void)
{
  SystemMutexLocker locker(this->mutex_Key);
  return this->security_Level.get();
}

void ScannerSettingCollection::securityLevel(uint8_t newVal, bool updateDB)
{
  SystemMutexLocker locker(this->mutex_Key);
  this->security_Level.set(newVal, updateDB);
}

uint8_t ScannerSettingCollection::enablePrefix(void)
{
  SystemMutexLocker locker(this->mutex_Key);
  return this->enable_Prefix.get();
}

void ScannerSettingCollection::enablePrefix(uint8_t newVal, bool updateDB)
{
  SystemMutexLocker locker(this->mutex_Key);
  this->enable_Prefix.set(newVal, updateDB);
}

uint8_t ScannerSettingCollection::customCodeFormat(void)
{
  SystemMutexLocker locker(this->mutex_Key);
  return this->custom_Code_Format.get();
}

void ScannerSettingCollection::customCodeFormat(uint8_t newVal, bool updateDB)
{
  SystemMutexLocker locker(this->mutex_Key);
  this->custom_Code_Format.set(newVal, updateDB);
}

uint8_t ScannerSettingCollection::messTerminator(void)
{
  SystemMutexLocker locker(this->mutex_Key);
  return this->mess_Terminator.get();
}

void ScannerSettingCollection::messTerminator(uint8_t newVal, bool updateDB)
{
  SystemMutexLocker locker(this->mutex_Key);
  this->mess_Terminator.set(newVal, updateDB);
}

char* ScannerSettingCollection::customPrefix(void)
{
  SystemMutexLocker locker(this->mutex_Key);
  return this->custom_Prefix.get();
}

void ScannerSettingCollection::customPrefix(char* newVal, DataSize_t newValLen, bool updateDB)
{
  SystemMutexLocker locker(this->mutex_Key);
  this->custom_Prefix.set(newVal, newValLen, updateDB);
}

char* ScannerSettingCollection::customSuffix(void)
{
  SystemMutexLocker locker(this->mutex_Key);
  return this->custom_Suffix.get();
}

void ScannerSettingCollection::customSuffix(char* newVal, DataSize_t newValLen, bool updateDB)
{
  SystemMutexLocker locker(this->mutex_Key);
  this->custom_Suffix.set(newVal, newValLen, updateDB);
}

uint8_t ScannerSettingCollection::triggerMode(void)
{
  SystemMutexLocker locker(this->mutex_Key);
  return this->trigger_Mode.get();
}

void ScannerSettingCollection::triggerMode(uint8_t newVal, bool updateDB)
{
  SystemMutexLocker locker(this->mutex_Key);
  this->trigger_Mode.set(newVal, updateDB);
}

uint8_t ScannerSettingCollection::distSSRangeBegin(void)
{
  SystemMutexLocker locker(this->mutex_Key);
  return this->dist_SS_Range_Begin.get();
}

void ScannerSettingCollection::distSSRangeBegin(uint8_t newVal, bool updateDB)
{
  SystemMutexLocker locker(this->mutex_Key);
  this->dist_SS_Range_Begin.set(newVal, updateDB);
}

uint8_t ScannerSettingCollection::distSSRangeEnd(void)
{
  SystemMutexLocker locker(this->mutex_Key);
  return this->dist_SS_Range_End.get();
}

void ScannerSettingCollection::distSSRangeEnd(uint8_t newVal, bool updateDB)
{
  SystemMutexLocker locker(this->mutex_Key);
  this->dist_SS_Range_End.set(newVal, updateDB);
}
#endif // _CONF_SCANNER_SETTING_COLLECTION_ENABLED