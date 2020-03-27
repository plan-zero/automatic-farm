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

#include "ota.h"
#include "e2p.h"
#include "stdlib.h"
#include "wdg.h"
#include "uart.h"
#include "util/delay.h"
#include "interrupt_hw.h"

typedef enum{
    ota_valid = 0,
    ota_invalid = 1,
}ota_validation_t;

uint8_t boot_key[PROGRAMMER_KEY_LENGTH];

inline ota_validation_t ota_validate()
{
    //TODO: add implementation
    return ota_valid;
}

void ota_get_key()
{
    e2p_read_boot_key(boot_key);
}

void ota_check_key(uint8_t *data, uint8_t msglen)
{
    uint8_t data_len = 0;
    if(msglen)
        data_len = msglen - 1;

    if(PROGRAMMER_KEY_LENGTH == data_len)
    {
        if(0 == memcmp(&boot_key, &data[1], data_len))
        {
            //the application has to enter in programming mode
            //TODO: add the validation mechanism for OTA
            if(ota_valid == ota_validate())
            {
                wdg_disable(); //make sure that there is enough time to write the E2P block
                INT_GLOBAL_DIS();
                uint8_t data[FLASH_CKS_LENGTH + DOWNLOAD_FLAG_LENGTH] = {0};
                data[0] = e2p_download_enable;
                e2p_update_downloadflag(data);
                e2p_update_flashchecksum(data);
                wdg_init(wdgto_15MS);
                wdg_explicit_reset(OTA_UPDATE_RESET);
            }
        }
    }
    
}

