#ifndef _TIMER_HW_H
#define _TIMER_HW_H

#include <avr/io.h>


#define TIMER_0     0
#define TIMER_1     1
#define TIMER_2     2

#define TIMER1_CH_A   A
#define TIMER1_CH_B   B

#define TIMER_H_REG   H
#define TIMER_L_REG   L

#define __NAME(reg,inst,ch, ...)    reg ## inst ## ch ## __VA_ARGS__
#define _NAME(reg,inst, ...)        __NAME(reg,inst,__VA_ARGS__)
#define NAME(register, ...)         _NAME(register, ## __VA_ARGS__)


//******general

#define TIMER_SET_OVF_INT(inst)                 TIMSK |= _BV(NAME(TOIE, inst))
#define TIMER_GET_OVF_INT(inst)                 (TIMSK >> _BV(NAME(TOIE, inst))) & 0x1

#define TIMER_SET_OCMP_INT(inst, ...)           TIMSK |= _BV(NAME(OCIE,inst, __VA_ARGS__))
#define TIMER_GET_OCMP_INT(inst, ...)           (TIMSK >> _BV(NAME(OCIE,inst, __VA_ARGS__))) & 0x1

#define TIMER_CLEAR_OVF_INT_FLAG(inst)          TIFR |= _BV(NAME(TOV, inst))
#define TIMER_GET_OVF_INT_FLAG(inst)            (TIFR >> _BV(NAME(TOV, inst))) & 0x1

#define TIMER_CLEAR_OCMP_INT_FLAG(inst, ...)    TIFR |= _BV(NAME(OCF,inst,__VA_ARGS__))
#define TIMER_GET_OCMP_INT_FLAG(inst, ...)      (TIFR >> _BV(NAME(OCF,inst,__VA_ARGS__))) & 0x1

//****** TIMER8 BITs

/*
    This is used just by TIMER0 & TIMER2
    MODE    WGMx1   WGMx0   NOTE
    0       0       0       Normal port operation
    1       0       1       PWM
    2       1       0       CTC
    3       1       1       Fast PWM
*/            
#define TIMER8_MODE_NORMAL_MODE(inst, ...)       NAME(TCCR, inst) &= ~(_BV(NAME(WGM,inst,0)) | _BV(NAME(WGM,inst,1)))
#define TIMER8_MODE_PWM(inst, ...)               NAME(TCCR, inst) = ( NAME(TCCR, inst, __VA_ARGS__) & ~_BV(NAME(WGM,inst,1)) ) | _BV(NAME(WGM,inst,0))
#define TIMER8_MODE_CTC(inst, ...)               NAME(TCCR, inst) = ( NAME(TCCR, inst, __VA_ARGS__) & ~_BV(NAME(WGM,inst,0)) ) | _BV(NAME(WGM,inst,1))
#define TIMER8_MODE_FAST_PWM(inst, ...)          NAME(TCCR, inst) |= _BV(NAME(WGM,inst,0)) | _BV(NAME(WGM,inst,1))


/*              
    MODE    COMx1   COMx0   NOTE
    0       0       0       Normal port operation
    1       0       1       Toggle 
    2       1       0       Clear
    3       1       1       Set
*/
#define TIMER8_NORMAL_OPERATION(inst)           NAME(TCCR, inst) &= ~(_BV(NAME(COM,inst,0)) | _BV(NAME(COM,inst,1)))
#define TIMER8_TOGGLE_CMP_MATCH(inst)           NAME(TCCR, inst) = ( NAME(TCCR, inst) & ~_BV(NAME(COM,inst,1)) ) | _BV(NAME(COM,inst,0))
#define TIMER8_CLEAR_CMP_MATCH(inst)            NAME(TCCR, inst) = ( NAME(TCCR, inst) & ~_BV(NAME(COM,inst,0)) ) | _BV(NAME(COM,inst,1))
#define TIMER8_SET_CMP_MATCH(inst)              NAME(TCCR, inst) |= _BV(NAME(COM,inst,0)) | _BV(NAME(COM,inst,1))

#define TIMER8_GET_COUNTER(inst)                NAME(TCNT, inst)
#define TIMER8_SET_COUNTER(inst, value)         NAME(TCNT, inst) = (unsigned char)value

#define TIMER8_GET_OUTPUT_CMP(inst)             NAME(OCR, inst)
#define TIMER8_SET_OUTPUT_CMP(inst, value)      NAME(OCR, inst) = (unsigned char)value



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
#define TIMER8_CLOCK_DISABLED(inst)         NAME(TCCR, inst) &= ~(_BV(NAME(CS,inst,0)) | _BV(NAME(CS,inst,1)) | _BV(NAME(CS,inst,2)))
#define TIMER8_CLOCK_PRE_1(inst)            NAME(TCCR, inst) = ( NAME(TCCR, inst) & ~(_BV(NAME(CS,inst,2)) | _BV(NAME(CS,inst,1)))) | _BV(NAME(CS,inst,0))
#define TIMER8_CLOCK_PRE_8(inst)            NAME(TCCR, inst) = ( NAME(TCCR, inst) & ~(_BV(NAME(CS,inst,2)) | _BV(NAME(CS,inst,0)))) | _BV(NAME(CS,inst,1))
#define TIMER8_CLOCK_PRE_64(inst)           NAME(TCCR, inst) = ( NAME(TCCR, inst) & ~_BV(NAME(CS,inst,2))) | _BV(NAME(CS,inst,0)) | _BV(NAME(CS,inst,1))
#define TIMER8_CLOCK_PRE_256(inst)          NAME(TCCR, inst) = ( NAME(TCCR, inst) & ~(_BV(NAME(CS,inst,1)) | _BV(NAME(CS,inst,0)))) | _BV(NAME(CS,inst,2))
#define TIMER8_CLOCK_PRE_1024(inst)         NAME(TCCR, inst) = ( NAME(TCCR, inst) & ~_BV(NAME(CS,inst,1))) | _BV(NAME(CS,inst,2)) | _BV(NAME(CS,inst,0))
#define TIMER8_CLOCK_EXT_T0FE(inst)         NAME(TCCR, inst) = ( NAME(TCCR, inst) & ~_BV(NAME(CS,inst,0))) | _BV(NAME(CS,inst,2)) | _BV(NAME(CS,inst,1))
#define TIMER8_CLOCK_EXT_T0RE(inst)         NAME(TCCR, inst) |= _BV(NAME(CS,inst,0)) | _BV(NAME(CS,inst,1)) | _BV(NAME(CS,inst,2))






//********************************************** TIMER 16 BITs

#define TIMER16_SET_IC_INT(inst)                 TIMSK |= _BV(NAME(TICIE, inst))
#define TIMER16_GET_IC_INT(inst)                 (TIMSK >> _BV(NAME(TICIE, inst))) & 0x1

#define TIMER16_CLEAR_IC_INT_FLAG(inst)          TIMSK |= _BV(NAME(ICF, inst))
#define TIMER16_GET_IC_INT_FLAG(inst)            (TIMSK >> _BV(NAME(ICF, inst))) & 0x1

#define TIMER16_MODE_NORMAL_MODE(inst)           NAME(TCCR, inst, TIMER1_CH_A) &= ~(_BV(NAME(WGM,inst,0)) | _BV(NAME(WGM,inst,1))); \
                                                 NAME(TCCR, inst, TIMER1_CH_B) &= ~(_BV(NAME(WGM,inst,2)) | _BV(NAME(WGM,inst,3)))

#define TIMER16_MODE_PWM_8BIT(inst)              NAME(TCCR, inst, TIMER1_CH_A) = ( NAME(TCCR, inst, TIMER1_CH_A) & ~_BV(NAME(WGM,inst,1)) ) | _BV(NAME(WGM,inst,0)); \
                                                 NAME(TCCR, inst, TIMER1_CH_B) &= ~(_BV(NAME(WGM,inst,2)) | _BV(NAME(WGM,inst,3)))

#define TIMER16_MODE_PWM_9BIT(inst)              NAME(TCCR, inst, TIMER1_CH_A) = ( NAME(TCCR, inst, TIMER1_CH_A) & ~_BV(NAME(WGM,inst,0)) ) | _BV(NAME(WGM,inst,1)); \
                                                 NAME(TCCR, inst, TIMER1_CH_B) &= ~(_BV(NAME(WGM,inst,2)) | _BV(NAME(WGM,inst,3)))

#define TIMER16_MODE_PWM_10BIT(inst)             NAME(TCCR, inst, TIMER1_CH_A) |= _BV(NAME(WGM,inst,0)) | _BV(NAME(WGM,inst,1)); \
                                                 NAME(TCCR, inst, TIMER1_CH_B) &= ~(_BV(NAME(WGM,inst,2)) | _BV(NAME(WGM,inst,3)))

#define TIMER16_MODE_CTC_1(inst)                 NAME(TCCR, inst, TIMER1_CH_A) &= ~(_BV(NAME(WGM,inst,0)) | _BV(NAME(WGM,inst,1))); \
                                                 NAME(TCCR, inst, TIMER1_CH_B) = ( NAME(TCCR, inst, TIMER1_CH_B) & ~_BV(NAME(WGM,inst,3)) ) | _BV(NAME(WGM,inst,2))

#define TIMER16_MODE_FASTPWM_8BIT(inst)          NAME(TCCR, inst, TIMER1_CH_A) = ( NAME(TCCR, inst, TIMER1_CH_A) & ~_BV(NAME(WGM,inst,1)) ) | _BV(NAME(WGM,inst,0)); \
                                                 NAME(TCCR, inst, TIMER1_CH_B) = ( NAME(TCCR, inst, TIMER1_CH_B) & ~_BV(NAME(WGM,inst,3)) ) | _BV(NAME(WGM,inst,2))

#define TIMER16_MODE_FASTPWM_9BIT(inst)          NAME(TCCR, inst, TIMER1_CH_A) = ( NAME(TCCR, inst, TIMER1_CH_A) & ~_BV(NAME(WGM,inst,0)) ) | _BV(NAME(WGM,inst,1)); \
                                                 NAME(TCCR, inst, TIMER1_CH_B) = ( NAME(TCCR, inst, TIMER1_CH_B) & ~_BV(NAME(WGM,inst,3)) ) | _BV(NAME(WGM,inst,2))

#define TIMER16_MODE_FASTPWM_10BIT(inst)         NAME(TCCR, inst, TIMER1_CH_A) |= _BV(NAME(WGM,inst,0)) | _BV(NAME(WGM,inst,1)); \     
                                                 NAME(TCCR, inst, TIMER1_CH_B) = ( NAME(TCCR, inst, TIMER1_CH_B) & ~_BV(NAME(WGM,inst,3)) ) | _BV(NAME(WGM,inst,2))  

#define TIMER16_MODE_PWM_PFC1(inst)              NAME(TCCR, inst, TIMER1_CH_A) &= ~(_BV(NAME(WGM,inst,0)) | _BV(NAME(WGM,inst,1))); \
                                                 NAME(TCCR, inst, TIMER1_CH_B) = ( NAME(TCCR, inst, TIMER1_CH_B) & ~_BV(NAME(WGM,inst,2)) ) | _BV(NAME(WGM,inst,3))

#define TIMER16_MODE_PWM_PFC2(inst)              NAME(TCCR, inst, TIMER1_CH_A) = ( NAME(TCCR, inst, TIMER1_CH_A) & ~_BV(NAME(WGM,inst,1)) ) | _BV(NAME(WGM,inst,0)); \
                                                 NAME(TCCR, inst, TIMER1_CH_B) = ( NAME(TCCR, inst, TIMER1_CH_B) & ~_BV(NAME(WGM,inst,2)) ) | _BV(NAME(WGM,inst,3))

#define TIMER16_MODE_PWM_PC1(inst)               NAME(TCCR, inst, TIMER1_CH_A) = ( NAME(TCCR, inst, TIMER1_CH_A) & ~_BV(NAME(WGM,inst,0)) ) | _BV(NAME(WGM,inst,1)); \
                                                 NAME(TCCR, inst, TIMER1_CH_B) = ( NAME(TCCR, inst, TIMER1_CH_B) & ~_BV(NAME(WGM,inst,2)) ) | _BV(NAME(WGM,inst,3))

#define TIMER16_MODE_PWM_PC2(inst)               NAME(TCCR, inst, TIMER1_CH_A) |= _BV(NAME(WGM,inst,0)) | _BV(NAME(WGM,inst,1)); \
                                                 NAME(TCCR, inst, TIMER1_CH_B) = ( NAME(TCCR, inst, TIMER1_CH_B) & ~_BV(NAME(WGM,inst,2)) ) | _BV(NAME(WGM,inst,3))

#define TIMER16_MODE_CTC_2(inst)                 NAME(TCCR, inst, TIMER1_CH_A) &= ~(_BV(NAME(WGM,inst,0)) | _BV(NAME(WGM,inst,1))); \
                                                 NAME(TCCR, inst, TIMER1_CH_A) |= _BV(NAME(WGM,inst,2)) | _BV(NAME(WGM,inst,3))

#define TIMER16_MODE_FASTPWM_1(inst)             NAME(TCCR, inst, TIMER1_CH_A) = ( NAME(TCCR, inst, TIMER1_CH_A) & ~_BV(NAME(WGM,inst,0)) ) | _BV(NAME(WGM,inst,1)); \
                                                 NAME(TCCR, inst, TIMER1_CH_A) |= _BV(NAME(WGM,inst,2)) | _BV(NAME(WGM,inst,3))

#define TIMER16_MODE_FASTPWM_2(inst)             NAME(TCCR, inst, TIMER1_CH_A) |= _BV(NAME(WGM,inst,0)) | _BV(NAME(WGM,inst,1)); \
                                                 NAME(TCCR, inst, TIMER1_CH_A) |= _BV(NAME(WGM,inst,2)) | _BV(NAME(WGM,inst,3))


#define TIMER16_CLOCK_DISABLED(inst, ...)         NAME(TCCR, inst, __VA_ARGS__) &= ~(_BV(NAME(CS,inst,0)) | _BV(NAME(CS,inst,1)) | _BV(NAME(CS,inst,2)))
#define TIMER16_CLOCK_PRE_1(inst, ...)            NAME(TCCR, inst, __VA_ARGS__) = ( NAME(TCCR, inst, __VA_ARGS__) & ~(_BV(NAME(CS,inst,2)) | _BV(NAME(CS,inst,1)))) | _BV(NAME(CS,inst,0))
#define TIMER16_CLOCK_PRE_8(inst, ...)            NAME(TCCR, inst, __VA_ARGS__) = ( NAME(TCCR, inst, __VA_ARGS__) & ~(_BV(NAME(CS,inst,2)) | _BV(NAME(CS,inst,0)))) | _BV(NAME(CS,inst,1))
#define TIMER16_CLOCK_PRE_64(inst, ...)           NAME(TCCR, inst, __VA_ARGS__) = ( NAME(TCCR, inst, __VA_ARGS__) & ~_BV(NAME(CS,inst,2))) | _BV(NAME(CS,inst,0)) | _BV(NAME(CS,inst,1))
#define TIMER16_CLOCK_PRE_256(inst, ...)          NAME(TCCR, inst, __VA_ARGS__) = ( NAME(TCCR, inst, __VA_ARGS__) & ~(_BV(NAME(CS,inst,1)) | _BV(NAME(CS,inst,0)))) | _BV(NAME(CS,inst,2))
#define TIMER16_CLOCK_PRE_1024(inst, ...)         NAME(TCCR, inst, __VA_ARGS__) = ( NAME(TCCR, inst, __VA_ARGS__) & ~_BV(NAME(CS,inst,1))) | _BV(NAME(CS,inst,2)) | _BV(NAME(CS,inst,0))
#define TIMER16_CLOCK_EXT_T0FE(inst, ...)         NAME(TCCR, inst, __VA_ARGS__) = ( NAME(TCCR, inst, __VA_ARGS__) & ~_BV(NAME(CS,inst,0))) | _BV(NAME(CS,inst,2)) | _BV(NAME(CS,inst,1))
#define TIMER16_CLOCK_EXT_T0RE(inst, ...)         NAME(TCCR, inst, __VA_ARGS__) |= _BV(NAME(CS,inst,0)) | _BV(NAME(CS,inst,1)) | _BV(NAME(CS,inst,2))


#define TIMER16_NORMAL_OPERATION(inst, ...)     NAME(TCCR, inst, TIMER1_CH_A) &= ~(_BV(NAME(COM,inst,__VA_ARGS__,0)) | _BV(NAME(COM,inst,__VA_ARGS__,1)))
#define TIMER16_TOGGLE_CMP_MATCH(inst, ...)     NAME(TCCR, inst, TIMER1_CH_A) = ( NAME(TCCR, inst, __VA_ARGS__) & ~_BV(NAME(COM,inst,__VA_ARGS__,1)) ) | _BV(NAME(COM,inst,__VA_ARGS__,0))
#define TIMER16_CLEAR_CMP_MATCH(inst, ...)      NAME(TCCR, inst, TIMER1_CH_A) = ( NAME(TCCR, inst, __VA_ARGS__) & ~_BV(NAME(COM,inst,__VA_ARGS__,0)) ) | _BV(NAME(COM,inst,__VA_ARGS__,1))
#define TIMER16_SET_CMP_MATCH(inst, ...)        NAME(TCCR, inst, TIMER1_CH_A) |= _BV(NAME(COM,inst,__VA_ARGS__,0)) | _BV(NAME(COM,inst,__VA_ARGS__,1))

#define TIMER16_GET_COUNTER(inst, ch, ...)                  NAME(TCNT, inst,ch ,__VA_ARGS__)
#define TIMER16_SET_COUNTER(inst, ch, value, ...)           NAME(TCNT, inst,ch ,__VA_ARGS__) = (unsigned char)value

#define TIMER16_GET_OUTPUT_CMP(inst, ch, ...)               NAME(OCR, inst,ch ,__VA_ARGS__)
#define TIMER16_SET_OUTPUT_CMP(inst, ch, value, ...)        NAME(OCR, inst,ch , __VA_ARGS__) = (unsigned char)value

#define TIMER16_GET_INPUT_CAPTURE(inst, ch, ...)            NAME(ICR, inst,ch ,__VA_ARGS__)
#define TIMER16_SET_INPUT_CAPTURE(inst, ch, value, ...)     NAME(ICR, inst,ch , __VA_ARGS__) = (unsigned char)value

#endif