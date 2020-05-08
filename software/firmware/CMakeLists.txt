# Copyright (C) 2020 Coding Night Romania
# 
# This file is part of automatic-farm.
# 
# automatic-farm is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# automatic-farm is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with automatic-farm.  If not, see <http://www.gnu.org/licenses/>.

cmake_minimum_required(VERSION 2.8)
set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/../../cmake_config/generic-avr-gcc.cmake")

#include HW definition for flash locations

set(AVR_LFLAGS "-Wl,--relax,--gc-sections -Wl,-section-start=.text=${BOOTLOADER_CODE_ADDR} -Wl,--section-start=${FLASH_SECTION_NRF24}=${NRF_CODE_ADDR} -Wl,--section-start=${FLASH_SECITON_RADIO_FPTR}=${NRF_HOOKS_ADDR}  -Wl,--undefined=ptrs -Wl,--undefined=${E2P_DATA_VAR_NAME}")

# Name your project
project(FIRMWARE)

#extract the NRF24 image
set(NRF24_IMAGE True)

if(RADIO_RATE)
    if(${RADIO_RATE} MATCHES "250KBS")
        add_definitions(-DRADIO_RATE=2)
    elseif(${RADIO_RATE} MATCHES "1MBS")
        add_definitions(-DRADIO_RATE=1)
    elseif(${RADIO_RATE} MATCHES "2MBS")
        add_definitions(-DRADIO_RATE=0)
    endif()
endif()

if(RADIO_RT)
    if(${RADIO_RT} MATCHES "250US")
        add_definitions(-DRADIO_RT=0)
    elseif(${RADIO_RT} MATCHES "500US")
        add_definitions(-DRADIO_RT=1)
    elseif(${RADIO_RT} MATCHES "750US")
        add_definitions(-DRADIO_RT=2)
    elseif(${RADIO_RT} MATCHES "1000US")
        add_definitions(-DRADIO_RT=3)
    elseif(${RADIO_RT} MATCHES "1250US")
        add_definitions(-DRADIO_RT=4)
    elseif(${RADIO_RT} MATCHES "1500US")
        add_definitions(-DRADIO_RT=5)
    elseif(${RADIO_RT} MATCHES "1750S")
        add_definitions(-DRADIO_RT=6)
    elseif(${RADIO_RT} MATCHES "2000US")
        add_definitions(-DRADIO_RT=7)
    elseif(${RADIO_RT} MATCHES "2250US")
        add_definitions(-DRADIO_RT=8)
    elseif(${RADIO_RT} MATCHES "2500US")
        add_definitions(-DRADIO_RT=9)
    elseif(${RADIO_RT} MATCHES "2750US")
        add_definitions(-DRADIO_RT=10)
    elseif(${RADIO_RT} MATCHES "3000US")
        add_definitions(-DRADIO_RT=11)
    elseif(${RADIO_RT} MATCHES "3250US")
        add_definitions(-DRADIO_RT=12)
    elseif(${RADIO_RT} MATCHES "3500US")
        add_definitions(-DRADIO_RT=13)
    elseif(${RADIO_RT} MATCHES "3750US")
        add_definitions(-DRADIO_RT=14)
    elseif(${RADIO_RT} MATCHES "4000US")
        add_definitions(-DRADIO_RT=15)
    endif()
endif()

set(HEADER_INC
    ./bootloader/inc
    ./../libraries/drivers/e2p/inc
    ./../libraries/drivers/flash/inc
    ./../libraries/drivers/wdg/inc
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
target_link_libraries(firmware.elf nrf24 e2p flash wdg)

#install binaries
install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/firmware.hex
          ${CMAKE_CURRENT_BINARY_DIR}/firmware_eep.hex
          ${CMAKE_CURRENT_BINARY_DIR}/nrf24.hex
    DESTINATION ${CMAKE_SOURCE_DIR}/assets/${AVR_MCU}/firmware

)
