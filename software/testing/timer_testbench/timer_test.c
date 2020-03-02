#include "uart.h"
#include "timer_hw.h"
#include "timer.h"

typedef enum{
    funct_timer_mode_normal_mode,
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


    exec_test(  funct_timer_mode_normal_mode,   //timer function
                TIMER_0,                        //timer instance
                0x48,                           //timer's reg init value
                0,                              //expected result
                &TCCR0 );                       //target register

    exec_test(  funct_timer_mode_normal_mode,   //timer function
            TIMER_1,                        //timer instance
            0x3,                           //timer's reg init value
            0,                              //expected result
            &TCCR1A );                       //target register

    exec_test(  funct_timer_mode_normal_mode,   //timer function
            TIMER_2,                        //timer instance
            0x48,                           //timer's reg init value
            0,                              //expected result
            &TCCR2 );                       //target register


    
    while(1);

    return 0;
}

