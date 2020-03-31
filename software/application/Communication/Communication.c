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
#include "Communication.h"
#include "ota.h"
#include "uart.h"
#include "radio_link.h"
#include "messages.h"
#include "string.h"
#include "uart.h"
#include "nrf24Radio_API.h"
#include "nrf24Radio.h"

#define COMMUNICATION_MAX_IN_BUFFER 5
message_t msg_in_buffer[COMMUNICATION_MAX_IN_BUFFER];
uint8_t msg_in_count = 0;

#define COMMUNICATION_MAX_OUT_BUFFER 5
message_t msg_out_buffer[COMMUNICATION_MAX_OUT_BUFFER];
uint8_t msg_out_count = 0;

void rx_handler(uint8_t pipe, uint8_t * data, uint8_t payload_length)
{
    //assemble the message in a known format
    //if(payload_length <= MSG_STRUCT_SIZE && msg_in_count < COMMUNICATION_MAX_IN_BUFFER)
    {
        msg_in_buffer[msg_in_count].type = data[0];
        memcpy(msg_in_buffer[msg_in_count].tx_address, &data[1], RADIO_MAX_ADDRESS);
        memcpy(msg_in_buffer[msg_in_count].rx_address, &data[6], RADIO_MAX_ADDRESS);
        memcpy((uint8_t*)&msg_in_buffer[msg_in_count].timestamp, &data[11], 2);
        msg_in_buffer[msg_in_count].TTL = (uint8_t)data[13];
        memcpy(msg_in_buffer[msg_in_count].data, &data[14], MESSAGE_MAX_LENGTH);
        msg_in_count++;
    }
}

void tx_handler(radio_tx_status status)
{

}

void communication_send_messages()
{
    if(msg_out_count)
    {
        __nrfRadio_TransmitMode();
        for(uint8_t idx = 0; idx < msg_out_count; idx++)
        {
            uint8_t msg[32] = {0};
            msg[0] = msg_out_buffer[idx].type;
            memcpy(&msg[1], msg_out_buffer[idx].rx_address, RADIO_MAX_ADDRESS);
            memcpy(&msg[6], msg_out_buffer[idx].tx_address, RADIO_MAX_ADDRESS);
            memcpy(&msg[11], (uint8_t*)&msg_out_buffer[idx].timestamp, 2);
            msg[13] = msg_out_buffer[idx].TTL;
            memcpy(&msg[14], msg_out_buffer[idx].data, MESSAGE_MAX_LENGTH);
            __nrfRadio_LoadMessages(msg, MSG_STRUCT_SIZE);
            __nrfRadio_Transmit(msg_out_buffer[idx].tx_address, RADIO_RETURN_ON_TX);
        }
    }
    __nrfRadio_ListeningMode();
}

void communication_execute_messages()
{
    for(uint8_t idx = 0; idx < msg_in_count; idx++)
    {
        switch (msg_in_buffer[idx].type)
        {
        case START_BYTE_BROADCAST:
            {

            }
            break;
        case START_BYTE_BOOTKEY:
            {
                //ota_check_key(data, payload_length);
            }
            break;
        case START_BYTE_PAIRING:
            {
                uart_printString("Pairing message received...",1);
                if(radio_link_execute(msg_in_buffer[idx]))
                {
                    //TODO add a warning because this message will be lost
                }
            }
            break;
        case START_BYTE_DATA:
            {

            }
            break;
        case START_BYTE_PING:
            {

            }
            break;
        default:
            break;
        }
        memset(&msg_in_buffer[idx],0,MSG_STRUCT_SIZE);
    }
    
    msg_in_count = 0;
}