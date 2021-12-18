/////////////////////////////////////////////////
// INCLUDE
#include "CommI2CSetting.h"
#if (_CONF_COMM_I2C_COMM_SETTING_ENABLED)
#include "task_codec/TaskCodecConstant.h"
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
//
//
//CommI2CSocketSetting::CommI2CSocketSetting(void)
//{
//  memset(&this->comm_Socket_Config, 0, sizeof(CommI2CSocketConfigTAG));
//}
//
//CommI2CSocketSetting::~CommI2CSocketSetting(void)
//{
//
//}
//
//bool CommI2CSocketSetting::isValidConfig(void)
//{
//  return CommI2CSocketSetting::isValidConfig(this->comm_Socket_Config);
//}
//
//bool CommI2CSocketSetting::enabled(void)
//{
//  return this->comm_Socket_Config.enabled;
//}
//
//byte CommI2CSocketSetting::busNumer(void)
//{
//  return this->comm_Socket_Config.busNumer;
//}
//
//byte CommI2CSocketSetting::pinSCL(void)
//{
//  return this->comm_Socket_Config.pin_SCL;
//}
//
//byte CommI2CSocketSetting::pinSDA(void)
//{
//  return this->comm_Socket_Config.pin_SDA;
//}
//
//uint32_t CommI2CSocketSetting::frequency(void)
//{
//  return this->comm_Socket_Config.frequency;
//}
//
//byte CommI2CSocketSetting::opMode(void)
//{
//  return this->comm_Socket_Config.opMode;
//}
//
//uint8_t CommI2CSocketSetting::address(void)
//{
//  return this->comm_Socket_Config.address;
//}
//
//uint16_t CommI2CSocketSetting::timeout(void)
//{
//  return this->comm_Socket_Config.timeout;
//}
//
//int CommI2CSocketSetting::setConfig(CommI2CSocketConfigTAG& commConfig)
//{
//  do
//  {
//    if (this->isValidConfig(commConfig) == false)
//    {
//      break;
//    }
//    memcpy(&this->comm_Socket_Config, &commConfig, sizeof(CommI2CSocketConfigTAG));
//    this->comm_Socket_Config.enabled = true;
//    return 0;
//  } while (0);
//  return -1;
//}
//
//bool CommI2CSocketSetting::isValidConfig(CommI2CSocketConfigTAG& commConfig)
//{
//  do
//  {
//    if (commConfig.enabled == false)
//    {
//      break;
//    }
//
//    // validate mode
//    if ((commConfig.opMode != (byte)CommI2COperationMode::CommI2CClientMode)
//      && (commConfig.opMode != (byte)CommI2COperationMode::CommI2CServerMode)
//      )
//    {
//      break;
//    }
//
//    // validate scl pin
//    if (commConfig.pin_SCL != COMM_I2C_PIN_INVALID)
//    {
//      if ((commConfig.pin_SCL < COMM_I2C_PIN_MIN)
//        || (commConfig.pin_SCL > COMM_I2C_PIN_MAX)
//        )
//      {
//        break;
//      }
//    }
//
//    // validate sda pin
//    if (commConfig.pin_SDA != COMM_I2C_PIN_INVALID)
//    {
//      if ((commConfig.pin_SDA < COMM_I2C_PIN_MIN)
//        || (commConfig.pin_SDA > COMM_I2C_PIN_MAX)
//        )
//      {
//        break;
//      }
//    }
//
//    // validate bus number
//    if (commConfig.busNumer != COMM_I2C_BUS_NUM_INVALID)
//    {
//      if ((commConfig.busNumer < COMM_I2C_BUS_NUM_MIN)
//        || (commConfig.busNumer > COMM_I2C_BUS_NUM_MAX)
//        )
//      {
//        break;
//      }
//    }
//
//    // validate frequency
//    if (commConfig.frequency != COMM_I2C_FREQUENCY_INVALID)
//    {
//      if ((commConfig.frequency < COMM_I2C_FREQUENCY_MIN)
//        || (commConfig.frequency > COMM_I2C_FREQUENCY_MAX)
//        )
//      {
//        break;
//      }
//    }
//
//    return true;
//  } while (0);
//
//  return false;
//}



/* CommI2CConnectionSetting*/
CommI2CConnectionSetting::CommI2CConnectionSetting(void)
{
  memset(&this->comm_Connection_Config, 0, sizeof(CommI2CConfigTAG));
}

CommI2CConnectionSetting::~CommI2CConnectionSetting(void)
{

}

bool CommI2CConnectionSetting::isValidConfig(void)
{
  return CommI2CConnectionSetting::isValidConfig(this->comm_Connection_Config);
}

bool CommI2CConnectionSetting::enabled(void)
{
  return this->comm_Connection_Config.enabled();
}

bool CommI2CConnectionSetting::useOnrequestCallback(void)
{
  return (this->comm_Connection_Config.cbDataRequested == NULL ? false : true);
}

//bool CommI2CConnectionSetting::eventUsingParent(void)
//{
//  return (this->comm_Connection_Config.parentEventManager == NULL ? false : true);
//}

//byte CommI2CConnectionSetting::opMode(void)
//{
//  return this->comm_Connection_Config.opMode;
//}

//byte CommI2CConnectionSetting::i2cProtocol(void)
//{
//  return this->comm_Connection_Config.i2cProtocol;
//}

bool CommI2CConnectionSetting::useCodec(void)
{
  return TaskCodecConstant::isValidCodecType(this->comm_Connection_Config.codecType);
}

bool CommI2CConnectionSetting::useCheckSum(void)
{
  return this->comm_Connection_Config.useCheckSum();
}


byte CommI2CConnectionSetting::codecType(void)
{
  return this->comm_Connection_Config.codecType;
}

bool CommI2CConnectionSetting::isServerMode(void)
{
  return this->comm_Connection_Config.isServerMode();
}

bool CommI2CConnectionSetting::isHeaderLess(void)
{
  return this->comm_Connection_Config.isHeaderless();
}

void* CommI2CConnectionSetting::parentPointer(void)
{
  return this->comm_Connection_Config.parentPointer;
}

CommI2CCbOnDataReceived CommI2CConnectionSetting::cbDataReceived(void)
{
  return this->comm_Connection_Config.cbDataReceived;
}

CommI2CCbOnDataRequested CommI2CConnectionSetting::cbDataRequested(void)
{
  return this->comm_Connection_Config.cbDataRequested;
}

DataSize_t CommI2CConnectionSetting::bufferMaxSize(void)
{
  return this->comm_Connection_Config.bufferMaxSize;
}

DataCount_t CommI2CConnectionSetting::bufferCount(void)
{
  return this->comm_Connection_Config.bufferCount;
}

bool CommI2CConnectionSetting::bufferUsePool(void)
{
  return this->comm_Connection_Config.bufferUsePool();
}

int CommI2CConnectionSetting::setConfig(CommI2CConfigTAG& commConfig)
{
  do
  {
    if (this->isValidConfig(commConfig) == false)
    {
      break;
    }

    memcpy(&this->comm_Connection_Config, &commConfig, sizeof(CommI2CConfigTAG));
    this->comm_Connection_Config.enabled(true);
    return 0;
  } while (0);
  return -1;
}

bool CommI2CConnectionSetting::isValidConfig(CommI2CConfigTAG& commConfig)
{
  do
  {
    if (commConfig.enabled() == false)
    {
      break;
    }

    // validate buffer
    if ((commConfig.bufferCount < COMM_I2C_BUFFER_COUNT_MIN)
      || (commConfig.bufferMaxSize < COMM_I2C_BUFFER_SIZE_MIN)
      )
    {
      break;
    }

    // validate for client
    if (commConfig.isServerMode() != false)
    {
      return true; // server mode
    }

    // for client mode
    {
      /*if ( CommI2CConstant::isValidI2CProtocol(commConfig.i2cProtocol) == false)
      {
        break;
      }
      */
      // simple protocol. Always callback onRequest to upper side.
      if ( (commConfig.isHeaderless() != false)
        && ((commConfig.parentPointer == NULL)
            || (commConfig.cbDataRequested == NULL)
          )
        )
      {
        break;
      }

      // advanced protocol
      // use callback
      /*if ((commConfig.parentEventManager == NULL)
        && (commConfig.cbDataReceived == NULL)
        )*/
      if (commConfig.cbDataReceived == NULL)
      {
        break;
      }

      return true;
    }

    return true;
  } while (0);

  return false;
}


#endif // _CONF_COMM_I2C_COMM_SETTING_ENABLED