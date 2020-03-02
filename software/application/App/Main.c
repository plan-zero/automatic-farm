/*
 * Main.c
 *
 *  Created on: Feb 7, 2020
 *      Author: Adi
 */

#include "Scheduler.h"
#include "timer.h"
#include <avr/io.h>
#include <avr/interrupt.h>

inline void MAIN_timer_config()
{
    timer_interrupt ti;
    ti.output_cmp_match_en = 1;
    timer_cfg tc = {
        timer_mode_ctc,
        timer_normal_operationm,
        timer_prescaler_64,
        0,
        123,
        0,
        0,
        0,
        timer_8_bits,
        timer_ch_none,
        ti,
    };
    timer_init(0,tc);
}


int  main()
{
    voidFunctionType oneMsTask = getPointerTo1msTask();
    
    DDRB |= 1;

    MAIN_timer_config();
    timer_register_callback(0, (timer_callback) oneMsTask, 1);
    timer_start(0,0);

    sei();

    while(1)
    {

    }


     
    return 0;
}