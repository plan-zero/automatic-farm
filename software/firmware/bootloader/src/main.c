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


/*
 * bootloader_m16.c
 *
 * Created: 11/4/2019 7:34:04 PM
 * Author : AR
 */ 
#include "WriteApp.h"
#include "util/delay.h"
#include "e2p.h"

#define LED_PORT_DIR DDRB
#define LED_PORT_PIN PINB0

#define TURN_LED_ON		PORTB |= 1
#define TURN_LED_OFF	PORTB &= 0xFE
#define TOGGLE_LED		PORTB ^= 1

int main(void)
{
	// Wait after power on in order to stabilize the tension
	_delay_ms(50);


	uint8_t rx_address[RADIO_RX_LENGTH] = {0};
	/*uint32_t eep_checksum = 0;
	uint32_t flash_checksum = 0;
	uint16_t nrf24_firmware_version = 0;
	uint16_t application_version = 0;
	uint16_t eep_version = 0;
	//uint16_t reserved = 0;*/
	uint8_t download = 0;
	uint8_t tx_address[RADIO_RX_LENGTH] = {0};
	
	//reading eeprom parameters
	
	e2p_read_rxaddress(rx_address);

	e2p_read_downloadflag(download);
	e2p_read_txaddress(tx_address);

		
	if( download == 0xAA) {
		startFlash(rx_address);
	}
	
	/*while(bootloader_state == BOOTLOADER_CHECK_APP)
	{
		TURN_LED_OFF;
	}*/
	//go to the application as soon as the code has downloaded
	asm("JMP 0");
}

