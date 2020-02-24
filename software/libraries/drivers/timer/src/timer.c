#include "timer.h"
#include "stdint.h"



void timer_init(timer_cfg cfg)
{
    TIMER_SET_OCMP_INT(TIMER_1, TIMER1_CH_A);
}