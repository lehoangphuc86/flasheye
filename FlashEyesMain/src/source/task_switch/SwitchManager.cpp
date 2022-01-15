#include "SwitchManager.h"
#if (_CONF_SWITCH_MANAGER_ENABLED)
/////////////////////////////////////////////////
// INCLUDE
#include "task_opcode/TaskOpcode.h"
/////////////////////////////////////////////////
// PREPROCESSOR

/////////////////////////////////////////////////
// DEFINE
//#define SWITCH_MANAGER_CONSOLE_DEBUG_ENABLE
/////////////////////////////////////////////////
// MARCO

/////////////////////////////////////////////////
// GLOBAL VARIABLES
#ifdef SWITCH_MANAGER_CONSOLE_DEBUG_ENABLE
char swManagerLogBuf[SYSTEM_CONSOLE_OUT_BUF_LEN];
#endif // SWITCH_MANAGER_CONSOLE_DEBUG_ENABLE
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

/* SwitchManager*/
SwitchManager& SwitchManager::getInstance(void)
{
  static SwitchManager instance;
  return instance;
}

SwitchManager::SwitchManager(void)
  : sw_Count(0)
{
  memset(this->sw_Opcode, 0, TASK_SWITCH_ITEM_COUNT_MAX * sizeof(byte));
}

SwitchManager::~SwitchManager(void)
{
  this->stop();
}

bool SwitchManager::isValid(void)
{
  return (this->sw_Count == 0? false : true);
}

int SwitchManager::start(SwitchManagerConfigTAG& swManagerConfig)
{
  int ret = 0;
  do
  {
    if (this->isValid() != false)
    {
      return -1; // already
    }

    this->stop();
    if ( (swManagerConfig.switchConfig.bounceTime < 0)
      || (swManagerConfig.switchConfig.switchItems == NULL)
      || (swManagerConfig.switchConfig.switchItemCount < TASK_SWITCH_ITEM_COUNT_MIN)
      || (swManagerConfig.switchConfig.switchItemCount > TASK_SWITCH_ITEM_COUNT_MAX)
      )
    {
      break;
    }

    this->sw_Count = swManagerConfig.switchConfig.switchItemCount;

    OpCodeMenuItemTAG menuItems[TASK_SWITCH_ITEM_COUNT_MAX];
    for (byte wkIdx = 0; wkIdx < this->sw_Count; wkIdx++)
    {
      menuItems[wkIdx].pin = swManagerConfig.switchConfig.switchItems[wkIdx].pin;
      menuItems[wkIdx].gpioFunc = swManagerConfig.switchConfig.switchItems[wkIdx].gpioFunc;
      menuItems[wkIdx].triggerType = swManagerConfig.switchConfig.switchItems[wkIdx].triggerType;
      menuItems[wkIdx].opCode = swManagerConfig.switchConfig.switchItems[wkIdx].opCode;
      menuItems[wkIdx].notIsr = 1;
      menuItems[wkIdx].cbOnPressed = NULL;
      menuItems[wkIdx].description[0] = '\0';
      this->sw_Opcode[wkIdx] = menuItems[wkIdx].opCode;
    }

    OpCodeMenuConfigTAG menuConfig = OpCodeMenuConfigTAG();
    menuConfig.menuItems = &menuItems[0];
    menuConfig.menuItemCount = this->sw_Count;
    menuConfig.bounceTime = swManagerConfig.switchConfig.bounceTime;
    menuConfig.outIndexs = NULL;
    ret = OpCodeMenu::getInstance().prepare(menuConfig);
    if (ret != 0)
    {
      break;
    }

    return 0;
  } while (0);
  this->stop();
  return -1;
}

SysMode_t SwitchManager::getSystemMode(void)
{
  
  SysMode_t orOpCode = 0;
  bool isPressed = false;
  do
  {
    orOpCode = this->getState();
    switch (orOpCode)
    {
      case FEM_SW_SYS_MODE_SETTING_BIT: // reset button pressed only
        return SYS_MODE_SETTING;
      case FEM_SW_SYS_MODE_RESET_BIT: // both button pressed
        return SYS_MODE_RESET;
      default:
        return SYS_MODE_NORMAL;
    }

  } while (0);
  return SYS_MODE_INVALID;
}

SysMode_t SwitchManager::getState(void)
{
  SysMode_t orOpCode = 0; 
  bool isPressed = false;
  do
  {
    if (this->isValid() == false)
    {
      break;
    }

    for (byte wk_idx = 0; wk_idx < this->sw_Count; wk_idx++)
    {
      isPressed = this->isPressed(this->sw_Opcode[wk_idx]);// OpCodeMenu::getInstance().isPressedByOpCode(this->sw_Opcode[wk_idx]);
      if (isPressed != false)
      {
        orOpCode |= this->sw_Opcode[wk_idx];
      }
    }

    return orOpCode;
  } while (0);
  return 0;
}

bool SwitchManager::isPressed(byte opCode)
{
  return OpCodeMenu::getInstance().isPressedByOpCode(opCode);
}

void SwitchManager::stop(void)
{
  for (byte wkIdx = 0; wkIdx < this->sw_Count; wkIdx++)
  {
    OpCodeMenu::getInstance().removeItemByOpCode(this->sw_Opcode[wkIdx]);
  }

  memset(this->sw_Opcode, TASK_OPCODE_MENU_ITEM_OPCODE_INVALID, TASK_SWITCH_ITEM_COUNT_MAX * sizeof(byte));
  this->sw_Count = 0;
}

#endif // _CONF_SWITCH_MANAGER_ENABLED
