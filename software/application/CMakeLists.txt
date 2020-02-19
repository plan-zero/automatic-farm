# Template for a AVR project

cmake_minimum_required(VERSION 2.8)

set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/../cmake_config/generic-avr-gcc.cmake")

# Name your project
project(APP)

set(HEADER_INC
    ./Scheduler
    ./Abstractization/Sensors/Humidity
    ./Abstractization/Sensors/Light
    ./Abstractization/Actuators/Relay
    ./Abstractization/Actuators/Valve
)
include_directories(${HEADER_INC})


set(SOURCE_FILES
    ./App/Main.c
    ./Scheduler/Scheduler.c
    ./Abstractization/Sensors/Humidity/Humidity.c
)
avr_add_executable(app ${SOURCE_FILES})

target_link_libraries(app.elf nrf24_api uart)

#install binaries
install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/app.hex
    DESTINATION ${CMAKE_SOURCE_DIR}/assets/${AVR_MCU}/app
)

