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

project(MEMORY_LAYOUT)

#FLASH LAYOUT DEFINES
##################################################################

set(APP_CODE_ADDR "0x0" CACHE INTERNAL "")
set(APP_CODE_SIZE "10240" CACHE INTERNAL "")

set(NRF_HOOKS_ADDR "0x2800" CACHE INTERNAL "")
set(NRF_HOOKS_SIZE "64" CACHE INTERNAL "")

set(NRF_CODE_ADDR "0x2840" CACHE INTERNAL "")
set(NRF_CODE_SIZE "4096" CACHE INTERNAL "")

set(BOOTLOADER_CODE_ADDR "0x3800" CACHE INTERNAL "")
set(BOOTLOADER_CODE_SIZE "2048" CACHE INTERNAL "")

set(FLASH_PAGE_COUNT "80")
set(FLASH_PAGE_SIZE  "128")

set(FLASH_SECTION_NRF24 .nrf24 CACHE INTERNAL "")
#we use the same section for spi driver
set(FLASH_SECTION_SPI ${FLASH_SECTION_NRF24} CACHE INTERNAL "")
set(FLASH_SECITON_RADIO_FPTR .radio_fptrs CACHE INTERNAL "")

#EEPROM LAYOUT DEFINES
##################################################################
#this will shift the whole e2p layout, currently we use start at 0
set(EEPROM_START_ADDR "0")

set(RADIO_RX_ADDRESS ${EEPROM_START_ADDR})
set(RADIO_RX_LENGTH  "5")


MATH(EXPR E2P_CKS_ADDRESS "${RADIO_RX_ADDRESS} + ${RADIO_RX_LENGTH}")
set(E2P_CKS_LENGTH "4")

MATH(EXPR FLASH_CKS_ADDRESS "${E2P_CKS_ADDRESS} + ${E2P_CKS_LENGTH}")
set(FLASH_CKS_LENGTH "4")

MATH(EXPR NRF_VERS_ADDRESS "${FLASH_CKS_ADDRESS} + ${FLASH_CKS_LENGTH}")
set(NRF_VERS_LENGTH "2")

MATH(EXPR APP_VERS_ADDRESS "${NRF_VERS_ADDRESS} + ${NRF_VERS_LENGTH}")
set(APP_VERS_LENGTH "2")

MATH(EXPR E2P_VERS_ADDRESS "${APP_VERS_ADDRESS} + ${APP_VERS_LENGTH}")
set(E2P_VERS_LENGTH "2")

MATH(EXPR RESERVED1_ADDRESS "${E2P_VERS_ADDRESS} + ${E2P_VERS_LENGTH}")
set(RESERVED1_LENGTH "2")

MATH(EXPR DOWNLOAD_FLAG_ADDRESS "${RESERVED1_ADDRESS} + ${RESERVED1_LENGTH}")
set(DOWNLOAD_FLAG_LENGTH "1")

MATH(EXPR PROGRAMMER_ADDR_ADDRESS "${DOWNLOAD_FLAG_ADDRESS} + ${DOWNLOAD_FLAG_LENGTH}")
set(PROGRAMMER_ADDR_LENGTH "5")

MATH(EXPR RESERVED2_ADDRESS "${PROGRAMMER_ADDR_ADDRESS} + ${PROGRAMMER_ADDR_LENGTH}")
set(RESERVED2_LENGTH "2")

MATH(EXPR PROGRAMMER_SECRET_KEY "${RESERVED2_ADDRESS} + ${RESERVED2_LENGTH}")
set(PROGRAMMER_KEY_LENGTH "10")

MATH(EXPR E2P_SIZE "${PROGRAMMER_SECRET_KEY} + ${PROGRAMMER_KEY_LENGTH}")

#define E2P memory section - by default the avr-gcc is using eeprom
set(EEP_MEMORY_SECTION .eeprom)


configure_file(flash_layout.h.in ${CMAKE_CURRENT_SOURCE_DIR}/flash_layout.h @ONLY)
configure_file(e2p_layout.h.in ${CMAKE_CURRENT_SOURCE_DIR}/e2p_layout.h @ONLY)