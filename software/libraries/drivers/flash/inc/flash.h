#ifndef _FLASH_H
#define _FLASH_H

#include "stdint.h"
#include <avr/boot.h>
#include <avr/pgmspace.h>

void boot_program_page (uint32_t page, uint8_t *buf);
void boot_erase_pages(uint16_t start_page, uint16_t end_page);

#endif