// This is the main program going on the Guppy microcontrollers
//
// This code listens to id 0x101 for float values, echoes the can frame on id 0x102, and outputs analog output on pin 20

#include "pico/stdlib.h"

#include "guppy_lib.h"
#include "modules/motor_controller.h"

int main(void)
{
    stdio_init_all();
    canbus_setup();

    const int module = 0; // identifier for the board

    switch(module)
    {
        case 0: // TODO: should be enum?
            run_motor_controller();
            break;
    }
}