/*
 * Main.c
 *
 *  Created on: Feb 7, 2020
 *      Author: Adi
 */

#include "Scheduler.h"

int  main()
{
    voidFunctionType oneMsTask = getPointerTo1msTask();

    oneMsTask();
     
    return 0;
}