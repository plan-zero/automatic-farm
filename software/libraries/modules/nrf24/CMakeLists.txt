# Template for a AVR project
cmake_minimum_required(VERSION 2.8)
set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/../../../cmake_config/generic-avr-gcc.cmake")

# Name your project
project(NRF24)

set(HEADER_INC
    ./inc
    ./../../drivers/spi/inc
    ./../../hardware/${AVR_MCU}
)
include_directories(${HEADER_INC})

set(SOURCE_FILES
    ./src/nrf24Radio.c
    ./src/nrf24Radio_shared.c
)
avr_add_library(nrf24 ${SOURCE_FILES})
target_link_libraries(nrf24 spi)

set(SOURCE_SHARED_FILES
    ./src/nrf24Radio_shared.c
)
avr_add_library(nrf24_api ${SOURCE_SHARED_FILES})



