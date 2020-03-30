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

#include "stdlib.h"
#include "stdint.h"
#include "string.h"

#include "nrf24Radio.h"
#include "nrf24Radio_API.h"
#include <util/delay.h>

uint8_t rx_test[5] = {'A','B','C','D','E'};
uint8_t radio_tx_address[RADIO_MAX_ADDRESS];
uint8_t radio_broadcast_address[RADIO_MAX_ADDRESS];

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

radio_link_t parent = {link_none, {0,0,0,0,0}, {0,0,0,0,0}};

void radio_link_init(uint8_t * broadcast_address, uint8_t _address_length)
{
    if(_address_length <= RADIO_MAX_ADDRESS)
        memcpy(radio_broadcast_address, broadcast_address, _address_length);
    __nrfRadio_PowerDown();
	pipe_config broadcast_pipe_rec =
	{
		RADIO_PIPE0,
		rx_test,
		_address_length,
		RADIO_PIPE_RX_ENABLED,
		RADIO_PIPE_AA_ENABLED,
		RADIO_PIPE_DYNAMIC_PYALOAD_ENABLED
	};
    
	__nrfRadio_PipeConfig(broadcast_pipe_rec);
    __nrfRadio_PowerUp();
    __nrfRadio_TransmitMode();
}

uint8_t radio_link_configure(uint8_t *address_tx, uint8_t *address_rx, uint8_t _address_length)
{
    if(address_rx && _address_length <= RADIO_MAX_ADDRESS)
    {
        memcpy(parent.radio_tx_pipe_address, address_tx, _address_length);
        memcpy(parent.radio_rx_pipe_address, address_rx, _address_length);
    }
    __nrfRadio_PowerDown();
    pipe_config rx_root =
	{
		RADIO_PIPE0,
		parent.radio_rx_pipe_address,
		_address_length,
		RADIO_PIPE_RX_ENABLED,
		RADIO_PIPE_AA_ENABLED,
		RADIO_PIPE_DYNAMIC_PYALOAD_ENABLED
	};
    __nrfRadio_PipeConfig(rx_root);
    __nrfRadio_PowerUp();
    __nrfRadio_ListeningMode();
    parent.radio_link_status = link_establising;
    
}

uint8_t radio_link_discovery()
{
    //we start to send a broadcast message to see if there is any node available for linking
    
    uint8_t broadcast_msg[6] = {0};
    broadcast_msg[0] = 'B';
    memcpy(&broadcast_msg[1], rx_test, 5);
    __nrfRadio_LoadMessages(broadcast_msg, 6);
    uint8_t status = __nrfRadio_Transmit(radio_broadcast_address, RADIO_WAIT_TX);
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
            uint8_t board_castaddr[5] = {'A','A','A','A','A'};
            radio_link_init(board_castaddr,5);
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

        break;
    default:
        break;
    }
}