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
#include <gpio.h>
#include <utils.h>

///////////////////////////////////////////////////////////////////////////////
// Clock related registers
///////////////////////////////////////////////////////////////////////////////
#define REGISTER_CLK_ICKR REGISTER     0x50C0 // Internal clock control register
#define REGISTER_CLK_ECKR REGISTER     0x50C1 // External clock control register
#define REGISTER_CLK_CMSR REGISTER     0x50C3 // Clock master status register
#define REGISTER_CLK_SWR REGISTER      0x50C4 // Clock master switch register
#define REGISTER_CLK_SWCR REGISTER     0x50C5 // Clock switch control register
#define REGISTER_CLK_CKDIVR REGISTER   0x50C6 // Clock divider register
#define REGISTER_CLK_PCKENR1 REGISTER  0x50C7 // Peripheral clock gaing register 1
#define REGISTER_CLK_CSSR REGISTER     0x50C8 // Clock security system register
#define REGISTER_CLK_CCOR REGISTER     0x50C9 // Configurable clock control register
#define REGISTER_CLK_PCKENR2 REGISTER  0x50CA // Periphera clock gating register 2
#define REGISTER_CLK_HSITRIMR REGISTER 0x50CC // HSI clock calibration trimming register
#define REGISTER_CLK_SWIMCCR REGISTER  0x50CD // SWIM clock control register

///////////////////////////////////////////////////////////////////////////////
// Helper values for setting the HSI divider
///////////////////////////////////////////////////////////////////////////////
#define _CLK_HSI_DIV_MASK (uint8_t)0b00011000
#define _CLK_HSI_DIV_1    (uint8_t)0b00000000
#define _CLK_HSI_DIV_2    (uint8_t)0b00001000
#define _CLK_HSI_DIV_4    (uint8_t)0b00010000
#define _CLK_HSI_DIV_8    (uint8_t)0b00011000

///////////////////////////////////////////////////////////////////////////////
// Helper values for setting the CPU divider
///////////////////////////////////////////////////////////////////////////////
#define _CLK_CPU_DIV_MASK (uint8_t)0b00000111
#define _CLK_CPU_DIV_1    (uint8_t)0b00000000
#define _CLK_CPU_DIV_2    (uint8_t)0b00000001
#define _CLK_CPU_DIV_4    (uint8_t)0b00000010
#define _CLK_CPU_DIV_8    (uint8_t)0b00000011
#define _CLK_CPU_DIV_16   (uint8_t)0b00000100
#define _CLK_CPU_DIV_32   (uint8_t)0b00000101
#define _CLK_CPU_DIV_64   (uint8_t)0b00000110
#define _CLK_CPU_DIV_128  (uint8_t)0b00000111

///////////////////////////////////////////////////////////////////////////////
// Helper values for setting the CCO clock selection
///////////////////////////////////////////////////////////////////////////////
#define _CLK_CCO_SEL_MASK   (uint8_t)0b00011110
#define _CLK_CCO_SEL_HSIDIV (uint8_t)0b00000000
#define _CLK_CCO_SEL_LSI    (uint8_t)0b00000010
#define _CLK_CCO_SEL_HSE    (uint8_t)0b00000100
#define _CLK_CCO_SEL_CPU    (uint8_t)0b00001000
#define _CLK_CCO_SEL_CPU_2  (uint8_t)0b00001010
#define _CLK_CCO_SEL_CPU_4  (uint8_t)0b00001100
#define _CLK_CCO_SEL_CPU_8  (uint8_t)0b00001110
#define _CLK_CCO_SEL_CPU_16 (uint8_t)0b00010000
#define _CLK_CCO_SEL_CPU_32 (uint8_t)0b00010010
#define _CLK_CCO_SEL_CPU_64 (uint8_t)0b00010100
#define _CLK_CCO_SEL_HSI    (uint8_t)0b00010110
#define _CLK_CCO_SEL_MASTER (uint8_t)0b00011000

///////////////////////////////////////////////////////////////////////////////
// The GPIO pin of the CCO
///////////////////////////////////////////////////////////////////////////////
#define _CLK_CCO_PORT C
#define _CLK_CCO_PIN  4

///////////////////////////////////////////////////////////////////////////////
// Useful register bits
///////////////////////////////////////////////////////////////////////////////
#define _CLK_CCOR_CCOBSY (uint8_t)0b01000000
#define _CLK_CCOR_CCORDY (uint8_t)0b00100000
#define _CLK_CCOR_CCOEN  (uint8_t)0b00000001


///////////////////////////////////////////////////////////////////////////////
// Macros for setting up the clock, to be used by the user
///////////////////////////////////////////////////////////////////////////////

// Sets the internal high speed clock divider
// Parameters:
// - divider: One of 1, 2, 4 or 8
#define _clkSetHsiDivider(divider) do {\
  REGISTER_CLK_CKDIVR &= ~_CLK_HSI_DIV_MASK;\
  REGISTER_CLK_CKDIVR |= _CLK_HSI_DIV_##divider;\
} while(0)
#define clkSetHsiDivider(divider) _clkSetHsiDivider(divider)

// Sets the CPU clock divider
// Parameters:
// - divider: One of 1, 2, 4, 8, 16, 32, 64 or 128
#define _clkSetCpuDivider(divider) do {\
  REGISTER_CLK_CKDIVR &= ~_CLK_CPU_DIV_MASK;\
  REGISTER_CLK_CKDIVR |= _CLK_CPU_DIV_##divider;\
} while(0)
#define clkSetCpuDivider(divider) _clkSetCpuDivider(divider)

// Select the source for the output clock
// Parameters:
// - source: One of HSIDIV, SI, HSE, CPU, CPU_2, CPU_4, CPU_8, CPU_16, CPU_32
//           CPU_64, HSI, MASTER
#define _clkSetCcoSource(source) do {\
  REGISTER_CLK_CCOR &= ~_CLK_CCO_SEL_MASK;\
  REGISTER_CLK_CCOR |= _CLK_CCO_SEL_##source;\
} while(0)
#define clkSetCcoSource(source) _clkSetCcoSource(source)

#define clkEnableCco() do {\
  gpioSetAsOutput(_CLK_CCO_PORT, _CLK_CCO_PIN);\
  gpioSetAsPushPull(_CLK_CCO_PORT, _CLK_CCO_PIN);\
  REGISTER_SET(REGISTER_CLK_CCOR, _CLK_CCOR_CCOEN);\
} while(0)

// Disable the clock output
#define clkDisableCco() REGISTER_UNSET(REGISTER_CLK_CCOR, _CLK_CCOR_CCOEN)

#endif /* STM8_CLK_H */

