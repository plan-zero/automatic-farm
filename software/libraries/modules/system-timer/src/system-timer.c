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

#include "system-timer.h"

#define ADD_TIMER2_OVF_INT 1
#include "timer.h"


//theoretically this variable will overflow after 
//~ 4 years of running without restarts (used at 8Mhz - 2 years at 16Mhz)
//please consider this timeframe
uint32_t timer2_ovf_int = 0;
timer_instance _timer_2 = 2;

#define MS_ROUND_VALUE 500

//check the F_CPU to select the right prescaler
#if defined(F_CPU) && (F_CPU == 16000000UL)
    #define TIMER_OVF_US (uint32_t)16384
    #define TIMER_UNIT_US   (uint32_t)64
    timer_prescaler _prescaler = timer_prescaler_1024;
#elif defined(F_CPU) && (F_CPU == 8000000UL)
    #define TIMER_OVF_ERR       (uint32_t)89
    #define TIMER_SEC_PER_MS_ERR (uint32_t)16
    #define TIMER_OVF_US_CAL    (uint32_t)32678
    #define TIMER_OVF_US        (uint32_t)(TIMER_OVF_US_CAL + TIMER_OVF_ERR)
    #define TIMER_OVF_ERROR (uint32_t)19320
    #define TIMER_UNIT_US   (uint32_t)128
    timer_prescaler _prescaler = timer_prescaler_1024;
#elif defined(F_CPU) && (F_CPU == 4000000UL)
    #define TIMER_OVF_US (uint32_t)65536
    #define TIMER_UNIT_US   (uint32_t)256
    timer_prescaler _prescaler = timer_prescaler_8;
#elif defined(F_CPU) && (F_CPU == 2000000UL)
    #define TIMER_OVF_US (uint32_t)131072
    #define TIMER_UNIT_US   (uint32_t)512
    timer_prescaler _prescaler = timer_prescaler_8;
#elif defined(F_CPU) && (F_CPU == 1000000UL)
    #define TIMER_OVF_US (uint32_t)262144
    #define TIMER_UNIT_US   (uint32_t)1024
    timer_prescaler _prescaler = timer_prescaler_1;
#endif

void system_timer_init()
{
    OSCCAL = 175;
    timer_interrupt ti;
    ti.overflow_en = 1;
    timer_cfg tc = {
        timer_mode_fast_pwm,
        timer_normal_operationm,
        _prescaler,
        0,
        127,
        0,
        0,
        0,
        timer_8_bits,
        timer_ch_none,
        ti,
    };
    timer_init(_timer_2,tc);
}

void system_timer_start()
{
    timer_start(_timer_2,0);
}

system_timer_info system_timer_get_info(system_timer_timestamp timestamp)
{
    system_timer_info tmp = {0};

    uint32_t seconds = timestamp.timer_period / 31;
    uint32_t mili = ((timestamp.timer_period % 31) * TIMER_OVF_US  + timestamp.timer_ticks * TIMER_UNIT_US)/1000;
    mili += seconds * TIMER_SEC_PER_MS_ERR;
    seconds += mili / 1000; // add seconds from mili
    //update min
    uint32_t min = seconds / 60;
    seconds = seconds % 60;
    //update hours
    uint32_t hours = min / 60;
    min = min % 60;

    tmp.hour = hours;
    tmp.mili = mili % 1000;
    tmp.sec = seconds;
    tmp.min = min;

    return tmp;
}

uint32_t system_timer_getus(system_timer_timestamp _start, system_timer_timestamp _end)
{
    return (TIMER_OVF_US * (_end.timer_period - _start.timer_period) + TIMER_UNIT_US * _end.timer_ticks) - TIMER_UNIT_US * _start.timer_ticks;
}
uint32_t system_timer_getms(system_timer_timestamp _start, system_timer_timestamp _end)
{
    return ((TIMER_OVF_US * (_end.timer_period - _start.timer_period) + TIMER_UNIT_US * _end.timer_ticks) - TIMER_UNIT_US * _start.timer_ticks + MS_ROUND_VALUE) / 1000;
}

system_timer_timestamp system_timer_get_timestamp()
{
    system_timer_timestamp tmp;
    tmp.timer_period = timer2_ovf_int;
    tmp.timer_ticks = timer_get(_timer_2);
    
    return tmp;
}