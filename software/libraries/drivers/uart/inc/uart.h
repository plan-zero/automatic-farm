#ifndef _UART_H
#define _UART_H

#include "stdint.h"

#define UART_RX_MAX 40
#define UART_RX_ERR 255

#define UART_PARITY_NONE 0
#define UART_PARITY_EVEN 1
#define UART_PARITY_ODD  2

#define UART_1MHZ			_UART_1MHZ
#define UART_2MHZ			_UART_2MHZ
#define UART_4MHZ			_UART_4MHZ
#define UART_8MHZ			_UART_8MHZ
#define UART_16MHZ			_UART_16MHZ
#define UART_COUNT_MHZ		_UART_COUNT_MHZ

#define UART_9600BAUD  		_UART_9600BAUD
#define UART_19200BAUD 		_UART_9600BAUD
#define UART_57600BAUD		_UART_9600BAUD
#define UART_115200BAUD		_UART_9600BAUD
#define UART_250000BAUD		_UART_9600BAUD
#define UART_COUNT_BAUD		_UART_9600BAUD

typedef enum {
	UART_INVALID_ARG,
	UART_BAUD_NOT_SUPPORTED,
	UART_CONFIG_OK	
}uart_err;

#define uartPrintHex(x) (x > 9 ? uart_sendByte('A' + (x - 10 ) ) : uart_sendByte('0' + x))
#define uartNewLine() /*
		*/ uart_sendByte(10); /*
		*/ uart_sendByte(13)

extern uart_err uart_init(uint8_t baud, uint8_t cpu_freq, uint8_t uart_parity);
extern void uart_sendByte(uint8_t byte);
extern uint8_t uart_sendByteNotBlocking(uint8_t byte);
extern uint8_t uart_rx_flush(uint8_t *buffer, uint8_t *rx_error);
extern void uart_printRegister(unsigned char reg);
extern void uart_printString(char *string, char crlf);
extern volatile uint8_t uart_rx_err_state;

#endif//_UART_H
