# Template for a AVR project

cmake_minimum_required(VERSION 2.8)

set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/../cmake_config/generic-avr-gcc.cmake")

# Name your project
project(APP)


include_directories(./Scheduler)
include_directories(./Abstractization/Sensors/Humidity)
include_directories(./Abstractization/Sensors/Light)
include_directories(./Abstractization/Actuators/Relay)
include_directories(./Abstractization/Actuators/Valve)



avr_add_executable(app ./App/Main.c ./Scheduler/Scheduler.c ./Abstractization/Sensors/Humidity/Humidity.c ./)

target_link_libraries(app.elf nrf24_api uart)

#install binaries
install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/app.hex
    DESTINATION ${CMAKE_SOURCE_DIR}/assets/app
)

