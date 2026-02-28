// This is the main program going on the Guppy microcontrollers
//
// This code listens to id 0x101 for float values, echoes the can frame on id 0x102, and outputs analog output on pin 20
extern "C" {
#include "pico/stdlib.h"
#include "guppy_lib.h"
}
#include "modules/board_motor.h"
#include "modules/board_wet.h"
#include "led.hpp"
#include <iostream>

#define NUM_LEDS 144

int main()
{
    stdio_init_all();
    canbus_setup();

    switch(BOARD_TYPE)
    {
        case 1: // TODO: should be enum?
            board_motor_loop();
            break;
        case 2:
            board_wet_loop();
            break;
        case -1:
        default: // test
            LEDState led_strip = LEDState(20);
            while (true)
            {
                for (int i = 0; i < 2500; ++i)
                {
                    led_strip.tick();
                    sleep_ms(1);
                }
                led_strip.state = (State) ((led_strip.state + 1) % 7);
            }
            break;
    }
}