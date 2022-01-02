/////////////////////////////////////////////////
// INCLUDE
#include "UiDpProcessor.h"
#if (_CONF_UI_DP_PROCESSOR_ENABLED)
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

/* UiDpProcessor*/
UiDpProcessor::UiDpProcessor(byte procType)
  : proc_Type(procType)
  , dp_Controller(NULL)
{

}

UiDpProcessor::~UiDpProcessor(void)
{
  this->stop();
}

bool UiDpProcessor::isValid(void)
{
  return (this->dp_Controller == NULL? false : true);
}

byte UiDpProcessor::procType(void)
{
  return this->proc_Type;
}

int UiDpProcessor::start(UiDpProcessorConfigTAG& deviceConfig)
{
  int ret = 0;
  do
  {
    
    if (this->isValid() != false)
    {
      return 0; // already
    }

    this->stop();

    this->dp_Controller = DisplayControllerFactory::generate(deviceConfig.deviceConfig.deviceType);
    if (this->dp_Controller == NULL)
    {
      break;
    }

    ret = this->dp_Controller->start(deviceConfig.deviceConfig);
    if (ret != 0)
    {
      break;
    }

    return ret;
  } while (0);

  this->stop();
  return -1;
}

void UiDpProcessor::stop(void)
{
  DisplayControllerFactory::release(this->dp_Controller);
}

#endif // _CONF_UI_DP_PROCESSOR_ENABLED