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


#ifndef _TIMER_H
#define _TIMER_H
#include "stdint.h"

typedef enum{
    timer_noinit,
    timer_ready,
    timer_stopped,
    timer_running,
    timer_err_inv_type,
}timer_status;

typedef uint8_t timer_instance;
typedef void (*timer_callback)(timer_instance);

typedef enum{
    timer_8_bits,
    timer_16_bits,
}timer_type;

typedef enum{
    timer_ch_a,
    timer_ch_b,
    timer_ch_none,
}timer_ch;

typedef enum{
  timer_mode_normal,
  timer_mode_pwm,
  timer_mode_ctc,
  timer_mode_fast_pwm,
  //suuported just for 16bit timers
  timer_mode_pwm_8_bit,
  timer_mode_pwm_9_bit,
  timer_mode_pwm_10_bit,
  timer_mode_fast_pwm_8_bit,
  timer_mode_fast_pwm_9_bit,
  timer_mode_fast_pwm_10_bit,
}timer_mode;

typedef enum{
  timer_prescaler_1,
  timer_prescaler_8,
  timer_prescaler_32,
  timer_prescaler_64,
  timer_prescaler_128,
  timer_prescaler_256,
  timer_prescaler_1024,
  timer_prescaler_ext_1,
  timer_prescaler_ext_2
}timer_prescaler;

typedef enum{
    timer_normal_operationm,
    timer_toggle_compare_match,
    timer_clear_compare_match,
    timer_set_compare_match,
}timer_compare_behaviour;

typedef enum{
    timer_cb_on_cmp_match,
    timer_cb_on_ovf,
}timer_cb_type;

typedef union{
    struct{
        uint8_t overflow_en : 1;
        uint8_t output_cmp_match_en : 1;
        uint8_t output_cmp_a_match_en : 1;
        uint8_t output_cmp_b_match_en : 1;
        uint8_t input_cmp_match_en : 1;
    };
    uint8_t interrupt;
}timer_interrupt;

typedef union{
    struct{
        uint8_t overflow : 1;
        uint8_t output_cmp_match : 1;
        uint8_t output_cmp_a_match : 1;
        uint8_t output_cmp_b_match : 1;
        uint8_t input_cmp_match : 1;
    };
    uint8_t flags;
}timer_flags;

typedef struct{
    timer_mode mode;
    timer_compare_behaviour cmp_behaviour;
    timer_prescaler prescaler;
    uint16_t initial_count_val;
    uint8_t initial_output_cmp_val;
    uint16_t initial_output_cmp_val_ch_a;
    uint16_t initial_output_cmp_val_ch_b;
    uint16_t initial_input_cmp_val;
    timer_type type;
    timer_ch channel;
    timer_interrupt interrupt;
}timer_cfg;

#if defined(ADD_TIMER2_OVF_INT) && (ADD_TIMER2_OVF_INT == 1)
#include "interrupt_hw.h"
extern uint32_t timer2_ovf_int;
INTERRUPT_ROUTINE(IRQ_TIMER2_OVF)
{
    timer2_ovf_int++;
}
#endif

timer_status timer_init(timer_instance inst, timer_cfg cfg);
void timer_register_callback(timer_instance inst, timer_callback cb, uint8_t period );
void timer_start(timer_instance inst, uint16_t initial_value);
void timer_reset(timer_instance inst);
uint16_t timer_get(timer_instance inst);

#endif