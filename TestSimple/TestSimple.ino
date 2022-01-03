/*
 Name:		TestSimple.ino
 Created:	9/4/2021 9:30:33 AM
 Author:	My Laptop
*/
#include "Arduino.h"
#if CONFIG_FREERTOS_UNICORE
#define ARDUINO_RUNNING_CORE 0
#else
#define ARDUINO_RUNNING_CORE 1
#endif
// CLASS DEFINITION
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB, on LEONARDO, MICRO, YUN, and other 32u4 based boards.
  }

  delay(500);
  Serial.println("S1 started");
  

  Serial2.begin(9600);
  while (!Serial2) {
    ; // wait for serial port to connect. Needed for native USB, on LEONARDO, MICRO, YUN, and other 32u4 based boards.
  }

  delay(500);
  Serial.println("S2 started");
  
}
// the loop function runs over and over again until power down or reset
void loop()
{
  delay(1000);
  Serial2.println("hello");
}