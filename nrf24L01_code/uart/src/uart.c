#include <avr/io.h>
#include "uart.h"
#include <avr/interrupt.h>
#include <stdlib.h>


#define UART_IDLE 		0x0
#define UART_SEND 		0x1
#define UART_RECEIVE 	0x2
#define UART_OK			0x4
#define UART_NOK		0x5
#define UART_RX_ERROR   0x6



#define UART_RXCIE UCSRB |= (0x1 << RXCIE);
#define UART_TXCIE UCSRB |= (0x1 << TXCIE);


uint8_t uart_tx_state;
uint8_t uart_rx_state;
uint8_t uart_rx_buffer[UART_RX_MAX];
uint8_t uart_rx_index;

volatile uint8_t uart_rx_err_state;

int16_t uart_values[UART_COUNT_MHZ][UART_COUNT_BAUD] = 
{
	{12, 6, 1, 0, -1 },
	{25, 12, 3, 1, 0},
	{51, 25, 8, 3, 1 },
	{103, 51, 16, 8, 3 },
	{207, 103, 34, 16, 7 }
};

ISR(USART_TXC_vect)
{
	uart_tx_state = UART_IDLE;
	
}

ISR(USART_RXC_vect)
{

	if ( ((UCSRA & 0x1C) >> 2) != 0)
	{
		uart_rx_err_state = (UCSRA & 0x1C) >> 2;
		uart_rx_state = UART_RX_ERROR;
		UCSRA &= ~0x1C;
	}
	else 
	{
		uart_rx_state = UART_RECEIVE;
		uart_rx_buffer[uart_rx_index++] = UDR;

		if(uart_rx_index >= UART_RX_MAX)
		{
			uart_rx_index = 0;
		}
	}

}

uint8_t uart_rx_flush(uint8_t *buffer, uint8_t *rx_error)
{
	cli();
	uint8_t uart_available = 0;
	if(uart_rx_state == UART_RECEIVE)
	{
		for(uint8_t idx = 0; idx < uart_rx_index; idx++)
		{
			*(buffer + idx) = *(uart_rx_buffer + idx);

		}
		uart_available = uart_rx_index;
		uart_rx_index = 0;
		uart_rx_state = UART_IDLE;
	}
	else if(uart_rx_state == UART_RX_ERROR)
	{
		//reset the uart buffer
		uart_rx_index = 0;
		uart_rx_state = UART_IDLE;
		(*rx_error) = uart_rx_err_state;
		uart_rx_err_state = 0;
		uart_available = UART_RX_ERR;
	}
	sei();

	return uart_available;
}



uart_err uart_init(uint8_t baud, uint8_t cpu_freq, uint8_t uart_parity) // 1Mhz baud, 8 data, 1 stop, none parity
{
	if( (baud > UART_250000BAUD) | (cpu_freq > UART_16MHZ) )
		return UART_INVALID_ARG;

	uart_rx_index = 0;
	uart_tx_state = UART_IDLE;
	uart_rx_state = UART_IDLE;

    UCSRA  = (0x1 << U2X);
	UCSRB  = (0x1 << TXEN) | (0x1 << RXEN) | (0x1 << RXCIE) | (0x1 << TXCIE);
	UCSRC  = (0x1 << URSEL) | (0x1 << UCSZ0) | (0x1 << UCSZ1);

	if( uart_values[cpu_freq][baud] != -1)
		UBRRL =  (uint8_t)uart_values[cpu_freq][baud];
	else
		return UART_BAUD_NOT_SUPPORTED;

	if(uart_parity == UART_PARITY_EVEN)
		UCSRC |= (0x1) << UPM1;
	else if (uart_parity == UART_PARITY_ODD)
		UCSRC |= (0x1) << UPM1 | (0x1) << UPM0;

	
	return UART_CONFIG_OK;
}

void uart_sendByte(uint8_t byte)
{
	UDR = byte;

	while( ( (UCSRA >> UDRE) & 0x1) == 0x0 )
	{
		
	}
}



uint8_t uart_sendByteNotBlocking(uint8_t byte)
{
	uint8_t retValue = UART_NOK;
	if( uart_tx_state == UART_IDLE)
	{
		retValue = UART_OK;
		
		UDR = byte;

	}

	return retValue;
}

void uart_printString(char *string, char crlf){
	char *idx = &string[0];

	while(idx != NULL && *idx != '\0'){
		uart_sendByte(*idx);
		idx++;
	}
	if(crlf) {
		uartNewLine();
	}
}

void uart_printRegister(unsigned char reg){
	uart_sendByte('0');
	uart_sendByte('x');
	unsigned char n1 = reg >> 4;
	unsigned char n2 = reg & 0x0F;
	uartPrintHex(n1);
	uartPrintHex(n2);
	//uart_sendByte(0x20);
	
}