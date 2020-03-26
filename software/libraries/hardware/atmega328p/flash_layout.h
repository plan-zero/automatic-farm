// Copyright (C) 2020 Coding Night Romania
// 
// This file is part of automatic-farm.
// 
// automatic-farm is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// automatic-farm is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with automatic-farm.  If not, see <http://www.gnu.org/licenses/>.



#ifndef _FLASH_LAYOUT_H
#define _FLASH_LAYOUT_H

/*This is automatically generated by cmake, don't edit manually! Change Cmake configuration for other values (CMakeLists.txt & .h.in file)*/
/* AVR_MCU = atmega328p */

#define NRF_CODE_ADDR              0x6040             //NRF24 code starting address in flash
#define NRF_CODE_SIZE              4032             //NRF24 code SIZE [Bytes]

#define BOOTLOADER_CODE_ADDR       0x7000      //BOOTLOADER code starting address in flash
#define BOOTLOADER_CODE_SIZE       2048      //BOOTLOADER code SIZE [Bytes]

#define NRF_HOOKS_ADDR             0x6000            //NRF24 HOOKS code starting address in flash
#define NRF_HOOKS_SIZE             64            //NRF24 HOOKS SIZE [Bytes]

#define FLASH_PAGE_COUNT           192          //The total number of flash pages
#define FLASH_PAGE_SIZE            128           //The size of one page in [Bytes]

#define APP_CODE_ADDR              0x0             //APP code starting address in flash
#define APP_CODE_SIZE              24576             //APP code SIZE [Bytes]

#define SPI_MEMORY              __attribute__((section(".nrf24")))
#define NRF24_MEMORY            __attribute__((section(".nrf24")))
#define RADIO_FPTRS_MEMORY      __attribute__((used, section(".radio_fptrs")))

#endif