#include "MainController.h"

#if (_CONF_MAIN_CONTROLLER_FACTORY_ENABLED)

#ifndef _MAIN_CONTROLLER_FACTORY_H
#define _MAIN_CONTROLLER_FACTORY_H

/////////////////////////////////////////////////
// INCLUDE

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
/*MainControllerFactory*/
class MainControllerFactory
{
public:
  MainControllerFactory(void);
  // WARNING: if inherite from this class, deconstructor must be virtual
  __ATTRIBUTE_VIRTUAL_OPTIMIZED ~MainControllerFactory(void);
public:
  static MainController*                                        generate(byte systemMode);
  static void                                                   release(MainController*& mainController);
};
#endif // _MAIN_CONTROLLER_FACTORY_H

#endif // _CONF_MAIN_CONTROLLER_FACTORY_ENABLED