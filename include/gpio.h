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
 * File:   gpio.h
 * Author: Nikolaos Apostolakos <nikoapos@gmail.com>
 *
 * Created on August 17, 2018, 5:00 PM
 */

#ifndef STM8S_GPIO_H
#define STM8S_GPIO_H

#include <stm8.h>
#include <utils.h>

///////////////////////////////////////////////////////////////////////////////
// GPIO pins
///////////////////////////////////////////////////////////////////////////////
#define GPIO_PIN_0 (uint8_t)0b00000001
#define GPIO_PIN_1 (uint8_t)0b00000010
#define GPIO_PIN_2 (uint8_t)0b00000100
#define GPIO_PIN_3 (uint8_t)0b00001000
#define GPIO_PIN_4 (uint8_t)0b00010000
#define GPIO_PIN_5 (uint8_t)0b00100000
#define GPIO_PIN_6 (uint8_t)0b01000000
#define GPIO_PIN_7 (uint8_t)0b10000000

///////////////////////////////////////////////////////////////////////////////
// GPIO registers
///////////////////////////////////////////////////////////////////////////////
#define REGISTER_PA_ODR REGISTER 0x5000
#define REGISTER_PA_IDR REGISTER_RO 0x5001
#define REGISTER_PA_DDR REGISTER 0x5002
#define REGISTER_PA_CR1 REGISTER 0x5003
#define REGISTER_PA_CR2 REGISTER 0x5004
#define REGISTER_PB_ODR REGISTER 0x5005
#define REGISTER_PB_IDR REGISTER_RO 0x5006
#define REGISTER_PB_DDR REGISTER 0x5007
#define REGISTER_PB_CR1 REGISTER 0x5008
#define REGISTER_PB_CR2 REGISTER 0x5009
#define REGISTER_PC_ODR REGISTER 0x500A
#define REGISTER_PC_IDR REGISTER_RO 0x500B
#define REGISTER_PC_DDR REGISTER 0x500C
#define REGISTER_PC_CR1 REGISTER 0x500D
#define REGISTER_PC_CR2 REGISTER 0x500E
#define REGISTER_PD_ODR REGISTER 0x500F
#define REGISTER_PD_IDR REGISTER_RO 0x5010
#define REGISTER_PD_DDR REGISTER 0x5011
#define REGISTER_PD_CR1 REGISTER 0x5012
#define REGISTER_PD_CR2 REGISTER 0x5013
#define REGISTER_PE_ODR REGISTER 0x5014
#define REGISTER_PE_IDR REGISTER_RO 0x5015
#define REGISTER_PE_DDR REGISTER 0x5016
#define REGISTER_PE_CR1 REGISTER 0x5017
#define REGISTER_PE_CR2 REGISTER 0x5018
#define REGISTER_PF_ODR REGISTER 0x5019
#define REGISTER_PF_IDR REGISTER_RO 0x501A
#define REGISTER_PF_DDR REGISTER 0x501B
#define REGISTER_PF_CR1 REGISTER 0x501C
#define REGISTER_PF_CR2 REGISTER 0x501D

///////////////////////////////////////////////////////////////////////////////
// Default register values
///////////////////////////////////////////////////////////////////////////////
#define GPIO_ODR_DEFAULT (uint8_t) 0x00
#define GPIO_DDR_DEFAULT (uint8_t) 0x00
#define GPIO_CR1_DEFAULT (uint8_t) 0x00
#define GPIO_CR2_DEFAULT (uint8_t) 0x00


///////////////////////////////////////////////////////////////////////////////
// Macros for handling the GPIOs by the user
///////////////////////////////////////////////////////////////////////////////

// Resets a given GPIO port to its default configuration
// Parameters:
//    port = The port name as an uppercase letter
# define GPIO_RESET_PORT(port) REGISTER_P##port##_ODR = GPIO_ODR_DEFAULT;\
                               REGISTER_P##port##_DDR = GPIO_DDR_DEFAULT;\
                               REGISTER_P##port##_CR1 = GPIO_CR1_DEFAULT;\
                               REGISTER_P##port##_CR2 = GPIO_CR2_DEFAULT

// Sets a given GPIO pin as an input by setting the corresponding DDR bit to 0.
// Parameters:
//    port - The port name as an uppercase letter
//    pin - The pin of the port to set as input, a number in range [0,7]
#define GPIO_SET_AS_INPUT(port, pin) REGISTER_UNSET(REGISTER_P##port##_DDR, GPIO_PIN_##pin)

// Sets a given GPIO pin as an output by setting the corresponding DDR bit to 1.
// Parameters:
//    port - The port name as an uppercase letter
//    pin - The pin of the port to set as output, a number in range [0,7]
#define GPIO_SET_AS_OUTPUT(port, pin) REGISTER_SET(REGISTER_P##port##_DDR, GPIO_PIN_##pin)

// Sets an input GPIO pin as floating by setting the corresponding CR1 bit to 0.
// Parameters:
//    port - The port name as an uppercase letter
//    pin - The pin of the port to set as floating, a number in range [0,7]
#define GPIO_SET_AS_FLOATING(port, pin) REGISTER_UNSET(REGISTER_P##port##_CR1, GPIO_PIN_##pin)

// Sets an input GPIO pin as pull-up by setting the corresponding CR1 bit to 1.
// Parameters:
//    port - The port name as an uppercase letter
//    pin - The pin of the port to set as pull-up, a number in range [0,7]
#define GPIO_SET_AS_PULL_UP(port, pin) REGISTER_SET(REGISTER_P##port##_CR1, GPIO_PIN_##pin)

// Sets an output GPIO pin as open drain by setting the corresponding CR1 bit to 0.
// Parameters:
//    port - The port name as an uppercase letter
//    pin - The pin of the port to set as open drain, a number in range [0,7]
#define GPIO_SET_AS_OPEN_DRAIN(port, pin) REGISTER_UNSET(REGISTER_P##port##_CR1, GPIO_PIN_##pin)

// Sets an output GPIO pin as push-pull by setting the corresponding CR1 bit to 1.
// Parameters:
//    port - The port name as an uppercase letter
//    pin - The pin of the port to set as push-pull, a number in range [0,7]
#define GPIO_SET_AS_PUSH_PULL(port, pin) REGISTER_SET(REGISTER_P##port##_CR1, GPIO_PIN_##pin)

// Set an output GPIO pin as high.
// Parameters:
//    port - The port name as an uppercase letter
//    pin - The pin of the port to set as high, a number in range [0,7]
#define GPIO_WRITE_HIGH(port, pin) REGISTER_SET(REGISTER_P##port##_ODR, GPIO_PIN_##pin)

// Set an output GPIO pin as low.
// Parameters:
//    port - The port name as an uppercase letter
//    pin - The pin of the port to set as low, a number in range [0,7]
#define GPIO_WRITE_LOW(port, pin) REGISTER_UNSET(REGISTER_P##port##_ODR, GPIO_PIN_##pin)

// Read the GPIO input pin from the IDR register
// Parameters:
//    port - The port name as an uppercase letter
//    pin - The pin of the port to read the input, a number in range [0,7]
// Returns:
//    true if the pin is set, false otherwise
#define GPIO_READ_INPUT(port,pin) REGISTER_P##port##_IDR & GPIO_PIN_##pin

// Read the GPIO output pin from the ODR register
// Parameters:
//    port - The port name as an uppercase letter
//    pin - The pin of the port to read the output, a number in range [0,7]
// Returns:
//    true if the pin is set, false otherwise
#define GPIO_READ_OUTPUT(port,pin) REGISTER_P##port##_ODR & GPIO_PIN_##pin

#endif /* STM8S_GPIO_H */

