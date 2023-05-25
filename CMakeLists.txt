cmake_minimum_required(VERSION 3.20.0)
set(BOARD nrf52dk_nrf52832)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(CE_recruitment)

include_directories(inc)

target_sources(app PRIVATE
		src/main.c
		src/blinkThread.c
		src/pwmThread.c
)
