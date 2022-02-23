/////////////////////////////////////////////////
// INCLUDE
#include "ExportProcessorFactory.h"

#if (_CONF_EXPORT_PROCESSOR_FACTORY_ENABLED)
#include "ExportHttpClientProcessor.h"
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
/*ExportProcessorFactory*/
ExportProcessorFactory::ExportProcessorFactory(void)
{

}

ExportProcessorFactory::~ExportProcessorFactory(void)
{

}

ExportProcessor* ExportProcessorFactory::generate(byte procType)
{
  ExportProcessor* dpProcessor = NULL;
  do
  {
    switch (procType)
    {
    case ExportProcessorTypeUN::ExportProcessorHttpClient:
      dpProcessor = new ExportHttpClientProcessor();
      break;
    default:
      break;
    }

    return dpProcessor;
  } while (0);

  ExportProcessorFactory::release(dpProcessor);
  return dpProcessor;
}

void ExportProcessorFactory::release(ExportProcessor*& exportProcessor)
{
  do
  {
    if (exportProcessor == NULL)
    {
      break;
    }
    delete exportProcessor;
    exportProcessor = NULL;
    return;
  } while (0);
  return;
}

#endif // _CONF_EXPORT_PROCESSOR_FACTORY_ENABLED