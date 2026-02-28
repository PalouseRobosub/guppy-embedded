#ifndef GUPPY_EMBEDDED_LED_H
#define GUPPY_EMBEDDED_LED_H

extern "C" {
    #include "pico/stdlib.h"
}
#include "Adafruit_NeoPixel.hpp"

#define NUM_LEDS 144
#define BRIGHTNESS 50 // brightness of pixels out of 255

enum State {
    STARTUP = 0,
    HOLDING = 1,
    NAV = 2,
    TASK = 3,
    TELEOP = 4,
    DISABLED = 5,
    FAULT = 6
};


class LEDState
{
public:
    State state;
    /// `pin` is the gpio pin the LED line data is connected to
    LEDState(int pin);
    /// Updates the LED strip
    void tick();
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