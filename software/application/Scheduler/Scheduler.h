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
void scheduler_add_custom_period_task(voidFunctionTypeVoid task, uint_least16_t period);
void scheduler_clear_all_custom_period_task();

#endif /* SCHEDULER_SCHEDULER_H_ */
