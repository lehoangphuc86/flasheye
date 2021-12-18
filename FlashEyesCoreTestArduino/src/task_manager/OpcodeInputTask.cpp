#include "OpcodeInputTask.h"
#ifdef _CONF_TEST_OP_CODE_INPUT_ENABLED
/////////////////////////////////////////////////
// INCLUDE
#ifdef SYSTEM_PC_BASED
#include <iostream>
#endif // SYSTEM_PC_BASED
/////////////////////////////////////////////////
// PREPROCESSOR

/////////////////////////////////////////////////
// DEFINE
//#define OP_CODE_INPUT_CONSOLE_DEBUG_ENABLE
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

/*OpCodeMenu*/

OpCodeMenu& OpCodeMenu::getInstance(void)
{
  static OpCodeMenu instance;
  return instance;
}

OpCodeMenu::OpCodeMenu(void)
  : item_Count(0)
{
  memset(this->item_List, 0, sizeof(OpCodeMenuItemTAG)* OPCODE_MENU_ITEM_COUNT_MAX);
  memset(this->item_Isr, 0, sizeof(IsrButtonPressed) * OPCODE_MENU_ITEM_COUNT_MAX);
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

int OpCodeMenu::setConfig(OpCodeMenuItemTAG* menuItems, byte menuItemCount)
{
  do
  {
    if ( (menuItems == NULL)
      || (menuItemCount <= 0)
      )
    {
      break;
    }

    this->item_Count = 0;//
    byte itemCount = SYSTEM_MIN(menuItemCount, OPCODE_MENU_ITEM_COUNT_MAX);
    for (byte wk_idx = 0; wk_idx < itemCount; wk_idx++)
    {
      if (this->appendItem(&menuItems[wk_idx]) != 0)
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
      || (this->item_Count >= OPCODE_MENU_ITEM_COUNT_MAX)
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
        pinMode(menuItem->pin, INPUT_PULLUP);
        attachInterrupt(digitalPinToInterrupt(menuItem->pin), this->item_Isr[wk_idx], menuItem->triggerType);
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
  CONSOLE_LOG("%s", "pin/opCode:");
  for (byte wk_idx = 0; wk_idx < this->item_Count; wk_idx++)
  {
    menuItem = &this->item_List[wk_idx];
    CONSOLE_LOG("%i %s", menuItem->pin, menuItem->description);
  }

#else // SYSTEM_ARDUINO_BASED
  OpCodeMenuItemTAG* menuItem = NULL;
  CONSOLE_LOG("%s", "input/opCode:");
  for (byte wk_idx = 0; wk_idx < this->item_Count; wk_idx++)
  {
    menuItem = &this->item_List[wk_idx];
    CONSOLE_LOG("%i:%s", menuItem->opCode, menuItem->description);
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
  //CONSOLE_LOG("opMe %i", opCode);
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



void OpCodeMenu::onButtonPressed(byte buttonIndex)
{
#ifdef SYSTEM_ARDUINO_BASED
  do
  {
#ifdef OP_CODE_INPUT_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG("[opT] irson %i", buttonIndex);
#endif // OP_CODE_INPUT_CONSOLE_DEBUG_ENABLE
    if (buttonIndex >= this->item_Count)
    {
      break;
    }

    OpCodeMenuItemTAG* menuItem = &this->item_List[buttonIndex];
    if ((millis() - menuItem->lastSelected) < OPCODE_MENU_BUTTON_DEBOUNC_TIME)
    { // Debounce
      return;
    }
    menuItem->isSelected = true;
    menuItem->lastSelected = millis();

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
#ifdef OPCODE_INPUT_TAS_USE_TASK
  : TaskManager()
  , cb_OpCode_Rev(NULL)
#else // OPCODE_INPUT_TAS_USE_TASK
  : cb_OpCode_Rev(NULL)
  , is_Running(false)
#endif // OPCODE_INPUT_TAS_USE_TASK
{

}

OpcodeInputTask::~OpcodeInputTask(void)
{
  this->stop();
  this->cleanUp();
}


bool OpcodeInputTask::isRunning(void)
{
#ifdef OPCODE_INPUT_TAS_USE_TASK
  return TaskManager::isTaskRunning();
#else // OPCODE_INPUT_TAS_USE_TASK
  return this->is_Running;
#endif // OPCODE_INPUT_TAS_USE_TASK
}


void OpcodeInputTask::isRunning(bool state)
{
#ifdef OPCODE_INPUT_TAS_USE_TASK
#else
  this->is_Running = state;
#endif // OPCODE_INPUT_TAS_USE_TASK
}

int OpcodeInputTask::setConfig(
  OpCodeMenuItemTAG* menuItems
  , byte menuItemCount
  , CbOnOpCodeRecevied cbOnOpCodeRev
#ifdef OPCODE_INPUT_TAS_USE_TASK
  , TaskManagerConfigTAG& taskConfig
#endif // OPCODE_INPUT_TAS_USE_TASK
)
{
  do
  {
    int result = 0;
    if (cbOnOpCodeRev == NULL)
    {
      break;
    }

    this->cb_OpCode_Rev = cbOnOpCodeRev;

    result = OpCodeMenu::getInstance().setConfig(menuItems, menuItemCount);
    if (result != 0)
    {
      break;
    }

    //result = OpCodeMenu::getInstance().prepare();
    //if (result != 0)
    //{
    //  break;
    //}

#ifdef OPCODE_INPUT_TAS_USE_TASK
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
#endif // OPCODE_INPUT_TAS_USE_TASK

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
#ifdef OPCODE_INPUT_TAS_USE_TASK
int OpcodeInputTask::start(TaskThreadConfigTAG& threadConfig)
#else // OPCODE_INPUT_TAS_USE_TASK
int OpcodeInputTask::start(void)
#endif // OPCODE_INPUT_TAS_USE_TASK
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
#ifdef OPCODE_INPUT_TAS_USE_TASK
    result = TaskManager::startProcess(threadConfig, true);
    if (result != 0)
    {
      break;
    }
    //result = this->waitPrepareResult();
    //if (result != 0)
    //{
    //  break;
    //}
#else // OPCODE_INPUT_TAS_USE_TASK
    
    this->isRunning(true);
    this->proc();
#endif // OPCODE_INPUT_TAS_USE_TASK
    return result;
  } while (0);
  this->stop();
  return -1;
}

void OpcodeInputTask::stop(void)
{
  if (this->isRunning() != false)
  {
#ifdef OPCODE_INPUT_TAS_USE_TASK
    TaskManager::stopProcess();
#endif // OPCODE_INPUT_TAS_USE_TASK
  }

  this->isRunning(false);
}

void OpcodeInputTask::cleanUp(void)
{
  this->cb_OpCode_Rev = NULL;
}

void OpcodeInputTask::proc(void)
{
#ifdef OP_CODE_INPUT_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG("[opT] proc %i", 0);
#endif // OP_CODE_INPUT_CONSOLE_DEBUG_ENABLE
  //int ret = 0;
  int opCode = -1;
  int isStopped = false;
  int prepareRet = this->prepare();
#ifdef OPCODE_INPUT_TAS_USE_TASK
  this->reportPrepareResult(prepareRet);
#ifdef OP_CODE_INPUT_CONSOLE_DEBUG_ENABLE
  //CONSOLE_LOG("[opT] proc %i %i", 1, prepareRet);
#endif // OP_CODE_INPUT_CONSOLE_DEBUG_ENABLE
  if (prepareRet != 0)
  {
    this->waitTerminating();
    goto _procEnd;
  }
#else // OPCODE_INPUT_TAS_USE_TASK
  if (prepareRet != 0)
  {
    goto _procEnd;
  }
#endif // OPCODE_INPUT_TAS_USE_TASK

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
  CONSOLE_LOG("%s", "[opcod] [proc] stop");
}

#endif // _CONF_TEST_OP_CODE_INPUT_ENABLED
