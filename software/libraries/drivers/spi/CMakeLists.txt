# Template for a AVR project

cmake_minimum_required(VERSION 2.8)

set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/../../../cmake_config/generic-avr-gcc.cmake")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}")

# Name your project
project(SPI)

#device dependent config, here for arduino uno on linux
set(AVR_MCU atmega16)

include_directories(./inc)
avr_add_library(spi ./src/spi.c)
install(TARGETS spi DESTINATION lib)
install(FILES "${PROJECT_BINARY_DIR}/../inc/spi.h" DESTINATION includes)


