/////////////////////////////////////////////////
// INCLUDE
#include "MainResetController.h"
#if (_CONF_MAIN_RESET_CONTROLLER_ENABLED)
/////////////////////////////////////////////////
// PREPROCESSOR
#define MAIN_RESET_CONTROLLER_CONSOLE_DEBUG_ENABLE
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
  : MainController((byte)SystemMode::ResetMode)
{

}

MainResetController::~MainResetController(void)
{
  //@@
}

bool MainResetController::isValid(void)
{
  do
  {
    /*if (this->js_Parser.isValid() == false)
    {
      break;
    }*/
    //@@
    return MainController::isValid();
  } while (0);
  return false;
}

void MainResetController::regEventSize(void)
{
  //@@
  
}

int MainResetController::prepare(void)
{
  //@@
  return 0;
}

void MainResetController::clear(void)
{

}

int MainResetController::onEventHandling(EventDataItem* eventData)
{
  return 0;
}
#endif // _CONF_MAIN_RESET_CONTROLLER_ENABLED