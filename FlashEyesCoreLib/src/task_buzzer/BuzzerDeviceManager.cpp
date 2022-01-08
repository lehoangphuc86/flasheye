#include "BuzzerDeviceManager.h"
#ifdef _CONF_BUZZER_DEVICE_MANAGER_ENABLED
/////////////////////////////////////////////////
// INCLUDE

/////////////////////////////////////////////////
// PREPROCESSOR

/////////////////////////////////////////////////
// DEFINE
#define BUZZER_DEVICE_MANAGER_CONSOLE_DEBUG_ENABLE
/////////////////////////////////////////////////
// MARCO

/////////////////////////////////////////////////
// GLOBAL VARIABLES
#ifdef BUZZER_DEVICE_MANAGER_CONSOLE_DEBUG_ENABLE
char buzzerDeviceManagerLogBuf[SYSTEM_CONSOLE_OUT_BUF_LEN];
#endif // BUZZER_DEVICE_MANAGER_CONSOLE_DEBUG_ENABLE
/////////////////////////////////////////////////
// GLOBAL FUNCTIONS

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
// CLASS IMPLEMENTAION

/*BuzzerDeviceManager*/

BuzzerDeviceManager& BuzzerDeviceManager::getInstance(void)
{
  static BuzzerDeviceManager instance;
  return instance;
}

BuzzerDeviceManager::BuzzerDeviceManager(void)
  : device_Count(0)
  , device_Max_Count(0)
  , device_List(NULL)
{
  
}

BuzzerDeviceManager::~BuzzerDeviceManager(void)
{
  this->clear();
}

bool BuzzerDeviceManager::isValid(void)
{
  return (this->device_List == NULL ? false : true);
}

int BuzzerDeviceManager::initialize(BuzzerDeviceManagerConfigTAG& deviceManagerConfig)
{
  do
  {
    if (this->isValid() != false)
    {
      return 0; // already
    }

    if ( (deviceManagerConfig.deviceMaxCount <= 0)
      || (deviceManagerConfig.deviceMaxCount > BUZZER_DEVICE_MANAGER_MAX_DEVICE)
      )
    {
      break;
    }

    this->device_Count = 0;
    this->device_Max_Count = deviceManagerConfig.deviceMaxCount;

    this->device_List = new BuzzerDeviceInfo[this->device_Max_Count + 1];
    if (this->device_List == NULL)
    {
      break;
    }

    return 0;
  } while (0);
  this->clear();
  return -1;
}

int BuzzerDeviceManager::appendDevices(BuzzerDeviceConfigTAG* deviceConfig, byte deviceCount)
{
  int ret = 0;
  byte wk_idx = 0;
  do
  {
    if ((deviceConfig == NULL)
      || (deviceCount <= 0)
      )
    {
      break;
    }

    for (wk_idx = 0; wk_idx < deviceCount; wk_idx++)
    {
      ret = this->appendDevice(deviceConfig[wk_idx]);
      if (ret != 0)
      {
        break;
      }
    }

    if (wk_idx < deviceCount)
    {
      break;
    }

    return 0;
  } while (0);
  return -1;
}

int BuzzerDeviceManager::appendDevice(BuzzerDeviceConfigTAG& deviceConfig)
{
  int ret = 0;
  BuzzerDeviceInfo* nextDevice = NULL;
  do
  {
#ifdef BUZZER_DEVICE_MANAGER_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(buzzerDeviceManagerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[bzzDMgr] ap %i", 0);
#endif // BUZZER_DEVICE_MANAGER_CONSOLE_DEBUG_ENABLE

    if (this->device_Count >= this->device_Max_Count)
    {
#ifdef BUZZER_DEVICE_MANAGER_CONSOLE_DEBUG_ENABLE
      CONSOLE_LOG_BUF(buzzerDeviceManagerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[bzzDMgr] ap %i", -1);
#endif // BUZZER_DEVICE_MANAGER_CONSOLE_DEBUG_ENABLE
      break;
    }

    nextDevice = &this->device_List[this->device_Count];
    nextDevice->clear();
    ret = nextDevice->initialize(deviceConfig);
#ifdef BUZZER_DEVICE_MANAGER_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(buzzerDeviceManagerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[bzzDMgr] ap %i %i", 2, ret);
#endif // BUZZER_DEVICE_MANAGER_CONSOLE_DEBUG_ENABLE
    if (ret != 0)
    {
      break;
    }
#ifdef BUZZER_DEVICE_MANAGER_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(buzzerDeviceManagerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[bzzDMgr] ap %i", 99);
#endif // BUZZER_DEVICE_MANAGER_CONSOLE_DEBUG_ENABLE
    this->device_Count++;
    return 0;
  } while (0);
#ifdef BUZZER_DEVICE_MANAGER_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(buzzerDeviceManagerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[bzzDMgr] ap %i", -99);
#endif // BUZZER_DEVICE_MANAGER_CONSOLE_DEBUG_ENABLE
  if (nextDevice != NULL)
  {
    nextDevice->clear();
  }
  return -1;
}


byte BuzzerDeviceManager::deviceCount(void)
{
  return this->device_Count;
}


BuzzerDeviceInfo* BuzzerDeviceManager::deviceByDeviceId(BuzzerDeviceId_t deviceId)
{
  BuzzerDeviceInfo* deviceInfo = NULL;
  byte wk_idx = 0;
  do
  {
    if ( (deviceId < BUZZER_DEVICE_ID_MIN)
      || (deviceId > BUZZER_DEVICE_ID_MAX)
      )
    {
      break;
    }

    for (wk_idx = 0; wk_idx < this->device_Count; wk_idx++)
    {
      if (this->device_List[wk_idx].deviceId() == deviceId)
      {
        return &this->device_List[wk_idx];
      }
    }

    if (wk_idx >= this->device_Count)
    {
      break;
    }

    return deviceInfo;
  } while (0);
  return NULL;
}

BuzzerDeviceInfo* BuzzerDeviceManager::deviceByIndex(byte deviceIndex)
{
  do
  {
    if ((deviceIndex < 0)
      || (deviceIndex >= this->device_Count)
      )
    {
      break;
    }

    return &this->device_List[deviceIndex];
  } while (0);
  return NULL;
}

void BuzzerDeviceManager::clear(void)
{
  if (this->device_List != NULL)
  {
    delete[] this->device_List;
  }
  this->device_List = NULL;
  this->device_Count = 0;
  this->device_Max_Count = 0;
}

void BuzzerDeviceManager::dump(void)
{
#ifdef BUZZER_DEVICE_MANAGER_CONSOLE_DEBUG_ENABLE
  do
  {
    if (this->isValid() == false)
    {
      CONSOLE_LOG_BUF(buzzerDeviceManagerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[bzzDMgr] %s", "Invalid");
      break;
    }

    for (byte wk_idx = 0; wk_idx < this->device_Count; wk_idx++)
    {
      CONSOLE_LOG_BUF(buzzerDeviceManagerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "dev%u:", wk_idx);
      if (this->device_List[wk_idx].isValid() == false)
      {
        CONSOLE_LOG_BUF(buzzerDeviceManagerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "%s", "  invalid");
        continue;
      }

      this->device_List[wk_idx].dump();
    }

    return;
  } while (0);
  return;
#endif // BUZZER_DEVICE_MANAGER_CONSOLE_DEBUG_ENABLE
}

#endif // _CONF_BUZZER_DEVICE_MANAGER_ENABLED
