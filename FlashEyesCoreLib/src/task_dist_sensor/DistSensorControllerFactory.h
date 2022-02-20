#include "DistSensorTaskConstant.h"

#if (_CONF_DIST_SENSOR_CONTROLLER_FACTORY_ENABLED)

#ifndef _DIST_SENSOR_CONTROLLER_FACTORY_H
#define _DIST_SENSOR_CONTROLLER_FACTORY_H

/////////////////////////////////////////////////
// INCLUDE
#include "DistSensorController.h"
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
// DATA TYPE (TYPEDEF)

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
// CLASS DEFINITION
/*DistSensorControllerFactory*/
class DistSensorControllerFactory
{
public:
  DistSensorControllerFactory(void);
  // WARNING: if inherite from this class, deconstructor must be virtual
  __ATTRIBUTE_VIRTUAL_OPTIMIZED ~DistSensorControllerFactory(void);
public:
  static DistSensorController*                                  generate(byte deviceType);
  static void                                                   release(DistSensorController*& controller);
};
#endif // _DIST_SENSOR_CONTROLLER_FACTORY_H

#endif // _CONF_DIST_SENSOR_CONTROLLER_FACTORY_ENABLED