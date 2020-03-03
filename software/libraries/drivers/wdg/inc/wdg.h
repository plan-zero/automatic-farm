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

#ifndef _WDG_H
#define _WDG_H

#include "stdint.h"

typedef enum{
    wdg_rr_poweronreset,
    wdg_rr_externalreset,
    wdg_rr_brownoutreset,
    wdg_rr_watchdogreset,
    wdg_rr_jtagreset,
    wdg_rr_none,
}wdg_hw_reset_reason;

#define 	wdgto_15MS      0
#define 	wdgto_30MS      1
#define 	wdgto_60MS      2
#define 	wdgto_120MS     3
#define 	wdgto_250MS     4
#define 	wdgto_500MS     5
#define 	wdgto_1S        6
#define 	wdgto_2S        7
#define 	wdgto_4S        8
#define 	wdgto_8S        9

extern inline void wdg_init(uint8_t timeout);
extern inline void wdg_kick();
extern inline void wdg_disable();
uint8_t wdg_sw_reset_reason();
wdg_hw_reset_reason wdg_get_hw_reason();
void wdg_explicit_reset(uint8_t sw_reason);


#endif