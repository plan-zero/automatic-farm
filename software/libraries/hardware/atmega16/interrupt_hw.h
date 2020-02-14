#ifndef _INTERRUPT_HW_H
#define _INTERRUPT_HW_H

#include <avr/io.h>
#include <avr/interrupt.h>

#define IRQ_EN			INT0
#define IRQ_EDGE		ISC01 //rising edge
#define IRQ_HANDLER		INT0_vect

#define INT_ENABLE_BOOTLOADER_INTVECT() \
    GICR |= (1 << IVCE); \
	GICR = 0x2
#define INT_ENABLE_INTERRUPT_HW() \
    GICR |= _BV(IRQ_EN); \
	MCUCR |= _BV(IRQ_EDGE)

#define INT_GLOBAL_EN()   sei()
#define INT_GLOBAL_DIS()  cli()


#endif