# Template for a AVR project
cmake_minimum_required(VERSION 2.8)
set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/../../cmake_config/generic-avr-gcc.cmake")

set(AVR_LFLAGS "-Wl,--relax,--gc-sections -Wl,-section-start=.text=0x3800 -Wl,--section-start=.nrf24=0x2840 -Wl,--section-start=.radio_fptrs=0x2800  -Wl,--undefined=ptrs")

# Name your project
project(FIRMWARE)

include_directories(./../libraries/modules/nrf24/inc)
include_directories(./bootloader/inc)

avr_add_executable(firmware ./bootloader/src/main.c ./bootloader/src/WriteApp.c)
target_link_libraries(firmware.elf nrf24)

#install binaries
install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/firmware.hex
    DESTINATION ${CMAKE_SOURCE_DIR}/assets/firmware
)
