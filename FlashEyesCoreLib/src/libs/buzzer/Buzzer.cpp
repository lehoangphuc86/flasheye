#include "Buzzer.h"
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

#ifdef SYSTEM_AVR_PLATFORM
void startTone(uint8_t pin, unsigned int frequency, unsigned long duration, uint8_t channel)
{
  tone(pin, frequency, duration);
}

void stopTone(uint8_t pin, uint8_t channel)
{
  noTone(pin);
}
#elif  defined (SYSTEM_ESP_PLATFORM)
void startTone(uint8_t pin, unsigned int frequency, unsigned long duration, uint8_t channel)
{
  do
  {
    if (ledcRead(channel))
    {
      return; // channel is already in use
    }

    //ledcAttachPin(pin, channel);
    ledcWriteTone(channel, frequency);
    if (duration > 0)
    {
      SYSTEM_SLEEP(duration);
      stopTone(pin, channel);
    }

    return;
  } while (0);
  return;

}

void stopTone(uint8_t pin, uint8_t channel)
{
  //ledcDetachPin(pin);
  ledcWrite(channel, 0);
}
#else // defined (SYSTEM_ESP_PLATFORM)
void startTone(uint8_t pin, unsigned int frequency, unsigned long duration, uint8_t channel)
{

}

void stopTone(uint8_t pin, uint8_t channel)
{

}
#endif // SYSTEM_AVR_PLATFORM
