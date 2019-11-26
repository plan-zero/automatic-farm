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

radio_error_code (* const __flash *fptr_nrfRadio_Main)(void) = (radio_error_code(* const __flash *)(void))0x2FE0;
radio_error_code (* const __flash *fptr_nrfRadio_TransmitMode)(void) = (radio_error_code(* const __flash *)(void))0x2FE2;
radio_error_code (* const __flash *fptr_nrfRadio_Init)(radio_config) = (radio_error_code(* const __flash *)(radio_config))0x2FE4;



#endif /* RADIO_FPTR_H_ */