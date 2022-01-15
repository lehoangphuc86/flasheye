#include "LedDeviceManager.h"
#ifdef _CONF_LED_DEVICE_MANAGER_ENABLED
/////////////////////////////////////////////////
// INCLUDE

/////////////////////////////////////////////////
// PREPROCESSOR

/////////////////////////////////////////////////
// DEFINE
//#define LED_DEVICE_MANAGER_CONSOLE_DEBUG_ENABLE
/////////////////////////////////////////////////
// MARCO

/////////////////////////////////////////////////
// GLOBAL VARIABLES
#ifdef LED_DEVICE_MANAGER_CONSOLE_DEBUG_ENABLE
char ledDeviceManagerLogBuf[SYSTEM_CONSOLE_OUT_BUF_LEN];
#endif // LED_DEVICE_MANAGER_CONSOLE_DEBUG_ENABLE
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

/*LedDeviceManager*/

LedDeviceManager& LedDeviceManager::getInstance(void)
{
  static LedDeviceManager instance;
  return instance;
}

LedDeviceManager::LedDeviceManager(void)
  : device_Count(0)
  , device_Max_Count(0)
  , device_List(NULL)
{
  
}

LedDeviceManager::~LedDeviceManager(void)
{
  this->clear();
}

bool LedDeviceManager::isValid(void)
{
  return (this->device_List == NULL ? false : true);
}

int LedDeviceManager::initialize(LedDeviceManagerConfigTAG& deviceManagerConfig)
{
  do
  {
    if (this->isValid() != false)
    {
      return 0; // already
    }

    if ( (deviceManagerConfig.deviceMaxCount <= 0)
      || (deviceManagerConfig.deviceMaxCount > LED_DEVICE_MANAGER_MAX_DEVICE)
      )
    {
      break;
    }

    this->device_Count = 0;
    this->device_Max_Count = deviceManagerConfig.deviceMaxCount;

    this->device_List = new LedDeviceInfo[this->device_Max_Count + 1];
    if (this->device_List == NULL)
    {
      break;
    }

    return 0;
  } while (0);
  this->clear();
  return -1;
}

int LedDeviceManager::appendDevices(LedDeviceConfigTAG* deviceConfig, byte deviceCount)
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

int LedDeviceManager::appendDevice(LedDeviceConfigTAG& deviceConfig)
{
  int ret = 0;
  LedDeviceInfo* nextDevice = NULL;
  do
  {
#ifdef LED_DEVICE_MANAGER_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(ledDeviceManagerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[ledDMgr] ap %i", 0);
#endif // LED_DEVICE_MANAGER_CONSOLE_DEBUG_ENABLE

    if (this->device_Count >= this->device_Max_Count)
    {
#ifdef LED_DEVICE_MANAGER_CONSOLE_DEBUG_ENABLE
      CONSOLE_LOG_BUF(ledDeviceManagerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[ledDMgr] ap %i", -1);
#endif // LED_DEVICE_MANAGER_CONSOLE_DEBUG_ENABLE
      break;
    }

    nextDevice = &this->device_List[this->device_Count];
    nextDevice->clear();
    //deviceConfig.devManager = this;
    ret = nextDevice->initialize(deviceConfig);
#ifdef LED_DEVICE_MANAGER_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(ledDeviceManagerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[ledDMgr] ap %i %i", 2, ret);
#endif // LED_DEVICE_MANAGER_CONSOLE_DEBUG_ENABLE
    if (ret != 0)
    {
      break;
    }
#ifdef LED_DEVICE_MANAGER_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(ledDeviceManagerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[ledDMgr] ap %i", 99);
#endif // LED_DEVICE_MANAGER_CONSOLE_DEBUG_ENABLE
    this->device_Count++;
    return 0;
  } while (0);
#ifdef LED_DEVICE_MANAGER_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(ledDeviceManagerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[ledDMgr] ap %i", -99);
#endif // LED_DEVICE_MANAGER_CONSOLE_DEBUG_ENABLE
  if (nextDevice != NULL)
  {
    nextDevice->clear();
  }
  return -1;
}


byte LedDeviceManager::deviceCount(void)
{
  return this->device_Count;
}


LedDeviceInfo* LedDeviceManager::deviceByChannel(LedChannelId_t channelId)
{
  LedDeviceInfo* deviceInfo = NULL;
  byte wk_idx = 0;
  do
  {
    if ( (channelId < LED_DEVICE_CHANNEL_ID_MIN)
      || (channelId > LED_DEVICE_CHANNEL_ID_MAX)
      )
    {
      break;
    }

    for (wk_idx = 0; wk_idx < this->device_Count; wk_idx++)
    {
      if (this->device_List[wk_idx].channelId() == channelId)
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

LedDeviceInfo* LedDeviceManager::deviceByIndex(byte deviceIndex)
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

void LedDeviceManager::clear(void)
{
  if (this->device_List != NULL)
  {
    delete[] this->device_List;
  }
  this->device_List = NULL;
  this->device_Count = 0;
  this->device_Max_Count = 0;
}

void LedDeviceManager::dump(void)
{
#ifdef LED_DEVICE_MANAGER_CONSOLE_DEBUG_ENABLE
  do
  {
    if (this->isValid() == false)
    {
      CONSOLE_LOG_BUF(ledDeviceManagerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[ledDMgr] %s", "Invalid");
      break;
    }

    for (byte wk_idx = 0; wk_idx < this->device_Count; wk_idx++)
    {
      CONSOLE_LOG_BUF(ledDeviceManagerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "dev%u:", wk_idx);
      if (this->device_List[wk_idx].isValid() == false)
      {
        CONSOLE_LOG_BUF(ledDeviceManagerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "%s", "  invalid");
        continue;
      }

      this->device_List[wk_idx].dump();
    }

    return;
  } while (0);
  return;
#endif // LED_DEVICE_MANAGER_CONSOLE_DEBUG_ENABLE
}

#endif // _CONF_LED_DEVICE_MANAGER_ENABLED
