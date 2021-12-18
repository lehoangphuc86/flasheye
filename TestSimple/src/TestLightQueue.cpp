
/////////////////////////////////////////////////
// INCLUDE
#include "TestLightQueue.h"
#if (1)
#include "os_system/SystemCommon.h"
#include "utility/Utility.h"
/////////////////////////////////////////////////
// PREPROCESSOR

/////////////////////////////////////////////////
// DEFINE
#define TASK_MANAGER_DEBUG_ENABLE
#define TASK_MANAGER_MEM_USAGE_ENABLED
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

/* TestTaskManager*/
uint16_t TestTaskManager::next_Task_Id = 0;
TestTaskManager::TestTaskManager(void)
  : is_Running(false)
  , thread_Handler(NULL)
  , task_Id(0)
  , max_Handling_Event_Size(sizeof(int))
{
  this->task_Id = TestTaskManager::getNextTaskId();
  memset(&this->task_Config, 0, sizeof(TaskManagerConfigTAG));
}

TestTaskManager::~TestTaskManager(void)
{
  
}

void TestTaskManager::proc(void)
{
  // @@ must to be overried by child classes
}

void TestTaskManager::mainProc(void)
{
//#ifdef TASK_MANAGER_MEM_USAGE_ENABLED
//  SYSTEM_TASK_MEM_DATA_TYPE usedMem = SYSTEM_TASK_MEM_USAGE(this->thread_Handler->nativeHandler());
//  int globalFreeMem = Utility::getFreeMemory();
//#endif // TASK_MANAGER_MEM_USAGE_ENABLED
  this->proc();
}

void TestTaskManager::navigatingProc(void* param)
{
  do
  {
    if (param == NULL)
    {
      break;
    }
    TestTaskManager* caller = (TestTaskManager*)param;
    caller->mainProc();
    //return;
  } while (0);
}

uint16_t TestTaskManager::getNextTaskId(void)
{
  TestTaskManager::next_Task_Id++;
  return TestTaskManager::next_Task_Id;
}
#endif // _CONF_TASK_MANAGER_ENABLED