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

#define MESSAGE_MAX_LENGTH 16

#define START_BYTE_BROADCAST    (uint8_t)('B')
#define START_BYTE_PAIRING      (uint8_t)('P')
#define START_BYTE_DATA         (uint8_t)('D')
#define START_BYTE_PING         (uint8_t)('U')
#define START_BYTE_BOOTKEY      (uint8_t)('K')
#define START_BYTE_ACK          (uint8_t)('A')


typedef struct{
    uint8_t type; // first byte is the message type
    uint8_t tx_address[RADIO_MAX_ADDRESS];
    uint8_t rx_address[RADIO_MAX_ADDRESS];
    uint16_t timestamp;
    uint8_t TTL;
    uint8_t data[MESSAGE_MAX_LENGTH];
    uint16_t ID;
}message_t;

#define MSG_STRUCT_SIZE 30//(uint8_t)(sizeof(message_t) / sizeof(uint8_t))

#endif //_MESSAGES_H