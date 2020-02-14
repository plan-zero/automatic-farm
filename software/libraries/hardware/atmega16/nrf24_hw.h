#ifndef _NRF24_HW_H
#define _NRF24_HW_H

#include <avr/io.h>
#include <avr/interrupt.h>

// Pin definitions for chip select and chip enable on the radio module
#define CE_DDR		DDRB
#define CSN_DDR		DDRB

#define CE_PORT		PORTB
#define CSN_PORT	PORTB

#define CE_PIN		PINB3
#define CSN_PIN		PINB2

#define INTERRUPT_DDR	DDRD
#define INTERRUPT_PORT  PORTD
#define INTERRUPT_PIN	PIND2

// Definitions for selecting and enabling the radio
#define CSN_HIGH()	CSN_PORT |=  _BV(CSN_PIN);
#define CSN_LOW()	CSN_PORT &= ~_BV(CSN_PIN);
#define CE_HIGH()	CE_PORT |=  _BV(CE_PIN);
#define CE_LOW()	CE_PORT &= ~_BV(CE_PIN);

#define NRF24_SET_HW() \
	CE_DDR |= _BV(CE_PIN); \
	CSN_DDR |= _BV(CSN_PIN); \
	INTERRUPT_DDR &= ~_BV(INTERRUPT_PIN); \
	INTERRUPT_PORT |= _BV(INTERRUPT_PIN)



#endif