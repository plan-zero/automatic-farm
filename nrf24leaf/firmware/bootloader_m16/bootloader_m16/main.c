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
#include "radio.h"

#define LED_PORT_DIR DDRB
#define LED_PORT_PIN PINB0

#define TURN_LED_ON		PORTB |= 1
#define TURN_LED_OFF	PORTB &= 0xFE;

#define RADIO_RX_ADDRESS (const uint8_t*)0
#define RADIO_RX_LENGTH	 5

volatile int rx_radio;
void radio_rxhandler(uint8_t pipenumber){
	rx_radio = 1;

}
radiopacket_t buffer;
uint8_t EEMEM _rx_address[RADIO_RX_LENGTH];

int main(void)
{
    LED_PORT_DIR |= 1 << LED_PORT_PIN;
	Radio_Init();
	
	uint8_t rx_address[RADIO_RX_LENGTH];
	eeprom_read_block ((void*)&rx_address, (void*)&_rx_address, RADIO_RX_LENGTH);
	
	Radio_Configure_Rx(RADIO_PIPE_1, rx_address, ENABLE);
	
	
	buffer.payload.message.address[0] = 0xE0;
	buffer.payload.message.address[1] = 0x70;
	buffer.payload.message.address[2] = 0x35;
	buffer.payload.message.address[3] = 0x01;
	buffer.payload.message.address[4] = 0xA1;
	
	Radio_Configure(RADIO_2MBPS, RADIO_HIGHEST_POWER);
	

	//for(uint8_t  offset = 0; offset < RADIO_RX_LENGTH; offset++)
	//	*(buffer.payload.message.address + offset) = *(rx_address + offset);
	//TURN_LED_ON;
	/*if(rx_address[0] == 0xE0 &&
		rx_address[1] == 0x70 &&
		rx_address[2] == 0x35 &&
		rx_address[3] == 0x01 &&
		rx_address[4] == 0xA1
	) {
		TURN_LED_ON;
	}*/
	
	//TURN_LED_ON;

	sei();
	
	
    while (1) 
    {
		if(rx_radio){
			
			Radio_Receive(&buffer);
			Radio_Flush();
			rx_radio = 0;
			
			if( buffer.payload.message.messagecontent[0] == 0xAA )
			{
				TURN_LED_ON;
			}
		}
		
    }
	
	//go to the application as soon as the code has downloaded
	//asm("JMP 0");
}

