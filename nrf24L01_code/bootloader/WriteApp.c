/*
 * WriteApp.c
 *
 * Created: 11/28/2019 9:38:05 PM
 *  Author: Adi
 */ 
#include "WriteApp.h"
#include "radio_fptr.h"
#include "GenericLib.h"



#define BUFFER_LENGTH				((uint8_t) 32)
#define MSG_REC_TIMEOUT				((uint32_t) 100000)
#define MSG_RECEIVED				((uint8_t) 0xAA)


#define BOOTLOADER_INIT_NRF			((uint8_t) 0x00)
#define BOOTLOADER_FLASH_INIT		((uint8_t) 0x01)
#define BOOTLOADER_REC_DATA			((uint8_t) 0x02)

#define BOOTLOADER_FLASH_END		((uint8_t) 0xFE)
#define BOOTLOADER_FLASH_ERROR		((uint8_t) 0xFF)



char bootloader_state = BOOTLOADER_INIT_NRF;


typedef struct rxDataStruct
{
	uint8_t data[BUFFER_LENGTH];
	uint8_t length;
	uint8_t hasMessage;
}rxDataStruct;

rxDataStruct rxData;


void rx_handler(uint8_t pipe, uint8_t * data, uint8_t payload_length) {
	memcpy(rxData.data, data, payload_length);
	rxData.length = payload_length;	
	rxData.hasMessage = 1;
}
void tx_handler(radio_tx_status tx_status) {
	
}



void initNrf(uint8_t * rx_address)
{
	radio_config cfg = {
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
		RADIO_BOOTLOADER
	};
	__nrfRadio_Init(cfg);
	
	pipe_config pipe_cfg0 =
	{
		RADIO_PIPE0,
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
	__nrfRadio_ListeningMode();
}
void waitRx()
{
	uint32_t timeoutCounter;
	timeoutCounter = MSG_REC_TIMEOUT;
	rxData.hasMessage = 0;	
	do{
		timeoutCounter--;
		__nrfRadio_Main();
		_delay_us(10);
	}while (!rxData.hasMessage && (timeoutCounter > 0));
	
	if (!rxData.hasMessage || (timeoutCounter == 0))
		rxData.hasMessage = 0;		
}


void startFlash(uint8_t * rx_address)
{
	uint8_t ackResponse [BUFFER_LENGTH];	
	do 
	{
		switch(bootloader_state)
		{
			case BOOTLOADER_INIT_NRF:				
				rxData.length = 0xFF;
				rxData.hasMessage = 0;
				initNrf(rx_address);
				sei();
				bootloader_state = BOOTLOADER_FLASH_INIT;
			break;
			case BOOTLOADER_FLASH_INIT:
				ackResponse[0] = BOOTLOADER_FLASH_INIT;
				__nrfRadio_LoadAckPayload(3, (uint8_t*)ackResponse, 1);
				
				waitRx();
				
				if (!rxData.hasMessage)
				{
					bootloader_state = BOOTLOADER_FLASH_ERROR;
				}
				else
				{
					bootloader_state = BOOTLOADER_REC_DATA;
				}
			rxData.hasMessage = 0;
			break;
			case BOOTLOADER_REC_DATA:
				do{
					ackResponse[0] = BOOTLOADER_FLASH_INIT;
					__nrfRadio_LoadAckPayload(3, (uint8_t*)ackResponse, 1);
					
				}while(1);
				
			break;
			default:
				bootloader_state = BOOTLOADER_FLASH_ERROR;
		}		
				
	} while ((bootloader_state != BOOTLOADER_FLASH_END) && (bootloader_state != BOOTLOADER_FLASH_ERROR));
		
		
		
	if (bootloader_state == BOOTLOADER_FLASH_ERROR)
	{
		// KAPUT
	}
}
