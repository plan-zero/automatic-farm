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
 * Main.c
 *
 *  Created on: Feb 7, 2020
 *      Author: Adi
 */

#include "stdlib.h"
#include "Scheduler.h"
#include "Communication.h"
#include "ota.h"

#include "timer.h"
#include "interrupt_hw.h"
#include "stdint.h"
#include "uart.h"
#include "wdg.h"
#include "e2p.h"
#include "nrf24Radio.h"
#include "nrf24Radio_API.h"
#include "radio_link.h"
#include "network_common.h"
#include "system-timer.h"

#ifdef ENABLE_TASK_TEST
#include <avr/io.h>
#include <util/delay.h>
void test_task_1ms()
{
    static int init_led = 0;
    if(!init_led)
    {
        init_led = 1;
        DDRD |= 1;
    }
    PORTD ^= 1;
    
}
void test_task_10ms()
{
    static int init_led = 0;
    if(!init_led)
    {
        init_led = 1;
        DDRD |= 2;
    }
    PORTD ^= 2;
    
}

void test_task_1s()
{
    static int init_led = 0;
    if(!init_led)
    {
        init_led = 1;
        DDRB |= 1;
    }
    PORTB ^= 1;
    
} 
#endif

void notify1ms(uint8_t ch);

volatile uint8_t oneMsFlag = 0;
volatile uint8_t trigger_wdg_reset = 0;

voidFunctionTypeVoid oneMsTask = NULL;

inline void MAIN_setup()
{
    //uart initialization
    uart_init(UART_9600BAUD, UART_8MHZ, UART_PARITY_NONE);
    uart_printString("Application", 1);
    //Timer initialization
    timer_interrupt ti;
    ti.output_cmp_match_en = 1;
    timer_type inst_tim_t = timer_get_type(0);
    timer_cfg tc = {
        timer_mode_ctc,
        timer_normal_operationm,
        timer_prescaler_64,
        0, //initial counter value
        85, //initial output cmp value
        0, //initial input cmp value
        inst_tim_t, //timer type
        timer_ch_a,//target timer channel
        ti,
    };
    timer_init(0,tc);
    timer_register_callback(0, (timer_callback) notify1ms, 1);
    
    oneMsTask = scheduler_getPointerTo1msTask();
    scheduler_add_task(sch_type_task_10ms, radio_link_task);
    scheduler_add_task(sch_type_task_10ms, communication_execute_messages);
    scheduler_add_task(sch_type_task_100ms, communication_send_messages);
#ifdef ENABLE_TASK_TEST
    //scheduler_add_task(sch_type_task_1s, radio_link_task);
#endif
    //WDG initialization
    uint8_t tmp = wdg_sw_reset_reason();
    uart_printString("wdg previous sw reset: ", 0);
    uart_printRegister(tmp);
    uartNewLine();
    tmp = wdg_get_hw_reason();
    uart_printString("wdg previous hw reset: ", 0);
    uart_printRegister(tmp);
    uartNewLine();
    

    //Radio communication initialization
	radio_config cfg = {
		RADIO_ADDRESS_5BYTES,
		RADIO_RT,
		RADIO_RETRANSMIT_15,
		CHANNEL_112,
		RADIO_RATE,
		RADIO_CRC2_ENABLED,
		RADIO_COUNT_WAVE_DISABLED,
		RADIO_HIGHEST_0DBM,
		RADIO_DYNAMIC_PAYLOAD_ENABLED,
		RADIO_ACK_PAYLOAD_ENABLED,
		RADIO_DYNAMIC_ACK_DISABLED,
		RADIO_APPLICATION
	};
	__nrfRadio_Init(cfg);
	__nrfRadio_FlushBuffer(RADIO_BOTH_BUFFER);

	__nrfRadio_SetRxCallback(rx_handler);


    //read BOOT key
    ota_get_key();
    //read network parameters
    network_read_parameters();
    uint8_t addr_tmp[6] = {0};
    addr_tmp[5] = '\0';
    memcpy(addr_tmp, network_rx_default_address, 5);
    uart_printString(addr_tmp, 1);
    //start the scheduler
    timer_start(0,0);
    //init and start the system timer used for measurments
    system_timer_init();
    system_timer_start();
}

// We use timer on channel 0
void notify1ms(uint8_t ch)
{
    if (ch ==  0)
    {
        oneMsFlag += 1;
        if (oneMsFlag >= 10)
        {
            trigger_wdg_reset = 1;
        }
    }
}


int  main()
{
    MAIN_setup();
    INT_GLOBAL_EN();

    wdg_init(wdgto_1S);
    while(1)
    {

        wdg_kick();
        if (oneMsFlag != 0)
        {
            oneMsFlag--;
            if(oneMsTask)
                oneMsTask();
        }
        if(trigger_wdg_reset)
        {
            //do wdg reset
            //wdg_explicit_reset(0x01);
        }
        __nrfRadio_Main();
    }
    return 0;
}