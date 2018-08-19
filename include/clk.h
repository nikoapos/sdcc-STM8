/*
 * Copyright (C) 2018 Nikolaos Apostolakos <nikoapos@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/* 
 * File:   clk.h
 * Author: Nikolaos Apostolakos <nikoapos@gmail.com>
 *
 * Created on August 19, 2018, 12:59 PM
 */

#ifndef STM8_CLK_H
#define STM8_CLK_H

#include <stm8.h>

///////////////////////////////////////////////////////////////////////////////
// Clock related registers
///////////////////////////////////////////////////////////////////////////////
#define REGISTER_CLK_ICKR REGISTER 0x50C0 // Internal clock control register
#define REGISTER_CLK_ECKR REGISTER 0x50C1 // External clock control register
#define REGISTER_CLK_CMSR REGISTER 0x50C3 // Clock master status register
#define REGISTER_CLK_SWR REGISTER 0x50C4 // Clock master switch register
#define REGISTER_CLK_SWCR REGISTER 0x50C5 // Clock switch control register
#define REGISTER_CLK_CKDIVR REGISTER 0x50C6 // Clock divider register
#define REGISTER_CLK_PCKENR1 REGISTER 0x50C7 // Peripheral clock gaing register 1
#define REGISTER_CLK_CSSR REGISTER 0x50C8 // Clock security system register
#define REGISTER_CLK_CCOR REGISTER 0x50C9 // Configurable clock control register
#define REGISTER_CLK_PCKENR2 REGISTER 0x50CA // Periphera clock gating register 2
#define REGISTER_CLK_HSITRIMR REGISTER 0x50CC // HSI clock calibration trimming register
#define REGISTER_CLK_SWIMCCR REGISTER 0x50CD // SWIM clock control register

///////////////////////////////////////////////////////////////////////////////
// Helper values for setting the HSI divider
///////////////////////////////////////////////////////////////////////////////
#define _CLK_HSI_DIV_MASK (uint8_t)0b00011000
#define _CLK_HSI_DIV_1 (uint8_t)0b00000000
#define _CLK_HSI_DIV_2 (uint8_t)0b00001000
#define _CLK_HSI_DIV_4 (uint8_t)0b00010000
#define _CLK_HSI_DIV_8 (uint8_t)0b00011000

///////////////////////////////////////////////////////////////////////////////
// Helper values for setting the CPU divider
///////////////////////////////////////////////////////////////////////////////
#define _CLK_CPU_DIV_MASK (uint8_t)0b00000111
#define _CLK_CPU_DIV_1 (uint8_t)0b00000000
#define _CLK_CPU_DIV_2 (uint8_t)0b00000001
#define _CLK_CPU_DIV_4 (uint8_t)0b00000010
#define _CLK_CPU_DIV_8 (uint8_t)0b00000011
#define _CLK_CPU_DIV_16 (uint8_t)0b00000100
#define _CLK_CPU_DIV_32 (uint8_t)0b00000101
#define _CLK_CPU_DIV_64 (uint8_t)0b00000110
#define _CLK_CPU_DIV_128 (uint8_t)0b00000111


///////////////////////////////////////////////////////////////////////////////
// Macros for setting up the clock, to be used by the user
///////////////////////////////////////////////////////////////////////////////

// Sets the internal high speed clock divider
// Parameters:
// - divider: One of 1, 2, 4 or 8
#define _CLK_SET_HSI_DIVIDER(divider) do {\
  REGISTER_CLK_CKDIVR &= ~_CLK_HSI_DIV_MASK;\
  REGISTER_CLK_CKDIVR |= _CLK_HSI_DIV_##divider;\
} while(0)
#define CLK_SET_HSI_DIVIDER(divider) _CLK_SET_HSI_DIVIDER(divider)

// Sets the CPU clock divider
// Parameters:
// - divider: One of 1, 2, 4, 8, 16, 32, 64 or 128
#define _CLK_SET_CPU_DIVIDER(divider) do {\
  REGISTER_CLK_CKDIVR &= ~_CLK_CPU_DIV_MASK;\
  REGISTER_CLK_CKDIVR |= _CLK_CPU_DIV_##divider;\
} while(0)
#define CLK_SET_CPU_DIVIDER(divider) _CLK_SET_CPU_DIVIDER(divider)

#endif /* STM8_CLK_H */

