/////////////////////////////////////////////////
// INCLUDE
#include "DisplayEngine.h"
#if (_CONF_DISPLAY_ENGINE_ENABLED)
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

/* DisplayEngine*/
DisplayEngine::DisplayEngine(void)
{
  
}
DisplayEngine::~DisplayEngine(void)
{

}

bool DisplayEngine::isTaskValid(void)
{
  return this->device_Controller.isTaskValid();
}

int DisplayEngine::setConfig(DisplayDeviceConfig& deviceConfig)
{
  return this->device_Controller.setConfig(deviceConfig);
}

int DisplayEngine::prepare(void)
{
  return this->device_Controller.prepare();
}

int DisplayEngine::start(void)
{
  return this->device_Controller.start();
}

int DisplayEngine::controlDisplay(EventDisplayControlTAG* data)
{
  do
  {
    if (data == NULL)
    {
      break;
    }
    switch (data->controlId)
    {
      case (int)DisplayControlId::DisplayControlBrightness:
      {
        CONSOLE_LOG("bri=%i", data->brighnessLevel);
        this->device_Controller.setBrightness(data->brighnessLevel);
        break;
      }
      case (int)DisplayControlId::DisplayControlBacklight:
      {
        CONSOLE_LOG("back=%i", data->backLightLevel);
        this->device_Controller.setBacklight(data->backLightLevel);
        break;
      }
      default:
        break;
    }

    return 0;
  } while (0);
  return -1;
}

void DisplayEngine::displayStatus(EventDisplayStatusTAG* data)
{
  do
  {
    if (data == NULL)
    {
      break;
    }
    this->device_Controller.setBrightness(data->count);

    this->device_Controller.setCursor(0, 1);
    this->device_Controller.print(data->time);
    return;
  } while (0);
  
  return;
}

void DisplayEngine::displayDistance(EventDisplayDistanceTAG* data)
{
  do
  {
    if (data == NULL)
    {
      break;
    }
    CONSOLE_LOG("%s", "[disen] dis");
    //this->device_Controller.clear();
    this->device_Controller.setCursor(0, 0);
    this->device_Controller.print(data->sensorId);
    this->device_Controller.print(" ");
    this->device_Controller.print(data->distance);
    //char buff[100];
    //sprintf(buff, "%i %i", data->sensorId, data->distance);
    //this->device_Controller.print(data->distance);
    //this->device_Controller.print(buff);

    this->device_Controller.setCursor(0, 1);
    //sprintf(buff, "%i %i", data->delay, data->measuredTime);
    this->device_Controller.print(data->measuredTime);
    this->device_Controller.print(" ");
    this->device_Controller.print(data->delay);
    //this->device_Controller.autoscroll();
    return;
  } while (0);

  return;
}

void DisplayEngine::displayTime(EventDisplayTimeTAG* data)
{
  do
  {
    if (data == NULL)
    {
      break;
    }
    CONSOLE_LOG("bak=%i", data->count);
    this->device_Controller.setBacklight(data->count);

    this->device_Controller.setCursor(0, 1);
    this->device_Controller.print(data->time);
    return;
  } while (0);

  return;
}

#endif // _CONF_DISPLAY_ENGINE_ENABLED