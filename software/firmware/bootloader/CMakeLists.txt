# Template for a AVR project

cmake_minimum_required(VERSION 2.8)

set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/../../cmake_config/generic-avr-gcc.cmake")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}")

set(AVR_LFLAGS "-Wl,--relax,--gc-sections -Wl,-section-start=.text=0x3800 -Wl,--section-start=.nrf24=0x2840 -Wl,--section-start=.radio_fptrs=0x2800  -Wl,--undefined=ptrs")

# Name your project
project(FIRMWARE)

#device dependent config, here for arduino uno on linux
set(AVR_MCU atmega16)

#find_package(RADIO REQUIRED COMPONENTS libradio)
include_directories(./../../libraries/drivers/nrf24/inc)
include_directories(./inc)


avr_add_executable(firmware ./src/main.c ./src/WriteApp.c ./../../libraries/drivers/nrf24/src/nrf24Radio_shared.c)
target_link_libraries(firmware.elf nrf24 spi)

