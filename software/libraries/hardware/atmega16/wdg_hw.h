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

#include <avr/io.h>

#include <avr/wdt.h>
#include <avr/io.h>

#define WDG_STATUS_GET      MCUCSR
#define WDG_STATUS_RESET    MCUCSR &= 0xE0 

#define IS_HW_POR()           (MCUCSR >> PORF) & 1
#define IS_HW_EXT()           (MCUCSR >> EXTRF) & 1
#define IS_HW_BOR()           (MCUCSR >> BORF) & 1
#define IS_HW_WDR()           (MCUCSR >> WDRF) & 1
#define IS_HW_JTR()           (MCUCSR >> JTRF) & 1
