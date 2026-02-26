// This is the main program going on the Guppy microcontrollers
//
// This code listens to id 0x101 for float values, echoes the can frame on id 0x102, and outputs analog output on pin 20
extern "C" {
#include "pico/stdlib.h"
#include "guppy_lib.h"
#include "modules/motor_controller.h"
}
#include "modules/barometer_sensor.h"
#include "led.hpp"
#include <iostream>

#define NUM_LEDS 144

int main()
{
    stdio_init_all();
    canbus_setup();

    LEDState led_strip = LEDState(20);
    while (true)
    {
        for (int i = 0; i < 10; ++i)
        {
            led_strip.tick();
            sleep_ms(250);
        }
        led_strip.state = (State) ((led_strip.state + 1) % 7);
    }

    switch(BOARD_TYPE)
    {
        case 1: // TODO: should be enum?
            run_motor_controller();
            break;
        case 2:
            run_barometer_sensor();
            break;
        default: // test
            break;
    }
}