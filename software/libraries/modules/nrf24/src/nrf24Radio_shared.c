/*
 * nrf24Radio_shared.c
 *
 * Created: 12/3/2019 11:26:45 AM
 *  Author: amestereaga
 */ 
#include <avr/interrupt.h>

//#define IRQ_HANDLER		INT0_vect
#include "nrf24Radio.h"
#include "nrf24Radio_API.h"
#include "nRF24L01.h"
#include <avr/io.h>

#include "nrf24_hw.h"

#define NRF_LIB_HOOKS_BASE_ADDR	0x2800
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

#define _SPI_WAIT()              while ((SPSR & _BV(SPIF)) == 0)
#define _SPI_LOAD(byte)			SPDR = byte
#define _SPI_DATA				SPDR

NRF24_MEMORY ISR(IRQ_HANDLER)
{
	GIFR = (1<<INTF0);
	_radio_instance.irq_triggered++;
	CSN_LOW();
	//load status
	_SPI_LOAD(NOP);
	_SPI_WAIT();
	_radio_instance.irq_status = _SPI_DATA;
	CSN_HIGH();
	

	CSN_LOW();
	//clear interrupt
	_SPI_LOAD(W_REGISTER | (REGISTER_MASK & STATUS));
	_SPI_WAIT();
	_SPI_LOAD( (uint8_t)(_BV(RX_DR) | _BV(TX_DS) | _BV(MAX_RT)) );
	_SPI_WAIT();
	
	CSN_HIGH();
}
