// Copyright (C) 2020 Coding Night Romania
// 
// This file is part of automatic-farm.
// 
// automatic-farm is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// automatic-farm is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with automatic-farm.  If not, see <http://www.gnu.org/licenses/>.


/*
 * Scheduler.h
 *
 *  Created on: Feb 7, 2020
 *      Author: Adi
 */

#ifndef SCHEDULER_SCHEDULER_H_
#define SCHEDULER_SCHEDULER_H_

#include "stdint.h"

typedef enum{
    sch_type_task_1ms,
    sch_type_task_10ms,
    sch_type_task_100ms,
    sch_type_task_1s,
}scheduler_task_type;

typedef void (*voidFunctionTypeVoid)();


extern voidFunctionTypeVoid scheduler_getPointerTo1msTask(void);
int8_t scheduler_remove_task(scheduler_task_type tasktype, int8_t task_id);
int8_t scheduler_add_task(scheduler_task_type tasktype, voidFunctionTypeVoid task);
void scheduler_add_custom_period_task(voidFunctionTypeVoid task, uint16_t period);
void scheduler_clear_all_custom_period_task();
void scheduler_add_alarm(voidFunctionTypeVoid task, uint16_t time);

#endif /* SCHEDULER_SCHEDULER_H_ */
