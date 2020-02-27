# Template for a AVR project
cmake_minimum_required(VERSION 2.8)
set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/../../../cmake_config/generic-avr-gcc.cmake")

# Name your project
project(TIMER_TEST)

set(HEADER_INC
    ./../../libraries/drivers/uart/inc
    ./../../libraries/drivers/timer/inc
    ./../../libraries/hardware/${AVR_MCU}
)
include_directories(${HEADER_INC})

set(SOURCE_FILES
    ./timer_test.c
)
avr_add_executable(timer_test ${SOURCE_FILES})
target_link_libraries(timer_test.elf uart timer)


