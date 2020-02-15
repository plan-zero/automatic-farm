#ifndef _UART_HW_H
#define _UART_HW_H

#include <avr/io.h>
#include "stdint.h"

#define UART_BUSY_STATUS            0
#define UART_2X_ENABLE              0

#define UART_HW_SET_LBAUD(x)        UBRRL = x
#define UART_HW_SET_HBAUD(x)        UBRRH = x
#define UART_HW_SET_PARITY_EVEN()   UCSRC |= (0x1) << UPM1
#define UART_HW_SET_PARITY_ODD()    UCSRC |= (0x1) << UPM1 | (0x1) << UPM0
#define UART_HW_SET_DATA(x)         UDR = x
#define UART_HW_GET_DATA()          UDR
#define UART_HW_IS_BUSY()           ((UCSRA >> UDRE) & 0x1)


#define UART_HW_RXCIE()              UCSRB |= (0x1 << RXCIE)
#define UART_HW_TXCIE()              UCSRB |= (0x1 << TXCIE)

#if (UART_2X_ENABLE == 1)
    #define UART_HW_ENABLE_2X()          UCSRA  = (0x1 << U2X)
#elif (UART_2X_ENABLE == 0)
    #define UART_HW_ENABLE_2X()          __asm__("nop");
#endif
#define UART_HW_IS_2X()              (UCSRA >> U2x) & 0x1

#define UART_HW_SETUP_CODE() \
	UCSRB  = (0x1 << TXEN) | (0x1 << RXEN); \
	UCSRC  = (0x1 << URSEL) | (0x1 << UCSZ0) | (0x1 << UCSZ1)


#define _UART_1MHZ 0
#define _UART_2MHZ 1
#define _UART_4MHZ 2
#define _UART_8MHZ 3
#define _UART_16MHZ 4
#define _UART_COUNT_MHZ 5

#define _UART_9600BAUD  0
#define _UART_19200BAUD 1
#define _UART_57600BAUD 2
#define _UART_115200BAUD 3
#define _UART_250000BAUD 4
#define _UART_COUNT_BAUD 5

#define UART_ERROR_STATE()      ((UCSRA & 0x1C) >> 2)
#define UART_ERROR_CLEAR()      UCSRA &= ~0x1C

#if (UART_2X_ENABLE == 1)
const int16_t uart_values[_UART_COUNT_MHZ][_UART_COUNT_BAUD] = 
{
// 9600	  19200  57600  115200  250000 (U2X = 0)
	{12,  6,     1,     0,      -1 },
	{25,  12,    3,     1,      0},
	{51,  25,    8,     3,      1 },
	{103, 51,    16,    8,      3 },
	{207, 103,   34,    16,     7 }
};
#elif (UART_2X_ENABLE == 0)
const int16_t uart_values[_UART_COUNT_MHZ][_UART_COUNT_BAUD] = 
{
// 9600	  19200  57600  115200  250000 (U2X = 0)
	{6,   2,     0,     -1,     -1 }, //1MHZ
	{12,  6,     1,     0,      -1 }, //2MHZ
	{25,  12,    3,     1,       0 }, //4MHZ
	{51,  25,    8,     3,       1 }, //8MHZ
	{103, 51,   16,     8,       3 }  //16MHZ
};
#endif

#endif