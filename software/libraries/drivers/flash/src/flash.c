#include "flash.h"
#include "flash_layout.h"
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