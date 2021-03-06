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

# Name your project
project(PROGRAMMER)

set(TARGET_NAME programmer)
set(NRF24HEX nrf24.hex)
set(TARGET_EXT_NAME _ext.hex)

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
    ./../libraries/drivers/adc/inc
    ./../libraries/modules/nrf24/inc
    ./../libraries/modules/system-timer/inc
    ./../libraries/drivers/uart/inc
    ./../libraries/drivers/wdg/inc
    ./../libraries/modules/ds18b20/inc
    ./../libraries/modules/LowPower/inc
    ./../libraries/hardware/${AVR_MCU}
)
include_directories(${HEADER_INC})

set (SOURCE_FILES
    ./src/main.c
)
avr_add_executable(${TARGET_NAME} ${SOURCE_FILES})

#link target with libraries
target_link_libraries(${TARGET_NAME}.elf nrf24_api uart system-timer wdg ds18b20 adc lowpower)

#set postbuild command to concatenate programmer with nrf24
add_custom_command(TARGET programmer POST_BUILD
    COMMAND test ! -e ${CMAKE_CURRENT_BINARY_DIR}/../firmware/${NRF24HEX} || 
    head -n-1 ${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}.hex > ${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}${TARGET_EXT_NAME}
    COMMAND test ! -e ${CMAKE_CURRENT_BINARY_DIR}/../firmware/${NRF24HEX} || 
    cat ${CMAKE_CURRENT_BINARY_DIR}/../firmware/${NRF24HEX} >> ${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}${TARGET_EXT_NAME}
)


#set command to clean the programmer + nrf24 output
get_directory_property(clean_files ADDITIONAL_MAKE_CLEAN_FILES)
set_property(DIRECTORY APPEND
    PROPERTY ADDITIONAL_MAKE_CLEAN_FILES "${TARGET_NAME}${TARGET_EXT_NAME}" "${TARGET_NAME}.map"
)

#install binaries
install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}.hex
          ${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}${TARGET_EXT_NAME}
    DESTINATION ${CMAKE_SOURCE_DIR}/assets/${AVR_MCU}/${TARGET_NAME}

)
