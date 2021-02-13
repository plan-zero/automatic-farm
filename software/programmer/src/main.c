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
 * programmer.c
 *
 * Created: 11/28/2019 9:48:02 PM
 * Author : AR
 */ 

#include <avr/io.h>
#include <stdint.h>
#include "uart.h"
#include "nrf24Radio.h"
#include "nrf24Radio_API.h"
#include "interrupt_hw.h"

#include <util/delay.h>
#include "system-timer.h"
#include "wdg.h"

#include "atmega-adc.h"
#include "lowpower.h"
#include "ds18b20.h"

#define BUCK_5V_SETUP() 	DDRD |= 0x20
#define BUCK_5V_ON() 		PORTD &= 0xDF
#define BUCK_5V_OFF()	 	PORTD |= 0x20

#define BUCK_12V_SETUP()	DDRD |= 0x40
#define BUCK_12V_ON()		PORTD &= 0xBF
#define BUCK_12V_OFF()		PORTD |= 0x40

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

#define MAX_ADDR	5
#define MAX_CMD_LEN 34
uint8_t command_type;
uint8_t command_len;
uint8_t cmd[MAX_CMD_LEN];
uint8_t cmd_available = 0;

#define CMD_SET_TX_ADDR				'A'
#define CMD_SET_RX_ADDR				'B'
#define CMD_CONFIGURE_RADIO			'C'
#define CMD_SEND_TX					'D'
#define CMD_SEND_ASCII_HEX			'E'
#define CMD_SEND_PING				'P'
#define CMD_RESET					'R'

#define CMD_TOGGLE_CONSOLE			'O'
uint8_t uart_console = 0;
uint8_t radio_enabled = 0;
uint16_t loop = 0;

uint8_t tmpstr[20];
uint32_t battery_lvl = 0;
int32_t temperature = 0;

uint8_t sleep_enabled = 0;

#define THRESHOLD_ENTER_SLEEP_VOLT		2400
#define THRESHOLD_EXIT_SLEEP_VOLT		2500

#define THRESHOLD_ENTER_SLEEP_TEMP		0
#define THRESHOLD_EXIT_SLEEP_TEMP		500

#define THRESHOLD_ENTER_SLEEP_TEMP_COUNT 	5
#define THRESHOLD_ENTER_SLEEP_VOLT_COUNT 	5
#define THRESHOLD_EXIT_SLEEP_TEMP_COUNT 	5
#define THRESHOLD_EXIT_SLEEP_VOLT_COUNT 	5

#define PROG_VERSION "2.1.0"

#define INVALID_HEX 255
#define _ASCII_HEX_TO_INT(x) (x >= '0' && x <= '9') ? x - '0' : INVALID_HEX
#define ASCII_HEX_TO_INT(x) ( x >= 'A' && x <= 'F') ? 10 + (x - 'A') : _ASCII_HEX_TO_INT(x)
#define CALCULATE_PAYLOAD_LENGTH(x)		(uint8_t)(x/2) + (uint8_t)1

void rx_handler(uint8_t pipe, uint8_t * data, uint8_t payload_length) {
	uart_printString("<RX_PIPE:",0);
	uart_printRegister(pipe);
	uart_printString(">",1);

	uart_printString("<RX_DATA:",0);
	char msg[33] = {0};
	msg[payload_length] = '\0';
	for(int i = 0; i < payload_length; i++)
	{
		msg[i] = data[i];
	}
	uart_printString(msg, 0);
	uart_printString(">", 1);
	
}

void tx_handler(radio_tx_status tx_status) {
	
}

static inline void pre_read_data_seonsor()
{
	if(STATUS_DS18B20_OK != ds18b20_start_conv(0))
	{
		uart_printString("DS18 device busy!",1);
	}
}

static inline void read_data_sensors()
{

	loop = 0;
	battery_lvl = adc_read(ADC_PRESCALER_32, ADC_VREF_AVCC, 0);
	battery_lvl = (battery_lvl * 330 / 1023) * 16;
	
	temperature = ds18b20_gettemp();

	sprintf(tmpstr,"TMP:%ld", temperature);
	uart_printString(tmpstr, 1);

	sprintf(tmpstr,"BAT:%ld", battery_lvl);
	uart_printString(tmpstr, 1);
}

static inline void check_sleep()
{
	static uint8_t under_zero_count = 0;
	static uint8_t temp_restore_count = 0;
	static uint8_t under_voltage_count = 0;
	static uint8_t voltage_restore_count;

	static uint8_t temp_restore_sleep = 1;
	static uint8_t voltage_restore_sleep = 1;

	//if the temperature is below 0C
	if(temperature < THRESHOLD_ENTER_SLEEP_TEMP)
	{
		under_zero_count++;
	}
	//if the temperature is below treshold (24V)
	if(battery_lvl < THRESHOLD_ENTER_SLEEP_VOLT)
	{
		under_voltage_count++;
	}


	if(under_zero_count > THRESHOLD_ENTER_SLEEP_TEMP_COUNT)
	{
		temp_restore_sleep = 0;
		under_zero_count = 0;
		sleep_enabled = 1;
		uart_printString("Enter sleep: below 0C", 1);
	}
	if(under_voltage_count > THRESHOLD_ENTER_SLEEP_VOLT_COUNT)
	{
		voltage_restore_sleep = 0;
		under_voltage_count = 0;
		sleep_enabled = 1;
		uart_printString("Enter sleep: under voltage", 1);
	}

	if(sleep_enabled)
	{
		//if battery volate get back and is over 25V
		if( (voltage_restore_sleep == 0) && battery_lvl > THRESHOLD_EXIT_SLEEP_VOLT)
		{
			voltage_restore_count++;
		}
		//temperature is over 5C
		if( (temp_restore_count == 0) && temperature > THRESHOLD_EXIT_SLEEP_TEMP)
		{
			temp_restore_count++;
		}

		if(voltage_restore_count > THRESHOLD_EXIT_SLEEP_VOLT_COUNT)
		{
			voltage_restore_sleep = 1;
			uart_printString("Voltage is over 24V, set flag ok", 1);
		}

		if(temp_restore_count > THRESHOLD_EXIT_SLEEP_TEMP_COUNT)
		{
			temp_restore_sleep = 1;
			uart_printString("Temperature is over 5C, set flag ok", 1);
		}

		//restore the sleep
		if( temp_restore_sleep && voltage_restore_sleep)
		{
			sleep_enabled = 0;
			uart_printString("Restore sleep", 1);
		}
	}
}

void process_uart_data(uint8_t * data, uint8_t len) 
{
	static uint8_t uart_state = UART_S0;
	static uint8_t tmp_len = 0;
	//static uint8_t leng_num = 0;
	for(uint8_t idx = 0; idx < len; idx ++)
	{
		if(uart_console)
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

	BUCK_5V_SETUP();
	BUCK_12V_SETUP();

	BUCK_5V_ON();
	BUCK_12V_ON();

	uart_init(UART_115200BAUD, UART_110592MHZ, UART_PARITY_NONE);
	system_timer_init();
	INT_GLOBAL_EN();
	system_timer_start();
	
	uart_printString("<NRF24L01_programmer:",0);
	uart_printString(PROG_VERSION, 0);
	uart_printString(">",1);
	
	//initilize the NRF data
    uint8_t uart_data_len = 0;
	uint8_t tx_addr[MAX_ADDR] = {0};
	uint8_t rx_addr[MAX_ADDR] = {0};
	//uint8_t init = 0;
	uint8_t uart_rx_err = 0;

	wdg_init(wdgto_500MS);


	radio_config cfg = 
	{ 
		RADIO_ADDRESS_5BYTES,
		RADIO_RT,
		RADIO_RETRANSMIT_15,
		CHANNEL_112,
		RADIO_RATE,
		RADIO_CRC2_ENABLED,
		RADIO_COUNT_WAVE_DISABLED,
		RADIO_HIGHEST_0DBM,
		RADIO_DYNAMIC_PAYLOAD_ENABLED,
		RADIO_ACK_PAYLOAD_ENABLED,
		RADIO_DYNAMIC_ACK_DISABLED,
		RADIO_APPLICATION 
	};
	__nrfRadio_Init(cfg);
	__nrfRadio_FlushBuffer(RADIO_BOTH_BUFFER);
	__nrfRadio_SetRxCallback(rx_handler);
	__nrfRadio_SetTxCallback(tx_handler);
	pipe_config pipe_cfg0 =
	{
		RADIO_PIPE0,
		rx_addr,
		5,
		RADIO_PIPE_RX_DISABLED,
		RADIO_PIPE_AA_ENABLED,
		RADIO_PIPE_DYNAMIC_PYALOAD_ENABLED
	};
	__nrfRadio_PipeConfig(pipe_cfg0);
	pipe_cfg0.enable_rx_address = RADIO_PIPE_RX_DISABLED;
	for(radio_pipe p = RADIO_PIPE1; p <= RADIO_PIPE5; p++)
	{
		pipe_cfg0.pipe = p;
		__nrfRadio_PipeConfig(pipe_cfg0);
	}
	__nrfRadio_PowerDown();
    while (1) 
    {
		wdg_kick();

		//Execute radio Main loop if this is enabled
		if(radio_enabled)
			__nrfRadio_Main();
		
		//Check UART data available
		uart_data_len = uart_rx_flush(uart_data, &uart_rx_err);
		if(UART_RX_ERR != uart_data_len)
			process_uart_data(uart_data, uart_data_len);
		else 
		{
			uart_printString("<UART_RX_ERROR:", 0);
			uart_printRegister(uart_rx_err);
			uart_printString(">",1);
			wdg_explicit_reset(0x1);
			_delay_ms(1000);
		}

		//execute commands
		if(cmd_available)
		{
			uart_printString("<EXECUTE_CMD:",0);
			switch(command_type) 
			{
				case CMD_TOGGLE_CONSOLE:
					uart_console ^= 1;
					if (uart_console)
						uart_printString("CONSOLE_ON>",1);
					else
						uart_printString("CONSOLE_OFF>",1);
					break;
				case CMD_SEND_PING:
					uart_printRegister(command_type);
					uart_printString(">",1);
					if(command_len < MAX_CMD_LEN)
					{
						uart_printString("<SEND_PING:",0);
						uint8_t payload[33] = {0};
						for(uint8_t idx = 0; idx < command_len; idx++)
						{
							payload[idx] = cmd[idx];
						}
						payload[command_len] = '\0';
						uart_printString((char*)payload,0);
						uart_printString(">",1);
						__nrfRadio_LoadMessages(payload, command_len);
						//calculate the round travel time here
						system_timer_timestamp _start =  system_timer_get_timestamp();
						uint8_t status = __nrfRadio_Transmit(tx_addr, RADIO_WAIT_TX);
						system_timer_timestamp _end =  system_timer_get_timestamp();
						if( status == RADIO_TX_OK || status == RADIO_TX_OK_ACK_PYL)
						{
							char str[11] = {0};
							str[10] = '\0';
							uint32_t us_trave_time = system_timer_getus(_start, _end);
							sprintf(str, "%lu",us_trave_time);
							uart_printString("<SEND_PING:ACK>",1);
							uart_printString("<PING:",0);
							uart_printString(str, 0);
							uart_printString(">",1);
						}
						else
						{
							uart_printString("<SEND_PING:NACK>",1);
						}
					}
					else
					{
						uart_printString("<SEND_PING:LENGTH_ERR>", 1);
					}
					break;
				case CMD_SET_TX_ADDR:
					uart_printRegister(command_type);
					uart_printString(">",1);
					if(command_len == 5)
					{
						uart_printString("<SET_TX:",0);
						char tmp[7];
						for(uint8_t idx = 0; idx < command_len; idx ++){
							tx_addr[idx] = cmd[idx];
							tmp[idx] = cmd[idx];
						}
						tmp[5] = '>';
						tmp[6] = '\0';
						uart_printString(tmp,1);
					}
					else
						uart_printString("<SET_TX:LENGTH_ERR>", 1);
				break;
				
				case CMD_SET_RX_ADDR:
					uart_printRegister(command_type);
					uart_printString(">",1);
					if(command_len == 6 && (cmd[0]>='0' && cmd[0]<='5'))
					{
						uart_printString("<SET_RX:",0);
						radio_pipe p = cmd[0] - '0';
						pipe_config pipecfg =
						{
							p,
							&cmd[1],
							5,
							RADIO_PIPE_RX_ENABLED,
							RADIO_PIPE_AA_ENABLED,
							RADIO_PIPE_DYNAMIC_PYALOAD_ENABLED
						};
						__nrfRadio_PowerDown();
						__nrfRadio_PipeConfig(pipecfg);
						char tmp[8];
						memcpy(tmp, cmd, 6);
						tmp[6] = '>';
						tmp[7] = '\0';
						uart_printString(tmp,1);
					}
					else
						uart_printString("<SET_RX:LENGTH_ERR>", 1);
				break;
				case CMD_RESET:
					//perform a reset here
					uart_printRegister(command_type);
					uart_printString(">",1);
					while(1);
					break;
				case CMD_CONFIGURE_RADIO:
					uart_printRegister(command_type);
					uart_printString(">",1);
					if(0 == command_len)
					{
						uart_printString("<NRF_CONFIG:STARTING>", 1);
						__nrfRadio_TransmitMode();
						uart_printString("<NRF_CONFIG:DONE>",1);
					}
					else if(cmd[0] == 'L' && command_len == 1)
					{
						uart_printString("<NRF_CONFIG:STARTING>", 1);
						__nrfRadio_ListeningMode();
						uart_printString("<NRF_CONFIG:DONE_RX>",1);
					}
					__nrfRadio_PowerUp();
				break;
				case CMD_SEND_TX:
					uart_printRegister(command_type);
					uart_printString(">",1);
					if(command_len < MAX_CMD_LEN)
					{
						uart_printString("<SEND_TX:",0);
						uint8_t payload[33] = {0};
						for(uint8_t idx = 0; idx < command_len; idx++)
						{
							payload[idx] = cmd[idx];
						}
						payload[command_len] = '\0';
						uart_printString((char*)payload,0);
						uart_printString(">",1);
						__nrfRadio_LoadMessages(payload, command_len);
						uint8_t status = __nrfRadio_Transmit(tx_addr, RADIO_WAIT_TX);
						if( status == RADIO_TX_OK || status == RADIO_TX_OK_ACK_PYL)
						{
							uart_printString("<SEND_TX:ACK>",1);
						}
						else
						{
							uart_printString("<SEND_TX:NACK>",1);
						}
					}
					else
					{
						uart_printString("<SEND_TX:LENGTH_ERR>", 1);
					}
				break;
				case CMD_SEND_ASCII_HEX:
					uart_printRegister(command_type);
					uart_printString(">",1);
					
					if(command_len < MAX_CMD_LEN)
					{
						uint8_t payload_count = 0;
						uint8_t payload[17] = {0};
						uint8_t nibble = 0;
						uint8_t nibble_count = 0;

						payload[payload_count++] = cmd[0];
						for(uint8_t idx = 1; idx < command_len; idx++)
						{
							if(nibble_count == 0)
							{
								nibble = ASCII_HEX_TO_INT(cmd[idx]);
								if (INVALID_HEX != nibble)
								{
									nibble *= 16;
								}
								else
								{
									uart_printString("<INVALID_HEX>", 1);
									break;
								}
								nibble_count++;
							}
							else if(nibble_count == 1)
							{
								uint8_t tmp;
								tmp = ASCII_HEX_TO_INT(cmd[idx]);
								
								if(INVALID_HEX != tmp)
								{
									nibble += tmp;
								}
								else
								{
									uart_printString("<INVALID_HEX>", 1);
									break;
								}
								if(payload_count < 17)
									payload[payload_count++] = nibble;
								nibble_count = 0;
								nibble = 0;
							}
							
						}
						__nrfRadio_LoadMessages(payload, CALCULATE_PAYLOAD_LENGTH(command_len));
						
						uart_printString("<TX_DATA:",0);
						for(uint8_t i = 0; i < CALCULATE_PAYLOAD_LENGTH(command_len); i++)
							uart_printRegister(payload[i]);
						uart_printString(">", 1);
						
						uint8_t status = __nrfRadio_Transmit(tx_addr, RADIO_WAIT_TX);
						if( status == RADIO_TX_OK || status == RADIO_TX_OK_ACK_PYL)
						{
							uart_printString("<SEND_TX:ACK>",1);
						}
						else
						{
							uart_printString("<SEND_TX:NACK>",1);
						}
					}
					else
					{
						uart_printString("<SEND_FLASH:LENGTH_ERR>", 1);
					}
				break;
				default:
					uart_printString("<CMD_INVALID>", 1);
				break;
			}
			cmd_available = 0;
		}
		loop++;

		//read battery and temperature each 60 seconds
		if((loop == 4000))
		{
			pre_read_data_seonsor();
		}

		if((loop == 5000))
		{
			read_data_sensors();
			check_sleep();

			//in case the device entered in sleep
			while(sleep_enabled)
			{
				BUCK_12V_OFF();
				BUCK_5V_OFF();
				wdg_disable();
				//give some time to finish the uart transmision
				_delay_ms(5);
				goToSleep(WAKEUP_8S);
				_delay_ms(5);
				wdg_init(wdgto_1S);
				pre_read_data_seonsor();
				//wait to read temp sensor, this takes up to 750ms
				while(!ds18b20_is_ready());
				read_data_sensors();
				check_sleep();
			}
			BUCK_12V_ON();
			BUCK_5V_ON();
		}


		_delay_ms(1);
    }
	
}

