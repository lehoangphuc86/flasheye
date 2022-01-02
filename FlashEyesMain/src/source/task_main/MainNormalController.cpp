/////////////////////////////////////////////////
// INCLUDE
#include "MainNormalController.h"
#if (_CONF_MAIN_NORMAL_CONTROLLER_ENABLED)
/////////////////////////////////////////////////
// PREPROCESSOR
#define MAIN_NORMAL_CONTROLLER_CONSOLE_DEBUG_ENABLE
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
#ifdef MAIN_NORMAL_CONTROLLER_CONSOLE_DEBUG_ENABLE
char mainNormalControllerLogBuf[SYSTEM_CONSOLE_OUT_BUF_LEN];
#endif // MAIN_NORMAL_CONTROLLER_CONSOLE_DEBUG_ENABLE
/////////////////////////////////////////////////
// STATIC FUNCTIONS

/////////////////////////////////////////////////
// EXTERN

/////////////////////////////////////////////////
// CLASS IMPLEMENTAION
/*MainNormalController*/

MainNormalController::MainNormalController(void)
  : MainController((byte)SystemMode::NormalMode)
{

}

MainNormalController::~MainNormalController(void)
{
  //@@
}

bool MainNormalController::isValid(void)
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

int MainNormalController::setConfig(MainControllerConfigTAG& controllerConfig)
{
  //@@
  return 0;
}

int MainNormalController::start(void)
{
  //@@
  return 0;
}

void MainNormalController::stop(void)
{
  //@@
}

void MainNormalController::cleanUp(void)
{
  //@@
}

void MainNormalController::regEventSize(void)
{
  //@@
  
}

int MainNormalController::prepare(void)
{
  //@@
  return 0;
}

#endif // _CONF_MAIN_NORMAL_CONTROLLER_ENABLED