/////////////////////////////////////////////////
// INCLUDE
#include "DistSensorControllerFactory.h"

#if (_CONF_DIST_SENSOR_CONTROLLER_FACTORY_ENABLED)
#include "DistSensorHCSR04Controller.h"
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
/*DistSensorControllerFactory*/
DistSensorControllerFactory::DistSensorControllerFactory(void)
{

}

DistSensorControllerFactory::~DistSensorControllerFactory(void)
{

}

DistSensorController* DistSensorControllerFactory::generate(byte deviceType)
{
  DistSensorController* controller = NULL;
  do
  {
    switch (deviceType)
    {
    case DistSensorDeviceTypeUN::DistSensorHRCS04Device:
      controller = new DistSensorHCSR04Controller();
      break;
    default:
      break;
    }

    return controller;
  } while (0);

  DistSensorControllerFactory::release(controller);
  return controller;
}

void DistSensorControllerFactory::release(DistSensorController*& controller)
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

#endif // _CONF_DIST_SENSOR_CONTROLLER_FACTORY_ENABLED