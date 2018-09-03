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
 * The controller uses the counters to compute the number of encoder disc cuts
 * per second. The frequency this computation is performed can be controlled
 * for each wheel individually via the registers 0xA1-0xA4. These registers can
 * be written with the period of the measurement in ms for each counter (the
 * default value is 100ms). This value can be tuned based on the speed of the
 * wheels. The longer the period the more accurate the measurement. The shorter
 * the period, more frequent updates. For example, if the wheel turns very slow
 * (once per second) this period should be significantly increased. The measured
 * frequency can be accessed via the I2C registers 0x11-0x14.
 * 
 * I2C registers:
 * 
 * - 0x01 (uint16_t) : Counter 1
 * - 0x02 (uint16_t) : Counter 2
 * - 0x03 (uint16_t) : Counter 3
 * - 0x04 (uint16_t) : Counter 4
 * - 0x11 (float 4 byte) : Counter 1 speed (in counts/sec)
 * - 0x12 (float 4 byte) : Counter 2 speed (in counts/sec)
 * - 0x13 (float 4 byte) : Counter 3 speed (in counts/sec)
 * - 0x14 (float 4 byte) : Counter 4 speed (in counts/sec)
 * - 0xA1 (uint16_t) : Speed measure period 1
 * - 0xA2 (uint16_t) : Speed measure period 2
 * - 0xA3 (uint16_t) : Speed measure period 3
 * - 0xA4 (uint16_t) : Speed measure period 4
 */

#include <stdbool.h>
#include <clk.h>
#include <i2c.h>
#include <gpio.h>
#include <itc.h>
#include <tim4.h>

// The slave I2C address the micro controller will listen to
#define I2C_ADDRESS 0x55

// The pins where each photo-interrupter is connected
#define PORT_IN C
#define PIN_IN_1 3
#define PIN_IN_2 4
#define PIN_IN_3 5
#define PIN_IN_4 6

typedef struct {
  uint16_t count; // The counter of the wheel
  bool state; // The current state of the photo-interrupter
  uint16_t period; // The period in ms to perform a speed measurement
  uint16_t last_meas_time; // The ms passed from the last measurement time
  float counts_speed; // The counter speed in counts/sec
  uint16_t last_count; // The value of the counter during the last measurement
} Wheel;

Wheel wheel_1;
Wheel wheel_2;
Wheel wheel_3;
Wheel wheel_4;

void updateWheelCounter(Wheel* wheel, bool new_state) {
  if (new_state != wheel->state) {
    wheel->count += 1;
    wheel->state = new_state;
  }
}

// The main method
int main() {
  
  // Set the f_master to 16 MHz
  clkSetHsiDivider(1);
  
  // Set the TIM4 timer to create an interrupt every 1ms
  // We run at 16 Mz and we set the prescaler to 128, so the frequency of the
  // TIM4 is 125 kHz
  TIM4_SET_PRESCALER(TIM4_PRESCALER_128);
  // We want 1 ms period, so the timer should overflow when it reaches 124
  TIM4_SET_PERIOD(124);
  
  // Set the period of all the wheels to 100ms (the default)
  wheel_1.period = 100;
  wheel_2.period = 100;
  wheel_3.period = 100;
  wheel_4.period = 100;
  
  // Enable the TIM4 interrupts and start it
  TIM4_ENABLE_INTERRUPT();
  TIM4_START();
  
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
  ITC_SET_PRIORITY(ITC_IRQ_TIM4_UPD_OVF, 2);
  enableInterrupts();
  
  // Start an infinite loop which updates the counters constantly
  while(1) {
    // Update the counters
    updateWheelCounter(&wheel_1, GPIO_READ_INPUT(PORT_IN, PIN_IN_1));
    updateWheelCounter(&wheel_2, GPIO_READ_INPUT(PORT_IN, PIN_IN_2));
    updateWheelCounter(&wheel_3, GPIO_READ_INPUT(PORT_IN, PIN_IN_3));
    updateWheelCounter(&wheel_4, GPIO_READ_INPUT(PORT_IN, PIN_IN_4));
  }
  
}

// This method controls the I2C register address each internal variable is
// exposed to
uint8_t* getMemoryPointer(uint8_t id, uint8_t* size) {
  // The first 4 bits of the address indicate the variable
  uint8_t var_id = id & 0xF0;
  // The last 4 bits of the address indicate the wheel
  uint8_t wheel_id = id & 0x0F;
  Wheel* wheel;
  
  switch (wheel_id) {
    case 0x01:
      wheel = &wheel_1;
      break;
    case 0x02:
      wheel = &wheel_2;
      break;
    case 0x03:
      wheel = &wheel_3;
      break;
    case 0x04:
      wheel = &wheel_4;
      break;
    default:
      return 0;
  }
  
  switch (var_id) {
    case 0x00:
      *size = 2;
      return &(wheel->count);
    case 0x10:
      *size = 4;
      return &(wheel->counts_speed);
    case 0xA0:
      *size = 2;
      return &(wheel->period);
  }
  
  return 0;
}

// Setup the I2C interruption to expose the variables in the memory
i2cMemorySlaveIterruptHandler(getMemoryPointer)


void measureSpeed(Wheel* wheel) {
  // Check if we have an overflow of the wheel counter
  if (wheel->last_count > wheel->count) {
    // We don't do a measurement a this time and we setup the variables to
    // restart the measurement period
    wheel->last_count = wheel->count;
    wheel->last_meas_time = 0;
    return;
  }
  
  // Increase the time from last measurement by 1ms and check if we need to
  // perform a measurement
  wheel->last_meas_time += 1;
  if (wheel->last_meas_time < wheel->period) {
    return;
  }
  
  // Compute the counts per second
  wheel->counts_speed = 1000. * (wheel->count - wheel->last_count) / wheel->period;
  
  // Restart the measurement period
  wheel->last_count = wheel->count;
  wheel->last_meas_time = 0;
  
}

// Called every time the TIM4 overflows, aka every 1ms
void measureSpeedEvent() __interrupt(ITC_IRQ_TIM4_UPD_OVF) {
  // First we clear the interrupt flag
  TIM4_CLEAR_UPDATE_INTERRUPT_FLAG();
  
  // Measure the speeds of all wheels
  measureSpeed(&wheel_1);
  measureSpeed(&wheel_2);
  measureSpeed(&wheel_3);
  measureSpeed(&wheel_4);
}