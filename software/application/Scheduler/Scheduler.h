/*
 * Scheduler.h
 *
 *  Created on: Feb 7, 2020
 *      Author: Adi
 */

#ifndef SCHEDULER_SCHEDULER_H_
#define SCHEDULER_SCHEDULER_H_

typedef enum{
    sch_type_task_1ms,
    sch_type_task_10ms,
    sch_type_task_100ms,
    sch_type_task_1s,
}scheduler_task_type;

//typedef void (*voidFunctionType)(int);
typedef void (*voidFunctionTypeVoid)();
extern voidFunctionTypeVoid getPointerTo1msTask(void);

#endif /* SCHEDULER_SCHEDULER_H_ */
