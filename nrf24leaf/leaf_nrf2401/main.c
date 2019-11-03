#include <avr/io.h>

#define F_CPU 4000000UL
#include <util/delay.h>
#include "radio.h"


#include "uart.h"


volatile int rx_radio = 0;

radiopacket_t buffer;

void uart_printString(char *string){
	char *idx = &string[0];

	while(idx != NULL && *idx != '\0'){
		uart_sendByte(*idx);
		idx++;
	}

}

void print_radiopacket(radiopacket_t packet){


	//send message ID
	uart_printString("ID:");
	uint8_t msg_id = buffer.payload.message.messageid;
	uart_sendByte(msg_id);

	//send address:
	uart_printString(":ADDR:");
	for(uint8_t idx = 0; idx < 5; idx ++){
		uint8_t address1 = buffer.payload.message.address[idx] % 10;
		uint8_t address2 = (buffer.payload.message.address[idx] / 10) % 10;
		uart_sendByte(address2);
		uart_sendByte(address1);
	}

	//send message
	uart_printString(":MSG:");
	for(uint8_t idx = 0; idx < 23; idx ++){
		uart_sendByte(buffer.payload.message.messagecontent[idx]);

	}
	uart_sendByte(13);
	uart_sendByte(10);
	
}

void radio_rxhandler(uint8_t pipenumber){
	rx_radio = 1;
}



void adc_init()
{
	// AREF = AVcc
	ADMUX = (1<<REFS0);

	// ADC Enable and prescaler of 128
	// 16000000/128 = 125000
	ADCSRA = (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1); //|(1<<ADPS0);
}


uint16_t adc_read(uint8_t ch)
{
	// select the corresponding channel 0~7
	// ANDing with ’7' will always keep the value
	// of ‘ch’ between 0 and 7
	ch &= 0b00000111;  // AND operation with 7
	ADMUX = (ADMUX & 0xF8)|ch; // clears the bottom 3 bits before ORing
	
	// start single convertion
	// write ’1' to ADSC
	ADCSRA |= (1<<ADSC);
	
	// wait for conversion to complete
	// ADSC becomes ’0' again
	// till then, run loop continuously
	while(ADCSRA & (1<<ADSC));
	
	return (ADC);
}

#define TRANS2
//#define REC

int main(){
	
	adc_init();

	Radio_Init();
	uart_init(BAUD9600);

	//_delay_ms(1000);
	uart_printString("Radio Test");

	
	

#ifdef TRANS1
	uint8_t address[5] = { 0xE4, 0xE4, 0xE4, 0xE4, 0xE4 };
	uint8_t my_add[5] = { 0x98, 0x76, 0x54, 0x32, 0x10 };
	Radio_Configure_Rx(RADIO_PIPE_0, my_add, ENABLE);
	Radio_Set_Tx_Addr(address);

	buffer.payload.message.address[0] = 0x98;
	buffer.payload.message.address[1] = 0x76;
	buffer.payload.message.address[2] = 0x54;
	buffer.payload.message.address[3] = 0x32;
	buffer.payload.message.address[4] = 0x10;
#endif
#ifdef TRANS2  //REC
	uint8_t address[5] = { 0xE4, 0xE4, 0xE4, 0xE4, 0xE4 };
	uint8_t my_add[5] = { 0x98, 0x76, 0x54, 0x32, 0x11 };
	Radio_Configure_Rx(RADIO_PIPE_0, my_add, ENABLE);
	Radio_Set_Tx_Addr(address);

	buffer.payload.message.address[0] = 0x98;
	buffer.payload.message.address[1] = 0x76;
	buffer.payload.message.address[2] = 0x54;
	buffer.payload.message.address[3] = 0x32;
	buffer.payload.message.address[4] = 0x11;
#endif
#ifdef REC
	uint8_t my_add[5] = { 0xE4, 0xE4, 0xE4, 0xE4, 0xE4 };
	uint8_t address1[5] = { 0x98, 0x76, 0x54, 0x32, 0x11 }; 
	uint8_t address2[5] = { 0x98, 0x76, 0x54, 0x32, 0x10 }; 
	Radio_Configure_Rx(RADIO_PIPE_1, my_add, ENABLE);
	
	
	buffer.payload.message.address[0] = 0xE4;
	buffer.payload.message.address[1] = 0xE4;
	buffer.payload.message.address[2] = 0xE4;
	buffer.payload.message.address[3] = 0xE4;
	buffer.payload.message.address[4] = 0xE4;

#endif

	Radio_Configure(RADIO_2MBPS, RADIO_HIGHEST_POWER);
	
	GIFR = (1<<INTF0);
	
	sei();
	while(1){
		if(rx_radio){
			Radio_Receive(&buffer);
			Radio_Flush();
			print_radiopacket(buffer);
			rx_radio = 0;
			if(buffer.payload.message.messagecontent[0] == 0xAA) {
				buffer.payload.message.messagecontent[0] = 0xBB;
				Radio_Transmit(&buffer, RADIO_WAIT_FOR_TX);//RADIO_WAIT_FOR_TX);
				Radio_Flush();
			}
		}

#ifdef REC
		uart_printString("ACK Time test");
		uart_sendByte(13);
		uart_sendByte(10);
		
		buffer.payload.message.messagecontent[0] = 0xAA;
		Radio_Set_Tx_Addr(address1);
		Radio_Transmit(&buffer, RADIO_WAIT_FOR_TX);//RADIO_WAIT_FOR_TX);
		Radio_Flush();
		Radio_Set_Tx_Addr(address2);
		Radio_Transmit(&buffer, RADIO_WAIT_FOR_TX);//RADIO_WAIT_FOR_TX);
		Radio_Flush();
		_delay_ms(1000);
#endif
	}


}
