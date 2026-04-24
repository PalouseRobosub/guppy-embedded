extern "C" {
    #include "can2040.h"
    #include "guppy_lib.h"
    #include "pico/stdlib.h"
    #include <stdio.h>
}
#include "board_motor.h"
#include "led.hpp"

#define NUM_PINS 8
static const uint8_t pwm_pins[NUM_PINS] = { 16, 17, 18, 20, 19, 25, 26, 27 }; // motors 3 & 4 swapped in hardware
static RateLimit last_updates[NUM_PINS]{};
static const uint8_t led_pin = 28;
static const uint16_t motor_board_id = 0x410;
static const uint16_t torpedo_servo_id = 0x419;
static const uint16_t grabber_servo_id = 0x41A;

#define MOTOR_MULT 0.5
#define ALLOW_STALE_MOTORS false // if this is true it won't set stale motors to 0

void board_motor_loop()
{
    for (int i = 0; i < NUM_PINS; i++) {
        add_pwm_pin(pwm_pins[i]);
        last_updates[i] = new_rate_limit(500);
    }

    size_t led_groups[3] = {42, 42, 42};
    LEDController<3> led_strip(led_pin, led_groups);

    while (true)
    {
        do_heartbeat(0x010);
        led_strip.tick();

        const absolute_time_t cur_time = get_absolute_time();

        struct can2040_msg msg = { 0 };
        if (canbus_read(&msg)) // returns true if has message. Sets the &msg to the message
        {
            led_strip.update(msg);
            if (msg.id >= (motor_board_id+1) && msg.id <= (motor_board_id+NUM_PINS)) {
                float value = can_read_float(msg) * MOTOR_MULT;
                if (value > 1.0) value = 1.0;
                if (value < -1.0) value = -1.0;
                int micro_seconds = throttle_to_pwm_us(value);

                // NDEBUG should be added by CMAKE on release builds
                printf("received motor power for id %x\n", msg.id);
                printf("pwm level (should be from 1100 to 1900): %d\n", micro_seconds);
                printf("recieved float: %f\n\n", value);

                const int index = msg.id - motor_board_id - 1;
                pwm_write(pwm_pins[index], micro_seconds);
                last_updates[index].time = cur_time;
            }

            if (msg.id == grabber_servo_id) {
                // TODO: send pwm to grabber servo
            }
            if (msg.id == torpedo_servo_id) {
                // TODO: send pwm to torpedo launch servo
            }
        }

        #if ALLOW_STALE_MOTORS == false
        for (int i = 0; i < NUM_PINS; i++) // set stale motors to 0
        {
            if (check_rate(&last_updates[i]))
            {
                printf("stale motor %d\n", i);
                pwm_write(pwm_pins[i], throttle_to_pwm_us(0.0));
            }
        }
        #endif
    }
}
