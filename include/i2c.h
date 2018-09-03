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
 * File:   i2c.h
 * Author: Nikolaos Apostolakos <nikoapos@gmail.com>
 *
 * Created on August 20, 2018, 8:51 AM
 */

#ifndef STM8_I2C_H
#define STM8_I2C_H

#include <stdbool.h>
#include <stm8.h>
#include <utils.h>

///////////////////////////////////////////////////////////////////////////////
// I2C related registers
///////////////////////////////////////////////////////////////////////////////
#define REGISTER_I2C_CR1 REGISTER    0x5210 // I2C control register 1
#define REGISTER_I2C_CR2 REGISTER    0x5211 // I2C control register 2
#define REGISTER_I2C_FREQR REGISTER  0x5212 // I2C frequency register
#define REGISTER_I2C_OARL REGISTER   0x5213 // I2C own address register low
#define REGISTER_I2C_OARH REGISTER   0x5214 // I2C own address register high
#define REGISTER_I2C_DR REGISTER     0x5216 // I2C data register
#define REGISTER_I2C_SR1 REGISTER    0x5217 // I2C status register 1
#define REGISTER_I2C_SR2 REGISTER    0x5218 // I2C status register 2
#define REGISTER_I2C_SR3 REGISTER    0x5219 // I2C status register 3 
#define REGISTER_I2C_ITR REGISTER    0x521A // I2C interrup control register
#define REGISTER_I2C_CCRL REGISTER   0x521B // I2C clock control register low
#define REGISTER_I2C_CCRH REGISTER   0x521C // I2C clock control register high
#define REGISTER_I2C_TRISER REGISTER 0x521D // I2C TRISE register
#define REGISTER_I2C_PECR REGISTER   0x521E // I2C packet error checking register

///////////////////////////////////////////////////////////////////////////////
// I2C register flags
///////////////////////////////////////////////////////////////////////////////
#define I2C_CR1_NOO_STRETCH (uint8_t) 0b10000000
#define I2C_CR1_ENGC        (uint8_t) 0b01000000
#define I2C_CR1_PE          (uint8_t) 0b00000001
#define I2C_CR2_ACK         (uint8_t) 0b00000100
#define I2C_CR2_STOP        (uint8_t) 0b00000010
#define I2C_CR2_START       (uint8_t) 0b00000001
#define I2C_SR1_TXE         (uint8_t) 0b10000000
#define I2C_SR1_RXNE        (uint8_t) 0b01000000
#define I2C_SR1_STOPF       (uint8_t) 0b00010000
#define I2C_SR1_ADD10       (uint8_t) 0b00001000
#define I2C_SR1_BTF         (uint8_t) 0b00000100
#define I2C_SR1_ADDR        (uint8_t) 0b00000010
#define I2C_SR1_SB          (uint8_t) 0b00000001
#define I2C_SR2_WUFH        (uint8_t) 0b00100000
#define I2C_SR2_OVR         (uint8_t) 0b00001000
#define I2C_SR2_AF          (uint8_t) 0b00000100
#define I2C_SR2_ARLO        (uint8_t) 0b00000010
#define I2C_SR2_BERR        (uint8_t) 0b00000001
#define I2C_SR3_GENCALL     (uint8_t) 0b00010000
#define I2C_SR3_TRA         (uint8_t) 0b00000100
#define I2C_SR3_BUSY        (uint8_t) 0b00000010
#define I2C_SR3_MSL         (uint8_t) 0b00000001
#define I2C_OARH_ADDMODE    (uint8_t) 0b10000000
#define I2C_OARH_ADDCONF    (uint8_t) 0b01000000
#define I2C_CCRH_FS         (uint8_t) 0b10000000
#define I2C_CCRH_DUTY       (uint8_t) 0b01000000
#define I2C_ITR_ITBUFEN     (uint8_t) 0b00000100
#define I2C_ITR_ITEVTEN     (uint8_t) 0b00000010
#define I2C_ITR_ITERREN     (uint8_t) 0b00000001


///////////////////////////////////////////////////////////////////////////////
// Macros and methods for handling I2C, to be used by the user
///////////////////////////////////////////////////////////////////////////////

// Initializes the I2C peripheral. 
// Current limitations:
// - Only normal mode (100 kbit/s) is supported
// - Only 7-bit addresses are supported
// Parameters:
// - address: The own address (7bit)
// - frequency: The frequency f_master which is fed to the peripheral (in MHz)
void i2cInitialize(uint8_t address, uint8_t frequency) {
  uint16_t ccr;
  
  // Disable the I2C peripheral
  registerUnset(REGISTER_I2C_CR1, I2C_CR1_PE);
  
  // Set the address
  registerUnset(REGISTER_I2C_OARH, I2C_OARH_ADDMODE); // Use 7-bit address
  registerSet(REGISTER_I2C_OARH, I2C_OARH_ADDCONF); // Must always be 1
  REGISTER_I2C_OARL = (uint8_t)(address << 1); // Write own address
  
  // Set the input frequency
  REGISTER_I2C_FREQR = frequency;
  
  // Compute the CCR based on the equation Period(I2C) = 2 * CCR * t_MASTER
  // CCR = frequency * 1000000 / (2 * 100000) = frequency * 5
  registerUnset(REGISTER_I2C_CCRH, I2C_CCRH_FS); // Set normal mode
  ccr = (uint16_t)frequency * 5;
  REGISTER_I2C_CCRL = (uint8_t)ccr;
  REGISTER_I2C_CCRH = (uint8_t)(ccr >> 8);
  
  // Set the maximum rise time
  REGISTER_I2C_TRISER = frequency + 1;
  
  // Enable the interrupts
  registerSet(REGISTER_I2C_ITR, I2C_ITR_ITBUFEN);
  registerSet(REGISTER_I2C_ITR, I2C_ITR_ITEVTEN);
  registerSet(REGISTER_I2C_ITR, I2C_ITR_ITERREN);
  
  // Enable the I2C peripheral
  registerSet(REGISTER_I2C_CR1, I2C_CR1_PE);
  
  // Send ACK after bytes (default I2C expectation)
  registerSet(REGISTER_I2C_CR2, I2C_CR2_ACK);
}


//
// This macro implements the I2C interrupt handler in such a way so that it can
// read and write locations at the memory in slave mode. It can handle multiple
// memory locations, which are identified by an unsigned 8bit integer key.
//
// The I2C communication is as follows:
// - The master sends a byte containing the ID of the memory location
// - If it keeps sending bytes, they are written in the memory in sequential
//   locations
// - If it starts reading bytes, the values are returned sequentially
//
// This behavior simulates the behavior of I2C controllers which provide access
// to the registers of different peripherals.
//
// The implementation controls the locations of the memory to be exposed by
// passing as argument the name of a function with the following signature:
// - First parameter (uint8_t id):
//      The ID of the memory location, as read from the I2C bus
// - Second parameter (uint8_t* size):
//      The implementation must write the number of bytes in the memory which
//      is to be accessed
// - Return type (uint8_t*):
//      A pointer to the first byte in memory for the given ID
//
// Note that if the master keeps sending write bytes after the declared size
// the are ignored. Similarly, if it keeps reading bytes, zeroes are returned.
//
// For an example of how to use this macro see the src/i2c_adder_example.c
//
void _i2cMemorySlave( uint8_t* (*handleId)(uint8_t, uint8_t*), uint8_t** ptr,
                     uint8_t* size, bool* read_id) {
  
  // Event EV1
  if (REGISTER_I2C_SR1 & I2C_SR1_ADDR) {
    REGISTER_I2C_SR3; // Read the SR3 to unblock the I2C
    *read_id = true; // We just got the address so we set that we want to read the ID
    return;
  }
  
  // Event EV2
  if (REGISTER_I2C_SR1 & I2C_SR1_RXNE) {
    if (*read_id) {
      // If we are reading the ID use the user method to get the pointer and the size
      *ptr = (*handleId)(REGISTER_I2C_DR, size);
      // If we have an unknown ID we do not want to read or write any bytes,
      // so we set the size to zero
      if (*ptr == 0) {
        *size = 0;
      }
      *read_id = false; // All rest bytes should be written in memory
    } else {
      // Write in memory the byte from I2C, if the size is not 0, otherwise ignore it
      if (*size > 0) {
        **ptr = REGISTER_I2C_DR;
        ++(*ptr);
        --(*size);
      }
    }
    return;
  }
  
  // Even EV3
  if (REGISTER_I2C_SR1 & I2C_SR1_TXE) {
    // Write in I2C the byte from memory if size is not 0, otherwise write 0
    if (*size > 0) {
      REGISTER_I2C_DR = **ptr;
        ++(*ptr);
        --(*size);
    } else {
      REGISTER_I2C_DR = 0;
    }
    return;
  }
  
  // Event EV3-2
  if (REGISTER_I2C_SR2 & I2C_SR2_AF) {
    registerUnset(REGISTER_I2C_SR2, I2C_SR2_AF);
    return;
  }
  
  // Event EV4
  if (REGISTER_I2C_SR1 & I2C_SR1_STOPF) {
    registerSet(REGISTER_I2C_CR2, I2C_CR2_ACK);
    return;
  }
  
}

#define i2cMemorySlaveIterruptHandler(handleId) \
uint8_t* _i2c_memory_slave_ptr = 0;\
uint8_t _i2c_memory_slave_size = 0;\
bool _i2c_memory_slave_read_id = false;\
void _i2cMemorySlaveInterruptHandler() __interrupt(ITC_IRQ_I2C) {\
  _i2cMemorySlave(handleId, &_i2c_memory_slave_ptr,\
                  &_i2c_memory_slave_size, &_i2c_memory_slave_read_id);\
}

#endif /* STM8_I2C_H */

