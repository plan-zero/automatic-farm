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

#include "stdint.h"
#include "string.h"
#include "stdlib.h"
#define MAX_CHILD_NETWORK 15
#define NODE_ADDR_LENGTH 5

#define TEST_X86

typedef struct{
    uint8_t * address;
    uint8_t fw_node;
    uint8_t node_state;
}fw_table_entry_t;

fw_table_entry_t fw_table[MAX_CHILD_NETWORK];
uint8_t this_node_id = 0;

static inline void _add_node(uint8_t node_id, uint8_t * node_address)
{
        //add node
        fw_table[node_id].address = malloc(sizeof(uint8_t) * NODE_ADDR_LENGTH);
        if(fw_table[node_id].address)
            memcpy(fw_table[node_id].address, node_address, NODE_ADDR_LENGTH);
        fw_table[node_id].node_state = 1;
}

uint8_t fw_table_set_id(uint8_t node_id, uint8_t * node_address)
{
    if(fw_table[node_id].node_state == 0 && node_id < MAX_CHILD_NETWORK)
    {
        this_node_id = node_id;
        fw_table[node_id].fw_node = node_id;
        _add_node(node_id, node_address);
    }
}

uint8_t fw_table_add_child(uint8_t node_id, uint8_t * node_address, uint8_t fw_node_id)
{
    if(fw_table[node_id].node_state == 0 && node_id < MAX_CHILD_NETWORK)
    {
        fw_table[node_id].fw_node = fw_node_id;
        _add_node(node_id, node_address);
        return 0;
    }
    return 1;
}

fw_table_entry_t * fw_table_get_child_list(uint8_t * child_count)
{
    child_count = MAX_CHILD_NETWORK;
    return fw_table;
}

void fw_table_update(fw_table_entry_t * new_list, uint8_t *child_count)
{
    if(child_count <= MAX_CHILD_NETWORK)
    {
        
    }
}


#ifdef TEST_X86
#include "stdio.h"
void print_table(fw_table_entry_t *table)
{
    printf("==========NODE:%d ==========\n", this_node_id);
    for(uint8_t idx = 0; idx < MAX_CHILD_NETWORK; idx++)
    {
        printf("DEST [%d] = FW [%d] ", 
                idx,
                table[idx].fw_node);
        if(table[idx].address)
            printf("\t address: %x:%x:%x:%x:%x",
                    table[idx].address[0],
                    table[idx].address[1],
                    table[idx].address[2],
                    table[idx].address[3],
                    table[idx].address[4]
            );
        printf("\n");
    }
}

void main()
{
    printf("Testing on X86 \n");
    // set this node ID
    fw_table_set_id(1, "ABCDE");
    
    // add few direct childrens
    fw_table_add_child(2, "AABCD", 1);
    fw_table_add_child(3, "TTABC", 1);

    print_table(fw_table);
}

#endif
