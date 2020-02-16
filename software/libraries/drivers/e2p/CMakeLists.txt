# Template for a AVR project
cmake_minimum_required(VERSION 2.8)
set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/../../../cmake_config/generic-avr-gcc.cmake")

# Name your project
project(E2P)

#set E2P variable that is passed down to the linker
set(E2P_DATA_VAR_NAME e2p_layout CACHE INTERNAL "")
#check if E2P values are set, otherwise use the default ones

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

include_directories(inc)
include_directories(./../../hardware/${AVR_MCU})
avr_add_library(e2p src/e2p.c)



