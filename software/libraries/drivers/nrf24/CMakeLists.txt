# Template for a AVR project

cmake_minimum_required(VERSION 2.8)

set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/../../../cmake_config/generic-avr-gcc.cmake")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}")

# Name your project
project(NRF24)

#device dependent config, here for arduino uno on linux
set(AVR_MCU atmega16)


include_directories(./inc)
include_directories(./../spi/inc)
avr_add_library(nrf24 ./src/nrf24Radio.c )


