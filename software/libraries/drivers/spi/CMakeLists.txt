# Template for a AVR project
cmake_minimum_required(VERSION 2.8)
set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/../../../cmake_config/generic-avr-gcc.cmake")

# Name your project
project(SPI)

set(HEADER_INC
    ./inc
    ./../../hardware/${AVR_MCU}
)
include_directories(${HEADER_INC})

set(SOURCE_FILES
    ./src/spi.c
)
avr_add_library(spi ${SOURCE_FILES})



