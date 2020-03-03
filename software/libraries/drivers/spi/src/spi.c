/**
 * Copyright (C) 2020 Coding Night Romania
 * 
 * This file is part of automatic-farm.
 * 
 * automatic-farm is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * automatic-farm is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with automatic-farm.  If not, see <http://www.gnu.org/licenses/>.
 */


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
