# Template for a AVR project

cmake_minimum_required(VERSION 2.8)

set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/cmake_config/generic-avr-gcc.cmake")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}")

# Name your project
project(FIRMWARE)
set(AVR_MCU atmega16)

add_subdirectory(libraries/drivers/spi)
add_subdirectory(libraries/drivers/nrf24)
add_subdirectory(firmware/bootloader)