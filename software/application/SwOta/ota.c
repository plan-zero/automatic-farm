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
#include "Communication.h"
#include "network_common.h"

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



inline void _ota_check_key(message_t msg, uint8_t datalen)
{
    if(0 == memcmp(&boot_key, &msg.data, datalen))
    {
        //the application has to enter in programming mode
        //TODO: add the validation mechanism for OTA
        if(ota_valid == ota_validate())
        {
            wdg_disable(); //make sure that there is enough time to write the E2P block
            INT_GLOBAL_DIS();
            uint8_t data[FLASH_CKS_LENGTH + DOWNLOAD_FLAG_LENGTH] = {0};
            data[0] = e2p_download_enable;
            e2p_update_txaddress(msg.tx_address);
            e2p_update_downloadflag(data);
            e2p_update_flashchecksum(data);
            wdg_init(wdgto_15MS);
            wdg_explicit_reset(OTA_UPDATE_RESET);
        }
    }
}

void ota_prepare(message_t msg, uint8_t datalen)
{
    uart_printString("ota prepare...",1);
    if(datalen == 1 && msg.data[0] == 'R')
    {
        //swap address
        //uint8_t tmp[5] = {0};
        //memcpy(&tmp, &msg.rx_address, 5);
        //memcpy(&msg.rx_address, &msg.tx_address, 5);
        //memcpy(&msg.tx_address, &tmp, 5);
        msg.data[0] = 'O';
        msg.data[1] = 'T';
        msg.data[2] = 'A';
        msg.data[3] = 'O';
        msg.data[4] = 'K';
        memcpy(&msg.data[5], &network_rx_default_address, 5);
        _delay_ms(150);
        communication_outbox_add(msg, 24);
        uart_printString("ota send rx def...",1);
    }
    else if (PROGRAMMER_KEY_LENGTH == datalen)
    {
        _ota_check_key(msg, datalen);
    }
}