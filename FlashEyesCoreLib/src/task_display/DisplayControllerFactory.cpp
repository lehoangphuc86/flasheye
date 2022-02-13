/////////////////////////////////////////////////
// INCLUDE
#include "DisplayControllerFactory.h"

#if (_CONF_DISPLAY_CONTROLLER_FACTORY_ENABLED)
#include "DisplayLCDController.h"
#include "DisplayLCDCI2CController.h"
#include "DisplaySCController.h"
#include "DisplayJMDController.h"
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
/*DisplayControllerFactory*/
DisplayControllerFactory::DisplayControllerFactory(void)
{

}

DisplayControllerFactory::~DisplayControllerFactory(void)
{

}

DisplayController* DisplayControllerFactory::generate(byte deviceType)
{
  DisplayController* controller = NULL;
  do
  {
    switch (deviceType)
    {
    case DisplayDeviceTypeUN::DisplayLCDDevice:
      controller = new DisplayLCDController();
      break;
    case DisplayDeviceTypeUN::DisplayLCDI2CDevice:
      controller = new DisplayLCDI2CController();
      break;
    case DisplayDeviceTypeUN::DisplaySerialConsoleDevice:
      controller = new DisplaySCController();
      break;
    case DisplayDeviceTypeUN::DisplayJMDDevice:
      controller = new DisplayJMDController();
      break;
    default:
      break;
    }

    return controller;
  } while (0);

  DisplayControllerFactory::release(controller);
  return controller;
}

void DisplayControllerFactory::release(DisplayController*& controller)
{
  do
  {
    if (controller == NULL)
    {
      break;
    }
    delete controller;
    controller = NULL;
    return;
  } while (0);
  return;
}

#endif // _CONF_DISPLAY_CONTROLLER_FACTORY_ENABLED