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
 * This example demonstrates how to handle external interrupts from the GPIOs.
 * A LED is connected to one pin (D4), which is set as output. A switch is
 * connected to another pin (C5), which is set as input with the interrupts
 * enabled. When an interrupt is detected the LED is turned from off to on and
 * vice versa.
 * 
 * Note: Because of switch bounce the interrupt might be triggered more than
 * once each time the switch changes state. This can be fixed with a capacitor,
 * but this is omitted here for simplicity.
 * 
 * Materials:
 * - A LED
 * - A 330 ohm resistor to be connected with the LED (to properly calculate
 *   the value see here: http://www.evilmadscientist.com/2012/resistors-for-leds)
 * - A switch
 * 
 * Connections:
 * - Connect the cathode of te LED (short leg) to the ground (GND)
 * - Connect the anode of the the LED (long leg) to the one side of the 330 ohm
 *   resistor
 * - Connect the other side of the 330 ohm resistor to the D4 pin
 * - Connect the one side of the switch to the C5 pin
 * - Connect the other side of the switch to the ground
 */

#include <itc.h>
#include <gpio.h>

// Define the port and pins where the LED and the switch are connected. This is
// optional. The values can be passed directly to the macro functions.
#define LED_PORT D
#define LED_PIN 4
#define SWITCH_PORT C
#define SWITCH_PIN 5

int main() {
  
  // We set the GPIO where the LED is connected as a push-pull output
  gpioSetAsOutput(LED_PORT, LED_PIN);
  gpioSetAsPushPull(LED_PORT, LED_PIN);
  
  // We set the GPIO where the switch is connected as a pull-up input
  gpioSetAsInput(SWITCH_PORT, SWITCH_PIN);
  gpioSetAsPullUp(SWITCH_PORT, SWITCH_PIN);
  
  // By default all external interrupts are masked. We have to unmask them for
  // the specific pin where the switch is connected
  gpioEnableInterrupt(SWITCH_PORT, SWITCH_PIN);
  
  // The STM8 interrupts can have different priorities. This is an example of
  // how to set the priority. The first parameter is the IRQ of the interrupt
  // (they are defined in the itc.h file) and the second is the priority level,
  // where 1 is the lowest and 3 the highest.
  itcSetPriority(ITC_IRQ_PORTC, 1);
  
  // Optionally we can set the interrupt sensitivity. This controls when the
  // interrupt is triggered. The allowed values are:
  // - ITC_EXT_FALL_LOW: Falling edge and low level
  // - ITC_EXT_RISE: Rising edge only
  // - ITC_EXT_FALL: Falling edge only
  // - ITC_EXT_RISE_FALL: Rising and falling edge
  itcSetPortSensitivity(SWITCH_PORT, ITC_EXT_RISE_FALL);

  // By default the interrupts are not enabled so we have to enable them
  enableInterrupts();

  // Start an infinite loop
  while (1) {
    
    // In the loop we just start waiting for interrupts. This will stop the CPU
    // and the STM8 will enter low power mode.
    waitForInterrupt();
  }
}


// This method will be called every time the PORTC interrupt is detected. The
// name of the method can be anything you like. A full list of the possible IRQs
// can be found in the itc.h file. Note that we do not need to do any special
// setup for the interrupts register, neither we need to call the IRET instruction.
// The sdcc takes care of all that.
void invertLed() __interrupt(ITC_IRQ_PORTC) {
  
  // When the interrupt is handled we just invert the state of the LED pin
  gpioInvert(LED_PORT, LED_PIN);
}