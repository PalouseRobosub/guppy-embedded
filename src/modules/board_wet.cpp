#include "board_wet.h"
#include "barometer.h"
#include "led.hpp"

extern "C" {
#include "pico/stdlib.h"
#include "can2040.h"
#include "guppy_lib.h"
}
#include <iostream>

#define PICO_I2C_INSTANCE   i2c0
#define PICO_I2C_SDA_PIN    16
#define PICO_I2C_SCL_PIN    17

#define SWITCH_PIN_ONE      18
#define SWITCH_PIN_TWO      19 

#define LEDS_PIN            20

void board_wet_loop()
{
    Adafruit_NeoPixel ledStrip(10, LEDS_PIN, NEO_GRB + NEO_KHZ800);
    ledStrip.begin();

    

    // #if !defined(i2c_default) || !defined(PICO_DEFAULT_I2C_SDA_PIN) || !defined(PICO_DEFAULT_I2C_SCL_PIN)
    // #warning You need a board with i2c pins! Cant run the barometer module
    // return;
    // #endif

    std::cout << "Initializing barometer..." << std::endl;

    MS5837 sensor;

    // From pico_examples
    i2c_init(PICO_I2C_INSTANCE, 400 * 1000);
    gpio_set_function(PICO_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(PICO_I2C_SDA_PIN);
    gpio_pull_up(PICO_I2C_SCL_PIN); // TODO: do I need these? there is external pull up!

    // init switches
    gpio_init(SWITCH_PIN_ONE);
    gpio_init(SWITCH_PIN_TWO);
    gpio_set_dir(SWITCH_PIN_ONE, GPIO_IN);
    gpio_set_dir(SWITCH_PIN_TWO, GPIO_IN);
    gpio_pull_down(SWITCH_PIN_ONE);
    gpio_pull_down(SWITCH_PIN_TWO);

    // make available for picotool
    //bi_decl(bi_2pins_with_func(PICO_DEFAULT_I2C_SDA_PIN, PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C));

    // TODO: remember to add heartbeat code
    while (!sensor.init(PICO_I2C_INSTANCE))
    {
        std::cout << "Initializing barometer failed!" << std::endl;
        std::cout << "Are SDA/SCL connected correctly?" << std::endl;
        std::cout << "Blue Robotics Bar30: White=SDA, Green=SCL";
        std::cout << std::endl << std::endl << std::endl;

        sleep_ms(1000);
    }

    sensor.setFluidDensity(997); // kg/m^3 (freshwater) TODO: change to actual density

    std::cout << "sensor initialized!" << std::endl;

    while (1)
    {   
        sensor.read(); // takes up to 40ms to run, this should have delay between uses

        float depth = sensor.depth();
        float temp = sensor.temperature();

        // printf("\nPressure: %f\n", sensor.pressure());
        // printf("Altitude: %f\n", sensor.altitude());
        // printf("Depth: %f\n", depth);
        // printf("Temperature: %f\n", temp);

        canbus_transmit_float(0x026, depth);
        canbus_transmit_float(0x025, temp);

        for (int i = 0; i < 10; i++)
        {
            do_heartbeat(0x20);

            canbus_transmit_int(0x022, gpio_get(SWITCH_PIN_ONE));
            canbus_transmit_int(0x023, gpio_get(SWITCH_PIN_TWO));

            sleep_ms(100); // TODO: use current time instead?
        }
    }
}