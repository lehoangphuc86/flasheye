#ifndef _TIME_READER_TASK_H
#define _TIME_READER_TASK_H

/////////////////////////////////////////////////
// INCLUDE
#include "TimeReaderConstant.h"
#include "task_comm/task_comm_i2c/CommI2CServer.h"
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

/*TimeReaderTask*/

class TimeReaderTask
{
public:
  ////////////////////Function to call from outside ISR///////////////////////
  TimeReaderTask(void);
  virtual ~TimeReaderTask(void);
  int                                                           setConfig(void);
  int                                                           prepare(void);
  int                                                           start(void);
  int                                                           getTime(uint8_t clientAddress);

protected:
  byte                                                          readTimerReg(uint8_t clientAddress, byte regAddress);
  byte                                                          readTimerData(uint8_t clientAddress, byte regAddress);
protected:
  CommMBCI2CServer                                              i2c_Server;
};

#endif // _TIME_READER_TASK_H