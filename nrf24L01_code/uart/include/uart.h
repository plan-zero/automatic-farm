#ifndef _UART_H
#define _UART_H

#include "stdint.h"
#define BAUD9600 103//51//103
#define UART_RX_MAX 20

#define uartPrintHex(x) (x > 9 ? uart_sendByte('A' + (x - 10 ) ) : uart_sendByte('0' + x))
#define uartNewLine() /*
		*/ uart_sendByte(10); /*
		*/ uart_sendByte(13)

extern void uart_init(uint8_t baud);
extern void uart_sendByte(uint8_t byte);
extern uint8_t uart_sendByteNotBlocking(uint8_t byte);
extern uint8_t uart_rx_flush(uint8_t *buffer);
extern void uart_printRegister(unsigned char reg);
extern void uart_printString(char *string, char crlf);

#endif//_UART_H
