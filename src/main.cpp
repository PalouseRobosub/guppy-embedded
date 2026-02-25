// This is the main program going on the Guppy microcontrollers
//
// This code listens to id 0x101 for float values, echoes the can frame on id 0x102, and outputs analog output on pin 20
extern "C" {
#include "pico/stdlib.h"
#include "guppy_lib.h"
#include "modules/motor_controller.h"
#include "pico/rand.h"
}
#include "modules/barometer_sensor.h"
#include "led.h"
#include <iostream>

#define NUM_LEDS 144

int main(void)
{
    stdio_init_all();
    canbus_setup();

    const int module = 1; // identifier for the board

    run_led_control();

    switch(module)
    {
        case 0: // TODO: should be enum?
            run_motor_controller();
            break;
        case 1:
            run_barometer_sensor();
            break;
    }
}