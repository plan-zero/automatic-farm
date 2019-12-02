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


extern radio_error_code (* const __flash *fptr_nrfRadio_Main)(void);
extern radio_error_code (* const __flash *fptr_nrfRadio_TransmitMode)(void);
extern radio_error_code (* const __flash *fptr_nrfRadio_Init)(radio_config);
extern radio_error_code (* const __flash *fptr_nrfRadio_PowerDown)(void);
extern radio_error_code (* const __flash *fptr_nrfRadio_GetInfo)(radio_registers*);
extern radio_error_code (* const __flash *fptr_nrfRadio_PipeConfig)(pipe_config);
extern radio_error_code (* const __flash *fptr_nrfRadio_LoadMessages)(uint8_t*, uint8_t);
extern radio_tx_status (* const __flash *fptr_nrfRadio_Transmit)(uint8_t*, radio_transmision_type);
extern radio_error_code (* const __flash *fptr_nrfRadio_ListeningMode)(void);
extern radio_error_code (* const __flash *fptr_nrfRadio_PowerUp)(void);
extern radio_error_code (* const __flash *fptr_nrfRadio_ChangeDataRate)(radio_data_rate);
extern radio_error_code (* const __flash *fptr_nrfRadio_ChangePower)(radio_rf_power);
extern radio_error_code (* const __flash *fptr_nrfRadio_SetTxCallback)(void (*)(radio_tx_status));
extern radio_error_code (* const __flash *fptr_nrfRadio_SetRxCallback)(void (*)(uint8_t, uint8_t *, uint8_t));
extern radio_error_code (* const __flash *fptr_nrfRadio_LoadAckPayload)(radio_pipe, uint8_t*, uint8_t);


// TODO: replace x,y,z with variable names (visual studio displays the names)

//**************************
// The main routine of Radio library which is used for asynchronous operations
// Method:	__nrfRadio_Main
// Returns:	radio_error_code
//************************** 
#define __nrfRadio_Main() (*fptr_nrfRadio_Main)()
//**************************
// Set the Radio in PTX mode, if the TX buffer is empty, the radio will enter in standby1
// Method:	__nrfRadio_TransmitMode
// Returns:	radio_error_code
//**************************
#define __nrfRadio_TransmitMode() (*fptr_nrfRadio_TransmitMode)()
//**************************
// Initialize the Radio library - also this initialize the SPI driver that is used by Radio
// Method:		__nrfRadio_Init
// Returns:		radio_error_code
// cfg:			radio_config
//**************************
#define __nrfRadio_Init(cfg) (*fptr_nrfRadio_Init)(cfg)
//**************************
// Set the radio in power done mode - This is different than Standby1 or 2.
// Method:		__nrfRadio_PowerDown
// Returns:		radio_error_code
//**************************
#define __nrfRadio_PowerDown() (*fptr_nrfRadio_PowerDown)()
//**************************
// Read all Radio's internal registers. This is used for debugging, not indicated to be used during operation
// Method:		__nrfRadio_GetInfo
// Returns:		radio_error_code
// registers:   radio_registers
//************************** 
#define __nrfRadio_GetInfo(registers) (*fptr_nrfRadio_GetInfo)(registers)
//**************************
// This configures one of Radio's pipes with the given configuration. Don't use this before the radio is initialized. Also this should be use during power-down / Standby1
// Method:		__nrfRadio_PipeConfig
// Returns:		radio_error_code
// pipe_cfg:	pipe_config
//**************************
#define __nrfRadio_PipeConfig(pipe_cfg) (*fptr_nrfRadio_PipeConfig)(pipe_cfg)
//**************************
// Load the payload in the Radio TX FIFO. This should be called when the radio is in Standby1
// Method:		__nrfRadio_LoadMessages
// Returns:		radio_error_code
// msg:			uint8_t*, radio payload
// length:		uint8_t, the length of radio payload, the maximum allowed length is 32B and the minimum is 1B
//**************************
#define __nrfRadio_LoadMessages(msg,length) (*fptr_nrfRadio_LoadMessages)(msg,length)
//**************************
// Send the payload to the given TX address. The radio must be in transmitting mode before this API is used.
// Method:		__nrfRadio_Transmit
// Returns:		radio_tx_status, TX_OK if ack is received, TX_MAX_RT if the ack is not received (after all retransmisions) and TX_OK_ACK_PYL if the ACK has a payload attached
// txaddr:		uint8_t*, the address of destination node, the maximum allowed length is 5
// txtrans:		radio_transmision_type, the transmission can be asynchronously (the __nrfRadio_Main have to be executed in this case and ack is reported via CB), or synchronously  
//**************************
#define __nrfRadio_Transmit(txaddr,txtrans) (*fptr_nrfRadio_Transmit)(txaddr,txtrans)
//**************************
// Put the radio in PRX mode, this mode is consuming and it should be used as little as possible
// Method:		__nrfRadio_ListeningMode
// Returns:		radio_error_code
//**************************
#define __nrfRadio_ListeningMode() (*fptr_nrfRadio_ListeningMode)()
//**************************
// Put the radio in power down. During this mode you can reconfigure the pipes / radio
// Method:		__nrfRadio_PowerUp
// Returns:		radio_error_code
//**************************
#define __nrfRadio_PowerUp() (*fptr_nrfRadio_PowerUp)()
//**************************
// Change the radio transmission rate. Using lower rates will increase the radio range
// Method:		__nrfRadio_ChangeDataRate
// Returns:		radio_error_code
// datarate:	radio_data_rate
//**************************
#define __nrfRadio_ChangeDataRate(datarate) (*fptr_nrfRadio_ChangeDataRate)(datarate)
#define __nrfRadio_ChangePower(x) (*fptr_nrfRadio_ChangePower)(x)
#define __nrfRadio_SetTxCallback(x) (*fptr_nrfRadio_SetTxCallback)(x)
#define __nrfRadio_SetRxCallback(x) (*fptr_nrfRadio_SetRxCallback)(x)
#define __nrfRadio_LoadAckPayload(x,y,z) (*fptr_nrfRadio_LoadAckPayload)(x,y,z)


#endif /* RADIO_FPTR_H_ */