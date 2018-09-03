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
 * This example demonstrates how to use the I2C peripheral in slave mode to read
 * and write data. The communication is handled via interrupt events, as described
 * in the STM8S reference manual figures 102 and 103.
 * 
 * The behavior of the example is echoing the last written byte. Every time a
 * byte is read from the bus it is stored in a variable. When a write event
 * occurs, the value of the variable is written on the bus.
 * 
 * To test the example you can connect the microcontroller SDA (B5) and SCL (B4)
 * pins to the SDA and SCL pins of a Raspberry pi. Running "i2cdetect -y 1"
 * should detect the microcontroller at the address 0x55. You can use the i2cget
 * to echo bytes, for example "i2cget -y 1 0x55 0xAF". This command first writes
 * the 0xAF to the bus and then reads a single byte, so it should return 0xAF.
 * 
 * Note that for simplification the error checking is omitted.
 */

#include <i2c.h>
#include <clk.h>
#include <itc.h>

int main() {
  
  // First we set the f_master to 16 MHz
  clkSetHsiDivider(1);
  
  // We initialize the I2C. The first parameter is the 7-bit address we want to
  // use for the slave. The second is the value of the f_master (in MHz).
  i2cInitialize(0x55, 16);
  
  // We must enable the interrupts
  enableInterrupts();
  
  // Start an infinite loop
  while(1) {
    // In the loop we just start waiting for interrupts. This will stop the CPU
    // and the STM8 will enter low power mode.
    waitForInterrupt();
  }
  
}

// This is the variable where we keep the last byte read
uint8_t echo = 0xFF;

// This method will be called every time an I2C event occurs. There is a single
// interrupt method for all the I2C events, so the status registers must be used
// to identify which the event trigger. The name of the method can be anything
// you like. Note that all the related events are detected for completeness,
// even if no action is taken.
void i2cSlave() __interrupt(ITC_IRQ_I2C) {
  
  // Event EV1
  // The slave address has been matched. The I2C peripheral continues after we
  // read the SR3 buffer.
  if (REGISTER_I2C_SR1 & I2C_SR1_ADDR) {
    REGISTER_I2C_SR3;
    return;
  }
  
  // Event EV2
  // A byte has been received and waits in the DR register to be read. The I2C
  // peripheral continues after we read the register.
  if (REGISTER_I2C_SR1 & I2C_SR1_RXNE) {
    // We set the echo value to the one we just received 
    echo = REGISTER_I2C_DR;
    return;
  }
  
  // Event EV3
  // A byte needs to be transmitted. The I2C peripheral continues right after
  // we write the value in the DR register.
  if (REGISTER_I2C_SR1 & I2C_SR1_TXE) {
    // Write the echo value
    REGISTER_I2C_DR = echo;
    return;
  }
  
  // Event EV3-2
  // Acknowledgment failure event. The I2C protocol defines that right after
  // the last transmitted byte, the master does not sent an acknowledgment, so
  // this event occurs right after the last received byte. To instruct the I2C
  // peripheral to continue we need to clear the flag.
  if (REGISTER_I2C_SR2 & I2C_SR2_AF) {
    registerUnset(REGISTER_I2C_SR2, I2C_SR2_AF);
    return;
  }
  
  // Event EV4
  // A stop condition is detected on the bus after an acknowledge. Cleared by
  // writing to the CR2 register.
  if (REGISTER_I2C_SR1 & I2C_SR1_STOPF) {
    // Enable he acknowledgments in the CR2 register
    registerSet(REGISTER_I2C_CR2, I2C_CR2_ACK);
    return;
  }
  
}