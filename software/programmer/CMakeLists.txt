# Template for a AVR project
cmake_minimum_required(VERSION 2.8)
set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/../../cmake_config/generic-avr-gcc.cmake")

#include HW definition for flash locations

#set(AVR_LFLAGS "-Wl,--relax,--gc-sections -Wl,-section-start=.text=${BOOTLOADER_CODE_ADDR} -Wl,--section-start=.nrf24=${NRF_CODE_ADDR} -Wl,--section-start=.radio_fptrs=${NRF_HOOKS_ADDR}  -Wl,--undefined=ptrs")

# Name your project
project(PROGRAMMER)

include_directories(./../libraries/modules/nrf24/inc)
include_directories(./../libraries/drivers/uart/inc)

avr_add_executable(programmer ./src/main.c)
target_link_libraries(programmer.elf nrf24_api uart)

#install binaries
install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/programmer.hex
    DESTINATION ${CMAKE_SOURCE_DIR}/assets/programmer
)
