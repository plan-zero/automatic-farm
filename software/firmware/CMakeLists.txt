# Template for a AVR project
cmake_minimum_required(VERSION 2.8)
set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/../../cmake_config/generic-avr-gcc.cmake")

#include HW definition for flash locations

set(AVR_LFLAGS "-Wl,--relax,--gc-sections -Wl,-section-start=.text=${BOOTLOADER_CODE_ADDR} -Wl,--section-start=${FLASH_SECTION_NRF24}=${NRF_CODE_ADDR} -Wl,--section-start=${FLASH_SECITON_RADIO_FPTR}=${NRF_HOOKS_ADDR}  -Wl,--undefined=ptrs -Wl,--undefined=${E2P_DATA_VAR_NAME}")

# Name your project
project(FIRMWARE)

#extract the NRF24 image
set(NRF24_IMAGE True)

set(HEADER_INC
    ./bootloader/inc
    ./../libraries/drivers/e2p/inc
    ./../libraries/drivers/flash/inc
    ./../libraries/modules/nrf24/inc
    ./../libraries/hardware/${AVR_MCU}
)
include_directories(${HEADER_INC})

#generate the eep file
set(PROGRAM_EEPROM True)

set(SOURCE_FILES
    ./bootloader/src/main.c
    ./bootloader/src/WriteApp.c
)
avr_add_executable(firmware ${SOURCE_FILES})
target_link_libraries(firmware.elf nrf24 e2p flash)

#install binaries
install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/firmware.hex
          ${CMAKE_CURRENT_BINARY_DIR}/firmware_eep.hex
          ${CMAKE_CURRENT_BINARY_DIR}/nrf24.hex
    DESTINATION ${CMAKE_SOURCE_DIR}/assets/${AVR_MCU}/firmware

)
