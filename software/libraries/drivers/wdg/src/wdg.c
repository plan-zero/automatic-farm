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

#include "wdg.h"
#include "wdg_hw.h"

uint8_t mcusr_mirror __attribute__ ((section (".noinit")));
uint8_t sw_reset_reason __attribute__ ((section (".noinit")));

void get_mcusr(void) \
  __attribute__((naked)) \
  __attribute__((section(".init3")));
void get_mcusr(void)
{
  mcusr_mirror = WDG_STATUS_GET;
  WDG_STATUS_RESET;
  wdt_disable();
}

void wdg_init(uint8_t timeout)
{
    wdt_enable(timeout);
}

void wdg_kick()
{
    wdt_reset();
}

void wdg_disable()
{
    wdt_disable();
}

void wdg_explicit_reset(uint8_t sw_reason)
{
    sw_reset_reason = sw_reason;
    while(1);
}

wdg_hw_reset_reason wdg_get_hw_reason()
{
    if(IS_HW_BOR())
        return wdg_rr_brownoutreset;
    else if(IS_HW_EXT())
        return wdg_rr_externalreset;
    else if(IS_HW_POR())
        return wdg_rr_poweronreset;
    else if(IS_HW_WDR())
        return wdg_rr_watchdogreset;
    else if(IS_HW_JTR())
        return wdg_rr_jtagreset;
    else
        return wdg_rr_none;
}

uint8_t wdg_sw_reset_reason()
{
    return sw_reset_reason;
}