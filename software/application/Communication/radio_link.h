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
#include "messages.h"

uint8_t radio_link_discovery();
void radio_link_init();
void radio_link_task();
uint8_t radio_link_configure(uint8_t *address_tx, uint8_t *address_rx, uint8_t _address_length);
uint8_t radio_link_execute(message_t msg);
uint8_t * radio_link_get_root_tx();