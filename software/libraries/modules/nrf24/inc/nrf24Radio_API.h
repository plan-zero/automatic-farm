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


/*
 * radio_fptr.h
 *
 * Created: 11/26/2019 12:23:25 AM
 *  Author: AR
 */ 


#ifndef RADIO_FPTR_H_
#define RADIO_FPTR_H_
#include <stdint.h>
#include "nrf24Radio.h"

extern radio_context _radio_instance;

extern radio_error_code (* const __flash *fptr_nrfRadio_Main)(radio_context *);
extern radio_error_code (* const __flash *fptr_nrfRadio_TransmitMode)(radio_context *);
extern radio_error_code (* const __flash *fptr_nrfRadio_Init)(radio_context *, radio_config);
extern radio_error_code (* const __flash *fptr_nrfRadio_PowerDown)(radio_context *);
extern radio_error_code (* const __flash *fptr_nrfRadio_GetInfo)(radio_context *, radio_registers*);
extern radio_error_code (* const __flash *fptr_nrfRadio_PipeConfig)(radio_context *, pipe_config);
extern radio_error_code (* const __flash *fptr_nrfRadio_LoadMessages)(radio_context *, uint8_t*, uint8_t);
extern radio_tx_status (* const __flash *fptr_nrfRadio_Transmit)(radio_context *, uint8_t*, radio_transmision_type);
extern radio_error_code (* const __flash *fptr_nrfRadio_ListeningMode)(radio_context *);
extern radio_error_code (* const __flash *fptr_nrfRadio_PowerUp)(radio_context *);
extern radio_error_code (* const __flash *fptr_nrfRadio_ChangeDataRate)(radio_context *, radio_data_rate);
extern radio_error_code (* const __flash *fptr_nrfRadio_ChangePower)(radio_context *, radio_rf_power);
extern radio_error_code (* const __flash *fptr_nrfRadio_SetTxCallback)(radio_context *, void (*)(radio_tx_status));
extern radio_error_code (* const __flash *fptr_nrfRadio_SetRxCallback)(radio_context *, void (*)(uint8_t, uint8_t *, uint8_t));
extern radio_error_code (* const __flash *fptr_nrfRadio_LoadAckPayload)(radio_context *, radio_pipe, uint8_t*, uint8_t);
extern radio_error_code (* const __flash *fptr_nrfRadio_FlushBuffer)(radio_context *, radio_buffer_t buffer);
//**************************
// The main routine of Radio library which is used for asynchronous operations
// Method:	__nrfRadio_Main
// Returns:	radio_error_code
//************************** 
#define __nrfRadio_Main() (*fptr_nrfRadio_Main)(&_radio_instance)
//**************************
// Set the Radio in PTX mode, if the TX buffer is empty, the radio will enter in standby1
// Method:	__nrfRadio_TransmitMode
// Returns:	radio_error_code
//**************************
#define __nrfRadio_TransmitMode() (*fptr_nrfRadio_TransmitMode)(&_radio_instance)
//**************************
// Initialize the Radio library - also this initialize the SPI driver that is used by Radio
// Method:		__nrfRadio_Init
// Returns:		radio_error_code
// cfg:			radio_config
//**************************
#define __nrfRadio_Init(cfg) (*fptr_nrfRadio_Init)(&_radio_instance, cfg)
//**************************
// Set the radio in power done mode - This is different than Standby1 or 2.
// Method:		__nrfRadio_PowerDown
// Returns:		radio_error_code
//**************************
#define __nrfRadio_PowerDown() (*fptr_nrfRadio_PowerDown)(&_radio_instance)
//**************************
// Read all Radio's internal registers. This is used for debugging, not indicated to be used during operation
// Method:		__nrfRadio_GetInfo
// Returns:		radio_error_code
// registers:   radio_registers
//************************** 
#define __nrfRadio_GetInfo(registers) (*fptr_nrfRadio_GetInfo)(&_radio_instance, registers)
//**************************
// This configures one of Radio's pipes with the given configuration. Don't use this before the radio is initialized. Also this should be use during power-down / Standby1
// Method:		__nrfRadio_PipeConfig
// Returns:		radio_error_code
// pipe_cfg:	pipe_config
//**************************
#define __nrfRadio_PipeConfig(pipe_cfg) (*fptr_nrfRadio_PipeConfig)(&_radio_instance, pipe_cfg)
//**************************
// Load the payload in the Radio TX FIFO. This should be called when the radio is in Standby1
// Method:		__nrfRadio_LoadMessages
// Returns:		radio_error_code
// msg:			uint8_t*, radio payload
// length:		uint8_t, the length of radio payload, the maximum allowed length is 32B and the minimum is 1B
//**************************
#define __nrfRadio_LoadMessages(msg,length) (*fptr_nrfRadio_LoadMessages)(&_radio_instance, msg,length)
//**************************
// Send the payload to the given TX address. The radio must be in transmitting mode before this API is used.
// Method:		__nrfRadio_Transmit
// Returns:		radio_tx_status, TX_OK if ACK is received, TX_MAX_RT if the ACK is not received (after all retransmissions) and TX_OK_ACK_PYL if the ACK has a payload attached
// txaddr:		uint8_t*, the address of destination node, the maximum allowed length is 5
// txtrans:		radio_transmision_type, the transmission can be asynchronously (the __nrfRadio_Main have to be executed in this case and ACK is reported via CB), or synchronously (blocks until transmission is done) 
//**************************
#define __nrfRadio_Transmit(txaddr,txtrans) (*fptr_nrfRadio_Transmit)(&_radio_instance, txaddr,txtrans)
//**************************
// Put the radio in PRX mode, this mode is consuming and it should be used as little as possible
// Method:		__nrfRadio_ListeningMode
// Returns:		radio_error_code
//**************************
#define __nrfRadio_ListeningMode() (*fptr_nrfRadio_ListeningMode)(&_radio_instance)
//**************************
// Put the radio in power down. During this mode you can reconfigure the pipes / radio
// Method:		__nrfRadio_PowerUp
// Returns:		radio_error_code
//**************************
#define __nrfRadio_PowerUp() (*fptr_nrfRadio_PowerUp)(&_radio_instance)
//**************************
// Change the radio transmission rate. Using lower rates will increase the radio range
// Method:		__nrfRadio_ChangeDataRate
// Returns:		radio_error_code
// datarate:	radio_data_rate
//**************************
#define __nrfRadio_ChangeDataRate(datarate) (*fptr_nrfRadio_ChangeDataRate)(&_radio_instance, datarate)
//**************************
// Change the radio transmission power. This must be called when the radio is in power down
// Method:		__nrfRadio_ChangePower
// Returns:		radio_error_code
// pwr:			radio_rf_power
//**************************
#define __nrfRadio_ChangePower(pwr) (*fptr_nrfRadio_ChangePower)(&_radio_instance, pwr)
//**************************
// Set the callback for TX. This is called during nrfRadio_Main execution when the transmission is complete
// Method:		__nrfRadio_SetTxCallback
// Returns:		radio_error_code
// pwr:			void (*)(radio_tx_status)
//**************************
#define __nrfRadio_SetTxCallback(tx_cb) (*fptr_nrfRadio_SetTxCallback)(&_radio_instance, tx_cb)
//**************************
// Set the callback for RX. This is called during nrfRadio_Main execution when the reception is complete
// Method:		__nrfRadio_SetRxCallback
// Returns:		radio_error_code
// pwr:			void (*)(uint8_t, uint8_t *, uint8_t)
//**************************
#define __nrfRadio_SetRxCallback(rx_cb) (*fptr_nrfRadio_SetRxCallback)(&_radio_instance, rx_cb)
//**************************
// Upload the ACK payload in advance, so when the radio receives the message, the payload should be already prepared.
// Method:		__nrfRadio_LoadAckPayload
// Returns:		radio_error_code
// ackpipe:		radio_pipe, the pipe which the ack payload is sent e.g. if the message was received on PIPE3 then the ack payload have to be already loaded for PIPE3
// ackpayload:	uint8_t*, the pointer to the ACK payload message
// ackpayload_length: uint8_t, the size of ACK payload
//**************************
#define __nrfRadio_LoadAckPayload(ackpipe,ackpayload,ackpayload_length) (*fptr_nrfRadio_LoadAckPayload)(&_radio_instance, ackpipe, ackpayload, ackpayload_length)
//**************************
// Used to flush the current RX/TX buffer - check the mode of transmision before you do this
// Method:		__nrfRadio_FlushBuffer
// Returns:		radio_error_code
// buffer_type:	radio_buffer_t: RADIO_RX_BUFFER, RADIO_TX_BUFFER, RADIO_BOTH_BUFFER (tx+rx buffers)
//**************************
#define __nrfRadio_FlushBuffer(buffer_type) fptr_nrfRadio_FlushBuffer(&_radio_instance, buffer_type)

#endif /* RADIO_FPTR_H_ */