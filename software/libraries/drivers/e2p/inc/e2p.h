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


#ifndef _E2P_H
#define _E2P_H

#include "stdlib.h"
#include "e2p_layout.h"

#define e2p_update_downloadflag(value) eeprom_update_block ((void*)value, (void*)DOWNLOAD_FLAG_ADDRESS, DOWNLOAD_FLAG_LENGTH)
#define e2p_read_downloadflag(value) eeprom_read_block ((void*)&value,(void*)DOWNLOAD_FLAG_ADDRESS,DOWNLOAD_FLAG_LENGTH)

#define e2p_read_txaddress(value) ((void*)&value,(void*)PROGRAMMER_ADDR_ADDRESS,PROGRAMMER_ADDR_LENGTH)

#define e2p_read_rxaddress(value) eeprom_read_block ((void*)&value,(void*)RADIO_RX_ADDRESS,RADIO_RX_LENGTH)

#define e2p_read_eepchecksum(value) eeprom_read_block((void*)&value,(void*)E2P_CKS_ADDRESS,E2P_CKS_LENGTH)

#define e2p_update_flashchecksum(value) eeprom_update_block ((void*)&value[0], (void*)FLASH_CKS_ADDRESS, FLASH_CKS_LENGTH)
#define e2p_read_flashchecksum(value) eeprom_read_block ((void*)&value,(void*)FLASH_CKS_ADDRESS,FLASH_CKS_LENGTH)

#define e2p_read_nrf24_version(value) eeprom_read_block ((void*)&value,(void*)NRF_VERS_ADDRESS,NRF_VERS_LENGTH)

#define e2p_read_app_version(value) eeprom_read_block ((void*)&value,(void*)APP_VERS_ADDRESS,APP_VERS_LENGTH)

#define e2p_read_eep_version(value) eeprom_read_block ((void*)&value,(void*)E2P_VERS_ADDRESS,E2P_VERS_LENGTH)


#endif