/*
 * bootloader_m16.c
 *
 * Created: 11/4/2019 7:34:04 PM
 * Author : AR
 */ 

#include "radio_fptr.h"
#include "uart.h"
#include "WriteApp.h"
#include "GenericLib.h"

#define LED_PORT_DIR DDRB
#define LED_PORT_PIN PINB0

#define TURN_LED_ON		PORTB |= 1
#define TURN_LED_OFF	PORTB &= 0xFE
#define TOGGLE_LED		PORTB ^= 1

#define RADIO_RX_ADDRESS (const uint8_t*)0
#define RADIO_RX_LENGTH	 5


int main(void)
{
	// Wait after power on in order to stabilize the tension
	_delay_ms(50);
	LED_PORT_DIR |= 1 << LED_PORT_PIN;

	uint8_t rx_address[RADIO_RX_LENGTH] = {0};
	//uint32_t eep_checksum = 0;
	//uint32_t flash_checksum = 0;
	//uint16_t nrf24_firmware_version = 0;
	//uint16_t application_version = 0;
	//uint16_t eep_version = 0;
	//uint16_t reserved = 0;
	uint8_t download = 0;
	uint8_t tx_address[RADIO_RX_LENGTH] = {0};
	
	//reading eeprom parameters

	eeprom_read_block ((void*)&rx_address, (void*)0, RADIO_RX_LENGTH);
	//eep_checksum = eeprom_read_dword((uint32_t*)5);
	//flash_checksum = eeprom_read_dword((uint32_t*)9);
	//nrf24_firmware_version = eeprom_read_word((uint16_t*)13);
	//application_version = eeprom_read_word((uint16_t*)15);
	//eep_version = eeprom_read_word((uint16_t*)17);
	//reserved = eeprom_read_word((uint16_t*)19);
	download = eeprom_read_byte((uint8_t*)21);
	eeprom_read_block((void*)&tx_address, (void*)22, RADIO_RX_LENGTH);
	
	if( download == 0xAA) {
		startFlash(rx_address);
	}
	
	/*while(bootloader_state == BOOTLOADER_CHECK_APP)
	{
		TURN_LED_OFF;
	}*/
	//go to the application as soon as the code has downloaded
	//asm("JMP 0");
}

