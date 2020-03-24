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


#ifndef _SPI_HW_H
#define _SPI_HW_H

#include <avr/io.h>

#define SPI_DDR DDRB	// DDR of SPI port
#define SPI_PORT PORTB	// SPI port
#define SPI_MOSI PINB3	// MOSI pin (Master out, Slave in)
#define SPI_MISO PINB4	// MISO pin (Master in, Slave out)
#define SPI_SCK PINB5	// SCK pin (SPI clock)
#define SPI_SS PINB2	// SS pin (Slave Select)

#define SPI_WAIT()              while ((SPSR & _BV(SPIF)) == 0)
#define SPI_LOAD(byte)			SPDR = byte
#define SPI_DATA				SPDR

#define SPI_HW_SETUP_CODE() \
    SPI_DDR &= ~(_BV(SPI_MISO)); \
    SPI_DDR |= (_BV(SPI_MOSI) | _BV(SPI_SS) | _BV(SPI_SCK)); \
    SPI_PORT |= _BV(SPI_SS); \
    SPCR = _BV(SPE) | _BV(MSTR); \
    SPSR = _BV(SPI2X); \
    SPI_PORT &= ~_BV(SPI_SS)


#endif