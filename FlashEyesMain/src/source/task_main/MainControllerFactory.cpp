/////////////////////////////////////////////////
// INCLUDE
#include "MainControllerFactory.h"

#if (_CONF_MAIN_CONTROLLER_FACTORY_ENABLED)
#include "MainResetController.h"
#include "MainSettingController.h"
#include "MainNormalController.h"

/////////////////////////////////////////////////
// PREPROCESSOR

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

/////////////////////////////////////////////////
// STATIC FUNCTIONS

/////////////////////////////////////////////////
// EXTERN

/////////////////////////////////////////////////
// CLASS IMPLEMENTAION
/*MainControllerFactory*/
MainControllerFactory::MainControllerFactory(void)
{

}

MainControllerFactory::~MainControllerFactory(void)
{

}

MainController* MainControllerFactory::generate(byte systemMode)
{
  MainController* mainController = NULL;
  do
  {
    switch (systemMode)
    {
      case SYS_MODE_RESET:
        mainController = new MainResetController();
        break;
      case SYS_MODE_SETTING:
        mainController = new MainSettingController();
        break;
      case SYS_MODE_NORMAL:
        mainController = new MainNormalController();
        break;
      default:
        break;
    }
    
    return mainController;
  } while (0);

  MainControllerFactory::release(mainController);
  return mainController;
}

void MainControllerFactory::release(MainController*& mainController)
{
  do
  {
    if (mainController == NULL)
    {
      break;
    }
    delete mainController;
    mainController = NULL;
    return;
  } while (0);
  return;
}

#endif // _CONF_MAIN_CONTROLLER_FACTORY_ENABLED