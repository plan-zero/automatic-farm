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
#define PAGE_SIZE					((uint8_t) SPM_PAGESIZE)
#define MSG_REC_TIMEOUT				((uint32_t) 100000000)
#define MSG_RECEIVED				((uint8_t) 0xAA)
#define MSG_NO_DATA					((uint8_t) 0x00)





// Flashing state machine
#define BOOTLOADER_INIT_NRF			((uint8_t) 'A')
#define BOOTLOADER_FLASH_INIT		((uint8_t) 'S')
#define BOOTLOADER_REC_DATA			((uint8_t) 'D')
#define BOOTLOADER_FLASH_DATA		((uint8_t) 'F')
#define BOOTLOADER_FLASH_PAGE_DONE	((uint8_t) 'G')
#define BOOTLOADER_CHECK_CKS		((uint8_t) 'H')

#define BOOTLOADER_FLASH_END		((uint8_t) 'K')
#define BOOTLOADER_FLASH_ERROR		((uint8_t) 'L')


// Flashing commands
#define COMM_GET_STATE				((uint8_t) 'q')
#define COMM_START_FLASH_PROCEDURE	((uint8_t) 'w')
#define COMM_SEND_CHUNCK			((uint8_t) 'e')
#define COMM_FLASH_PAGE				((uint8_t) 'r')
#define COMM_NEXT_PAGE				((uint8_t) 't')
#define COMM_CHECK_CKS				((uint8_t) 'y')
#define COMM_FINISH_FLASH			((uint8_t) 'u')



#define COMM_ABORT					((uint8_t) 'o')
#define COMM_NO_COMMAND				((uint8_t) 'p')

char bootloader_state = BOOTLOADER_INIT_NRF;


typedef struct rxDataStruct
{
	uint8_t data[BUFFER_LENGTH];
	uint8_t command;
	uint8_t length;
	uint8_t hasMessage;
}rxDataStruct;

rxDataStruct rxData;
uint8_t flashPipe = 0;


void rx_handler(uint8_t pipe, uint8_t * data, uint8_t payload_length) {
	rxData.command = data[0];
	memcpy(rxData.data, data + 1, payload_length - 1);	
	rxData.length = payload_length - 1;	
	rxData.hasMessage = MSG_RECEIVED;
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
	rxData.hasMessage = MSG_NO_DATA;	
	rxData.command = COMM_NO_COMMAND;
	do{
		timeoutCounter--;
		__nrfRadio_Main();
		_delay_us(10);
	}while (!rxData.hasMessage && (timeoutCounter > 0));
	
	if (!rxData.hasMessage || (timeoutCounter == 0))
		rxData.hasMessage = MSG_NO_DATA;		
}


// Bootloader flash
void boot_program_page (uint32_t page, uint8_t *buf)
{
	uint16_t i;
	uint8_t sreg;
	// Disable interrupts.
	sreg = SREG;
	cli();
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


void startFlash(uint8_t * rx_address)
{
	uint8_t ackResponse [BUFFER_LENGTH];			
	uint8_t pageData [PAGE_SIZE];
	uint8_t bytesReceived = 0;
	uint16_t currentPage = 0;
	uint16_t recvCKS = 0;

	do 
	{
		switch(bootloader_state)
		{
			case BOOTLOADER_INIT_NRF:				
			{
				rxData.length = 0x00;
				rxData.hasMessage = MSG_NO_DATA;
				initNrf(rx_address);
				sei();
				bootloader_state = BOOTLOADER_FLASH_INIT;
				break;
			}
			case BOOTLOADER_FLASH_INIT:
			{		
				waitRx();
				
				if (rxData.hasMessage)
				{
					if (rxData.command == COMM_START_FLASH_PROCEDURE)
					{
						bootloader_state = BOOTLOADER_REC_DATA;
					}
					else if (rxData.command == COMM_GET_STATE)
					{
						memset(ackResponse, 0x00, BUFFER_LENGTH);
						ackResponse[0] = BOOTLOADER_FLASH_INIT;
						__nrfRadio_LoadAckPayload(flashPipe, (uint8_t*)ackResponse, BUFFER_LENGTH);
						
						waitRx();
						// We remain in this state
						bootloader_state = BOOTLOADER_FLASH_INIT;
					}
					else
						bootloader_state = BOOTLOADER_FLASH_ERROR;
				}
				else
				{
					bootloader_state = BOOTLOADER_FLASH_ERROR;
				}
				rxData.hasMessage = MSG_NO_DATA;
				break;
			}
			case BOOTLOADER_REC_DATA:
			{												
				do{	
					waitRx();
					if (rxData.hasMessage)
					{
						if (rxData.command == COMM_SEND_CHUNCK)
						{							
							if ((bytesReceived + rxData.length) <= PAGE_SIZE)
							{	
								memcpy((uint8_t*) &pageData[bytesReceived],	rxData.data, rxData.length);
								bytesReceived += rxData.length;
								rxData.hasMessage = MSG_NO_DATA;
							}
							else
							{								
								bootloader_state = BOOTLOADER_FLASH_ERROR;
							}
						}
						else if (rxData.command == COMM_GET_STATE)
						{
							memset(ackResponse, 0x00, BUFFER_LENGTH);
							ackResponse[0] = BOOTLOADER_REC_DATA;
							ackResponse[1] = '0' + bytesReceived / 16;
							__nrfRadio_LoadAckPayload(flashPipe, (uint8_t*)ackResponse, 2);
							
							waitRx();
							// We remain in this state
							bootloader_state = BOOTLOADER_REC_DATA;
						}
						else
							bootloader_state = BOOTLOADER_FLASH_ERROR;
					}					
					else
					{
						bootloader_state = BOOTLOADER_FLASH_ERROR;
					}
				}while((bytesReceived < PAGE_SIZE) && (bootloader_state != BOOTLOADER_FLASH_ERROR));	
					
				if (bootloader_state != BOOTLOADER_FLASH_ERROR)
					bootloader_state = BOOTLOADER_FLASH_DATA;
				break;
			}
			case BOOTLOADER_FLASH_DATA:
			{
				waitRx();
				if (rxData.hasMessage)
				{
					if (rxData.command == COMM_FLASH_PAGE)
					{
						memset(ackResponse, 0x00, BUFFER_LENGTH);
						ackResponse[0] = BOOTLOADER_FLASH_DATA;
						ackResponse[1] = '0' + currentPage;
						__nrfRadio_LoadAckPayload(flashPipe, (uint8_t*)ackResponse, 2);
						
						cli();
						boot_program_page(currentPage, (uint8_t*) &pageData[0]);
						sei();
						
						bootloader_state = BOOTLOADER_FLASH_PAGE_DONE;
					}
					else if (rxData.command == COMM_GET_STATE)
					{
						memset(ackResponse, 0x00, BUFFER_LENGTH);
						ackResponse[0] = BOOTLOADER_FLASH_DATA;
						ackResponse[1] = '0' + currentPage;
						__nrfRadio_LoadAckPayload(flashPipe, (uint8_t*)ackResponse, 2);
						waitRx();
						// We remain in this state
						bootloader_state = BOOTLOADER_FLASH_DATA;
					}
					else
						bootloader_state = BOOTLOADER_FLASH_ERROR;
				}
				else
				{
					bootloader_state = BOOTLOADER_FLASH_ERROR;
				}
				
				break;
			}
			case BOOTLOADER_FLASH_PAGE_DONE:
			{	
				waitRx();				
				
				if (rxData.hasMessage)
				{
					if (rxData.command == COMM_NEXT_PAGE)
					{
						currentPage++;
						memset(pageData, 0x00, PAGE_SIZE);
						bootloader_state = BOOTLOADER_REC_DATA;
					}
					else if (rxData.command == COMM_CHECK_CKS)
					{
						recvCKS = (rxData.data[0] << 8) | (rxData.data[1]);
						bootloader_state = BOOTLOADER_CHECK_CKS;
					}
					else if (rxData.command == COMM_GET_STATE)
					{
						memset(ackResponse, 0x00, BUFFER_LENGTH);
						ackResponse[0] = BOOTLOADER_FLASH_PAGE_DONE;
						ackResponse[1] = '0' + currentPage;
						__nrfRadio_LoadAckPayload(flashPipe, (uint8_t*)ackResponse, 2);
						waitRx();
						// We remain in this state
						bootloader_state = BOOTLOADER_FLASH_PAGE_DONE;
					}
					else
					bootloader_state = BOOTLOADER_FLASH_ERROR;
				}
				else
				{
					bootloader_state = BOOTLOADER_FLASH_ERROR;
				}				
				break;
			}
			case BOOTLOADER_CHECK_CKS:
			{
				bootloader_state = BOOTLOADER_FLASH_END;
				uint8_t dummy = 0;
				cli();
				eeprom_write_block ((void*)&dummy, (void*)DOWNLOAD_FLAG_ADDRESS, DOWNLOAD_FLAG_LENGTH);
				sei();
				/*
				memset(ackResponse, 0x00, BUFFER_LENGTH);
				ackResponse[0] = BOOTLOADER_CHECK_CKS;
				ackResponse[1] = 0xAA;
				__nrfRadio_LoadAckPayload(flashPipe, (uint8_t*)ackResponse, 2);
				waitRx();
						
				if (rxData.hasMessage)
				{
					if (rxData.command == COMM_FINISH_FLASH)
					{
						uint8_t dummy = 0;
						eeprom_write_block ((void*)&dummy, (void*)DOWNLOAD_FLAG_ADDRESS, DOWNLOAD_FLAG_LENGTH);
					}					
					else if (rxData.command == COMM_GET_STATE)
					{
						memset(ackResponse, 0x00, BUFFER_LENGTH);
						ackResponse[0] = BOOTLOADER_CHECK_CKS;
						ackResponse[1] = 0xAA;
						__nrfRadio_LoadAckPayload(flashPipe, (uint8_t*)ackResponse, 2);
						waitRx();
						// We remain in this state
						bootloader_state = BOOTLOADER_CHECK_CKS;
					}
					else
						bootloader_state = BOOTLOADER_FLASH_ERROR;
				}
				else
				{
					bootloader_state = BOOTLOADER_FLASH_ERROR;
				}		
				*/
				break;
			}
			default:
				bootloader_state = BOOTLOADER_FLASH_ERROR;
		}		
				
	} while ((bootloader_state != BOOTLOADER_FLASH_END) && (bootloader_state != BOOTLOADER_FLASH_ERROR));
		
		
	
	if (bootloader_state == BOOTLOADER_FLASH_ERROR)
	{
		do{
			
			memset(ackResponse, 0x00, BUFFER_LENGTH);
			ackResponse[0] = BOOTLOADER_FLASH_ERROR;
			ackResponse[1] = 0xAA;
			__nrfRadio_LoadAckPayload(flashPipe, (uint8_t*)ackResponse, 2);			
			waitRx();
		}while (1);
		// KAPUT
	}
}
