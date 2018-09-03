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
 * File:   tim4.h
 * Author: Nikolaos Apostolakos <nikoapos@gmail.com>
 *
 * Created on August 31, 2018, 9:13 AM
 */

#ifndef STM8_TIM4_H
#define STM8_TIM4_H

#include <stm8.h>
#include <utils.h>

///////////////////////////////////////////////////////////////////////////////
// TIM4 registers
///////////////////////////////////////////////////////////////////////////////
#define REGISTER_TIM4_CR1  REGISTER 0x5340 // Control register 1
#define REGISTER_TIM4_IER  REGISTER 0x5343 // Interrupt enable register
#define REGISTER_TIM4_SR   REGISTER 0x5344 // Status register
#define REGISTER_TIM4_EGR  REGISTER 0x5345 // Even generation register
#define REGISTER_TIM4_CNTR REGISTER 0x5346 // Counter
#define REGISTER_TIM4_PSCR REGISTER 0x5347 // Prescaler register
#define REGISTER_TIM4_ARR  REGISTER 0x5348 // Auto-reoad register

///////////////////////////////////////////////////////////////////////////////
// TIM4 register flags
///////////////////////////////////////////////////////////////////////////////
#define TIM4_CR1_ARPE (uint8_t) 0b10000000 // Auto-reload preload enable
#define TIM4_CR1_OPM  (uint8_t) 0b00001000 // One-pulse mode
#define TIM4_CR1_URS  (uint8_t) 0b00000100 // Update request source
#define TIM4_CR1_UDIS (uint8_t) 0b00000010 // Update disable
#define TIM4_CR1_CEN  (uint8_t) 0b00000001 // Counter enable
#define TIM4_IER_UIE  (uint8_t) 0b00000001 // Update interrupt enable
#define TIM4_SR_UIF   (uint8_t) 0b00000001 // Update interrupt flag
#define TIM4_EGR_UG   (uint8_t) 0b00000001 // Update generation

///////////////////////////////////////////////////////////////////////////////
// TIM4 prescaler values
///////////////////////////////////////////////////////////////////////////////
#define _TIM4_PRESCALER_MASK (uint8_t) 0b00000111
#define TIM4_PRESCALER_1     (uint8_t) 0b00000000
#define TIM4_PRESCALER_2     (uint8_t) 0b00000001
#define TIM4_PRESCALER_4     (uint8_t) 0b00000010
#define TIM4_PRESCALER_8     (uint8_t) 0b00000011
#define TIM4_PRESCALER_16    (uint8_t) 0b00000100
#define TIM4_PRESCALER_32    (uint8_t) 0b00000101
#define TIM4_PRESCALER_64    (uint8_t) 0b00000110
#define TIM4_PRESCALER_128   (uint8_t) 0b00000111


///////////////////////////////////////////////////////////////////////////////
// Macros for using the TIM4 by the user
///////////////////////////////////////////////////////////////////////////////

// Sets the TIM4 prescaler
// Parameters:
// - value: One of the TIM4_PRESCALER_***
#define _tim4SetPrescaler(value) do {\
  registerUnset(REGISTER_TIM4_PSCR, _TIM4_PRESCALER_MASK);\
  registerSet(REGISTER_TIM4_PSCR, value);\
} while(0)
#define tim4SetPrescaler(value) _tim4SetPrescaler(value)

// Sets the TIM4 auto-reload value
// Parameters:
// - value: A uint8_t with the value where overflow will happen
#define _tim4SetPeriod(value) REGISTER_TIM4_ARR = value
#define tim4SetPeriod(value) _tim4SetPeriod(value)

// Clears the update interrupt flag
#define tim4ClearUpdateInterruptFlag() registerUnset(REGISTER_TIM4_SR, TIM4_SR_UIF)

// Enables the update interrupt
#define tim4EnableInterrupt() do {\
  tim4ClearUpdateInterruptFlag();\
  registerSet(REGISTER_TIM4_IER, TIM4_IER_UIE);\
} while(0)

// Starts the TIM4 timer
#define tim4Start() registerSet(REGISTER_TIM4_CR1, TIM4_CR1_CEN)

#endif /* STM8_TIM4_H */

