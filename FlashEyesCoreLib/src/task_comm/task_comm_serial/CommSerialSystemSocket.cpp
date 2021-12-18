
/////////////////////////////////////////////////
// INCLUDE
#include "CommSerialSystemSocket.h"
#if (_CONF_COMM_SERIAL_SYSTEM_SOCKET_ENABLED)
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



/* CommSerialSystemSocket*/
CommSerialSystemSocket::CommSerialSystemSocket(void)
  : serial_Timeout(0)
  , serial_Handler(COMM_SERIAL_SOCKET_HANDLE_INVALID)
#if defined(SYSTEM_WIN_PLATFORM)
  , thread_Exit_Event(COMM_SERIAL_EVENT_INVALID)
  , rev_Event(COMM_SERIAL_EVENT_INVALID)
  , sen_Event(COMM_SERIAL_EVENT_INVALID)
#endif // defined(SYSTEM_WIN_PLATFORM)
{

}
CommSerialSystemSocket::~CommSerialSystemSocket(void)
{
  this->stop();
}

int CommSerialSystemSocket::setConfig(void)
{
  do
  {
    if (this->isValid() != false)
    {
      return -1; // already 
    }

    /*int ret = this->comm_Socket_Config.setConfig(socketConfig);
    if (ret != 0)
    {
      break;
    }*/

    return 0;
  } while (0);
  return -1;
}

//int CommSerialSystemSocket::prepare(void)
//{
//  do
//  {
//    if (this->isValid() != false)
//    {
//      return -1;
//    }
//
//    if (this->comm_Socket_Config.isValidConfig() == false)
//    {
//      break;
//    }
//   
//    return 0;
//  } while (0);
//
//  this->stop();
//  return -1;
//}

int CommSerialSystemSocket::start(CommSerialSocketConfigTAG& socketConfig)
{
  do
  {
    //if (this->comm_Socket_Config.isValidConfig() == false)
    if (CommSerialConstant::isValidSocketConfig(socketConfig) == false)
    {
      return -1;
    }

    this->stop();
    {
      // initialized thread exit event
#ifdef SYSTEM_WIN_PLATFORM
      this->thread_Exit_Event = CreateEvent(NULL, TRUE, FALSE, NULL);
      this->rev_Event = CreateEvent(NULL, TRUE, FALSE, NULL);
      this->sen_Event = CreateEvent(NULL, TRUE, FALSE, NULL);
      if ((this->thread_Exit_Event == COMM_SERIAL_EVENT_INVALID)
        || (this->rev_Event == COMM_SERIAL_EVENT_INVALID)
        || (this->sen_Event == COMM_SERIAL_EVENT_INVALID)
        )
      {
        break;
    }
#endif // SYSTEM_WIN_PLATFORM
      
    }

    this->serial_Handler = CommSerialHandlerFactory::generate(socketConfig);
    if (this->serial_Handler == COMM_SERIAL_SOCKET_HANDLE_INVALID)
    {
      break;
    }

    if (this->setSocketConfig(socketConfig) != 0)
    {
      break;
    }

    if (this->setSocketTimeout(socketConfig) != 0)
    {
      break;
    }

    this->serial_Timeout = socketConfig.timeout;

    return 0;
  } while (0);

  this->stop();
  return -1;
}

int CommSerialSystemSocket::notifyExitEvent(void)
{
  do
  {
    if ( this->isValid() == false)
    {
      break;
    }

#ifdef SYSTEM_WIN_PLATFORM
    SetEvent(this->thread_Exit_Event);
#endif // SYSTEM_WIN_PLATFORM
    return 0;
  } while (0);

  return -1;
}

void CommSerialSystemSocket::stop(void)
{
  do
  {
    if (this->isValid() == false)
    {
      break;
    }
    CommSerialHandlerFactory::release(this->serial_Handler);
#ifdef SYSTEM_WIN_PLATFORM
    ResetEvent(this->thread_Exit_Event);
    CloseHandle(this->thread_Exit_Event);
    ResetEvent(this->rev_Event);
    CloseHandle(this->rev_Event);
    ResetEvent(this->sen_Event);
    CloseHandle(this->sen_Event);
    this->thread_Exit_Event = COMM_SERIAL_EVENT_INVALID;
    this->rev_Event = COMM_SERIAL_EVENT_INVALID;
    this->sen_Event = COMM_SERIAL_EVENT_INVALID;
#endif // SYSTEM_WIN_PLATFORM
    this->serial_Handler = COMM_SERIAL_SOCKET_HANDLE_INVALID;
    return;
  } while (0);

  this->serial_Handler = COMM_SERIAL_SOCKET_HANDLE_INVALID;
#ifdef SYSTEM_WIN_PLATFORM
  this->thread_Exit_Event = COMM_SERIAL_EVENT_INVALID;
  this->rev_Event = COMM_SERIAL_EVENT_INVALID;
  this->sen_Event = COMM_SERIAL_EVENT_INVALID;
#endif // SYSTEM_WIN_PLATFORM
  return;
}

bool CommSerialSystemSocket::isValid(void)
{
  do
  {
#ifdef SYSTEM_WIN_PLATFORM
    if ( (this->thread_Exit_Event == COMM_SERIAL_EVENT_INVALID)
      || (this->rev_Event == COMM_SERIAL_EVENT_INVALID)
      || (this->sen_Event == COMM_SERIAL_EVENT_INVALID)
      )
    {
      break;
    }
#endif // SYSTEM_WIN_PLATFORM
    return (this->serial_Handler == COMM_SERIAL_SOCKET_HANDLE_INVALID ? false : true);
  } while (0);
  return false;
}

uint16_t CommSerialSystemSocket::serialTimeout(void)
{
  return this->serial_Timeout;
}

//CommSerialSocketSetting* CommSerialSystemSocket::socketConfig(void)
//{
//  return (this->comm_Socket_Config.enabled() == false ? NULL : &this->comm_Socket_Config);
//}


#ifdef SYSTEM_ARDUINO_BASED
int CommSerialSystemSocket::writeData(const uint8_t* data, DataSize_t lenToWrite, DataSize_t& writtenLen)
{
  do
  {
    if (this->isValid() == false)
    {
      break;
    }

    writtenLen = this->serial_Handler->write(data, lenToWrite);
    return 0;
  } while (0);
  writtenLen = 0;
  return -1;
}
#else // SYSTEM_ARDUINO_BASED

#ifdef SYSTEM_WIN_PLATFORM
int CommSerialSystemSocket::writeData(const uint8_t* data, DataSize_t lenToWrite, DataSize_t& writtenLen)
{
  OVERLAPPED osWrite = { 0 };
  osWrite.hEvent = NULL;
  HANDLE hArray[2] = { NULL, NULL};
  DWORD dwWritten = 0;
  DWORD dwRes = 0;
  writtenLen = 0;
  do
  {
    if (this->isValid() == false)
    {
      break;
    }
    ResetEvent(this->sen_Event);
    osWrite.hEvent = this->sen_Event;// CreateEvent(NULL, TRUE, FALSE, NULL);
    if (osWrite.hEvent == NULL)
    {
      break;
    }

    hArray[0] = osWrite.hEvent;
    hArray[1] = this->thread_Exit_Event;
    bool isDisconnected = false;
    // try to write
    if (WriteFile(this->serial_Handler, data, (DWORD)lenToWrite, &dwWritten, &osWrite) == false)
    {
      if (GetLastError() != ERROR_IO_PENDING)
      {
        isDisconnected = true;
        break;
      }

      dwRes = WaitForMultipleObjects(2, hArray, FALSE, INFINITE); //INFINITE
      switch (dwRes)
      {
      case WAIT_OBJECT_0:
      {
        SetLastError(ERROR_SUCCESS);
        if (GetOverlappedResult(this->serial_Handler, &osWrite, &dwWritten, FALSE) == false)
        {
          isDisconnected = true;
        }
        break;
      }
      case WAIT_OBJECT_0 + 1: // exit
      case WAIT_TIMEOUT:
      case WAIT_FAILED:
      {
        isDisconnected = true;
        break;
      }
      default:
        break;

      }
    }

    if (isDisconnected != false)
    {
      break;
    }

    /*if (osWrite.hEvent != NULL)
    {
      CloseHandle(osWrite.hEvent);
      osWrite.hEvent = NULL;
    }*/

    writtenLen = (DataSize_t)dwWritten;
    return 0;
  } while (0);

  /*if (osWrite.hEvent != NULL)
  {
    CloseHandle(osWrite.hEvent);
    osWrite.hEvent = NULL;
  }*/

  writtenLen = (DataSize_t)dwWritten;
  return -1;
}

#else // SYSTEM_WIN_PLATFORM
// linux 
int CommSerialSystemSocket::writeData(const uint8_t* data, DataSize_t lenToWrite, DataSize_t& writtenLen)
{
  do
  {
    if (this->isValid() == false)
    {
      break;
    }

    writtenLen = (DataSize_t)write(this->serial_Handler, data, lenToWrite);
    if (writtenLen < 0)
    {
      break;
    }

    return 0;
  } while (0);

  writtenLen = 0;
  return -1;
}
#endif // SYSTEM_WIN_PLATFORM

#endif // SYSTEM_ARDUINO_BASED



#ifdef SYSTEM_ARDUINO_BASED
int CommSerialSystemSocket::readData(uint8_t* data, DataSize_t maxToRead, DataSize_t& readLen)
{
  do
  {
    readLen = 0;
    if (this->isValid() == false)
    {
      return 0;
    }
    DataSize_t avail = 0;
    readLen = 0;
    uint16_t timeOut = this->serialTimeout();
    while (maxToRead>0)
    {
      avail = this->serial_Handler->available();
      if (avail <= 0)
      {
        if (timeOut < COMM_SERIAL_TIME_OUT_SLICE)
        {
          break;
        }
        timeOut -= COMM_SERIAL_TIME_OUT_SLICE;
        SYSTEM_SLEEP(COMM_SERIAL_TIME_OUT_SLICE);
        continue;
      }

      if (avail > maxToRead)
      {
        avail = maxToRead;
      }
      //CONSOLE_LOG("[soc] rev %i %i %i %i", 1, maxToRead, readLen, avail);
      maxToRead = maxToRead - avail;
      while (avail > 0)
      {
        data[readLen] = this->serial_Handler->read();
        readLen++;
        avail--;
      }
      //CONSOLE_LOG("[soc] rev %i %i %i %i", 2, maxToRead, readLen, avail);
    }
    
    return 0;
  } while (0);

  readLen = 0;
  return -1;
}

#else // SYSTEM_ARDUINO_BASED

#ifdef SYSTEM_WIN_PLATFORM
int CommSerialSystemSocket::readData(uint8_t* data, DataSize_t maxToRead, DataSize_t& readLen)
{
  OVERLAPPED osReader = { 0 };  // overlapped structure for read operations
  osReader.hEvent = NULL;
  HANDLE hArray[2] = { NULL, NULL};
  DWORD dwRead = 0; // bytes actually read
  DWORD dwRes = 0; // result from WaitForSingleObject
  readLen = 0;
  do
  {
    if (this->isValid() == false)
    {
      return 0;
    }
    bool isDisconnected = false;
    ResetEvent(this->rev_Event);
    osReader.hEvent = this->rev_Event;// CreateEvent(NULL, TRUE, FALSE, NULL);
    if (osReader.hEvent == NULL)
    {
      break;
    }
    hArray[0] = osReader.hEvent;
    hArray[1] = this->thread_Exit_Event;;

    if (ReadFile(this->serial_Handler, data, (DWORD)maxToRead, &dwRead, &osReader) == false)
    {
      if (GetLastError() != ERROR_IO_PENDING)
      {
        isDisconnected = true;
        break;
      }

      dwRes = WaitForMultipleObjects(2, hArray, FALSE, INFINITE); // INFINITE
      switch (dwRes)
      {
        case WAIT_OBJECT_0:
        {
          if (GetOverlappedResult(this->serial_Handler, &osReader, &dwRead, FALSE) == false)
          {
            isDisconnected = true;
          }
          break;
        }
        case WAIT_OBJECT_0 + 1:
        {
          isDisconnected = true;
          break;
        }
        case WAIT_TIMEOUT:
          break;
        default:
          break;
      }
    }

    if (isDisconnected != false)
    {
      break;
    }

    readLen = (DataSize_t)dwRead;
    return 0;
  } while (0);
  
  readLen = (DataSize_t)dwRead;
  return -1;
}

#else // SYSTEM_WIN_PLATFORM
// linux 
int CommSerialSystemSocket::readData(uint8_t* data, DataSize_t maxToRead, DataSize_t& readLen)
{
  do
  {
    readLen = 0;
    if (this->isValid() == false)
    {
      return 0;
    }

    readLen = (DataSize_t)read(this->serial_Handler, data, maxToRead);
    return 0;
  } while (0);

  readLen = 0;
  return -1;
}
#endif // SYSTEM_WIN_PLATFORM

#endif // SYSTEM_ARDUINO_BASED


int CommSerialSystemSocket::setSocketConfig(CommSerialSocketConfigTAG& socketConfig)
{
  bool error = false;
  do
  {
    if (this->isValid() == false)
    {
      break;
    }

#ifdef SYSTEM_ARDUINO_BASED
    uint32_t configSet = SERIAL_8N1;
    switch (socketConfig.parityType)
    {
    case (byte)CommSerialConstant::SERIAL_PARITY_NONE:
    {
      switch (socketConfig.bitPerByte)
      {
      case 5:
        configSet = (socketConfig.stopBitNum == 1 ? SERIAL_5N1 : SERIAL_5N2);
        break;
      case 6:
        configSet = (socketConfig.stopBitNum == 1 ? SERIAL_6N1 : SERIAL_6N2);
        break;
      case 7:
        configSet = (socketConfig.stopBitNum == 1 ? SERIAL_7N1 : SERIAL_7N2);
        break;
      case 8:
        configSet = (socketConfig.stopBitNum == 1 ? SERIAL_8N1 : SERIAL_8N2);
        break;
      default:
        break;
      }
      break;
    }
    case (byte)CommSerialConstant::SERIAL_PARITY_EVEN:
    {
      switch (socketConfig.bitPerByte)
      {
      case 5:
        configSet = (socketConfig.stopBitNum == 1 ? SERIAL_5E1 : SERIAL_5E2);
        break;
      case 6:
        configSet = (socketConfig.stopBitNum == 1 ? SERIAL_6E1 : SERIAL_6E2);
        break;
      case 7:
        configSet = (socketConfig.stopBitNum == 1 ? SERIAL_7E1 : SERIAL_7E2);
        break;
      case 8:
        configSet = (socketConfig.stopBitNum == 1 ? SERIAL_8E1 : SERIAL_8E2);
        break;
      default:
        break;
      }
      break;
    }
    case (byte)CommSerialConstant::SERIAL_PARITY_ODD:
    {
      switch (socketConfig.bitPerByte)
      {
      case 5:
        configSet = (socketConfig.stopBitNum == 1 ? SERIAL_5O1 : SERIAL_5O2);
        break;
      case 6:
        configSet = (socketConfig.stopBitNum == 1 ? SERIAL_6O1 : SERIAL_6O2);
        break;
      case 7:
        configSet = (socketConfig.stopBitNum == 1 ? SERIAL_7O1 : SERIAL_7O2);
        break;
      case 8:
        configSet = (socketConfig.stopBitNum == 1 ? SERIAL_8O1 : SERIAL_8O2);
        break;
      default:
        break;
      }
      break;
    }
    default:
      break;
  }

#ifdef SYSTEM_AVR_PLATFORM
    this->serial_Handler->begin(
      socketConfig.baudrate
      , configSet
    );
#elif defined (SYSTEM_ESP_PLATFORM)
    int8_t pinTx = (socketConfig.pin_TX == COMM_SERIAL_PIN_INVALID ? COMM_SERIAL_PIN_LIB_DEFAULT : socketConfig.pin_TX);
    int8_t pinRx = (socketConfig.pin_RX == COMM_SERIAL_PIN_INVALID ? COMM_SERIAL_PIN_LIB_DEFAULT : socketConfig.pin_RX);
    this->serial_Handler->begin(
      socketConfig.baudrate
      , configSet
      , pinRx
      , pinTx
      , false
      , socketConfig.timeout
    );
#else
    break;
#endif // SYSTEM_AVR_PLATFORM

#else 
    // defined(SYSTEM_PC_BASED)
#ifdef SYSTEM_WIN_PLATFORM
    // get config
    DCB dcb = DCB();
    dcb.DCBlength = sizeof(dcb);
    if (GetCommState(this->serial_Handler, &dcb) == false)
    {
      break;
    }

    dcb.BaudRate = socketConfig.baudrate;
    dcb.ByteSize = socketConfig.bitPerByte;
    switch (socketConfig.parityType)
    {
      case (byte)CommSerialConstant::SERIAL_PARITY_NONE:
        dcb.Parity = NOPARITY;
        break;
      case (byte)CommSerialConstant::SERIAL_PARITY_ODD:
        dcb.Parity = ODDPARITY;
        break;
      case (byte)CommSerialConstant::SERIAL_PARITY_EVEN:
        dcb.Parity = EVENPARITY;
        break;
      default:
        break;
    };

    switch (socketConfig.stopBitNum)
    {
      case 1:
        dcb.StopBits = ONESTOPBIT;
        break;
      case 2:
        dcb.StopBits = TWOSTOPBITS;
        break;
      default:
        break;
    };
    
    // set config
    if (SetCommState(this->serial_Handler, &dcb) == false)
    {
      DWORD dw = GetLastError();
      break;
    }
#else // SYSTEM_WIN_PLATFORM
    // linux
    {
      //struct termios2 term2;
      termios2 ttyConfig = termios2();
      ioctl(this->serial_Handler, TCGETS2, &ttyConfig);
      // c_cflag
      // set parity
      switch (socketConfig.parityType)
      {
        case (byte)CommSerialConstant::SERIAL_PARITY_NONE:
        {
          ttyConfig.c_cflag &= ~PARENB; // no parity bit is added to the output characters
          break;
        }
        case (byte)CommSerialConstant::SERIAL_PARITY_ODD:
        {
          ttyConfig.c_cflag |= PARENB; // enable parity
          ttyConfig.c_cflag |= PARODD; // set parity odd
          break;
        }
        case (byte)CommSerialConstant::SERIAL_PARITY_EVEN:
        {
          ttyConfig.c_cflag |= PARENB; // enable parity. It is even by default
          break;
        }
        default:
          error = true;
          break;
      };


      if (error != false)
      {
        break;
      }

      switch (socketConfig.stopBitNum)
      {
        case 1:
          ttyConfig.c_cflag &= ~CSTOPB; // Only one stop-bit is used
          break;
        case 2:
          ttyConfig.c_cflag |= CSTOPB;  // Set stop field, two stop bits used in communication
          break;
        default:
          error = true;
          break;
      };
      
      if (error != false)
      {
        break;
      }

      ttyConfig.c_cflag &= ~CSIZE; // CSIZE is a mask for the number of bits per character
      switch (socketConfig.bitPerByte)
      {
        case 5: 
          ttyConfig.c_cflag |= CS5;
          break;
        case 6:
          ttyConfig.c_cflag |= CS6;
          break;
        case 7:
          ttyConfig.c_cflag |= CS7;
          break;
        case 8:
          ttyConfig.c_cflag |= CS8;
          break;
        default:
          error = true;
          break;
      }

      if (error != false)
      {
        break;
      }
      
      ttyConfig.c_cflag &= ~CRTSCTS; // Disable hadrware flow control (RTS/CTS)
      ttyConfig.c_cflag |= CREAD | CLOCAL; // Turn on READ & ignore ctrl lines (CLOCAL = 1)
      
      //c_lflag 
      // Canonical input is when read waits for EOL or EOF characters before returning. In non-canonical mode, the rate at which
      // read() returns is instead controlled by c_cc[VMIN] and c_cc[VTIME]
      ttyConfig.c_lflag &= ~ICANON; // Turn off canonical input, which is suitable for pass-through
      // Configure echo depending on echo_ boolean
      ttyConfig.c_lflag &= ~(ECHO); // Disable echo
      ttyConfig.c_lflag &= ~ECHOE; // Turn off echo erase (echo erase only relevant if canonical input is active)
      ttyConfig.c_lflag &= ~ECHONL; // Disable new-line echo
      ttyConfig.c_lflag &= ~ISIG; // Disables recognition of INTR (interrupt), QUIT and SUSP (suspend) characters
      // .c_iflag
      ttyConfig.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off s/w flow ctrl
      ttyConfig.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL);
      // c_oflag
      ttyConfig.c_oflag = 0; // No remapping, no delays
      ttyConfig.c_oflag &= ~OPOST; // Make raw

      // set baurdrate
      ttyConfig.c_ispeed = socketConfig.baudrate;
      ttyConfig.c_ospeed = socketConfig.baudrate;

      //set
      ioctl(this->serial_Handler, TCSETS2, &ttyConfig);
    }
#endif // SYSTEM_WIN_PLATFORM

#endif // SYSTEM_ARDUINO_BASED
    return 0;
  } while (0);

  return -1;
}

int CommSerialSystemSocket::setSocketTimeout(CommSerialSocketConfigTAG& socketConfig)
{

  do
  {
    if (this->isValid() == false)
    {
      break;
    }

#ifdef SYSTEM_ARDUINO_BASED
    this->serial_Handler->setTimeout(socketConfig.timeout);
    return 0;
#else // SYSTEM_ARDUINO_BASED
    // SYSTEM_PC_BASED
#ifdef SYSTEM_WIN_PLATFORM
    // set timeout
    COMMTIMEOUTS commTimeout = COMMTIMEOUTS();
    if (GetCommTimeouts(this->serial_Handler, &commTimeout) == false)
    {
      break;
    }
    //commTimeout.ReadIntervalTimeout = socketConfig.timeout;
    commTimeout.ReadIntervalTimeout = 0;
    commTimeout.ReadTotalTimeoutConstant = socketConfig.timeout;
    commTimeout.ReadTotalTimeoutMultiplier = 0;
    //commTimeout.ReadTotalTimeoutMultiplier = socketConfig.timeout;
    commTimeout.WriteTotalTimeoutConstant = socketConfig.timeout;
    //commTimeout.WriteTotalTimeoutMultiplier = socketConfig.timeout;

    if (SetCommTimeouts(this->serial_Handler, &commTimeout) == false)
    {
      break;
    }

#else // SYSTEM_WIN_PLATFORM
    // SYSTEM_LIMUX_PLATFORM
    {
      //struct termios2 term2;
      termios2 ttyConfig = termios2();
      ioctl(this->serial_Handler, TCGETS2, &ttyConfig);
      
      // set timeout
      ttyConfig.c_cc[VTIME] = (cc_t)(socketConfig.timeout / 100); // deciseconds = 1/10 second
      ttyConfig.c_cc[VMIN] = 0;
      //set
      ioctl(this->serial_Handler, TCSETS2, &ttyConfig);
    }

#endif // SYSTEM_WIN_PLATFORM
    
#endif // SYSTEM_ARDUINO_BASED
    return 0;
  } while (0);

  return -1;
}


/*CommSerialHandlerFactory*/
CommSerialHandlerFactory::CommSerialHandlerFactory(void)
{

}

CommSerialHandlerFactory::~CommSerialHandlerFactory(void)
{

}


SerialHandler CommSerialHandlerFactory::generate(CommSerialSocketConfigTAG& socketConfig)
{
  SerialHandler serialController = COMM_SERIAL_SOCKET_HANDLE_INVALID;
  do
  {
    

#ifdef SYSTEM_ARDUINO_BASED
    bool error = false;
    switch (socketConfig.instanceIndex)
    {
    case 0:
#ifdef COMM_SERIAL_HAVE_HWSERIAL0
      serialController = &Serial;
#else // COMM_SERIAL_HAVE_HWSERIAL0
      error = true;
#endif // COMM_SERIAL_HAVE_HWSERIAL0
      break;
    case 1:
#ifdef COMM_SERIAL_HAVE_HWSERIAL1
      serialController = &Serial1;
#else // COMM_SERIAL_HAVE_HWSERIAL1
      error = true;
#endif // COMM_SERIAL_HAVE_HWSERIAL1
      break;
    case 2:
#ifdef COMM_SERIAL_HAVE_HWSERIAL2
      serialController = &Serial2;
#else // COMM_SERIAL_HAVE_HWSERIAL2
      error = true;
#endif // COMM_SERIAL_HAVE_HWSERIAL2
      break;
    case 3:
#ifdef COMM_SERIAL_HAVE_HWSERIAL3
      serialController = &Serial3;
#else // COMM_SERIAL_HAVE_HWSERIAL3
      error = true;
#endif // COMM_SERIAL_HAVE_HWSERIAL3
      break;
    default:
      error = true;
      break;
    }

    if (error != false)
    {
      break;
    }

#elif defined (SYSTEM_PC_BASED)
    // create windows / linux
#ifdef SYSTEM_WIN_PLATFORM
    {
      // create handler
      char* port = socketConfig.port;
      size_t portLen = (strlen(port) + 1); //Plus null
      wchar_t* wtext = new wchar_t[portLen * sizeof(wchar_t)];
      size_t outSize = 0;
      mbstowcs_s(&outSize, wtext, portLen, port, portLen - 1);
      serialController = CreateFile(wtext,
        GENERIC_READ | GENERIC_WRITE,
        0,
        0,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,
        0);

      if (serialController == INVALID_HANDLE_VALUE)
      {
        break;
      }
      
    }

#else // SYSTEM_WIN_PLATFORM
    // SYSTEM_LIMUX_PLATFORM
    {
      char* port = socketConfig.port;
      // O_RDONLY for read-only, O_WRONLY for write only, O_RDWR for both read/write access
      serialController = open(port, O_RDWR);

      // Check for errors
      if (serialController < 0)
      {
        CONSOLE_LOG("Error %i from open: %s\n", errno, strerror(errno));
        break;
      }
    }

#endif // SYSTEM_WIN_PLATFORM
#endif // SYSTEM_ARDUINO_BASED

    return serialController;
  } while (0);

  CommSerialHandlerFactory::release(serialController);
  return serialController;
}

void CommSerialHandlerFactory::release(SerialHandler& serialController)
{
  do
  {
    if (serialController == COMM_SERIAL_SOCKET_HANDLE_INVALID)
    {
      break;
    }
#ifdef SYSTEM_ARDUINO_BASED
    serialController->end();
#elif defined (SYSTEM_PC_BASED)
#ifdef SYSTEM_WIN_PLATFORM
    {
      EscapeCommFunction(serialController, CLRDTR);
      PurgeComm(serialController, PURGE_TXABORT | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_RXCLEAR);
      CloseHandle(serialController);
    }

#else // SYSTEM_WIN_PLATFORM
    // SYSTEM_LIMUX_PLATFORM
    {
      close(serialController);
    }

#endif // SYSTEM_WIN_PLATFORM


#endif // SYSTEM_ARDUINO_BASED
    serialController = COMM_SERIAL_SOCKET_HANDLE_INVALID;
    return;
  } while (0);

  return;
  
}
#endif // _CONF_COMM_SERIAL_SYSTEM_SOCKET_ENABLED