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
 * This example demonstrates very simple use of the GPIOs as input and outputs.
 * A LED is connected to one pin (D4), which is set as output. A switch is
 * connected to another pin (C5), which is set as input. The LED turns on and
 * off together with the switch. Note that this is a very simple example, which
 * repeatedly synchronizes the state of the two pins, which is not optimal for
 * power consumption.
 * 
 * Materials:
 * - A LED
 * - A 330 ohm resistor to be connected with the LED (to properly calculate
 *   the value see here: http://www.evilmadscientist.com/2012/resistors-for-leds)
 * - A switch
 * - A 5k ohm resistor for pull-down the switch
 * 
 * Connections:
 * - Connect the cathode of te LED (short leg) to the ground (GND)
 * - Connect the anode of the the LED (long leg) to the one side of the 330 ohm
 *   resistor
 * - Connect the other side of the 330 ohm resistor to the D4 pin
 * - Connect the one side of the 5k ohm resistor to the ground
 * - Connect the other side of the 5k ohm resistor to the C5 pin
 * - Connect the one side of the switch to the C5 pin
 * - Connect the other side of the switch to the 3V3
 */

#include <gpio.h>

// Define the port and pins where the LED and the switch are connected. This is
// optional. The values can be passed directly to the macro functions
#define LED_PORT D
#define LED_PIN 4
#define SWITCH_PORT C
#define SWITCH_PIN 5

int main(void) {
    
    // Set the pin where the LED is connected as a push-pull output
    GPIO_SET_AS_OUTPUT(LED_PORT, LED_PIN);
    GPIO_SET_AS_PUSH_PULL(LED_PORT, LED_PIN);
    
    // Set the pin where the switch is connected as input
    GPIO_SET_AS_INPUT(SWITCH_PORT, SWITCH_PIN);

    // Start an infinite loop
    while(1) {
        
        // Check the state of the switch
        if (GPIO_READ_INPUT(SWITCH_PORT, SWITCH_PIN)) {
            // If it is high we turn on the LED
            GPIO_WRITE_HIGH(LED_PORT, LED_PIN);
        } else {
            // If it is low we turn off the LED
            GPIO_WRITE_LOW(LED_PORT, LED_PIN);
        }
    }
}
