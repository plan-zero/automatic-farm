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

set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/../cmake_config/generic-avr-gcc.cmake")

# Name your project
project(APP)

if(TASK_TEST)
    add_definitions(-DENABLE_TASK_TEST)
endif()

set(HEADER_INC
    ./Scheduler
    ./Abstractization/Sensors/Humidity
    ./Abstractization/Sensors/Light
    ./Abstractization/Actuators/Relay
    ./Abstractization/Actuators/Valve
    ./Communication
    ./SwOta
    ./../libraries/drivers/timer/inc
    ./../libraries/drivers/uart/inc
    ./../libraries/drivers/e2p/inc
    ./../libraries/drivers/wdg/inc
    ./../libraries/modules/nrf24/inc
    ./../libraries/modules/system-timer/inc
    ./../libraries/hardware/${AVR_MCU}
)
include_directories(${HEADER_INC})


set(SOURCE_FILES
    ./App/Main.c
    ./Scheduler/Scheduler.c
    ./Abstractization/Sensors/Humidity/Humidity.c
    ./Communication/Communication.c
    ./Communication/radio_link.c
    ./Communication/network_def.c
    ./SwOta/ota.c
)
avr_add_executable(app ${SOURCE_FILES})

target_link_libraries(app.elf nrf24_api uart timer wdg nrf24_api system-timer)

#install binaries
install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/app.hex
    DESTINATION ${CMAKE_SOURCE_DIR}/assets/${AVR_MCU}/app
)

