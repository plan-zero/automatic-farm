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


#include "timer.h"
#include "stdlib.h"

#include "interrupt_hw.h"
#include "timer_hw.h"

#define EN_TIMER0_CMP_IRQ

timer_callback trigger_cb[TIMER_COUNT] = {NULL, NULL, NULL};
uint8_t cb_period[TIMER_COUNT] = {0, 0, 0};
uint8_t cb_count[TIMER_COUNT] = {0, 0, 0};
uint8_t cb_count_ch[2] = {0, 0};
timer_status timer_internal_state[TIMER_COUNT] = {timer_noinit,timer_noinit,timer_noinit};
//define ISRs
#ifdef EN_TIMER0_CMP_IRQ
INTERRUPT_ROUTINE(TIMER0_COMPA_vect)
{
    cb_count[TIMER_0]++;
    if(cb_count[TIMER_0] == cb_period[TIMER_0])
    {
        cb_count[TIMER_0] = 0;
        trigger_cb[TIMER_0](TIMER_0);
    }
}
#endif
#ifdef EN_TIMER0_OVF_IRQ
INTERRUPT_ROUTINE(IRQ_TIMER0_OVF)
{
    cb_count[TIMER_0]++;
    if(cb_count[TIMER_0] == cb_period[TIMER_0])
    {
        cb_count[TIMER_0] = 0;
        trigger_cb[TIMER_0](TIMER_0);
    }
}
#endif
#ifdef EN_TIMER1_OVF_IRQ
INTERRUPT_ROUTINE(IRQ_TIMER1_OVF)
{
    cb_count[TIMER_1]++;
    if(cb_count[TIMER_1] == cb_period[TIMER_1])
    {
        cb_count[TIMER_1] = 0;
        trigger_cb[TIMER_1](TIMER_1);
    }
}
#endif
#ifdef EN_TIMER1A_CMP_IRQ
INTERRUPT_ROUTINE(IRQ_TIMER1A_CMP)
{
    cb_count_ch[0]++;
    if(cb_count_ch[0] == cb_period[TIMER_1])
    {
        cb_count[TIMER_1] = 0;
        trigger_cb[TIMER_1](TIMER_1);
    }
}
#endif
#ifdef EN_TIMER1B_CMP_IRQ
INTERRUPT_ROUTINE(IRQ_TIMER1B_CMP)
{
    cb_count_ch[1]++;
    if(cb_count_ch[1] == cb_period[TIMER_1])
    {
        cb_count[TIMER_1] = 0;
        trigger_cb[TIMER_1](TIMER_1);
    }
}
#endif
#ifdef EN_TIMER2_CMP_IRQ
INTERRUPT_ROUTINE(IRQ_TIMER2_CMP)
{
    cb_count[TIMER_2]++;
    if(cb_count[TIMER_2] == cb_period[TIMER_2])
    {
        cb_count[TIMER_2] = 0;
        trigger_cb[TIMER_2](TIMER_2);
    }
}
#endif


timer_prescaler cached_timer_pres[TIMER_COUNT];

timer_status timer_init(timer_instance inst, timer_cfg cfg)
{

    switch (cfg.mode)
    {
    case timer_mode_normal:
        TIMER_FUNCTIONS(MODE_NORMAL_MODE, inst);
        break;
    case timer_mode_pwm:
        TIMER_FUNCTIONS(MODE_PWM, inst);
        break;
    case timer_mode_fast_pwm:
        TIMER_FUNCTIONS(MODE_FAST_PWM, inst);
        break;
    case timer_mode_ctc:
        TIMER_FUNCTIONS(MODE_CTC, inst);
        break;
    default:
        break;
    }
    switch (cfg.cmp_behaviour)
    {
    case timer_normal_operationm:
        if(cfg.channel == timer_ch_a)
            TIMER_FUNCTIONS(NORMAL_OPERATION, inst, TIMER_CH_A);
        else
            TIMER_FUNCTIONS(NORMAL_OPERATION, inst, TIMER_CH_B);
        break;
    case timer_set_compare_match:
        if(cfg.channel == timer_ch_a)
            TIMER_FUNCTIONS(SET_CMP_MATCH, inst, TIMER_CH_A);
        else
            TIMER_FUNCTIONS(SET_CMP_MATCH, inst, TIMER_CH_B);
        break;
    case timer_clear_compare_match:
        if(cfg.channel == timer_ch_a)
            TIMER_FUNCTIONS(CLEAR_CMP_MATCH, inst, TIMER_CH_A);
        else
            TIMER_FUNCTIONS(CLEAR_CMP_MATCH, inst, TIMER_CH_B);
        break;
    case timer_toggle_compare_match:
        if(cfg.channel == timer_ch_a)
            TIMER_FUNCTIONS(TOGGLE_CMP_MATCH, inst, TIMER_CH_A);
        else
            TIMER_FUNCTIONS(TOGGLE_CMP_MATCH, inst, TIMER_CH_B);
        break;
    default:
        break;
    }
    
    cached_timer_pres[inst] = cfg.prescaler;
    TIMER_FUNCTIONS(CLOCK_DISABLED,inst);
 
    if(cfg.interrupt.output_cmp_match_en)
    {
        if(cfg.channel == timer_ch_a)
            TIMER_FUNCTIONS(SET_OCMP_INT, inst, TIMER_CH_A);
        else
            TIMER_FUNCTIONS(SET_OCMP_INT, inst, TIMER_CH_B);
    } 
    if(cfg.interrupt.overflow_en)
        TIMER_FUNCTIONS(SET_OVF_INT, inst);

    TIMER_FUNCTIONS(SET_COUNTER, inst, cfg.initial_count_val);
    if(cfg.channel == timer_ch_a)
        TIMER_FUNCTIONS(SET_OUTPUT_CMP,inst, cfg.initial_output_cmp_val_ch_a, TIMER_CH_A);
    else if(cfg.channel == timer_ch_b)
        TIMER_FUNCTIONS(SET_OUTPUT_CMP,inst, cfg.initial_output_cmp_val_ch_b, TIMER_CH_B);
    else
        TIMER_FUNCTIONS(SET_OUTPUT_CMP,inst, cfg.initial_output_cmp_val, TIMER_CH_A);

    timer_internal_state[inst] = timer_ready;
    return timer_ready;
}

void timer_register_callback(timer_instance inst, timer_callback cb, uint8_t period )
{
    if(timer_internal_state[inst] == timer_ready || timer_internal_state[inst] == timer_stopped)
    {
        trigger_cb[inst] = cb;
        cb_period[inst] = period;
    }
}

void timer_set_cmp_value(timer_instance inst, timer_ch channel, uint16_t value)
{
    if(channel == timer_ch_a)
        TIMER_FUNCTIONS(SET_OUTPUT_CMP, inst, value, TIMER_CH_A);
    else if(channel == timer_ch_b)
        TIMER_FUNCTIONS(SET_OUTPUT_CMP, inst, value, TIMER_CH_B);
    else
        TIMER_FUNCTIONS(SET_OUTPUT_CMP, inst, (uint8_t)value, TIMER_CH_A);
}

void timer_start(timer_instance inst, uint16_t initial_value)
{
    if(timer_internal_state[inst] == timer_ready || timer_internal_state[inst] == timer_stopped)
    {
        switch (cached_timer_pres[inst])
        {
        case timer_prescaler_1:
            if(TIMER_8_ASYNC == TIMER_GET_TYPE_SYNC(inst))
                TIMER_FUNCTIONS(ASYNC_CLOCK_PRE_1,inst);
            else
                TIMER_FUNCTIONS(CLOCK_PRE_1,inst);
            break;
        case timer_prescaler_8:
            if(TIMER_8_ASYNC == TIMER_GET_TYPE_SYNC(inst))
                TIMER_FUNCTIONS(ASYNC_CLOCK_PRE_8,inst);
            else
                TIMER_FUNCTIONS(CLOCK_PRE_8,inst);
            break;
        case timer_prescaler_32:
            TIMER_FUNCTIONS(ASYNC_CLOCK_PRE_32,inst);
            break;
        case timer_prescaler_64:
            if(TIMER_8_ASYNC == TIMER_GET_TYPE_SYNC(inst))
                TIMER_FUNCTIONS(ASYNC_CLOCK_PRE_64,inst);
            else
                TIMER_FUNCTIONS(CLOCK_PRE_64,inst);
            break;
        case timer_prescaler_128:
            TIMER_FUNCTIONS(ASYNC_CLOCK_PRE_32,inst);
            break;
        case timer_prescaler_256:
            if(TIMER_8_ASYNC == TIMER_GET_TYPE_SYNC(inst))
                TIMER_FUNCTIONS(ASYNC_CLOCK_PRE_256,inst);
            else
                TIMER_FUNCTIONS(CLOCK_PRE_256,inst);
            break;
        case timer_prescaler_1024:
            if(TIMER_8_ASYNC == TIMER_GET_TYPE_SYNC(inst))
                TIMER_FUNCTIONS(ASYNC_CLOCK_PRE_1024,inst);
            else
                TIMER_FUNCTIONS(CLOCK_PRE_1024,inst);
            break;
        default:
            break;
        }
        timer_internal_state[inst] = timer_running;
    }
}
void timer_stop(timer_instance inst)
{
    if(timer_internal_state[inst] == timer_running)
    {
        TIMER_FUNCTIONS(CLOCK_DISABLED,inst);
        timer_internal_state[inst] = timer_stopped;
    }
}
void timer_set_prescaler(timer_instance inst, timer_prescaler prescaler)
{
    cached_timer_pres[inst] = prescaler;
}

void timer_reset(timer_instance inst)
{
    TIMER_FUNCTIONS(SET_COUNTER, inst, 0);
}
uint16_t timer_get(timer_instance inst)
{
    return (uint16_t)TIMER_FUNCTIONS(GET_COUNTER, inst);
}