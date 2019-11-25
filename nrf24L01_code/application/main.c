#include <avr/io.h>
#include <stdlib.h>

#define F_CPU 8000000UL
#include <util/delay.h>
#include <avr/interrupt.h>
#include "uart.h"

#include "nrf24Radio.h"
#include "radio_fptr.h"

void printRadioInfo(radio_registers regs)
{ 
	uart_printString("CONFIG register: ", 0);
	uart_printRegister(regs.config);
	uartNewLine();
	uart_printString("EN_AA register: ", 0);
	uart_printRegister(regs.en_aa);
	uartNewLine();
	uart_printString("EN_RXADDR register: ", 0);
	uart_printRegister(regs.en_rxaddr);
	uartNewLine();
	uart_printString("SETUP_AW register: ", 0);
	uart_printRegister(regs.setup_aw);
	uartNewLine();
	uart_printString("SETUP_RETR register: ", 0);
	uart_printRegister(regs.setup_retr);
	uartNewLine();
	uart_printString("RF_CH register: ", 0);
	uart_printRegister(regs.rf_ch);
	uartNewLine();
	uart_printString("RF_SETUP register: ", 0);
	uart_printRegister(regs.rf_setup);
	uartNewLine();
	uart_printString("STATUS register: ", 0);
	uart_printRegister(regs.status);
	uartNewLine();
	uart_printString("OBSERVE TX register: ", 0);
	uart_printRegister(regs.observed_tx);
	uartNewLine();
	uart_printString("PIPE 0 address: ", 0);
	uart_printRegister(regs.rx_addr_p0_0);
	uart_printRegister(regs.rx_addr_p0_1);
	uart_printRegister(regs.rx_addr_p0_2);
	uart_printRegister(regs.rx_addr_p0_3);
	uart_printRegister(regs.rx_addr_p0_4);
	uartNewLine();
	uart_printString("PIPE 1 address: ", 0);
	uart_printRegister(regs.rx_addr_p1_0);
	uart_printRegister(regs.rx_addr_p1_1);
	uart_printRegister(regs.rx_addr_p1_2);
	uart_printRegister(regs.rx_addr_p1_3);
	uart_printRegister(regs.rx_addr_p1_4);
	uartNewLine();
	uart_printString("PIPE 2 address: ", 0);
	uart_printRegister(regs.rx_addr_p2);
	uartNewLine();
	uart_printString("PIPE 3 address: ", 0);
	uart_printRegister(regs.rx_addr_p3);
	uartNewLine();
	uart_printString("PIPE 4 address: ", 0);
	uart_printRegister(regs.rx_addr_p4);
	uartNewLine();
	uart_printString("PIPE 5 address: ", 0);
	uart_printRegister(regs.rx_addr_p5);
	uartNewLine();
	uart_printString("TX_ADDR address: ", 0);
	uart_printRegister(regs.tx_addr_0);
	uart_printRegister(regs.tx_addr_1);
	uart_printRegister(regs.tx_addr_2);
	uart_printRegister(regs.tx_addr_3);
	uart_printRegister(regs.tx_addr_4);
	uartNewLine();
	uart_printString("FIFO STATUS: ", 0);
	uart_printRegister(regs.fifo_status);
	uartNewLine();
	uart_printString("DYNPD: ", 0);
	uart_printRegister(regs.dynpd);
	uartNewLine();
	uart_printString("FEATURE: ", 0);
	uart_printRegister(regs.feature);
}

void rx_handler(uint8_t pipe, uint8_t * data, uint8_t payload_length) {
	
	uart_printString("Data received on pipe:", 0);
	uart_sendByte('0' + pipe);
	uartNewLine();
	char string[33] = {0};
	string[32] = '\0';
	for(uint8_t idx = 0; idx < payload_length; idx ++){
		string[idx] = data[idx];
	}
	uart_printString(string, 1);
	uint8_t ack_payload [2] = {'A', 'F'};
	nrfRadio_LoadAckPayload(RADIO_PIPE3, ack_payload, 2);
}

void tx_handler(radio_tx_status tx_status) {
	if(RADIO_TX_OK == tx_status) {
		uart_printString("TX received ack",1);
	}
	else
		uart_printString("TX not received ack",1);
}

int main(){

	
	uart_init(BAUD9600);
	_delay_ms(500);
	
	uart_printString("Radio Initialization", 1);
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
						 RADIO_APPLICATION };
	(*fptr_nrfRadio_Init)(cfg);
	
	//NOTE: actually the first byte is setting the address because the NRF takes firstly the LSByte 
	uint8_t pipe0_address[5] = { 0xE2, 0xE0, 0xE0, 0xE2, 0xA2 };
	uint8_t pipe1_address[5] = { 0xE1, 0xE0, 0xE0, 0xE2, 0xA2 };
	uint8_t pipe5_address[5] = {0xE0};
	pipe_config pipe_cfg0 = {	RADIO_PIPE0,
								pipe0_address,
								5,
								RADIO_PIPE_RX_ENABLED,
								RADIO_PIPE_AA_ENABLED,
								RADIO_PIPE_DYNAMIC_PYALOAD_ENABLED
							};
	pipe_config pipe_cfg1 = {	RADIO_PIPE1,
		pipe1_address,
		5,
		RADIO_PIPE_RX_ENABLED,
		RADIO_PIPE_AA_ENABLED,
		RADIO_PIPE_DYNAMIC_PYALOAD_ENABLED
	};
	pipe_config pipe_cfg2 = {	RADIO_PIPE3,
		pipe5_address,
		5,
		RADIO_PIPE_RX_ENABLED,
		RADIO_PIPE_AA_ENABLED,
		RADIO_PIPE_DYNAMIC_PYALOAD_ENABLED
	};
	nrfRadio_PipeConfig(pipe_cfg0);
	nrfRadio_PipeConfig(pipe_cfg1);
	nrfRadio_PipeConfig(pipe_cfg2);
	
	uart_printString("Read radio information", 1);
	radio_registers regs = {0}; //init the register
	nrfRadio_GetInfo(&regs);
	printRadioInfo(regs);
	
	//uart_printString("TX done", 1);
	
	nrfRadio_SetRxCallback(rx_handler);
	nrfRadio_SetTxCallback(tx_handler);
	nrfRadio_PowerUp();
	nrfRadio_TransmitMode();
	//nrfRadio_ListeningMode();
	uint8_t ack_payload [2] = {'A', 'F'};
	nrfRadio_LoadAckPayload(RADIO_PIPE3, ack_payload, 2);
	
	sei();
	uint8_t payload[5] = {'A','B','C','D','E'};
	uint8_t tx_address[5] =  { 0xE0, 0xE0, 0xE0, 0xE2, 0xA2 };

	
	uint32_t transmit_count = 0;
	
	while(1){
		transmit_count++;
		
		if(transmit_count == 100000) {
			transmit_count = 0;
			payload[0]++;
			if (payload[0] == 'E')
				payload[0] = 'A';
			nrfRadio_LoadMessages(payload, 5);
			if( RADIO_TX_OK == nrfRadio_Transmit(tx_address, RADIO_RETURN_ON_TX) )
				uart_printString("ack ok",1);
			else
				uart_printString("ack not ok",1);
		}
			
		//nrfRadio_Main();
		(*fptr_nrfRadio_Main)();
		//_delay_ms(500);
	}


}
