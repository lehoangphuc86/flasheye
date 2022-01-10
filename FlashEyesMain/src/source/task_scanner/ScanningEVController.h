#include "ScanningController.h"

#if (_CONF_SCANNING_EV_CONTROLLER_ENABLED)

#ifndef _SCANNING_SC_CONTROLLER_H
#define _SCANNING_SC_CONTROLLER_H

/////////////////////////////////////////////////
// INCLUDE
#include "HardwareSerial.h"
#include "os_system/SystemCriticalSession.h"
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
// DATA TYPE (TYPEDEF)

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
// CLASS DEFINITION

/*ScanningEVController*/

class ScanningEVController
  : public ScanningController
{
public:
  ScanningEVController(void);
  virtual ~ScanningEVController(void);
  bool                                                          isValid(void) override;
  int                                                           start(ScanningDeviceConfigTAG& deviceConfig) override;
  void                                                          stop(void) override;
  // controlling functions
  
  int                                                           startScan(void) override;
  int                                                           readResult(ScanningDeviceResultTAG& scanResult) override;
  void                                                          stopScan(void) override;
  int                                                           setting(ScanningDeviceSettingTAG& setting, ScanningDeviceSettingResultTAG& outResult) override;
protected:
  int                                                           createSocket(ScanningEVSerialConfigTAG& socketConfig);
  int                                                           setSocketConfig(ScanningEVSerialConfigTAG& socketConfig);
  int                                                           exeCommand(ScanningCmdCode_t cmdCode, uint16_t timeout = SCANNING_DEVICE_DATA_TIMEOUT_DEFAULT);
  int                                                           readCode(ScanningBarCodeTAG& barCode, uint16_t timeout = SCANNING_DEVICE_DATA_TIMEOUT_DEFAULT);
  int                                                           readCommand(ScanningCmdCode_t& cmdCode, uint16_t timeout = SCANNING_DEVICE_DATA_TIMEOUT_DEFAULT);
  int                                                           readRawCommand(uint16_t timeout = SCANNING_DEVICE_DATA_TIMEOUT_DEFAULT);
  int                                                           writeCommand(ScanningCmdCode_t cmdCode, uint16_t timeout = SCANNING_DEVICE_DATA_TIMEOUT_DEFAULT);
  int                                                           writeString(char* buff, DataSize_t buffLen);
  int                                                           writeChar(char ch);
  void                                                          char2Hex(char inChar, char& hex0, char& hex1);
  ScanningCmdCode_t                                             hex2Cmd(char inHex);

  void                                                          flushCommBuff(void);

  int                                                           setCommBaudrate(byte baudrate);
  int                                                           setCommParity(byte parity);
  int                                                           setCommStopbit(byte stopbit);
  int                                                           setCommDatabit(byte databit);
  int                                                           setCommFlowCtrl(byte flowCtrl);
  int                                                           setCharacterFormat(byte format);
  int                                                           setReadState(byte readState);
  
  int                                                           setAutoScan(byte flag);
  int                                                           setContScan(byte flag);
  int                                                           setSecurityLevel(byte level);
  int                                                           setEnablePrefix(byte flag);
  int                                                           setCustomPrefix(char* prefix);
  int                                                           setCustomSuffix(char* suffix);
  int                                                           setCustomCodeFormat(byte format);
  int                                                           setMessTerminator(byte terminator);


protected:
  HardwareSerial*                                               dev_Comm;
  void*                                                         signal_Arg;
  ScanningDeviceCbOnSignal                                      signal_Callback;
  byte                                                          instance_Indx;
  char*                                                         comm_Buff;
  char*                                                         cmd_Buff;
  uint16_t                                                      data_Timeout;

// static
private:
  static int                                                    regInstance(ScanningEVController* instance);
  static int                                                    regCallback(ScanningEVController* instance, ScanningEVDeviceConfigTAG& deviceConfig);
  static void                                                   cbDeviceSignalLed(byte index);
  static void                                                   cbDeviceSignalLed00(void);
  static void                                                   cbDeviceSignalLed01(void);
  static void                                                   cbDeviceSignalLed02(void);
  static void                                                   cbDeviceSignalLed03(void);
  static void                                                   cbDeviceSignalLed04(void);
  static void                                                   cbDeviceSignalBuzzer(byte index);
  static void                                                   cbDeviceSignalBuzzer00(void);
  static void                                                   cbDeviceSignalBuzzer01(void);
  static void                                                   cbDeviceSignalBuzzer02(void);
  static void                                                   cbDeviceSignalBuzzer03(void);
  static void                                                   cbDeviceSignalBuzzer04(void);

private:
  static ScanningEVController*                                  instance_List[SCANNING_CONTROLLER_MAX_INSTANCE];
  static byte                                                   cur_Index;
  static SystemCriticalSession                                  instance_Key;
};

#endif // _SCANNING_SC_CONTROLLER_H

#endif // _CONF_SCANNING_EV_CONTROLLER_ENABLED