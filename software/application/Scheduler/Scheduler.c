/*
 * Scgheduler.c
 *
 *  Created on: Feb 7, 2020
 *      Author: Adi
 */
#include "Scheduler.h"
#include "Humidity.h"
#include <avr/io.h>


#define TASK_NUMBER_1MS 	((char)1)
#define TASK_NUMBER_10MS 	((char)0)
#define TASK_NUMBER_100MS 	((char)0)
#define TASK_NUMBER_1S 		((char)0)


static volatile char taskFinished = 1;
unsigned short taskCounter = 0;

voidFunctionTypeVoid table_task_1ms     [TASK_NUMBER_1MS]   = 
{
    HUM_Acquire
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
        if (taskCounter == 1000)
        {
            taskCounter = 0;
            PORTB ^= 1;
        }

        

        taskFinished = 1;
    }
    else
    {
        //TODO: WDG reset
    }
    
}







