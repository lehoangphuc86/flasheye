#ifndef _SYSTEM_COMMON_ARDUINO_H
#define _SYSTEM_COMMON_ARDUINO_H

#include "../../FlashEyesCoreLibConfig.h"
#ifdef SYSTEM_ARDUINO_BASED
/////////////////////////////////////////////////
// Preprocessor

#ifndef SYSTEM_ARDUINO_BASED
#error The system is not Arduino based!!
#endif // !SYSTEM_ARDUINO_BASED

#if defined(SYSTEM_AVR_PLATFORM) | defined(CONFIG_FREERTOS_UNICORE)
#define SYSTEM_CPU_UNICORE
#endif // defined(SYSTEM_AVR_PLATFORM) | defined(CONFIG_FREERTOS_UNICORE)

/////////////////////////////////////////////////
// INCLUDE
#define Binary_h
#include "Arduino.h"
#include <stdio.h>
#include "string.h"

#if defined(SYSTEM_AVR_PLATFORM)
#include "SystemCommonAVR.h"
#elif defined(SYSTEM_ESP_PLATFORM)
#include "SystemCommonESP.h"
#else // unsupported 
#error Not supported platform
#endif // ARDUINO_ESP32_DEV
/////////////////////////////////////////////////
// PREPROCESSOR

/////////////////////////////////////////////////
// DEFINE

/////////////////////////////////////////////////
// MARCO

// remove unneccessary virtual to save memory
#define __ATTRIBUTE_VIRTUAL_OPTIMIZED                     

// SYSTEM_CONSOLE()
#if (_CONF_SYSTEM_CONSOLE_LOG_ENABLED)
#define SYSTEM_CONSOLE_HANDLER                              (Serial)
#define SYSTEM_CONSOLE(format, ...)                         \
do {                                                        \
  const __FlashStringHelper* fFormat = F(format);           \
  SystemCommon::consoleVA(fFormat, __VA_ARGS__);\
} while(0)                                  

#define SYSTEM_CONSOLE_BUF(buf, bufLen, format, ...)        \
do {                                                        \
  snprintf(buf, bufLen, format, __VA_ARGS__);               \
  SYSTEM_CONSOLE_HANDLER.write(buf);                        \
  SYSTEM_CONSOLE_HANDLER.write("\r\n");                     \
} while(0)                                  

//#define SYSTEM_CONSOLE_ISR(buf, bufLen, format, ...)        SYSTEM_CONSOLE_BUF(buf, bufLen, format, __VA_ARGS__)

#else // _CONF_SYSTEM_CONSOLE_LOG_ENABLED
#define SYSTEM_CONSOLE(format, ...)
#define SYSTEM_CONSOLE_BUF(buf, bufLen, format, ...)
#define SYSTEM_CONSOLE_ISR(format, ...)
#endif // _CONF_SYSTEM_CONSOLE_LOG_ENABLED


#ifdef SYSTEM_HAS_OS_SYSTEM
#define SYSTEM_IS_OS_RUNNING()                            (xTaskGetSchedulerState() == taskSCHEDULER_RUNNING)
#define SYSTEM_SLEEP(x)                                   vTaskDelay(x / portTICK_PERIOD_MS)
#define SYSTEM_TASK_MEM_DATA_TYPE                         UBaseType_t
#define SYSTEM_TASK_MEM_USAGE(handler)                    uxTaskGetStackHighWaterMark(handler)
#else // SYSTEM_HAS_OS_SYSTEM
#define SYSTEM_IS_OS_RUNNING()                            false
#define SYSTEM_SLEEP(x)                                   delay(x)
#define SYSTEM_TASK_MEM_DATA_TYPE                         uint8_t
#define SYSTEM_TASK_MEM_USAGE(taskHandler)                0
#endif // SYSTEM_HAS_OS_SYSTEM

// SYSTEM TIME() 
#define SYSTEM_TIME_MILLIS()                              millis() // current time in micros
#define SYSTEM_TIME_MICROS()                              micros() // current time in micros

// system hardware
#define PIN_NUMBER_MAX                                    NUM_DIGITAL_PINS
#define PIN_MODE(pin, mode)                               pinMode(pin, mode)
#define ATTACH_ISR(pin, isrHandler, triggerType)          attachInterrupt(digitalPinToInterrupt(pin), isrHandler, triggerType)
#define DETTACH_ISR(pin)                                  detachInterrupt(digitalPinToInterrupt(pin))

// ISR trigger type
#define ISR_RISING                                        RISING
#define ISR_FALLING                                       FALLING
#define ISR_CHANGE                                        CHANGE
#define ISR_ONLOW                                         ONLOW
#define ISR_ONHIGH                                        ONHIGH
#define ISR_ONLOW_WE                                      ONLOW_WE
#define ISR_ONHIGH_WE                                     ONHIGH_WE
#define ISR_INVALID                                       0

// GPIO function
#define GPIO_INPUT                                        INPUT
#define GPIO_OUTPUT                                       OUTPUT
#define GPIO_PULLUP                                       PULLUP
#define GPIO_INPUT_PULLUP                                 INPUT_PULLUP

// Pin write
#define SYSTEM_PIN_DIGITAL_WRITE(pin, val)                digitalWrite(pin, val)
#define SYSTEM_PIN_DIGITAL_READ(pin)                      digitalRead(pin)
#define SYSTEM_PIN_ANALOG_WRITE(pin, val)                 ANALOG_WRITE(pin, val)

// Digital state
#define SYSTEM_PIN_HIGH                                   HIGH
#define SYSTEM_PIN_LOW                                    LOW

// bit set
#define SYSTEM_BITREAD(value, bit)                        bitRead(value, bit)
#define SYSTEM_BITSET(value, bit)                         bitSet(value, bit)
#define SYSTEM_BITCLEAR(value, bit)                       bitClear(value, bit)
#define SYSTEM_BITWRITE(value, bit, bitvalue)             bitWrite(value, bit, bitvalue)
/////////////////////////////////////////////////
// GLOBAL VARIABLES

/////////////////////////////////////////////////
// GLOBAL FUNCTIONS

/////////////////////////////////////////////////
// DATA TYPE (TYPEDEF)
typedef HardwareSerial                                    ConsoleLogger;
typedef byte                                              DataCount_t;
typedef uint16_t                                          DataSize_t;
typedef byte                                              EventId_t;
typedef byte                                              EventCount_t;
typedef byte                                              EventSize_t;
typedef byte                                              TaskId_t;
typedef unsigned short                                    TaskSize_t;
typedef DataSize_t                                        RankingScore_t;
typedef byte                                              PoolId_t;
typedef byte                                              ClientId_t;
typedef ClientId_t                                        ClientCount_t;
typedef byte                                              TimerId_t;
typedef uint64_t                                          TimePoint_t;
typedef uint64_t                                          TimerCount_t;
typedef uint16_t                                          TimerReg_t;
typedef uint32_t                                          TimerOVFCount_t;
typedef byte                                              TimerHanlder_t;
typedef uint8_t                                           MessageId_t;
typedef uint16_t                                          HttpReqId_t;
typedef uint8_t                                           MbcMessageId_t;
typedef uint16_t                                          MbcDataSize_t;
typedef uint8_t                                           CommId_t;
typedef uint8_t                                           DbTableId_t;
typedef uint8_t                                           UIMessageId_t;
typedef uint16_t                                          Seq_t;
/////////////////////////////////////////////////
// DATA TYPE (ENUM)

/////////////////////////////////////////////////
// DATA TYPE (STRUCT)

/////////////////////////////////////////////////
// STATIC DATA

/////////////////////////////////////////////////
// STATIC FUNCTIONS
//
class SystemCommon
{
public:
  static void printUint64t(uint64_t num)
  {
    char strBuff[21];
    char* ptr = strBuff + 1;

    while (num > 0)
    {
      *ptr++ = '0' + (num % 10);
      num /= 10;
    }

    ptr--;
    /*Print the number which is now in reverse*/
    while (ptr > strBuff)
    {
      SYSTEM_CONSOLE_HANDLER.print(*ptr--);
    }
  }

  static size_t consoleVA(const __FlashStringHelper * fFormat, ...)
  {
    size_t count = 0;
    do
    {
      char* format = (char*)(fFormat);
      if (format == NULL)
      {
        break;
      }
      //Serial.println(format);
      char inputFormat = pgm_read_byte_near(format);
      va_list vaList;
      va_start(vaList, fFormat);
      while (inputFormat != 0)
      {
        if (inputFormat != '%')
        {
          SYSTEM_CONSOLE_HANDLER.print(inputFormat);
          format++;
          inputFormat = pgm_read_byte_near(format);
          continue;
        }
        //Serial.println(*inputFormat);
        // is % character
        format++;
        inputFormat = pgm_read_byte_near(format); //next char
        count++;
        //Serial.println(*inputFormat);
        switch (inputFormat)
        {
          case 0:
          {
            count--;
            continue; //end
            break;
          }
          case 'c':
          {
            char var = (char)va_arg(vaList, int);
            SYSTEM_CONSOLE_HANDLER.print(var);
            break;
          }
          case 'i':
          case 'd':
          {
            int var = va_arg(vaList, int);
            //Serial.println("i");
            SYSTEM_CONSOLE_HANDLER.print(var);
            break;
          }
          case 'u':
          {
            unsigned int var = va_arg(vaList, unsigned int);
            SYSTEM_CONSOLE_HANDLER.print(var);
            break;
          }
          case 'f':
          {
            double var = va_arg(vaList, double);
            SYSTEM_CONSOLE_HANDLER.print(var);
            break;
          }
          case 'l':
          {
            long var = va_arg(vaList, long);
            SYSTEM_CONSOLE_HANDLER.print(var);
            break;
          }
          case 'm':
          {
            uint64_t var = va_arg(vaList, uint64_t);
            SystemCommon::printUint64t(var);
            break;
          }
          case 's':
          {
            const char* var = va_arg(vaList, const char*);
            SYSTEM_CONSOLE_HANDLER.print(var);
            break;
          }
          default:
          {
            SYSTEM_CONSOLE_HANDLER.print('%');
            SYSTEM_CONSOLE_HANDLER.print(inputFormat);
            count--;
            break;
          }
        }
        format++;
        inputFormat = pgm_read_byte_near(format); //next char
      }
      va_end(vaList);

      SYSTEM_CONSOLE_HANDLER.println();
      return count;
    } while (0);
    return 0;
  }
};

/////////////////////////////////////////////////
// EXTERN

/////////////////////////////////////////////////
// CLASS DEFINITION

#endif // SYSTEM_ARDUINO_BASED
#endif // _SYSTEM_COMMON_ARDUINO_H