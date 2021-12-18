/////////////////////////////////////////////////
// INCLUDE
#include "CommSerialSetting.h"
#if (_CONF_COMM_SERIAL_SETTING_ENABLED)
/////////////////////////////////////////////////
// PREPROCESSOR

/////////////////////////////////////////////////
// DEFINE


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

/////////////////////////////////////////////////
// STATIC FUNCTIONS

/////////////////////////////////////////////////
// EXTERN

/////////////////////////////////////////////////
// CLASS IMPLEMENTAION
/* CommSerialConnectionSetting*/
CommSerialConnectionSetting::CommSerialConnectionSetting(void)
{
  memset(&this->comm_Connection_Config, 0, sizeof(CommSerialConfigTAG));
}

CommSerialConnectionSetting::~CommSerialConnectionSetting(void)
{

}

bool CommSerialConnectionSetting::isValidConfig(void)
{
  return CommSerialConnectionSetting::isValidConfig(this->comm_Connection_Config);
}

bool CommSerialConnectionSetting::enabled(void)
{
  return this->comm_Connection_Config.enabled();
}

bool CommSerialConnectionSetting::isHeaderless(void)
{
  return this->comm_Connection_Config.isHeaderless();
}

bool CommSerialConnectionSetting::useCheckSum(void)
{
  return this->comm_Connection_Config.useCheckSum();
}

bool CommSerialConnectionSetting::useCodec(void)
{
  return TaskCodecConstant::isValidCodecType(this->comm_Connection_Config.codecType);
}

byte CommSerialConnectionSetting::codecType(void)
{
  return this->comm_Connection_Config.codecType;
}

void* CommSerialConnectionSetting::cbParams(void)
{
  return this->comm_Connection_Config.cbParams;
}

CommSerialCbOnDataReceived CommSerialConnectionSetting::cbDataReceived(void)
{
  return this->comm_Connection_Config.cbDataReceived;
}

CommSerialCbOnConnectionClosed CommSerialConnectionSetting::cbConnectionClosed(void)
{
  return this->comm_Connection_Config.cbConnectionClosed;
}

//unsigned int CommSerialConnectionSetting::taskRevPollingTime(void)
//{
//  return this->comm_Connection_Config.taskRevPollingTime;
//}

DataSize_t CommSerialConnectionSetting::bufferMaxSize(void)
{
  return this->comm_Connection_Config.bufferMaxSize;
}

DataCount_t CommSerialConnectionSetting::bufferCount(void)
{
  return this->comm_Connection_Config.bufferCount;
}

bool CommSerialConnectionSetting::bufferUsePool(void)
{
  return this->comm_Connection_Config.bufferUsePool();
}

char CommSerialConnectionSetting::terminator(void)
{
  return this->comm_Connection_Config.terminator;
}

EventCount_t CommSerialConnectionSetting::taskEventItemNumer(void)
{
  return this->comm_Connection_Config.taskEventItemNumber;
}

bool CommSerialConnectionSetting::taskEventUsePool(void)
{
  return this->comm_Connection_Config.taskEventUsePool();
}

unsigned short CommSerialConnectionSetting::taskStackDepth(void)
{
  return this->comm_Connection_Config.taskStackDepth;
}

byte CommSerialConnectionSetting::taskPriority(void)
{
  return this->comm_Connection_Config.taskPriority;
}

bool CommSerialConnectionSetting::taskThreadUsePool(void)
{
  return this->comm_Connection_Config.taskThreadUsePool();
}
int CommSerialConnectionSetting::setConfig(CommSerialConfigTAG& commConfig)
{
  do
  {
    if (this->isValidConfig(commConfig) == false)
    {
      break;
    }
    memcpy(&this->comm_Connection_Config, &commConfig, sizeof(CommSerialConfigTAG));
    this->comm_Connection_Config.enabled(true);
    return 0;
  } while (0);
  return -1;
}

bool CommSerialConnectionSetting::isValidConfig(CommSerialConfigTAG& commConfig)
{
  do
  {
    if (commConfig.enabled() == false)
    {
      break;
    }
    // validate buffer
    if ((commConfig.bufferCount < COMM_SERIAL_BUFFER_COUNT_MIN)
      || (commConfig.bufferMaxSize < COMM_SERIAL_BUFFER_SIZE_MIN)
      )
    {
      break;
    }

    /*if ( (commConfig.rawMaxRevSize <=0)
        || (commConfig.rawMaxRevSize > COMM_SERIAL_SYS_BUFFER_LEN)
      )
    {
      break;
    }*/
    
#ifdef COMM_SERIAL_USE_TASK
    if ((commConfig.cbDataReceived == NULL)
      || (commConfig.cbConnectionClosed == NULL)
      )
    {
      break;
    }
#endif // COMM_SERIAL_USE_TASK

    return true;
  } while (0);

  return false;
}


#endif // _CONF_COMM_I2C_COMM_SETTING_ENABLED