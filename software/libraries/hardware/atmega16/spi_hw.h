#ifndef _SPI_HW_H
#define _SPI_HW_H

#include <avr/io.h>

#define SPI_DDR DDRB	// DDR of SPI port
#define SPI_PORT PORTB	// SPI port
#define SPI_MOSI PINB5	// MOSI pin (Master out, Slave in)
#define SPI_MISO PINB6	// MISO pin (Master in, Slave out)
#define SPI_SCK PINB7	// SCK pin (SPI clock)
#define SPI_SS PINB4	// SS pin (Slave Select)

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