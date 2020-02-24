#ifndef _TIMER_H
#define _TIMER_H

#include "timer_hw.h"

typedef enum{
    timer_8_bits,
    timer_16_bits,
}timer_type;

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
  timer_prescaler_64,
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
    uint8_t instance;
    timer_interrupt interrupt;
    timer_flags flags;
}timer_cfg;

void timer_init(timer_cfg cfg);

#endif