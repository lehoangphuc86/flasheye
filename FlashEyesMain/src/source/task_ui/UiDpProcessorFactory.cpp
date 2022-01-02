/////////////////////////////////////////////////
// INCLUDE
#include "UiDpProcessorFactory.h"

#if (_CONF_UI_DP_PROCESSOR_FACTORY_ENABLED)
#include "UiDpLcdProcessor.h"
#include "UiDpLcdI2CProcessor.h"
#include "UiDpSCProcessor.h"
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
/*UiDpProcessorFactory*/
UiDpProcessorFactory::UiDpProcessorFactory(void)
{

}

UiDpProcessorFactory::~UiDpProcessorFactory(void)
{

}

UiDpProcessor* UiDpProcessorFactory::generate(byte procType)
{
  UiDpProcessor* dpProcessor = NULL;
  do
  {
    switch (procType)
    {
    case UIControlTypeUN::UIControlLCDDevice:
      dpProcessor = new UiDpLcdProcessor();
      break;
    case UIControlTypeUN::UIControlLCDI2CDevice:
      dpProcessor = new UiDpLcdI2CProcessor();
      break;
    case UIControlTypeUN::UIControlSerialConsoleDevice:
      dpProcessor = new UiDpSCProcessor();
      break;
    default:
      break;
    }

    return dpProcessor;
  } while (0);

  UiDpProcessorFactory::release(dpProcessor);
  return dpProcessor;
}

void UiDpProcessorFactory::release(UiDpProcessor*& dpProcessor)
{
  do
  {
    if (dpProcessor == NULL)
    {
      break;
    }
    delete dpProcessor;
    dpProcessor = NULL;
    return;
  } while (0);
  return;
}

#endif // _CONF_UI_DP_PROCESSOR_FACTORY_ENABLED