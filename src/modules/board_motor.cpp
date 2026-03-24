extern "C" {
    #include "can2040.h"
    #include "guppy_lib.h"
    #include "pico/stdlib.h"
    #include <stdio.h>
}
#include "board_motor.h"
#include "led.hpp"

// TODO: untested pin numbers
#define NUM_PINS 8
static const uint8_t pwm_pins[NUM_PINS] = { 16, 17, 18, 19, 20, 25, 26, 27 };
static const uint8_t led_pin = 28;
static const uint16_t motor_board_id = 0x410;

static float max_motor_update_step = 0.1;
static int min_motor_update_delay_ms = 10;
static absolute_time_t last_motor_state_update;
static int last_motor_update_time[NUM_PINS]{};
static float desired_motor_state[NUM_PINS]{};
static float sent_motor_state[NUM_PINS]{};

#define MOTOR_MULT 0.5
#define ALLOW_STALE_MOTORS true // if this is true it won't set stale motors to 0

void update_pwm()
{
    const absolute_time_t cur_time = get_absolute_time();
    if (absolute_time_diff_us(last_motor_state_update, cur_time)/1000 > min_motor_update_delay_ms) {
        last_motor_state_update = cur_time;
        for (int i = 0; i < NUM_PINS; i++) {
            if (sent_motor_state[i] == desired_motor_state[i]) continue;

            if (sent_motor_state[i] < desired_motor_state[i]) {
                sent_motor_state[i] = sent_motor_state[i] + max_motor_update_step;
                if (sent_motor_state[i] > desired_motor_state[i]) {
                    sent_motor_state[i] = desired_motor_state[i];
                }
            }
            else if (sent_motor_state[i] > desired_motor_state[i]) {
                sent_motor_state[i] = sent_motor_state[i] - max_motor_update_step;
                if (sent_motor_state[i] < desired_motor_state[i]) {
                    sent_motor_state[i] = desired_motor_state[i];
                }
            }

            int micro_seconds = throttle_to_pwm_us(sent_motor_state[i]);
            pwm_write(pwm_pins[i], micro_seconds);
        }
    }
}

void board_motor_loop()
{
    for (int i = 0; i < NUM_PINS; i++) {
        add_pwm_pin(pwm_pins[i]);
        last_motor_update_time[i] = 0;
        desired_motor_state[i] = 0.0;
        sent_motor_state[i] = 0.0;
    }

    LEDController led_strip(led_pin);

    while (true)
    {
        do_heartbeat(0x010);
        led_strip.tick();

        const int cur_time_ms = to_ms_since_boot(get_absolute_time());

        struct can2040_msg msg = { 0 };
        if (canbus_read(&msg)) // returns true if has message. Sets the &msg to the message
        {
            led_strip.update(msg);
            if (msg.id >= (motor_board_id+1) && msg.id <= (motor_board_id+NUM_PINS)) {
                float value = can_read_float(msg) * MOTOR_MULT;
                printf("float before clamp: %f\n", value);
                if (value > MOTOR_MULT) value = MOTOR_MULT;
                if (value < -MOTOR_MULT) value = -MOTOR_MULT;

                // NDEBUG should be added by CMAKE on release builds
                printf("received motor power for id %x\n", msg.id);
                printf("recieved float: %f\n\n", value);

                const int index = msg.id - motor_board_id - 1;
                desired_motor_state[index] = value;
                last_motor_update_time[index] = cur_time_ms;
            }
        }

        update_pwm();

        #ifndef ALLOW_STALE_MOTORS
        for (int i = 0; i < NUM_PINS; i++) // set stale motors to 0
        {
            if (cur_time_ms - last_motor_update_time[i] > 500)
            {
                // pwm_write(pwm_pins[i], throttle_to_pwm_us(0)); // write 0
                desired_motor_state[i] = 0.0;
                last_motor_update_time[i] = cur_time_ms;
            }
        }
        #endif
    }
}
