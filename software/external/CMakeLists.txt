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

cmake_minimum_required(VERSION 3.0)

include(${CMAKE_ROOT}/Modules/ExternalProject.cmake)

set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/cmake_config/generic-avr-gcc.cmake")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}")

project(EXTERNAL)


if(ENABLE_AES_TINY)
#add AES external project
set(AES_REPO "https://github.com/plan-zero/tiny-AES-c.git")
set(aec_DESTDIR ${CMAKE_CURRENT_BINARY_DIR}/lib/)
get_filename_component(TOOLCHAIN_ABSOLUTE_PATH "./../cmake_config/generic-avr-gcc.cmake" ABSOLUTE)
ExternalProject_Add(AES_EXT
            GIT_REPOSITORY    ${AES_REPO}
            GIT_TAG           master
            CMAKE_ARGS -DCMAKE_TOOLCHAIN_FILE=${TOOLCHAIN_ABSOLUTE_PATH} -DAVR_MCU=${AVR_MCU}
            INSTALL_COMMAND cmake -E echo "Skipping install step."
)
ExternalProject_Get_Property(AES_EXT binary_dir)
#get AES include directory
ExternalProject_Get_Property(AES_EXT source_dir)
set(AES_INC_PATH ${source_dir} CACHE INTERNAL "")

add_library(tiny-aes STATIC IMPORTED GLOBAL)
set_target_properties(tiny-aes PROPERTIES 
        IMPORTED_LOCATION ${binary_dir}/libtiny-aes.a
)
endif()
