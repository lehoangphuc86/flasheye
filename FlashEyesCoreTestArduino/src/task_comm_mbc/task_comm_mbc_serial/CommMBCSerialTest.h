#include "../../FlashEyeCoreTestConfig.h"

#ifdef _CONF_TEST_TASK_COMM_MBC_SERIAL_ENABLED

#ifndef _COMM_MBC_SERIAL_TEST_H
#define _COMM_MBC_SERIAL_TEST_H

/////////////////////////////////////////////////
// INCLUDE
#include "task_comm_mbc/task_comm_mbc_serial/CommMBCSerial.h"

/////////////////////////////////////////////////
// PREPROCESSOR

/////////////////////////////////////////////////
// DEFINE
#define COMM_SERIAL_TEST_DUMP_LEN               8 //COMM_SERIAL_SYS_BUFFER_LEN
#define COMM_SERIAL_MBC_HEADERLESS              false
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

typedef struct
{
  uint8_t param1;
  uint8_t param2;
  uint8_t nameLen;
  unsigned char name[COMM_MBC_NAME_MAX_LEN];
} EventCommMBCSerialStart1TAG;

/////////////////////////////////////////////////
// STATIC DATA

/////////////////////////////////////////////////
// STATIC FUNCTIONS

/////////////////////////////////////////////////
// EXTERN

/////////////////////////////////////////////////
// CLASS DEFINITION

/*CommMBCSerialTest*/
class CommMBCSerialTest
#ifdef COMM_MBC_SERIAL_USE_TASK
  : public TaskManager
#endif // COMM_MBC_SERIAL_USE_TASK
{
public:
  CommMBCSerialTest(byte sourceId);
  virtual ~CommMBCSerialTest(void);
  int                                                           setConfig(
                                                                    CommMBCSerialConfigTAG& commConfig
#ifdef COMM_MBC_SERIAL_USE_TASK
                                                                    , TaskManagerConfigTAG& taskConfig
#endif // COMM_MBC_SERIAL_USE_TASK
                                                                  );
#ifdef COMM_MBC_SERIAL_USE_TASK
  int                                                           start(TaskThreadConfigTAG& threadConfig);
#else // COMM_MBC_SERIAL_USE_TASK
  int                                                           start(void);
#endif // COMM_MBC_SERIAL_USE_TASK
  int                                                           connect(CommMBCSerialSocketConfigTAG& socketConfig);
  int                                                           disconnect(void);
  int                                                           sendStart1(uint8_t param1, uint8_t param2, uint8_t nameLen, unsigned char* name);
  int                                                           read(byte tryTime = 0xFF);
  int                                                           stop(void);
protected:
  int                                                           prepare(void);
#ifdef COMM_MBC_SERIAL_USE_TASK
  void                                                          proc(void) override;
#endif // COMM_MBC_SERIAL_USE_TASK
  int                                                           onEventOnSendTestData(unsigned char* data, unsigned int dataSize);
  void                                                          onEventOnDataReceived(unsigned char* data, unsigned int dataSize);
  void                                                          onEventOnConnectionClosed(unsigned char* data, unsigned int dataSize);

#ifdef COMM_MBC_SERIAL_USE_TASK
public:
  static void                                                   cbOnDataReceivedNavigator(void* param, DataSize_t paramSize);
  static void                                                   cbOnConnectionClosedNavigator(void* param, DataSize_t paramSize);
#endif // COMM_MBC_SERIAL_USE_TASK
protected:
  byte                                                          source_Id;
  CommMBCSerial                                                 comm_Serial;
};


#endif // _COMM_MBC_SERIAL_TEST_H

#endif // _CONF_TEST_TASK_COMM_MBC_SERIAL_ENABLED