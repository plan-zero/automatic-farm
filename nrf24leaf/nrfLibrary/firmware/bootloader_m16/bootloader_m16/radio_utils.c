/*
 * CFile1.c
 *
 * Created: 11/8/2019 11:04:10 PM
 *  Author: AR
 */ 

#include "packet.h"
#include "radio_utils.h"
#include "stdint.h"
#include "string.h"

#define RADIO_PAYLOAD_SIZE 23

void RadioUtils_clearPayload(radiopacket_t *radiopck) {
	for(uint8_t idx = 0; idx < RADIO_PAYLOAD_SIZE; idx++ )
		radiopck->payload.message.messagecontent[idx] = 0;
	radiopck->payload.message.messageid = 0;
}

void RadioUtils_CopyString(radiopacket_t *radiopck, char * str) {
	if(strlen(str) <= RADIO_PAYLOAD_SIZE){
		for(uint8_t idx = 0; idx < RADIO_PAYLOAD_SIZE; idx++ )
			radiopck->payload.message.messagecontent[idx] = (uint8_t)*(str + idx);
	}
}

