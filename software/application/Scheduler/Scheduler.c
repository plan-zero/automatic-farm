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
#include "stdlib.h"

typedef void (**ptr_voidFunctionTypeVoid)();

#define TASK_1MS_MAX        ((uint8_t)8)
#define TASK_10MS_MAX       ((uint8_t)8)
#define TASK_100MS_MAX      ((uint8_t)8)
#define TASK_1S_MAX         ((uint8_t)8)

uint8_t taskNumber1ms   =	((uint8_t)0);
uint8_t taskNumber10ms  =	((uint8_t)0);
uint8_t taskNumber100ms =	((uint8_t)0);
uint8_t taskNumber1s    =	((uint8_t)0);


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
typedef struct CustomPeriodTask
{
    voidFunctionTypeVoid pFunction;
    uint16_t period;
    uint16_t counter;
}CustomPeriodTask;
typedef struct Alarm
{
    voidFunctionTypeVoid pFunction;
    uint16_t counter;
}Alarm;




Timer timer1ms      = {table_task_1ms,      0};
Timer timer10ms     = {table_task_10ms,     0};
Timer timer100ms    = {table_task_100ms,    0};
Timer timer1s       = {table_task_1s,       0};


CustomPeriodTask* customTaskList = NULL;
uint8_t customTaskListCount = 0;

Alarm* alarmList = NULL;
uint8_t alarmListCount = 0;



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
            if(task_id < taskNumber1ms)
            {
                shift_task(table_task_1ms, TASK_1MS_MAX, task_id);
                task_found = task_id;
                taskNumber1ms--;
            }
        break;
        case sch_type_task_10ms:
            if(task_id < taskNumber10ms)
            {
                shift_task(table_task_10ms, TASK_10MS_MAX, task_id);
                task_found = task_id;
                taskNumber10ms--;
            }
        break;
        case sch_type_task_100ms:
            if(task_id < taskNumber100ms)
            {
                shift_task(table_task_100ms, TASK_100MS_MAX, task_id);
                task_found = task_id;
                taskNumber100ms--;
            }
        break;
        case sch_type_task_1s:
            if(task_id < taskNumber1s)
            {
                shift_task(table_task_1s, TASK_1S_MAX, task_id);
                task_found = task_id;
                taskNumber1s--;
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
            if(taskNumber1ms < TASK_1MS_MAX){
                task_id = (int8_t)taskNumber1ms;
                table_task_1ms[taskNumber1ms++] = task;
            }
        break;
        case sch_type_task_10ms:
            if(taskNumber10ms < TASK_10MS_MAX){
                task_id =  (int8_t)taskNumber10ms;
                table_task_10ms[taskNumber10ms++] = task;
            }
        break;
        case sch_type_task_100ms:
            if(taskNumber100ms < TASK_100MS_MAX){
                task_id =  (int8_t)taskNumber100ms;
                table_task_100ms[taskNumber100ms++] = task;
            }
        break;
        case sch_type_task_1s:
            if(taskNumber1s < TASK_1S_MAX){
                task_id =  (int8_t)taskNumber1s;
                table_task_1s[taskNumber1s++] = task;
            }
        break;
        default:
        break;
    }

    return task_id;    
}


void scheduler_add_custom_period_task(voidFunctionTypeVoid task, uint16_t period)
{
    if (customTaskListCount == 0)
    {
        customTaskList = (CustomPeriodTask *)malloc(sizeof(CustomPeriodTask));
    }
    else
    {     
        customTaskList = realloc(customTaskList, (customTaskListCount + 1) * sizeof(CustomPeriodTask));        
    }

    customTaskList[customTaskListCount].pFunction = task;
    customTaskList[customTaskListCount].period = period;
    customTaskList[customTaskListCount].counter = period;

    customTaskListCount++;
}
void scheduler_clear_all_custom_period_task()
{
    free(customTaskList);
    customTaskList = NULL;
    customTaskListCount = 0;
}

void scheduler_add_alarm(voidFunctionTypeVoid task, uint16_t time)
{
    if (alarmListCount == 0)
    {        
        alarmList = (Alarm *)malloc(sizeof(Alarm));
    }
    else
    {
        alarmList = realloc(alarmList, (alarmListCount + 1) * sizeof(Alarm));        
    }
    alarmList[alarmListCount].pFunction = task;
    alarmList[alarmListCount].counter = time;
    alarmListCount++;
}


voidFunctionTypeVoid scheduler_getPointerTo1msTask()
{
    return task_1ms;
}





void task_1ms()
{  

    // Call all the 1ms tasks
    for (uint8_t taskIterator = 0; taskIterator < taskNumber1ms; taskIterator++)
    {
        timer1ms.table[taskIterator]();
    }

    // Call the 10ms tasks in their respective slots
    if(timer10ms.slot > 0)
    {
        timer10ms.table[taskNumber10ms - timer10ms.slot]();       
        timer10ms.slot --;
    }
    if (taskCounter % 10 == 0)
    {
        timer10ms.slot = taskNumber10ms;        
    }


    // Call the 100ms tasks in their respective slots
    if (taskCounter % 10 == 0)
    {
        if(timer100ms.slot > 0)
        {
            timer100ms.table[taskNumber100ms - timer100ms.slot]();                
            timer100ms.slot --;
        }
    }  
    if (taskCounter % 100 == 0)
    {
        timer100ms.slot = taskNumber100ms;
    }
   
   
    // Call the 1 s tasks in their respective slots
    if ((taskCounter + 1) % 100 == 0)
    {
        if(timer1s.slot > 0)
        {
            timer1s.table[taskNumber1s - timer1s.slot]();       
            timer1s.slot --;
        }        
    }
    if (taskCounter % 1000 == 0)
    {
        timer1s.slot = taskNumber1s;
        taskCounter = 0;
    }

    taskCounter++;

    // Call the custom period tasks if needed
    if (customTaskList != NULL)
    {
        for (uint8_t taskIterator = 0; taskIterator < customTaskListCount; taskIterator++)
        {
            customTaskList[taskIterator].counter--;    
            if (customTaskList[taskIterator].counter == 0)
            {
                customTaskList[taskIterator].counter = customTaskList[taskIterator].period;
                customTaskList[taskIterator].pFunction();
            }
        }
    }


    // Call the alarm requests if needed
    if(alarmList != NULL)
    {
        for (uint8_t alarmIterator = 0; alarmIterator < alarmListCount; alarmIterator++)
        {
            alarmList[alarmIterator].counter--; 
            if (alarmList[alarmIterator].counter == 0)
            {
                alarmList[alarmIterator].pFunction();
                if (alarmListCount > 1) 
                {           
                    Alarm* tempAlarmList;
                    tempAlarmList = (Alarm *)malloc((alarmListCount - 1) * sizeof(Alarm));
                    uint8_t tempIterator = 0;
                    for (uint8_t it = 0; it < alarmListCount; it ++)
                    {
                        if (it != alarmIterator)
                        {
                            tempAlarmList[tempIterator].counter = alarmList[it].counter;
                            tempAlarmList[tempIterator].pFunction = alarmList[it].pFunction;
                            tempIterator++;
                        }
                    }
                    free(alarmList);
                    alarmList = tempAlarmList;
                    alarmIterator--;
                    alarmListCount--;
                }
                else
                {
                    free(alarmList);
                    alarmList = NULL;
                }
            }        
        }
    }

}


// TODO: adaugat alarma (dai sa iti apeleze o functie dupa x secunde)







