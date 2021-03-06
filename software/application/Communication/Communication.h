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

typedef enum{
    communication_inbox_full,
    communication_inbox_empty,
    communication_inbox_ok,
    communication_outbox_full,
    communication_outbox_empty,
    communication_outbox_ok,
}communication_status_t;

void rx_handler(uint8_t pipe, uint8_t * data, uint8_t payload_length);
void communication_execute_messages();
communication_status_t communication_outbox_add(message_t msg, uint8_t len, uint8_t delay);
void communication_send_messages();
void communication_init();