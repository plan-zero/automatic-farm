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
#include "nrf24Radio_API.h"
#include "messages.h"
#include "string.h"

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
            //power down the radio
            __nrfRadio_PowerDown();
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
        uint8_t data[10] = {'O', 'T', 'A', 'O', 'K','0','0','0','0','0'};
        message_t ota_response = {0};
        memcpy(&data[5], &network_rx_default_address, RADIO_MAX_ADDRESS);
        message_create(START_BYTE_BOOTKEY ,&ota_response ,msg.tx_address, &data, sizeof(data));
        communication_outbox_add(ota_response, 24, 15);
        uart_printString("ota send rx def...",1);
    }
    else if (PROGRAMMER_KEY_LENGTH == datalen)
    {
        _ota_check_key(msg, datalen);
    }
}