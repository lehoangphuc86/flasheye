/////////////////////////////////////////////////
// INCLUDE
#include "MainSettingController .h"
#if (_CONF_MAIN_SETTING_CONTROLLER_ENABLED)
/////////////////////////////////////////////////
// PREPROCESSOR
#define MAIN_SETTING_CONTROLLER_CONSOLE_DEBUG_ENABLE
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
#ifdef MAIN_SETTING_CONTROLLER_CONSOLE_DEBUG_ENABLE
char mainSettingControllerLogBuf[SYSTEM_CONSOLE_OUT_BUF_LEN];
#endif // MAIN_SETTING_CONTROLLER_CONSOLE_DEBUG_ENABLE
/////////////////////////////////////////////////
// STATIC FUNCTIONS

/////////////////////////////////////////////////
// EXTERN

/////////////////////////////////////////////////
// CLASS IMPLEMENTAION
/*MainSettingController*/

MainSettingController::MainSettingController(void)
  : MainController((byte)SystemMode::SettingMode)
{

}

MainSettingController::~MainSettingController(void)
{
  //@@
}

bool MainSettingController::isValid(void)
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

int MainSettingController::setConfig(MainControllerConfigTAG& controllerConfig)
{
  //@@
  return 0;
}

int MainSettingController::start(void)
{
  //@@
  return 0;
}

void MainSettingController::stop(void)
{
  //@@
}

void MainSettingController::cleanUp(void)
{
  //@@
}

void MainSettingController::regEventSize(void)
{
  //@@
  
}

int MainSettingController::prepare(void)
{
  //@@
  return 0;
}

#endif // _CONF_MAIN_SETTING_CONTROLLER_ENABLED