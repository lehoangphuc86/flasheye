#include "../../FlashEyeCoreTestConfig.h"

#ifdef _CONF_TEST_TASK_COMM_SERIAL_ENABLED

#ifndef _COMM_SERIAL_TEST_H
#define _COMM_SERIAL_TEST_H

/////////////////////////////////////////////////
// INCLUDE
//#include "../../../../FlashEyesCoreLib/src/task_comm/task_comm_serial/CommSerial.h"
#include "task_comm/task_comm_serial/CommSerial.h"

/////////////////////////////////////////////////
// PREPROCESSOR

/////////////////////////////////////////////////
// DEFINE
//#define COMM_SERIAL_TEST_DUMP_LEN           8 //COMM_SERIAL_SYS_BUFFER_LEN
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

//typedef struct __ATTRIBUTE_ALIGN
//{
//  byte count;
//  byte sourceId;
//  char testDump[COMM_SERIAL_TEST_DUMP_LEN];
//} CommSerialTestDataTAG;

typedef struct
{
  byte count;
} EventCommSerialTestDataTAG;

/////////////////////////////////////////////////
// STATIC DATA

/////////////////////////////////////////////////
// STATIC FUNCTIONS

/////////////////////////////////////////////////
// EXTERN

/////////////////////////////////////////////////
// CLASS DEFINITION

/*CommSerialTest*/
class CommSerialTest
#ifdef COMM_SERIAL_USE_TASK
  : public TaskManager
#endif // COMM_SERIAL_USE_TASK
{
public:
  CommSerialTest(byte sourceId);
  virtual ~CommSerialTest(void);
  int                                                           setConfig(
                                                                    CommSerialConfigTAG& commConfig
#ifdef COMM_SERIAL_USE_TASK
                                                                    , TaskManagerConfigTAG& taskConfig
#endif // COMM_SERIAL_USE_TASK
                                                                  );
#ifdef COMM_SERIAL_USE_TASK
  int                                                           start(TaskThreadConfigTAG& threadConfig);
#else // COMM_SERIAL_USE_TASK
  int                                                           start(void);
#endif // COMM_SERIAL_USE_TASK
  int                                                           connect(CommSerialSocketConfigTAG& socketConfig);
  int                                                           disconnect(void);
  int                                                           send(byte count);
  int                                                           read(byte tryTime = 0xFF);
  int                                                           stop(void);
protected:
  int                                                           prepare(void);
#ifdef COMM_SERIAL_USE_TASK
  void                                                          proc(void) override;
#endif // COMM_SERIAL_USE_TASK
  int                                                           onEventOnSendTestData(unsigned char* data, unsigned int dataSize);
  void                                                          onEventOnDataReceived(unsigned char* data, unsigned int dataSize);
  void                                                          onEventOnConnectionClosed(unsigned char* data, unsigned int dataSize);

#ifdef COMM_SERIAL_USE_TASK
public:
  static void                                                   cbOnDataReceivedNavigator(void* param, DataSize_t paramSize);
  static void                                                   cbOnConnectionClosedNavigator(void* param, DataSize_t paramSize);
#endif // COMM_SERIAL_USE_TASK
protected:
  byte                                                          source_Id;
  CommSerial                                                    comm_Serial;
};


#endif // _COMM_SERIAL_TEST_H

#endif // _CONF_TEST_TASK_COMM_SERIAL_ENABLED