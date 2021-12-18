/*
 Name:		FlashEyesExtender.ino
 Created:	5/23/2021 12:05:00 PM
 Author:	My Laptop
*/

#include <Wire.h>
#include "src/task_main/MainControllerTask.h"
#include "Arduino.h"
#include "driver/uart.h"
//#include "driver/gpio.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
//#include <esp_log.h>
//#include <driver/rmt.h>
#include "sdkconfig.h"


//gpio_num_t::GPIO_NUM_17
#include <driver/rmt.h>
rmt_config_t config;
#if CONFIG_FREERTOS_UNICORE
#define ARDUINO_RUNNING_CORE 0
#else
#define ARDUINO_RUNNING_CORE 1
#endif

#ifndef LED_BUILTIN
#define LED_BUILTIN 13
#endif

// define two tasks for Blink & AnalogRead
void TaskBlink(void* pvParameters);



MainControllerTask mainController;

// the setup function runs once when you press reset or power the board
void setup() {
  //analogWrite()
  // initialize serial communication at 115200 bits per second:
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB, on LEONARDO, MICRO, YUN, and other 32u4 based boards.
  }
  Serial.println();
  Serial.println("Configuring access point...");
  int result = 0;
  do
  {
    CONSOLE_LOG("%s", "[setup] start");
    // Set up tasks
    result = mainController.setConfig();
    if (result != 0)
    {
      CONSOLE_LOG("%s", "[setup] mainController config failed");
      break;
    }
    CONSOLE_LOG("%s", "[setup] mainController config ok");
    result = mainController.prepare();
    if (result != 0)
    {
      break;
    }
    result = mainController.start();
    if (result != 0)
    {
      break;
    }
    result = 0;
    CONSOLE_LOG("%s", "[setup] mainController completed");
  } while (0);

  if (result != 0)
  {
    CONSOLE_LOG("%s", "[setup] failed");
  }

  //xTaskCreate(
  //  TaskBlink
  //  , "TaskBlink"   // A name just for humans
  //  , 1024  // This stack size can be checked & adjusted by reading the Stack Highwater
  //  , NULL
  //  , 2  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
  //  , NULL
  //);


  CONSOLE_LOG("%s", "[setup] completed");
}

void loop()
{
  //delay(1000);
  //CONSOLE_LOG("%s", "loop running");
  // Empty. Things are done in Tasks.
}

/*--------------------------------------------------*/
/*---------------------- Tasks ---------------------*/
/*--------------------------------------------------*/

void TaskBlink(void* pvParameters)  // This is a task.
{
  (void)pvParameters;

  /*
    Blink
    Turns on an LED on for one second, then off for one second, repeatedly.

    If you want to know what pin the on-board LED is connected to on your ESP32 model, check
    the Technical Specs of your board.
  */

  // initialize digital LED_BUILTIN on pin 13 as an output.
  pinMode(LED_BUILTIN, OUTPUT);

  for (;;) // A Task shall never return or exit.
  {
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    vTaskDelay(100);  // one tick delay (15ms) in between reads for stability
    digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
    vTaskDelay(100);  // one tick delay (15ms) in between reads for stability
    CONSOLE_LOG("%s", "blinking");
  }
}
