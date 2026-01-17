#include "motor_controller.h"
#include "can2040.h"

#define PWM_PIN 20

void run_motor_controller()
{

    add_pwm_pin(PWM_PIN);

    while (1)
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