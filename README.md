# Flasheye
## _A high performance bar code scanning system_

[![Build Status](https://travis-ci.org/joemccann/dillinger.svg?branch=master)](https://travis-ci.org/joemccann/dillinger)

- A high performance bar code scanning system that could be integrated with other systems via various interfaces including Wifi, Bluetooth, Serial by diverse communication protocols such as TCP/IP, HTTP/HTTPS, RS232/RS485, I2C.
- Design, implement, assemble and test modules in cross platforms including Windows, Linux, FreeRTOS (ESP32 Dev/ AVR Mega2560/ AVR Uno). 
- Related knowledge: RTOS, event based multi-thread, cross platform, pattern design, mutex,  timer, data queue, codec, Wifi, BLE, UART, I2C, TCP/IP, HTTP, MBC, sqlite3, sensors…
## Features

- Trigger scanning by button
- Auto scanning by n millisecond (set n via setting page)
- Congfigure via http web page (http://{IP}/setting.html)
- Output is sent via:
    LCD
    HTTP/HTTPS (http://{IP}/operating.html)
    TCP/IP (json is sent to specified client url)

## Hardware:
-   Scanner: Symbol SE1200
-   Board: ESP32 dev
-   HCSR04 sensor (distance)
-   Active Buzzer (sound)
-   JMD LCD (display)
-   Buttons

## Source structures:
| Plugin | README |
| ------ | ------ |
| FlashEyesCoreLib | Shared core lib source code |
| FlashEyesMain | Flash eye main source code |
| FlashEyesCoreTestArduino | Test core lib in Arduino (Esp32, Uno, Mega2560) |
| FlashEyesCoreTestLinux | Test core lib in Linux OS |
| FlashEyesCoreTestWindows | Test core lib in Windows OS |
| TestScanner | Test scanner device |
| TestSimple | Test simple cases |
| TimeReader | Test scanner reading feature |
| FlashEyesExtender | Placeholder |


## IDE
- Visual studio community 2019 + Visual micro extension

## Demo
https://youtu.be/UlQpwY2KyLM

## License
No license
