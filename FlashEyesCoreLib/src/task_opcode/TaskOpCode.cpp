#include "TaskOpcode.h"
#if (_CONF_TASK_OPCODE_ENABLED)
/////////////////////////////////////////////////
// INCLUDE
#include "timer_manager/TimerManager.h"

#ifdef SYSTEM_PC_BASED
#include <iostream>
#endif // SYSTEM_PC_BASED
/////////////////////////////////////////////////
// PREPROCESSOR

/////////////////////////////////////////////////
// DEFINE
#define TASK_OPCODE_CONSOLE_DEBUG_ENABLE
/////////////////////////////////////////////////
// MARCO

/////////////////////////////////////////////////
// GLOBAL VARIABLES
//#ifdef TASK_OPCODE_CONSOLE_DEBUG_ENABLE
char taskOpCodeLogBuf[SYSTEM_CONSOLE_OUT_BUF_LEN];
//#endif // TASK_OPCODE_CONSOLE_DEBUG_ENABLE
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

/*OpCodeMenu*/

OpCodeMenu& OpCodeMenu::getInstance(void)
{
  static OpCodeMenu instance;
  return instance;
}

OpCodeMenu::OpCodeMenu(void)
  : item_Count(0)
  , bounce_Time(TASK_OPCODE_BUTTON_DEBOUNCE_TIME_DEFAULT)
{
  memset(this->item_List, 0, sizeof(OpCodeMenuItemTAG)* TASK_OPCODE_MENU_ITEM_COUNT_MAX);
  memset(this->item_Isr, 0, sizeof(IsrButtonPressed) * TASK_OPCODE_MENU_ITEM_COUNT_MAX);
  this->item_Isr[0] = OpCodeMenu::isrButtonPressed00;
  this->item_Isr[1] = OpCodeMenu::isrButtonPressed01;
  this->item_Isr[2] = OpCodeMenu::isrButtonPressed02;
  this->item_Isr[3] = OpCodeMenu::isrButtonPressed03;
  this->item_Isr[4] = OpCodeMenu::isrButtonPressed04;
  this->item_Isr[5] = OpCodeMenu::isrButtonPressed05;
  this->item_Isr[6] = OpCodeMenu::isrButtonPressed06;
  this->item_Isr[7] = OpCodeMenu::isrButtonPressed07;
}

OpCodeMenu::~OpCodeMenu(void)
{

}

int OpCodeMenu::setConfig(OpCodeMenuConfigTAG& menuConfig)
{
  do
  {
    if ( (menuConfig.menuItems == NULL)
      || (menuConfig.menuItemCount <= 0)
      )
    {
      break;
    }
    this->bounce_Time = menuConfig.bounceTime;
    this->item_Count = 0;//
    byte itemCount = SYSTEM_MIN(menuConfig.menuItemCount, TASK_OPCODE_MENU_ITEM_COUNT_MAX);
    for (byte wk_idx = 0; wk_idx < itemCount; wk_idx++)
    {
      if (this->appendItem(&menuConfig.menuItems[wk_idx]) != 0)
      {
        break;
      }
    }

    return 0;
  } while (0);
  return -1;
}

int OpCodeMenu::appendItem(OpCodeMenuItemTAG* menuItem)
{
  do
  {
    if ((menuItem == NULL)
      || (this->item_Count >= TASK_OPCODE_MENU_ITEM_COUNT_MAX)
      )
    {
      break;
    }

    memcpy(&this->item_List[this->item_Count], menuItem, sizeof(OpCodeMenuItemTAG));
    this->item_List[this->item_Count].isSelected = false;
    this->item_List[this->item_Count].lastSelected = 0;

    this->item_Count++;
    return 0;
  } while (0);
  return -1;
}

byte OpCodeMenu::menuItemCount(void)
{
  return this->item_Count;
}

int OpCodeMenu::prepare(void)
{
  do
  {
    if (this->item_Count <= 0)
    {
      break;
    }

#ifdef SYSTEM_ARDUINO_BASED
    {
      OpCodeMenuItemTAG* menuItem = NULL;
      for (byte wk_idx = 0; wk_idx < this->item_Count; wk_idx++)
      {
        menuItem = &this->item_List[wk_idx];
        if ( (menuItem->pin <= 0)
          || (menuItem->pin > PIN_NUMBER_MAX)
          )
        {
          continue;
        }
        PIN_MODE(menuItem->pin, menuItem->gpioFunc);
        //attachInterrupt(digitalPinToInterrupt(menuItem->pin), this->item_Isr[wk_idx], menuItem->triggerType);
        ATTACH_ISR(menuItem->pin, this->item_Isr[wk_idx], menuItem->triggerType);
      }
    }
#endif // SYSTEM_ARDUINO_BASED
    return 0;
  } while (0);

  return -1;
}

void OpCodeMenu::display(void)
{
#ifdef SYSTEM_ARDUINO_BASED
  OpCodeMenuItemTAG* menuItem = NULL;
  CONSOLE_LOG_BUF(taskOpCodeLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "%s", "pin/opCode:");
  for (byte wk_idx = 0; wk_idx < this->item_Count; wk_idx++)
  {
    menuItem = &this->item_List[wk_idx];
    CONSOLE_LOG_BUF(taskOpCodeLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "%i %s", menuItem->pin, menuItem->description);
  }
#else // SYSTEM_ARDUINO_BASED
  OpCodeMenuItemTAG* menuItem = NULL;
  CONSOLE_LOG_BUF(taskOpCodeLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "%s", "pin/opCode:");
  for (byte wk_idx = 0; wk_idx < this->item_Count; wk_idx++)
  {
    menuItem = &this->item_List[wk_idx];
    CONSOLE_LOG_BUF(taskOpCodeLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "%i:%s", menuItem->opCode, menuItem->description);
  }
#endif // SYSTEM_ARDUINO_BASED
}

int OpCodeMenu::select(void)
{
#ifdef SYSTEM_ARDUINO_BASED

#else // SYSTEM_ARDUINO_BASED
  this->display();
#endif // SYSTEM_ARDUINO_BASED
  return this->getOpCode();
}

int OpCodeMenu::select(int* outOpCodes, int outOpCodeMaxCount, int& outOpCodeCount)
{
#ifdef SYSTEM_ARDUINO_BASED

#else // SYSTEM_ARDUINO_BASED
  this->display();
#endif // SYSTEM_ARDUINO_BASED
  return this->getOpCode(outOpCodes, outOpCodeMaxCount, outOpCodeCount);
}

#ifdef SYSTEM_ARDUINO_BASED
int OpCodeMenu::getOpCode(void)
{
  int opCode = -1;
  OpCodeMenuItemTAG* menuItem = NULL;
  for (byte wk_idx = 0; wk_idx < this->item_Count; wk_idx++)
  {
    menuItem = &this->item_List[wk_idx];
    if ( (opCode < 0) 
      && (menuItem->isSelected != false)
      )
    {
      opCode = menuItem->opCode;
    }
    menuItem->isSelected = false;
  }
  return opCode;
}
#else // SYSTEM_ARDUINO_BASED
// PC
int OpCodeMenu::getOpCode(void)
{
  int opInt = -1;
  char opChar = (char)getchar();
  if (opChar == 10)
  {
    opChar = (char)getchar();
  }

  opInt = std::atoi(&opChar);
  return opInt;
}
#endif // SYSTEM_ARDUINO_BASED

int OpCodeMenu::getOpCode(int* outOpCodes, int outOpCodeMaxCount, int& outOpCodeCount)
{
  outOpCodeCount = 0;
  do
  {
    if ( (outOpCodes == NULL)
      || (outOpCodeMaxCount <= 0)
      )
    {
      break;
    }
#ifdef TASK_OPCODE_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(taskOpCodeLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[opT] gOP2 %i %i", 1, outOpCodeMaxCount);
#endif // TASK_OPCODE_CONSOLE_DEBUG_ENABLE
    memset(outOpCodes, TASK_OPCODE_MENU_ITEM_OPCODE_INVALID, outOpCodeMaxCount);
#ifdef SYSTEM_ARDUINO_BASED
    OpCodeMenuItemTAG* menuItem = NULL;
    for (byte wk_idx = 0; wk_idx < this->item_Count; wk_idx++)
    {
      if (outOpCodeCount >= outOpCodeMaxCount)
      {
        break;
      }

      menuItem = &this->item_List[wk_idx];
      if (menuItem->isSelected != false)
      {
        outOpCodes[outOpCodeCount] = menuItem->opCode;
        outOpCodeCount++;
      }
      menuItem->isSelected = false;
    }
    
#else // SYSTEM_ARDUINO_BASED
    int opCode = this->getOpCode();
    if (opCode < 0)
    {
      outOpCodeCount = 0;
    }
    else
    {
      outOpCodes[0] = opCode;
    }
#endif // SYSTEM_ARDUINO_BASED
    return 0;
  } while (0);
  outOpCodeCount = 0;
  return -1;
}

void OpCodeMenu::onButtonPressed(byte buttonIndex)
{
#ifdef SYSTEM_ARDUINO_BASED
  do
  {
//#ifdef TASK_OPCODE_CONSOLE_DEBUG_ENABLE
//    CONSOLE_LOG_BUF(taskOpCodeLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[opT] irson %i", buttonIndex);
//#endif // TASK_OPCODE_CONSOLE_DEBUG_ENABLE
    if (buttonIndex >= this->item_Count)
    {
      break;
    }
    TimePoint_t nowMs = TimerManager::getInstance().nowFromISR();
    OpCodeMenuItemTAG* menuItem = &this->item_List[buttonIndex];
    if ((nowMs - menuItem->lastSelected) < (this->bounce_Time*TIMER_DEVICE_TIMER_MS_2_UNIT))
    { // Debounce
      return;
    }
    menuItem->isSelected = true;
    menuItem->lastSelected = nowMs;

  } while (0);
#else
  return;
#endif // #SYSTEM_ARDUINO_BASED
}

void OpCodeMenu::isrButtonPressed00(void)
{
  OpCodeMenu::getInstance().onButtonPressed(0);
}

void OpCodeMenu::isrButtonPressed01(void)
{
  OpCodeMenu::getInstance().onButtonPressed(1);
}

void OpCodeMenu::isrButtonPressed02(void)
{
  OpCodeMenu::getInstance().onButtonPressed(2);
}

void OpCodeMenu::isrButtonPressed03(void)
{
  OpCodeMenu::getInstance().onButtonPressed(3);
}

void OpCodeMenu::isrButtonPressed04(void)
{
  OpCodeMenu::getInstance().onButtonPressed(4);
}

void OpCodeMenu::isrButtonPressed05(void)
{
  OpCodeMenu::getInstance().onButtonPressed(5);
}

void OpCodeMenu::isrButtonPressed06(void)
{
  OpCodeMenu::getInstance().onButtonPressed(6);
}

void OpCodeMenu::isrButtonPressed07(void)
{
  OpCodeMenu::getInstance().onButtonPressed(7);
}


/* OpcodeInputTask*/

////////////////////Function to call from outside ISR///////////////////////

OpcodeInputTask& OpcodeInputTask::getInstance(void)
{
  static OpcodeInputTask instance;
  return instance;
}

OpcodeInputTask::OpcodeInputTask(void)
  : TaskManager()
  , cb_OpCode_Rev(NULL)
  , is_Running(false)
  , use_Task(true)
{

}

OpcodeInputTask::~OpcodeInputTask(void)
{
  this->stop();
  this->cleanUp();
}


bool OpcodeInputTask::isRunning(void)
{
  return (this->use_Task == false? this->is_Running : TaskManager::isTaskRunning());
}


void OpcodeInputTask::isRunning(bool state)
{
  if (this->use_Task == false)
  {
    this->is_Running = state;
  }
}

int OpcodeInputTask::setConfig(
  OpCodeMenuConfigTAG& menuConfig
  , CbOnOpCodeRecevied cbOnOpCodeRev
  , TaskManagerConfigTAG& taskConfig
  , bool useTask
)
{
  do
  {
    int result = 0;
    if (cbOnOpCodeRev == NULL)
    {
      break;
    }
#ifdef SYSTEM_PC_BASED
    useTask = false; // always
#endif // SYSTEM_PC_BASED

    this->cb_OpCode_Rev = cbOnOpCodeRev;
    result = OpCodeMenu::getInstance().setConfig(menuConfig);
    if (result != 0)
    {
      break;
    }
    this->use_Task = useTask;

    
    if (this->use_Task == false)
    {
      return result;
    }

    // Set up tasks
    {
      this->registerHanldingEventStructSize(sizeof(EventButtonPressedParamsTAG));
      result = TaskManager::setConfig(taskConfig);
      if (result != 0)
      {
        break;
      }
      result = TaskManager::prepare();
      if (result != 0)
      {
        break;
      }
    }

    return result;
  } while (0);
  this->stop();
  return -1;
}

int OpcodeInputTask::appendMenuItem(OpCodeMenuItemTAG* menuItem)
{
  return OpCodeMenu::getInstance().appendItem(menuItem);
}

int OpcodeInputTask::prepare(void)
{
  do
  {
    int result = 0;
    if (this->cb_OpCode_Rev == NULL)
    {
      break;
    }

    result = OpCodeMenu::getInstance().prepare();
    if (result != 0)
    {
      break;
    }
    return result;
  } while (0);
  return -1;
}


int OpcodeInputTask::start(TaskThreadConfigTAG& threadConfig)
{
  do
  {
    int result = 0;
    if (this->isRunning() != false)
    {
      return 0;
    }
    //this->stop();
    if (this->cb_OpCode_Rev == NULL)
    {
      break;
    }
    if (this->use_Task != false)
    {
      result = TaskManager::startProcess(threadConfig, true);
      if (result != 0)
      {
        break;
      }
      return result;
    }
    
    this->isRunning(true);
    this->proc();
    return result;
  } while (0);
  this->stop();
  return -1;
}

void OpcodeInputTask::stop(void)
{
  if (this->isRunning() != false)
  {
    if (this->use_Task != false)
    {
      TaskManager::stopProcess();
    }
  }

  this->isRunning(false);
}

void OpcodeInputTask::cleanUp(void)
{
  this->cb_OpCode_Rev = NULL;
}

void OpcodeInputTask::proc(void)
{
#ifdef TASK_OPCODE_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(taskOpCodeLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[opT] proc %i", 0);
#endif // TASK_OPCODE_CONSOLE_DEBUG_ENABLE
  //int ret = 0;
  int opCode = -1;
  int isStopped = false;
  int prepareRet = this->prepare();
  if (this->use_Task != false)
  {
    this->reportPrepareResult(prepareRet);
  #ifdef TASK_OPCODE_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(taskOpCodeLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[opT] proc %i %i", 1, prepareRet);
  #endif // TASK_OPCODE_CONSOLE_DEBUG_ENABLE
    if (prepareRet != 0)
    {
      this->waitTerminating();
      goto _procEnd;
    }
  }
  else
  {
    if (prepareRet != 0)
    {
      goto _procEnd;
    }
  }

  while (this->isRunning() != false)
  {
    opCode = OpCodeMenu::getInstance().select();
    if (opCode < 0)
    {
      SYSTEM_SLEEP(100);
      continue;
    }

    isStopped = this->cb_OpCode_Rev(opCode);
    if (isStopped != false)
    {
      this->isRunning(false);
      break;
    }
  }
  
_procEnd:
  CONSOLE_LOG_BUF(taskOpCodeLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "%s", "[opcod] [proc] stop");
}

#endif // _CONF_TASK_OPCODE_ENABLED
