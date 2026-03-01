extern "C" {
    #include "can2040.h"
    #include "guppy_lib.h"
}
#include "Adafruit_NeoPixel.hpp"
#include "led.hpp"

/// Number of LEDs in a row of a continuous color
#define LED_GROUP_SIZE 13

#define LED_WHITE Adafruit_NeoPixel::Color(BRIGHTNESS, BRIGHTNESS, BRIGHTNESS)
#define LED_RED Adafruit_NeoPixel::Color(BRIGHTNESS, 0, 0)
#define LED_GREEN Adafruit_NeoPixel::Color(0, BRIGHTNESS, 0)
#define LED_BLUE Adafruit_NeoPixel::Color(0, 0, BRIGHTNESS)
#define LED_YELLOW Adafruit_NeoPixel::Color(BRIGHTNESS, BRIGHTNESS, 0)
// #define ORANGE Adafruit_NeoPixel::Color(BRIGHTNESS, BRIGHTNESS/2, 0)
// #define PURPLE Adafruit_NeoPixel::Color(BRIGHTNESS/2, 0, BRIGHTNESS)
#define LED_OFF Adafruit_NeoPixel::Color(0, 0, 0)

LEDController::LEDController(int pin, int led_count) : led_strip(led_count, pin, NEO_GRB + NEO_KHZ800)
{
    tick_count = 0;
    time_last_updated = nil_time; // TODO: hopefully this works
    update_rate_ms = 250;
    led_strip.begin();
    state = State::STARTUP;
    this->led_count = led_count;
}

void LEDController::two_color(uint32_t color1, uint32_t color2)
{
    for (int i = 0; i <= led_count - LED_GROUP_SIZE*2; i += LED_GROUP_SIZE*2)
    {
        for (int j = 0; j < LED_GROUP_SIZE; ++j) {
            led_strip.setPixelColor(i+j, color1);
        }
        for (int j = LED_GROUP_SIZE; j < LED_GROUP_SIZE*2; ++j) {
            led_strip.setPixelColor(i+j, color2);
        }
    }
}

void LEDController::update(const can2040_msg& msg)
{
    if (msg.id == 0x201)
    {
        // TODO: I have no clue how big of an integer the state is sent as,
        //       check that if this doesn't work
        state = static_cast<State>(can_read_int(msg));
    }
}

void LEDController::tick()
{
    auto current_time = get_absolute_time();
    if (absolute_time_diff_us(time_last_updated, current_time)/1000 < update_rate_ms)
    {
        return;
    }
    time_last_updated = current_time;

    switch (state)
    {
        case State::STARTUP:  this->startup();  break;
        case State::HOLDING:  this->holding();  break;
        case State::NAV:      this->nav();      break;
        case State::TASK:     this->task();     break;
        case State::TELEOP:   this->teleop();   break;
        case State::DISABLED: this->disabled(); break;
        case State::FAULT:    this->fault();    break;
        default:       this->fault();    break;
    }
    led_strip.show();
    ++tick_count;
}

void LEDController::startup()
{
    this->two_color(LED_GREEN, LED_WHITE);
}
void LEDController::holding()
{
    this->two_color(LED_BLUE, LED_OFF);
}
void LEDController::nav()
{
    this->two_color(LED_BLUE, LED_RED);
}
void LEDController::task()
{
    this->two_color(LED_RED, LED_RED);
}
void LEDController::teleop()
{
    for (int i = 0; i <= led_count - 1; i += 1)
    {
        led_strip.setPixelColor(i, LED_GREEN);
    }
}
void LEDController::disabled()
{
    for (int i = 0; i <= led_count - 6; i += 6)
    {
        led_strip.setPixelColor(i, LED_YELLOW);
        led_strip.setPixelColor(i+1, LED_OFF);
        led_strip.setPixelColor(i+2, LED_OFF);
        led_strip.setPixelColor(i+3, LED_OFF);
        led_strip.setPixelColor(i+4, LED_OFF);
        led_strip.setPixelColor(i+5, LED_OFF);
    }
}
void LEDController::fault()
{
    if (this->tick_count % 2)
        two_color(LED_RED, LED_WHITE);
    else
        two_color(LED_WHITE, LED_RED);
}

