/*
 * spi.c
 *
 *  Created on: 24-Jan-2009
 *      Author: Neil MacMillan
 *
 *  Functions for using the AT90 as an SPI master.
 */

#include "spi.h"
#include "spi_hw.h"


void SPI_Init()
{
	SPI_HW_SETUP_CODE();
}

void SPI_ReadWrite_Block(uint8_t* data, uint8_t* buffer, uint8_t len)
{
    uint8_t i;
    for (i = 0; i < len; i++) {
          SPI_DATA = data[i];
          SPI_WAIT();
          buffer[i] = SPI_DATA;
    }
}

void SPI_Write_Block(uint8_t* data, uint8_t len)
{
    uint8_t i;
    for (i = 0; i < len; i++) {
          SPI_DATA = data[i];
          SPI_WAIT();
    }
}

uint8_t SPI_Write_Byte(uint8_t byte)
{
    SPI_DATA = byte;
    SPI_WAIT();
    return SPI_DATA;
}
