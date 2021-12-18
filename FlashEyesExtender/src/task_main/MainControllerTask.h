#ifndef _MAIN_CONTROLLER_TASK_H
#define _MAIN_CONTROLLER_TASK_H

/////////////////////////////////////////////////
// INCLUDE
#include "../FlashEyesExtenderConstant.h"
#include "src/shared/task_manager/TaskManager.h"
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

/*MainControllerTask*/

class MainControllerTask
  : public TaskManager
{
public:
  MainControllerTask(void);
  virtual ~MainControllerTask(void);
  virtual int                                                   setConfig(void);
  virtual int                                                   prepare(void);
  virtual int                                                   start(void);

protected:
  virtual void                                                  proc(void) override;
  virtual void                                                  onEventInputReceived(unsigned int dataSize, unsigned char* data);
  
};

#endif // _MAIN_CONTROLLER_TASK_H