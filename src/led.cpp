#include "led.hpp"

#define WHITE Adafruit_NeoPixel::Color(BRIGHTNESS, BRIGHTNESS, BRIGHTNESS)
#define RED Adafruit_NeoPixel::Color(BRIGHTNESS, 0, 0)
#define GREEN Adafruit_NeoPixel::Color(0, BRIGHTNESS, 0)
#define BLUE Adafruit_NeoPixel::Color(0, 0, BRIGHTNESS)
#define YELLOW Adafruit_NeoPixel::Color(BRIGHTNESS, BRIGHTNESS, 0)
#define ORANGE Adafruit_NeoPixel::Color(BRIGHTNESS, BRIGHTNESS/2, 0)
#define PURPLE Adafruit_NeoPixel::Color(BRIGHTNESS/2, 0, BRIGHTNESS)
#define OFF Adafruit_NeoPixel::Color(0, 0, 0)

LEDState::LEDState(int pin)
{
    tick_count = 0;
    led_strip = Adafruit_NeoPixel(NUM_LEDS, pin, NEO_GRB + NEO_KHZ800);
    led_strip.begin();
    state = STARTUP;
}

void LEDState::two_color(uint32_t color1, uint32_t color2)
{
    for (int i = 0; i <= NUM_LEDS - 12; i += 12)
    {
        for (int j = 0; j < 6; ++j) {
            led_strip.setPixelColor(i+j, color1);
        }
        for (int j = 6; j < 12; ++j) {
            led_strip.setPixelColor(i+j, color2);
        }
    }
}

void LEDState::tick()
{
    switch (state)
    {
        case STARTUP:
            this->startup();
            break;
        case HOLDING:
            this->holding();
            break;
        case NAV:
            this->nav();
            break;
        case TASK:
            this->task();
            break;
        case TELEOP:
            this->teleop();
            break;
        case DISABLED:
            this->disabled();
            break;
        case FAULT:
            this->fault();
            break;
        default:
            this->fault();
            break;
    }
    led_strip.show();
    ++tick_count;
}

void LEDState::startup()
{
    this->two_color(GREEN, WHITE);
}
void LEDState::holding()
{
    this->two_color(BLUE, OFF);
}
void LEDState::nav()
{
    this->two_color(BLUE, ORANGE);
}
void LEDState::task()
{
    this->two_color(BLUE, PURPLE);
}
void LEDState::teleop()
{
    for (int i = 0; i <= NUM_LEDS - 1; i += 1)
    {
        led_strip.setPixelColor(i, GREEN);
    }
}
void LEDState::disabled()
{
    for (int i = 0; i <= NUM_LEDS - 6; i += 6)
    {
        led_strip.setPixelColor(i, YELLOW);
        led_strip.setPixelColor(i+1, OFF);
        led_strip.setPixelColor(i+2, OFF);
        led_strip.setPixelColor(i+3, OFF);
        led_strip.setPixelColor(i+4, OFF);
        led_strip.setPixelColor(i+5, OFF);
    }
}
void LEDState::fault()
{
    if (this->tick_count % 2)
        two_color(RED, WHITE);
    else
        two_color(WHITE, RED);
    /*
    int chunk_size = 6;
    for (int i = 0; i < NUM_LEDS/chunk_size; ++i)
    {
        for (int j = 0; j < chunk_size; ++j)
        {
            int index = (chunk_size*i + j) % NUM_LEDS;
            if (i % 2 == tick_count % 2)
                led_strip.setPixelColor(index, WHITE);
            if (i % 2 == (tick_count + 1) % 2)
                led_strip.setPixelColor(index, RED);
        }
    }
    */
}

