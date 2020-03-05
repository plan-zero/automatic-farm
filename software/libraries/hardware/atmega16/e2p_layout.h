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

/*
* E2P_Layout.h
*
* Created: 12/2/2019 9:16:29 PM
*  Author: Adi
*/ 

/*This is automatically generated by cmake, don't edit manually! Change Cmake configuration for other values (CMakeLists.txt & .h.in file)*/
/* AVR_MCU = atmega16 */

#ifndef E2P_LAYOUT_H_
#define E2P_LAYOUT_H_

#define RADIO_RX_ADDRESS               (const uint8_t*) 0
#define RADIO_RX_LENGTH                (uint8_t) 5

#define E2P_CKS_ADDRESS                (const uint8_t*) 5
#define E2P_CKS_LENGTH                 (uint8_t) 4

#define FLASH_CKS_ADDRESS              (const uint8_t*) 9
#define FLASH_CKS_LENGTH               (uint8_t) 4  

#define NRF_VERS_ADDRESS               (const uint8_t*) 13
#define NRF_VERS_LENGTH                (uint8_t) 2   

#define APP_VERS_ADDRESS               (const uint8_t*) 15
#define APP_VERS_LENGTH                (uint8_t) 2  

#define E2P_VERS_ADDRESS               (const uint8_t*) 17
#define E2P_VERS_LENGTH                (uint8_t) 2 

#define RESERVED1_ADDRESS              (const uint8_t*) 19
#define RESERVED1_LENGTH               (uint8_t) 2  

#define DOWNLOAD_FLAG_ADDRESS          (const uint8_t*) 21
#define DOWNLOAD_FLAG_LENGTH           (uint8_t) 1

#define PROGRAMMER_ADDR_ADDRESS        (const uint8_t*) 22
#define PROGRAMMER_ADDR_LENGTH         (uint8_t) 5 

#define RESERVED2_ADDRESS              (const uint8_t*) 27
#define RESERVED2_LENGTH               (uint8_t) 2  

#define PROGRAMMER_SECRET_KEY          (const uint8_t*) 29
#define PROGRAMMER_KEY_LENGTH          (uint8_t) 10 

#define E2P_SIZE                       (uint16_t)39

#define EEP_MEMORY_SECTION __attribute__((section(".eeprom")))

#endif
