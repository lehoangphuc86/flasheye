#include "CommMBCConstant.h"

#if (_CONF_COMM_MBC_PROCESSOR_FACTORY_ENABLED)

#ifndef _COMM_MBC_PROCESSOR_FACTORY_H
#define _COMM_MBC_PROCESSOR_FACTORY_H

/////////////////////////////////////////////////
// INCLUDE
#include "CommMBCProcessor.h"
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
/*CommMBCProcessorFactory*/
class CommMBCProcessorFactory
{
public:
  CommMBCProcessorFactory(void);
  // WARNING: if inherite from this class, deconstructor must be virtual
  __ATTRIBUTE_VIRTUAL_OPTIMIZED ~CommMBCProcessorFactory(void);
public:
  static CommMBCProcessor*                                      generate(byte processorType);
  static void                                                   release(CommMBCProcessor*& mbcProcessor);
};
#endif // _COMM_MBC_PROCESSOR_FACTORY_H

#endif // _CONF_COMM_MBC_PROCESSOR_FACTORY_ENABLED