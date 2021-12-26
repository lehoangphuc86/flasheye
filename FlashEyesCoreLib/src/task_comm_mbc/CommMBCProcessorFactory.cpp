/////////////////////////////////////////////////
// INCLUDE
#include "CommMBCProcessorFactory.h"

#if (_CONF_COMM_MBC_PROCESSOR_FACTORY_ENABLED)
#include "CommMBCPureDataProcessor.h"
#include "CommMBCJsonDataProcessor.h"

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
/*CommMBCProcessorFactory*/
CommMBCProcessorFactory::CommMBCProcessorFactory(void)
{

}

CommMBCProcessorFactory::~CommMBCProcessorFactory(void)
{

}

CommMBCProcessor* CommMBCProcessorFactory::generate(byte processorType)
{
  CommMBCProcessor* mbcProcessor = NULL;
  do
  {
    switch (processorType)
    {
      case CommMBCDataType::CommMBCPureData:
        mbcProcessor = new CommMBCPureDataProcessor();
        break;
      case CommMBCDataType::CommMbcJsonData:
        mbcProcessor = new CommMBCJsonDataProcessor();
        break;
      default:
        break;
    }
    
    return mbcProcessor;
  } while (0);

  CommMBCProcessorFactory::release(mbcProcessor);
  return mbcProcessor;
}

void CommMBCProcessorFactory::release(CommMBCProcessor*& mbcProcessor)
{
  do
  {
    if (mbcProcessor == NULL)
    {
      break;
    }
    delete mbcProcessor;
    mbcProcessor = NULL;
    return;
  } while (0);
  return;
}

#endif // _CONF_COMM_MBC_PROCESSOR_FACTORY_ENABLED