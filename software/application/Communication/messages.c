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

#include "messages.h"
#include "stdint.h"
#include "network_common.h"
#include "nrf24Radio.h"
#include "string.h"

uint8_t GLOBAL_MSG_ID = 0;
#define DEFAULT_RETRY_MSG 3

void messages_pack(message_packet_t *msg_pachet, uint8_t *data, uint8_t data_len)
{
    if( (data_len <= sizeof(message_t)) && (data_len >= 14))
    {
        memcpy(&msg_pachet->msg.raw, data, data_len); 
        //considering that the message is for this device, rearange the rx and tx
        memcpy(&msg_pachet->msg.rx_address, &data[6], RADIO_MAX_ADDRESS);
        memcpy(&msg_pachet->msg.tx_address, &data[1], RADIO_MAX_ADDRESS);
        msg_pachet->id = GLOBAL_MSG_ID++; //assign an ID to recgonize it later
        msg_pachet->data_length = data_len - 14;
    }
}

uint8_t messages_is_this_rx(uint8_t *data, uint8_t data_len)
{
    if(data_len >= 14)
        if(0 == memcmp(&data[6], network_rx_default_address, RADIO_MAX_ADDRESS))
        {
            return 1;
        }
    return 0;
}

void message_create(uint8_t type, message_t *msg, uint8_t *tx, uint8_t *data, uint8_t data_len)
{
    msg->type = type;
    memcpy(msg->rx_address, network_rx_default_address, RADIO_MAX_ADDRESS);
    memcpy(msg->tx_address, tx, RADIO_MAX_ADDRESS);
    //if(sizeof(msg->data) >= data_len)
    msg->TTL = 0x30;
    msg->timestamp = 0x3030;
    {
        memcpy(&msg->data, data, data_len);
    }
}