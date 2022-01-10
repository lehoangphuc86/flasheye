/////////////////////////////////////////////////
// INCLUDE
#include "DisplaySCController.h"
#if (_CONF_DISPLAY_SC_CONTROLLER_ENABLED)
#include "../os_system/SystemCommon.h"
/////////////////////////////////////////////////
// PREPROCESSOR

#ifdef SYSTEM_ESP_PLATFORM
#undef DP_SC_INSTANCE_IDX_MAX
#define DP_SC_INSTANCE_IDX_MAX   2

#define DP_SC_HAVE_HWSERIAL0
#define DP_SC_HAVE_HWSERIAL1
#define DP_SC_HAVE_HWSERIAL2

#if defined(NO_GLOBAL_INSTANCES) || defined(NO_GLOBAL_SERIAL)
HardwareSerial Serial(0);
HardwareSerial Serial1(1);
HardwareSerial Serial2(2);
#endif // defined(NO_GLOBAL_INSTANCES) || defined(NO_GLOBAL_SERIAL)

#else // SYSTEM_ESP_PLATFORM

// Arduino 
#if defined(UBRRH) || defined(UBRR0H)
#define DP_SC_HAVE_HWSERIAL0
#undef DP_SC_INSTANCE_IDX_MAX
#define DP_SC_INSTANCE_IDX_MAX   0
#endif // defined(UBRRH) || defined(UBRR0H)

#if defined(UBRR1H)
#define DP_SC_HAVE_HWSERIAL1
#undef DP_SC_INSTANCE_IDX_MAX
#define DP_SC_INSTANCE_IDX_MAX   1
#endif // defined(UBRR1H)

#if defined(UBRR2H)
#define DP_SC_HAVE_HWSERIAL2
#undef DP_SC_INSTANCE_IDX_MAX
#define DP_SC_INSTANCE_IDX_MAX   2
#endif // defined(UBRR2H)

#if defined(UBRR3H)
#define DP_SC_HAVE_HWSERIAL3
#undef DP_SC_INSTANCE_IDX_MAX
#define DP_SC_INSTANCE_IDX_MAX   3
#endif // defined(UBRR3H)

#endif // SYSTEM_ESP_PLATFORM

//#define DP_SC_CONTROLLER_CONSOLE_DEBUG_ENABLE
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
#ifdef DP_SC_CONTROLLER_CONSOLE_DEBUG_ENABLE
char dpSCControllerLogBuf[SYSTEM_CONSOLE_OUT_BUF_LEN];
#endif // DP_SC_CONTROLLER_CONSOLE_DEBUG_ENABLE
/////////////////////////////////////////////////
// STATIC FUNCTIONS

/////////////////////////////////////////////////
// EXTERN

/////////////////////////////////////////////////
// CLASS IMPLEMENTAION

/* DisplaySCController*/
DisplaySCController::DisplaySCController(void)
  : DisplayController(DisplayDeviceTypeUN::DisplaySerialConsoleDevice)
  , lcd_Controller(NULL)
{

}
DisplaySCController::~DisplaySCController(void)
{

}

bool DisplaySCController::isValid(void)
{
  do
  {
    if (DisplayController::isValid() == false)
    {
      break;
    }

    if (this->lcd_Controller == NULL)
    {
      break;
    }

    return true;
  } while (0);
  return false;
}

int DisplaySCController::start(DisplayDeviceConfig& deviceConfig)
{
  int ret = 0;
  do
  {
    if (this->isValid() != false)
    {
      return 0; //already
    }

    ret = this->createSocket(deviceConfig.spec.serialConsole);
    if (ret != 0)
    {
      break;
    }

    ret = this->setSocketConfig(deviceConfig.spec.serialConsole);
    if (ret != 0)
    {
      break;
    }

    return 0;
  } while (0);
  this->stop();
  return -1;
}

int DisplaySCController::createSocket(DisplaySCDeviceConfigTAG& socketConfig)
{
  this->lcd_Controller = NULL;
  do
  {
    bool error = false;
    switch (socketConfig.instanceIndex)
    {
    case 0:
#ifdef DP_SC_HAVE_HWSERIAL0
      this->lcd_Controller = &Serial;
#else // DP_SC_HAVE_HWSERIAL0
      error = true;
#endif // DP_SC_HAVE_HWSERIAL0
      break;
    case 1:
#ifdef DP_SC_HAVE_HWSERIAL1
      this->lcd_Controller = &Serial1;
#else // DP_SC_HAVE_HWSERIAL1
      error = true;
#endif // DP_SC_HAVE_HWSERIAL1
      break;
    case 2:
#ifdef DP_SC_HAVE_HWSERIAL2
      this->lcd_Controller = &Serial2;
#else // DP_SC_HAVE_HWSERIAL2
      error = true;
#endif // DP_SC_HAVE_HWSERIAL2
      break;
    case 3:
#ifdef DP_SC_HAVE_HWSERIAL3
      this->lcd_Controller = &Serial3;
#else // DP_SC_HAVE_HWSERIAL3
      error = true;
#endif // DP_SC_HAVE_HWSERIAL3
      break;
    default:
      error = true;
      break;
    }

    if (error != false)
    {
      break;
    }
    return 0;
  } while (0);

  this->stop();
  return -1;
}

int DisplaySCController::setSocketConfig(DisplaySCDeviceConfigTAG& socketConfig)
{
  bool error = false;
  do
  {
    if (this->isValid() == false)
    {
      break;
    }

    uint32_t configSet = SERIAL_8N1;
    uint32_t baudrate = 0;
    switch (socketConfig.parityType)
    {
      case (byte)DISPLAY_SET_COMM_PARITY_NONE:
      {
        switch (socketConfig.bitPerByte)
        {
        case DISPLAY_SET_COMM_DATABIT_5:
          configSet = (socketConfig.stopBitNum == DISPLAY_SET_COMM_STOPBIT_1 ? SERIAL_5N1 : SERIAL_5N2);
          break;
        case DISPLAY_SET_COMM_DATABIT_6:
          configSet = (socketConfig.stopBitNum == DISPLAY_SET_COMM_STOPBIT_1 ? SERIAL_6N1 : SERIAL_6N2);
          break;
        case DISPLAY_SET_COMM_DATABIT_7:
          configSet = (socketConfig.stopBitNum == DISPLAY_SET_COMM_STOPBIT_1 ? SERIAL_7N1 : SERIAL_7N2);
          break;
        case DISPLAY_SET_COMM_DATABIT_8:
          configSet = (socketConfig.stopBitNum == DISPLAY_SET_COMM_STOPBIT_1 ? SERIAL_8N1 : SERIAL_8N2);
          break;
        default:
          error = true;
          break;
        }
        break;
      }
      case (byte)DISPLAY_SET_COMM_PARITY_EVEN:
      {
        switch (socketConfig.bitPerByte)
        {
        case DISPLAY_SET_COMM_DATABIT_5:
          configSet = (socketConfig.stopBitNum == DISPLAY_SET_COMM_STOPBIT_1 ? SERIAL_5E1 : SERIAL_5E2);
          break;
        case DISPLAY_SET_COMM_DATABIT_6:
          configSet = (socketConfig.stopBitNum == DISPLAY_SET_COMM_STOPBIT_1 ? SERIAL_6E1 : SERIAL_6E2);
          break;
        case DISPLAY_SET_COMM_DATABIT_7:
          configSet = (socketConfig.stopBitNum == DISPLAY_SET_COMM_STOPBIT_1 ? SERIAL_7E1 : SERIAL_7E2);
          break;
        case DISPLAY_SET_COMM_DATABIT_8:
          configSet = (socketConfig.stopBitNum == DISPLAY_SET_COMM_STOPBIT_1 ? SERIAL_8E1 : SERIAL_8E2);
          break;
        default:
          error = true;
          break;
        }
        break;
      }
      case (byte)DISPLAY_SET_COMM_PARITY_ODD:
      {
        switch (socketConfig.bitPerByte)
        {
        case DISPLAY_SET_COMM_DATABIT_5:
          configSet = (socketConfig.stopBitNum == DISPLAY_SET_COMM_STOPBIT_1 ? SERIAL_5O1 : SERIAL_5O2);
          break;
        case DISPLAY_SET_COMM_DATABIT_6:
          configSet = (socketConfig.stopBitNum == DISPLAY_SET_COMM_STOPBIT_1 ? SERIAL_6O1 : SERIAL_6O2);
          break;
        case DISPLAY_SET_COMM_DATABIT_7:
          configSet = (socketConfig.stopBitNum == DISPLAY_SET_COMM_STOPBIT_1 ? SERIAL_7O1 : SERIAL_7O2);
          break;
        case DISPLAY_SET_COMM_DATABIT_8:
          configSet = (socketConfig.stopBitNum == DISPLAY_SET_COMM_STOPBIT_1 ? SERIAL_8O1 : SERIAL_8O2);
          break;
        default:
          error = true;
          break;
        }
        break;
      }
      default:
        error = true;
        break;
    }

    switch (socketConfig.baudrate)
    {
    case DISPLAY_SET_COMM_BAUD_1200:
      baudrate = 1200;
      break;
    case DISPLAY_SET_COMM_BAUD_2400:
      baudrate = 2400;
      break;
    case DISPLAY_SET_COMM_BAUD_4800:
      baudrate = 4800;
      break;
    case DISPLAY_SET_COMM_BAUD_9600:
      baudrate = 9600;
      break;
    case DISPLAY_SET_COMM_BAUD_14400:
      baudrate = 14400;
      break;
    case DISPLAY_SET_COMM_BAUD_19200:
      baudrate = 19200;
      break;
    case DISPLAY_SET_COMM_BAUD_38400:
      baudrate = 38400;
      break;
    case DISPLAY_SET_COMM_BAUD_57600:
      baudrate = 57600;
      break;
    case DISPLAY_SET_COMM_BAUD_115200:
      baudrate = 115200;
      break;
    default:
      error = true;
      break;
    }

    if (error != false)
    {
      break;
    }

#ifdef SYSTEM_AVR_PLATFORM
    this->lcd_Controller->begin(
      baudrate
      , configSet
    );
#elif defined (SYSTEM_ESP_PLATFORM)
    int8_t pinTx = (socketConfig.pin_TX == DISPLAY_DEVICE_PIN_INVALID ? DISPLAY_DEVICE_PIN_LIB_DEFAULT : socketConfig.pin_TX);
    int8_t pinRx = (socketConfig.pin_RX == DISPLAY_DEVICE_PIN_INVALID ? DISPLAY_DEVICE_PIN_LIB_DEFAULT : socketConfig.pin_RX);
#ifdef DP_SC_CONTROLLER_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(dpSCControllerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[dpsc] set %d %d %d", 1, socketConfig.pin_TX, socketConfig.pin_RX);
    CONSOLE_LOG_BUF(dpSCControllerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[dpsc] set %d %d", 2, baudrate);
    CONSOLE_LOG_BUF(dpSCControllerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[dpsc] set %d %d", 3, configSet);
#endif // DP_SC_CONTROLLER_CONSOLE_DEBUG_ENABLE
    this->lcd_Controller->begin(
      baudrate
      , configSet
      , pinRx
      , pinTx
      , false
      , socketConfig.timeout
    );
#else // defined (SYSTEM_ESP_PLATFORM)
    error = true;
    break;
#endif // SYSTEM_AVR_PLATFORM
    return 0;
  } while (0);

  return -1;
}

void DisplaySCController::stop(void)
{
  if (this->lcd_Controller != NULL)
  {
    this->lcd_Controller->end();
  }

  this->lcd_Controller = NULL;
}

// controlling functions
void DisplaySCController::clear()
{
  // not supported
}

void DisplaySCController::home()
{
  // not supported
}

void DisplaySCController::noDisplay()
{
  // not supported
}

void DisplaySCController::display()
{
  // not supported
}

void DisplaySCController::noBlink()
{
  // not supported
}

void DisplaySCController::blink()
{
  // not supported
}

void DisplaySCController::noCursor()
{
  // not supported
}

void DisplaySCController::cursor()
{
  // not supported
}

void DisplaySCController::scrollDisplayLeft()
{
  // not supported
}

void DisplaySCController::scrollDisplayRight()
{
  // not supported
}

void DisplaySCController::leftToRight()
{
  // not supported
}

void DisplaySCController::rightToLeft()
{
  // not supported
}

void DisplaySCController::autoscroll()
{
  // not supported
}

void DisplaySCController::noAutoscroll()
{
  // not supported
}

void DisplaySCController::setRowOffsets(int row1, int row2, int row3, int row4)
{
  // not supported
}

void DisplaySCController::createChar(uint8_t charId, uint8_t charArray[])
{
  // not supported
}

void DisplaySCController::setCursor(uint8_t col, uint8_t row)
{
  // not supported
}

void DisplaySCController::setBacklight(byte level)
{
  // not supported
}

void DisplaySCController::setBrightness(byte level)
{
  // not supported
}


size_t DisplaySCController::print(const __FlashStringHelper* param)
{
  return this->lcd_Controller->print(param);
}

size_t DisplaySCController::print(const String& param)
{
  return this->lcd_Controller->print(param);
}

size_t DisplaySCController::print(const char param[])
{
  return this->lcd_Controller->print(param);
}

size_t DisplaySCController::print(char  param)
{
  return this->lcd_Controller->print(param);
}

size_t DisplaySCController::print(unsigned char  param1, int param2)
{
  return this->lcd_Controller->print(param1, param2);
}

size_t DisplaySCController::print(int param1, int param2)
{
  return this->lcd_Controller->print(param1, param2);
}

size_t DisplaySCController::print(unsigned int param1, int param2)
{
  return this->lcd_Controller->print(param1, param2);
}

size_t DisplaySCController::print(long param1, int param2)
{
  return this->lcd_Controller->print(param1, param2);
}

size_t DisplaySCController::print(unsigned long param1, int param2)
{
  return this->lcd_Controller->print(param1, param2);
}

size_t DisplaySCController::print(double param1, int param2)
{
  return this->lcd_Controller->print(param1, param2);
}

size_t DisplaySCController::print(const Printable& param1)
{
  return this->lcd_Controller->print(param1);
}

size_t DisplaySCController::println(const __FlashStringHelper* param1)
{
  return this->lcd_Controller->println(param1);
}

size_t DisplaySCController::println(const String& s)
{
  return this->lcd_Controller->println(s);
}

size_t DisplaySCController::println(char* param)
{
  return this->lcd_Controller->println(param);
}

size_t DisplaySCController::println(const char param[])
{
  return this->lcd_Controller->write(param);
}

size_t DisplaySCController::println(char param)
{
  return this->lcd_Controller->println(param);
}

size_t DisplaySCController::println(unsigned char param1, int param2)
{
  return this->lcd_Controller->println(param1, param2);
}

size_t DisplaySCController::println(int param1, int param2 )
{
  return  this->lcd_Controller->println(param1, param2);
}

size_t DisplaySCController::println(unsigned int param1, int param2)
{
  return this->lcd_Controller->println(param1, param2);
}

size_t DisplaySCController::println(long param1, int param2)
{
  return this->lcd_Controller->println(param1, param2);
}

size_t DisplaySCController::println(unsigned long param1, int param2)
{
  return this->lcd_Controller->println(param1, param2);
}

size_t DisplaySCController::println(double param1, int param2)
{
  return this->lcd_Controller->println(param1, param2);
}

size_t DisplaySCController::println(const Printable& param)
{
  return this->lcd_Controller->println(param);
}

size_t DisplaySCController::println(void)
{
  return this->lcd_Controller->println();
}

#endif // _CONF_DISPLAY_SC_CONTROLLER_ENABLED