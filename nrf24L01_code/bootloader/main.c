/*
 * bootloader_m16.c
 *
 * Created: 11/4/2019 7:34:04 PM
 * Author : AR
 */ 

#define F_CPU 4000000UL

#include <util/delay.h>
#include <avr/io.h>
#include <avr/boot.h>
#include <avr/eeprom.h> 
#include <avr/interrupt.h>
#include <stdint.h>

#include "radio_fptr.h"
#include "uart.h"


#define LED_PORT_DIR DDRB
#define LED_PORT_PIN PINB0

#define TURN_LED_ON		PORTB |= 1
#define TURN_LED_OFF	PORTB &= 0xFE
#define TOGGLE_LED		PORTB ^= 1

#define RADIO_RX_ADDRESS (const uint8_t*)0
#define RADIO_RX_LENGTH	 5


#define BOOTLOADER_IDLE			0
#define BOOTLOADER_DOWNLOAD		1
#define BOOTLOADER_CHECK_APP	2

#define BOOTLOADER_TIMEOUT 5 //seconds

void rx_handler(uint8_t pipe, uint8_t * data, uint8_t payload_length) {

}

void tx_handler(radio_tx_status tx_status) {
	
}

int main(void)
{
	uint8_t wait_time = BOOTLOADER_TIMEOUT;
	uint8_t bootloader_state = BOOTLOADER_IDLE;
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
	
	if( download == 0) {
		bootloader_state = BOOTLOADER_DOWNLOAD;
	}
	

	switch(bootloader_state){
		case BOOTLOADER_IDLE:
			
			
			while(wait_time--){
				_delay_ms(1000);
				TOGGLE_LED;
			}
			bootloader_state = BOOTLOADER_CHECK_APP;
		break;
		case BOOTLOADER_DOWNLOAD:
			_delay_ms(500);
			bootloader_state = BOOTLOADER_DOWNLOAD;
			
			
			radio_config cfg = { RADIO_ADDRESS_5BYTES,
								RADIO_RETRANSMIT_WAIT_3000US,
								RADIO_RETRANSMIT_15,
								CHANNEL_112,
								RADIO_250KBIT,
								RADIO_CRC2_ENABLED,
								RADIO_COUNT_WAVE_DISABLED,
								RADIO_HIGHEST_0DBM,
								RADIO_DYNAMIC_PAYLOAD_ENABLED,
								RADIO_ACK_PAYLOAD_ENABLED,
								RADIO_DYNAMIC_ACK_DISABLED,
								RADIO_BOOTLOADER };
			__nrfRadio_Init(cfg);
			
			pipe_config pipe_cfg0 = {	RADIO_PIPE0,
								rx_address,
								5,
								RADIO_PIPE_RX_ENABLED,
								RADIO_PIPE_AA_ENABLED,
								RADIO_PIPE_DYNAMIC_PYALOAD_ENABLED
			};
			__nrfRadio_PipeConfig(pipe_cfg0);
			
			__nrfRadio_SetRxCallback(rx_handler);
			__nrfRadio_SetTxCallback(tx_handler);
			__nrfRadio_PowerUp();
			__nrfRadio_TransmitMode();
			uint8_t payload[5] = {'A','B','C','D','E'};
			uint32_t transmit_count = 0;
			
			sei();
			
			while (1)
			{
				
				transmit_count++;
				
				if(transmit_count == 100000) {
					transmit_count = 0;
					payload[0]++;
					if (payload[0] == 'E')
						payload[0] = 'A';
					__nrfRadio_LoadMessages(payload, 5);
					if( RADIO_TX_OK == __nrfRadio_Transmit(tx_address, RADIO_WAIT_TX) )
						TURN_LED_ON;
					else
						TURN_LED_OFF;
					TOGGLE_LED;
				}
				
				__nrfRadio_Main();
			}
		break;
	}
	
	
	while(bootloader_state == BOOTLOADER_CHECK_APP){
		TURN_LED_OFF;
	}
	//go to the application as soon as the code has downloaded
	//asm("JMP 0");
}

