cmake_minimum_required(VERSION 3.20.0)
set(BOARD nrf52dk_nrf52832)

add_compile_options(-Wall)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(CE_recruitment)

include_directories(inc)
include_directories(vl6180_API/inc)
include_directories(vl6180_API/cci-i2c)

target_sources(app PRIVATE
		src/main.c
#		src/blinkThread.c
		src/displayThread.c
		src/oledDisplay.c
		src/fadingLed.c
		src/measThread.c
		vl6180_API/src/vl6180x_api.c
		vl6180_API/cci-i2c/vl6180x_i2c.c
		src/rangeMeter.c
)
