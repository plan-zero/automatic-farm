#ifndef _E2P_H
#define _E2P_H

#include "stdlib.h"
#include "e2p_layout.h"

#define e2p_update_downloadflag(value) sizeof(value) / sizeof(unsigned char) ? -1 : eeprom_update_block ((void*)value, (void*)DOWNLOAD_FLAG_ADDRESS, DOWNLOAD_FLAG_LENGTH)

#endif