#include "CommMBCSerialConstant.h"

#if (_CONF_COMM_MBC_SERIAL_ENABLED)
#ifndef _COMM_MBC_SERIAL_H
#define _COMM_MBC_SERIAL_H

/////////////////////////////////////////////////
// INCLUDE
#include "../CommMBCEngine.h"
#include "../../task_comm/task_comm_serial/CommSerial.h"

#if (!_CONF_COMM_MBC_ENGINE_ENABLED)
#error Comm MBC engine is required
#endif // _CONF_COMM_MBC_ENGINE_ENABLED

#if (!_CONF_COMM_SERIAL_ENABLED)
#error Comm serial is required
#endif // _CONF_COMM_SERIAL_ENABLED

/////////////////////////////////////////////////
// PREPROCESSOR
#ifdef COMM_SERIAL_USE_TASK
#define COMM_MBC_SERIAL_USE_TASK
#endif // COMM_SERIAL_USE_TASK


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

/*CommMBCSerial*/

class CommMBCSerial
  : public CommSerial
{
public:
  ////////////////////Function to call from outside ISR///////////////////////
  CommMBCSerial(void);
  // WARNING: if inherite from this class, deconstructor must be virtual
  __ATTRIBUTE_VIRTUAL_OPTIMIZED ~CommMBCSerial(void);
  uint16_t                                                      registerMessageId(uint8_t messageId, uint16_t maxRawSize = 0);
  int                                                           setConfig(CommMBCSerialConfigTAG& mbcSerialConfig);
  int                                                           start(void) override;
  int                                                           connect(CommMBCSerialSocketConfigTAG& mbcSocketConfig) override;
  void                                                          stop(void) override;
  int                                                           send(uint8_t messageId, unsigned char* inputBuffer, uint16_t inputBufferSize);
  int                                                           read(uint8_t inTryTime, uint8_t& messageId, BufferDataItem*& dataItem, bool& isDisconnected);
  void                                                          cleanUp(void) override;
protected:
  int                                                           prepare(void) override;
  int                                                           setConfig(CommSerialConfigTAG& commConfig) override; // hide from public
  //int                                                           connect(CommSerialSocketConfigTAG& socketConfig) override; // hide from public
  int                                                           send(unsigned char* buffer, DataSize_t bufferSize) override; // hide from public
  int                                                           read(uint8_t inTryTime, BufferDataItem*& dataItem, bool& isDisconnected) override; // hide from public
#ifdef COMM_MBC_SERIAL_USE_TASK
  int                                                           onEventDateReceived(unsigned char* data, unsigned int dataSize) override;
#endif // COMM_MBC_SERIAL_USE_TASK
protected:
  CommMBCEngine                                                 comm_Mbc_Engine;
  CommMBCSerialConfigTAG                                        comm_Mbc_Serial_Config;
};

#endif // _COMM_MBC_SERIAL_H

#endif // _CONF_COMM_MBC_SERIAL_ENABLED