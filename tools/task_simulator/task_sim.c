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

#include "stdio.h"
#include <stdlib.h>
#include <string.h>
#include <time.h> 

#define task_max 20
#define task_1ms_slots 4

int task_num = 0;
int task_slots = 10;

int task_count = 0;

int task_offset[task_max];
int task_period[task_max];
char * task_name[task_max];


void add_task(char * name, int period){
  
    task_period[task_count] = period;
    task_name[task_count] = (char*)malloc(sizeof(char) * strlen(name));
    strcpy(task_name[task_count], name);
    task_offset[task_count] = 0;
    task_count++;
}

void print_task(int task_entry)
{
    printf("task: %s %d \n", task_name[task_entry], task_period[task_entry]);
}

int get_max_period()
{
    int max = 0;
    for(int i = 0; i < task_count; i++)
        if (task_period[i] > max)
            max = task_period[i];

    return max;
}

int ** task_matrix = NULL;

void print_matrix()
{
    for(int i = 0; i < task_slots; i++)
    {
        for(int j = 0; j < task_num; j++)
        {
            printf("%d ",task_matrix[i][j]);
        }
        printf("\n");
    }
}


void generate_task_matrix()
{
    for(int i = 0; i < task_slots; i++)
    {
        for(int j = 0; j < task_num; j++)
        {
            for(int k = 0; k < task_count; k++)
            {
                if(i  % task_period[k] == 0){
                    task_matrix[i][k] = 1;
                }
            }
            printf("%d ",task_matrix[i][j]);
        }
        printf("\n");
    }
}
int max_shift = 10;
float calculate_load()
{
    float load = 0;
    for(int i = max_shift; i < task_slots; i++)
    {
        int slot_load = 0;
        for(int j = 0; j < task_num; j++)
        {
            //printf("%d ",task_matrix[i][j]);
            slot_load += task_matrix[i][j];
        }
        float fslot_load = ((float)slot_load / (float)task_1ms_slots);
        //printf("slot [%d]:%.2f \n",i, fslot_load);
        load += fslot_load;
    }
    //calculate global load:
    load = load / task_slots;
    printf("all: %.2f \n", load);

    return load;
}



void shift_task(int task_entry, int pos)
{
    for(int i = 0; i < task_slots; i++)
    {
        task_matrix[i][task_entry] = 0;
    }

    if(task_offset[task_entry] + pos >= 0 && task_offset[task_entry] + pos <= max_shift)
        task_offset[task_entry] += pos;

    for(int i = task_offset[task_entry]; i < task_slots; i++)
    {
        if(i % task_period[task_entry] == 0)
        {
            task_matrix[i][task_entry] = 1;
        }
    }
}

void apply_gradient(int * grad)
{
    for(int i = 0; i < task_count; i++)
    {
        shift_task(i, grad[i]);
    }
}

int gen_random() 
{ 
    return (rand() % 3) - 1; 
} 



int main()
{
    srand(time(0));

    add_task("task1", 3);
    add_task("task1", 3);
    add_task("task2", 2);
    add_task("task2", 2);


    task_num = task_count;
    task_slots = get_max_period() * 15;

    int * gradient = malloc(sizeof(int)* task_count);
    for(int i = 0;  i < task_count; i++)
        gradient[i] = 0;

    task_matrix = malloc( sizeof(int*) * task_slots );
    for (int i = 0; i < task_slots; i++)
        task_matrix[i] = malloc( sizeof(int) * task_num );

    generate_task_matrix();

    float initial_load = calculate_load();
    float new_load = 0;
    float error = 0;
    float min = 100;
    int * rezult = malloc(sizeof(int)* task_count);

    //generate a gradient
    int iterations = 500;
    for(int i = 0; i < iterations; i++)
    {
        for(int j = 0; j < task_count; j++)
        {
            gradient[j] = gen_random();
            printf("%d ", gradient[j]);
        }
        printf("\n");
        apply_gradient(gradient);
        new_load = calculate_load();
        error = (new_load - initial_load);
        initial_load = new_load;
        if(min > new_load)
        {
            min = new_load;
            for(int j = 0; j < task_count; j++)
            {
                rezult[j] = task_offset[j];
            }
        }
        printf("err: %f \n", error);
        while(error < 0)
        {
            apply_gradient(gradient);
            new_load = calculate_load();
            error = (new_load - initial_load);
            initial_load = new_load;
            printf("err: %f \n", error);
        }
    }

    calculate_load();
    print_matrix();
    printf("\n");
    for(int j = 0; j < task_count; j++)
        printf("%d ", rezult[j]);
    printf("\n");


    return 0;
}