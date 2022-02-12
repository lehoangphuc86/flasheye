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
//#define TASK_OPCODE_CONSOLE_DEBUG_ENABLE
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
  this->clear();
}

int OpCodeMenu::appendItem(OpCodeMenuItemConfigTAG* menuItem, byte* outIndexP)
{
  byte outIndex = TASK_OPCODE_MENU_ITEM_INDEX_INVALID;
  do
  {
    SystemCriticalLocker locker(this->critical_Key);
    if ((menuItem == NULL)
      || (this->item_Count >= TASK_OPCODE_MENU_ITEM_COUNT_MAX)
      )
    {
      break;
    }

    if (this->findItemByOpCode(menuItem->opCode) != NULL)
    {
      break; // already
    }

#ifdef SYSTEM_ARDUINO_BASED
    if ((menuItem->pin <= 0)
      || (menuItem->pin > PIN_NUMBER_MAX)
      )
    {
      break;
    }
#endif // SYSTEM_ARDUINO_BASED

    byte wkIdx = 0;
    for (wkIdx = 0; wkIdx < TASK_OPCODE_MENU_ITEM_COUNT_MAX; wkIdx++)
    {
      if (this->item_List[wkIdx].enabled == false)
      {
        break;
      }
    }

    if (wkIdx >= TASK_OPCODE_MENU_ITEM_COUNT_MAX)
    {
      break; // full
    }

    outIndex = wkIdx;
    this->item_List[outIndex].notIsr = menuItem->notIsr;
    this->item_List[outIndex].opCode = menuItem->opCode;
    this->item_List[outIndex].cbOnPressEx = menuItem->cbOnPressEx;
    this->item_List[outIndex].cbOnPressExArg = menuItem->cbOnPressExArg;
    this->item_List[outIndex].pin = menuItem->pin;
    this->item_List[outIndex].triggerType = menuItem->triggerType;
    memcpy(this->item_List[outIndex].description, menuItem->description, TASK_OPCODE_MENU_ITEM_DESCRIPTION_LEN_MAX);
#ifdef SYSTEM_ARDUINO_BASED
    PIN_MODE(menuItem->pin, menuItem->gpioFunc);
    if (menuItem->notIsr == false)
    {
#ifdef TASK_OPCODE_CONSOLE_DEBUG_ENABLE
      CONSOLE_LOG_ISR(taskOpCodeLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[opM] ap %d %d", 0, outIndex);
#endif // TASK_OPCODE_CONSOLE_DEBUG_ENABLE
      ATTACH_ISR(menuItem->pin, this->item_Isr[outIndex], menuItem->triggerType);
    }
#endif // SYSTEM_ARDUINO_BASED
    this->item_List[outIndex].isSelected = 0;
    this->item_List[outIndex].lastSelected = 0;
    this->item_List[outIndex].enabled = 1;
    this->item_Count++;

    if (outIndexP != NULL)
    {
      *outIndexP = outIndex;
    }
    return 0;
  } while (0);

  outIndex = TASK_OPCODE_MENU_ITEM_INDEX_INVALID;
  if (outIndexP != NULL)
  {
    *outIndexP = outIndex;
  }
  return -1;
}

int OpCodeMenu::removeItemByIndex(byte opIndex)
{
  do
  {
    SystemCriticalLocker locker(this->critical_Key);
    if ((opIndex < TASK_OPCODE_MENU_ITEM_INDEX_MIN)
      || (opIndex > TASK_OPCODE_MENU_ITEM_INDEX_MAX)
      )
    {
      break;
    }

    OpCodeMenuItemTAG* menuItem = &this->item_List[opIndex];
    if (menuItem->enabled == false)
    {
      break;
    }

    return this->removeItem(menuItem);
  } while (0);
  return -1;
}

int OpCodeMenu::removeItemByOpCode(byte opCode)
{
  do
  {
    SystemCriticalLocker locker(this->critical_Key);
    if ((opCode < TASK_OPCODE_MENU_ITEM_OPCODE_MIN)
      || (opCode > TASK_OPCODE_MENU_ITEM_OPCODE_MAX)
      )
    {
      break;
    }

    OpCodeMenuItemTAG* menuItem = this->findItemByOpCode(opCode);
    if (menuItem == NULL)
    {
      break;
    }

    return this->removeItem(menuItem);
  } while (0);
  return -1;
}

byte OpCodeMenu::menuItemCount(void)
{
  return this->item_Count;
}


int OpCodeMenu::prepare(OpCodeMenuConfigTAG& menuConfig)
{
  byte wk_idx = 0;
  do
  {
    if (this->item_Count > 0)
    {
      return 0; // already
    }
    this->clear();
    if ((menuConfig.menuItems == NULL)
      || (menuConfig.menuItemCount <= 0)
      )
    {
      break;
    }
    this->bounce_Time = menuConfig.bounceTime;
    this->item_Count = 0;//
    byte itemCount = SYSTEM_MIN(menuConfig.menuItemCount, TASK_OPCODE_MENU_ITEM_COUNT_MAX);
    byte* outIndexP = NULL;
    for (wk_idx = 0; wk_idx < itemCount; wk_idx++)
    {
      if (menuConfig.outIndexs != NULL)
      {
        outIndexP = menuConfig.outIndexs + wk_idx;
      }

      if (this->appendItem(&menuConfig.menuItems[wk_idx], outIndexP) != 0)
      {
        break;
      }
    }

    if (wk_idx < itemCount)
    {
      break;
    }
    return 0;
  } while (0);
  this->clear();
  return -1;
}

void OpCodeMenu::clear(void)
{
  do
  {
    this->bounce_Time = 0;
    OpCodeMenuItemTAG* menuItem = NULL;
    {
      SystemCriticalLocker locker(this->critical_Key);
      for (byte wkIdx = 0; wkIdx < TASK_OPCODE_MENU_ITEM_COUNT_MAX; wkIdx++)
      {
        menuItem = &this->item_List[wkIdx];
        if (menuItem->enabled == false)
        {
          continue;
        }
        this->removeItem(menuItem);
      }
    }
    //this->item_Count = 0;
    return;
  } while (0);
  return;
}

int OpCodeMenu::removeItem(OpCodeMenuItemTAG* menuItem)
{
  do
  {
    if ( (menuItem == NULL)
      || (menuItem->enabled == false)
      )
    {
      break;
    }

    if ((menuItem->pin > 0)
      && (menuItem->pin <= PIN_NUMBER_MAX)
      )
    {
      PIN_MODE(menuItem->pin, GPIO_OUTPUT);
      if (menuItem->notIsr == 0)
      {
        DETTACH_ISR(menuItem->pin);
      }
    }
    memset(menuItem, 0, sizeof(OpCodeMenuItemTAG));
    this->item_Count--;
    return 0;
  } while (0);
  return -1;
}

OpCodeMenuItemTAG* OpCodeMenu::findItemByOpCode(byte opCode)
{
  do
  {
    if ((opCode < TASK_OPCODE_MENU_ITEM_OPCODE_MIN)
      || (opCode > TASK_OPCODE_MENU_ITEM_OPCODE_MAX)
      )
    {
      break;
    }

    byte wkIdx = 0;
    for (wkIdx = 0; wkIdx < TASK_OPCODE_MENU_ITEM_COUNT_MAX; wkIdx++)
    {
      if ((this->item_List[wkIdx].enabled != false)
        && (this->item_List[wkIdx].opCode == opCode)
        )
      {
        return &this->item_List[wkIdx];
      }
    }

    return NULL;
  } while (0);
  return NULL;
}

void OpCodeMenu::display(void)
{
#ifdef SYSTEM_ARDUINO_BASED
  OpCodeMenuItemTAG* menuItem = NULL;
  CONSOLE_LOG_BUF(taskOpCodeLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "%s", "pin/opCode:");
  for (byte wk_idx = 0; wk_idx < TASK_OPCODE_MENU_ITEM_COUNT_MAX; wk_idx++)
  {
    menuItem = &this->item_List[wk_idx];
    if (menuItem->enabled == false)
    {
      continue;
    }
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

bool OpCodeMenu::isPressedByIndex(byte opIndex)
{
  do
  {
    SystemCriticalLocker locker(this->critical_Key);
    if ((opIndex < TASK_OPCODE_MENU_ITEM_INDEX_MIN)
      || (opIndex > TASK_OPCODE_MENU_ITEM_INDEX_MAX)
      )
    {
      break;
    }

    OpCodeMenuItemTAG* menuItem = &this->item_List[opIndex];
    if (menuItem->enabled == false)
    {
      break;
    }

    return this->isPressed(menuItem);
  } while (0);
  return false;

}

bool OpCodeMenu::isPressedByOpCode(byte opCode)
{
  do
  {
    SystemCriticalLocker locker(this->critical_Key);
    if ((opCode < TASK_OPCODE_MENU_ITEM_OPCODE_MIN)
      || (opCode > TASK_OPCODE_MENU_ITEM_OPCODE_MAX)
      )
    {
      break;
    }

    OpCodeMenuItemTAG* menuItem = this->findItemByOpCode(opCode);
    if (menuItem == NULL)
    {
      break;
    }

    return this->isPressed(menuItem);
  } while (0);
  return false;
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

bool OpCodeMenu::isPressed(OpCodeMenuItemTAG* menuItem)
{
  bool isPressed = false;
  int state = SYSTEM_PIN_LOW;
  do
  {
    if (menuItem == NULL)
    {
      break;
    }

    isPressed = menuItem->isSelected; //via isr
    if (menuItem->notIsr != false)
    {
      state = SYSTEM_PIN_DIGITAL_READ(menuItem->pin);
      if ((menuItem->triggerType == ISR_ONLOW)
        && (state == SYSTEM_PIN_LOW)
        )
      {
        isPressed = true;
      }
      else if ((menuItem->triggerType == ISR_ONHIGH)
        && (state == SYSTEM_PIN_HIGH)
        )
      {
        isPressed = true;
      }
      else
      {
        isPressed = false;
      }
    }
    return isPressed;
  } while (0);
  return false;
}

int OpCodeMenu::getOpCode(void)
{
  int opCode = -1;
  OpCodeMenuItemTAG* menuItem = NULL;
  bool isPressed = false;
  for (byte wk_idx = 0; wk_idx < TASK_OPCODE_MENU_ITEM_COUNT_MAX; wk_idx++)
  {
    menuItem = &this->item_List[wk_idx];
    if (menuItem->enabled == 0)
    {
      continue;
    }
    isPressed = this->isPressed(menuItem);

    if ( (opCode < 0) 
      && (isPressed != false)
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

bool OpCodeMenu::isPressed(OpCodeMenuItemTAG* menuItem)
{
  bool isPressed = false;
  do
  {
    if (menuItem == NULL)
    {
      break;
    }

    isPressed = menuItem->isSelected; //via isr
    return isPressed;
  } while (0);
  return false;

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
    memset(outOpCodes, TASK_OPCODE_MENU_ITEM_OPCODE_INVALID, outOpCodeMaxCount);
#ifdef SYSTEM_ARDUINO_BASED
    OpCodeMenuItemTAG* menuItem = NULL;
    for (byte wk_idx = 0; wk_idx < TASK_OPCODE_MENU_ITEM_COUNT_MAX; wk_idx++)
    {
      if (outOpCodeCount >= outOpCodeMaxCount)
      {
        break;
      }

      menuItem = &this->item_List[wk_idx];
      if (menuItem->enabled == false)
      {
        continue;
      }

      if (this->isPressed(menuItem) != false)
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

void OpCodeMenu::onButtonPressed(byte buttonIndex, bool* woken)
{
#ifdef SYSTEM_ARDUINO_BASED
  do
  {
#ifdef TASK_OPCODE_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_ISR(taskOpCodeLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[opM] cbBP %d %d", 0, buttonIndex);
#endif // TASK_OPCODE_CONSOLE_DEBUG_ENABLE
    if (buttonIndex >= TASK_OPCODE_MENU_ITEM_COUNT_MAX)
    {
#ifdef TASK_OPCODE_CONSOLE_DEBUG_ENABLE
      CONSOLE_LOG_ISR(taskOpCodeLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[opM] cbBP %d", -10);
#endif // TASK_OPCODE_CONSOLE_DEBUG_ENABLE
      break;
    }
    TimePoint_t nowMs = TimerManager::getInstance().nowMsFromISR();
    OpCodeMenuItemTAG* menuItem = &this->item_List[buttonIndex];
    if ((nowMs - menuItem->lastSelected) < (this->bounce_Time)) // * TIMER_DEVICE_TIMER_MS_2_UNIT))
    { // Debounce
#ifdef TASK_OPCODE_CONSOLE_DEBUG_ENABLE
      CONSOLE_LOG_ISR(taskOpCodeLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[opM] cbBP %d", -20);
#endif // TASK_OPCODE_CONSOLE_DEBUG_ENABLE
      return;
    }
    menuItem->isSelected = true;
    menuItem->lastSelected = nowMs;
    if ((menuItem->notIsr != false)
      || (menuItem->cbOnPressEx == NULL)
      )
    {
#ifdef TASK_OPCODE_CONSOLE_DEBUG_ENABLE
      CONSOLE_LOG_ISR(taskOpCodeLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[opM] cbBP %d", -30);
#endif // TASK_OPCODE_CONSOLE_DEBUG_ENABLE
      return;
    }

    menuItem->cbOnPressEx(menuItem->cbOnPressExArg, menuItem->opCode, woken);
#ifdef TASK_OPCODE_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_ISR(taskOpCodeLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[opM] cbBP %d", 99);
#endif // TASK_OPCODE_CONSOLE_DEBUG_ENABLE
  } while (0);
#else
  return;
#endif // #SYSTEM_ARDUINO_BASED
}

void OpCodeMenu::isrButtonPressed00(void)
{
  bool woken = false;
  OpCodeMenu::getInstance().onButtonPressed(0, &woken);
  SYSTEM_YEILD_FROM_ISR(woken);
}

void OpCodeMenu::isrButtonPressed01(void)
{
  bool woken = false;
  OpCodeMenu::getInstance().onButtonPressed(1, &woken);
  SYSTEM_YEILD_FROM_ISR(woken);
}

void OpCodeMenu::isrButtonPressed02(void)
{
  bool woken = false;
  OpCodeMenu::getInstance().onButtonPressed(2, &woken);
  SYSTEM_YEILD_FROM_ISR(woken);
}

void OpCodeMenu::isrButtonPressed03(void)
{
  bool woken = false;
  OpCodeMenu::getInstance().onButtonPressed(3, &woken);
  SYSTEM_YEILD_FROM_ISR(woken);
}

void OpCodeMenu::isrButtonPressed04(void)
{
  bool woken = false;
  OpCodeMenu::getInstance().onButtonPressed(4, &woken);
  SYSTEM_YEILD_FROM_ISR(woken);
}

void OpCodeMenu::isrButtonPressed05(void)
{
  bool woken = false;
  OpCodeMenu::getInstance().onButtonPressed(5, &woken);
  SYSTEM_YEILD_FROM_ISR(woken);
}

void OpCodeMenu::isrButtonPressed06(void)
{
  bool woken = false;
  OpCodeMenu::getInstance().onButtonPressed(6, &woken);
  SYSTEM_YEILD_FROM_ISR(woken);
}

void OpCodeMenu::isrButtonPressed07(void)
{
  bool woken = false;
  OpCodeMenu::getInstance().onButtonPressed(7, &woken);
  SYSTEM_YEILD_FROM_ISR(woken);
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

int OpcodeInputTask::appendMenuItem(OpCodeMenuItemConfigTAG* menuItem, byte* outIndex)
{
  return OpCodeMenu::getInstance().appendItem(menuItem, outIndex);
}

int OpcodeInputTask::prepare(void)
{
  return 0;
}


int OpcodeInputTask::start(OpCodeInputTaskConfigTAG& inputTaskConfig) //TaskThreadConfigTAG& threadConfig)
{
  do
  {
    int result = 0;
    if (this->isRunning() != false)
    {
      return 0;
    }

    this->stop();

    if (inputTaskConfig.menuConfig.cbOnOpCodeRev == NULL)
    {
      break;
    }
#ifdef SYSTEM_PC_BASED
    inputTaskConfig.useTask = false; // always
#endif // SYSTEM_PC_BASED

    this->cb_OpCode_Rev = inputTaskConfig.menuConfig.cbOnOpCodeRev;
    this->use_Task = inputTaskConfig.useTask;
    result = OpCodeMenu::getInstance().prepare(inputTaskConfig.menuConfig);
    if (result != 0)
    {
      break;
    }

    if (this->use_Task == false)
    {
      this->isRunning(true);
      this->proc();
      return result;
    }

    // Set up tasks
    {
      this->registerHanldingEventStructSize(sizeof(EventButtonPressedParamsTAG));
      result = TaskManager::setConfig(inputTaskConfig.taskConfig);
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

    result = TaskManager::startProcess(inputTaskConfig.threadConfig, true);
    if (result != 0)
    {
      break;
    }

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
  this->cb_OpCode_Rev = NULL;
  this->use_Task = false;
  OpCodeMenu::getInstance().clear();
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
