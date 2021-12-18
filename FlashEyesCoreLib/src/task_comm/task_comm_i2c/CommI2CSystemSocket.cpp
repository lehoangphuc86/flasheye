/////////////////////////////////////////////////
// INCLUDE
#include "CommI2CSystemSocket.h"
#if (_CONF_COMM_I2C_SYSTEM_SOCKET_ENABLED)
/////////////////////////////////////////////////
// PREPROCESSOR
//#define COMM_I2C_SYSTEM_SOCKET_CONSOLE_DEBUG_ENABLE
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
/* CommI2CSystemSocket*/
CommI2CSystemSocket::CommI2CSystemSocket(void)
  : i2c_Address(COMM_I2C_ADDRESS_INVALID)
  , i2c_Controller(NULL)
{

}
CommI2CSystemSocket::~CommI2CSystemSocket(void)
{
  this->stop();
}

bool CommI2CSystemSocket::isServerMode(void)
{
  return (this->i2c_Address == COMM_I2C_ADDRESS_INVALID ? true : false);
}

uint8_t CommI2CSystemSocket::address(void)
{
  return this->i2c_Address;
}

bool CommI2CSystemSocket::isValid(void)
{
  return (this->i2c_Controller == NULL ? false : true);
}


int CommI2CSystemSocket::start(CommI2CSocketConfigTAG& socketConfig)
{
  do
  {
    if (this->isValid() != false)
    {
      return -1; // already started
    }

    if (socketConfig.isValid() == false)
    {
      break;
    }

    this->i2c_Address = socketConfig.address;
    this->i2c_Controller = CommI2CSocketHandlerFactory::generate(socketConfig);
    if (this->i2c_Controller == NULL)
    {
      break;
    }

    this->setTimeOut(socketConfig.timeout);

    bool ret = true;
#ifdef ARDUINO_ESP32_DEV
    if (socketConfig.isServerMode() == false)
    {
      // Currently ESP32 does not support slave mode by Wire library.
      CONSOLE_LOG("[i2cSo] sta %i", -1);
      break;
    }
    else
    {
      int sda = (socketConfig.pin_SDA == COMM_I2C_PIN_INVALID ? -1 : socketConfig.pin_SDA);
      int scl = (socketConfig.pin_SCL == COMM_I2C_PIN_INVALID ? -1 : socketConfig.pin_SCL);
      ret = this->i2c_Controller->begin(sda, scl, socketConfig.frequency);
#ifdef COMM_I2C_SYSTEM_SOCKET_CONSOLE_DEBUG_ENABLE
      CONSOLE_LOG("[i2cSo] %i %i %i %i", sda, scl, socketConfig.frequency, ret);
#endif // COMM_I2C_SYSTEM_SOCKET_CONSOLE_DEBUG_ENABLE
    }
    
#endif // ARDUINO_ESP32_DEV

#ifdef SYSTEM_AVR_PLATFORM
    if (socketConfig.isServerMode() == false)
    {
      this->i2c_Controller->begin(socketConfig.address);
    }
    else
    {
      this->i2c_Controller->begin();
    }
#endif // SYSTEM_AVR_PLATFORM

    if (ret == false)
    {
      break;
    }
    return 0;
  } while (0);

  this->stop(); // clear if it false
  return -1;
}


void CommI2CSystemSocket::stop(void)
{
  do
  {
    if (this->isValid() == false)
    {
      break;
    }
    this->i2c_Address = COMM_I2C_ADDRESS_INVALID;
    CommI2CSocketHandlerFactory::release(this->i2c_Controller);
    return;
  } while (0);

  return;
}

// config
void CommI2CSystemSocket::onReceive(CommI2CSystemCbOnDataReceived cbOnReceived)
{
#ifdef ARDUINO_ESP32_DEV
  CONSOLE_LOG("[i2cSo] setRev %i", -1);
#else
  do
  {
    if (this->isValid() == false)
    {
      break;
    }
    this->i2c_Controller->onReceive(cbOnReceived);

    return;
  } while (0);

  return;

#endif // ARDUINO_ESP32_DEV

}

void CommI2CSystemSocket::onRequest(CommI2CSystemCbOnRequested cbOnRequest)
{
#ifdef ARDUINO_ESP32_DEV
  CONSOLE_LOG("[i2cSo] setReq %i", -1);
#else
  do
  {
    if (this->isValid() == false)
    {
      break;
    }
    this->i2c_Controller->onRequest(cbOnRequest);
    return;
  } while (0);

  return;
#endif // ARDUINO_ESP32_DEV
}

void CommI2CSystemSocket::setClock(uint32_t frequency)
{
  do
  {
    if (this->isValid() == false)
    {
      break;
    }

    this->i2c_Controller->setClock(frequency);
    return;
  } while (0);

  return;
}

void CommI2CSystemSocket::setTimeOut(uint16_t timeOutMillis)
{
  do
  {
    if (this->isValid() == false)
    {
      break;
    }

#ifdef SYSTEM_AVR_PLATFORM
    this->i2c_Controller->setTimeout(timeOutMillis);
#endif // SYSTEM_AVR_PLATFORM

#ifdef ARDUINO_ESP32_DEV
    this->i2c_Controller->setTimeOut(timeOutMillis);
#endif // ARDUINO_ESP32_DEV
    return;
  } while (0);

  return;
}

void CommI2CSystemSocket::beginTransmission(int address)
{
  do
  {
    if (this->isValid() == false)
    {
      break;
    }

    this->i2c_Controller->beginTransmission(address);
    return;
  } while (0);

  return;
}

uint8_t CommI2CSystemSocket::endTransmission(bool sendStop)
{

  do
  {
    if (this->isValid() == false)
    {
      break;
    }

    return this->i2c_Controller->endTransmission(sendStop);
  } while (0);

  return 0;
}

uint8_t CommI2CSystemSocket::requestFrom(int address, int size, int sendStop)
{
  do
  {
    if (this->isValid() == false)
    {
      break;
    }

    return this->i2c_Controller->requestFrom(address, size, sendStop);
  } while (0);

  return 0;
}

DataSize_t CommI2CSystemSocket::write(uint8_t val)
{
  return this->i2c_Controller->write(val);
}

DataSize_t CommI2CSystemSocket::write(const uint8_t* data, DataSize_t quantity)
{
  do
  {
    if (this->isValid() == false)
    {
      break;
    }

    for (DataSize_t i = 0; i < quantity; ++i)
    {
      if (write(data[i])==0)
      {
        return i;
      }
    }
    return quantity;

  } while (0);

  return 0;
}

int CommI2CSystemSocket::writeData(uint8_t clientAddress, const uint8_t* data, DataSize_t quantity, DataSize_t& writtenLen)
{
  writtenLen = 0;
  int result = 0;
  do
  {
    if (this->isValid() == false)
    {
      break;
    }
    if (clientAddress != COMM_I2C_ADDRESS_INVALID)
    {
      this->i2c_Controller->beginTransmission(clientAddress);
    }

    for (writtenLen = 0; writtenLen < quantity; ++writtenLen)
    {
      if (this->i2c_Controller->write(data[writtenLen]) == 0)
      {
        break;
      }
    }

    if (clientAddress != COMM_I2C_ADDRESS_INVALID)
    {
      result = this->endTransmission(COMM_I2C_ENDTRANSACTION_SEND_STOP);
    }

    if ( (result!=0)
      || (writtenLen != quantity)
      )
    {
#ifdef COMM_I2C_SYSTEM_SOCKET_CONSOLE_DEBUG_ENABLE
//      CONSOLE_LOG("[sSoc] %i %i %i %i", -33, writtenLen, quantity, result);
#endif // COMM_I2C_SYSTEM_SOCKET_CONSOLE_DEBUG_ENABLE

      break;
    }

    return 0;
  } while (0);
  //writtenLen = 0;
  return -1;
}


int CommI2CSystemSocket::available(void)
{
  return this->i2c_Controller->available();
}

int CommI2CSystemSocket::readData(uint8_t clientAddress, uint8_t* data, DataSize_t maxToRead, DataSize_t& readLen)
{
  do
  {
    readLen = 0;
    if (this->isValid() == false)
    {
      return -1;
    }
    DataSize_t avail = 0;
    readLen = 0;
    if (clientAddress != COMM_I2C_ADDRESS_INVALID)
    {
      byte retRequest = this->i2c_Controller->requestFrom((uint8_t)clientAddress, (uint8_t)maxToRead, (uint8_t)COMM_I2C_ENDTRANSACTION_SEND_STOP);
      if (retRequest != maxToRead)
      {
        // failed to request
//#ifdef COMM_I2C_SYSTEM_SOCKET_CONSOLE_DEBUG_ENABLE
//        CONSOLE_LOG("[sSoc] rev %i %i %i %i", -22, clientAddress, maxToRead, retRequest);
//#ifdef ESP_PLATFORM
//        CONSOLE_LOG("[sSoc] rev %i %i", -23, this->i2c_Controller->lastError());
//#endif // ESP_PLATFORM
//#endif // COMM_I2C_SYSTEM_SOCKET_CONSOLE_DEBUG_ENABLE
        break;
      }
    }

    while (maxToRead > 0)
    {
      avail = this->i2c_Controller->available();
      if (avail <= 0)
      {
        break;
      }

      if (avail > maxToRead)
      {
        avail = maxToRead;
      }
      maxToRead = maxToRead - avail;
      while (avail > 0)
      {
        data[readLen] = this->i2c_Controller->read();
        readLen++;
        avail--;
      }
    }
    return 0;
  } while (0);

  readLen = 0;
  return -1;
}

int CommI2CSystemSocket::peek(void)
{
  return this->i2c_Controller->peek();
}

void CommI2CSystemSocket::flush(void)
{
  if (this->isValid() != false)
  {
    this->i2c_Controller->flush();
  }
}

DataSize_t CommI2CSystemSocket::flushReceivedBuff(void)
{
  DataSize_t count = 0;
  do
  {
    if (this->isValid() == false)
    {
      break;
    }

    while ((this->i2c_Controller->available() != false))
    {
      this->i2c_Controller->read();
      count++;
    }
    return count;
  } while (0);

  return count;
}

DataSize_t CommI2CSystemSocket::write(const char* s)
{
  return this->write((uint8_t*)s, strlen(s));
}

DataSize_t CommI2CSystemSocket::write(unsigned long n)
{
  return this->write((uint8_t)n);
}

DataSize_t CommI2CSystemSocket::write(long n)
{
  return this->write((uint8_t)n);
}

DataSize_t CommI2CSystemSocket::write(int n)
{
  return this->write((uint8_t)n);
}


/*CommI2CSocketHandlerFactory*/
CommI2CSocketHandlerFactory::CommI2CSocketHandlerFactory(void)
{

}

CommI2CSocketHandlerFactory::~CommI2CSocketHandlerFactory(void)
{

}


CommI2CSocketHandler CommI2CSocketHandlerFactory::generate(CommI2CSocketConfigTAG& socketConfig)
{
  CommI2CSocketHandler i2cController = COMM_I2C_SOCKET_HANDLE_INVALID;
  do
  {
#ifdef ARDUINO_ESP32_DEV
    i2cController = new TwoWire(socketConfig.busNumer);
#elif defined (SYSTEM_AVR_PLATFORM)
    i2cController = new TwoWire();
#endif // SYSTEM_AVR_PLATFORM
    return i2cController;
  } while (0);
  CommI2CSocketHandlerFactory::release(i2cController);
  return i2cController;
}

void CommI2CSocketHandlerFactory::release(CommI2CSocketHandler& i2cController)
{
  do
  {
    if (i2cController == COMM_I2C_SOCKET_HANDLE_INVALID)
    {
      break;
    }
    delete i2cController;
    i2cController = COMM_I2C_SOCKET_HANDLE_INVALID;
    return;
  } while (0);

  return;

}
#endif // _CONF_COMM_I2C_SYSTEM_SOCKET_ENABLED