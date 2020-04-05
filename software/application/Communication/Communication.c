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
message_packet_t msg_in_buffer[COMMUNICATION_MAX_IN_BUFFER];
uint8_t msg_in_count = 0;

#define COMMUNICATION_TX_FIFO_SIZE 3
#define COMMUNICATION_MAX_OUT_BUFFER 5
message_packet_t msg_out_buffer[COMMUNICATION_MAX_OUT_BUFFER];
uint8_t msg_out_count = 0;

uint8_t GLOBAL_MSG_ID = 0;
uint8_t pending_msg = 0;

void rx_handler(uint8_t pipe, uint8_t * data, uint8_t payload_length)
{
    //assemble the message in a known format
    if(payload_length <= (sizeof(message_t)) && msg_in_count < COMMUNICATION_MAX_IN_BUFFER)
    {
        memcpy(&msg_in_buffer[msg_in_count].msg.raw, data, payload_length);
        msg_in_buffer[msg_in_count].id = GLOBAL_MSG_ID++; //assign an ID to recgonize it later
        if(payload_length > 14) //this is the header of msg
            msg_in_buffer[msg_in_count].data_length = payload_length - 14;
        else
            msg_in_buffer[msg_in_count].data_length = 0; //can't determine the msg length
        msg_in_count++;
    }
}

/* this is not used for the moment
void tx_handler(radio_tx_status status)
{

}*/

communication_status_t communication_outbox_add(message_t msg, uint8_t len)
{
    if(msg_out_count < COMMUNICATION_MAX_OUT_BUFFER)
    {
        memcpy(&msg_out_buffer[msg_out_count].msg, &msg, sizeof(message_t));
        msg_out_buffer[msg_out_count].id = GLOBAL_MSG_ID++;
        msg_out_buffer[msg_out_count].status = msg_status_pending;
        msg_out_buffer[msg_out_count].data_length = len;
        msg_out_count++;
        return communication_outbox_ok;
    }
    return communication_outbox_full;
}

void communication_send_messages()
{

    if(msg_out_count)
    {
        uint8_t not_sent_count = 0;
        __nrfRadio_TransmitMode();
        for(uint8_t idx = 0; idx < msg_out_count; idx++)
        {
            uart_printString("Send msg from outbox...",1);
            __nrfRadio_LoadMessages(msg_out_buffer[idx].msg.raw, msg_out_buffer[idx].data_length);
            radio_tx_status status = __nrfRadio_Transmit(msg_out_buffer[idx].msg.tx_address, RADIO_WAIT_TX);
            if(RADIO_TX_OK == status || RADIO_TX_OK_ACK_PYL == status)
            {
                //we'll delete this message because it was sent
                memset(&msg_out_buffer[idx], 0, sizeof(message_packet_t));
                msg_out_buffer[idx].status = msg_status_empty;
            }
            else
            {
                not_sent_count++;
                uart_printString("MSG:NACK",1);
                msg_out_buffer[idx].status = msg_status_not_sent;
                //TODO: add an error handler for this case
            }
            
        }
        if(not_sent_count)
        {
            //rearange messages
        }
        msg_out_count = 0;
        __nrfRadio_ListeningMode();
    }
}

void communication_execute_messages()
{
    for(uint8_t idx = 0; idx < msg_in_count; idx++)
    {
        switch (msg_in_buffer[idx].msg.type)
        {
        case START_BYTE_BROADCAST:
            {

            }
            break;
        case START_BYTE_BOOTKEY:
            {
                ota_prepare(msg_in_buffer[idx].msg, msg_in_buffer[idx].data_length);
            }
            break;
        case START_BYTE_PAIRING:
            {
                uart_printString("Pairing message received...",1);
                if(radio_link_execute(msg_in_buffer[idx].msg))
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
        memset(&msg_in_buffer[idx],0,sizeof(message_packet_t));
    }
    
    msg_in_count = 0;
}