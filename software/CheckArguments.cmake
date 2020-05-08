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

#when a new hardware is added, please update these strings as well
set(SUPPORTED_MCU "atmega16 atmega328p")
set(SUPPORTED_FREQ "1000000UL 2000000UL 4000000UL 8000000UL 16000000UL 9216000UL")
set(SUPPORTED_RADIO_RATE "1MBS 2MBS 250KBS") #supported transfer rate for radio, default is 250KBS
 #supported retransmision time in case of ACK fail, default is 3ms
set(SUPPORTED_RADIO_RT "250US 500US 750US 1000US 1250US 1500US 1750US 2000US 2250US 2500US 2750US 3000US 3250US 3500US 3750US 4000US") 

#check input arguments that are passed down to the submodules
#check Radio transfer rate
if(RADIO_RATE)
    string(FIND ${SUPPORTED_RADIO_RATE} ${RADIO_RATE} RES)
    if(${RES} MATCHES "-1")
           message(FATAL_ERROR "RADIO_RATE=${RADIO_RATE} is not a valid configuration. The supported rates are: ${SUPPORTED_RADIO_RATE}")
    endif()
else()
    message(WARNING "The RADIO_RATE is not set, this is set to default: 250KBS")
    set(RADIO_RATE 250KBS)
endif()
#check Radio retransmision time
if(RADIO_RT)
    string(FIND ${SUPPORTED_RADIO_RT} ${RADIO_RT} RES)
    if(${RES} MATCHES "-1")
           message(FATAL_ERROR "RADIO_RT=${RADIO_RT} is not a valid configuration. The supported RT are: ${SUPPORTED_RADIO_RT}")
    endif()
else()
    message(WARNING "The RADIO_RT is not set, this is set to default: 3000us")
    set(RADIO_RT 3000US)
endif()
#check AVR_MCU
if(AVR_MCU)
    string(FIND ${SUPPORTED_MCU} ${AVR_MCU} RES)
    if(${RES} MATCHES "-1")
           message(FATAL_ERROR "AVR_MCU=${AVR_MCU} is not a valid configuration. The supported MCUs are: ${SUPPORTED_MCU}")
    endif()
else()
    message(WARNING "The AVR_MCU is not set, this is set to default: atmega16")
    set(AVR_MCU atmega16)
endif()

if(MCU_FREQ)
    string(FIND ${SUPPORTED_FREQ} ${MCU_FREQ} RES)
    if(${RES} MATCHES "-1")
        message(FATAL_ERROR "MCU_FREQ=${AVR_MCU} is not a valid configuration. The supported MCU FREQ are: ${SUPPORTED_FREQ}")
    endif()
else()
    message(WARNING "The MCU_FREQ is not set, this is set to default: 8000000UL")
    set(MCU_FREQ 8000000UL)
endif()