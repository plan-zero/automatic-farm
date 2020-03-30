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


typedef enum{
    link_none,
    link_establising,
    link_established,
    link_lost,
    link_error,
}link_status_t;

typedef struct{
    uint8_t radio_link_status;
    uint8_t radio_tx_pipe_address[RADIO_MAX_ADDRESS];
    uint8_t radio_rx_pipe_address[RADIO_MAX_ADDRESS];
}radio_link_t;

typedef union{
    struct{
        uint8_t rx_address[5];
        uint8_t tx_address[5];
        uint8_t cmd_data[2];
    };
    uint8_t data[12];
}radio_link_cmd_t;

#define MAX_CMD 3
radio_link_cmd_t _cmds[MAX_CMD];
uint8_t cmd_idx = 0;

radio_link_t root = {link_none, {0,0,0,0,0}, {0,0,0,0,0}};

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
        memcpy(_cmds[cmd_idx].tx_address, msg.tx_address, RADIO_MAX_ADDRESS);
        memcpy(_cmds[cmd_idx].rx_address, msg.rx_address, RADIO_MAX_ADDRESS);
        memcpy(_cmds[cmd_idx].data, msg.data, 2);
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
        __nrfRadio_ListeningMode();
        return 1;
    }
    return 0;
}

typedef enum{
    link_init,
    link_discovery,
    link_pairing,
    link_paired,
}link_state_t;

#include <avr/io.h>

void radio_link_task()
{
    static link_state_t state = 0;

    switch (state)
    {
    case link_init:
        {
            radio_link_init(network_broadcast_address,5);
            DDRB |= 1;
            state = link_discovery;
        }
        break;
    case link_discovery:
        PORTB ^= 1;
       if(radio_link_discovery())
        {
            
           state = link_pairing;
       }
        break;
    case link_pairing:
        while(cmd_idx--)
        {
            switch (_cmds[cmd_idx].data[0])
            {
            case '1':
            {
                radio_link_configure(_cmds[cmd_idx].tx_address,
                                    _cmds[cmd_idx].tx_address, 
                                    RADIO_MAX_ADDRESS);
            }
                break;
            case '2':
            {
                uint8_t config_ack_msg[2] = {'O','K'};
                __nrfRadio_LoadAckPayload(RADIO_PIPE0, config_ack_msg, 2);
                break;
            }
            case '3':
            {
                root.radio_link_status = link_established;
                state = link_paired;
                break;
            }
            default:
                break;
            }
        }
        break;
    case link_paired:
        break;
    default:
        break;
    }
}