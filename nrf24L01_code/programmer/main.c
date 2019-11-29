/*
 * programmer.c
 *
 * Created: 11/28/2019 9:48:02 PM
 * Author : AR
 */ 

#include <avr/io.h>
#include <stdint.h>
#include "uart.h"
#include "nrf24Radio.h"
#include "radio_fptr.h"

#define F_CPU 8000000UL
#include <util/delay.h>

uint8_t uart_data[UART_RX_MAX];

#define UART_S0 0
#define UART_CMD_1	1
#define UART_CMD_2	2
#define UART_CMD_3	3
#define UART_END_CMD 4
#define UART_CMD_READ 5
#define UART_DATA_LEN_1 6
#define UART_DATA_LEN_2 10
#define UART_DATA_READ 7
#define UART_END 8
#define UART_END_2 9

#define MAX_CMD_LEN 33
uint8_t command_type;
uint8_t command_len;
uint8_t cmd[MAX_CMD_LEN];
uint8_t cmd_available = 0;

#define CMD_SET_TX_ADDR 'A'
#define CMD_SET_RX_ADDR	'B'
#define CMD_CONFIGURE_RADIO 'C'
#define CMD_SEND_TX	'D'

void rx_handler(uint8_t pipe, uint8_t * data, uint8_t payload_length) {
	uart_printString("<rx_pipe>:",0);
	uart_printRegister(pipe);
	uartNewLine();
	char msg[33] = {0};
	msg[payload_length] = '\0';
	for(int i = 0; i < payload_length; i++)
	{
		msg[i] = data[i];
	}
	uart_printString(msg, 1);
	
}

void tx_handler(radio_tx_status tx_status) {
	
}

void process_uart_data(uint8_t * data, uint8_t len) 
{
	static uint8_t uart_state = UART_S0;
	static uint8_t tmp_len = 0;
	//static uint8_t leng_num = 0;
	for(uint8_t idx = 0; idx < len; idx ++)
	{
		uart_sendByte(data[idx]);
		switch(uart_state)
		{
			case UART_S0:
				command_type = 0;
				command_len = 0;
				tmp_len = 0;
				if(data[idx] == '<')
				{
					uart_state = UART_CMD_1;
				}
				else
					uart_state = UART_S0;
			break;
			case UART_CMD_1:
				if(data[idx] == 'C')
				{
					uart_state = UART_CMD_2;
				}
				else
					uart_state = UART_S0;
			break;
			case UART_CMD_2:
				if(data[idx] == 'M')
				{
					uart_state = UART_CMD_3;
				}
				else
					uart_state = UART_S0;
			break;
			case UART_CMD_3:
				if(data[idx] == 'D')
				{
					uart_state = UART_END_CMD;
				}
				else
					uart_state = UART_S0;
			break;
			case UART_END_CMD:
				if(data[idx] == '>')
				{
					uart_state = UART_CMD_READ;
				}
				else
					uart_state = UART_S0;
			break;
			case UART_CMD_READ:
				command_type = data[idx];
				uart_state = UART_DATA_LEN_1;
			break;
			case UART_DATA_LEN_1:
				if(data[idx] <= '9' && data[idx] >= '0') {
					tmp_len = (data[idx] - '0') * 10;
					uart_state = UART_DATA_LEN_2;
				}
				else
					uart_state = UART_S0;
			break;
			case UART_DATA_LEN_2:
				if(data[idx] <= '9' && data[idx] >= '0') {
					tmp_len += (data[idx] - '0');
					uart_state = UART_DATA_READ;
					command_len = tmp_len;
				}
				else
					uart_state = UART_S0;
			break;
			case UART_DATA_READ:
				if((int)tmp_len != 0) {
					if( data[idx] == 13)
					{
						uart_state = UART_S0;
					}
					else
					{
						uart_state = UART_DATA_READ;
						cmd[command_len - tmp_len] = data[idx];
						tmp_len--;
					}
				}
				else if (data[idx] == 13) {
					uart_state = UART_S0;
					cmd_available = 1;
					
				}
				else {
					uart_state = UART_S0;
				}
			break;
			default:

			break;
		}
	}
}

int main(void)
{
	//initilize uart
	uart_init(UART_250000BAUD, UART_8MHZ, UART_PARITY_NONE);
	uart_printString("NRF24L01 programmer",1);
	
	//initilize the NRF 
	
    uint8_t uart_data_len;
	uint8_t tx_addr[5] = {0};
	uint8_t rx_addr[5] = {0};
	uint8_t init = 1;
	uint8_t uart_rx_err;
	
    while (init) 
    {
		uart_data_len = uart_rx_flush(uart_data, &uart_rx_err);
		if(UART_RX_ERR != uart_data_len)
			process_uart_data(uart_data, uart_data_len);
		else 
		{
			uart_printString("UART rx error", 0);
			uart_printRegister(uart_rx_err);
			uartNewLine();
		}
		
		if(cmd_available)
		{
			uart_printString("cmd executing...",1);
			switch(command_type) 
			{
				case CMD_SET_TX_ADDR:
					if(command_len == 5)
					{
						uart_printString("Set tx address to ",0);
						char tmp[6];
						for(uint8_t idx = 0; idx < command_len; idx ++){
							tx_addr[idx] = cmd[idx];
							tmp[idx] = cmd[idx];
						}
						tmp[5] = '\0';
						uart_printString(tmp,1);
					}
					else
						uart_printString("TX address must be 5 bytes long", 1);
				break;
				
				case CMD_SET_RX_ADDR:
					if(command_len == 5)
					{
						uart_printString("Set rx address to ",0);
						char tmp[6];
						for(uint8_t idx = 0; idx < command_len; idx ++){
							rx_addr[idx] = cmd[idx];
							tmp[idx] = cmd[idx];
						}
						tmp[5] = '\0';
						uart_printString(tmp,1);
					}
					else
						uart_printString("RX address must be 5 bytes long", 1);
				break;
				
				case CMD_CONFIGURE_RADIO:
					uart_printString("Configure radio...", 1);
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
					__nrfRadio_TransmitMode();
					uart_printString("Configuration done. Starting TX",1);
				break;
				case CMD_SEND_TX:
					if(command_len <= UART_RX_MAX)
					{
						uart_printString("Send TX payload:",0);
						uint8_t payload[33] = {0};
						for(uint8_t idx = 0; idx < command_len; idx++)
						{
							payload[idx] = cmd[idx];
						}
						payload[32] = '\0';
						uart_printString((char*)payload,1);
						__nrfRadio_LoadMessages(payload, command_len);
						uint8_t status = __nrfRadio_Transmit(tx_addr, RADIO_WAIT_TX);
						if( status == RADIO_TX_OK || status == RADIO_TX_OK_ACK_PYL)
						{
							uart_printString("<ACK>",1);
						}
						else
						{
							uart_printString("<NACK>",1);
						}
					}
					else
					{
						uart_printString("Payload exceed the maximum NRF24 size [32]", 1);
					}
				break;
				
				default:
					uart_printString("Command not available", 1);
				break;
			}
			cmd_available = 0;
		}
    }
}

