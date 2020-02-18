# Template for a AVR project

cmake_minimum_required(VERSION 2.8)

set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/cmake_config/generic-avr-gcc.cmake")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}")

project(AUTOMATIC_FARM)
# Name your project
set(AVR_MCU atmega16)
set(MCU_FREQ 8000000UL)

#generate the driver headers
add_subdirectory(libraries/hardware/${AVR_MCU})

#compile the drivers
add_subdirectory(libraries/drivers/spi)
add_subdirectory(libraries/drivers/uart)
add_subdirectory(libraries/drivers/e2p)
add_subdirectory(libraries/drivers/flash)

#compile the libs modules
add_subdirectory(libraries/modules/nrf24)

#compile the firmware
add_subdirectory(firmware)

#compile the programmer sw
add_subdirectory(programmer)

#compile the application
add_subdirectory(application)