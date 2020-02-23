#include "timer.h"


void timer_init()
{
    TIMER_SET_OCMP_INT(TIMER_1, TIMER1_CH_A);
}