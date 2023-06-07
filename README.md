# Optical rangefinder device with OLED display
___
## Basic Informations
- Device is running on [nRF52-DK_nRF52832 board](https://www.nordicsemi.com/Products/Development-hardware/nrf52-dk),
- Firmware works under [Zephyr RTOS](https://zephyrproject.org/),
- device contains 0,96" OLED I2C display (128x64 px) based on [SSD1306 controller](https://cdn-shop.adafruit.com/datasheets/SSD1306.pdf),
- Driver for SSD1306 is included within Zephyr RTOS source files,
- rangemeter used in device is [VL6180X I2C ToF sensor](https://www.st.com/resource/en/datasheet/vl6180x.pdf), API for this SoC is available [here](https://www.st.com/en/embedded-software/stsw-img003.html#get-software).
- LED0 change brightness based on VL6180X measurement. The closer is object, the brighter LED glow.

Purpose of this project is to get into zephyr RTOS.
Author: Krzysztof W.

## Hardware
OLED display and VL6180X sensor share same I2C bus. Connections:
| nRF52-DK | OLED and VL6180X |
| :---: | :---: |
| 5V | VCC |
| GND | GND |
| P0.27 | SCL |
| P0.26 | SDA |
Both SDA and SCL are pulled up with 4k7 resistors on nRF52-DK board.

## Development environment
Project created abd developed on Ubuntu desktop 22.04 LTS.
Information how to get and fully configure zephyr can be found [here](https://docs.zephyrproject.org/latest/develop/getting_started/index.html).

Versions of most important development software used:
- West version: v1.0.0
- cmake version 3.26.4
- Python 3.10.6
- Package: device-tree-compiler Version: 1.6.1-1
- Package: jlink Version: 7.803
- Package: nrf-command-line-tools Version: 10.21.0 (nfrjprog)
- minicom version 2.8

Because this project is based on basic setup of nRF52-DK board, it includes setup for UART over interface MCU which handles programming of nRF52832 MCU. This UART has default setup 8N1 and 115200 Baud rate and it's linked to the _/dev/ttyACM0_.
To listen UART connection open terminal window and run following command:
```sh
sudo minicom -D /dev/ttyACM0 -b 115200
```
Because _/dev/ttyACM0_ is not available for regular user (need to be run as root) you can add perrmisions to use serial ports for current user by following command:
```sh
sudo usermod -a -G dialout $(whoami)
```
After that there is no need of using _sudo_ before launching minicom.

## Building and Flashing
This projects builds was tested only under nRF52-DK board and building process includes only this board (build parameter set in _CMakeLists.txt_ file).
For building project is using zephyr RTOS environment for _west_ command. To invoke zephyr environment run this command:
```sh
source <path_to_zephyrproject>/zephyr/zephyr-env.sh
```
To build project run following commands in terminal window with sourced *zephyr-env.sh*:
```sh
cd <proj-dir>
west build -p always
```
To flash development board:
```sh
cd <proj-dir>
west flash
```
To make sure that development board flash is fully erased, run _west flash_ command with _--erase_ param.
If there is a need to reset MCU, run following command:
```sh
nrfjprog --reset
```

## Useful links
[Zephyr OS docs: Getting started guide](https://docs.zephyrproject.org/latest/develop/getting_started/index.html)
[Zephyr OS docs: nRF52 DK Overwiev](https://docs.zephyrproject.org/latest/boards/arm/nrf52dk_nrf52832/doc/index.html)
[Zephyr OS dosc: Application development](https://docs.zephyrproject.org/latest/develop/application/index.html)
[Zephyr OS docs: HW support - Peripherials](https://docs.zephyrproject.org/latest/hardware/peripherals/index.html)
[Zephyr OS docs: Build and Configuration Systems](https://docs.zephyrproject.org/latest/build/index.html)
[Zephyr OS docs: Kernel Services](https://docs.zephyrproject.org/latest/kernel/services/index.html)
[Zephyr OS docs: SSD1306 I2C driver support](https://docs.zephyrproject.org/latest/build/dts/api/bindings/display/solomon%2Cssd1306fb-i2c.html#dtbinding-solomon-ssd1306fb-i2c)
[VL6180x Datasheet](https://www.st.com/resource/en/datasheet/vl6180x.pdf)
[VL6180x Application Note](https://www.st.com/resource/en/application_note/an4545-vl6180x-basic-ranging-application-note-stmicroelectronics.pdf)
[VL6180x API](https://www.st.com/en/embedded-software/stsw-img003)
[SSD1306 Datasheet](https://cdn-shop.adafruit.com/datasheets/SSD1306.pdf)
