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
set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/../../../cmake_config/generic-avr-gcc.cmake")

# Name your project
project(E2P)

#set E2P variable that is passed down to the linker
set(E2P_DATA_VAR_NAME e2p_default_data CACHE INTERNAL "")
#check if E2P values are set, otherwise use the default ones

#check BOOT key value
if(BOOT_KEY)
    set(BYTE_IDX 0)
    foreach(BYTE_VALUE ${BOOT_KEY})
        add_definitions(-DE2P_BOOT_KEY_BYTE_${BYTE_IDX}=0x${BYTE_VALUE})
        MATH(EXPR BYTE_IDX "${BYTE_IDX} + 1")
    endforeach()
else()
    add_definitions(-DE2P_BOOT_KEY_BYTE_0=0x30)
    add_definitions(-DE2P_BOOT_KEY_BYTE_1=0x31)
    add_definitions(-DE2P_BOOT_KEY_BYTE_2=0x32)
    add_definitions(-DE2P_BOOT_KEY_BYTE_3=0x33)
    add_definitions(-DE2P_BOOT_KEY_BYTE_4=0x34)
    add_definitions(-DE2P_BOOT_KEY_BYTE_5=0x35)
    add_definitions(-DE2P_BOOT_KEY_BYTE_6=0x36)
    add_definitions(-DE2P_BOOT_KEY_BYTE_7=0x37)
    add_definitions(-DE2P_BOOT_KEY_BYTE_8=0x38)
    add_definitions(-DE2P_BOOT_KEY_BYTE_9=0x39)
endif(BOOT_KEY)

#check RX address value
if(E2P_RX_ADDRESS)
    string(REPLACE "x" ";" E2P_RX_ADDRESS_LIST ${E2P_RX_ADDRESS})
    message(STATUS "string= ${E2P_RX_ADDRESS_LIST}")
    set(BYTE_IDX 0)
    foreach(BYTE_VALUE ${E2P_RX_ADDRESS_LIST})
        add_definitions(-DE2P_RX_ADDRESS_BYTE_${BYTE_IDX}=0x${BYTE_VALUE})
        MATH(EXPR BYTE_IDX "${BYTE_IDX} + 1")
    endforeach()
else()
    add_definitions(-DE2P_RX_ADDRESS_BYTE_0=0x41)
    add_definitions(-DE2P_RX_ADDRESS_BYTE_1=0x42)
    add_definitions(-DE2P_RX_ADDRESS_BYTE_2=0x43)
    add_definitions(-DE2P_RX_ADDRESS_BYTE_3=0x44)
    add_definitions(-DE2P_RX_ADDRESS_BYTE_4=0x45)
endif(E2P_RX_ADDRESS)

#set download flag to true by default
add_definitions(-DE2P_DOWNLOAD_FLAG_VALUE=0xAA)

set(HEADER_INC
    ./inc
    ./../../hardware/${AVR_MCU}
)
include_directories(${HEADER_INC})

set(SOURCE_FILES
    ./src/e2p.c
)
avr_add_library(e2p ${SOURCE_FILES})



