// This is the main program going on the Guppy microcontrollers
//
// This code listens to id 0x101 for float values, echoes the can frame on id 0x102, and outputs analog output on pin 20

#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include <stdio.h>
#include <string.h>

#include "../src/guppylib.h"

#define PWM_PIN 20

int main(void)
{
    stdio_init_all();
    canbus_setup();

    add_pwm_pin(PWM_PIN);

    while (true)
    {
        struct can2040_msg msg = { 0 };
        if (canbus_read(&msg)) // returns true if has message. Sets the &msg to the message
        {
            printf("Recieved frame\n");
            if (msg.id != 0x101) continue;

            float value = can_read_float(msg);
            int us = throttle_to_pwm_us(value);
            printf("pwm level (should be from 1100 to 1900): %d\n", us);

            pwm_write(PWM_PIN, us);

            canbus_transmit_float(0x102, value); // echo the value
        }
    }
}
