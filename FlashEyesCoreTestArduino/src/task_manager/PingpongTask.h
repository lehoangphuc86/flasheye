#include "../FlashEyeCoreTestConfig.h"

#ifdef _CONF_TEST_TASK_MANAGER_ENABLED

#ifndef _PING_PONG_TASK_H
#define _PING_PONG_TASK_H

/////////////////////////////////////////////////
// INCLUDE
#include "task_manager/TaskManager.h"
#include "data_manager/BufferDataManager.h"
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

typedef struct
{
  byte count;
  byte sourceId;
} EventPingpongTestDataTAG;

/////////////////////////////////////////////////
// STATIC DATA

/////////////////////////////////////////////////
// STATIC FUNCTIONS

/////////////////////////////////////////////////
// EXTERN

/////////////////////////////////////////////////
// CLASS DEFINITION

/*Task1*/
class PingpongTask
  : public TaskManager
{
public:
  PingpongTask(void);
  virtual ~PingpongTask(void);
  int                                                           setConfig(bool isSource, byte baseNumber, unsigned int timePace, PingpongTask* partner, TaskManagerConfigTAG& taskConfig);
  int                                                           prepare(void);
  int                                                           start(TaskThreadConfigTAG& threadConfig);
  int                                                           stop(void);
protected:
  void                                                          proc(void) override;
  void                                                          onEventTestData(unsigned char* data, unsigned int dataSize);
protected:
  bool                                                          is_Source;
  byte                                                          base_Number;
  unsigned int                                                  time_Pace;
  PingpongTask*                                                 partner_Task;
  byte                                                          expected_SeqId;
  BufferDataManager                                             data_Manager;
};


#endif // _PING_PONG_TASK_H

#endif // _CONF_TEST_TASK_MANAGER_ENABLED