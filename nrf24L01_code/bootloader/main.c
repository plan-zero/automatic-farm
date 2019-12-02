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

int main(void)
{
	// Wait after power on in order to stabilize the tension
	_delay_ms(50);
	LED_PORT_DIR |= 1 << LED_PORT_PIN;

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
	
	eeprom_read_block ((void*)&rx_address,				(void*)RADIO_RX_ADDRESS,			RADIO_RX_LENGTH);
	/*eeprom_read_block ((void*)&eep_checksum,			(void*)E2P_CKS_ADDRESS,				E2P_CKS_LENGTH);
	eeprom_read_block ((void*)&flash_checksum,			(void*)FLASH_CKS_ADDRESS,			FLASH_CKS_LENGTH);
	eeprom_read_block ((void*)&nrf24_firmware_version,	(void*)NRF_VERS_ADDRESS,			NRF_VERS_LENGTH);
	eeprom_read_block ((void*)&application_version,		(void*)APP_VERS_ADDRESS,			APP_VERS_LENGTH);
	eeprom_read_block ((void*)&eep_version,				(void*)E2P_VERS_ADDRESS,			E2P_VERS_LENGTH);*/
	eeprom_read_block ((void*)&download,				(void*)DOWNLOAD_FLAG_ADDRESS,		DOWNLOAD_FLAG_LENGTH);
	eeprom_read_block ((void*)&tx_address,				(void*)PROGRAMMER_ADDR_ADDRESS,		PROGRAMMER_ADDR_LENGTH);

		
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

