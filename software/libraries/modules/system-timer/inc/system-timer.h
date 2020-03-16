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

#ifndef _SYSTEM_TIMER_H
#define _SYSTEM_TIMER_H

#include "stdint.h"

typedef struct{
    uint32_t timer_ticks;
    uint16_t timer_period;
}system_timer_timestamp;

typedef struct{
    uint32_t mili;
    uint32_t min;
    uint32_t hour;
    uint32_t sec;
}system_timer_info;

void system_timer_init();
void system_timer_start();
//uint32_t system_timer_getms(uint32_t ticks);
uint32_t system_timer_getus(system_timer_timestamp _start, system_timer_timestamp _end);
uint32_t system_timer_getms(system_timer_timestamp _start, system_timer_timestamp _end);
system_timer_timestamp system_timer_get_timestamp();
system_timer_info system_timer_get_info(system_timer_timestamp timestamp);


#endif