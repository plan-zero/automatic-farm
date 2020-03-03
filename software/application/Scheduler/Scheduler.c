/*
 * Scgheduler.c
 *
 *  Created on: Feb 7, 2020
 *      Author: Adi
 */
#include "Scheduler.h"
#include "Humidity.h"



#define TASK_NUMBER_1MS 	((char)1)
#define TASK_NUMBER_10MS 	((char)0)
#define TASK_NUMBER_100MS 	((char)0)
#define TASK_NUMBER_1S 		((char)0)


static volatile char taskFinished = 1;
unsigned short taskCounter = 0;

#ifdef ENABLE_TASK_TEST
#include <avr/io.h>
void test_task_1ms()
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

voidFunctionTypeVoid table_task_1ms     []   = 
{
    HUM_Acquire,

//this is available just for testing
#ifdef ENABLE_TASK_TEST
    test_task_1ms
#endif
};
voidFunctionTypeVoid table_task_10ms    [TASK_NUMBER_10MS]  = { };
voidFunctionTypeVoid table_task_100ms   [TASK_NUMBER_100MS] = { };
voidFunctionTypeVoid table_task_1s      [TASK_NUMBER_1S]    = { };

void task_1ms();

voidFunctionType getPointerTo1msTask()
{
    return task_1ms;
}


void task_1ms(int timer_instance)
{
    if(taskFinished)
    {
        taskFinished = 0;

        taskCounter++;
    
        for (int taskIterator = 0; taskIterator < TASK_NUMBER_1MS; taskIterator++)
        {
            table_task_1ms[taskIterator]();
        }
        taskFinished = 1;
    }
    else
    {
        //TODO: WDG reset
    }
    
}







