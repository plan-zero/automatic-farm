/*
 * WriteApp.c
 *
 * Created: 11/28/2019 9:38:05 PM
 *  Author: Adi
 */ 
#include "WriteApp.h"
#include "radio_fptr.h"
#include "GenericLib.h"
#include <util/crc16.h>
#include <avr/pgmspace.h>


#define BUFFER_LENGTH				((uint8_t) 32)
#define RESPONSE_SIZE				((uint8_t) 2)
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
#define BOOTLOADER_WAIT_FLASH_END	((uint8_t) 'J')

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



// COMM errors
#define COMM_ERR_TIMEOUT	 		((uint8_t) '1')
#define COMM_ERR_WRONG_COMMAND 		((uint8_t) '2')
#define COMM_ERR_WRONG_PAGE_LENGTH	((uint8_t) '3')
#define COMM_ERR_WRONG_CRC	 		((uint8_t) '4')









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

void wdgReset(void)
{
    WDTCR=0x18;
    WDTCR=0x08;
    asm("wdr");
    while(1);
}

void startFlash(uint8_t * rx_address)
{
	uint8_t ackResponse [RESPONSE_SIZE];
	uint8_t pageData [PAGE_SIZE];
	uint8_t bytesReceived = 0;
	uint16_t currentPage = 0;
	uint16_t recvCKS = 0;
	uint32_t timeoutCounter;

	timeoutCounter = MSG_REC_TIMEOUT;
	
	rxData.length = 0x00;
	rxData.hasMessage = MSG_NO_DATA;
	initNrf(rx_address);
	sei();
	bootloader_state = BOOTLOADER_FLASH_INIT;

	do 
	{
		__nrfRadio_Main();
		
		if (rxData.hasMessage)
		{
			// Clear response
			memset(ackResponse, 0x00, RESPONSE_SIZE);
			ackResponse[1] = '0';
			if ((rxData.command == COMM_NO_COMMAND) || (rxData.command == COMM_GET_STATE))
			{
				// I love goats
			}
			else
			{
				switch(bootloader_state)
				{
					case BOOTLOADER_FLASH_INIT:
					{
						// TODO: wait the programmer to confirm  PROGRAMMER_ADDR_ADDRESS

						if (rxData.command == COMM_START_FLASH_PROCEDURE)
							bootloader_state = BOOTLOADER_REC_DATA;
						else
							bootloader_state = BOOTLOADER_FLASH_ERROR;
						break;
					}
					case BOOTLOADER_REC_DATA:
					{
						if (rxData.command == COMM_SEND_CHUNCK)
						{
							memcpy((uint8_t*) &pageData[bytesReceived],	rxData.data, rxData.length);
							bytesReceived += rxData.length;
							ackResponse[1] = '0' + bytesReceived / 16;
							if (bytesReceived == PAGE_SIZE)
							{
								bootloader_state = BOOTLOADER_FLASH_DATA;
							}
							else if (bytesReceived > PAGE_SIZE)
							{
								bootloader_state = BOOTLOADER_FLASH_ERROR;
								ackResponse[1] = '1';
							}
						}
						else
						{
							bootloader_state = BOOTLOADER_FLASH_ERROR;
							ackResponse[1] = '2';
						}
						break;
					}
					case BOOTLOADER_FLASH_DATA:
					{
						if (rxData.command == COMM_FLASH_PAGE)
						{
							ackResponse[1] = '0' + currentPage;
							cli();
							boot_program_page(currentPage*128, (uint8_t*) &pageData[0]);
							sei();

							bootloader_state = BOOTLOADER_FLASH_PAGE_DONE;
						}
						else
						{
							bootloader_state = BOOTLOADER_FLASH_ERROR;
							ackResponse[1] = '3';
						}
						break;
					}
					case BOOTLOADER_FLASH_PAGE_DONE:
					{
						if (rxData.command == COMM_NEXT_PAGE)
						{
							currentPage++;
							bytesReceived = 0;
							memset(pageData, 0x00, PAGE_SIZE);
							bootloader_state = BOOTLOADER_REC_DATA;
						}
						else if (rxData.command == COMM_CHECK_CKS)
						{
							recvCKS = (rxData.data[0] << 8) | (rxData.data[1]);
							bootloader_state = BOOTLOADER_CHECK_CKS;
						}
						else
						{
							bootloader_state = BOOTLOADER_FLASH_ERROR;
							ackResponse[1] = '4';
						}

						break;
					}
					case BOOTLOADER_CHECK_CKS:
					{
						if (rxData.command == COMM_CHECK_CKS)
						{
							uint8_t flash_byte = 0;
							uint16_t crc = 0;
							//read application byte by byte
							for(uint32_t addr = APP_ADDR_START; addr < APP_ADDR_END; addr++)
							{
								flash_byte = pgm_read_byte(addr);
								crc = _crc16_update(crc, flash_byte);
							}

							if (recvCKS == crc)
							{
								bootloader_state = BOOTLOADER_WAIT_FLASH_END;
							}
							else
							{
								bootloader_state = BOOTLOADER_FLASH_ERROR;
								ackResponse[1] = '5';
							}
						}
						else
						{
							bootloader_state = BOOTLOADER_FLASH_ERROR;
							ackResponse[1] = '6';
						}

						break;
					}
					case BOOTLOADER_WAIT_FLASH_END:
					{
						if (rxData.command == COMM_FINISH_FLASH)
						{
							uint8_t dummy[5] = {0};
							cli();
							eeprom_write_block ((void*)&dummy[0], (void*)DOWNLOAD_FLAG_ADDRESS, DOWNLOAD_FLAG_LENGTH);
							eeprom_write_block ((void*)&dummy, (void*)PROGRAMMER_ADDR_ADDRESS, PROGRAMMER_ADDR_LENGTH);
							sei();

							//  WDG reset
							wdgReset();
						}
						else
						{
							bootloader_state = BOOTLOADER_FLASH_ERROR;
							ackResponse[1] = '7';
						}
						break;
					}
					default:
					{
						bootloader_state = BOOTLOADER_FLASH_ERROR;
						ackResponse[1] = '8';
					}
				}
			}

			// Set state
			ackResponse[0] = bootloader_state;
			__nrfRadio_LoadAckPayload(flashPipe, (uint8_t*)ackResponse, RESPONSE_SIZE);

			// Reset counter
			timeoutCounter = MSG_REC_TIMEOUT;
			// Clear data
			rxData.hasMessage = MSG_NO_DATA;
			rxData.command = COMM_NO_COMMAND;
		}
		else if (timeoutCounter == 0)
		{			
			bootloader_state = BOOTLOADER_FLASH_ERROR;
			ackResponse[1] = '9';
		}
		else
		{
			// Wait message
			_delay_us(10);
			timeoutCounter--;
		}
	} while ((bootloader_state != BOOTLOADER_FLASH_END) && (bootloader_state != BOOTLOADER_FLASH_ERROR));
		
	
	if (bootloader_state == BOOTLOADER_FLASH_ERROR)
	{
		// 30s counter
		uint16_t count = 30000;
		ackResponse[0] = BOOTLOADER_FLASH_ERROR;
		// Do not overwrite ackResponse[1]
		do{
			__nrfRadio_LoadAckPayload(flashPipe, (uint8_t*)ackResponse,RESPONSE_SIZE);
			// Clear data
			rxData.hasMessage = MSG_NO_DATA;
			rxData.command = COMM_NO_COMMAND;

			__nrfRadio_Main();
			if (rxData.hasMessage)
			{
				if ((rxData.command == COMM_NO_COMMAND) || (rxData.command == COMM_GET_STATE))
				{
					// let him have the error (it is already loaded)
				}
				else if (rxData.command == COMM_ABORT)
				{
					wdgReset();
				}
			}
			_delay_ms(1);
			count--;
		}while (count > 1);

		// Enough time to wait. Reset that motherfucker!
		wdgReset();
	}

	// Asta la vista baby
	wdgReset();
}
