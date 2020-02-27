#include "timer.h"
#include "stdint.h"

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
        TIMER_FUNCTIONS(SET_OUTPUT_CMP,inst, cfg.initial_output_cmp_val_ch_b, TIMER_CH_A);

    return timer_ok;
}

void timer_register_callback(timer_instance inst, timer_interrupt intcfg, timer_callback cb )
{

}
void timer_start(timer_instance inst, uint16_t initial_value)
{
    switch (cached_timer_pres[inst])
    {
    case timer_prescaler_1:
        TIMER_FUNCTIONS(CLOCK_PRE_1,inst);
        break;
    case timer_prescaler_8:
        TIMER_FUNCTIONS(CLOCK_PRE_8,inst);
        break;
    case timer_prescaler_64:
        TIMER_FUNCTIONS(CLOCK_PRE_64,inst);
        break;
    case timer_prescaler_256:
        TIMER_FUNCTIONS(CLOCK_PRE_256,inst);
        break;
    case timer_prescaler_1024:
        TIMER_FUNCTIONS(CLOCK_PRE_1024,inst);
        break;
    default:
        break;
    }
}
void timer_stop(timer_instance inst)
{
    TIMER_FUNCTIONS(CLOCK_DISABLED,inst);
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