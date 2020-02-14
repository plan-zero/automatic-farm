# Template for a AVR project

cmake_minimum_required(VERSION 2.8)

set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/cmake_config/generic-avr-gcc.cmake")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}")

project(AUTOMATIC_FARM)
# Name your project
set(AVR_MCU atmega16)

add_subdirectory(libraries/hardware/${AVR_MCU})
add_subdirectory(libraries/drivers/spi)
add_subdirectory(libraries/modules/nrf24)
add_subdirectory(firmware)