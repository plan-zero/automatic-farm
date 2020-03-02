#ifndef _INTERRUPT_HW_H
#define _INTERRUPT_HW_H

#include <avr/io.h>
#include <avr/interrupt.h>


//generat interrupt definitions
#define INTERRUPT_ROUTINE(x) ISR(x)

#define INT_ENABLE_BOOTLOADER_INTVECT() \
    GICR |= (1 << IVCE); \
	GICR = 0x2

#define INT_GLOBAL_EN()   sei()
#define INT_GLOBAL_DIS()  cli()

//EXT INT0 used for nrf24 RX/TX notification pin
#define IRQ_INT0_EN			    INT0
#define IRQ_INT0_EDGE		    ISC01 //rising edge
#define IRQ_INT0_HANDLER		INT0_vect

#define INT_ENABLE_INTERRUPT0_HW() \
    GICR |= _BV(IRQ_INT0_EN); \
	MCUCR |= _BV(IRQ_INT0_EDGE)

//USART_TXC and USART_RXC used by uart module
#define IRQ_USART_TXC_HANDLER       USART_TXC_vect
#define IRQ_USART_RXC_HANDLER       USART_RXC_vect

//TIMERS

#define IRQ_TIMER0_OVF  TIMER0_OVF_vect
#define IRQ_TIMER1_OVF  TIMER1_OVF_vect
#define IRQ_TIMER2_OVF  TIMER2_OVF_vect

#define IRQ_TIMER0_CMP      TIMER0_COMP_vect
#define IRQ_TIMER1A_CMP     TIMER1_COMPA_vect
#define IRQ_TIMER1B_CMP     TIMER1_COMPB_vect
#define IRQ_TIMER2_CMP      TIMER2_COMP_vect

#define IRQ_TIMER1_CAPT     TIMER1_CAPT_vect







#endif