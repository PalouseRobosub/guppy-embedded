#ifndef GUPPY_EMBEDDED_LED_H
#define GUPPY_EMBEDDED_LED_H

extern "C" {
    #include "pico/stdlib.h"
}
#include "led.hpp"
#include "Adafruit_NeoPixel.hpp"

#define BRIGHTNESS 50 // brightness of pixels out of 255


class LEDController
{
public:
    enum class State {
        STARTUP = 0,
        HOLDING = 1,
        NAV = 2,
        TASK = 3,
        TELEOP = 4,
        DISABLED = 5,
        FAULT = 6
    };

    State state;
    int led_count;
    /// `pin` is the gpio pin the LED line data is connected to
    LEDController(int pin, int led_count=78); // 78 from 26*3 for guppy
    /// Updates the LED strip
    void tick();
    /// Updates LEDController based on the provided CAN message
    /// (does care about CAN ID and will ignore messages from most IDs)
    void update(const struct can2040_msg& msg);
private:
    int tick_count;
    Adafruit_NeoPixel led_strip;
    absolute_time_t time_last_updated;
    int update_rate_ms;
    void two_color(uint32_t color1, uint32_t color2);
    void startup();
    void holding();
    void nav();
    void task();
    void teleop();
    void disabled();
    void fault();
};

#endif //GUPPY_EMBEDDED_LED_H