# Template for a AVR project
cmake_minimum_required(VERSION 2.8)
set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/../../../cmake_config/generic-avr-gcc.cmake")

# Name your project
project(FLASH)

include_directories(inc)
include_directories(./../../hardware/${AVR_MCU})
avr_add_library(flash src/flash.c)



