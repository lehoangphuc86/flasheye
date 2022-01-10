/////////////////////////////////////////////////
// INCLUDE
#include "ScanningControllerFactory.h"

#if (_CONF_SCANNING_CONTROLLER_FACTORY_ENABLED)
#include "ScanningEVController.h"
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
/*ScanningControllerFactory*/
ScanningControllerFactory::ScanningControllerFactory(void)
{

}

ScanningControllerFactory::~ScanningControllerFactory(void)
{

}

ScanningController* ScanningControllerFactory::generate(byte deviceType)
{
  ScanningController* controller = NULL;
  do
  {
    switch (deviceType)
    {
    case ScanningDeviceTypeUN::ScanningEVDevice:
      controller = new ScanningEVController();
      break;
    default:
      break;
    }

    return controller;
  } while (0);

  ScanningControllerFactory::release(controller);
  return controller;
}

void ScanningControllerFactory::release(ScanningController*& controller)
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

#endif // _CONF_SCANNING_CONTROLLER_FACTORY_ENABLED