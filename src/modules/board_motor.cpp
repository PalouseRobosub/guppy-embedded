extern "C" {
    #include "can2040.h"
    #include "guppy_lib.h"
    #include "pico/stdlib.h"
    #include <stdio.h>
}
#include "board_motor.h"
#include "led.hpp"

// TODO: untested pin numbers
static const uint8_t pwm_pins[8] = { 16, 17, 18, 19, 20, 25, 26, 27 };
static const uint8_t led_pin = 28;
static const uint16_t motor_board_id = 0x410;

#define MOTOR_MULT 0.1

void board_motor_loop()
{
    for (int i = 0; i < sizeof(pwm_pins)/sizeof(uint8_t); i++) {
        add_pwm_pin(pwm_pins[i]);
    }

    LEDState led_strip(led_pin);

    while (true)
    {
        do_heartbeat(0x010);
        led_strip.tick();

        struct can2040_msg msg = { 0 };
        if (canbus_read(&msg)) // returns true if has message. Sets the &msg to the message
        {
            led_strip.update(msg);
            #ifndef NDEBUG
                printf("Recieved frame\n");
            #endif
            if (msg.id >= (motor_board_id+1) && msg.id <= (motor_board_id+8)) {
                float value = can_read_float(msg) * MOTOR_MULT;
                if (value > MOTOR_MULT) value = MOTOR_MULT;
                if (value < -MOTOR_MULT) value = -MOTOR_MULT;
                int micro_seconds = throttle_to_pwm_us(value);

                // NDEBUG should be added by CMAKE on release builds
                #ifndef NDEBUG
                    printf("received motor power for id %x\n", msg.id);
                    printf("pwm level (should be from 1100 to 1900): %d\n", micro_seconds);
                    printf("recieved float: %f\n", value);
                #endif

                pwm_write(pwm_pins[msg.id - motor_board_id], micro_seconds);

                // tech debt: 0x102 is a temp test id
                #ifndef NDEBUG
                    canbus_transmit_float(0x102, value); // echo the value
                #endif
            }
        }
    }
}
