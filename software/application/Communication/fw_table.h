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
#include "stdint.h"
#include "stdlib.h"
#include "string.h"

#define ADDRESS_LENGTH  5
#define NETWORK_MAX_DEPTH 5
#define CHILD_MAX   4

typedef struct addr_table{
    addr_table * next;
    uint8_t *address[CHILD_MAX];
}addr_table_t;

uint8_t this_depth = 0;
uint8_t this_state = 0;
uint8_t address_count;

addr_table_t network_addr_tables[NETWORK_MAX_DEPTH];

void addr_table_add(uint8_t *address)
{
    if(this_depth < NETWORK_MAX_DEPTH)
    {
        if(address_count < CHILD_MAX)
        {
            //allocate memory for address
            uint8_t * _address = (uint8_t*)malloc(sizeof(uint8_t) * ADDRESS_LENGTH);
            memcpy(_address, address, ADDRESS_LENGTH);
            network_addr_tables[this_depth].address[address_count] = _address;

        }
        
    }
}
