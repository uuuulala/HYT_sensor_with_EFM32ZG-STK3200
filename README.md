# HYT_sensor_with_EFM32ZG-STK3200

HYT-939 digital humidity & temperature sensor reading with EFM32ZG-STK3200 board.
Can be used for HYT-271 and HYT-221 sensors without any changes. 

![3 types of HYT sensor](http://i.imgur.com/esmIGBk.png)

Every time the button #1 is pressed we receive temperature and humidity data from the sensor and update information at the display.

The code isn't optimized for low energy consuption as any EFM32 programm could be.

![gif](https://media.giphy.com/media/RDYxxGEi6f4SQ/giphy.gif)

![wellcome screen](http://i.imgur.com/VGCd4PY.jpg) ![normal screen](http://i.imgur.com/CHwuMoh.jpg) ![error screen](http://i.imgur.com/CBgmkUM.jpg)

## Connecting the sensor

HYT sensors use SDA, SCL, VDD and GND pins to be connected. We use EFM32 integrated pull-up resisrors so no additional components are needed.
All information about protocol available in [Application Note](https://www.ist-ag.com/sites/default/files/AHHYTM_E.pdf)

## EFM32ZG-STK3200 board

EFM32ZG-STK3200 is a starter Kit designed by Silicon Labs company to get familiar with the EFM32 Zero Gecko 32-bit microcontrollers. 
You can find all information about the kit at [silabs.com](http://www.silabs.com/products/development-tools/mcu/32-bit/efm32-zero-gecko-starter-kit).

Here we use the following board components: 
* expansion header (I2C pins, Vdd and GND) to connect HYT sensor
* user button #1
* 128 x 128 pixel Memory-LCD
* Integrated SEGGER J-Link USB debugger/emulator
* EFM32ZG222F32 Cortex-M0+ MCU

## LCD

Nice LCD is embedded in EFM32ZG-STK3200 starter kit. The best way to use it is GLIB graphic library provided by Silicon Labs.

## IDE

The project was created with Simplicity Studio IDE v4.
