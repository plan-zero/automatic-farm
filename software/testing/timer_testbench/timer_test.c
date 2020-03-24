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


#include "uart.h"
#include "timer_hw.h"
#include "timer.h"

typedef enum{
    funct_timer_mode_normal_mode,
    funct_timer_mode_pwm,
    funct_timer_mode_ctc,
    funct_timer_fast_pwm,
    funct_timer_set_ovf_int,
    funct_timer_get_ovf_int,
    funct_timer_set_ocmp_int,
    funct_timer_get_ocmp_int,
    funct_timer_clear_ovf_int_flag,
    funct_timer_get_ovf_int_flag,
    funct_timer_clear_ocmp_int_flag,
    funct_timer_get_ocmp_init_flag,
    funct_timer_normal_operation,
    funct_timer_toggle_cmp_match,
    funct_timer_clear_cmp_match,
    funct_timer_set_cmp_match,
    funct_timer_clock_disabled,
    funct_timer_clock_pre_1,
    funct_timer_clock_pre_8,
    funct_timer_clock_pre_64,
    funct_timer_clock_pre_256,
    funct_timer_clock_pre_1024,
    funct_timer_clock_pre_T0RE,
    funct_timer_mode_pwm_8bit,
    funct_timer_mode_pwm_9bit,
    funct_timer_mode_10bit,
    funct_timer_mode_ctc_1,
    funct_timer_mode_fastpwm_8bit,
    funct_timer_mode_fastpwm_9bit,
    funct_timer_mode_fastpwm_10bit,
    funct_timer_mode_pwm_pfc1,
    funct_timer_mode_pwm_pfc2,
    funct_timer_mode_pwm_pc1,
    funct_timer_mode_pwm_pc2,
    funct_timer_mode_ctc_2,
    funct_timer_mode_fastpwm_1,
    funct_timer_mode_fastpwm_2,
    funct_timer_mode_set_ic_int,
    funct_timer_mode_get_ic_int,
    funct_timer_get_output_cmp,
    funct_timer_set_output_cmp,
    funct_timer_get_input_capture,
    funct_timer_set_input_capture,


}funct_timer;

void exec_test(funct_timer funct, uint8_t inst, uint8_t reg_initial, uint8_t expected, volatile uint8_t * reg)
{
    uint8_t register_value = 0;
    *reg = reg_initial;

    switch (funct)
    {
    case funct_timer_mode_normal_mode:
        uart_printString("Execute MODE_NORMAL_MODE",1);
        TIMER_FUNCTIONS(MODE_NORMAL_MODE, inst);
        break;
    case funct_timer_mode_pwm:
        uart_printString("Execute MODE_PWM",1);
        TIMER_FUNCTIONS(MODE_PWM, inst);
        break;
    case funct_timer_mode_ctc:
        uart_printString("Execute MODE_PWM",1);
        TIMER_FUNCTIONS(MODE_CTC, inst);
        break;   
    case funct_timer_fast_pwm:
        uart_printString("Execute MODE_FAST_PWM",1);
        TIMER_FUNCTIONS(MODE_FAST_PWM, inst);
        break;
    case funct_timer_set_ovf_int:
        uart_printString("Execute SET_OVF_INT",1);
        TIMER_FUNCTIONS(SET_OVF_INT, inst);
        break;
    case funct_timer_get_ovf_int:
        uart_printString("Execute GET_OVF_INT",1);
        TIMER_FUNCTIONS(GET_OVF_INT, inst);
        break;
    case funct_timer_set_ocmp_int:
        uart_printString("Execute SET_OCMP_INT",1);
        TIMER_FUNCTIONS(SET_OCMP_INT, inst, TIMER_CH_A);
        break;
    case funct_timer_get_ocmp_int:
        uart_printString("Execute GET_OCMP_INT",1);
        TIMER_FUNCTIONS(GET_OCMP_INT, inst, TIMER_CH_A);
        break;
    case funct_timer_clear_ovf_int_flag:
        uart_printString("Execute CLEAR_OVF_INT_FLAG",1);
        TIMER_FUNCTIONS(CLEAR_OVF_INT_FLAG, inst);
        break;
    case funct_timer_get_ovf_int_flag:
        uart_printString("Execute GET_OVF_INT_FLAG",1);
        TIMER_FUNCTIONS(GET_OVF_INT_FLAG, inst);
        break;
    case funct_timer_clear_ocmp_int_flag:
        uart_printString("Execute CLEAR_OCMP_INT_FLAG",1);
        TIMER_FUNCTIONS(CLEAR_OCMP_INT_FLAG, inst, TIMER_CH_A);
        break; 
    default:
        break;
    }
    

    register_value = *reg;
    uart_printString("Expected value:",0);
    uart_printRegister(expected);
    uartNewLine();
    uart_printString("Got:",0);
    uart_printRegister(register_value);
    uartNewLine();
    if(expected == register_value)
        uart_printString("PASS", 1);
    else
        uart_printString("FAIL", 1);
}

int main()
{

    uart_init(UART_250000BAUD, UART_8MHZ, UART_PARITY_NONE);
    uartNewLine();
    uart_printString("Timer testing env", 1);

/*
    exec_test(  funct_timer_mode_normal_mode,   //timer function
                TIMER_0,                        //timer instance
                0x48,                           //timer's reg init value
                0,                              //expected result
                &TCCR0A );                       //target register

    exec_test(  funct_timer_mode_normal_mode,   //timer function
            TIMER_1,                        //timer instance
            0x3,                           //timer's reg init value
            0,                              //expected result
            &TCCR1A );                       //target register

    exec_test(  funct_timer_mode_normal_mode,   //timer function
            TIMER_2,                        //timer instance
            0x48,                           //timer's reg init value
            0,                              //expected result
            &TCCR2A );                       //target register
*/

    
    while(1);

    return 0;
}

