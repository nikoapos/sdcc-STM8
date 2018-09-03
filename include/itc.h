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
 * File:   itc.h
 * Author: Nikolaos Apostolakos <nikoapos@gmail.com>
 *
 * Created on August 18, 2018, 7:22 PM
 */

#ifndef STM8_ITC_H
#define STM8_ITC_H

#include <stm8.h>

///////////////////////////////////////////////////////////////////////////////
// Interrupt IRQ numbers
///////////////////////////////////////////////////////////////////////////////
#define ITC_IRQ_TLI          0 // External top level interrupt
#define ITC_IRQ_AWU          1 // Auto wake up from halt
#define ITC_IRQ_CLK          2 // Clock controller
#define ITC_IRQ_EXTI0        3 // Port A external interrupts
#define ITC_IRQ_PORTA        3 // Port A external interrupts
#define ITC_IRQ_EXTI1        4 // Port B external interrupts
#define ITC_IRQ_PORTB        4 // Port B external interrupts
#define ITC_IRQ_EXTI2        5 // Port C external interrupts
#define ITC_IRQ_PORTC        5 // Port C external interrupts
#define ITC_IRQ_EXTI3        6 // Port D external interrupts
#define ITC_IRQ_PORTD        6 // Port D external interrupts
#define ITC_IRQ_EXTI4        7 // Port E external interrupts
#define ITC_IRQ_PORTE        7 // Port E external interrupts
#define ITC_IRQ_SPI          10 // End of transfer
#define ITC_IRQ_TIM1_UPD_OVF 11 // TIM1 update/overflow/underflow/trigger/break
#define ITC_IRQ_TIM1_CPT_CMP 12 // TIM1 capture/compare
#define ITC_IRQ_TIM2_UPD_OVF 13 // TIM2 update/overflow
#define ITC_IRQ_TIM2_CPT_CMP 14 // TIM2 capture/compare
#define ITC_IRQ_UART1_TX     17 // Tx complete
#define ITC_IRQ_UART1_RX     18 // Receive register DATA FULL
#define ITC_IRQ_I2C          19 // I2C interrupt
#define ITC_IRQ_ADC1         22 // ADC1 end of conversion/analog watchdog interrupt
#define ITC_IRQ_TIM4_UPD_OVF 23 // TIM4 update/overflow
#define ITC_IRQ_FLASH        24 // EOP/WR_PG_DIS

///////////////////////////////////////////////////////////////////////////////
// Interrupt registers
///////////////////////////////////////////////////////////////////////////////
// Software Priority Registers
#define REGISTER_ITC_SPR1 REGISTER 0x7F70
#define REGISTER_ITC_SPR2 REGISTER 0x7F71
#define REGISTER_ITC_SPR3 REGISTER 0x7F72
#define REGISTER_ITC_SPR4 REGISTER 0x7F73
#define REGISTER_ITC_SPR5 REGISTER 0x7F74
#define REGISTER_ITC_SPR6 REGISTER 0x7F75
#define REGISTER_ITC_SPR7 REGISTER 0x7F76
#define REGISTER_ITC_SPR8 REGISTER 0x7F77
// External interrupt control registers
#define REGISTER_EXTI_CR1 REGISTER 0x50A0
#define REGISTER_EXTI_CR2 REGISTER 0x50A1

///////////////////////////////////////////////////////////////////////////////
// The SPR register each interrupt is configured in
///////////////////////////////////////////////////////////////////////////////
#define _ITC_0_SPR  REGISTER_ITC_SPR1
#define _ITC_1_SPR  REGISTER_ITC_SPR1
#define _ITC_2_SPR  REGISTER_ITC_SPR1
#define _ITC_3_SPR  REGISTER_ITC_SPR1
#define _ITC_4_SPR  REGISTER_ITC_SPR2
#define _ITC_5_SPR  REGISTER_ITC_SPR2
#define _ITC_6_SPR  REGISTER_ITC_SPR2
#define _ITC_7_SPR  REGISTER_ITC_SPR2
#define _ITC_8_SPR  REGISTER_ITC_SPR3
#define _ITC_9_SPR  REGISTER_ITC_SPR3
#define _ITC_10_SPR REGISTER_ITC_SPR3
#define _ITC_11_SPR REGISTER_ITC_SPR3
#define _ITC_12_SPR REGISTER_ITC_SPR4
#define _ITC_13_SPR REGISTER_ITC_SPR4
#define _ITC_14_SPR REGISTER_ITC_SPR4
#define _ITC_15_SPR REGISTER_ITC_SPR4
#define _ITC_16_SPR REGISTER_ITC_SPR5
#define _ITC_17_SPR REGISTER_ITC_SPR5
#define _ITC_18_SPR REGISTER_ITC_SPR5
#define _ITC_19_SPR REGISTER_ITC_SPR5
#define _ITC_20_SPR REGISTER_ITC_SPR6
#define _ITC_21_SPR REGISTER_ITC_SPR6
#define _ITC_22_SPR REGISTER_ITC_SPR6
#define _ITC_23_SPR REGISTER_ITC_SPR6
#define _ITC_24_SPR REGISTER_ITC_SPR7
#define _ITC_25_SPR REGISTER_ITC_SPR7
#define _ITC_26_SPR REGISTER_ITC_SPR7
#define _ITC_27_SPR REGISTER_ITC_SPR7
#define _ITC_28_SPR REGISTER_ITC_SPR8
#define _ITC_29_SPR REGISTER_ITC_SPR8

///////////////////////////////////////////////////////////////////////////////
// The shift in SPR register of each interrupt
///////////////////////////////////////////////////////////////////////////////
#define _ITC_0_SPR_SHIFT  0
#define _ITC_1_SPR_SHIFT  2
#define _ITC_2_SPR_SHIFT  4
#define _ITC_3_SPR_SHIFT  6
#define _ITC_4_SPR_SHIFT  0
#define _ITC_5_SPR_SHIFT  2
#define _ITC_6_SPR_SHIFT  4
#define _ITC_7_SPR_SHIFT  6
#define _ITC_8_SPR_SHIFT  0
#define _ITC_9_SPR_SHIFT  2
#define _ITC_10_SPR_SHIFT 4
#define _ITC_11_SPR_SHIFT 6
#define _ITC_12_SPR_SHIFT 0
#define _ITC_13_SPR_SHIFT 2
#define _ITC_14_SPR_SHIFT 4
#define _ITC_15_SPR_SHIFT 6
#define _ITC_16_SPR_SHIFT 0
#define _ITC_17_SPR_SHIFT 2
#define _ITC_18_SPR_SHIFT 4
#define _ITC_19_SPR_SHIFT 6
#define _ITC_20_SPR_SHIFT 0
#define _ITC_21_SPR_SHIFT 2
#define _ITC_22_SPR_SHIFT 4
#define _ITC_23_SPR_SHIFT 6
#define _ITC_24_SPR_SHIFT 0
#define _ITC_25_SPR_SHIFT 2
#define _ITC_26_SPR_SHIFT 4
#define _ITC_27_SPR_SHIFT 6
#define _ITC_28_SPR_SHIFT 0
#define _ITC_29_SPR_SHIFT 2

///////////////////////////////////////////////////////////////////////////////
// The allowed software priorities
///////////////////////////////////////////////////////////////////////////////
#define _ITC_PRIORITY_1 0b01
#define _ITC_PRIORITY_2 0b00
#define _ITC_PRIORITY_3 0b11

///////////////////////////////////////////////////////////////////////////////
// The available external interrupt sensitivities
///////////////////////////////////////////////////////////////////////////////
#define ITC_EXT_FALL_LOW  0b00 // Falling edge and low level
#define ITC_EXT_RISE      0b01 // Rising edge only
#define ITC_EXT_FALL      0b10 // Falling edge only
#define ITC_EXT_RISE_FALL 0b11 // Rising and falling edge

///////////////////////////////////////////////////////////////////////////////
// The EXTI_CR register each port sensitivity is programmed in
///////////////////////////////////////////////////////////////////////////////
#define _ITC_PORT_A_CR REGISTER_EXTI_CR1
#define _ITC_PORT_B_CR REGISTER_EXTI_CR1
#define _ITC_PORT_C_CR REGISTER_EXTI_CR1
#define _ITC_PORT_D_CR REGISTER_EXTI_CR1
#define _ITC_PORT_E_CR REGISTER_EXTI_CR2

///////////////////////////////////////////////////////////////////////////////
// The shift in EXTI_CR registers for each port
///////////////////////////////////////////////////////////////////////////////
#define _ITC_PORT_A_CR_SHIFT 0
#define _ITC_PORT_B_CR_SHIFT 2
#define _ITC_PORT_C_CR_SHIFT 4
#define _ITC_PORT_D_CR_SHIFT 6
#define _ITC_PORT_E_CR_SHIFT 0

///////////////////////////////////////////////////////////////////////////////
// STM8S interrupt related assembly shortcuts
///////////////////////////////////////////////////////////////////////////////
#define rim() {__asm__("rim\n");} // Enable interrupts
#define enableInterrupts() rim() // Alias for enable interrupts
#define sim() {__asm__("sim\n");} // Disable interrupts
#define disableInterrupts() sim() // Alias for disable interrupts
#define wfi() {__asm__("wfi\n");} // Wait for interrupt
#define waitForInterrupt() wfi() // Alias for wait for interrupt


///////////////////////////////////////////////////////////////////////////////
// Macros for setting up the interrupts by the user
///////////////////////////////////////////////////////////////////////////////

// Sets the software priority for the given interrupt source
// Parameters:
// - irq: The IRQ of the interrupt
// - level: The priority level, one of 1, 2 or 3
//
// Note to developers: First we set both bits to zero and then we set the bits
// to whatever the user gave.
#define _itcSetPriority(irq, level) do {\
  _ITC_##irq##_SPR &= ~(uint8_t)(0b11 << _ITC_##irq##_SPR_SHIFT);\
  _ITC_##irq##_SPR |= (uint8_t)(_ITC_PRIORITY_##level << _ITC_##irq##_SPR_SHIFT);\
} while(0)
#define itcSetPriority(irq, level) _itcSetPriority(irq, level)

// Sets the external port interrupt sensitivity
// Parameters:
// - port: The port name as an uppercase letter 
// - sensitivity: One of ITC_EXT_FALL_LOW, ITC_EXT_RISE, ITC_EXT_FALL, ITC_EXT_RISE_FALL
//
// Note to developers: First we set both bits to zero and then we set the bits
// to whatever the user gave.
#define _itcSetPortSensitivity(port, sensitivity) do {\
  _ITC_PORT_##port##_CR &= ~(uint8_t)(0b11 << _ITC_PORT_##port##_CR_SHIFT);\
  _ITC_PORT_##port##_CR |= (uint8_t)(sensitivity << _ITC_PORT_##port##_CR_SHIFT);\
} while(0)
#define itcSetPortSensitivity(port, sensitivity) _itcSetPortSensitivity(port, sensitivity)


#endif /* STM8_ITC_H */