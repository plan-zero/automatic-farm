#include <avr/io.h>
#include "uart.h"

#include "ds18b20.h"
#include <util/delay.h>
#include "stdint.h"
#include "stdio.h"



int main()
{

    uart_init(UART_115200BAUD, UART_110592MHZ, UART_PARITY_NONE);
    uart_printString("One wire testing", 1);

    char string[20];
    uint32_t data = 0;

    while(1)
    {
        //sendData(75);
        data = ds18b20_gettemp();
        sprintf(string,"TMP:%u", (int)data);
        uart_printString(string,1);
        PORTD ^= 0x08;
        _delay_ms(1000);

    }
}