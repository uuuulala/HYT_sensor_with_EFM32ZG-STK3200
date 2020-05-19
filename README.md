# HYT_sensor_with_EFM32ZG-STK3200

HYT-939 digital humidity & temperature sensor & EFM32ZG-STK3200 devKit (Cortex-M0+ MCU based board).
HYT-939 sensor can be replaced with HYT-271 or HYT-221 sensor without code changes. 

![3 types of HYT sensor](http://i.imgur.com/esmIGBk.png)

Button #1 triggers the following actions: 
1) receive temperature and humidity data
4) display the values on the LCD display

The code isn't optimized for low energy consuption as any EFM32 program supposed to be. We just demonstrate a simple case of HYT sensor usage.

![gif](https://media.giphy.com/media/RDYxxGEi6f4SQ/giphy.gif)

![wellcome screen](http://i.imgur.com/VGCd4PY.jpg) ![normal screen](http://i.imgur.com/CHwuMoh.jpg) ![error screen](http://i.imgur.com/CBgmkUM.jpg)

## Connecting the sensor

HYT sensor has 4 pins (SDA, SCL, VDD and GND) to be connected. We use EFM32 integrated pull-up resistors so no additional components are needed.
All information about protocol is available in [Application Note](https://www.ist-ag.com/sites/default/files/AHHYTM_E.pdf)

## EFM32ZG-STK3200 board

EFM32ZG-STK3200 is a development kit designed by Silicon Labs company. It provides all the components you may need to get familiar with EFM32 Zero Gecko 32-bit microcontrollers family. 
Learn more about the kit: [silabs.com](http://www.silabs.com/products/development-tools/mcu/32-bit/efm32-zero-gecko-starter-kit).

The following components are used in this project: 
* 20 pin expansion header (2 x I2C pins, Vdd and GND) to connect HYT sensor
* user button #1
* 128 x 128 pixel Memory-LCD
* Integrated SEGGER J-Link USB debugger/emulator
* EFM32ZG222F32 Cortex-M0+ MCU

## LCD

Really nice LCD is available at EFM32ZG-STK3200 board, the graphic library (GLIB) is provided by vendor.

## IDE

The project was created with Simplicity Studio IDE v4, but you can use any IDE which supports EFM32 family
