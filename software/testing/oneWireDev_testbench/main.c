#include <avr/io.h>
#include "uart.h"

#include "ds18b20.h"
#include <util/delay.h>
#include "stdint.h"
#include "stdio.h"
#include "atmega-adc.h"


int main()
{

    uart_init(UART_115200BAUD, UART_110592MHZ, UART_PARITY_NONE);
    uart_printString("One wire testing", 1);

    char string[20];
    int32_t data = 0;
    uint32_t adc = 0;
    double tmp = 0;

    while(1)
    {
        //sendData(75);
        adc = adc_read(ADC_PRESCALER_32, ADC_VREF_AVCC, 0);
        tmp = (adc * 3.3) / 1023.0 * 100.00;
        adc = (int)tmp;
        data = ds18b20_gettemp();
        sprintf(string,"TMP:%ld", data);
        uart_printString(string,1);
        sprintf(string, "ADC:%d",adc);
        uart_printString(string,1);
        PORTD ^= 0x08;
        _delay_ms(1000);

    }
}