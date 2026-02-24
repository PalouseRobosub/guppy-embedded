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
#include <iostream>
#include "Adafruit_NeoPixel.hpp"


int main(void)
{

    Adafruit_NeoPixel ledStrip(100, 16, NEO_GRB + NEO_KHZ800);
    ledStrip.begin();
    

    stdio_init_all();
    canbus_setup();

    int brightnesses[100] = { 0 };
    int inc = 0;

    while(1)
    {
        if (inc++ > 10)
        {
            inc = 0;
            brightnesses[get_rand_32() % 100] = 100;
        }

        for (int i = 0; i < 100; i++)
        {
            ledStrip.setPixelColor(i, ledStrip.Color(brightnesses[i], 0, 0));
            if (brightnesses[i] > 0) brightnesses[i]--;
        }
        ledStrip.show();

        sleep_ms(10);
    }

    sleep_ms(10000);
    std::cout << "Hello" << std::endl;

    const int module = 1; // identifier for the board

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