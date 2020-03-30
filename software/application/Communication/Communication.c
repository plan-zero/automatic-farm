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


void rx_handler(uint8_t pipe, uint8_t * data, uint8_t payload_length)
{
    if(payload_length)
    {
        if(data[0] == 'K') //check if this is a key message
        {
            ota_check_key(data, payload_length);
        }
        else if(data[0] == 'P') //this is a pipe config message
        {
            radio_link_inbox_cpy(&data[1], payload_length - 1);
        }
        else if(data[0] == 'O')
        {
            
        }
    }
}