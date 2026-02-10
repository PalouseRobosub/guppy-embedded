#include "barometer_sensor.h"
#include "../barometer.h"

extern "C" {
#include "pico/stdlib.h"
#include "can2040.h"
#include "../guppy_lib.h"
#include <cstdio>
}

#define PWM_PIN 20

void run_barometer_sensor()
{
    MS5837 sensor;
    i2c_inst_t *REMOVE_ME_PLEASE; // add actual i2c inst stuff

    // TODO: remember to add heartbeat code
    while (!sensor.init(REMOVE_ME_PLEASE))
    {
        printf("Initializing barometer failed!\n");
        printf("Are SDA/SCL connected correctly?");
        printf("Blue Robotics Bar30: White=SDA, Green=SCL");
        printf("\n\n\n");
    }

    sensor.setFluidDensity(997); // kg/m^3 (freshwater) TODO: change to actual density

    while (1)
    {
        //struct can2040_msg msg = { 0 };
        
        sensor.read(); // takes up to 40ms to run, this should have delay between uses

        printf("Pressure: %f\n", sensor.pressure());

        sleep_ms(1000);
    }
}