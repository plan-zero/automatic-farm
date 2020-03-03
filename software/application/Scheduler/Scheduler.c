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

voidFunctionType table_task_1ms     [TASK_NUMBER_1MS]   = 
{
    HUM_Acquire
};
voidFunctionType table_task_10ms    [TASK_NUMBER_10MS]  = { };
voidFunctionType table_task_100ms   [TASK_NUMBER_100MS] = { };
voidFunctionType table_task_1s      [TASK_NUMBER_1S]    = { };

void task_1ms();

voidFunctionType getPointerTo1msTask()
{
    return task_1ms;
}


void task_1ms()
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







