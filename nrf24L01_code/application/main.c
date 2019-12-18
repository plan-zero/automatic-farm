/*
 * test2.c
 *
 * Created: 12/6/2019 12:22:40 AM
 * Author : AR
 */ 

#include <avr/io.h>
#include "radio_fptr.h"
#include "nrf24Radio.h"

#define LED_PORT_DIR DDRB
#define LED_PORT_PIN PINB0

#define TURN_LED_ON		PORTB |= 1
#define TURN_LED_OFF	PORTB &= 0xFE
#define TOGGLE_LED		PORTB ^= 1

#define  F_CPU 8000000
#include <util/delay.h>
#include <avr/interrupt.h>
#include "E2P_Layout.h"
#include <avr/eeprom.h>

#define BOOT_KEY "Z4zC6i5FmM"
#define BOOT_KEY_SIZE 10

void check_bootloader_key(uint8_t *key, uint8_t key_len)
{
	if(key_len == BOOT_KEY_SIZE)
	{
		uint8_t boot_key_received = 1;
		for(uint8_t i = 0; i < BOOT_KEY_SIZE; i++)
		{
			if( key[i] != BOOT_KEY[i] )
			{
				boot_key_received = 0;
				break;
			}
		}
		if(boot_key_received)
		{
			cli();
			uint8_t dummy[6] = {0xAA,'B','B','C','D','E'};
			eeprom_update_block ((void*)&dummy[0], (void*)DOWNLOAD_FLAG_ADDRESS, DOWNLOAD_FLAG_LENGTH + PROGRAMMER_ADDR_LENGTH);
			_delay_ms(100);
			sei();
			//enter bootloader
			WDTCR=0x18;
			WDTCR=0x08;
			asm("wdr");
			while(1);
		}
	}
}

void rx_handler(uint8_t pipe, uint8_t * data, uint8_t payload_length) {
	
	check_bootloader_key(data, payload_length);
}

void tx_handler(radio_tx_status tx_status) {
	
}


int main(void)
{
    /* Replace with your application code */
	
	 LED_PORT_DIR |= 1 << LED_PORT_PIN;
	 TURN_LED_ON;
	radio_config cfg =
	{
		RADIO_ADDRESS_5BYTES,
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
		RADIO_APPLICATION
	};
	__nrfRadio_Init(cfg);
	
	uint8_t rx_addr[5] = {'A','B','C','D','E'};
	uint8_t tx_addr[5] = {'B','B','C','D','E'};
	pipe_config pipe_cfg0 =
	{
		RADIO_PIPE0,
		rx_addr,
		5,
		RADIO_PIPE_RX_ENABLED,
		RADIO_PIPE_AA_ENABLED,
		RADIO_PIPE_DYNAMIC_PYALOAD_ENABLED
	};

	__nrfRadio_PipeConfig(pipe_cfg0);
	__nrfRadio_SetRxCallback(rx_handler);
	__nrfRadio_SetTxCallback(tx_handler);
	__nrfRadio_PowerUp();
	__nrfRadio_ListeningMode();
	_delay_ms(1000);
	TURN_LED_OFF;
	sei();
	uint8_t msg[2] = {'A','F'};
    while (1) 
    {
		//
		//__nrfRadio_LoadMessages(msg,2);
		//__nrfRadio_Transmit(tx_addr,RADIO_WAIT_TX);
		__nrfRadio_Main();
		TOGGLE_LED;
		_delay_ms(200);
    }
}

