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

radio_error_code (*fptr_nrfRadio_Main)(void) = (radio_error_code(*)(void))0x17F0;
radio_error_code (*fptr_nrfRadio_TransmitMode)(void) = (radio_error_code(*)(void))0x17F1;
radio_error_code (*fptr_nrfRadio_Init)(radio_config) = (radio_error_code(*)(radio_config))0x17F2;



#endif /* RADIO_FPTR_H_ */