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
project(ONEWIRE_TEST)

set(HEADER_INC
    ./../../libraries/drivers/adc/inc
    ./../../libraries/drivers/uart/inc
    ./../../libraries/drivers/wdg/inc
    ./../../libraries/modules/ds18b20/inc
    ./../../libraries/modules/LowPower/inc
    ./../../libraries/hardware/${AVR_MCU}
)
include_directories(${HEADER_INC})

set(SOURCE_FILES
    ./main.c
)
avr_add_executable(onewire_test ${SOURCE_FILES})
target_link_libraries(onewire_test.elf uart ds18b20 adc lowpower wdg)


