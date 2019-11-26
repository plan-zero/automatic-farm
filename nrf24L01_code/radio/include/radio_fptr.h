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

#define NRF_LIB_HOOKS_BASE_ADDR	0x2FC0

radio_error_code (* const __flash *fptr_nrfRadio_Main)(void) = (radio_error_code(* const __flash *)(void))NRF_LIB_HOOKS_BASE_ADDR;
radio_error_code (* const __flash *fptr_nrfRadio_TransmitMode)(void) = (radio_error_code(* const __flash *)(void))(NRF_LIB_HOOKS_BASE_ADDR + 2);
radio_error_code (* const __flash *fptr_nrfRadio_Init)(radio_config) = (radio_error_code(* const __flash *)(radio_config))(NRF_LIB_HOOKS_BASE_ADDR + 4);
radio_error_code (* const __flash *fptr_nrfRadio_PowerDown)(void) = (radio_error_code(* const __flash *)(void))(NRF_LIB_HOOKS_BASE_ADDR + 6);
radio_error_code (* const __flash *fptr_nrfRadio_GetInfo)(radio_registers*) = (radio_error_code(* const __flash *)(radio_registers*))(NRF_LIB_HOOKS_BASE_ADDR + 8);
radio_error_code (* const __flash *fptr_nrfRadio_PipeConfig)(pipe_config) = (radio_error_code(* const __flash *)(pipe_config))(NRF_LIB_HOOKS_BASE_ADDR + 10);
radio_error_code (* const __flash *fptr_nrfRadio_LoadMessages)(uint8_t*, uint8_t) = (radio_error_code(* const __flash *)(uint8_t*, uint8_t))(NRF_LIB_HOOKS_BASE_ADDR + 12);
radio_tx_status (* const __flash *fptr_nrfRadio_Transmit)(uint8_t*, radio_transmision_type) = (radio_tx_status(* const __flash *)(uint8_t*, radio_transmision_type))(NRF_LIB_HOOKS_BASE_ADDR + 14);
radio_error_code (* const __flash *fptr_nrfRadio_ListeningMode)(void) = (radio_error_code(* const __flash *)(void))(NRF_LIB_HOOKS_BASE_ADDR + 16);
radio_error_code (* const __flash *fptr_nrfRadio_PowerUp)(void) = (radio_error_code(* const __flash *)(void))(NRF_LIB_HOOKS_BASE_ADDR + 18);
radio_error_code (* const __flash *fptr_nrfRadio_ChangeDataRate)(radio_data_rate) = (radio_error_code(* const __flash *)(radio_data_rate))(NRF_LIB_HOOKS_BASE_ADDR + 20);
radio_error_code (* const __flash *fptr_nrfRadio_ChangePower)(radio_rf_power) = (radio_error_code(* const __flash *)(radio_rf_power))(NRF_LIB_HOOKS_BASE_ADDR + 22);
radio_error_code (* const __flash *fptr_nrfRadio_SetTxCallback)(void (*)(radio_tx_status)) = (radio_error_code(* const __flash *)(void (*)(radio_tx_status)))(NRF_LIB_HOOKS_BASE_ADDR + 24);
radio_error_code (* const __flash *fptr_nrfRadio_SetRxCallback)(void (*)(uint8_t, uint8_t *, uint8_t)) = (radio_error_code(* const __flash *)(void (*)(uint8_t, uint8_t *, uint8_t)))(NRF_LIB_HOOKS_BASE_ADDR + 26);
radio_error_code (* const __flash *fptr_nrfRadio_LoadAckPayload)(radio_pipe, uint8_t*, uint8_t) = (radio_error_code(* const __flash *)(radio_pipe, uint8_t*, uint8_t))(NRF_LIB_HOOKS_BASE_ADDR + 28);

#define __nrfRadio_Main() (*fptr_nrfRadio_Main)()
#define __nrfRadio_TransmitMode() (*fptr_nrfRadio_TransmitMode)()
#define __nrfRadio_Init(x) (*fptr_nrfRadio_Init)(x)
#define __nrfRadio_PowerDown() (*fptr_nrfRadio_PowerDown)()
#define __nrfRadio_GetInfo(x) (*fptr_nrfRadio_GetInfo)(x)
#define __nrfRadio_PipeConfig(x) (*fptr_nrfRadio_PipeConfig)(x)
#define __nrfRadio_LoadMessages(x,y) (*fptr_nrfRadio_LoadMessages)(x,y)
#define __nrfRadio_Transmit(x,y) (*fptr_nrfRadio_Transmit)(x,y)
#define __nrfRadio_ListeningMode() (*fptr_nrfRadio_ListeningMode)()
#define __nrfRadio_PowerUp() (*fptr_nrfRadio_PowerUp)()
#define __nrfRadio_ChangeDataRate(x) (*fptr_nrfRadio_ChangeDataRate)(x)
#define __nrfRadio_ChangePower(x) (*fptr_nrfRadio_ChangePower)(x)
#define __nrfRadio_SetTxCallback(x) (*fptr_nrfRadio_SetTxCallback)(x)
#define __nrfRadio_SetRxCallback(x) (*fptr_nrfRadio_SetRxCallback)(x)
#define __nrfRadio_LoadAckPayload(x,y,z) (*fptr_nrfRadio_LoadAckPayload)(x,y,z)


#endif /* RADIO_FPTR_H_ */