# Template for a AVR project

cmake_minimum_required(VERSION 2.8)

set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/generic-avr-gcc.cmake")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}")

set(AVR_LFLAGS "-Wl,--relax,--gc-sections -Wl,-section-start=.text=0x3800 -Wl,--section-start=.nrf24=0x2840 -Wl,--section-start=.radio_fptrs=0x2800  -Wl,--undefined=ptrs" CACHE STRING "AVR link flags")

# Name your project
project(BOOTLOADER)

#device dependent config, here for arduino uno on linux
set(AVR_MCU atmega16)
set(MCU_FREQ 8000000UL)
set(AVR_PROGRAMMER arduino)
set(AVR_PROGRAMMER_PORT /dev/ttyACM0)
#set(AVR_PROGRAMMER_BAUDRATE 19200)
set(PROGRAM_EEPROM False)

#find_package(RADIO REQUIRED COMPONENTS libradio)
include_directories(./bootloader)
include_directories(./radio/include)

avr_add_executable(bootloader ./bootloader/main.c ./bootloader/WriteApp.c ./radio/src/nrf24Radio_shared.c)
target_link_libraries(bootloader.elf ${CMAKE_SOURCE_DIR}/radio/build/libradio.a)

