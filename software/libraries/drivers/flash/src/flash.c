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


#include "flash.h"
#include "interrupt_hw.h"

// Bootloader flash
void boot_program_page (uint32_t page, uint8_t *buf)
{
	uint16_t i;
	uint8_t sreg;
	// Disable interrupts.
	sreg = SREG;
	INT_GLOBAL_DIS();
	eeprom_busy_wait ();
	boot_page_erase (page);
	boot_spm_busy_wait (); // Wait until the memory is erased.
	for (i=0; i<SPM_PAGESIZE; i+=2)
	{
		// Set up little-endian word.
		uint16_t w = *buf++;
		w += (*buf++) << 8;
		boot_page_fill (page + i, w);
	}
	boot_page_write (page); // Store buffer in flash page.
	boot_spm_busy_wait(); // Wait until the memory is written.
	// Reenable RWW-section again. We need this if we want to jump back
	// to the application after bootloading.
	boot_rww_enable ();
	// Re-enable interrupts (if they were ever enabled).
	SREG = sreg;
}

void boot_erase_pages(uint16_t start_page, uint16_t end_page)
{
    uint8_t sreg = SREG;
    INT_GLOBAL_DIS();
    for(uint16_t page = start_page + 1; end_page < 80; page++)
    {
        eeprom_busy_wait ();
        boot_page_erase (page * 128);
        boot_spm_busy_wait (); // Wait until the memory is erased.
        boot_rww_enable();
    }
    SREG = sreg;
    INT_GLOBAL_EN();
}