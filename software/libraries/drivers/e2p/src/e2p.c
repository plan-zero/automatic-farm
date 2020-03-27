/**
 * Copyright (C) 2020 Coding Night Romania
 * 
 * This file is part of automatic-farm.
 * 
 * automatic-farm is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * automatic-farm is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with automatic-farm.  If not, see <http://www.gnu.org/licenses/>.
 */


#include "e2p_layout.h"
#include "stdint.h"
#include "string.h"


union e2p_data
{
    struct{
        uint8_t rx_address[RADIO_RX_LENGTH];
        uint8_t e2p_checksum[E2P_CKS_LENGTH];
        uint8_t flash_checksum[FLASH_CKS_LENGTH];
        uint8_t nrf_version[NRF_VERS_LENGTH];
        uint8_t app_version[APP_VERS_LENGTH];
        uint8_t e2p_version[E2P_VERS_LENGTH];
        uint8_t reserved1[RESERVED1_LENGTH];
        uint8_t download_flag[DOWNLOAD_FLAG_LENGTH];
        uint8_t programmer_address[PROGRAMMER_ADDR_LENGTH];
        uint8_t reserved2[RESERVED2_LENGTH];
        uint8_t programmer_key[PROGRAMMER_KEY_LENGTH];
    }e2p_fields;
    uint8_t e2p_raw[E2P_SIZE];
};

union e2p_data e2p_default_data EEP_MEMORY_SECTION = 
{
    .e2p_fields.programmer_key =
        {
         E2P_BOOT_KEY_BYTE_0,
         E2P_BOOT_KEY_BYTE_1,
         E2P_BOOT_KEY_BYTE_2,
         E2P_BOOT_KEY_BYTE_3,
         E2P_BOOT_KEY_BYTE_4,
         E2P_BOOT_KEY_BYTE_5,
         E2P_BOOT_KEY_BYTE_6,
         E2P_BOOT_KEY_BYTE_7,
         E2P_BOOT_KEY_BYTE_8,
         E2P_BOOT_KEY_BYTE_9,
        },
    .e2p_fields.rx_address = 
        {
         E2P_RX_ADDRESS_BYTE_0,
         E2P_RX_ADDRESS_BYTE_1, 
         E2P_RX_ADDRESS_BYTE_2,
         E2P_RX_ADDRESS_BYTE_3,
         E2P_RX_ADDRESS_BYTE_4,
        },
    .e2p_fields.download_flag = E2P_DOWNLOAD_FLAG_VALUE
};
