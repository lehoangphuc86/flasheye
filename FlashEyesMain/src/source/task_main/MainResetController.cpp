/////////////////////////////////////////////////
// INCLUDE
#include "MainResetController.h"
#if (_CONF_MAIN_RESET_CONTROLLER_ENABLED)
#include "../db/DBManager.h"
#include "../task_ui/UiManager.h"
#include "../task_switch/SwitchManager.h"
/////////////////////////////////////////////////
// PREPROCESSOR
//#define MAIN_RESET_CONTROLLER_CONSOLE_DEBUG_ENABLE
/////////////////////////////////////////////////
// DEFINE


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
#ifdef MAIN_RESET_CONTROLLER_CONSOLE_DEBUG_ENABLE
char mainResetControllerLogBuf[SYSTEM_CONSOLE_OUT_BUF_LEN];
#endif // MAIN_RESET_CONTROLLER_CONSOLE_DEBUG_ENABLE
/////////////////////////////////////////////////
// STATIC FUNCTIONS

/////////////////////////////////////////////////
// EXTERN

/////////////////////////////////////////////////
// CLASS IMPLEMENTAION
/*MainResetController*/

MainResetController::MainResetController(void)
  : MainController(SYS_MODE_RESET)
  , sql_Script_Buff(NULL)
{

}

MainResetController::~MainResetController(void)
{

}

bool MainResetController::isValid(void)
{
  do
  {

    return MainController::isValid();
  } while (0);
  return false;
}

void MainResetController::regEventSize(void)
{

}

int MainResetController::prepare(void)
{
  int ret = 0;
  do
  {
    ret = MainController::prepare();
    if (ret != 0)
    {
      break;
    }

    if (DBManager::getInstance().isValid() == false)
    {
      break;
    }

    // tmp sql cmd
    this->sql_Script_Buff = new char[DB_SCRIPT_LINE_LEN_MAX];
    if (this->sql_Script_Buff == NULL)
    {
      break;
    }

    return 0;
  } while (0);
  return -1;
}

void MainResetController::clear(void)
{
  do
  {
    MainController::clear();
    if (this->sql_Script_Buff != NULL)
    {
      delete[] this->sql_Script_Buff;
    }
    this->sql_Script_Buff = NULL;
    return;
  } while (0);
  return;
}

int MainResetController::onEventHandling(EventDataItem* eventData)
{
  int ret = 0;
  SysMode_t swButton = 0;
  do
  {
    UiManager::getInstance().showSysState(UI_MESS_SYS_STATE_RESETTING, UI_MESS_SYS_STATE_SUB_START);
    // user can shutdown to cancel
    // wait until 1 button pressed
    //  cancel if reset button pressed
    //  reset db if scanning button pressed
    byte pressCount = FEM_SW_PRESS_TIME_REQUIRED;
    {
      SysMode_t swState = 0;
      do
      {
        SYSTEM_SLEEP(500);
        swState = SwitchManager::getInstance().getState();
#ifdef MAIN_RESET_CONTROLLER_CONSOLE_DEBUG_ENABLE
        CONSOLE_LOG_BUF(mainResetControllerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mre] oEh %i %i", 10, swState);
#endif // MAIN_RESET_CONTROLLER_CONSOLE_DEBUG_ENABLE
        if (swState == FEM_SW_SYS_MODE_SETTING_BIT)
        {
          pressCount++; // cancel
        }
        else if (swState == FEM_SW_SYS_MODE_SCANING_BIT)
        {
          pressCount--; // proceed
        }

        if ( (pressCount <= 0)
          || (pressCount >= FEM_SW_PRESS_TIME_REQUIRED*2)
          )
        {
          break;
        }
      } while (1);
    }
    
    if (pressCount >= FEM_SW_PRESS_TIME_REQUIRED * 2) // canceled
    {
      // cancel
      UiManager::getInstance().showSysState(UI_MESS_SYS_STATE_RESETTING, UI_MESS_SYS_STATE_SUB_CANCEL);
      // restart
      ret = 0;
      goto _onEventHandling_End;
    }

    UiManager::getInstance().showSysState(UI_MESS_SYS_STATE_RESETTING, UI_MESS_SYS_STATE_SUB_RUNNING);
    // load script file setting
    // reset db
    ret = DBManager::getInstance().resetDB(g_FEM_Db_Tbl_Scripts, FEM_DB_TBL_ID_MAX, this->sql_Script_Buff, DB_SCRIPT_LINE_LEN_MAX);
    if (ret != 0)
    {
      break;
    }

    // write to db sysmode next
    SettingManager::getInstance().system().lastSysMode(SYS_MODE_SETTING, true);
    
#ifdef MAIN_RESET_CONTROLLER_CONSOLE_DEBUG_ENABLE
    CONSOLE_LOG_BUF(mainResetControllerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mre] oEh %i", 22);
#endif // MAIN_RESET_CONTROLLER_CONSOLE_DEBUG_ENABLE
    // restart
    UiManager::getInstance().showSysState(UI_MESS_SYS_STATE_RESETTING, UI_MESS_SYS_STATE_SUB_END);
    ret = 0;
    goto _onEventHandling_End;
  } while (0);

  UiManager::getInstance().showSysState(UI_MESS_SYS_STATE_RESETTING, UI_MESS_SYS_STATE_SUB_ERROR);
  ret = -1;
_onEventHandling_End:
  // restart
#ifdef MAIN_RESET_CONTROLLER_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(mainResetControllerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mre] oEh %i %i", 98, ret);
#endif // MAIN_RESET_CONTROLLER_CONSOLE_DEBUG_ENABLE
  SYSTEM_SLEEP(1000); // wait 1sec
  SYSTEM_REBOOT();  
#ifdef MAIN_RESET_CONTROLLER_CONSOLE_DEBUG_ENABLE
  CONSOLE_LOG_BUF(mainResetControllerLogBuf, SYSTEM_CONSOLE_OUT_BUF_LEN, "[mre] oEh %i %i", 99, ret);
#endif // MAIN_RESET_CONTROLLER_CONSOLE_DEBUG_ENABLE
  return ret;
}
#endif // _CONF_MAIN_RESET_CONTROLLER_ENABLED