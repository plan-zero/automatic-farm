/*
 * IncFile1.h
 *
 * Created: 11/28/2019 11:59:14 PM
 *  Author: Adi
 */ 


#ifndef INCFILE1_H_
#define INCFILE1_H_

#define F_CPU 4000000UL

#include <util/delay.h>
#include <avr/io.h>
#include <avr/boot.h>
#include <avr/eeprom.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <string.h>

#include "E2P_Layout.h"
#define LED_PORT_DIR DDRB
#define LED_PORT_PIN PINB0

#define TURN_LED_ON		PORTB |= 1
#define TURN_LED_OFF	PORTB &= 0xFE
#define TOGGLE_LED		PORTB ^= 1


#endif /* INCFILE1_H_ */