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
 * nrf24Radio_shared.c
 *
 * Created: 12/3/2019 11:26:45 AM
 *  Author: amestereaga
 */ 

#include "nrf24Radio_API.h"
#include "nRF24L01.h"

#include "nrf24_hw.h"
#include "interrupt_hw.h"
#include "spi_hw.h"
#include "flash_layout.h"

#define NRF_LIB_HOOKS_BASE_ADDR	NRF_HOOKS_ADDR
radio_error_code (* const __flash *fptr_nrfRadio_Main)(radio_context *) = (radio_error_code(* const __flash *)(radio_context *))NRF_LIB_HOOKS_BASE_ADDR;
radio_error_code (* const __flash *fptr_nrfRadio_TransmitMode)(radio_context *) = (radio_error_code(* const __flash *)(radio_context *))(NRF_LIB_HOOKS_BASE_ADDR + 2);
radio_error_code (* const __flash *fptr_nrfRadio_Init)(radio_context *, radio_config) = (radio_error_code(* const __flash *)(radio_context *, radio_config))(NRF_LIB_HOOKS_BASE_ADDR + 4);
radio_error_code (* const __flash *fptr_nrfRadio_PowerDown)(radio_context *) = (radio_error_code(* const __flash *)(radio_context *))(NRF_LIB_HOOKS_BASE_ADDR + 6);
radio_error_code (* const __flash *fptr_nrfRadio_GetInfo)(radio_context *, radio_registers*) = (radio_error_code(* const __flash *)(radio_context *, radio_registers*))(NRF_LIB_HOOKS_BASE_ADDR + 8);
radio_error_code (* const __flash *fptr_nrfRadio_PipeConfig)(radio_context *, pipe_config) = (radio_error_code(* const __flash *)(radio_context *, pipe_config))(NRF_LIB_HOOKS_BASE_ADDR + 10);
radio_error_code (* const __flash *fptr_nrfRadio_LoadMessages)(radio_context *, uint8_t*, uint8_t) = (radio_error_code(* const __flash *)(radio_context *, uint8_t*, uint8_t))(NRF_LIB_HOOKS_BASE_ADDR + 12);
radio_tx_status (* const __flash *fptr_nrfRadio_Transmit)(radio_context *, uint8_t*, radio_transmision_type) = (radio_tx_status(* const __flash *)(radio_context *, uint8_t*, radio_transmision_type))(NRF_LIB_HOOKS_BASE_ADDR + 14);
radio_error_code (* const __flash *fptr_nrfRadio_ListeningMode)(radio_context *) = (radio_error_code(* const __flash *)(radio_context *))(NRF_LIB_HOOKS_BASE_ADDR + 16);
radio_error_code (* const __flash *fptr_nrfRadio_PowerUp)(radio_context *) = (radio_error_code(* const __flash *)(radio_context *))(NRF_LIB_HOOKS_BASE_ADDR + 18);
radio_error_code (* const __flash *fptr_nrfRadio_ChangeDataRate)(radio_context *, radio_data_rate) = (radio_error_code(* const __flash *)(radio_context *, radio_data_rate))(NRF_LIB_HOOKS_BASE_ADDR + 20);
radio_error_code (* const __flash *fptr_nrfRadio_ChangePower)(radio_context *, radio_rf_power) = (radio_error_code(* const __flash *)(radio_context *, radio_rf_power))(NRF_LIB_HOOKS_BASE_ADDR + 22);
radio_error_code (* const __flash *fptr_nrfRadio_SetTxCallback)(radio_context *, void (*)(radio_tx_status)) = (radio_error_code(* const __flash *)(radio_context *, void (*)(radio_tx_status)))(NRF_LIB_HOOKS_BASE_ADDR + 24);
radio_error_code (* const __flash *fptr_nrfRadio_SetRxCallback)(radio_context *, void (*)(uint8_t, uint8_t *, uint8_t)) = (radio_error_code(* const __flash *)(radio_context *, void (*)(uint8_t, uint8_t *, uint8_t)))(NRF_LIB_HOOKS_BASE_ADDR + 26);
radio_error_code (* const __flash *fptr_nrfRadio_LoadAckPayload)(radio_context *, radio_pipe, uint8_t*, uint8_t) = (radio_error_code(* const __flash *)(radio_context *, radio_pipe, uint8_t*, uint8_t))(NRF_LIB_HOOKS_BASE_ADDR + 28);

//the instance that is used to store the radio details (states, pipes configuration and so on)
radio_context _radio_instance;

NRF24_MEMORY INTERRUPT_ROUTINE(IRQ_INT0_HANDLER)
{
	//GIFR = (1<<INTF0);
	_radio_instance.irq_triggered++;
	CSN_LOW();
	//load status
	SPI_LOAD(NOP);
	SPI_WAIT();
	_radio_instance.irq_status = SPI_DATA;
	CSN_HIGH();
	

	CSN_LOW();
	//clear interrupt
	SPI_LOAD(W_REGISTER | (REGISTER_MASK & STATUS));
	SPI_WAIT();
	SPI_LOAD( (uint8_t)(_BV(RX_DR) | _BV(TX_DS) | _BV(MAX_RT)) );
	SPI_WAIT();
	
	CSN_HIGH();
}
