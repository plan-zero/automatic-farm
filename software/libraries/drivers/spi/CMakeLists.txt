# Template for a AVR project

cmake_minimum_required(VERSION 2.8)

set(AVR_CFLAGS "-ffunction-sections -fdata-sections -x c -funsigned-char -funsigned-bitfields -DDEBUG  -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums" CACHE STRING "AVR compilation flags")
set(AVR_LFLAGS "-Wl,--relax,--gc-sections" CACHE STRING "AVR link flags")

set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "")   # remove -rdynamic for C

set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/../../../cmake_config/generic-avr-gcc.cmake")

# Name your project
project(SPI)

set(AVR_MCU atmega16)

include_directories(inc)
avr_add_library(spi src/spi.c)

#install(TARGETS spi DESTINATION lib)
#install(FILES "${PROJECT_BINARY_DIR}/../inc/spi.h" DESTINATION includes)


