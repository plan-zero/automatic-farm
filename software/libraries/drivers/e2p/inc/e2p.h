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