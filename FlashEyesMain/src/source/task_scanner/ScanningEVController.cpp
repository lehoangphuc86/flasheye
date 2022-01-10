/////////////////////////////////////////////////
// INCLUDE
#include "ScanningEVController.h"
#if (_CONF_SCANNING_EV_CONTROLLER_ENABLED)
#include "os_system/SystemCommon.h"
#include "../settings/SettingManager.h"
#include "../../libs/scanner/EV/EV_ER080.h"
#include "timer_manager/TimerManager.h"
/////////////////////////////////////////////////
// PREPROCESSOR

#ifdef SYSTEM_ESP_PLATFORM
#undef SC_EV_INSTANCE_IDX_MAX
#define SC_EV_INSTANCE_IDX_MAX   2

#define SC_EV_HAVE_HWSERIAL0
#define SC_EV_HAVE_HWSERIAL1
#define SC_EV_HAVE_HWSERIAL2

#if defined(NO_GLOBAL_INSTANCES) || defined(NO_GLOBAL_SERIAL)
HardwareSerial Serial(0);
HardwareSerial Serial1(1);
HardwareSerial Serial2(2);
#endif // defined(NO_GLOBAL_INSTANCES) || defined(NO_GLOBAL_SERIAL)

#else // SYSTEM_ESP_PLATFORM

// Arduino 
#if defined(UBRRH) || defined(UBRR0H)
#define SC_EV_HAVE_HWSERIAL0
#undef SC_EV_INSTANCE_IDX_MAX
#define SC_EV_INSTANCE_IDX_MAX   0
#endif // defined(UBRRH) || defined(UBRR0H)

#if defined(UBRR1H)
#define SC_EV_HAVE_HWSERIAL1
#undef SC_EV_INSTANCE_IDX_MAX
#define SC_EV_INSTANCE_IDX_MAX   1
#endif // defined(UBRR1H)

#if defined(UBRR2H)
#define SC_EV_HAVE_HWSERIAL2
#undef SC_EV_INSTANCE_IDX_MAX
#define SC_EV_INSTANCE_IDX_MAX   2
#endif // defined(UBRR2H)

#if defined(UBRR3H)
#define SC_EV_HAVE_HWSERIAL3
#undef SC_EV_INSTANCE_IDX_MAX
#define SC_EV_INSTANCE_IDX_MAX   3
#endif // defined(UBRR3H)

#endif // SYSTEM_ESP_PLATFORM

//#define SC_EV_CONTROLLER_CONSOLE_DEBUG_ENABLE
/////////////////////////////////////////////////
// DEFINE
//-------------------------irs-----------------------------
#define SCANNING_DEVICE_EV_LED_PIN_MODE               GPIO_INPUT
#define SCANNING_DEVICE_EV_BUZZER_PIN_MODE            GPIO_INPUT_PULLUP
#define SCANNING_DEVICE_EV_LED_ISR_TYPE               ISR_RISING
#define SCANNING_DEVICE_EV_BUZZER_ISR_TYPE            ISR_FALLING
//-------------------------read cmd state-----------------------------
#define SCANNING_DEVICE_EV_READ_CMD_STATE_START       0
#define SCANNING_DEVICE_EV_READ_CMD_STATE_OPENNING    1
#define SCANNING_DEVICE_EV_READ_CMD_STATE_DATA        2
#define SCANNING_DEVICE_EV_READ_CMD_STATE_CLOSING     3
#define SCANNING_DEVICE_EV_READ_CMD_STATE_COMPLETE    4
#define SCANNING_DEVICE_EV_READ_CMD_STATE_ERROR       99
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
#ifdef SC_EV_CONTROLLER_CONSOLE_DEBUG_ENABLE
char scEVControllerLogBuf[SYSTEM_CONSOLE_OUT_BUF_LEN];
#endif // SC_EV_CONTROLLER_CONSOLE_DEBUG_ENABLE
/////////////////////////////////////////////////
// STATIC FUNCTIONS

/////////////////////////////////////////////////
// EXTERN

/////////////////////////////////////////////////
// CLASS IMPLEMENTAION
/* ScanningEVController*/
ScanningEVController::ScanningEVController(void)
  : ScanningController(ScanningDeviceTypeUN::ScanningEVDevice)
  , dev_Comm(NULL)
  , signal_Arg(NULL)
  , signal_Callback(NULL)
  , instance_Indx(SCANNING_CONTROLLER_INSTANCE_INDX_INVALID)
  , comm_Buff(NULL)
  , cmd_Buff(NULL)
  , data_Timeout(0)
{
  ScanningEVController::regInstance(this);
}

ScanningEVController::~ScanningEVController(void)
{

}

bool ScanningEVController::isValid(void)
{
  do
  {
    if (ScanningController::isValid() == false)
    {
      break;
    }

    if (this->dev_Comm == NULL)
    {
      break;
    }

    return true;
  } while (0);
  return false;
}

int ScanningEVController::start(ScanningDeviceConfigTAG& deviceConfig)
{
  int ret = 0;
#ifdef SC_EV_CONTROLLER_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(scEVControllerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[scev] sta %i", 0);
#endif // SC_EV_CONTROLLER_CONSOLE_DEBUG_ENABLE
  do
  {
    if (this->isValid() != false)
    {
      return 0; //already
    }
    this->stop();
    this->data_Timeout = deviceConfig.dataTimeout;
    this->signal_Callback = deviceConfig.cbOnSignal;
    this->signal_Arg = deviceConfig.signalUserArg;
    ret = ScanningEVController::regCallback(this, deviceConfig.spec.evScanner);
    if (ret != 0)
    {
      break;
    }

    ret = this->createSocket(deviceConfig.spec.evScanner.serialConfig);
    if (ret != 0)
    {
      break;
    }

    ret = this->setSocketConfig(deviceConfig.spec.evScanner.serialConfig);
    if (ret != 0)
    {
      break;
    }

    this->comm_Buff = new char[EV_COMM_BUFF_LEN_MAX + 1];
    if (this->comm_Buff == NULL)
    {
      break;
    }

    this->cmd_Buff= new char[EV_SEND_CMD_BUFF_LEN_MAX + 1];
    if (this->cmd_Buff == NULL)
    {
      break;
    }
#ifdef SC_EV_CONTROLLER_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(scEVControllerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[scev] sta %i", 99);
#endif // SC_EV_CONTROLLER_CONSOLE_DEBUG_ENABLE
    return 0;
  } while (0);
  this->stop();
#ifdef SC_EV_CONTROLLER_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(scEVControllerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[scev] sta %i", -99);
#endif // SC_EV_CONTROLLER_CONSOLE_DEBUG_ENABLE
  return -1;
}

int ScanningEVController::createSocket(ScanningEVSerialConfigTAG& socketConfig)
{
  this->dev_Comm = NULL;
  do
  {
    bool error = false;
    switch (socketConfig.instanceIndex)
    {
    case 0:
#ifdef SC_EV_HAVE_HWSERIAL0
      this->dev_Comm = &Serial;
#else // SC_EV_HAVE_HWSERIAL0
      error = true;
#endif // SC_EV_HAVE_HWSERIAL0
      break;
    case 1:
#ifdef SC_EV_HAVE_HWSERIAL1
      this->dev_Comm = &Serial1;
#else // SC_EV_HAVE_HWSERIAL1
      error = true;
#endif // SC_EV_HAVE_HWSERIAL1
      break;
    case 2:
#ifdef SC_EV_HAVE_HWSERIAL2
      this->dev_Comm = &Serial2;
#else // SC_EV_HAVE_HWSERIAL2
      error = true;
#endif // SC_EV_HAVE_HWSERIAL2
      break;
    case 3:
#ifdef SC_EV_HAVE_HWSERIAL3
      this->dev_Comm = &Serial3;
#else // SC_EV_HAVE_HWSERIAL3
      error = true;
#endif // SC_EV_HAVE_HWSERIAL3
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

int ScanningEVController::setSocketConfig(ScanningEVSerialConfigTAG& socketConfig)
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
      case (byte)SCANNING_SET_COMM_PARITY_NONE:
      {
        switch (socketConfig.bitPerByte)
        {
        case SCANNING_SET_COMM_DATABIT_5:
          configSet = (socketConfig.stopBitNum == SCANNING_SET_COMM_STOPBIT_1 ? SERIAL_5N1 : SERIAL_5N2);
          break;
        case SCANNING_SET_COMM_DATABIT_6:
          configSet = (socketConfig.stopBitNum == SCANNING_SET_COMM_STOPBIT_1 ? SERIAL_6N1 : SERIAL_6N2);
          break;
        case SCANNING_SET_COMM_DATABIT_7:
          configSet = (socketConfig.stopBitNum == SCANNING_SET_COMM_STOPBIT_1 ? SERIAL_7N1 : SERIAL_7N2);
          break;
        case SCANNING_SET_COMM_DATABIT_8:
          configSet = (socketConfig.stopBitNum == SCANNING_SET_COMM_STOPBIT_1 ? SERIAL_8N1 : SERIAL_8N2);
          break;
        default:
          error = true;
          break;
        }
        break;
      }
      case (byte)SCANNING_SET_COMM_PARITY_EVEN:
      {
        switch (socketConfig.bitPerByte)
        {
        case SCANNING_SET_COMM_DATABIT_5:
          configSet = (socketConfig.stopBitNum == SCANNING_SET_COMM_STOPBIT_1 ? SERIAL_5E1 : SERIAL_5E2);
          break;
        case SCANNING_SET_COMM_DATABIT_6:
          configSet = (socketConfig.stopBitNum == SCANNING_SET_COMM_STOPBIT_1 ? SERIAL_6E1 : SERIAL_6E2);
          break;
        case SCANNING_SET_COMM_DATABIT_7:
          configSet = (socketConfig.stopBitNum == SCANNING_SET_COMM_STOPBIT_1 ? SERIAL_7E1 : SERIAL_7E2);
          break;
        case SCANNING_SET_COMM_DATABIT_8:
          configSet = (socketConfig.stopBitNum == SCANNING_SET_COMM_STOPBIT_1 ? SERIAL_8E1 : SERIAL_8E2);
          break;
        default:
          error = true;
          break;
        }
        break;
      }
      case (byte)SCANNING_SET_COMM_PARITY_ODD:
      {
        switch (socketConfig.bitPerByte)
        {
        case SCANNING_SET_COMM_DATABIT_5:
          configSet = (socketConfig.stopBitNum == SCANNING_SET_COMM_STOPBIT_1 ? SERIAL_5O1 : SERIAL_5O2);
          break;
        case SCANNING_SET_COMM_DATABIT_6:
          configSet = (socketConfig.stopBitNum == SCANNING_SET_COMM_STOPBIT_1 ? SERIAL_6O1 : SERIAL_6O2);
          break;
        case SCANNING_SET_COMM_DATABIT_7:
          configSet = (socketConfig.stopBitNum == SCANNING_SET_COMM_STOPBIT_1 ? SERIAL_7O1 : SERIAL_7O2);
          break;
        case SCANNING_SET_COMM_DATABIT_8:
          configSet = (socketConfig.stopBitNum == SCANNING_SET_COMM_STOPBIT_1 ? SERIAL_8O1 : SERIAL_8O2);
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
    case SCANNING_SET_COMM_BAUD_1200:
      baudrate = 1200;
      break;
    case SCANNING_SET_COMM_BAUD_2400:
      baudrate = 2400;
      break;
    case SCANNING_SET_COMM_BAUD_4800:
      baudrate = 4800;
      break;
    case SCANNING_SET_COMM_BAUD_9600:
      baudrate = 9600;
      break;
    case SCANNING_SET_COMM_BAUD_14400:
      baudrate = 14400;
      break;
    case SCANNING_SET_COMM_BAUD_19200:
      baudrate = 19200;
      break;
    case SCANNING_SET_COMM_BAUD_38400:
      baudrate = 38400;
      break;
    case SCANNING_SET_COMM_BAUD_57600:
      baudrate = 57600;
      break;
    case SCANNING_SET_COMM_BAUD_115200:
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
    this->dev_Comm->begin(
      baudrate
      , configSet
    );
#elif defined (SYSTEM_ESP_PLATFORM)
    int8_t pinTx = (socketConfig.pin_TX == SCANNING_DEVICE_PIN_INVALID ? SCANNING_DEVICE_PIN_LIB_DEFAULT : socketConfig.pin_TX);
    int8_t pinRx = (socketConfig.pin_RX == SCANNING_DEVICE_PIN_INVALID ? SCANNING_DEVICE_PIN_LIB_DEFAULT : socketConfig.pin_RX);
    this->dev_Comm->begin(
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

void ScanningEVController::stop(void)
{
  if (this->dev_Comm != NULL)
  {
    this->dev_Comm->end();
  }

  this->dev_Comm = NULL;

  if (this->comm_Buff != NULL)
  {
    delete[] this->comm_Buff;
  }
  this->comm_Buff = NULL;

  if (this->cmd_Buff != NULL)
  {
    delete[] this->cmd_Buff;
  }
  this->cmd_Buff = NULL;

  this->data_Timeout = 0;
  this->signal_Callback = NULL;
  this->signal_Arg = NULL;
}


int ScanningEVController::startScan(void)
{
  int ret = 0;
  do
  {
#ifdef SC_EV_CONTROLLER_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(scEVControllerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[scev] scsta %i", 0);
#endif // SC_EV_CONTROLLER_CONSOLE_DEBUG_ENABLE
    ret = this->exeCommand(EV_CMD_OP_START_SCAN);
    if (ret != 0)
    {
      break;
    }
#ifdef SC_EV_CONTROLLER_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(scEVControllerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[scev] scsta %i", 99);
#endif // SC_EV_CONTROLLER_CONSOLE_DEBUG_ENABLE
    return 0;
  } while (0);
#ifdef SC_EV_CONTROLLER_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(scEVControllerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[scev] scsta %i", -99);
#endif // SC_EV_CONTROLLER_CONSOLE_DEBUG_ENABLE
  return -1;
}

int ScanningEVController::readResult(ScanningDeviceResultTAG& scanResult)
{
  int ret = 0;
  scanResult.errorId = SCANNING_ERR_NONE;
#ifdef SC_EV_CONTROLLER_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(scEVControllerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[scev] rre %i", 0);
#endif // SC_EV_CONTROLLER_CONSOLE_DEBUG_ENABLE
  do
  {
    ret = this->readCode(scanResult.code);
    if (ret != 0)
    {
      break;
    }
#ifdef SC_EV_CONTROLLER_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(scEVControllerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[scev] rre %i %s", 99, scanResult.code.code);
#endif // SC_EV_CONTROLLER_CONSOLE_DEBUG_ENABLE
    return 0;
  } while (0);
  scanResult.errorId = SCANNING_ERR_DEVICE_ERROR;
#ifdef SC_EV_CONTROLLER_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(scEVControllerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[scev] rre %i", -99);
#endif // SC_EV_CONTROLLER_CONSOLE_DEBUG_ENABLE
  return -1;
}

void ScanningEVController::stopScan(void)
{
  int ret = 0;
  do
  {
#ifdef SC_EV_CONTROLLER_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(scEVControllerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[scev] sto %i", 0);
#endif // SC_EV_CONTROLLER_CONSOLE_DEBUG_ENABLE
    ret = this->exeCommand(EV_CMD_OP_STOP_SCAN);
    if (ret != 0)
    {
      break;
    }
#ifdef SC_EV_CONTROLLER_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(scEVControllerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[scev] sto %i", 99);
#endif // SC_EV_CONTROLLER_CONSOLE_DEBUG_ENABLE
    return;
  } while (0);
#ifdef SC_EV_CONTROLLER_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(scEVControllerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[scev] sto %i", -99);
#endif // SC_EV_CONTROLLER_CONSOLE_DEBUG_ENABLE
  return;
}

int ScanningEVController::setting(ScanningDeviceSettingTAG& setting, ScanningDeviceSettingResultTAG& outResult)
{
  int ret = 0;
  ScanningSettingIdSet_t idSet = setting.settingIdSet0;
  ScanningSettingIdSet_t& errorSet = outResult.errorSet0;
  ScannerSettingCollection& scannerSetting = SettingManager::getInstance().scanner();
  errorSet = 0;
#ifdef SC_EV_CONTROLLER_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(scEVControllerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[scev] set %i %ld", 0, (long)idSet);
  CONSOLE_LOG_BUF(scEVControllerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[scev] set %i %ld", 1, (long)(idSet << 32));
#endif // SC_EV_CONTROLLER_CONSOLE_DEBUG_ENABLE
  do
  {
    ret = this->exeCommand(EV_CMD_OP_START_SETTING);
    if (ret != 0)
    {
      errorSet = SCANNING_SETTING_ID_SET_ALL;
      return -1;
    }

    if (SYSTEM_BITREAD(idSet, SCANNING_SET_ID_COM_BAUDRATE) != 0)
    {
      ret = this->setCommBaudrate(scannerSetting.commBaudrate());
      if (ret != 0)
      {
        SYSTEM_BITSET(errorSet, SCANNING_SET_ID_COM_BAUDRATE);
        break;
      }
    }

    if (SYSTEM_BITREAD(idSet, SCANNING_SET_ID_COM_PARITY) != 0)
    {
      ret = this->setCommParity(scannerSetting.commParity());
      if (ret != 0)
      {
        SYSTEM_BITSET(errorSet, SCANNING_SET_ID_COM_BAUDRATE);
        break;
      }
    }

    if (SYSTEM_BITREAD(idSet, SCANNING_SET_ID_COM_STOPBIT) != 0)
    {
      ret = this->setCommStopbit(scannerSetting.commStopbit());
      if (ret != 0)
      {
        SYSTEM_BITSET(errorSet, SCANNING_SET_ID_COM_BAUDRATE);
        break;
      }
    }

    if (SYSTEM_BITREAD(idSet, SCANNING_SET_ID_COM_DATABIT) != 0)
    {
      ret = this->setCommDatabit(scannerSetting.commDatabit());
      if (ret != 0)
      {
        SYSTEM_BITSET(errorSet, SCANNING_SET_ID_COM_BAUDRATE);
        break;
      }
    }

    if (SYSTEM_BITREAD(idSet, SCANNING_SET_ID_COM_FLOW_CTRL) != 0)
    {
      ret = this->setCommFlowCtrl(scannerSetting.commFlowCtrl());
      if (ret != 0)
      {
        SYSTEM_BITSET(errorSet, SCANNING_SET_ID_COM_BAUDRATE);
        break;
      }
    }

    if (SYSTEM_BITREAD(idSet, SCANNING_SET_ID_CHARACTER) != 0)
    {
      ret = this->setCharacterFormat(scannerSetting.characterFormat());
      if (ret != 0)
      {
        SYSTEM_BITSET(errorSet, SCANNING_SET_ID_COM_BAUDRATE);
        break;
      }
    }

    if (SYSTEM_BITREAD(idSet, SCANNING_SET_ID_READ_STATE) != 0)
    {
      ret = this->setReadState(scannerSetting.readState());
      if (ret != 0)
      {
        SYSTEM_BITSET(errorSet, SCANNING_SET_ID_COM_BAUDRATE);
        break;
      }
    }

    if (SYSTEM_BITREAD(idSet, SCANNING_SET_ID_AUTO_SCAN) != 0)
    {
      ret = this->setAutoScan(scannerSetting.autoScan());
      if (ret != 0)
      {
        SYSTEM_BITSET(errorSet, SCANNING_SET_ID_COM_BAUDRATE);
        break;
      }
    }

    if (SYSTEM_BITREAD(idSet, SCANNING_SET_ID_CONT_SCAN) != 0)
    {
      ret = this->setContScan(scannerSetting.contScan());
      if (ret != 0)
      {
        SYSTEM_BITSET(errorSet, SCANNING_SET_ID_COM_BAUDRATE);
        break;
      }
    }

    if (SYSTEM_BITREAD(idSet, SCANNING_SET_ID_SECURITY_LEVEL) != 0)
    {
      ret = this->setSecurityLevel(scannerSetting.securityLevel());
      if (ret != 0)
      {
        SYSTEM_BITSET(errorSet, SCANNING_SET_ID_COM_BAUDRATE);
        break;
      }
    }

    if (SYSTEM_BITREAD(idSet, SCANNING_SET_ID_CODE_PREFIX) != 0)
    {
      ret = this->setEnablePrefix(scannerSetting.enablePrefix());
      if (ret != 0)
      {
        SYSTEM_BITSET(errorSet, SCANNING_SET_ID_COM_BAUDRATE);
        break;
      }
    }

    if (SYSTEM_BITREAD(idSet, SCANNING_SET_ID_CODE_PREFIX_MOD) != 0)
    {
      ret = this->setCustomPrefix(scannerSetting.customPrefix());
      if (ret != 0)
      {
        SYSTEM_BITSET(errorSet, SCANNING_SET_ID_COM_BAUDRATE);
        break;
      }
    }

    if (SYSTEM_BITREAD(idSet, SCANNING_SET_ID_CODE_SUFFIX_MOD) != 0)
    {
      ret = this->setCustomSuffix(scannerSetting.customSuffix());
      if (ret != 0)
      {
        SYSTEM_BITSET(errorSet, SCANNING_SET_ID_COM_BAUDRATE);
        break;
      }
    }

    if (SYSTEM_BITREAD(idSet, SCANNING_SET_ID_CODE_CUS_FORMAT) != 0)
    {
      ret = this->setCustomCodeFormat(scannerSetting.customCodeFormat());
      if (ret != 0)
      {
        SYSTEM_BITSET(errorSet, SCANNING_SET_ID_COM_BAUDRATE);
        break;
      }
    }

    if (SYSTEM_BITREAD(idSet, SCANNING_SET_ID_MESS_TER) != 0)
    {
      ret = this->setMessTerminator(scannerSetting.messTerminator());
      if (ret != 0)
      {
        SYSTEM_BITSET(errorSet, SCANNING_SET_ID_COM_BAUDRATE);
        break;
      }
    }

    ret = this->exeCommand(EV_CMD_OP_STOP_SETTING);
    if (ret != 0)
    {
      break;
    }
#ifdef SC_EV_CONTROLLER_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(scEVControllerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[scev] set %i", 99);
#endif // SC_EV_CONTROLLER_CONSOLE_DEBUG_ENABLE
    return 0;
  } while (0);
  this->exeCommand(EV_CMD_OP_STOP_SETTING);
#ifdef SC_EV_CONTROLLER_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(scEVControllerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[scev] set %i", -99);
#endif // SC_EV_CONTROLLER_CONSOLE_DEBUG_ENABLE
  return -1;
}

int ScanningEVController::exeCommand(ScanningCmdCode_t cmdCode, uint16_t timeout)
{
  int ret = 0;
  ScanningCmdCode_t resCode = 0;
  do
  {
    ret = this->writeCommand(cmdCode);
    if (ret != 0)
    {
      break;
    }

    ret = this->readCommand(resCode);
    if ( (ret != 0)
      || (resCode != cmdCode)
      )
    {
      break;
    }
    return 0;
  } while (0);
  this->flushCommBuff();
  return -1;
}

int ScanningEVController::readCode(ScanningBarCodeTAG& barCode, uint16_t timeout)
{
  int readLen = 0;
  uint8_t readCh = 0;
  bool stop = false;
  bool error = false;
  TimePoint_t expTime = TimerManager::getInstance().now() + timeout * TIMER_DEVICE_TIMER_MS_2_UNIT;
  barCode.codeLen = 0;
  barCode.code[0] = '\0';
  barCode.type = 0;
  do
  {
    while (!stop)
    {
      if (readLen >= EV_COMM_BUFF_LEN_MAX)
      {
        error = true;
        break;
      }

      // check timeout
      if ((timeout > 0)
        && (TimerManager::getInstance().now() >= expTime)
        )
      {
//#ifdef SC_EV_CONTROLLER_CONSOLE_DEBUG_ENABLE
//        CONSOLE_LOG_BUF(scEVControllerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[scev] rco %d", -1);
//#endif // SC_EV_CONTROLLER_CONSOLE_DEBUG_ENABLE
        error = true;
        break;
      }

      if (this->dev_Comm->available() <= 0)
      {
        continue;
      }

      readCh = this->dev_Comm->read();
//#ifdef SC_EV_CONTROLLER_CONSOLE_DEBUG_ENABLE
//      CONSOLE_LOG_BUF(scEVControllerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[scev] rco %d %c", 2, readCh);
//#endif // SC_EV_CONTROLLER_CONSOLE_DEBUG_ENABLE
      if (readCh == EV_CODE_TERMINATOR_CHARACTER)
      {
        stop = true;
        continue;
      }

      barCode.code[readLen] = readCh;
      readLen++;
    }

    if (error != 0)
    {
      break;
    }
//#ifdef SC_EV_CONTROLLER_CONSOLE_DEBUG_ENABLE
//    CONSOLE_LOG_BUF(scEVControllerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[scev] rco %d %d", 99, readLen);
//#endif // SC_EV_CONTROLLER_CONSOLE_DEBUG_ENABLE
    barCode.code[readLen] = '\0';
    barCode.codeLen = readLen;
    return 0;
  } while (0);
//#ifdef SC_EV_CONTROLLER_CONSOLE_DEBUG_ENABLE
//  CONSOLE_LOG_BUF(scEVControllerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[scev] rco %d", -99);
//#endif // SC_EV_CONTROLLER_CONSOLE_DEBUG_ENABLE
  barCode.codeLen = 0;
  barCode.code[0] = '\0';
  barCode.type = 0;
  return -1;
  
  
}

int ScanningEVController::readCommand(ScanningCmdCode_t& cmdCode, uint16_t timeout)
{
  cmdCode = 0;
  int ret = 0;
  int8_t errorCoeff = 1;
  do
  {
    ret = this->readRawCommand(timeout);
    if (ret != 0)
    {
      break;
    }
    // set null terminator
    this->comm_Buff[EV_CMD_RES_CODE_LEN + EV_CMD_PREFIX_CHARACTER_NUM + EV_CMD_CODE_LEN] = '\0';

//#ifdef SC_EV_CONTROLLER_CONSOLE_DEBUG_ENABLE
//    CONSOLE_LOG_BUF(scEVControllerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[scev] r %d %s", 2, this->comm_Buff);
//#endif // SC_EV_CONTROLLER_CONSOLE_DEBUG_ENABLE

    // expected buff is "!Axxxx;" or "?Axxxx;"
    // analysic
    if (this->comm_Buff[0] == EV_CMD_RES_CODE_FAIL) // ?
    {
      errorCoeff = -1; // return negative code if it failed
    }
    else if (this->comm_Buff[0] == EV_CMD_RES_CODE_SUCCESS) // !
    {
      errorCoeff = 1; // return positive code if it succeeded
    }
    else
    {
      break;
    }

    // expected buff is "!Axxxx\0" or "?Axxxx\0"
    cmdCode = atoi(this->comm_Buff + (EV_CMD_RES_CODE_LEN + EV_CMD_PREFIX_CHARACTER_NUM));
    cmdCode *= errorCoeff;
//#ifdef SC_EV_CONTROLLER_CONSOLE_DEBUG_ENABLE
//    CONSOLE_LOG_BUF(scEVControllerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[scev] r %d %d", 99, cmdCode);
//#endif // SC_EV_CONTROLLER_CONSOLE_DEBUG_ENABLE
    return 0;
  } while (0);
//#ifdef SC_EV_CONTROLLER_CONSOLE_DEBUG_ENABLE
//  CONSOLE_LOG_BUF(scEVControllerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[scev] r %d", -99);
//#endif // SC_EV_CONTROLLER_CONSOLE_DEBUG_ENABLE
  return -1;
}

int ScanningEVController::readRawCommand(uint16_t timeout)
{
  int readLen = 0;
  uint8_t readCh = 0;
  bool error = false;
  byte closing = 0;
  TimePoint_t expTime = TimerManager::getInstance().now() + timeout * TIMER_DEVICE_TIMER_MS_2_UNIT;
  do
  {
    while (1)
    {
      // check timeout
      if ( (timeout >0) 
        && (TimerManager::getInstance().now() >= expTime)
        )
      {
//#ifdef SC_EV_CONTROLLER_CONSOLE_DEBUG_ENABLE
//        CONSOLE_LOG_BUF(scEVControllerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[scev] rr %d", -1);
//#endif // SC_EV_CONTROLLER_CONSOLE_DEBUG_ENABLE
        error = true;
        break;
      }

      if (this->dev_Comm->available() <= 0)
      {
        continue;
      }

      readCh = this->dev_Comm->read();
//#ifdef SC_EV_CONTROLLER_CONSOLE_DEBUG_ENABLE
//      CONSOLE_LOG_BUF(scEVControllerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[scev] rr %d %c", 2, readCh);
//#endif // SC_EV_CONTROLLER_CONSOLE_DEBUG_ENABLE
      switch (readCh)
      {
        case EV_CMD_OPEN_CHARACTER:
          readLen = 0;
          break;
        case EV_CMD_CLOSE_CHARACTER:
          closing++;
          break;
        default:
          this->comm_Buff[readLen] = readCh;
          readLen++;
          break;
      }

      if (closing >= EV_CMD_CLOSE_CHARACTER_NUM)
      {
#ifdef SC_EV_CONTROLLER_CONSOLE_DEBUG_ENABLE
        CONSOLE_LOG_BUF(scEVControllerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[scev] rr %d", 5);
#endif // SC_EV_CONTROLLER_CONSOLE_DEBUG_ENABLE
        break;
      }

      // check len
      if (readLen >= EV_COMM_BUFF_LEN_MAX)
      {
//#ifdef SC_EV_CONTROLLER_CONSOLE_DEBUG_ENABLE
//        CONSOLE_LOG_BUF(scEVControllerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[scev] rr %d %d", -6, readLen);
//#endif // SC_EV_CONTROLLER_CONSOLE_DEBUG_ENABLE
        error = true;
        break;
      }
    }

    if (error != false)
    {
      break;
    }

    if (readLen < EV_REV_CMD_BODY_LEN)
    {
//#ifdef SC_EV_CONTROLLER_CONSOLE_DEBUG_ENABLE
//      CONSOLE_LOG_BUF(scEVControllerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[scev] rr %d %d", -9, readLen);
//#endif // SC_EV_CONTROLLER_CONSOLE_DEBUG_ENABLE
      break;
    }
//#ifdef SC_EV_CONTROLLER_CONSOLE_DEBUG_ENABLE
//    CONSOLE_LOG_BUF(scEVControllerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[scev] rr %d", 99);
//#endif // SC_EV_CONTROLLER_CONSOLE_DEBUG_ENABLE
    return 0;
  } while (0);
  this->flushCommBuff();
//#ifdef SC_EV_CONTROLLER_CONSOLE_DEBUG_ENABLE
//  CONSOLE_LOG_BUF(scEVControllerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[scev] rr %d", -99);
//#endif // SC_EV_CONTROLLER_CONSOLE_DEBUG_ENABLE
  return -1;
}

int ScanningEVController::writeString(char* buff, DataSize_t buffLen)
{
  DataSize_t wk_idx = 0;
  int ret = 0;
  do
  {
    for (wk_idx = 0; wk_idx < buffLen; wk_idx++)
    {
      ret = this->writeChar(buff[wk_idx]);
      if (ret != 0)
      {
        break;
      }
    }

    if (wk_idx < buffLen)
    {
      break;
    }

    return 0;
  } while (0);
  return -1;
}

int ScanningEVController::writeChar(char ch)
{
  ScanningCmdCode_t cmd0 = EV_CMD_SET_PARAM_CODE_0;
  ScanningCmdCode_t cmd1 = EV_CMD_SET_PARAM_CODE_0;
  char hex0 = '0';
  char hex1 = '0';
  int ret = 0;
  do
  {
    this->char2Hex(ch, hex0, hex1);
    cmd0 = this->hex2Cmd(hex0);
    cmd1 = this->hex2Cmd(hex1);
    ret = this->writeCommand(cmd0);
    if (ret != 0)
    {
      break;
    }

    ret = this->writeCommand(cmd1);
    if (ret != 0)
    {
      break;
    }

    return 0;
  } while(0);
  return -1;
}

void ScanningEVController::char2Hex(char inChar, char& hex0, char& hex1)
{
  int ret = 0;
  char hexChar[3];
  SYSTEM_PRINT_BUF(hexChar, 3, "%02x", (int)inChar);
  hex0 = hexChar[0];
  hex1 = hexChar[1];
}

ScanningCmdCode_t ScanningEVController::hex2Cmd(char inHex)
{
  ScanningCmdCode_t outCmdCode = EV_CMD_SET_PARAM_CODE_F;
  switch (inHex)
  {
  case '0':
    outCmdCode = EV_CMD_SET_PARAM_CODE_0;
    break;
  case '1':
    outCmdCode = EV_CMD_SET_PARAM_CODE_1;
    break;
  case '2':
    outCmdCode = EV_CMD_SET_PARAM_CODE_2;
    break;
  case '3':
    outCmdCode = EV_CMD_SET_PARAM_CODE_3;
    break;
  case '4':
    outCmdCode = EV_CMD_SET_PARAM_CODE_4;
    break;
  case '5':
    outCmdCode = EV_CMD_SET_PARAM_CODE_5;
    break;
  case '6':
    outCmdCode = EV_CMD_SET_PARAM_CODE_6;
    break;
  case '7':
    outCmdCode = EV_CMD_SET_PARAM_CODE_7;
    break;
  case '8':
    outCmdCode = EV_CMD_SET_PARAM_CODE_8;
    break;
  case '9':
    outCmdCode = EV_CMD_SET_PARAM_CODE_9;
    break;
  case 'a':
  case 'A':
    outCmdCode = EV_CMD_SET_PARAM_CODE_A;
    break;
  case 'b':
  case 'B':
    outCmdCode = EV_CMD_SET_PARAM_CODE_B;
    break;
  case 'c':
  case 'C':
    outCmdCode = EV_CMD_SET_PARAM_CODE_C;
    break;
  case 'd':
  case 'D':
    outCmdCode = EV_CMD_SET_PARAM_CODE_D;
    break;
  case 'e':
  case 'E':
    outCmdCode = EV_CMD_SET_PARAM_CODE_E;
    break;
  default:
    outCmdCode = EV_CMD_SET_PARAM_CODE_F;
    break;
  }
  return outCmdCode;
}

void ScanningEVController::flushCommBuff(void)
{
  uint8_t readCh = 0;
  while (this->dev_Comm->available() > 0)
  {
    readCh = this->dev_Comm->read();
    if (readCh == EV_CMD_OPEN_CHARACTER)
    {
      break;
    }
  }
}

int ScanningEVController::writeCommand(ScanningCmdCode_t cmdCode, uint16_t timeout)
{
  size_t sentLen = 0;
  size_t cmdLen = 0;
  do
  {
//#ifdef SC_EV_CONTROLLER_CONSOLE_DEBUG_ENABLE
//    CONSOLE_LOG_BUF(scEVControllerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[scev] w %d", 0);
//#endif // SC_EV_CONTROLLER_CONSOLE_DEBUG_ENABLE
    cmdLen = SYSTEM_PRINT_BUF(
      this->cmd_Buff
      , EV_SEND_CMD_BUFF_LEN_MAX
      , EV_CMD_PRINT_FORMAT
      , EV_CMD_OPEN_STR
      , EV_CMD_PREFIX_CHARACTER
      , cmdCode
      , EV_CMD_POSTFIX_CHARACTER
      , EV_CMD_CLOSE_STR
    );
//#ifdef SC_EV_CONTROLLER_CONSOLE_DEBUG_ENABLE
//    CONSOLE_LOG_BUF(scEVControllerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[scev] w %s", this->cmd_Buff);
//#endif // SC_EV_CONTROLLER_CONSOLE_DEBUG_ENABLE
    if ((cmdLen <= 0)
      || (cmdLen >= EV_SEND_CMD_BUFF_LEN_MAX)
      )
    {
      break;
    }
//#ifdef SC_EV_CONTROLLER_CONSOLE_DEBUG_ENABLE
//    CONSOLE_LOG_BUF(scEVControllerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[scev] w %d", 2);
//#endif // SC_EV_CONTROLLER_CONSOLE_DEBUG_ENABLE
    bool stop = false;
    TimePoint_t expTime = TimerManager::getInstance().now() + timeout*TIMER_DEVICE_TIMER_MS_2_UNIT;
    while (!stop)
    {
      sentLen += this->dev_Comm->write(this->cmd_Buff + sentLen, cmdLen);
      if (sentLen >= cmdLen)
      {
        stop = true;
        continue;
      }

      if (timeout == 0)
      {
        continue;
      }

      if (TimerManager::getInstance().now() >= expTime)
      {
        stop = true;
        continue;
      }
    }

//#ifdef SC_EV_CONTROLLER_CONSOLE_DEBUG_ENABLE
//    CONSOLE_LOG_BUF(scEVControllerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[scev] w %d %d %d", 98, sentLen, cmdLen);
//#endif // SC_EV_CONTROLLER_CONSOLE_DEBUG_ENABLE
    if (sentLen != cmdLen)
    {
      break;
    }
//#ifdef SC_EV_CONTROLLER_CONSOLE_DEBUG_ENABLE
//    CONSOLE_LOG_BUF(scEVControllerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[scev] w %d", 99);
//#endif // SC_EV_CONTROLLER_CONSOLE_DEBUG_ENABLE
    return 0;
  } while (0);
//#ifdef SC_EV_CONTROLLER_CONSOLE_DEBUG_ENABLE
//  CONSOLE_LOG_BUF(scEVControllerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[scev] w %d", -99);
//#endif // SC_EV_CONTROLLER_CONSOLE_DEBUG_ENABLE
  return -1;
}

int ScanningEVController::setCommBaudrate(byte baudrate)
{
  ScanningCmdCode_t cmdCode = EV_CMD_SET_INVALID;
  do
  {
    switch (baudrate)
    {
      case SCANNING_SET_COMM_BAUD_1200:
        cmdCode = EV_CMD_SET_COMM_BAUD_1200;
        break;
      case SCANNING_SET_COMM_BAUD_2400:
        cmdCode = EV_CMD_SET_COMM_BAUD_2400;
        break;
      case SCANNING_SET_COMM_BAUD_4800:
        cmdCode = EV_CMD_SET_COMM_BAUD_4800;
        break;
      case SCANNING_SET_COMM_BAUD_9600:
        cmdCode = EV_CMD_SET_COMM_BAUD_9600;
        break;
      case SCANNING_SET_COMM_BAUD_14400:
        cmdCode = EV_CMD_SET_COMM_BAUD_14400;
        break;
      case SCANNING_SET_COMM_BAUD_19200:
        cmdCode = EV_CMD_SET_COMM_BAUD_19200;
        break;
      case SCANNING_SET_COMM_BAUD_38400:
        cmdCode = EV_CMD_SET_COMM_BAUD_38400;
        break;
      case SCANNING_SET_COMM_BAUD_57600:
        cmdCode = EV_CMD_SET_COMM_BAUD_57600;
        break;
      case SCANNING_SET_COMM_BAUD_115200:
        cmdCode = EV_CMD_SET_COMM_BAUD_115200;
        break;
      default:
        break;
    }

    if (cmdCode == EV_CMD_SET_INVALID)
    {
      break;
    }

    return this->exeCommand(cmdCode);
  } while (0);
  return -1;
}

int ScanningEVController::setCommParity(byte parity)
{
  ScanningCmdCode_t cmdCode = EV_CMD_SET_INVALID;
  do
  {
    switch (parity)
    {
    case SCANNING_SET_COMM_PARITY_NONE:
      cmdCode = EV_CMD_SET_COMM_PARITY_NONE;
      break;
    case SCANNING_SET_COMM_PARITY_ODD:
      cmdCode = EV_CMD_SET_COMM_PARITY_ODD;
      break;
    case SCANNING_SET_COMM_PARITY_EVEN:
      cmdCode = EV_CMD_SET_COMM_PARITY_EVEN;
      break;
    default:
      break;
    }

    if (cmdCode == EV_CMD_SET_INVALID)
    {
      break;
    }

    return this->exeCommand(cmdCode);
  } while (0);
  return -1;
}

int ScanningEVController::setCommStopbit(byte stopbit)
{
  ScanningCmdCode_t cmdCode = EV_CMD_SET_INVALID;
  do
  {
    switch (stopbit)
    {
    case SCANNING_SET_COMM_STOPBIT_1:
      cmdCode = EV_CMD_SET_COMM_STOPBIT_1;
      break;
    case SCANNING_SET_COMM_STOPBIT_2:
      cmdCode = EV_CMD_SET_COMM_STOPBIT_2;
      break;
    default:
      break;
    }

    if (cmdCode == EV_CMD_SET_INVALID)
    {
      break;
    }

    return this->exeCommand(cmdCode);
  } while (0);
  return -1;
}

int ScanningEVController::setCommDatabit(byte databit)
{
  ScanningCmdCode_t cmdCode = EV_CMD_SET_INVALID;
  do
  {
    switch (databit)
    {
    case SCANNING_SET_COMM_DATABIT_8:
      cmdCode = EV_CMD_SET_COMM_DATABIT_8;
      break;
    case SCANNING_SET_COMM_DATABIT_7:
      cmdCode = EV_CMD_SET_COMM_DATABIT_7;
      break;
    default:
      break;
    }

    if (cmdCode == EV_CMD_SET_INVALID)
    {
      break;
    }

    return this->exeCommand(cmdCode);
  } while (0);
  return -1;
}

int ScanningEVController::setCommFlowCtrl(byte flowCtrl)
{
  ScanningCmdCode_t cmdCode = EV_CMD_SET_INVALID;
  do
  {
    switch (flowCtrl)
    {
    case SCANNING_SET_COMM_FLOW_CTRL_NONE:
      cmdCode = EV_CMD_SET_COMM_FLOW_CTRL_NONE;
      break;
    case SCANNING_SET_COMM_FLOW_CTRL_RTS_NO:
      cmdCode = EV_CMD_SET_COMM_FLOW_CTRL_RTS_NO;
      break;
    case SCANNING_SET_COMM_FLOW_CTRL_CTS_NO:
      cmdCode = EV_CMD_SET_COMM_FLOW_CTRL_CTS_NO;
      break;
    case SCANNING_SET_COMM_FLOW_CTRL_CTS:
      cmdCode = EV_CMD_SET_COMM_FLOW_CTRL_CTS;
      break;
    default:
      break;
    }

    if (cmdCode == EV_CMD_SET_INVALID)
    {
      break;
    }

    return this->exeCommand(cmdCode);
  } while (0);
  return -1;
}

int ScanningEVController::setCharacterFormat(byte format)
{
  ScanningCmdCode_t cmdCode = EV_CMD_SET_INVALID;
  do
  {
    switch (format)
    {
    case SCANNING_SET_CHARACTER_NO_CONVERT:
      cmdCode = EV_CMD_SET_CHARACTER_NO_CONVERT;
      break;
    case SCANNING_SET_CHARACTER_TO_UPPER:
      cmdCode = EV_CMD_SET_CHARACTER_TO_UPPER;
      break;
    case SCANNING_SET_CHARACTER_TO_LOWER:
      cmdCode = EV_CMD_SET_CHARACTER_TO_LOWER;
      break;
    case SCANNING_SET_CHARACTER_REVERSE:
      cmdCode = EV_CMD_SET_CHARACTER_REVERSE;
      break;
    default:
      break;
    }

    if (cmdCode == EV_CMD_SET_INVALID)
    {
      break;
    }

    return this->exeCommand(cmdCode);
  } while (0);
  return -1;
}

int ScanningEVController::setReadState(byte readState)
{
  ScanningCmdCode_t cmdCode = EV_CMD_SET_READ_STATE_DISABLE;
  do
  {
    if (readState != 0)
    {
      cmdCode = EV_CMD_SET_READ_STATE_ENABLE;
    }

    return this->exeCommand(cmdCode);
  } while (0);
  return -1;
}

int ScanningEVController::setAutoScan(byte flag)
{
  ScanningCmdCode_t cmdCode = EV_CMD_OP_START_SCAN; // press button to stop auto scanning
  do
  {
    if (flag != 0)
    {
      cmdCode = EV_CMD_SET_AUTO_SCAN_ENABLE;
    }

    return this->exeCommand(cmdCode);
  } while (0);
  return -1;
}

int ScanningEVController::setContScan(byte flag)
{
  ScanningCmdCode_t cmdCode = EV_CMD_OP_START_SCAN; // press button to stop continue scanning @@need to confirm
  do
  {
    if (flag != 0)
    {
      cmdCode = EV_CMD_SET_CONT_SCAN_ENABLE;
    }

    return this->exeCommand(cmdCode);
  } while (0);
  return -1;
}

int ScanningEVController::setSecurityLevel(byte level)
{
  ScanningCmdCode_t cmdCode = EV_CMD_SET_INVALID;
  do
  {
    switch (level)
    {
    case SCANNING_SET_SECURITY_LEVEL_1:
      cmdCode = EV_CMD_SET_SECURITY_LEVEL_1;
      break;
    case SCANNING_SET_SECURITY_LEVEL_2:
      cmdCode = EV_CMD_SET_SECURITY_LEVEL_2;
      break;
    case SCANNING_SET_SECURITY_LEVEL_3:
      cmdCode = EV_CMD_SET_SECURITY_LEVEL_3;
      break;
    case SCANNING_SET_SECURITY_LEVEL_4:
      cmdCode = EV_CMD_SET_SECURITY_LEVEL_4;
      break;
    default:
      break;
    }

    if (cmdCode == EV_CMD_SET_INVALID)
    {
      break;
    }

    return this->exeCommand(cmdCode);
  } while (0);
  return -1;
}

int ScanningEVController::setEnablePrefix(byte flag)
{
  ScanningCmdCode_t cmdCode = EV_CMD_SET_CODE_PREFIX_DISABLE;
  do
  {
    if (flag != 0)
    {
      cmdCode = EV_CMD_SET_CODE_PREFIX_ENABLE;
    }

    return this->exeCommand(cmdCode);
  } while (0);
  return -1;
}

int ScanningEVController::setCustomPrefix(char* prefix)
{
  int ret = 0;
  do
  {
    if ((prefix == NULL)
      || (strlen(prefix) <= 0)
      )
    {
      return this->exeCommand(EV_CMD_SET_CODE_PREFIX_MOD_HIDE);
    }

    ret = this->exeCommand(EV_CMD_SET_CODE_PREFIX_MOD_SHOW);
    if (ret != 0)
    {
      break;
    }

    ret = this->writeString(prefix, strlen(prefix));
    if (ret != 0)
    {
      break;
    }

    ret = this->exeCommand(EV_CMD_SET_PARAM_CODE_SAVE);
    if (ret != 0)
    {
      break;
    }

    return 0;
  } while (0);
  this->exeCommand(EV_CMD_SET_PARAM_CODE_RESET);
  return -1;
}

int ScanningEVController::setCustomSuffix(char* suffix)
{
  int ret = 0;
  do
  {
    if ((suffix == NULL)
      || (strlen(suffix) <= 0)
      )
    {
      return this->exeCommand(EV_CMD_SET_CODE_SUFFIX_MOD_HIDE);
    }

    ret = this->exeCommand(EV_CMD_SET_CODE_SUFFIX_MOD_SHOW);
    if (ret != 0)
    {
      break;
    }

    ret = this->writeString(suffix, strlen(suffix));
    if (ret != 0)
    {
      break;
    }

    ret = this->exeCommand(EV_CMD_SET_PARAM_CODE_SAVE);
    if (ret != 0)
    {
      break;
    }

    return 0;
  } while (0);
  this->exeCommand(EV_CMD_SET_PARAM_CODE_RESET);
  return -1;
}

int ScanningEVController::setCustomCodeFormat(byte format)
{
  ScanningCmdCode_t cmdCode = EV_CMD_SET_INVALID;
  do
  {
    switch (format)
    {
    case SCANNING_SET_CODE_CUS_FORMAT_1:
      cmdCode = EV_CMD_SET_CODE_CUS_FORMAT_1;
      break;
    case SCANNING_SET_CODE_CUS_FORMAT_2:
      cmdCode = EV_CMD_SET_CODE_CUS_FORMAT_2;
      break;
    default:
      break;
    }

    if (cmdCode == EV_CMD_SET_INVALID)
    {
      break;
    }

    return this->exeCommand(cmdCode);
  } while (0);
  return -1;
}

int ScanningEVController::setMessTerminator(byte terminator)
{
  ScanningCmdCode_t cmdCode = EV_CMD_SET_INVALID;
  do
  {
    switch (terminator)
    {
    case SCANNING_SET_MESS_TER_NONE:
      cmdCode = EV_CMD_SET_MESS_TER_NONE;
      break;
    case SCANNING_SET_MESS_TER_ENTER:
      cmdCode = EV_CMD_SET_MESS_TER_ENTER;
      break;
    case SCANNING_SET_MESS_TER_ENTER_N_LINE_BREAK:
      cmdCode = EV_CMD_SET_MESS_TER_ENTER_N_LINE_BREAK;
      break;
    case SCANNING_SET_MESS_TER_LINE_BREAK:
      cmdCode = EV_CMD_SET_MESS_TER_LINE_BREAK;
      break;
    default:
      break;
    }

    if (cmdCode == EV_CMD_SET_INVALID)
    {
      break;
    }

    return this->exeCommand(cmdCode);
  } while (0);
  return -1;
}

// static init
ScanningEVController* ScanningEVController::instance_List[] = {};
byte ScanningEVController::cur_Index = 0;
SystemCriticalSession ScanningEVController::instance_Key;
int ScanningEVController::regInstance(ScanningEVController* instance)
{
  do
  {
    byte assignedIndx = 0;
    {
      SystemCriticalLocker lock(ScanningEVController::instance_Key);
      if ((instance == NULL)
        || (ScanningEVController::cur_Index >= SCANNING_CONTROLLER_MAX_INSTANCE)
        )
      {
        break;
      }
      assignedIndx = ScanningEVController::cur_Index;
      ScanningEVController::instance_List[assignedIndx] = instance;
      ScanningEVController::cur_Index++;
    }
    instance->instance_Indx = assignedIndx;
    return 0;
  } while (0);
  return -1;
}
int ScanningEVController::regCallback(ScanningEVController* instance, ScanningEVDeviceConfigTAG& deviceConfig)
{
  do
  {
    ScanningDeviceIsrOnSignal isrLed = NULL;
    ScanningDeviceIsrOnSignal isrBuzzer = NULL;
    switch (instance->instance_Indx)
    {
      case 0:
        isrLed = ScanningEVController::cbDeviceSignalLed00;
        isrBuzzer = ScanningEVController::cbDeviceSignalBuzzer00;
        break;
      case 1:
        isrLed = ScanningEVController::cbDeviceSignalLed01;
        isrBuzzer = ScanningEVController::cbDeviceSignalBuzzer01;
        break;
      case 2:
        isrLed = ScanningEVController::cbDeviceSignalLed02;
        isrBuzzer = ScanningEVController::cbDeviceSignalBuzzer02;
        break;
      case 3:
        isrLed = ScanningEVController::cbDeviceSignalLed03;
        isrBuzzer = ScanningEVController::cbDeviceSignalBuzzer03;
        break;
      case 4:
        isrLed = ScanningEVController::cbDeviceSignalLed04;
        isrBuzzer = ScanningEVController::cbDeviceSignalBuzzer04;
        break;
      default:
        break;
    }

    if ( (isrLed == NULL)
      || (isrBuzzer == NULL)
      )
    {
      break;
    }

    PIN_MODE(deviceConfig.pin_Led, SCANNING_DEVICE_EV_LED_PIN_MODE);
    ATTACH_ISR(deviceConfig.pin_Led, isrLed, SCANNING_DEVICE_EV_LED_ISR_TYPE);

    /*PIN_MODE(deviceConfig.pin_Buzzer, SCANNING_DEVICE_EV_BUZZER_PIN_MODE);
    ATTACH_ISR(deviceConfig.pin_Buzzer, isrBuzzer, SCANNING_DEVICE_EV_BUZZER_ISR_TYPE);*/

    return 0;
  } while (0);
  return -1;
}

void ScanningEVController::cbDeviceSignalLed(byte index)
{
  do
  {
//#ifdef SC_EV_CONTROLLER_CONSOLE_DEBUG_ENABLE
//    CONSOLE_LOG_ISR(scEVControllerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[scev] cbSL %d", 0);
//#endif // SC_EV_CONTROLLER_CONSOLE_DEBUG_ENABLE

    ScanningEVController* instance = NULL;
    bool woken = false;
    {
      SystemCriticalLockerFromISR lock(ScanningEVController::instance_Key);
      if (index > ScanningEVController::cur_Index)
      {
        break;
      }
      instance = ScanningEVController::instance_List[index];
    }
    

    if (instance->signal_Callback == NULL)
    {
//#ifdef SC_EV_CONTROLLER_CONSOLE_DEBUG_ENABLE
//      CONSOLE_LOG_ISR(scEVControllerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[scev] cbSL %d", -2);
//#endif // SC_EV_CONTROLLER_CONSOLE_DEBUG_ENABLE
      break;
    }

    instance->signal_Callback(instance->signal_Arg, SCANNING_DEVICE_SIGNAL_ID_LED, NULL, 0, &woken);

    if (woken != false)
    {
      SYSTEM_YEILD_FROM_ISR(woken);
    }
//#ifdef SC_EV_CONTROLLER_CONSOLE_DEBUG_ENABLE
//    CONSOLE_LOG_ISR(scEVControllerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[scev] cbSL %d", 99);
//#endif // SC_EV_CONTROLLER_CONSOLE_DEBUG_ENABLE
    return;
  } while (0);
//#ifdef SC_EV_CONTROLLER_CONSOLE_DEBUG_ENABLE
//  CONSOLE_LOG_ISR(scEVControllerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[scev] cbSL %d", -99);
//#endif // SC_EV_CONTROLLER_CONSOLE_DEBUG_ENABLE
  return;
}

void ScanningEVController::cbDeviceSignalBuzzer(byte index)
{
  do
  {
//#ifdef SC_EV_CONTROLLER_CONSOLE_DEBUG_ENABLE
//    CONSOLE_LOG_ISR(scEVControllerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[scev] cbSB %d", 0);
//#endif // SC_EV_CONTROLLER_CONSOLE_DEBUG_ENABLE
    ScanningEVController* instance = NULL;
    bool woken = false;
    {
      SystemCriticalLockerFromISR lock(ScanningEVController::instance_Key);
      if (index > ScanningEVController::cur_Index)
      {
        break;
      }
      instance = ScanningEVController::instance_List[index];
    }


    if (instance->signal_Callback == NULL)
    {
      break;
    }

    instance->signal_Callback(instance->signal_Arg, SCANNING_DEVICE_SIGNAL_ID_BUZZER, NULL, 0, &woken);

    if (woken != false)
    {
      SYSTEM_YEILD_FROM_ISR(woken);
    }
//#ifdef SC_EV_CONTROLLER_CONSOLE_DEBUG_ENABLE
//    CONSOLE_LOG_ISR(scEVControllerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[scev] cbSB %d", 99);
//#endif // SC_EV_CONTROLLER_CONSOLE_DEBUG_ENABLE
    return;
  } while (0);
//#ifdef SC_EV_CONTROLLER_CONSOLE_DEBUG_ENABLE
//  CONSOLE_LOG_ISR(scEVControllerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[scev] cbSB %d", -99);
//#endif // SC_EV_CONTROLLER_CONSOLE_DEBUG_ENABLE
  return;
}


void ScanningEVController::cbDeviceSignalLed00(void)
{
  ScanningEVController::cbDeviceSignalLed(0);
}

void ScanningEVController::cbDeviceSignalLed01(void)
{
  ScanningEVController::cbDeviceSignalLed(1);
}

void ScanningEVController::cbDeviceSignalLed02(void)
{
  ScanningEVController::cbDeviceSignalLed(2);
}

void ScanningEVController::cbDeviceSignalLed03(void)
{
  ScanningEVController::cbDeviceSignalLed(3);
}

void ScanningEVController::cbDeviceSignalLed04(void)
{
  ScanningEVController::cbDeviceSignalLed(4);
}


void ScanningEVController::cbDeviceSignalBuzzer00(void)
{
  ScanningEVController::cbDeviceSignalBuzzer(0);
}

void ScanningEVController::cbDeviceSignalBuzzer01(void)
{
  ScanningEVController::cbDeviceSignalBuzzer(1);
}

void ScanningEVController::cbDeviceSignalBuzzer02(void)
{
  ScanningEVController::cbDeviceSignalBuzzer(2);
}

void ScanningEVController::cbDeviceSignalBuzzer03(void)
{
  ScanningEVController::cbDeviceSignalBuzzer(3);
}

void ScanningEVController::cbDeviceSignalBuzzer04(void)
{
  ScanningEVController::cbDeviceSignalBuzzer(4);
}
#endif // _CONF_SCANNING_EV_CONTROLLER_ENABLED