#ifndef _NRF24_HW_H
#define _NRF24_HW_H


// Pin definitions for chip select and chip enable on the radio module
#define CE_DDR		DDRB
#define CSN_DDR		DDRB

#define CE_PORT		PORTB
#define CSN_PORT	PORTB

#define CE_PIN		PINB3
#define CSN_PIN		PINB2

#define INTERRUPT_DDR	DDRD
#define INTERRUPT_PIN	PIND2
#define IRQ_EN			INT0
#define IRQ_EDGE		ISC01 //rising edge
#define IRQ_HANDLER		INT0_vect

// Definitions for selecting and enabling the radio
#define CSN_HIGH()	CSN_PORT |=  _BV(CSN_PIN);
#define CSN_LOW()	CSN_PORT &= ~_BV(CSN_PIN);
#define CE_HIGH()	CE_PORT |=  _BV(CE_PIN);
#define CE_LOW()	CE_PORT &= ~_BV(CE_PIN);


#endif