/*
 Name:		TimeReader.ino
 Created:	8/7/2021 12:41:05 PM
 Author:	My Laptop
*/

#include "src/TimeReaderTask.h"
TimeReaderTask timeReaderTask;

#define PIN_INTERRUPT_SEND_BUTTON     19
volatile unsigned long last_Send_Fired = 0;
volatile bool sendData = false;

void sendButtonPressISR(void)
{
  bool woken = false;
  do
  {
    if (millis() - last_Send_Fired < 200)
    { // Debounce
      return;
    }
    last_Send_Fired = millis();
    CONSOLE_LOG("%s", "[isr] sed start");
    sendData = true;
  } while (0);

  SYSTEM_YEILD_FROM_ISR(woken);
}

void setupInterruption(void)
{
  pinMode(PIN_INTERRUPT_SEND_BUTTON, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(PIN_INTERRUPT_SEND_BUTTON), sendButtonPressISR, FALLING);
}

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize serial communication at 9600 bits per second:
#if (_CONF_SYSTEM_SERIAL_ENABLED)
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB, on LEONARDO, MICRO, YUN, and other 32u4 based boards.
  }
#endif // _CONF_SYSTEM_SERIAL_ENABLED
  setupInterruption();
  //Wire.begin(-1,-1,0);
  int result = 0;
  do
  {
    CONSOLE_LOG("%s", "[set] start");
    result = timeReaderTask.setConfig();
    if (result != 0)
    {
      CONSOLE_LOG("%s", "[set] conf failed");
      break;
    }
    result = timeReaderTask.prepare();
    if (result != 0)
    {
      CONSOLE_LOG("%s", "[set] prep failed");
      break;
    }
    result = timeReaderTask.start();
    if (result != 0)
    {
      CONSOLE_LOG("%s", "[set] start failed");
      break;
    }
    result = 0;
    CONSOLE_LOG("%s", "[set] commTask completed");

  } while (0);

  if (result != 0)
  {
    CONSOLE_LOG("%s", "[set] failed");
  }
  CONSOLE_LOG("%s", "[set] completed");
}
// the loop function runs over and over again until power down or reset
void loop()
{
  if (sendData != false)
  {
    sendData = false;
    timeReaderTask.getTime(TIME_READER_SERVER_CLIENT_SOCKET_1_ADDRESS);
  }

}
