#include "ButtonManager.h"
#if (_CONF_BUTTON_MANAGER_ENABLED)
/////////////////////////////////////////////////
// INCLUDE
#include "task_opcode/TaskOpcode.h"
/////////////////////////////////////////////////
// PREPROCESSOR

/////////////////////////////////////////////////
// DEFINE
#define BUTTON_MANAGER_CONSOLE_DEBUG_ENABLE
/////////////////////////////////////////////////
// MARCO

/////////////////////////////////////////////////
// GLOBAL VARIABLES
#ifdef BUTTON_MANAGER_CONSOLE_DEBUG_ENABLE
char btnManagerLogBuf[SYSTEM_CONSOLE_OUT_BUF_LEN];
#endif // BUTTON_MANAGER_CONSOLE_DEBUG_ENABLE
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

/* ButtonManager*/
ButtonManager& ButtonManager::getInstance(void)
{
  static ButtonManager instance;
  return instance;
}

ButtonManager::ButtonManager(void)
  : btn_Count(0)
{
  memset(this->btn_Opcode, 0, TASK_BUTTON_ITEM_COUNT_MAX * sizeof(byte));
}

ButtonManager::~ButtonManager(void)
{
  this->stop();
}

bool ButtonManager::isValid(void)
{
  return (this->btn_Count == 0? false : true);
}

int ButtonManager::start(ButtonManagerConfigTAG& btnManagerConfig)
{
  int ret = 0;
  do
  {
    if (this->isValid() != false)
    {
      return -1; // already
    }

    this->stop();
    if ( (btnManagerConfig.config.bounceTime < 0)
      || (btnManagerConfig.config.items == NULL)
      || (btnManagerConfig.config.itemCount < TASK_BUTTON_ITEM_COUNT_MIN)
      || (btnManagerConfig.config.itemCount > TASK_BUTTON_ITEM_COUNT_MAX)
      )
    {
      break;
    }

    this->btn_Count = btnManagerConfig.config.itemCount;

    OpCodeMenuItemConfigTAG menuItems[TASK_BUTTON_ITEM_COUNT_MAX];
    for (byte wkIdx = 0; wkIdx < this->btn_Count; wkIdx++)
    {
      menuItems[wkIdx].pin = btnManagerConfig.config.items[wkIdx].pin;
      menuItems[wkIdx].gpioFunc = btnManagerConfig.config.items[wkIdx].gpioFunc;
      menuItems[wkIdx].triggerType = btnManagerConfig.config.items[wkIdx].triggerType;
      menuItems[wkIdx].opCode = btnManagerConfig.config.items[wkIdx].opCode;
      menuItems[wkIdx].notIsr = 0;
      menuItems[wkIdx].cbOnPressEx = btnManagerConfig.config.items[wkIdx].cbOnPressedEx;
      menuItems[wkIdx].cbOnPressExArg = btnManagerConfig.config.items[wkIdx].cbOnPressedExArg;
      menuItems[wkIdx].description[0] = '\0';
      this->btn_Opcode[wkIdx] = menuItems[wkIdx].opCode;
    }

    OpCodeMenuConfigTAG menuConfig = OpCodeMenuConfigTAG();
    menuConfig.menuItems = &menuItems[0];
    menuConfig.menuItemCount = this->btn_Count;
    menuConfig.bounceTime = btnManagerConfig.config.bounceTime;
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

bool ButtonManager::isPressed(byte opCode)
{
  return OpCodeMenu::getInstance().isPressedByOpCode(opCode);
}

void ButtonManager::stop(void)
{
  for (byte wkIdx = 0; wkIdx < this->btn_Count; wkIdx++)
  {
    OpCodeMenu::getInstance().removeItemByOpCode(this->btn_Opcode[wkIdx]);
  }

  memset(this->btn_Opcode, TASK_OPCODE_MENU_ITEM_OPCODE_INVALID, TASK_BUTTON_ITEM_COUNT_MAX * sizeof(byte));
  this->btn_Count = 0;
}

#endif // _CONF_SWITCH_MANAGER_ENABLED
