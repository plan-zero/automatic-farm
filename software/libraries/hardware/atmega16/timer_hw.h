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


#ifndef _TIMER_HW_H
#define _TIMER_HW_H

#include <avr/io.h>


//timer 0 instance
#define TIMER_0     0
//timer 1 instance
#define TIMER_1     1
//timer 2 instance
#define TIMER_2     2
//totla number of instances
#define TIMER_COUNT 3

//timer type 8BITS
#define TIMER_8_BITS 99
//tiemr type 16BITS
#define TIMER_16_BITS 98
#define TIMER_8_ASYNC   100
#define TIMER_8_SYNC    101

//return the timer type based on instance parameter (can be used at runtime)
#define TIMER_GET_TYPE(inst) ( inst == TIMER_0 || inst == TIMER_2 ) ? TIMER_8_BITS : TIMER_16_BITS
#define TIMER_GET_TYPE_SYNC(inst) ((inst == TIMER_2 ) ? TIMER_8_ASYNC : TIMER_8_SYNC )

//timer channel A - used just for 16BITs timers
#define TIMER_CH_A   A
//timer channel B - used just for 16BITs timers
#define TIMER_CH_B   B


//just for internal usage: register name macro extension 2
#define __NAME(reg,inst,ch, ...)    reg ## inst ## ch ## __VA_ARGS__
//just for internal usage: register name macro extension 1
#define _NAME(reg,inst, ...)        __NAME(reg,inst,__VA_ARGS__)
//just for internal usage: register name macro extension
#define NAME(register, ...)         _NAME(register, ## __VA_ARGS__)


//******general timers macros for interrupt flags

//set timer overflow interrupt
#define _TIMER8_SET_OVF_INT(inst, ...)                 TIMSK |= _BV(NAME(TOIE, inst))
//get timer overflow
#define _TIMER8_GET_OVF_INT(inst, ...)                 (TIMSK >> _BV(NAME(TOIE, inst))) & 0x1

#define _TIMER16_SET_OVF_INT(inst, ...)                 TIMSK |= _BV(NAME(TOIE, inst))
//get timer overflow
#define _TIMER16_GET_OVF_INT(inst, ...)                 (TIMSK >> _BV(NAME(TOIE, inst))) & 0x1



//set timer output compare match interrupt
#define _TIMER8_SET_OCMP_INT(inst, ...)            TIMSK |= _BV(NAME(OCIE,inst))
//get timer output compare match interrupt
#define _TIMER8_GET_OCMP_INT(inst, ...)            (TIMSK >> _BV(NAME(OCIE,inst))) & 0x1
//set timer output compare match interrupt
#define _TIMER16_SET_OCMP_INT(inst, ...)           TIMSK |= _BV(NAME(OCIE,inst, __VA_ARGS__))
//get timer output compare match interrupt
#define _TIMER16_GET_OCMP_INT(inst, ...)           (TIMSK >> _BV(NAME(OCIE,inst, __VA_ARGS__))) & 0x1


//clear timer overflow interrupt flag
#define _TIMER8_CLEAR_OVF_INT_FLAG(inst, ...)           TIFR |= _BV(NAME(TOV, inst))
//read timer overflow interrupt flag
#define _TIMER8_GET_OVF_INT_FLAG(inst, ...)             (TIFR >> _BV(NAME(TOV, inst))) & 0x1
//clear timer overflow interrupt flag
#define _TIMER16_CLEAR_OVF_INT_FLAG(inst, ...)          TIFR |= _BV(NAME(TOV, inst))
//read timer overflow interrupt flag
#define _TIMER16_GET_OVF_INT_FLAG(inst, ...)            (TIFR >> _BV(NAME(TOV, inst))) & 0x1



//clear timer output compare match interrupt flag
#define _TIMER8_CLEAR_OCMP_INT_FLAG(inst, ...)     TIFR |= _BV(NAME(OCF,inst))
//clear read output compare match interrupt flag
#define _TIMER8_GET_OCMP_INT_FLAG(inst, ...)       (TIFR >> _BV(NAME(OCF,inst))) & 0x1
//clear timer output compare match interrupt flag
#define _TIMER16_CLEAR_OCMP_INT_FLAG(inst, ...)    TIFR |= _BV(NAME(OCF,inst,__VA_ARGS__))
//clear read output compare match interrupt flag
#define _TIMER16_GET_OCMP_INT_FLAG(inst, ...)      (TIFR >> _BV(NAME(OCF,inst,__VA_ARGS__))) & 0x1


#define _TIMER16_SET_IC_INT(inst)                 TIMSK |= _BV(NAME(TICIE, inst))
#define _TIMER16_GET_IC_INT(inst)                 (TIMSK >> _BV(NAME(TICIE, inst))) & 0x1

#define _TIMER16_CLEAR_IC_INT_FLAG(inst)          TIMSK |= _BV(NAME(ICF, inst))
#define _TIMER16_GET_IC_INT_FLAG(inst)            (TIMSK >> _BV(NAME(ICF, inst))) & 0x1

//******8BIT timer macros
/*
    This is used just by TIMER0 & TIMER2
    MODE    WGMx1   WGMx0   NOTE
    0       0       0       Normal port operation
    1       0       1       PWM
    2       1       0       CTC
    3       1       1       Fast PWM
*/            
#define _TIMER8_MODE_NORMAL_MODE(inst, ...)       NAME(TCCR, inst) &= ~(_BV(NAME(WGM,inst,0)) | _BV(NAME(WGM,inst,1)))
#define _TIMER8_MODE_PWM(inst, ...)               NAME(TCCR, inst) = ( NAME(TCCR, inst) & ~_BV(NAME(WGM,inst,1)) ) | _BV(NAME(WGM,inst,0))
#define _TIMER8_MODE_CTC(inst, ...)               NAME(TCCR, inst) = ( NAME(TCCR, inst) & ~_BV(NAME(WGM,inst,0)) ) | _BV(NAME(WGM,inst,1))
#define _TIMER8_MODE_FAST_PWM(inst, ...)          NAME(TCCR, inst) |= _BV(NAME(WGM,inst,0)) | _BV(NAME(WGM,inst,1))


/*            
    MODE    COMx1   COMx0   NOTE
    0       0       0       Normal port operation
    1       0       1       Toggle 
    2       1       0       Clear
    3       1       1       Set
*/
#define _TIMER8_NORMAL_OPERATION(inst, ...)           NAME(TCCR, inst) &= ~(_BV(NAME(COM,inst,0)) | _BV(NAME(COM,inst,1)))
#define _TIMER8_TOGGLE_CMP_MATCH(inst, ...)           NAME(TCCR, inst) = ( NAME(TCCR, inst) & ~_BV(NAME(COM,inst,1)) ) | _BV(NAME(COM,inst,0))
#define _TIMER8_CLEAR_CMP_MATCH(inst, ...)            NAME(TCCR, inst) = ( NAME(TCCR, inst) & ~_BV(NAME(COM,inst,0)) ) | _BV(NAME(COM,inst,1))
#define _TIMER8_SET_CMP_MATCH(inst, ...)              NAME(TCCR, inst) |= _BV(NAME(COM,inst,0)) | _BV(NAME(COM,inst,1))

#define _TIMER8_GET_COUNTER(inst, ...)                NAME(TCNT, inst)
#define _TIMER8_SET_COUNTER(inst, value, ...)         NAME(TCNT, inst) = (unsigned char)value

#define _TIMER8_GET_OUTPUT_CMP(inst, ...)             NAME(OCR, inst)
#define _TIMER8_SET_OUTPUT_CMP(inst, value, ...)      NAME(OCR, inst) = (unsigned char)value

/*
    MODE    CSx2    CSx1    CSx0    NOTE
    0       0       0       0       Disabled
    1       0       0       1       no prescaler
    2       0       1       0       prescaler 8
    3       0       1       1       prescaler 64
    4       1       0       0       prescaler 256
    5       1       0       1       prescaler 1024
    6       1       1       0       external clock on T0 pin, clock on falling edge
    7       1       1       1       external clock on T0 pin, clock on rising edge
*/
#define _TIMER8_CLOCK_DISABLED(inst, ...)         NAME(TCCR, inst) &= ~(_BV(NAME(CS,inst,0)) | _BV(NAME(CS,inst,1)) | _BV(NAME(CS,inst,2)))
#define _TIMER8_CLOCK_PRE_1(inst, ...)            NAME(TCCR, inst) = ( NAME(TCCR, inst) & ~(_BV(NAME(CS,inst,2)) | _BV(NAME(CS,inst,1)))) | _BV(NAME(CS,inst,0))
#define _TIMER8_CLOCK_PRE_8(inst, ...)            NAME(TCCR, inst) = ( NAME(TCCR, inst) & ~(_BV(NAME(CS,inst,2)) | _BV(NAME(CS,inst,0)))) | _BV(NAME(CS,inst,1))
#define _TIMER8_CLOCK_PRE_64(inst, ...)           NAME(TCCR, inst) = ( NAME(TCCR, inst) & ~_BV(NAME(CS,inst,2))) | _BV(NAME(CS,inst,0)) | _BV(NAME(CS,inst,1))
#define _TIMER8_CLOCK_PRE_256(inst, ...)          NAME(TCCR, inst) = ( NAME(TCCR, inst) & ~(_BV(NAME(CS,inst,1)) | _BV(NAME(CS,inst,0)))) | _BV(NAME(CS,inst,2))
#define _TIMER8_CLOCK_PRE_1024(inst, ...)         NAME(TCCR, inst) = ( NAME(TCCR, inst) & ~_BV(NAME(CS,inst,1))) | _BV(NAME(CS,inst,2)) | _BV(NAME(CS,inst,0))
#define _TIMER8_CLOCK_EXT_T0FE(inst, ...)         NAME(TCCR, inst) = ( NAME(TCCR, inst) & ~_BV(NAME(CS,inst,0))) | _BV(NAME(CS,inst,2)) | _BV(NAME(CS,inst,1))
#define _TIMER8_CLOCK_EXT_T0RE(inst, ...)         NAME(TCCR, inst) |= _BV(NAME(CS,inst,0)) | _BV(NAME(CS,inst,1)) | _BV(NAME(CS,inst,2))

//ASYNC specific prescaler
#define _TIMER8_ASYNC_CLOCK_PRE_1(inst, ...)            NAME(TCCR, inst) = ( NAME(TCCR, inst) & ~(_BV(NAME(CS,inst,2)) | _BV(NAME(CS,inst,1)))) | _BV(NAME(CS,inst,0))
#define _TIMER8_ASYNC_CLOCK_PRE_8(inst, ...)            NAME(TCCR, inst) = ( NAME(TCCR, inst) & ~(_BV(NAME(CS,inst,2)) | _BV(NAME(CS,inst,0)))) | _BV(NAME(CS,inst,1))
#define _TIMER8_ASYNC_CLOCK_PRE_32(inst, ...)           NAME(TCCR, inst) = ( NAME(TCCR, inst) & ~_BV(NAME(CS,inst,2))) | _BV(NAME(CS,inst,0)) | _BV(NAME(CS,inst,1))
#define _TIMER8_ASYNC_CLOCK_PRE_64(inst, ...)           NAME(TCCR, inst) = ( NAME(TCCR, inst) & ~(_BV(NAME(CS,inst,1)) | _BV(NAME(CS,inst,0)))) | _BV(NAME(CS,inst,2))
#define _TIMER8_ASYNC_CLOCK_PRE_128(inst, ...)          NAME(TCCR, inst) = ( NAME(TCCR, inst) & ~_BV(NAME(CS,inst,1))) | _BV(NAME(CS,inst,2)) | _BV(NAME(CS,inst,0))
#define _TIMER8_ASYNC_CLOCK_PRE_256(inst, ...)          NAME(TCCR, inst) = ( NAME(TCCR, inst) & ~_BV(NAME(CS,inst,0))) | _BV(NAME(CS,inst,2)) | _BV(NAME(CS,inst,1))
#define _TIMER8_ASYNC_CLOCK_PRE_1024(inst, ...)         NAME(TCCR, inst) |= _BV(NAME(CS,inst,0)) | _BV(NAME(CS,inst,1)) | _BV(NAME(CS,inst,2))

//unsuported macros
#define _TIMER8_MODE_PWM_8BIT(inst, ...)             -1
#define _TIMER8_MODE_PWM_9BIT(inst, ...)             -1
#define _TIMER8_MODE_PWM_10BIT(inst, ...)            -1
#define _TIMER8_MODE_CTC_1(inst, ...)                -1
#define _TIMER8_MODE_FASTPWM_8BIT(inst, ...)         -1
#define _TIMER8_MODE_FASTPWM_9BIT(inst, ...)         -1
#define _TIMER8_MODE_FASTPWM_10BIT(inst, ...)        -1
#define _TIMER8_MODE_PWM_PFC1(inst, ...)             -1
#define _TIMER8_MODE_PWM_PFC2(inst, ...)             -1
#define _TIMER8_MODE_PWM_PC1(inst, ...)              -1
#define _TIMER8_MODE_PWM_PC2(inst, ...)              -1
#define _TIMER8_MODE_CTC_2(inst, ...)                -1
#define _TIMER8_MODE_FASTPWM_1(inst, ...)            -1
#define _TIMER8_MODE_FASTPWM_2(inst, ...)            -1
#define _TIMER8_SET_IC_INT(inst)                     -1
#define _TIMER8_GET_IC_INT(inst)                     -1



//******16BIT timer macros

//unsuported macros for timer16
#define _TIMER16_MODE_PWM(inst, ...)             -1
#define _TIMER16_MODE_CTC(inst, ...)             -1
#define _TIMER16_MODE_FAST_PWM(inst, ...)        -1
//ASYNC specific prescaler
#define _TIMER16_ASYNC_CLOCK_PRE_1(inst, ...)      -1     
#define _TIMER16_ASYNC_CLOCK_PRE_8(inst, ...)      -1          
#define _TIMER16_ASYNC_CLOCK_PRE_32(inst, ...)     -1      
#define _TIMER16_ASYNC_CLOCK_PRE_64(inst, ...)     -1      
#define _TIMER16_ASYNC_CLOCK_PRE_128(inst, ...)    -1      
#define _TIMER16_ASYNC_CLOCK_PRE_256(inst, ...)    -1      
#define _TIMER16_ASYNC_CLOCK_PRE_1024(inst, ...)   -1


//
#define _TIMER16_MODE_NORMAL_MODE(inst, ...)             NAME(TCCR, inst, TIMER_CH_A) &= ~(_BV(NAME(WGM,inst,0)) | _BV(NAME(WGM,inst,1))),\
                                                        NAME(TCCR, inst, TIMER_CH_B) &= ~(_BV(NAME(WGM,inst,2)) | _BV(NAME(WGM,inst,3)))    

#define _TIMER16_MODE_PWM_8BIT(inst, ...)                NAME(TCCR, inst, TIMER_CH_A) = ( NAME(TCCR, inst, TIMER_CH_A) & ~_BV(NAME(WGM,inst,1)) ) | _BV(NAME(WGM,inst,0)),\
                                                        NAME(TCCR, inst, TIMER_CH_B) &= ~(_BV(NAME(WGM,inst,2)) | _BV(NAME(WGM,inst,3)))

#define _TIMER16_MODE_PWM_9BIT(inst, ...)                NAME(TCCR, inst, TIMER_CH_A) = ( NAME(TCCR, inst, TIMER_CH_A) & ~_BV(NAME(WGM,inst,0)) ) | _BV(NAME(WGM,inst,1)),\
                                                        NAME(TCCR, inst, TIMER_CH_B) &= ~(_BV(NAME(WGM,inst,2)) | _BV(NAME(WGM,inst,3)))

#define _TIMER16_MODE_PWM_10BIT(inst, ...)               NAME(TCCR, inst, TIMER_CH_A) |= _BV(NAME(WGM,inst,0)) | _BV(NAME(WGM,inst,1)),\
                                                        NAME(TCCR, inst, TIMER_CH_B) &= ~(_BV(NAME(WGM,inst,2)) | _BV(NAME(WGM,inst,3)))

#define _TIMER16_MODE_CTC_1(inst, ...)                   NAME(TCCR, inst, TIMER_CH_A) &= ~(_BV(NAME(WGM,inst,0)) | _BV(NAME(WGM,inst,1))),\
                                                        NAME(TCCR, inst, TIMER_CH_B) = ( NAME(TCCR, inst, TIMER_CH_B) & ~_BV(NAME(WGM,inst,3)) ) | _BV(NAME(WGM,inst,2))

#define _TIMER16_MODE_FASTPWM_8BIT(inst, ...)            NAME(TCCR, inst, TIMER_CH_A) = ( NAME(TCCR, inst, TIMER_CH_A) & ~_BV(NAME(WGM,inst,1)) ) | _BV(NAME(WGM,inst,0)),\
                                                        NAME(TCCR, inst, TIMER_CH_B) = ( NAME(TCCR, inst, TIMER_CH_B) & ~_BV(NAME(WGM,inst,3)) ) | _BV(NAME(WGM,inst,2))

#define _TIMER16_MODE_FASTPWM_9BIT(inst, ...)            NAME(TCCR, inst, TIMER_CH_A) = ( NAME(TCCR, inst, TIMER_CH_A) & ~_BV(NAME(WGM,inst,0)) ) | _BV(NAME(WGM,inst,1)),\
                                                        NAME(TCCR, inst, TIMER_CH_B) = ( NAME(TCCR, inst, TIMER_CH_B) & ~_BV(NAME(WGM,inst,3)) ) | _BV(NAME(WGM,inst,2))

#define _TIMER16_MODE_FASTPWM_10BIT(inst, ...)           NAME(TCCR, inst, TIMER_CH_A) |= _BV(NAME(WGM,inst,0)) | _BV(NAME(WGM,inst,1)),\
                                                        NAME(TCCR, inst, TIMER_CH_B) = ( NAME(TCCR, inst, TIMER_CH_B) & ~_BV(NAME(WGM,inst,3)) ) | _BV(NAME(WGM,inst,2))  

#define _TIMER16_MODE_PWM_PFC1(inst, ...)                NAME(TCCR, inst, TIMER_CH_A) &= ~(_BV(NAME(WGM,inst,0)) | _BV(NAME(WGM,inst,1))),\
                                                        NAME(TCCR, inst, TIMER_CH_B) = ( NAME(TCCR, inst, TIMER_CH_B) & ~_BV(NAME(WGM,inst,2)) ) | _BV(NAME(WGM,inst,3))

#define _TIMER16_MODE_PWM_PFC2(inst, ...)                NAME(TCCR, inst, TIMER_CH_A) = ( NAME(TCCR, inst, TIMER_CH_A) & ~_BV(NAME(WGM,inst,1)) ) | _BV(NAME(WGM,inst,0)),\
                                                        NAME(TCCR, inst, TIMER_CH_B) = ( NAME(TCCR, inst, TIMER_CH_B) & ~_BV(NAME(WGM,inst,2)) ) | _BV(NAME(WGM,inst,3))

#define _TIMER16_MODE_PWM_PC1(inst, ...)                 NAME(TCCR, inst, TIMER_CH_A) = ( NAME(TCCR, inst, TIMER_CH_A) & ~_BV(NAME(WGM,inst,0)) ) | _BV(NAME(WGM,inst,1)),\
                                                        NAME(TCCR, inst, TIMER_CH_B) = ( NAME(TCCR, inst, TIMER_CH_B) & ~_BV(NAME(WGM,inst,2)) ) | _BV(NAME(WGM,inst,3))

#define _TIMER16_MODE_PWM_PC2(inst, ...)                 NAME(TCCR, inst, TIMER_CH_A) |= _BV(NAME(WGM,inst,0)) | _BV(NAME(WGM,inst,1)),\
                                                        NAME(TCCR, inst, TIMER_CH_B) = ( NAME(TCCR, inst, TIMER_CH_B) & ~_BV(NAME(WGM,inst,2)) ) | _BV(NAME(WGM,inst,3))

#define _TIMER16_MODE_CTC_2(inst, ...)                   NAME(TCCR, inst, TIMER_CH_A) &= ~(_BV(NAME(WGM,inst,0)) | _BV(NAME(WGM,inst,1))),\
                                                        NAME(TCCR, inst, TIMER_CH_A) |= _BV(NAME(WGM,inst,2)) | _BV(NAME(WGM,inst,3))

#define _TIMER16_MODE_FASTPWM_1(inst, ...)               NAME(TCCR, inst, TIMER_CH_A) = ( NAME(TCCR, inst, TIMER_CH_A) & ~_BV(NAME(WGM,inst,0)) ) | _BV(NAME(WGM,inst,1)),\
                                                        NAME(TCCR, inst, TIMER_CH_A) |= _BV(NAME(WGM,inst,2)) | _BV(NAME(WGM,inst,3))

#define _TIMER16_MODE_FASTPWM_2(inst, ...)               NAME(TCCR, inst, TIMER_CH_A) |= _BV(NAME(WGM,inst,0)) | _BV(NAME(WGM,inst,1)),\
                                                        NAME(TCCR, inst, TIMER_CH_A) |= _BV(NAME(WGM,inst,2)) | _BV(NAME(WGM,inst,3))


#define _TIMER16_CLOCK_DISABLED(inst, ...)         NAME(TCCR, inst, TIMER_CH_B) &= ~(_BV(NAME(CS,inst,0)) | _BV(NAME(CS,inst,1)) | _BV(NAME(CS,inst,2)))
#define _TIMER16_CLOCK_PRE_1(inst, ...)            NAME(TCCR, inst, TIMER_CH_B) = ( NAME(TCCR, inst, TIMER_CH_B) & ~(_BV(NAME(CS,inst,2)) | _BV(NAME(CS,inst,1)))) | _BV(NAME(CS,inst,0))
#define _TIMER16_CLOCK_PRE_8(inst, ...)            NAME(TCCR, inst, TIMER_CH_B) = ( NAME(TCCR, inst, TIMER_CH_B) & ~(_BV(NAME(CS,inst,2)) | _BV(NAME(CS,inst,0)))) | _BV(NAME(CS,inst,1))
#define _TIMER16_CLOCK_PRE_64(inst, ...)           NAME(TCCR, inst, TIMER_CH_B) = ( NAME(TCCR, inst, TIMER_CH_B) & ~_BV(NAME(CS,inst,2))) | _BV(NAME(CS,inst,0)) | _BV(NAME(CS,inst,1))
#define _TIMER16_CLOCK_PRE_256(inst, ...)          NAME(TCCR, inst, TIMER_CH_B) = ( NAME(TCCR, inst, TIMER_CH_B) & ~(_BV(NAME(CS,inst,1)) | _BV(NAME(CS,inst,0)))) | _BV(NAME(CS,inst,2))
#define _TIMER16_CLOCK_PRE_1024(inst, ...)         NAME(TCCR, inst, TIMER_CH_B) = ( NAME(TCCR, inst, TIMER_CH_B) & ~_BV(NAME(CS,inst,1))) | _BV(NAME(CS,inst,2)) | _BV(NAME(CS,inst,0))
#define _TIMER16_CLOCK_EXT_T0FE(inst, ...)         NAME(TCCR, inst, TIMER_CH_B) = ( NAME(TCCR, inst, TIMER_CH_B) & ~_BV(NAME(CS,inst,0))) | _BV(NAME(CS,inst,2)) | _BV(NAME(CS,inst,1))
#define _TIMER16_CLOCK_EXT_T0RE(inst, ...)         NAME(TCCR, inst, TIMER_CH_B) |= _BV(NAME(CS,inst,0)) | _BV(NAME(CS,inst,1)) | _BV(NAME(CS,inst,2))


#define _TIMER16_NORMAL_OPERATION(inst, ...)     NAME(TCCR, inst, TIMER_CH_A) &= ~(_BV(NAME(COM,inst,__VA_ARGS__,0)) | _BV(NAME(COM,inst,__VA_ARGS__,1)))
#define _TIMER16_TOGGLE_CMP_MATCH(inst, ...)     NAME(TCCR, inst, TIMER_CH_A) = ( NAME(TCCR, inst, __VA_ARGS__) & ~_BV(NAME(COM,inst,__VA_ARGS__,1)) ) | _BV(NAME(COM,inst,__VA_ARGS__,0))
#define _TIMER16_CLEAR_CMP_MATCH(inst, ...)      NAME(TCCR, inst, TIMER_CH_A) = ( NAME(TCCR, inst, __VA_ARGS__) & ~_BV(NAME(COM,inst,__VA_ARGS__,0)) ) | _BV(NAME(COM,inst,__VA_ARGS__,1))
#define _TIMER16_SET_CMP_MATCH(inst, ...)        NAME(TCCR, inst, TIMER_CH_A) |= _BV(NAME(COM,inst,__VA_ARGS__,0)) | _BV(NAME(COM,inst,__VA_ARGS__,1))

#define _TIMER16_GET_COUNTER(inst, ...)                  NAME(TCNT, inst ,__VA_ARGS__)
#define _TIMER16_SET_COUNTER(inst, value, ...)           NAME(TCNT, inst ,__VA_ARGS__) = (unsigned char)value

#define _TIMER16_GET_OUTPUT_CMP(inst, ch, ...)               NAME(OCR, inst,ch ,__VA_ARGS__)
#define _TIMER16_SET_OUTPUT_CMP(inst, value, ch, ...)        NAME(OCR, inst,ch , __VA_ARGS__) = (unsigned char)value

#define _TIMER16_GET_INPUT_CAPTURE(inst, ch, ...)            NAME(ICR, inst,ch ,__VA_ARGS__)
#define _TIMER16_SET_INPUT_CAPTURE(inst, value, ch, ...)     NAME(ICR, inst,ch , __VA_ARGS__) = (unsigned char)value



//MODE_NORMAL_MODE: set timer in normal mode operation
//@instance: timer instance
//usage: ##MODE_NORMAL_MODE(instance)
#define MODE_NORMAL_MODE    MODE_NORMAL_MODE

#define __TIMER_FUNCTIONS(function, inst, ...)          inst == TIMER_2 ? _TIMER8_ ## function(TIMER_2, __VA_ARGS__) : 0
#define _TIMER_FUNCTIONS(function, inst, ...)           inst == TIMER_0 ? _TIMER8_ ## function(TIMER_0, __VA_ARGS__) : __TIMER_FUNCTIONS(function, inst, __VA_ARGS__)
#define TIMER_FUNCTIONS(function,  inst, ...)           inst == TIMER_1 ? _TIMER16_ ## function(TIMER_1, __VA_ARGS__) : _TIMER_FUNCTIONS(function, inst, __VA_ARGS__)


#endif