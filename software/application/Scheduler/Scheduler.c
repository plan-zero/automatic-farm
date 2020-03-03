/**
 * Copyright (C) 2020 Coding Night Romania
 * 
 * This file is part of AutomaticFarm.
 * 
 * AutomaticFarm is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * AutomaticFarm is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with AutomaticFarm.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "Scheduler.h"
#include "Humidity.h"
#include "stdint.h"


#define TASK_NUMBER_1MS 	((uint8_t)1)
#define TASK_NUMBER_10MS 	((uint8_t)0)
#define TASK_NUMBER_100MS 	((uint8_t)0)
#define TASK_NUMBER_1S 		((uint8_t)0)


uint16_t taskCounter = 0;
//#define ENABLE_TASK_TEST 1

#ifdef ENABLE_TASK_TEST
#include <avr/io.h>
void test_task_1ms()
{
    static init_led = 0;
    if(!init_led)
    {
        init_led = 1;
        DDRD |= 1;
    }
    PORTD ^= 1;
    
}
void test_task_10ms()
{
    static init_led = 0;
    if(!init_led)
    {
        init_led = 1;
        DDRD |= 2;
    }
    PORTD ^= 2;
    
}

void test_task_1s()
{
    static init_led = 0;
    if(!init_led)
    {
        init_led = 1;
        DDRB |= 1;
    }
    PORTB ^= 1;
    
} 



#endif

voidFunctionTypeVoid table_task_1ms     [TASK_NUMBER_1MS]   = 
{
    HUM_Acquire,    
};
voidFunctionTypeVoid table_task_10ms    [TASK_NUMBER_10MS]  = { };
voidFunctionTypeVoid table_task_100ms   [TASK_NUMBER_100MS] = { };
voidFunctionTypeVoid table_task_1s      [TASK_NUMBER_1S]    = { };

typedef struct Timer
{
    voidFunctionTypeVoid* table;
    uint8_t slot;
}Timer;

Timer timer1ms      = {table_task_1ms,      0};
Timer timer10ms     = {table_task_10ms,     0};
Timer timer100ms    = {table_task_100ms,    0};
Timer timer1s       = {table_task_1s,       0};



void task_1ms();

voidFunctionTypeVoid getPointerTo1msTask()
{
    return task_1ms;
}


void task_1ms()
{    
    int8_t multiplier;

    // Call all the 1ms tasks
    for (uint8_t taskIterator = 0; taskIterator < TASK_NUMBER_1MS; taskIterator++)
    {
        timer1ms.table[taskIterator]();
    }

    // Call the 10ms tasks in their respective slots
    if(timer10ms.slot > 0)
    {
        timer10ms.table[TASK_NUMBER_10MS - timer10ms.slot]();       
        timer10ms.slot --;
    }

    if (taskCounter % 10 == 0)
    {
        timer10ms.slot = TASK_NUMBER_10MS;
        
    }



    // Call the 100ms tasks in their respective slots
    if (taskCounter % 10 == 0)
    {
        if(timer100ms.slot > 0)
        {
            timer100ms.table[TASK_NUMBER_100MS - timer100ms.slot]();                
            timer100ms.slot --;
        }
    }  
    if (taskCounter % 100 == 0)
    {
        timer100ms.slot = TASK_NUMBER_100MS;
    }
   
   
    // Call the 1 tasks in their respective slots
    if ((taskCounter + 1) % 100 == 0)
    {
        if(timer1s.slot > 0)
        {
            timer1s.table[TASK_NUMBER_1S - timer1s.slot]();       
            timer1s.slot --;
        }        
    }
    if (taskCounter % 1000 == 0)
    {
        timer1s.slot = TASK_NUMBER_1S;
        taskCounter = 0;
    }
}



// TODO: adaugat alarma (dai sa iti apeleze o functie dupa x secunde)
// TODO adaugat custom task la x ms






