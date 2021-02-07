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

set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/cmake_config/generic-avr-gcc.cmake")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}")

project(AUTOMATIC_FARM)


include(CheckArguments.cmake)

#download external dependencies
add_subdirectory(external)

if(ENABLE_AES)
add_subdirectory(external/aes)
add_subdirectory(testing/aes_testbench)
endif()

#generate the driver headers
add_subdirectory(libraries/hardware/${AVR_MCU})

#compile the drivers
add_subdirectory(libraries/drivers/spi)
add_subdirectory(libraries/drivers/uart)
add_subdirectory(libraries/drivers/e2p)
add_subdirectory(libraries/drivers/flash)
add_subdirectory(libraries/drivers/timer)
add_subdirectory(libraries/drivers/wdg)

#compile the libs modules
add_subdirectory(libraries/modules/nrf24)
add_subdirectory(libraries/modules/system-timer)
add_subdirectory(libraries/modules/ds18b20)

#compile the firmware
add_subdirectory(firmware)

#compile the programmer sw
add_subdirectory(programmer)


#compile the application
add_subdirectory(application)

#compile the testbench env
add_subdirectory(testing/timer_testbench)
add_subdirectory(testing/system_timer_testbench)

#compile the application
add_subdirectory(testing/oneWireDev_testbench)
                         


