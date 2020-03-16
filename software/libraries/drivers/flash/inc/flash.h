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


#ifndef _FLASH_H
#define _FLASH_H

#include "stdint.h"
#include "flash_layout.h"
#include <avr/boot.h>
#include <avr/pgmspace.h>

void boot_program_page (uint32_t page, uint8_t *buf);
void boot_erase_pages(uint16_t start_page, uint16_t end_page);

#endif