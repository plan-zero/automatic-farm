#ifndef _UART_H
#define _UART_H

#include "stdint.h"

#define UART_RX_MAX 40

#define UART_1MHZ 0
#define UART_2MHZ 1
#define UART_4MHZ 2
#define UART_8MHZ 3
#define UART_16MHZ 4
#define UART_COUNT_MHZ 5

#define UART_9600BAUD  0
#define UART_19200BAUD 1
#define UART_57600BAUD 2
#define UART_115200BAUD 3
#define UART_250000BAUD 4
#define UART_COUNT_BAUD 5

#define UART_PARITY_NONE 0
#define UART_PARITY_EVEN 1
#define UART_PARITY_ODD  2

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
extern uint8_t uart_rx_flush(uint8_t *buffer);
extern void uart_printRegister(unsigned char reg);
extern void uart_printString(char *string, char crlf);

#endif//_UART_H
