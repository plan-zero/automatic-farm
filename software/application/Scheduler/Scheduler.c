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
#include "stdlib.h"

typedef void (**ptr_voidFunctionTypeVoid)();

#define TASK_1MS_MAX        ((uint8_t)8)
#define TASK_10MS_MAX       ((uint8_t)8)
#define TASK_100MS_MAX      ((uint8_t)8)
#define TASK_1S_MAX         ((uint8_t)8)

uint8_t TASK_NUMBER_1MS =	((uint8_t)0);
uint8_t TASK_NUMBER_10MS =	((uint8_t)0);
uint8_t TASK_NUMBER_100MS =	((uint8_t)0);
uint8_t TASK_NUMBER_1S =	((uint8_t)0);


uint16_t taskCounter = 0;

voidFunctionTypeVoid table_task_1ms     [TASK_1MS_MAX];
voidFunctionTypeVoid table_task_10ms    [TASK_10MS_MAX];
voidFunctionTypeVoid table_task_100ms   [TASK_100MS_MAX];
voidFunctionTypeVoid table_task_1s      [TASK_1S_MAX];

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

inline void shift_task(ptr_voidFunctionTypeVoid ptrTask, uint8_t task_no, int8_t task_id)
{
    //shift the rest of tasks
    for(uint8_t idx = task_id; idx < task_no; idx++)
    {
        if(idx + 1 == task_no)
        {
            *(ptrTask + idx) = NULL;
        }
        else
        {
            *(ptrTask + idx) = *(ptrTask + idx + 1);
        }  
    }
}

int8_t scheduler_remove_task(scheduler_task_type tasktype, int8_t task_id)
{
    int8_t task_found = -1;
    switch(tasktype){
        case sch_type_task_1ms:
            if(task_id < TASK_NUMBER_1MS)
            {
                shift_task(table_task_1ms, TASK_1MS_MAX, task_id);
                task_found = task_id;
            }
        break;
        case sch_type_task_10ms:
            if(task_id < TASK_NUMBER_10MS)
            {
                shift_task(table_task_1ms, TASK_10MS_MAX, task_id);
                task_found = task_id;
            }
        break;
        case sch_type_task_100ms:
            if(task_id < TASK_NUMBER_100MS)
            {
                shift_task(table_task_1ms, TASK_100MS_MAX, task_id);
                task_found = task_id;
            }
        break;
        case sch_type_task_1s:
            if(task_id < TASK_NUMBER_1S)
            {
                shift_task(table_task_1ms, TASK_1S_MAX, task_id);
                task_found = task_id;
            }
        break;
        default:
        break;
    }

    return task_found;
}

int8_t scheduler_add_task(scheduler_task_type tasktype, voidFunctionTypeVoid task)
{
    int8_t task_id = -1;
    switch(tasktype){
        case sch_type_task_1ms:
            if(TASK_NUMBER_1MS < TASK_1MS_MAX){
                task_id = (int8_t)TASK_NUMBER_1MS;
                table_task_1ms[TASK_NUMBER_1MS++] = task;
            }
        break;
        case sch_type_task_10ms:
            if(TASK_NUMBER_10MS < TASK_10MS_MAX){
                task_id =  (int8_t)TASK_NUMBER_10MS;
                table_task_1ms[TASK_NUMBER_10MS++] = task;
            }
        break;
        case sch_type_task_100ms:
            if(TASK_NUMBER_100MS < TASK_100MS_MAX){
                task_id =  (int8_t)TASK_NUMBER_100MS;
                table_task_1ms[TASK_NUMBER_100MS++] = task;
            }
        break;
        case sch_type_task_1s:
            if(TASK_NUMBER_1S < TASK_1S_MAX){
                task_id =  (int8_t)TASK_NUMBER_1S;
                table_task_1ms[TASK_NUMBER_1S++] = task;
            }
        break;
        default:
        break;
    }

    return task_id;    
}

voidFunctionTypeVoid scheduler_getPointerTo1msTask()
{
    return task_1ms;
}


void task_1ms()
{  

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






