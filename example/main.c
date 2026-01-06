// This is the main program going on the Guppy microcontrollers
//
// I have no idea if this code works

#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include <stdio.h>
#include <string.h>

#include "../src/guppylib.h"

int main(void)
{
    stdio_init_all();
    canbus_setup();

    add_pwm_pin(pin_num); // set default duty cycle 1500 us

    

    can2040_msg msg = { 0 };
    if (canbus_read(*msg)) // returns true if has message. Sets the *msg to the message
    {
        // TODO: id checking


        float value = can_read_float(msg);
        int us = throttle_to_pwm_us(value);
        pwm_write(pin_num, us);

        canbus_transmit_float(101, value); // echo the value
    }
}
