#include <avr/io.h>

#define F_CPU 8000000UL
#include <util/delay.h>
#include "radio.h"


#include "uart.h"


volatile int rx_radio = 0;

radiopacket_t buffer;

void print_radiopacket(radiopacket_t packet){


	//send message ID
	uint8_t msg_id = buffer.payload.message.messageid;
	uart_sendByte(msg_id);

	//send address:
	uart_sendByte(':');
	for(uint8_t idx = 0; idx < 5; idx ++){
		uart_sendByte(buffer.payload.message.address[idx]);

	}

	//send message
	uart_sendByte(':');
	for(uint8_t idx = 0; idx < 23; idx ++){
		uart_sendByte(buffer.payload.message.messagecontent[idx]);

	}
	uart_sendByte('\n');
	
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



int main(){
	
	adc_init();
	uart_init(BAUD9600);

	uint8_t address_tx[5] = {0xFF, 0xFC, 0xE1, 0xA8, 0xA8};

    Radio_Configure_Rx(RADIO_PIPE_0, address_tx, ENABLE);

	address_tx[0] = 0x00;
	//Radio_Configure_Rx(RADIO_PIPE_1, address_tx, ENABLE);
	Radio_Set_Tx_Addr(address_tx);
	Radio_Flush();
	Radio_Configure(RADIO_1MBPS, RADIO_HIGH_POWER);

	Radio_Init();
	buffer.payload.message.messagecontent[0] = 'A';
	buffer.payload.message.messagecontent[1] = 'B';
	

	while(1){
		if(rx_radio){
			Radio_Receive(&buffer);
			print_radiopacket(buffer);
		}

	////	Radio_Transmit(&buffer, RADIO_RETURN_ON_TX);
	//	_delay_ms(1000);

		

	}


}
