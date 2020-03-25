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


#ifndef _NRF24_HW_H
#define _NRF24_HW_H

#include <avr/io.h>
#include <avr/interrupt.h>

// Pin definitions for chip select and chip enable on the radio module
#define CE_DDR		DDRB
#define CSN_DDR		DDRB

#define CE_PORT		PORTB
#define CSN_PORT	PORTB

#define CE_PIN		PINB1
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