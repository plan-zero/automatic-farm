// Copyright (C) 2020 Coding Night Romania
// 
// This file is part of automatic-farm.
// 
// automatic-farm is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// automatic-farm is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with automatic-farm.  If not, see <http://www.gnu.org/licenses/>.


#ifndef _MESSAGES_H
#define _MESSAGES_H

#include "stdint.h"
#include "nrf24Radio.h"
/* COMMON  :ID:PIPEID
DEVICE 00 SLAVE
P0 AA:AA:AA:AA:AA - broadcast
P1 AA:AA:AA:00:01 - master tx
P2 AA:AA:AA:00:02 - *
P3 AA:AA:AA:00:03 - *
P4 AA:AA:AA:00:04 - *
P5 AA:AA:AA:00:05 - *
*/

/* COMMON  :ID:PIPEID
DEVICE 01 MASTER
P0 AA:AA:AA:AA:AA - broadcast
P1 AA:AA:AA:01:01 - *
P2 AA:AA:AA:01:02 - SALVE 00 tx AA:AA:AA:00:01
P3 AA:AA:AA:01:03
P4 AA:AA:AA:01:04
P5 AA:AA:AA:01:05
*/

#define MESSAGE_MAX_LENGTH 18

#define START_BYTE_BROADCAST    (uint8_t)('B')
#define START_BYTE_PAIRING      (uint8_t)('P')
#define START_BYTE_DATA         (uint8_t)('D')
#define START_BYTE_PING         (uint8_t)('U')
#define START_BYTE_BOOTKEY      (uint8_t)('K')
#define START_BYTE_ACK          (uint8_t)('A')
#define START_BYTE_JOINNET      (uint8_t)('J')

typedef enum{
    msg_status_pending,
    msg_status_processing,
    msg_status_executing,
    msg_status_executed,
    msg_status_forwarding,
    msg_status_empty,
    msg_status_not_sent,
}msg_status_t;

typedef union{
    struct{
        uint8_t type; // first byte is the message type
        uint8_t rx_address[RADIO_MAX_ADDRESS]; //the rx address of sending node is actually tx for this
        uint8_t tx_address[RADIO_MAX_ADDRESS]; //same, the tx address of sending node is actually rx for this
        uint16_t timestamp;
        uint8_t TTL;
        uint8_t data[MESSAGE_MAX_LENGTH];
    };
    uint8_t raw[32];
}message_t;

typedef struct{
    message_t msg;
    uint8_t data_length;
    uint8_t id;
    uint8_t delay;
    uint8_t retry;
    msg_status_t status;
}message_packet_t;

extern uint8_t GLOBAL_MSG_ID;

void messages_pack(message_packet_t *msg_pachet, uint8_t *data, uint8_t data_len);
void message_create(uint8_t type, message_t *msg, uint8_t *tx, uint8_t *data, uint8_t data_len);
uint8_t messages_is_this_rx(uint8_t *data, uint8_t data_len);

#endif //_MESSAGES_H