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

#define COMMUNICATION_TX_FIFO_SIZE 3
#define COMMUNICATION_MAX_OUT_BUFFER 5
message_packet_t msg_out_buffer[COMMUNICATION_MAX_OUT_BUFFER];


void rx_handler(uint8_t pipe, uint8_t * data, uint8_t payload_length)
{
    //assemble the message in a known format
    
    for(uint8_t idx = 0; idx < COMMUNICATION_MAX_IN_BUFFER; idx++)
    {
        //find an empty slot
        if(msg_in_buffer[idx].status == msg_status_empty)
        {
            if(messages_is_this_rx(data, payload_length))
            {
                uart_printString("message received, processing",1);
                messages_pack(&msg_in_buffer[idx], data, payload_length);
                msg_in_buffer[idx].status = msg_status_processing;
            }
            else if(payload_length >= 14)
            {
                //copy the message to buffer
                uart_printString("message received, fw",1);
                memcpy(&msg_in_buffer[idx].msg.raw, data, payload_length);
                msg_in_buffer[idx].status = msg_status_forwarding;
            }
            else
            {
                uart_printString("message received, ignore",1);
            }
            break;
        }
    }
}

/* this is not used for the moment
void tx_handler(radio_tx_status status)
{

}*/

void communication_init()
{
    for(uint8_t idx = 0; idx < COMMUNICATION_MAX_OUT_BUFFER; idx++)
    {
        msg_out_buffer[idx].status = msg_status_empty;
    }
    for(uint8_t idx = 0; idx < COMMUNICATION_MAX_IN_BUFFER; idx++)
    {
        msg_in_buffer[idx].status = msg_status_empty;
    }
}

communication_status_t communication_outbox_add(message_t msg, uint8_t len, uint8_t delay)
{
    for(uint8_t idx = 0; idx < COMMUNICATION_MAX_OUT_BUFFER; idx++)
    {
        if(msg_out_buffer[idx].status == msg_status_empty)
        {
            memcpy(&msg_out_buffer[idx].msg.raw, &msg.raw, sizeof(message_t));
            msg_out_buffer[idx].id = GLOBAL_MSG_ID++;
            msg_out_buffer[idx].status = msg_status_pending;
            msg_out_buffer[idx].data_length = len;
            msg_out_buffer[idx].delay = delay;
            msg_out_buffer[idx].retry = 3; //by default
            return communication_outbox_ok;
        }

    }
    return communication_outbox_full;
}

void communication_send_messages()
{

    for(uint8_t idx = 0; idx < COMMUNICATION_MAX_OUT_BUFFER; idx++)
    {
        if(msg_out_buffer[idx].status == msg_status_pending)
        {
            if( msg_out_buffer[idx].delay == 0 )
            {

                uart_printString("Send msg from outbox...",1);
                __nrfRadio_TransmitMode();
                __nrfRadio_LoadMessages(msg_out_buffer[idx].msg.raw, msg_out_buffer[idx].data_length);
                radio_tx_status status = __nrfRadio_Transmit(msg_out_buffer[idx].msg.tx_address, RADIO_WAIT_TX);
                __nrfRadio_ListeningMode();
                if(RADIO_TX_OK == status || RADIO_TX_OK_ACK_PYL == status)
                {
                    //we'll delete this message because it was sent
                    memset(&msg_out_buffer[idx], 0, sizeof(message_packet_t));
                    msg_out_buffer[idx].status = msg_status_empty;
                }
                else
                {
                    uart_printString("MSG:NACK",1);
                    msg_out_buffer[idx].status = msg_status_not_sent;
                    //TODO: add an error handler for this case
                }
            }
            else
            {
                msg_out_buffer[idx].delay--;
            }
        }
        else if(msg_out_buffer[idx].status == msg_status_not_sent)
        {
            if(msg_out_buffer[idx].retry)
            {
                __nrfRadio_TransmitMode();
                __nrfRadio_LoadMessages(msg_out_buffer[idx].msg.raw, msg_out_buffer[idx].data_length);
                radio_tx_status status = __nrfRadio_Transmit(msg_out_buffer[idx].msg.tx_address, RADIO_WAIT_TX);
                __nrfRadio_ListeningMode();
                if(RADIO_TX_OK == status || RADIO_TX_OK_ACK_PYL == status)
                {
                    //we'll delete this message because it was sent
                    memset(&msg_out_buffer[idx], 0, sizeof(message_packet_t));
                    msg_out_buffer[idx].status = msg_status_empty;
                }
                else
                {
                    msg_out_buffer[idx].retry--;
                    uart_printString("MSG:NACK",1);
                }
            }

            if(msg_out_buffer[idx].retry == 0)
            {
                    //we'll delete this message because it can't be sent
                    memset(&msg_out_buffer[idx], 0, sizeof(message_packet_t));
                    msg_out_buffer[idx].status = msg_status_empty;
            }
        }
        
    }
}

void communication_execute_messages()
{
    for(uint8_t idx = 0; idx < COMMUNICATION_MAX_IN_BUFFER; idx++)
    {
        //if the message is processing
        if(msg_in_buffer[idx].status == msg_status_processing)
            switch (msg_in_buffer[idx].msg.type)
            {
            case START_BYTE_BROADCAST:
                {
                    uart_printString("Broadcast message received...",1);
                    if(radio_link_execute(&msg_in_buffer[idx]))
                    {
                        msg_in_buffer[idx].status == msg_status_executing;
                    }
                }
                break;
            case START_BYTE_BOOTKEY:
                {
                    ota_prepare(msg_in_buffer[idx].msg, msg_in_buffer[idx].data_length);
                    memset(&msg_in_buffer[idx],0,sizeof(message_packet_t));
                    msg_in_buffer[idx].status == msg_status_empty;
                }
                break;
            case START_BYTE_PAIRING:
                {
                    uart_printString("Pairing message received...",1);
                    if(radio_link_execute(&msg_in_buffer[idx]))
                    {
                        msg_in_buffer[idx].status == msg_status_executing;
                    }
                }
                break;
            case START_BYTE_JOINNET:
                {
                    uart_printString("Send this msg to the root",1);
                    message_t msg = {0};
                    message_create(START_BYTE_JOINNET, &msg, radio_link_get_root_tx(), msg.data, sizeof(msg.data));
                    communication_outbox_add(msg, 19, 0);
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
            //
        else if(msg_in_buffer[idx].status == msg_status_forwarding)
        {
            //fw the message here
        }
    }
}