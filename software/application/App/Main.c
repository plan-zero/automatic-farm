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
#include "timer.h"
#include "interrupt_hw.h"

volatile uint8_t oneMsFlag = 0;

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

// We use timer on channel 0
void notify1ms(uint8_t ch)
{
    if (ch ==  0)
    {
        oneMsFlag += 1;
        if (oneMsFlag >= 10)
        {
            // TODO: WDG reset
        }
    }
}


int  main()
{
    voidFunctionTypeVoid oneMsTask = getPointerTo1msTask();

    MAIN_timer_config();
    // We use timer on channel 0
    timer_register_callback(0, (timer_callback) notify1ms, 1);
    timer_start(0,0);

    INT_GLOBAL_EN();

    while(1)
    {
        if (oneMsFlag != 0)
        {
            oneMsFlag--;
            oneMsTask();
        }
    }
    return 0;
}