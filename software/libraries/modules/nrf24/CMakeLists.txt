# Template for a AVR project
cmake_minimum_required(VERSION 2.8)
set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/../../../cmake_config/generic-avr-gcc.cmake")

# Name your project
project(NRF24)

include_directories(inc)
include_directories(./../../drivers/spi/inc)
include_directories(./../../hardware/${AVR_MCU})

avr_add_library(nrf24 ./src/nrf24Radio.c ./src/nrf24Radio_shared.c)
avr_add_library(nrf24_api ./src/nrf24Radio_shared.c)
target_link_libraries(nrf24 spi)


