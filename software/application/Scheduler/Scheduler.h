/*
 * Scheduler.h
 *
 *  Created on: Feb 7, 2020
 *      Author: Adi
 */

#ifndef SCHEDULER_SCHEDULER_H_
#define SCHEDULER_SCHEDULER_H_

typedef void (*voidFunctionType)(int);
typedef void (*voidFunctionTypeVoid)();
extern voidFunctionType getPointerTo1msTask(void);

#endif /* SCHEDULER_SCHEDULER_H_ */
