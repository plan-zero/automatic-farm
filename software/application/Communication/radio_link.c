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

#include "radio_link.h"
#include "stdlib.h"
#include "string.h"

#include "nrf24Radio.h"
#include "nrf24Radio_API.h"
#include <util/delay.h>
#include "network_common.h"
#include "e2p.h"
#include "uart.h"
#include "system-timer.h"
#include <avr/io.h>

#define STATE_COUNT_3S      300
#define STATE_COUNT_2S      200
#define STATE_COUNT_1S      100
#define STATE_COUNT_500MS   50
#define STATE_COUNT_100MS   10

typedef enum{
    link_none,
    link_establising,
    link_established,
    link_lost,
    link_error,
}link_status_t;

#define MAX_FAILED_ACK 5

typedef struct{
    uint8_t radio_link_status;
    uint8_t radio_tx_pipe_address[RADIO_MAX_ADDRESS];
    uint8_t radio_rx_pipe_address[RADIO_MAX_ADDRESS];
    radio_pipe pipe;
    uint8_t failed_ack;
}radio_link_t;

typedef union{
    struct{
        uint8_t rx_address[5];
        uint8_t tx_address[5];
        uint8_t cmd_data[3];
    };
    uint8_t data[12];
}radio_link_cmd_t;

#define MAX_CMD 3
radio_link_cmd_t _cmds[MAX_CMD];
uint8_t cmd_idx = 0;

uint16_t min_latency = 0xFFFF;
uint8_t selected_tx[RADIO_MAX_ADDRESS];

#define MAX_MASTER_COUNT 3
uint8_t tx_address_idx = 0;
uint8_t *tx_address_latency[3];

radio_link_t root = {link_none, {0,0,0,0,0}, {0,0,0,0,0}, RADIO_PIPE0};

void radio_link_init()
{
    __nrfRadio_PowerDown();

    
	pipe_config default_pipe =
	{
		RADIO_PIPE0,
		network_rx_default_address,
		RADIO_MAX_ADDRESS,
		RADIO_PIPE_RX_ENABLED,
		RADIO_PIPE_AA_ENABLED,
		RADIO_PIPE_DYNAMIC_PYALOAD_ENABLED
	};
    
	__nrfRadio_PipeConfig(default_pipe);
    __nrfRadio_PowerUp();
    __nrfRadio_TransmitMode();
}

uint8_t radio_link_execute(message_t msg)
{
    if(cmd_idx < MAX_CMD)
    {
        uart_printString("Received execute cmd...",1);
        memcpy(_cmds[cmd_idx].tx_address, msg.tx_address, RADIO_MAX_ADDRESS);
        memcpy(_cmds[cmd_idx].rx_address, msg.rx_address, RADIO_MAX_ADDRESS);
        memcpy(_cmds[cmd_idx].cmd_data, msg.data, 3);
        cmd_idx++;
        return 1;
    }
    return 0;
}

uint8_t radio_link_configure(uint8_t *address_tx, uint8_t *address_rx, uint8_t _address_length)
{
    if(address_rx && _address_length <= RADIO_MAX_ADDRESS)
    {
        memcpy(root.radio_tx_pipe_address, address_tx, _address_length);
        memcpy(root.radio_rx_pipe_address, address_rx, _address_length);
    }
    __nrfRadio_PowerDown();
    pipe_config rx_root =
	{
		RADIO_PIPE0,
		root.radio_rx_pipe_address,
		_address_length,
		RADIO_PIPE_RX_ENABLED,
		RADIO_PIPE_AA_ENABLED,
		RADIO_PIPE_DYNAMIC_PYALOAD_ENABLED
	};
    __nrfRadio_PipeConfig(rx_root);
    __nrfRadio_PowerUp();
    __nrfRadio_ListeningMode();
    root.radio_link_status = link_establising;
    root.pipe = RADIO_PIPE0;
    return 1;
}

uint8_t radio_link_discovery()
{
    //we start to send a broadcast message to see if there is any node available for linking
    
    uint8_t broadcast_msg[6] = {0};
    broadcast_msg[0] = 'B';
    memcpy(&broadcast_msg[1], &network_rx_default_address[0], 5);
    __nrfRadio_LoadMessages(broadcast_msg, 6);
    uint8_t status = __nrfRadio_Transmit(network_broadcast_address, RADIO_WAIT_TX);
    if(status == RADIO_TX_OK){
        //the mesage was received, enter in listening and wait instructions on pipe0
        uart_printString("Master ACK received!",1);
        __nrfRadio_ListeningMode();
        return 1;
    }
    return 0;
}

typedef enum{
    link_init,
    link_discovery,
    link_wait_master,
    link_ping_master,
    link_pairing,
    link_paired,
    link_failed,
}link_state_t;

#include <avr/io.h>

void radio_link_task()
{
    static link_state_t state = link_init;
    static uint16_t state_count = 0;

    state_count++;
    switch (state)
    {
    case link_init:
        {
            radio_link_init(network_broadcast_address,5);
            state = link_discovery;
            tx_address_idx = 0;
            state_count = 0;
            uart_printString("Enter discovering...",1);
        }
        break;
    case link_discovery:
        if(state_count % STATE_COUNT_1S == 0)
        {
            state_count = 0;
            
            if(radio_link_discovery())
            {
                uart_printString("Enter wait master...",1);
                state = link_wait_master;
                min_latency = 0xFFFF;
                memset(tx_address_latency, 0, RADIO_MAX_ADDRESS);
            }
        }
        break;
    case link_wait_master:
        {
            for(uint8_t idx = 0; idx < cmd_idx; idx++)
            {
                //check just the master answers
                if(_cmds[idx].cmd_data[0] == '1' && tx_address_idx < MAX_MASTER_COUNT)
                {
                    uart_printString("Master request ...",1);
                    //save master address
                    tx_address_latency[tx_address_idx] = malloc(sizeof(uint8_t)* RADIO_MAX_ADDRESS);
                    if(tx_address_latency[tx_address_idx])
                    {
                        memcpy(tx_address_latency[tx_address_idx], _cmds[idx].tx_address, RADIO_MAX_ADDRESS);
                        tx_address_idx++;
                    }
                }
                //clean cmd message
                memset(&_cmds[idx], 0, sizeof(radio_link_cmd_t));
            }
            cmd_idx = 0;
            //after 2 seconds
            if(state_count >= STATE_COUNT_3S)
            {
                state_count = 0;
                if(tx_address_idx)
                {
                    uart_printString("Calculate ping for masters ...",1);
                    state = link_ping_master;
                    //change radio to transmit cause we'll start the ping the masters
                    __nrfRadio_TransmitMode();
                }
                else
                {
                    state = link_init;
                }
                
            }
        }
        break;
    case link_ping_master:
        {
            for(uint8_t idx = 0; idx < tx_address_idx; idx++)
            {
                uart_printString("ping master ...",1);
                int32_t delta = 0;
                system_timer_timestamp _start = {0};
                system_timer_timestamp _end = {0};
                uint8_t ping[1] = {'A'};
                uint8_t mean_latency = 0;
                
                
                if(tx_address_latency[idx])
                {
                    for(uint8_t itr = 0; itr < 4; itr++)
                    {
                        uart_printString("Ping test",1);
                        __nrfRadio_LoadMessages(ping, 1);
                        _start =  system_timer_get_timestamp();
                        radio_tx_status ping_status =  __nrfRadio_Transmit(tx_address_latency[idx], RADIO_WAIT_TX);
                        _end = system_timer_get_timestamp();
                        if(RADIO_TX_OK == ping_status)
                        {
                            delta = system_timer_getus(_start,_end);
                            mean_latency += delta;
                        }
                        else
                        {
                            uart_printString("NACK",1);
                            //TODO: raise an error since we don't get the ack
                        }
                    }
                    mean_latency = mean_latency / 4;
                    //DBG INFO
                    uint8_t tmp[12] = {0};
                    sprintf(tmp, "%u",mean_latency);
                    tmp[11] = '\0';
                    uart_printString(tmp, 1);
                    /////////////////////////
                    if(mean_latency < min_latency)
                    {
                        uart_printString("save new master..",1);
                        min_latency = mean_latency;
                        memcpy(selected_tx, tx_address_latency[idx], RADIO_MAX_ADDRESS);
                    }
                }
                //free memory that was allocated for tx latency calculation
                free(tx_address_latency[idx]); 
            }

            //send the request to the master
            uint8_t request_msg[10] = {0};
            request_msg[0] = 'P';
            request_msg[1] = 'O';
            request_msg[2] = 'K';
            __nrfRadio_LoadMessages(request_msg, 3);
            radio_tx_status res = __nrfRadio_Transmit(selected_tx, RADIO_WAIT_TX);
            if(RADIO_TX_OK == res)
            {
                __nrfRadio_ListeningMode();
                __nrfRadio_FlushBuffer(RADIO_BOTH_BUFFER);
                state = link_pairing;
            }
            else
            {
                uart_printString("no master ack",1);
                state = link_init;
            }
                
            
        }
        break;
    case link_pairing:
        //TODO: Add a timeout for pairing, then go back in discovery if there is no answer
        for(uint8_t idx = 0; idx < cmd_idx; idx++)
        {
            uart_printString("execute cmd...",1);
            switch (_cmds[idx].cmd_data[0])
            {
            case '2':
            {
                uart_printString("radio_link execute cmd 2",1);
                //check if this master has the lowest ping rate
                if(0 == memcmp(_cmds[idx].tx_address, selected_tx, RADIO_MAX_ADDRESS))
                {
                    //request pair
                    if(_cmds[idx].cmd_data[1] == 'R')
                    {
                        uart_printString("request pair...", 1);
                        uint8_t config_ack_msg[7] = {'O','K'};
                        memcpy(&config_ack_msg[2], _cmds[idx].tx_address, RADIO_MAX_ADDRESS);
                        __nrfRadio_LoadAckPayload(RADIO_PIPE0, config_ack_msg, 7);
                    }
                    //Configure with the following configuration
                    else if(_cmds[idx].cmd_data[1] == 'P')
                    {
                        uart_printString("configure link...", 1);
                        radio_link_configure(_cmds[idx].tx_address,
                                            _cmds[idx].rx_address, 
                                            RADIO_MAX_ADDRESS);
                    }
                }
                else
                {
                    //refuse the pairing, there is another master that is more suitable
                    uint8_t config_ack_msg[8] = {'N','O','K'};
                    memcpy(&config_ack_msg[3], _cmds[idx].tx_address, RADIO_MAX_ADDRESS);
                    __nrfRadio_LoadAckPayload(RADIO_PIPE0, config_ack_msg, 8);
                }
                break;
            }
            break;
            //check the connection and if this is stable, then the pairing is complete
            case '3':
            {
                if(0 == memcmp(_cmds[idx].tx_address, selected_tx, RADIO_MAX_ADDRESS))
                {
                    if(_cmds[idx].cmd_data[1] == 'C')
                    {
                        uart_printString("check connection...", 1);
                        uint8_t check_con_msg[7] = {'O','K'};
                        memcpy(&check_con_msg[2], _cmds[idx].tx_address, RADIO_MAX_ADDRESS);
                        __nrfRadio_LoadAckPayload(RADIO_PIPE0, check_con_msg, 7);
                    }
                    else if(_cmds[idx].cmd_data[1] == 'D')
                    {
                        memset(tx_address_latency, 0, RADIO_MAX_ADDRESS);
                        min_latency = 0xFFFF;
                        uart_printString("connection established...", 1);
                        state = link_paired;
                        root.radio_link_status = link_established;
                    }
                }
                break;
            }
            default:
                break;
            }
            memset(&_cmds[idx], 0, sizeof(radio_link_cmd_t));
        }
        cmd_idx = 0;
        break;
    case link_paired:
        //send an ACK msg to parent to make sure that the connection is still working
        if(state_count % STATE_COUNT_1S == 0)
        {
            state_count = 0;
            __nrfRadio_TransmitMode();
            uint8_t ack_msg[1] = {'A'};
            __nrfRadio_LoadMessages(ack_msg, 1);
            radio_tx_status status = __nrfRadio_Transmit(root.radio_tx_pipe_address, RADIO_WAIT_TX);
            __nrfRadio_ListeningMode();
            if(RADIO_TX_OK == status)
            {
                //uart_printString("ACK Root: OK",1);
                if(root.failed_ack)
                    root.failed_ack--;
            }
            else
            {
                uart_printString("ACK Root: NOK",1);
                root.failed_ack++;
            }
            if(root.failed_ack >= MAX_FAILED_ACK)
            {
                uart_printString("Connection lost...",1);
                state = link_failed;
            }
        }
        break;
    case link_failed:
        memset(&root, 0, sizeof(radio_link_t));
        __nrfRadio_FlushBuffer(RADIO_BOTH_BUFFER);
        state = link_init;
        break;
    default:
        break;
    }
}