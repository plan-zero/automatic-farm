#include <avr/io.h>
#include "uart.h"


#include <util/delay.h>
#include "stdint.h"



//0 logic ii 50 ms pe low si 1 logic 2ms pe low in rest pe high

void sendData(uint8_t data)
{
    uint8_t bit_data = 0;
    for(int i=7; i>=0; i--)
    {
        PORTB |=0x08;
        _delay_ms(1);
        
        bit_data = (data>>i) & 0x01;
        if(bit_data == 1)
        {
            PORTB &= 0xF7;
            _delay_ms(10);
        }
        else
        {
            PORTB &=0xF7;
            _delay_ms(2);
        }
    }
    PORTB |=0x08;
}


int main()
{

    DDRD |= 0x08;



    uart_init(UART_9600BAUD, UART_110592MHZ, UART_PARITY_NONE);
    uart_printString("One wire setup", 1);
    while(1)
    {
        //sendData(75);
        PORTD ^= 0x08;
        _delay_ms(500);
    }
}