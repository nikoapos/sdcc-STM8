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
 * The WheelSpeedReader can monitor the speed of up to four wheels using
 * photo-interrupters and encoder discs. The measured speed can be accessed via
 * I2C interface.
 * 
 * Note that the microcontroller should not be connected directly to the
 * photo-interrupters, but a dual comparator (like the LM393) should be used.
 * 
 * Internally the microcontroller keeps a counter for each wheel, which is
 * increased every time the photo-interrupter state changes (from high to low
 * AND for low to high), which means that every time a wheel does a full
 * turn, the related counter is increased twice as the number of the encoder
 * disc cuts. These counters can be accessed via the I2C registers 0x01-0x04.
 * 
 * I2C registers:
 * 
 * - 0x01 (uint16_t) : Counter 1
 * - 0x02 (uint16_t) : Counter 2
 * - 0x03 (uint16_t) : Counter 3
 * - 0x04 (uint16_t) : Counter 4
 */

#include <stdbool.h>
#include <clk.h>
#include <i2c.h>
#include <gpio.h>
#include <itc.h>

// The slave I2C address the micro controller will listen to
#define I2C_ADDRESS 0x55

// The pins where each photo-interrupter is connected
#define PORT_IN C
#define PIN_IN_1 3
#define PIN_IN_2 4
#define PIN_IN_3 5
#define PIN_IN_4 6

// The counters for the four inputs
uint16_t count_1 = 0;
uint16_t count_2 = 0;
uint16_t count_3 = 0;
uint16_t count_4 = 0;

// The current states of the inputs
bool state_1 = false;
bool state_2 = false;
bool state_3 = false;
bool state_4 = false;

// Updates the counters if the state of the input has changed
void updateCounters() {
  // Read the new state of the inputs
  bool new_state_1 = GPIO_READ_INPUT(PORT_IN, PIN_IN_1);
  bool new_state_2 = GPIO_READ_INPUT(PORT_IN, PIN_IN_2);
  bool new_state_3 = GPIO_READ_INPUT(PORT_IN, PIN_IN_3);
  bool new_state_4 = GPIO_READ_INPUT(PORT_IN, PIN_IN_4);

  // Update the counters
  if (new_state_1 != state_1) {
    ++count_1;
    state_1 = new_state_1;
  }
  if (new_state_2 != state_2) {
    ++count_2;
    state_2 = new_state_2;
  }
  if (new_state_3 != state_3) {
    ++count_3;
    state_3 = new_state_3;
  }
  if (new_state_4 != state_4) {
    ++count_4;
    state_4 = new_state_4;
  }
}

// The main method
int main() {
  
  // Set the f_master to 16 MHz
  CLK_SET_HSI_DIVIDER(1);
  
  // Reset all pins to use the minimum power
  GPIO_SET_ALL_PORTS_INPUT_PULL_UP_NO_INT();
  
  // Set the input pins as inputs
  GPIO_SET_AS_INPUT(PORT_IN, PIN_IN_1);
  GPIO_SET_AS_INPUT(PORT_IN, PIN_IN_2);
  GPIO_SET_AS_INPUT(PORT_IN, PIN_IN_3);
  GPIO_SET_AS_INPUT(PORT_IN, PIN_IN_4);
  
  // Initialize the I2C peripheral
  i2cInitialize(I2C_ADDRESS, 16);
  
  // Enable the interrupts
  ITC_SET_PRIORITY(ITC_IRQ_I2C, 3); // I2C must have the highest priority
  enableInterrupts();
  
  // Start an infinite loop which updates the counters constantly
  while(1) {
    updateCounters();
  }
  
}

// This method controls the I2C register address each internal variable is
// exposed to
uint8_t* getMemoryPointer(uint8_t id, uint8_t* size) {
  switch (id) {
    case 0x01:
      *size = 2;
      return &count_1;
    case 0x02:
      *size = 2;
      return &count_2;
    case 0x03:
      *size = 2;
      return &count_3;
    case 0x04:
      *size = 2;
      return &count_4;
  }
  return 0;
}

// Setup the I2C interruption to expose the variables in the memory
i2cMemorySlaveIterruptHandler(getMemoryPointer)