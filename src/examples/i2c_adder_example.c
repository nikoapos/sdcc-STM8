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
 * This example demonstrates how to expose local memory locations via I2C,
 * using the i2cMemorySlaveIterruptHandler() macro. The example sets the I2C
 * address to 0x55 and exposes three variables via I2C, two single byte variables
 * with I2C IDs 0x01 and 0x02 and a 2 byte variable with ID 0x02. The main
 * program is constantly adding the two first variables and updates the third
 * one with the result.
 * 
 * A Raspberry Pi can be used for testing. The initial values of the variables
 * can be checked by running:
 * 
 *   i2cget -y 1 0x55 0x01     # Should return 0x00
 *   i2cget -y 1 0x55 0x02     # Should return 0x00
 *   i2cget -y 1 0x55 0x03 w   # Should return 0x0000
 * 
 * All the variables are initialized to zero by default. To set the first and
 * second variables you can run the commands:
 * 
 *   i2cset -y 1 0x55 0x01 0x82
 *   i2cset -y 1 0x55 0x02 0xA3
 * 
 * Now you can check again the variables:
 * 
 *   i2cget -y 1 0x55 0x01     # Should return 0x82
 *   i2cget -y 1 0x55 0x02     # Should return 0xA3
 *   i2cget -y 1 0x55 0x03 w   # Should return 0x2501
 * 
 * Note above that the i2cget shows the LSB first, so the result is 0x0125.
 */

#include <i2c.h>
#include <clk.h>
#include <itc.h>

// The three variables
uint8_t var1 = 0;
uint8_t var2 = 0;
uint16_t var3 = 0;

int main() {
  
  // First we set the f_master to 16 MHz
  CLK_SET_HSI_DIVIDER(1);
  
  // We initialize the I2C. The first parameter is the 7-bit address we want to
  // use for the slave. The second is the value of the f_master (in MHz).
  i2cInitialize(0x55, 16);
  
  // We must enable the interrupts
  enableInterrupts();
  
  // Start an infinite loop
  while(1) {
    // Inside the loop we constantly add the variables
    var3 = var1 + var2;
  }
  
}

// The i2cMemorySlaveIterruptHandler() macro requires a method which gets as
// input the ID read from the I2C bus and returns a pointer to the variable
// which corresponds to this I2C ID. The size parameter must be set to the size
// of the variable in bytes. The mapping we do is the following:
// - var1: 0x01
// - var2: 0x02
// - var3: 0x03
// If the ID is unknown we have to return 0.
uint8_t* getPointer(uint8_t id, uint8_t* size) {
  switch (id) {
    case 0x01:
      *size = 1;
      return &var1;
    case 0x02:
      *size = 1;
      return &var2;
    case 0x03:
      *size = 2;
      return &var3;
  }
  return 0;
}

// Finally, we call the i2cMemorySlaveIterruptHandler macro. This will generate
// the function which handles the I2C interrupts.
//
// Warning: This means that you cannot define another I2C interrupt handler. If
// you want to implement some extra functionality, you will have to implement
// the handler from scratch!
i2cMemorySlaveIterruptHandler(getPointer)