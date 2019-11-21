/*
 * radio_utils.h
 *
 * Created: 11/8/2019 11:05:07 PM
 *  Author: AR
 */ 


#ifndef RADIO_UTILS_H_
#define RADIO_UTILS_H_

__attribute__((section(".shared"))) void RadioUtils_clearPayload(radiopacket_t *radiopck);
__attribute__((section(".shared"))) void RadioUtils_CopyString(radiopacket_t *radiopck, char * str);


#endif /* RADIO_UTILS_H_ */