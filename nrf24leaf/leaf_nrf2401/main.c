#include <avr/io.h>

#define F_CPU 4000000UL
#include <util/delay.h>


#include "radio.h"
#include "uart.h"


volatile int rx_radio = 0;

radiopacket_t send_buffer;
radiopacket_t rec_buffer;

#define RADIO_PAYLOAD_SIZE 23

void RadioUtils_clearPayload(radiopacket_t *radiopck) {
	for(uint8_t idx = 0; idx < RADIO_PAYLOAD_SIZE; idx++ )
	radiopck->payload.message.messagecontent[idx] = 0;
	radiopck->payload.message.messageid = 0;
}

void RadioUtils_CopyString(radiopacket_t *radiopck, char * str) {
	if(strlen(str) <= RADIO_PAYLOAD_SIZE){
		for(uint8_t idx = 0; idx < RADIO_PAYLOAD_SIZE; idx++ )
		radiopck->payload.message.messagecontent[idx] = (uint8_t)*(str + idx);
	}
}

void uart_printString(char *string){
	char *idx = &string[0];

	while(idx != NULL && *idx != '\0'){
		uart_sendByte(*idx);
		idx++;
	}

}

void print_radiopacket(radiopacket_t *buffer){


	//send message ID
	uart_printString("ID:");
	uint8_t msg_id = rec_buffer.payload.message.messageid;
	uart_sendByte(msg_id);

	//send address:
	uart_printString(":ADDR:");
	for(uint8_t idx = 0; idx < 5; idx ++){
		uint8_t  high = (rec_buffer.payload.message.address[idx] & 0xF0)>>4;
		uint8_t  low = rec_buffer.payload.message.address[idx] & 0x0F;
		if (high > 9)
			uart_sendByte(high - 10 + 'A');
		else
			uart_sendByte(high + '0');
		if (low > 9)
			uart_sendByte(low - 10 + 'A');
		else
			uart_sendByte(low + '0');
	}

	//send message
	uart_printString(":MSG:");
	for(uint8_t idx = 0; idx < 23; idx ++){
		uart_sendByte(rec_buffer.payload.message.messagecontent[idx]);

	}
	uart_sendByte(13);
	uart_sendByte(10);
	
}

void radio_rxhandler(uint8_t pipenumber){
	rx_radio = 1;
}


int main(){

	Radio_Init();
	uart_init(BAUD9600);

	_delay_ms(1000);
	uart_printString("Radio Test");
	uint8_t my_add[5] = { 0xE4, 0xE4, 0xE4, 0xE4, 0xE4 };
	uint8_t address1[5] = { 0xE0, 0x70, 0x35, 0x01, 0xA1 }; 
	Radio_Configure_Rx(RADIO_PIPE_1, my_add, ENABLE);
	Radio_Set_Tx_Addr(address1);
	
	
	rec_buffer.payload.message.address[0] = 0xE4;
	rec_buffer.payload.message.address[1] = 0xE4;
	rec_buffer.payload.message.address[2] = 0xE4;
	rec_buffer.payload.message.address[3] = 0xE4;
	rec_buffer.payload.message.address[4] = 0xE4;

	Radio_Configure(RADIO_2MBPS, RADIO_HIGHEST_POWER);
	
	GIFR = (1<<INTF0);
	sei();
	while(1){
		if(rx_radio){
			Radio_Receive(&rec_buffer);
			Radio_Flush();
			print_radiopacket(&rec_buffer);
			rx_radio = 0;
		}
		
		_delay_ms(500);
		//Radio_Transmit(&send_buffer, RADIO_WAIT_FOR_TX);//RADIO_WAIT_FOR_TX);
		//Radio_Flush();
	
	}


}
