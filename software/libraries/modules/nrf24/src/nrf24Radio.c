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
 * nrf24Radio.c
 *
 * Created: 11/19/2019 1:38:29 PM
 *  Author: amestereaga
 */ 
#include "nRF24L01.h"
#include "nrf24Radio.h"
#include "spi.h"
#include "stdint.h"
#include <stddef.h>

#include "nrf24_hw.h"
#include "interrupt_hw.h"
#include "flash_layout.h"


#include <util/delay.h>


#define MAX_PAYLOAD_WIDTH 32
#define MIN_PAYLOAD_WIDTH 1

//the IRQ flag used to see if the interrupt has been triggered
/*volatile uint8_t irq_triggered = 0;
volatile uint8_t irq_status = 0;
void (*tx_callback)(radio_tx_status) = NULL;
void (*rx_callback)(uint8_t, uint8_t*, uint8_t) = NULL;*/



NRF24_MEMORY static uint8_t set_register(radio_register_t reg, uint8_t* value, uint8_t len)
{
	uint8_t status;
	CSN_LOW();

	status = SPI_Write_Byte(W_REGISTER | (REGISTER_MASK & reg));
	SPI_Write_Block(value, len);

	CSN_HIGH();

	return status;
}

NRF24_MEMORY static uint8_t get_register(radio_register_t reg, uint8_t* buffer, uint8_t len)
{
	uint8_t status, i;
	for (i = 0; i < len; i++)
	{
		// If the buffer is too long for the register results, then the radio will interpret the extra bytes as instructions.
		// To remove the risk, we set the buffer elements to NOP instructions.
		buffer[i] = 0xFF;
	}
	CSN_LOW();

	status = SPI_Write_Byte(R_REGISTER | (REGISTER_MASK & reg));
	SPI_ReadWrite_Block(NULL, buffer, len);

	CSN_HIGH();

	return status;
}

NRF24_MEMORY static void send_instruction(uint8_t instruction, uint8_t* data, uint8_t* buffer, uint8_t len)
{
	CSN_LOW();
	// send the instruction
	SPI_Write_Byte(instruction);
	// pass in args
	if (len > 0)
	{
		if (buffer == NULL)	//
		SPI_Write_Block(data, len);
		else
		SPI_ReadWrite_Block(data, buffer, len);
	}
	// resynch SPI
	CSN_HIGH();
}

NRF24_MEMORY radio_error_code nrfRadio_SetTxCallback(radio_context *instance, void (*callback)(radio_tx_status)) {
	instance->tx_callback = callback;
	return RADIO_ERR_OK;
}

NRF24_MEMORY radio_error_code nrfRadio_SetRxCallback(radio_context *instance, void (*callback)(uint8_t, uint8_t *, uint8_t)) {
	instance->rx_callback = callback;
	return RADIO_ERR_OK;
}

NRF24_MEMORY radio_error_code nrfRadio_GetInfo(radio_context *instance, radio_registers * reg)
{
	//read CONFIG register
	get_register(CONFIG, &reg->config, 1);
	//read EN_AA register
	get_register(EN_AA, &reg->en_aa, 1);
	//read EN_RXADDR register
	get_register(EN_RXADDR, &reg->en_rxaddr, 1);
	//read SETUP_AW register
	get_register(SETUP_AW, &reg->setup_aw, 1);
	//read SETUP_RETR register
	get_register(SETUP_RETR, &reg->setup_retr, 1);
	//read RF_CH register
	get_register(RF_CH, &reg->rf_ch, 1);
	//read RF_SETUP
	get_register(RF_SETUP, &reg->rf_setup, 1);
	//read STATUS register
	get_register(STATUS, &reg->status, 1);
	//read OBSERVE_TX register
	get_register(OBSERVE_TX, &reg->observed_tx, 1);
	//read RPD register
	get_register(RPD, &reg->rpd, 1);
	//read RX_PIPE_0 address
	get_register(RX_ADDR_P0, (uint8_t *)&reg->rx_addr_p0, 5);
	//read RX_PIPE_1 address
	get_register(RX_ADDR_P1, (uint8_t *)&reg->rx_addr_p1, 5);
	//read RX_PIPE_2 address
	get_register(RX_ADDR_P2, &reg->rx_addr_p2, 1);
	//read RX_PIPE_3 address
	get_register(RX_ADDR_P3, &reg->rx_addr_p3, 1);
	//read RX_PIPE_4 address
	get_register(RX_ADDR_P4, &reg->rx_addr_p4, 1);
	//read RX_PIPE_5 address
	get_register(RX_ADDR_P5, &reg->rx_addr_p5, 1);
	//read TX address
	get_register(TX_ADDR, (uint8_t *)&reg->tx_addr, 5);
	//read FIFO status register
	get_register(FIFO_STATUS, &reg->fifo_status, 1);
	//read DYNPD register
	get_register(DYNPD, &reg->dynpd, 1);
	//read FEATURE register
	get_register(FEATURE, &reg->feature, 1);
	
	return RADIO_ERR_OK;
}

NRF24_MEMORY radio_error_code nrfRadio_Init(radio_context *instance, radio_config cfg)
{
	radio_error_code err = RADIO_ERR_OK;
	uint8_t value;

	//set hardware registers
	NRF24_SET_HW();
		
	//move the IRQ table to the bootloader starting section, which is different than the application usage (where it starts from 0x0)
	if(RADIO_BOOTLOADER == cfg.usecase) {
		INT_ENABLE_BOOTLOADER_INTVECT();
	}
	INT_ENABLE_INTERRUPT0_HW();


	//CONFIGURE the radio, the radio is disabled during configuration
	CE_LOW();
	// wait for POR (defined as 10.3 ms for NRF24L01 )
	_delay_ms(11);
	//initialize the SPI
	SPI_Init();
	//enter in power down
	nrfRadio_PowerDown(instance);
	//set the address length
	value = cfg.address_length;
	set_register(SETUP_AW, &value, 1);
	//save the address length in context
	instance->address_length = cfg.address_length + 2;
	
	//set ESB retry delay and retry count (it's working only if AA is enabled)
	value = (uint8_t)cfg.retransmit_delay << 4 | ( (uint8_t)cfg.retransmit_count & 0x0F);
	set_register(SETUP_RETR, &value, 1);
	
	//setup the channel (for best performance use a channel which is above the standard 2.4 GHz Wi-fi, channel 12 )
	value = cfg.frequency_channel;
	set_register(RF_CH, &value, 1);
	
	//set the data rate of RF and radio power
	value = 0;
	if(RADIO_COUNT_WAVE_ENABLED == cfg.enable_count_wave) {
		value |= 0x80; //TODO: define count wave as 0x80 in header
	}
	switch(cfg.data_rate){
		case RADIO_2MBIT:
			value |= _BV(RF_DR_HIGH);
		break;
		case RADIO_250KBIT:
			value |= _BV(RF_DR_LOW);
		break;
		default:
			//for 1 MBIT we don't have to set the bits (both RF_DR are zero - '00' )
		break;
	}
	
	switch(cfg.rf_power) {
		case RADIO_LOWEST_18DBM:
			//'00'
		break;
		case RADIO_LOW_12DBM:
			//'01'
			value |= (uint8_t)RADIO_LOW_12DBM << 1;
		break;
		case RADIO_HIGH_6DBM:
			value |= (uint8_t)RADIO_HIGH_6DBM << 1;
		break;
		case RADIO_HIGHEST_0DBM:
			value |= (uint8_t)RADIO_HIGHEST_0DBM << 1;
		break;
		default:
		break;
	}
	set_register(RF_SETUP, &value, 1);
	
	//set the CRC verification
	if(RADIO_CRC2_ENABLED == cfg.crc) {
		value = _BV(EN_CRC) | _BV(CRCO);
		set_register(CONFIG, &value, 1);
	}
	else if(RADIO_CRC1_ENABLED == cfg.crc) {
		value = _BV(EN_CRC);
		set_register(CONFIG, &value, 1);
	}
	
	//set the features register
	value = 0;
	if(RADIO_DYNAMIC_ACK_ENABLED == cfg.en_dynamic_ack) {
		value |= _BV(EN_DYN_ACK);
	}
	if(RADIO_ACK_PAYLOAD_ENABLED == cfg.en_ack_payload) {
		value |= _BV(EN_ACK_PAY);
	}
	if(RADIO_DYNAMIC_PAYLOAD_ENABLED == cfg.en_dynamic_payload_length) {
		value |= _BV(EN_DPL);
	}
	set_register(FEATURE, &value, 1);
	
	//clear any interrupt in Radio status
	get_register(STATUS, &value, 1);
	value |= (_BV(RX_DR) | _BV(TX_DS) | _BV(MAX_RT));
	set_register(STATUS, &value, 1);
	
	//Flush FIFO buffers
	send_instruction(FLUSH_TX, NULL, NULL, 0);
	send_instruction(FLUSH_RX, NULL, NULL, 0);

	
	return err;
}

NRF24_MEMORY radio_error_code nrfRadio_FlushBuffer(radio_context *instance, radio_buffer_t buffer)
{
	radio_error_code err = RADIO_ERR_OK;
	switch (buffer)
	{
	case RADIO_RX_BUFFER:
		send_instruction(FLUSH_RX, NULL, NULL, 0);
		break;
	case RADIO_TX_BUFFER:
		send_instruction(FLUSH_TX, NULL, NULL, 0);
		break;
	case RADIO_BOTH_BUFFER:
		send_instruction(FLUSH_TX, NULL, NULL, 0);
		send_instruction(FLUSH_RX, NULL, NULL, 0);
		break;
	default:
		err = RADIO_ERR_INVALID;
		break;
	}
	return err;
}

NRF24_MEMORY radio_error_code nrfRadio_PipeConfig(radio_context *instance, pipe_config pipe_cfg){
	radio_error_code err = RADIO_ERR_OK;
	uint8_t value = 0;
	
	//return if the requested payload width doesn't match the NRF24 payload minimum and maximum width
	if (pipe_cfg.payload_width < MIN_PAYLOAD_WIDTH || pipe_cfg.payload_width > MAX_PAYLOAD_WIDTH) {
		err = RADIO_ERR_INVALID;
		return err;
	}
	
	//store the pipe0 address that is used for retransmission, also the pipe zero address should always be enabled for auto-ack feature
	if(RADIO_PIPE0 == pipe_cfg.pipe) {
		for(uint8_t idx = 0; idx < instance->address_length; idx ++) {
			instance->rxAddressP0[idx] = pipe_cfg.rx_address[idx];
		}
	}
	
	if(RADIO_PIPE1 < pipe_cfg.pipe)
		set_register(RX_ADDR_P0 + pipe_cfg.pipe, pipe_cfg.rx_address, 1);
	else
		set_register(RX_ADDR_P0 + pipe_cfg.pipe, pipe_cfg.rx_address, instance->address_length);
	
	//enable auto-ack
	get_register(EN_AA, &value, 1);
	if(RADIO_PIPE_AA_ENABLED == pipe_cfg.enable_aa) {
		value |= _BV(pipe_cfg.pipe);
	}
	else {
		value &= ~_BV(pipe_cfg.pipe);
	}
	set_register(EN_AA, &value, 1);
	
	//enable payload length (dynamic or static)
	
	if(RADIO_PIPE_DYNAMIC_PYALOAD_ENABLED == pipe_cfg.enable_dynamic_payload_length) {
		//if the RX is using dynamic payload then we have to set the DYNPD register
		value = 0;
		get_register(DYNPD, &value, 1);
		value |= _BV(pipe_cfg.pipe);
		set_register(DYNPD, &value, 1);
		//we save in the context that we use the dynamic payload
		instance->dynamic_payload = 1;
	}
	else {
		//set the static pipe payload width
		instance->dynamic_payload = 0;
		if(RADIO_PIPE_RX_DISABLED == pipe_cfg.enable_rx_address) {
			value = 0;
			set_register(RX_PW_P0 + pipe_cfg.pipe, &value, 1);
			instance->rx_pipe_widths[(uint8_t)pipe_cfg.pipe] = 0;
		}
		else {
			value = pipe_cfg.payload_width;
			set_register(RX_PW_P0 + pipe_cfg.pipe, &value, 1);
			instance->rx_pipe_widths[(uint8_t)pipe_cfg.pipe] = pipe_cfg.payload_width;
		}
		
	}
	
	get_register(EN_RXADDR, &value, 1);
	if(RADIO_PIPE_RX_DISABLED == pipe_cfg.enable_rx_address) {
		value &= ~_BV(pipe_cfg.pipe);
	}
	else {
		value |= _BV(pipe_cfg.pipe);
	}
	set_register(EN_RXADDR, &value, 1);
	
	
	
	return err;
}

NRF24_MEMORY radio_error_code nrfRadio_ChangeDataRate(radio_context *instance, radio_data_rate datarate) {
	
	uint8_t value = 0;
	uint8_t read_value = 0;
		
	switch(datarate){
		case RADIO_2MBIT:
			value |= _BV(RF_DR_HIGH);
		break;
		case RADIO_250KBIT:
			value |= _BV(RF_DR_LOW);
		break;
		default:
			return RADIO_ERR_INVALID;
		break;
	}
	set_register(RF_SETUP, &value, 1);
	get_register(RF_SETUP, &read_value, 1);
	
	if(value != read_value)
		return RADIO_ERR_WRITE_ERR;
	
	return RADIO_ERR_OK;
}

NRF24_MEMORY radio_error_code nrfRadio_ChangePower(radio_context *instance, radio_rf_power power){
	uint8_t value = 0;
	uint8_t read_value = 0;
	
	switch(power) {
		case RADIO_LOWEST_18DBM:
		//'00'
		break;
		case RADIO_LOW_12DBM:
		//'01'
			value |= (uint8_t)RADIO_LOW_12DBM << 1;
		break;
		//'10'
		case RADIO_HIGH_6DBM:
			value |= (uint8_t)RADIO_HIGH_6DBM << 1;
		break;
		//'11'
		case RADIO_HIGHEST_0DBM:
			value |= (uint8_t)RADIO_HIGHEST_0DBM << 1;
		break;
		default:
			return RADIO_ERR_INVALID;
		break;
	}
	set_register(RF_SETUP, &value, 1);
	get_register(RF_SETUP, &read_value, 1);
	
	if(value != read_value)
		return RADIO_ERR_WRITE_ERR;
	
	return RADIO_ERR_OK;
}

NRF24_MEMORY radio_error_code nrfRadio_PowerDown(radio_context *instance) {
	
	uint8_t value = 0;
	
	if(RADIO_POWER_DOWN == instance->currentState )
		return RADIO_ERR_INVALID;
	
	get_register(CONFIG, &value, 1);
	value &= ~_BV(PWR_UP);
	set_register(CONFIG, &value, 1);
	CE_LOW();
	_delay_ms(2);
	
	instance->currentState = RADIO_POWER_DOWN;
	return RADIO_ERR_OK;
}

NRF24_MEMORY radio_error_code nrfRadio_PowerUp(radio_context *instance) {
	uint8_t value = 0;
	
	//if the state is not power down, then the radio is already powered up
	if(RADIO_POWER_DOWN != instance->currentState)
		return RADIO_ERR_INVALID;
		
	get_register(CONFIG, &value, 1);
	value |= _BV(PWR_UP);
	// make sure that it doesn't enter in PRX after power up
	value &= ~_BV(PRIM_RX);
	set_register(CONFIG, &value, 1);
	CE_LOW();
	_delay_ms(2);
	
	//we go in standby 1, so the CE pin will be low
	instance->currentState = RADIO_STANDBY_1;
	return RADIO_ERR_OK;
}

NRF24_MEMORY radio_error_code nrfRadio_ListeningMode(radio_context *instance) {
	uint8_t value = 0;
	
	//the radio must be powered up before it enters in listening mode
	if(RADIO_POWER_DOWN == instance->currentState)
		return RADIO_ERR_INVALID;
	get_register(CONFIG, &value, 1);
	
	if ((value & _BV(PRIM_RX)) == 0) {
		
		value |= _BV(PRIM_RX);
		send_instruction(FLUSH_RX, NULL, NULL, 0);
		set_register(CONFIG, &value, 1);
		//wait 130 us until it can operate in RX mode + 10 us to avoid the clock errors
		_delay_us(140);
		CE_HIGH();
		instance->currentState = RADIO_PRX;
		return RADIO_ERR_OK;
		
	}
	
	return RADIO_ERR_INVALID;
}

NRF24_MEMORY radio_error_code nrfRadio_TransmitMode(radio_context *instance) {
	uint8_t value = 0;
	
	//the radio must be powered up before it enters in listening mode
	if(RADIO_POWER_DOWN == instance->currentState)
		return RADIO_ERR_INVALID;
		
	get_register(CONFIG, &value, 1);
	//if it is in RX mode then we switch to TX
	if ((value & _BV(PRIM_RX)) != 0) {
		value &= ~_BV(PRIM_RX);
		send_instruction(FLUSH_TX, NULL, NULL, 0);
		set_register(CONFIG, &value, 1);
		//wait 130 us until it can operate in TX mode + 10 us to avoid the clock errors
		_delay_us(140);
		CE_LOW();
		instance->currentState = RADIO_STANDBY_1;
	}
	
	return RADIO_ERR_INVALID;
}
NRF24_MEMORY radio_error_code nrfRadio_LoadMessages(radio_context *instance, uint8_t * payload, uint8_t payload_length) {
	uint8_t value;
	
	//radio is transmiting
	if (RADIO_PTX == instance->currentState)
		return RADIO_ERR_INVALID;

	get_register(FIFO_STATUS, &value, 1);
	//check if the FIFO buffer is empty, otherwise there is still data to be transmited
	if( (value & _BV(TX_FIFO_EMPTY)) == 0)
		return RADIO_ERR_INVALID;

	if(payload_length < 1 || payload_length > 32)
		return RADIO_ERR_INVALID;
	//make sure the radio is disabled during payload load
	CE_LOW();
	send_instruction(W_TX_PAYLOAD, payload, NULL, payload_length);

	return RADIO_ERR_OK;
}

NRF24_MEMORY radio_tx_status nrfRadio_Transmit(radio_context *instance, uint8_t * tx_address, radio_transmision_type trans_type) 
{
	uint8_t value;
	radio_tx_status txerr = RADIO_TX_OK;
	
	//if radio is already transmitting	
	if(RADIO_PTX == instance->currentState)
		return RADIO_ERR_INVALID;
	//radio is powered up and ready to start the transmission
	if(RADIO_STANDBY_1 != instance->currentState)
		return RADIO_ERR_INVALID;
	get_register(FIFO_STATUS, &value, 1);
	//if there is nothing to transmit then return
	if( (value & _BV(TX_FIFO_EMPTY)) == 1)
		return RADIO_ERR_INVALID;
	//set the PIPE0 to TX address for auto-ack
	set_register(RX_ADDR_P0, (uint8_t*)tx_address, instance->address_length); //TODO done
	set_register(TX_ADDR, (uint8_t*)tx_address, instance->address_length);
	
	//start the transmission, we set the radio in PTX
	CE_HIGH();	
	_delay_us(11);
	CE_LOW();
	
	if(RADIO_WAIT_TX == trans_type) 
	{
		
		while(instance->irq_triggered == 0); //wait to transmit
		instance->irq_triggered = 0;
		if(instance->irq_status & _BV(TX_DS)) 
		{
			txerr = RADIO_TX_OK;
		}
		else if(instance->irq_status & _BV(MAX_RT)) 
		{
			txerr = RADIO_TX_MAX_RT;
		}
		//there is a ACK payload
		if(instance->irq_status & _BV(RX_DR)) 
		{
			uint8_t rec_buffer[32] = {0};
			uint8_t pipe_number = (instance->irq_status & 0xE) >> 1;
			uint8_t tmp[1];
			if(instance->dynamic_payload) 
			{
				send_instruction(R_RX_PL_WID, (uint8_t*)tmp, (uint8_t*)tmp, 1);
				send_instruction(R_RX_PAYLOAD, (uint8_t*)rec_buffer, (uint8_t*)rec_buffer, tmp[0]);
				if (instance->rx_callback)
				{
					txerr = RADIO_TX_OK_ACK_PYL;
					instance->rx_callback(pipe_number, rec_buffer, tmp[0]);					
				}
			}
			else 
			{
				send_instruction(R_RX_PAYLOAD, (uint8_t*)rec_buffer, (uint8_t*)rec_buffer, instance->rx_pipe_widths[pipe_number]);
				if (instance->rx_callback)
				{
					txerr = RADIO_TX_OK_ACK_PYL;
					instance->rx_callback(pipe_number, rec_buffer, instance->rx_pipe_widths[pipe_number]);
				}
			}
		}
		set_register(RX_ADDR_P0, (uint8_t*)instance->rxAddressP0, instance->address_length);
		instance->currentState = RADIO_STANDBY_1;
		_delay_us(140);
	}
	else 
	{
		txerr = RADIO_TX_OK;
		instance->currentState = RADIO_PTX;
	}
	
	return txerr;
}

NRF24_MEMORY radio_error_code nrfRadio_LoadAckPayload(radio_context *instance, radio_pipe pipe, uint8_t * payload, uint8_t payload_length)
{
	if(RADIO_PRX == instance->currentState) {
		CE_LOW();
		_delay_us(140);
		send_instruction(W_ACK_PAYLOAD | pipe, payload, NULL, payload_length);	
		CE_HIGH();
		_delay_us(140);
		return RADIO_ERR_OK;
	}
	return RADIO_ERR_INVALID;
}


//the main radio task, this should be called in a loop
NRF24_MEMORY radio_error_code nrfRadio_Main(radio_context *instance) {
	
	switch(instance->currentState){
		// the radio is not already initialized, but the main loop has been called.
		case RADIO_STARTUP:
		
		break;
		//the radio stay in power down until the PWR_UP bit is set high
		case RADIO_POWER_DOWN:
		
		break;
		// the radio is in this state when the PWR_UP bit is set to one in CONFIG register (according to datasheet)
		case RADIO_STANDBY_1:
		
		break;
		//the radio enters in standby mode when it is PTX and the CE ping is held high while the TX_FIFO_BUFFER is empty
		case RADIO_STANDBY_2:
		CE_LOW();
		_delay_us(140);
		//set back the RX pipe 0 address
		set_register(RX_ADDR_P0, (uint8_t*)instance->rxAddressP0, instance->address_length);
		instance->currentState = RADIO_STANDBY_1;
		break;
		//PWR_UP bit must be set high and and PRIM_RX bit low, the TX_FIFO must be loaded and is required a high pulse on CE that is longer than 10us
		//when transmision is complete, if the CE pin is held HIGH then the radio returns to standby2 (TX_FIFO must be empty), otherwise it returns to standby1
		//for short transmission is indicated to not keep the CE pin high and the radio will stay in standby 1
		case RADIO_PTX:
			
			//we're transmitting so we expect to get the TX_DS or MAX_RT
			if(instance->irq_triggered > 0) {
				INT_GLOBAL_DIS();
				instance->irq_triggered = 0;
				INT_GLOBAL_EN();
				instance->currentState = RADIO_STANDBY_2;
				if(instance->irq_status & _BV(TX_DS)) {
					if(instance->tx_callback != NULL)
						instance->tx_callback(RADIO_TX_OK);
				}else if(instance->irq_status & _BV(MAX_RT)) {
					if(instance->tx_callback != NULL)
						instance->tx_callback(RADIO_TX_MAX_RT);
				}
				
				//there is a ACK payload
				if(instance->irq_status & _BV(RX_DR)) {
					uint8_t rec_buffer[32] = {0};
					uint8_t pipe_number = (instance->irq_status & 0xE) >> 1;
					uint8_t tmp[1];
					if(instance->dynamic_payload) {
						send_instruction(R_RX_PL_WID, (uint8_t*)tmp, (uint8_t*)tmp, 1);
						send_instruction(R_RX_PAYLOAD, (uint8_t*)rec_buffer, (uint8_t*)rec_buffer, tmp[0]);
						// TODO: check rx_callback for NULL
						instance->rx_callback(pipe_number, rec_buffer, tmp[0]);
					}
					else {
						send_instruction(R_RX_PAYLOAD, (uint8_t*)rec_buffer, (uint8_t*)rec_buffer, instance->rx_pipe_widths[pipe_number]);
						instance->rx_callback(pipe_number, rec_buffer, instance->rx_pipe_widths[pipe_number]);
					}
				}
				
			}
			
		
		break;
		//the PWR_BIT must be set and CE pin must be high during RX operation
		case RADIO_PRX:
		if(instance->irq_triggered > 0) {
			INT_GLOBAL_DIS();
			instance->irq_triggered = 0;
			
			if(instance->irq_status & _BV(RX_DR)) {
				_delay_us(200); //make sure that the radio has already send the ACK TODO: wait just if the radio is using AA
				CE_LOW(); //disable the radio
				//get the pipe number
				uint8_t rec_buffer[32] = {0};
				uint8_t pipe_number = (instance->irq_status & 0xE) >> 1;
				while(pipe_number != RADIO_PIPE_EMPTY) {
					uint8_t tmp[1];
					if(instance->dynamic_payload) {
						send_instruction(R_RX_PL_WID, (uint8_t*)tmp, (uint8_t*)tmp, 1);
						send_instruction(R_RX_PAYLOAD, (uint8_t*)rec_buffer, (uint8_t*)rec_buffer, tmp[0]);
						instance->rx_callback(pipe_number, rec_buffer, tmp[0]);
					} 
					else {
						send_instruction(R_RX_PAYLOAD, (uint8_t*)rec_buffer, (uint8_t*)rec_buffer, instance->rx_pipe_widths[pipe_number]);
						instance->rx_callback(pipe_number, rec_buffer, instance->rx_pipe_widths[pipe_number]);
					}
					//get again the radio status
					CSN_LOW();
					instance->irq_status = SPI_Write_Byte(NOP);
					CSN_HIGH();
					pipe_number = (instance->irq_status & 0xE) >> 1;
				}
				CE_HIGH();
			}
			INT_GLOBAL_EN();
			
		}
		break;
		
		default:
			return RADIO_ERR_UNKNOWN;
	}
	
	return RADIO_ERR_OK;
}

fptr_t ptrs[] RADIO_FPTRS_MEMORY = {
	(fptr_t)nrfRadio_Main,
	(fptr_t)nrfRadio_TransmitMode,
	(fptr_t)nrfRadio_Init,
	(fptr_t)nrfRadio_PowerDown,
	(fptr_t)nrfRadio_GetInfo,
	(fptr_t)nrfRadio_PipeConfig,
	(fptr_t)nrfRadio_LoadMessages,
	(fptr_t)nrfRadio_Transmit,
	(fptr_t)nrfRadio_ListeningMode,
	(fptr_t)nrfRadio_PowerUp,
	(fptr_t)nrfRadio_ChangeDataRate,
	(fptr_t)nrfRadio_ChangePower,
	(fptr_t)nrfRadio_SetTxCallback,
	(fptr_t)nrfRadio_SetRxCallback,
	(fptr_t)nrfRadio_LoadAckPayload,
	(fptr_t)nrfRadio_FlushBuffer
};