/**
 * Copyright (C) 2020 Coding Night Romania
 * 
 * This file is part of automatic-farm.
 * 
 * automatic-farm is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * automatic-farm is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with automatic-farm.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "system-timer.h"
#include "util/delay.h"
#include "uart.h"

#include "stdio.h"
#include <avr/interrupt.h>
#include <avr/io.h>

int main()
{
    DDRD |= 0x80;

    system_timer_init();
    
    
    uart_init(UART_9600BAUD, UART_8MHZ, UART_PARITY_NONE);
    uart_printString("Testing system timer",1);
    char str[12] = {0};
    str[11] = '\0';
    
    sei();
    system_timer_start();
    //_delay_ms(1000);
    system_timer_timestamp _st =  system_timer_get_timestamp();
    _delay_ms(124);
    system_timer_timestamp _en =  system_timer_get_timestamp();
    uint32_t delta = system_timer_getms(_st, _en);
    sprintf(str, "%lu",delta);
    uart_printString(str, 1);

    system_timer_info inf = system_timer_get_info(_en);
    char mili[12] = {0};
    char sec[12] = {0};
    char min[12] = {0};
    char h[12] = {0};
    sprintf(mili, "%lu",inf.mili);
    sprintf(sec, "%lu",inf.sec);
    sprintf(min, "%lu",inf.min);
    sprintf(h, "%lu",inf.hour);
    uart_printString(mili, 1);
    uart_printString(sec, 1);
    uart_printString(min, 1);
    uart_printString(h, 1);

    uart_printString("Test done", 1);



    while(1){
        _delay_ms(1000);
        PORTD ^= 0x80;
    }
    return 0;
}